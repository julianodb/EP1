/* 
  PROJETO USPDesigner
  MODULO: HIE (Estrutura Hierarquica)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: hieaxscd.cpp
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
void MSD_execNameCreateAxisTranslation(void)
{
   char  name[30];
   float x, y, z;

   while (4 != sscanf(restbuf, "%s %f %f %f", name, &x, &y, &z))
   {
      printf("crtaxstrn: nome x y z\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highNameCreateAxisTranslation(name, x, y, z);
}

#endif

int MSD_highNameCreateAxisTranslation(char *name, real x, real y, real z)
{
   APTYPE aptr;
   int    an;

   if (MSD_getAxisIdFromName(name) != -1)
   {
      fprintf(stderr, MEN_AxisJaExiste,
              NAM_CreateAxisOfTranslation, name);
      return(ERROR);
   }
   if ((an = MSD_highCreateAxisTranslation(x, y, z)) != -1)
   {
      aptr = MSD_getAxis(an);
      strcpy(AxsName(aptr), name);
      return(SUCCESS);
   }
   return(ERROR);
}

#ifdef __Extended_Version
void MSD_execCreateAxisTranslation(void)
{
   float x, y, z;

   while (3 != sscanf(restbuf, "%f %f %f", &x, &y, &z))
   {
      printf("crtaxstrn: an x y z\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   printf("Id = %d\n", MSD_highCreateAxisTranslation(x, y, z));
}

#endif

int MSD_highCreateAxisTranslation(real x, real y, real z)
{
   vector vec;
   Id     an;

   makevec(vec, x, y, z, 1.0);
   an = MSD_getMaxAxisId();
   return(MSD_lowCreateAxisTranslation(an, vec) == ERROR ? -1 : an);
}

int MSD_middleCreateAxisTranslation(int an, real x, real y, real z)
{
   vector vec;

   makevec(vec, x, y, z, 1.0);
   return(MSD_lowCreateAxisTranslation(an, vec));
}

int MSD_lowCreateAxisTranslation(int an, vector vec)
{
   APTYPE aptr;

   if ((aptr = (APTYPE)MSD_lowNewElement(AXIS, NNIL)) == ANIL)
   {
      fprintf(stderr, MEN_NaoPossuiMemoria, NAM_CreateAxisOfTranslation);
      return(ERROR);
   }
   AxsAxisNo(aptr) = an;
   AxsMode(aptr)   = AxisOfTranslation;
   veccopy(AxsVector(aptr), vec);

   if (Gdoinversion)
   {
      MSD_lowAddEulerOp(SNIL, REMAXS, an, 0, 0, 0, 0, 0, 0, 0,
                        0.0, 0.0, 0.0, 0.0);
   }
   return(SUCCESS);
}

int MSD_execStructCrtAxsTranslation(EulerOp *op)
{
   Id   an;
   real fd1, fd2, fd3;

   an  = op->ip1;
   fd1 = op->fp1;
   fd2 = op->fp2;
   fd3 = op->fp3;
   return(MSD_middleCreateAxisTranslation(an, fd1, fd2, fd3));
}

void MSD_printStructCrtAxsTranslation(FILE *f, EulerOp *op)
{
   Id   sid, ip1;
   real fd1, fd2, fd3, fd4;

   sid = op->solidno;
   ip1 = op->ip1;
   fd1 = op->fp1;
   fd2 = op->fp2;
   fd3 = op->fp3;
   fd4 = op->fp4;
   fprintf(f, "crtaxstrs %d %d %f %f %f %f\n", sid, ip1, fd1, fd2, fd3, fd4);
}

int MSD_readStructCrtAxsTranslation(FILE *f, EulerOp *op)
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

int MSD_modifyStructCrtAxsTranslation(EulerOp *op)
{
   int axisno;

   axisno  = MSD_lowCheckTranslatorTable(op->ip1, AXIS);
   op->ip1 = axisno;
   return(SUCCESS);
}
