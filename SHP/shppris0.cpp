/* 
  PROJETO USPDesigner
  MODULO: SHP (Criacao de Formas Primitivas)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: shppris0.cpp
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
#include "memvirtu.h"
#include "lowparam.h"
#include "lowsolid.h"
#include "eulerops.h"
#include "mancommd.h"
#include "shpshape.h"

SPTYPE MSD_lowMakePrism0(Id sn, XY *listvtx, int n, int pln)
{
   SPTYPE s;
   int flag = 0, GOld;

   GOld = Gdoinversion;
   Gdoinversion = FALSE;
   CommandCount++;
   if ((s = MSD_lowMakePolygon(sn, listvtx, n, pln)) != SNIL)
   {
      flag = 1;
      MSD_lowSetNormal(s, TRUE);
      MSD_lowSetEdgeAngle(s);
      MSD_lowSetInfo(s);
      MSD_lowAddEulerOp(s, HARDREM, 0, 0, 0, 0, 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0);
   }
   Gdoinversion = GOld;
   return(flag == 0 ? SNIL : s);
}

SPTYPE MSD_lowMakePolygon(Id sn, XY *listvtx, int n, int pln)
{
   SPTYPE s;
   int i, flag = 1;
   real x, y, z;

   x = listvtx->x;
   y = pln == 1 ? listvtx->y : 0.0;
   z = pln == 2 ? listvtx->y : 0.0;
   if ((s = MSD_lowMVSF(sn, maxf++, maxv++, maxd++, x, y, z)) == SNIL)
   {
      flag = 0;
   }
   for (i = 1; i < n; i++)
   {
      x = (listvtx + i)->x;
      y = pln == 1 ? (listvtx + i)->y : 0.0;
      z = pln == 2 ? (listvtx + i)->y : 0.0;
      if (MSD_highMEV(sn, maxf - 1, maxv - 1, maxv, x, y, z) == ERROR)
      {
         flag = 0;
      }
      maxv++;
   }
   if (MSD_highMEF(sn, maxv - n, maxv - 1, maxf - 1, maxf) == ERROR)
   {
      flag = 0;
   }
   maxf++;
   if (flag == 1)
   {
      MSD_lowSetNormal(s, TRUE);
   }
   if (flag == 0)
   {
      MSD_lowHardRemove(s);
   }
   return(flag == 0 ? SNIL : s);
}
