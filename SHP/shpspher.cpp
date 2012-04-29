/* 
  PROJETO USPDesigner
  MODULO: SHP (Criacao de Formas Primitivas)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: shpspher.cpp
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
#include <math.h>
#include "mensagem.h"
#include "memvirtu.h"
#include "lowparam.h"
#include "lowsolid.h"
#include "eulerops.h"
#include "mancommd.h"
#include "shpshape.h"
#include "disdispl.h"

void MSD_execNameMakeSphere(void)
{
   char  name[30];
   float r;
   int   n;

   while (3 != sscanf(restbuf, "%s %d %f", name, &n, &r))
   {
      printf("Objeto  N_div  Raio\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highNameMakeSphere(name, n, r);
}

void MSD_highNameMakeSphere(char *name, int n, real r)
{
   Id sn;

   if (MSD_getSolidIdFromName(name) != -1)
   {
      fprintf(stderr, MEN_SolidoJaExiste, NAM_MakeSphere, name);
      return;
   }
   if ((sn = MSD_highMakeSphere(n, r)) != -1)
   {
      MSD_highName(sn, name, SOLID);
   }
}

#ifdef __Extended_Version
void MSD_execMakeSphere(void)
{
   float r;
   int   n;

   while (2 != sscanf(restbuf, "%d %f", &n, &r))
   {
      printf("N_div  Raio\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   printf("Id = %d\n", MSD_highMakeSphere(n, r));
}

#endif

Id MSD_highMakeSphere(int n, real r)
{
   SPTYPE optr;
   Id     sn;

   sn = MSD_getMaxSolidId();
   if ((r != 0.0) && (n > 1))
   {
      if ((optr = MSD_lowMakeSphere(sn, r, n, 2 * n)) != SNIL)
      {
         MSD_lowDisplayMonitor('o', optr, GNIL);
         return(sn);
      }
   }
   fprintf(stderr, MEN_ParametrosIncorretos, NAM_MakeSphere);
   return(-1);
}

SPTYPE MSD_lowMakeSphere(Id sn, real r, int nver, int nhor)
{
   SPTYPE        s;
   int           flag = 1, GOld;
   static vector vect = { 0.0, 1.0, 0.0, 0.0 };

   if ((nver < 2) || (nhor < 2))
   {
      return(SNIL);
   }
   GOld         = Gdoinversion;
   Gdoinversion = FALSE;
   CommandCount++;
   if ((s = MSD_lowMVSF(sn, maxf++, maxv++, maxd++, 0.0, 0.0, r)) == SNIL)
   {
      flag = 0;
   }
   if (MSD_lowMakeArc(sn, maxf - 1, maxv - 1, 0.0, 0.0, r, 0.0, 0.0, 180.0, nver) == ERROR)
   {
      flag = 0;
   }
   if (MSD_lowMakeRotationalSweep(s, nhor, 2, 0, vect) == ERROR)
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

/* The procedure arc generates an approximation of a circular arc   */
/* segment with n edges, centered at (cx cz), on plane y = h, and   */
/* with radius rad. The arc ranges from angle phi1 to phi2,         */
/* measured in degrees, where 0.0 = x-axis and angles grow counter  */
/* clockwise. The arc starts from existing vertex v of face f.      */
char MSD_lowMakeArc(Id s, Id f, Id v, real cx, real cz, real rad, real \
                    h, real phi1, real phi2, int n)
{
   real x, z, angle, inc;
   Id   prev;
   int  i;

   angle = phi1 * PI / 180.0;
   inc   = (phi2 - phi1) * PI / (180.0 * n);
   maxv  = MSD_getMaxVertexId(MSD_getSolid(s));
   for (prev = v, i = 0; i < n; ++i)
   {
      angle += inc;
      x      = cx + sin(angle) * rad;
      z      = cz + cos(angle) * rad;
      if (MSD_highMEV(s, f, prev, maxv, x, h, z) == ERROR)
      {
         return(ERROR);
      }
      prev = maxv++;
   }
   return(SUCCESS);
}

SPTYPE MSD_lowMakeCircle(Id sn, real cx, real cz, real rad, real h, int n)
{
   SPTYPE s;
   int    flag = 1;

   if ((s = MSD_lowMVSF(sn, maxf++, maxv++, maxd++, cx, h, cz + rad)) == SNIL)
   {
      flag = 0;
   }
   if (MSD_lowMakeArc(sn, maxf - 1, maxv - 1, cx, cz, rad, h, 0.0, (n - 1) * 360.0 / n, n - 1) == ERROR)
   {
      flag = 0;
   }
   if (MSD_highMEF(sn, maxv - n, maxv - 1, maxf - 1, maxf) == ERROR)
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
   return(flag == 0 ? SNIL : s);
}
