/* 
  PROJETO USPDesigner
  MODULO: SHP (Criacao de Formas Primitivas)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: shptorus.cpp
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
#include "lowsolid.h"
#include "eulerops.h"
#include "mancommd.h"
#include "shpshape.h"
#include "disdispl.h"

void MSD_execNameMakeTorus(void)
{
   char  name[30];
   float r0, r1;
   int   n;

   while (4 != sscanf(restbuf, "%s %d %f %f", name, &n, &r0, &r1))
   {
      printf("Objeto  N_div R_maior R_menor\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highNameMakeTorus(name, n, r0, r1);
}

void MSD_highNameMakeTorus(char *name, int n, real r0, real r1)
{
   Id sn;

   if (MSD_getSolidIdFromName(name) != -1)
   {
      fprintf(stderr, MEN_SolidoJaExiste, NAM_MakeTorus, name);
      return;
   }
   if ((sn = MSD_highMakeTorus(n, r0, r1)) != -1)
   {
      MSD_highName(sn, name, SOLID);
   }
}

#ifdef __Extended_Version
void MSD_execMakeTorus(void)
{
   float r0, r1;
   int   n;

   while (3 != sscanf(restbuf, "%d %f %f", &n, &r0, &r1))
   {
      printf("N_div R_maior R_menor\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   printf("Id = %d\n", MSD_highMakeTorus(n, r0, r1));
}

#endif

Id MSD_highMakeTorus(int n, real r0, real r1)
{
   SPTYPE optr;
   Id     sn;

   sn = MSD_getMaxSolidId();
   if ((r1 < r0) && (r1 != 0.0) && (n > 2))
   {
      if ((optr = MSD_lowMakeTorus(sn, r0, r1, 2 * n, n)) != SNIL)
      {
         MSD_lowDisplayMonitor('o', optr, GNIL);
         return(sn);
      }
   }
   fprintf(stderr, MEN_ParametrosIncorretos, NAM_MakeTorus);
   return(-1);
}

SPTYPE MSD_lowMakeTorus(Id sn, real r1, real r2, int nf1, int nf2)
{
   SPTYPE        s;
   int           flag = 1, GOld;
   static vector vect = { 0.0, 1.0, 0.0, 0.0 };

   GOld         = Gdoinversion;
   Gdoinversion = FALSE;
   CommandCount++;
   if ((s = MSD_lowMakeCircle(sn, r1, 0.0, r2, 0.0, nf2)) == SNIL)
   {
      flag = 0;
   }
   if (MSD_lowMakeRotationalSweep(s, nf1, 2, 0, vect) == ERROR)
   {
      flag = 0;
   }
   if (flag == 1)
   {
      MSD_lowSetNormal(s, TRUE);
      MSD_lowSetEdgeAngle(s);
      MSD_lowSetInfo(s);
   }
   if (flag == 0)
   {
      MSD_lowHardRemove(s);
   }
   if (flag == 1)
   {
      MSD_lowAddEulerOp(s, HARDREM, 0, 0, 0, 0, 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0);
   }
   Gdoinversion = GOld;
   return(flag == 0 ? SNIL : s);
}
