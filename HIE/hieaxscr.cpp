/* 
  PROJETO USPDesigner
  MODULO: HIE (Estrutura Hierarquica)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: hieaxscr.cpp
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
#include <string.h>
#include "mensagem.h"
#include "memvirtu.h"
#include "lowparam.h"
#include "lowmacro.h"
#include "lowsolid.h"
#include "vectorop.h"
#include "eulerops.h"
#include "mancommd.h"
#include "hiegroup.h"

#ifdef __Extended_Version
void MSD_execNameCreateAxisRotation(void)
{
   char  name[30];
   float x1, y1, z1, x2, y2, z2;

   while (7 != sscanf(restbuf, "%s %f %f %f %f %f %f", name, &x1, &y1, &z1,
                      &x2, &y2, &z2))
   {
      printf("crtaxsrot: nome x1 y1 z1 x2 y2 z2\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highNameCreateAxisRotation(name, x1, y1, z1, x2, y2, z2);
}

#endif

int MSD_highNameCreateAxisRotation(char *name, real x1, real y1, real z1,
                                   real x2, real y2, real z2)
{
   APTYPE aptr;
   int    an;

   if (MSD_getAxisIdFromName(name) != -1)
   {
      fprintf(stderr, MEN_AxisJaExiste, NAM_CreateAxisOfRotation, name);
      return(ERROR);
   }
   if ((an = MSD_highCreateAxisRotation(x1, y1, z1, x2, y2, z2)) != -1)
   {
      aptr = MSD_getAxis(an);
      strcpy(AxsName(aptr), name);
      return(SUCCESS);
   }
   return(ERROR);
}

#ifdef __Extended_Version
void MSD_execCreateAxisRotation(void)
{
   float x1, y1, z1, x2, y2, z2;

   while (6 != sscanf(restbuf, "%f %f %f %f %f %f", &x1, &y1, &z1, &x2, &y2, &z2))
   {
      printf("crtaxsrot: an x y z\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   printf("Id = %d\n", MSD_highCreateAxisRotation(x1, y1, z1, x2, y2, z2));
}

#endif

int MSD_highCreateAxisRotation(real x1, real y1, real z1,
                               real x2, real y2, real z2)
{
   vector vec1, vec2;

   makevec(vec1, x1, y1, z1, 1.0);
   makevec(vec2, x2, y2, z2, 1.0);
   return(MSD_lowCreateAxisRotation(MSD_getMaxAxisId(), vec1, vec2));
}

int MSD_middleCreateAxisRotation(int an, real x1, real y1, real z1,
                                 real x2, real y2, real z2)
{
   vector vec1, vec2;

   makevec(vec1, x1, y1, z1, 1.0);
   makevec(vec2, x2, y2, z2, 1.0);
   return(MSD_lowCreateAxisRotation(an, vec1, vec2));
}

int MSD_lowCreateAxisRotation(Id an, vector pos, vector vec)
{
   APTYPE aptr;

   if ((aptr = (APTYPE)MSD_lowNewElement(AXIS, NNIL)) == ANIL)
   {
      fprintf(stderr, MEN_NaoPossuiMemoria, NAM_CreateAxisOfRotation);
      return(ERROR);
   }
   AxsAxisNo(aptr) = an;
   AxsMode(aptr)   = AxisOfRotation;
   veccopy(AxsCenter(aptr), pos);
   veccopy(AxsVector(aptr), vec);
   if (Gdoinversion)
   {
      MSD_lowAddEulerOp(SNIL, REMAXS, an, 0, 0, 0, 0, 0, 0, 0,
                        0.0, 0.0, 0.0, 0.0);
   }
   return(an);
}

int MSD_execStructCrtAxsRotation(EulerOp *op)
{
   EulerOp *op1;
   Id      an;
   real    fd1, fd2, fd3, fd4, fd5, fd6;

   an  = op->ip1;
   fd1 = op->fp1;
   fd2 = op->fp2;
   fd3 = op->fp3;

#ifndef __VIRTUAL_MEM
   op1 = op->opnext;
#else
   op1 = EULEROPS(op->pp - 1);
#endif

   fd4 = op1->fp1;
   fd5 = op1->fp2;
   fd6 = op1->fp3;
   return(MSD_middleCreateAxisRotation(an, fd1, fd2, fd3, fd4, fd5, fd6));
}

void MSD_printStructCrtAxsRotation(FILE *f, EulerOp *op)
{
   Id   sid, ip1;
   real fd1, fd2, fd3, fd4;

   sid = op->solidno;
   ip1 = op->ip1;
   fd1 = op->fp1;
   fd2 = op->fp2;
   fd3 = op->fp3;
   fd4 = op->fp4;
   fprintf(f, "crtaxsrot %d %d %f %f %f %f\n", sid, ip1, fd1, fd2, fd3, fd4);
}

int MSD_readStructCrtAxsRotation(FILE *f, EulerOp *op)
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

int MSD_modifyStructCrtAxsRotation(EulerOp *op)
{
   int axisno;

   axisno  = MSD_lowCheckTranslatorTable(op->ip1, AXIS);
   op->ip1 = axisno;
   return(SUCCESS);
}
