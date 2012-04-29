/* 
  PROJETO USPDesigner
  MODULO: HIE (Estrutura Hierarquica)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: hieaxsmv.cpp
   Coded by Marcos Tsuzuki

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote 
        products derived from this software without specific prior written 
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


   Any feedback is very welcome.
   email: mtsuzuki at usp.br (remove space)
*/
#include <stdio.h>
#include "mensagem.h"
#include "memvirtu.h"
#include "lowparam.h"
#include "lowmacro.h"
#include "lowsolid.h"
#include "vectorop.h"
#include "eulerops.h"
#include "mancommd.h"
#include "disdispl.h"
#include "hiegroup.h"

#ifdef __Extended_Version
void MSD_execNameMoveAxis(void)
{
   char  aname[30];
   float x, y, z;

   while (4 != sscanf(restbuf, "%s %f %f %f", aname, &x, &y, &z))
   {
      printf("MoveAxis: axis dx dy dz\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highNameMoveAxis(aname, x, y, z);
}

#endif

int MSD_highNameMoveAxis(char *name, real x, real y, real z)
{
   int an;

   if ((an = MSD_getAxisIdFromName(name)) == -1)
   {
      fprintf(stderr, MEN_NaoEncontrouAxisNome,
              NAM_MoveAxis, name);
      return(ERROR);
   }
   return(MSD_highMoveAxis(an, x, y, z));
}

#ifdef __Extended_Version
void MSD_execMoveAxis(void)
{
   float x, y, z;
   int   an;

   while (4 != sscanf(restbuf, "%d %f %f %f", &an, &x, &y, &z))
   {
      printf("MoveAxis: an desl\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highMoveAxis(an, x, y, z);
}

#endif

int MSD_highMoveAxis(int an, real x, real y, real z)
{
   APTYPE aptr;
   vector vec;

   if ((aptr = MSD_getAxis(an)) == ANIL)
   {
      fprintf(stderr, MEN_NaoEncontrouAxisId,
              NAM_MoveAxis, an);
      return(ERROR);
   }
   makevec(vec, x, y, z, 1.0);
   return(MSD_lowMoveAxis(aptr, vec));
}

int MSD_lowMoveAxis(APTYPE aptr, vector vec)
{
   Id an;

   if (AxsMode(aptr) == AxisOfTranslation)
   {
      return(SUCCESS);
   }
   if (Gdoinversion)
   {
      an = AxsAxisNo(aptr);
      MSD_lowAddEulerOp(SNIL, MOVAXIS, an, 0, 0, 0, 0, 0, 0, 0,
                        -vec[0], -vec[1], -vec[2], 1.0);
   }
   vecplus(AxsCenter(aptr), vec, AxsCenter(aptr));
   return(SUCCESS);
}

int MSD_execStructMoveAxis(EulerOp *op)
{
   Id   an;
   real fd1, fd2, fd3;

   an  = op->ip1;
   fd1 = op->fp1;
   fd2 = op->fp2;
   fd3 = op->fp3;
   return(MSD_highMoveAxis(an, fd1, fd2, fd3));
}

void MSD_printStructMoveAxis(FILE *f, EulerOp *op)
{
   Id   sid, ip1;
   real fd1, fd2, fd3, fd4;

   sid = op->solidno;
   ip1 = op->ip1;
   fd1 = op->fp1;
   fd2 = op->fp2;
   fd3 = op->fp3;
   fd4 = op->fp4;
   fprintf(f, "movaxs %d %d %f %f %f %f\n", sid, ip1, fd1, fd2, fd3, fd4);
}

int MSD_readStructMoveAxis(FILE *f, EulerOp *op)
{
   int  solidno, ip1;
   real fd1, fd2, fd3, fd4;

   if (fscanf(f, "%d %d %f %f %f\n", &solidno, &ip1,
              &fd1, &fd2, &fd3, &fd4) != 6)
   {
      return(ERROR);
   }
   op->solidno = solidno;
   op->ip1     = ip1;
   op->ip2     = 0;
   op->ip3     = 0;
   op->ip4     = 0;
   op->ip5     = 0;
   op->ip6     = 0;
   op->ip7     = 0;
   op->ip8     = 0;
   op->fp1     = fd1;
   op->fp2     = fd2;
   op->fp3     = fd3;
   op->fp4     = fd4;
   return(SUCCESS);
}

int MSD_modifyStructMoveAxis(EulerOp *op)
{
   int axisno;

   axisno  = MSD_lowCheckTranslatorTable(op->ip1, AXIS);
   op->ip1 = axisno;
   return(SUCCESS);
}
