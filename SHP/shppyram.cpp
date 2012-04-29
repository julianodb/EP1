/* 
  PROJETO USPDesigner
  MODULO: SHP (Criacao de Formas Primitivas)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: shppyram.cpp
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
#include "vectorop.h"
#include "mancommd.h"
#include "shpshape.h"

SPTYPE MSD_lowMakePyramid(Id sn, XY *listvtx, real h, int n)
{
   SPTYPE        s;
   Id            face;
   int           flag = 1, GOld;
   static vector vec  = { 0.0, 0.0, 1.0, 0.0 };

   GOld         = Gdoinversion;
   Gdoinversion = FALSE;
   CommandCount++;
   if ((s = MSD_lowMakePolygon(sn, listvtx, n, 1)) == SNIL)
   {
      flag = 0;
   }
   face = dot(FacFeq(MSD_getFace(s, maxf - 2)), vec) > 0.0 ? maxf - 2 : maxf - 1;
   if (MSD_lowMakeSweep(MSD_getFace(s, face), 0.0, 0.0, h) == ERROR)
   {
      flag = 0;
   }
   if (flag == 1)
   {
      MSD_lowSetFaceToPoint(MSD_getFace(s, face), 0.0, 0.0, h);
      MSD_lowMakeFaceToPoint(MSD_getFace(s, face));
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

/*
 * Fazer estas alteracoes com newloc !!
 */
void MSD_lowSetFaceToPoint(FPTYPE fptr, real x, real y, real z)
{
   HPTYPE he;

   he = LooLEdg(FacFLoops(fptr));
   do
   {
      VerVCoord(HalVtx(he))[0] = x;
      VerVCoord(HalVtx(he))[1] = y;
      VerVCoord(HalVtx(he))[2] = z;
   } while (LooLEdg(FacFLoops(fptr)) != (he = HalNxt(he)));
}
