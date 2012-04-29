/* 
  PROJETO USPDesigner
  MODULO: SHP (Criacao de Formas Primitivas)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: shpcylin.cpp
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
#include <math.h>
#include <string.h>
#include "mensagem.h"
#include "memvirtu.h"
#include "lowparam.h"
#include "lowmacro.h"
#include "lowsolid.h"
#include "eulerops.h"
#include "vectorop.h"
#include "mancommd.h"
#include "shpshape.h"
#include "disdispl.h"

void MSD_execNameMakeCylinder(void)
{
   char name1[15];
   int n;
   float r, h;

   while (4 != sscanf(restbuf, "%s %d %f %f", name1, &n, &r, &h))
   {
      printf("Objeto N_div Raio Altura\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highNameMakeCylinder(name1, n, r, h);
}

void MSD_highNameMakeCylinder(char *name, int n, real r, real h)
{
   Id sn;

   if (MSD_getSolidIdFromName(name) != -1)
   {
      fprintf(stderr, MEN_SolidoJaExiste, NAM_MakeCylinder, name);
      return;
   }
   if ((sn = MSD_highMakeCylinder(n, r, h)) != -1)
   {
      MSD_highName(sn, name, SOLID);
   }
}

#ifdef __Extended_Version
void MSD_execMakeCylinder(void)
{
   int n, sn;
   float r, h;

   while (4 != sscanf(restbuf, "%d %d %f %f", &sn, &n, &r, &h))
   {
      printf("N_div Raio Altura\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   maxs = sn;
   printf("Id = %d\n", MSD_highMakeCylinder(n, r, h));
}

#endif

Id MSD_highMakeCylinder(int n, real r, real h)
{
   SPTYPE optr;
   Id sn;

   sn = MSD_getMaxSolidId();
   if ((r != 0.0) && (h != 0.0) && (n > 2))
   {
      if ((optr = MSD_lowMakeCylinder(sn, r, h, n)) != SNIL)
      {
         MSD_lowDisplayMonitor('o', optr, GNIL);
         return(sn);
      }
   }
   if ((r != 0.0) && (n > 2))
   {
      if ((optr = MSD_lowMakeCylinder0(sn, r, n)) != SNIL)
      {
         MSD_lowDisplayMonitor('o', optr, GNIL);
         return(sn);
      }
   }
   fprintf(stderr, MEN_ParametrosIncorretos, NAM_MakeCylinder);
   return(-1);
}

void MSD_execNameMakeCone(void)
{
   char name1[15];
   int n;
   float r, h;

   while (4 != sscanf(restbuf, "%s %d %f %f", name1, &n, &r, &h))
   {
      printf("Objeto N_div Raio Altura\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highNameMakeCone(name1, n, r, h);
}

void MSD_highNameMakeCone(char *name, int n, real r, real h)
{
   Id sn;

   if (MSD_getSolidIdFromName(name) != -1)
   {
      fprintf(stderr, MEN_SolidoJaExiste, NAM_MakeCone, name);
      return;
   }
   if ((sn = MSD_highMakeCone(n, r, h)) != -1)
   {
      MSD_highName(sn, name, SOLID);
   }
}

#ifdef __Extended_Version
void MSD_execMakeCone(void)
{
   int n, sn;
   float r, h;

   while (4 != sscanf(restbuf, "%d %d %f %f", &sn, &n, &r, &h))
   {
      printf("N_div Raio Altura\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   maxs = sn;
   printf("Id = %d\n", MSD_highMakeCone(n, r, h));
}

#endif

Id MSD_highMakeCone(int n, real r, real h)
{
   SPTYPE optr;
   Id sn;

   sn = MSD_getMaxSolidId();
   if ((r != 0.0) && (h != 0.0) && (n > 2))
   {
      if ((optr = MSD_lowMakeCone(sn, r, h, n)) != SNIL)
      {
         MSD_lowDisplayMonitor('o', optr, GNIL);
         return(sn);
      }
   }
   if ((r != 0.0) && (n > 2))
   {
      if ((optr = MSD_lowMakeCylinder0(sn, r, n)) != SNIL)
      {
         MSD_lowDisplayMonitor('o', optr, GNIL);
         return(sn);
      }
   }
   fprintf(stderr, MEN_ParametrosIncorretos, NAM_MakeCone);
   return(-1);
}

SPTYPE MSD_lowMakeCylinder(Id sn, real rad, real h, int n)
{
   SPTYPE s;
   Id face;
   int flag = 1, GOld;
   static vector vec  = { 0.0, 1.0, 0.0, 0.0 };

   GOld = Gdoinversion;
   Gdoinversion = FALSE;
   CommandCount++;
   if ((s = MSD_lowMakeCircle(sn, 0.0, 0.0, rad, 0.0, n)) == SNIL)
   {
      flag = 0;
   }
   if (flag == 1)
   {
      face = dot(FacFeq(MSD_getFace(s, maxf - 2)), vec) > 0.0 ? maxf - 2 : maxf - 1;
      if (MSD_lowMakeSweep(MSD_getFace(s, face), 0.0, h, 0.0) == ERROR)
      {
         flag = 0;
      }
      if (flag == 1)
      {
         MSD_lowRot90Dg(s);
         MSD_lowSetNormal(s, TRUE);
         MSD_lowSetEdgeAngle(s);
         MSD_lowSetInfo(s);
      }
      if (flag == 0)
      {
         MSD_lowHardRemove(s);
      }
   }
   if (flag == 1)
   {
      MSD_lowAddEulerOp(s, HARDREM, 0, 0, 0, 0, 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0);
   }
   Gdoinversion = GOld;
   return(flag == 0 ? SNIL : s);
}

SPTYPE MSD_lowMakeCylinder0(Id sn, real rad, int n)
{
   SPTYPE s;
   int GOld;

   GOld         = Gdoinversion;
   Gdoinversion = FALSE;
   CommandCount++;
   if ((s = MSD_lowMakeCircle(sn, 0.0, 0.0, rad, 0.0, n)) != SNIL)
   {
      MSD_lowRot90Dg(s);
      MSD_lowSetNormal(s, TRUE);
      MSD_lowSetEdgeAngle(s);
      MSD_lowSetInfo(s);
      MSD_lowAddEulerOp(s, HARDREM, 0, 0, 0, 0, 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0);
   }
   Gdoinversion = GOld;
   return(s);
}

SPTYPE MSD_lowMakeCone(Id sn, real rad, real h, int n)
{
   SPTYPE s;
   Id face;
   int flag = 1, GOld;
   static vector vec  = { 0.0, 1.0, 0.0, 0.0 };

   GOld = Gdoinversion;
   Gdoinversion = FALSE;
   CommandCount++;
   if ((s = MSD_lowMakeCircle(sn, 0.0, 0.0, rad, 0.0, n)) == SNIL)
   {
      flag = 0;
   }
   face = dot(FacFeq(MSD_getFace(s, maxf - 2)), vec) > 0.0 ? maxf - 2 : maxf - 1;
   if (MSD_lowMakeSweep(MSD_getFace(s, face), 0.0, h, 0.0) == ERROR)
   {
      flag = 0;
   }
   if (flag == 1)
   {
      MSD_lowSetFaceToPoint(MSD_getFace(s, face), 0.0, h, 0.0);
      MSD_lowMakeFaceToPoint(MSD_getFace(s, face));
      MSD_lowRot90Dg(s);
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

void MSD_lowRot90Dg(SPTYPE s)
{
   matrix m;
   DPTYPE d;
   VPTYPE vptr;

   matident(m);
   matrotat(m, 90.0, 0.0, 0.0);
   for (AllShellsSolid(s, d))
   {
      for (AllVerticesShell(d, vptr))
      {
         vecmult(VerVCoord(vptr), VerVCoord(vptr), m);
      }
   }
}
