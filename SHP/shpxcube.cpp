/* 
  PROJETO USPDesigner
  MODULO: SHP (Criacao de Formas Primitivas)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: shpxcube.cpp
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
#include "vectorop.h"
#include "mancommd.h"
#include "shpshape.h"
#include "disdispl.h"

void MSD_execNameMakeCube(void)
{
   char  name[30];
   float a, b, c;

   while (4 != sscanf(restbuf, "%s %f %f %f", name, &a, &b, &c))
   {
      printf("Objeto  A  B  C\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highNameMakeCube(name, a, b, c);
}

void MSD_highNameMakeCube(char *name, real a, real b, real c)
{
   Id sn;

   if (MSD_getSolidIdFromName(name) != -1)
   {
      fprintf(stderr, MEN_SolidoJaExiste, NAM_MakeCube, name);
      return;
   }
   if ((sn = MSD_highMakeCube(a, b, c)) != -1)
   {
      MSD_highName(sn, name, SOLID);
   }
}

#ifdef __Extended_Version
void MSD_execMakeCube(void)
{
   float a, b, c;
   int   sn;

   while (4 != sscanf(restbuf, "%d %f %f %f", &sn, &a, &b, &c))
   {
      printf("A  B  C\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   maxs = sn;
   printf("Id = %d\n", MSD_highMakeCube(a, b, c));
}

#endif

Id MSD_highMakeCube(real a, real b, real c)
{
   SPTYPE optr;
   Id     sn;

   sn = MSD_getMaxSolidId();
   if ((a > 0.0) && (b > 0.0) && (c > 0.0))
   {
      if ((optr = MSD_lowMakeCube(sn, a, b, c)) != SNIL)
      {
         MSD_lowDisplayMonitor('o', optr, GNIL);
         return(sn);
      }
   }

   if ((a > 0.0) && (b > 0.0) && (c <= 0.0))
   {
      if ((optr = MSD_lowMakeCube0(sn, a, b)) != SNIL)
      {
         MSD_lowDisplayMonitor('o', optr, GNIL);
         return(sn);
      }
   }
   if ((b > 0.0) && (c > 0.0) && (a <= 0.0))
   {
      if ((optr = MSD_lowMakeCube0(sn, b, c)) != SNIL)
      {
         MSD_lowDisplayMonitor('o', optr, GNIL);
         return(sn);
      }
   }
   if ((a > 0.0) && (c > 0.0) && (b <= 0.0))
   {
      if ((optr = MSD_lowMakeCube0(sn, a, c)) != SNIL)
      {
         MSD_lowDisplayMonitor('o', optr, GNIL);
         return(sn);
      }
   }
   fprintf(stderr, MEN_ParametrosIncorretos, NAM_MakeCube);
   return(-1);
}

SPTYPE MSD_lowMakeCube(Id sn, real dx, real dy, real dz)
{
   SPTYPE        s;
   Id            face;
   int           flag = 1, GOld;
   vector        normal;
   static vector vec = { 0.0, 0.0, 1.0, 0.0 };

   GOld         = Gdoinversion;
   Gdoinversion = FALSE;
   CommandCount++;
   if ((s = MSD_lowMVSF(sn, maxf++, maxv++, maxd++, 0.0, 0.0, 0.0)) == SNIL)
   {
      flag = 0;
   }
   if (MSD_highMEV(sn, maxf - 1, maxv - 1, maxv, dx, 0.0, 0.0) == ERROR)
   {
      flag = 0;
   }
   maxv++;
   if (MSD_highMEV(sn, maxf - 1, maxv - 1, maxv, dx, dy, 0.0) == ERROR)
   {
      flag = 0;
   }
   maxv++;
   if (MSD_highMEV(sn, maxf - 1, maxv - 1, maxv, 0.0, dy, 0.0) == ERROR)
   {
      flag = 0;
   }
   maxv++;
   if (MSD_highMEF(sn, maxv - 4, maxv - 1, maxf - 1, maxf) == ERROR)
   {
      flag = 0;
   }
   maxf++;
   if (flag == 1)
   {
      MSD_lowFaceEq(FacFLOut(MSD_getFace(s, maxf - 2)), normal);
      face = dot(normal, vec) > 0.0 ? maxf - 2 : maxf - 1;
      if (MSD_lowMakeSweep(MSD_getFace(s, face), 0.0, 0.0, dz) == ERROR)
      {
         flag = 0;
      }
      if (flag == 1)
      {
         MSD_lowSetNormal(s, TRUE);
         MSD_lowSetEdgeAngle(s);
         MSD_lowSetInfo(s);
      }
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
   return((flag == 0) ? SNIL : s);
}

SPTYPE MSD_lowMakeCube0(Id sn, real dx, real dy)
{
   SPTYPE s;
   int    flag = 1, GOld;

   GOld         = Gdoinversion;
   Gdoinversion = FALSE;
   CommandCount++;
   if ((s = MSD_lowMVSF(sn, maxf++, maxv++, maxd++, 0.0, 0.0, 0.0)) == SNIL)
   {
      flag = 0;
   }
   if (MSD_highMEV(sn, maxf - 1, maxv - 1, maxv, dx, 0.0, 0.0) == ERROR)
   {
      flag = 0;
   }
   maxv++;
   if (MSD_highMEV(sn, maxf - 1, maxv - 1, maxv, dx, dy, 0.0) == ERROR)
   {
      flag = 0;
   }
   maxv++;
   if (MSD_highMEV(sn, maxf - 1, maxv - 1, maxv, 0.0, dy, 0.0) == ERROR)
   {
      flag = 0;
   }
   maxv++;
   if (MSD_highMEF(sn, maxv - 4, maxv - 1, maxf - 1, maxf) == ERROR)
   {
      flag = 0;
   }
   maxf++;
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
   return((flag == 0) ? SNIL : s);
}
