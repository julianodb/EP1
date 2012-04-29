/* 
  PROJETO USPDesigner
  MODULO: HIE (Estrutura Hierarquica)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: hieaxsrm.cpp
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
#include "hiegroup.h"

#ifdef __Extended_Version
void MSD_execNameRemoveAxis(void)
{
   char aname[30];

   while (1 != sscanf(restbuf, "%s", aname))
   {
      printf("RemAxis: axis\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highNameRemoveAxis(aname);
}

#endif

int MSD_highNameRemoveAxis(char *name)
{
   int an;

   if ((an = MSD_getAxisIdFromName(name)) == -1)
   {
      fprintf(stderr, MEN_NaoEncontrouAxisNome, NAM_RemoveAxis, name);
      return(ERROR);
   }
   return(MSD_highRemoveAxis(an));
}

#ifdef __Extended_Version
void MSD_execRemoveAxis(void)
{
   int an;

   while (1 != sscanf(restbuf, "%d", &an))
   {
      printf("RemAxis: axis\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highRemoveAxis(an);
}

#endif

int MSD_highRemoveAxis(int an)
{
   APTYPE aptr;

   if ((aptr = MSD_getAxis(an)) == ANIL)
   {
      fprintf(stderr, MEN_NaoEncontrouAxisId, NAM_RemoveAxis, an);
      return(ERROR);
   }
   return(MSD_lowRemoveAxis(aptr));
}

int MSD_lowRemoveAxis(APTYPE aptr)
{
   real fd1, fd2, fd3, fd4;
   Id   an;

   MSD_lowAxisName(aptr, "");
   if (Gdoinversion)
   {
      if (AxsMode(aptr) == AxisOfTranslation)
      {
         an  = AxsAxisNo(aptr);
         fd1 = AxsVector(aptr)[0];
         fd2 = AxsVector(aptr)[1];
         fd3 = AxsVector(aptr)[2];
         fd4 = AxsVector(aptr)[3];
         MSD_lowAddEulerOp(SNIL, CRTAXSTRS, an, 0, 0, 0, 0, 0, 0, 0,
                           fd1, fd2, fd3, fd4);
      }
      if (AxsMode(aptr) == AxisOfRotation)
      {
         an  = AxsAxisNo(aptr);
         fd1 = AxsVector(aptr)[0];
         fd2 = AxsVector(aptr)[1];
         fd3 = AxsVector(aptr)[2];
         fd4 = AxsVector(aptr)[3];
         MSD_lowAddEulerOp(SNIL, CRTAXSROT, an, 0, 0, 0, 0, 0, 0, 0,
                           fd1, fd2, fd3, fd4);
         fd1 = AxsVector(aptr)[0];
         fd2 = AxsVector(aptr)[1];
         fd3 = AxsVector(aptr)[2];
         fd4 = AxsVector(aptr)[3];
         MSD_lowAddEulerOp(SNIL, CRTAXSROT, -1, 0, 0, 0, 0, 0, 0, 0,
                           fd1, fd2, fd3, fd4);
      }
   }

   --axsused;
   MSD_lowDelElement(AXIS, (NPTYPE)aptr, NNIL);
   return(SUCCESS);
}

int MSD_execStructRemoveAxis(EulerOp *op)
{
   return(MSD_highRemoveAxis(op->ip1));
}

void MSD_printStructRemoveAxis(FILE *f, EulerOp *op)
{
   Id sid, ip1;

   sid = op->solidno;
   ip1 = op->ip1;
   fprintf(f, "axsdel %d %d\n", sid, ip1);
}

int MSD_readStructRemoveAxis(FILE *f, EulerOp *op)
{
   int solidno, ip1;

   if (fscanf(f, "%d %d", &solidno, &ip1) != 2)
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
   op->fp1     = 0.0;
   op->fp2     = 0.0;
   op->fp3     = 0.0;
   op->fp4     = 0.0;
   return(SUCCESS);
}

int MSD_modifyStructRemoveAxis(EulerOp *op)
{
   int axisno;

   axisno  = MSD_lowCheckTranslatorTable(op->ip1, AXIS);
   op->ip1 = axisno;
   return(SUCCESS);
}
