/* 
  PROJETO USPDesigner
  MODULO: RND (Arredondamento)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: rndaux__.cpp
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
#include "memvirtu.h"
#include "lowmacro.h"
#include "lowparam.h"
#include "lowsolid.h"
#include "eulerops.h"
#include "vectorop.h"
#include "mancommd.h"
#include "rndround.h"
#include "curvgeom.h"

void calc_point(vector v1, vector v2, real t, vector p)
{
   vecminus(p, v2, v1);
   vecesc(p, p, t);
   vecplus(p, p, v1);
}

real angle_edge(HPTYPE he)
{
   vector v1, v2, v3;
   real   alfa, aux1, aux2;

   veccopy(v3, VerVCoord(HalVtx(mate(he))));
   vecminus(v1, v3, VerVCoord(HalVtx(he)));
   veccopy(v3, VerVCoord(HalVtx(HalPrv(he))));
   vecminus(v2, v3, VerVCoord(HalVtx(he)));
   aux1 = dot(v1, v2);
   aux2 = sqrt(v1[0] * v1[0] + v1[1] * v1[1] + v1[2] * v1[2]) * sqrt(v2[0] * v2[0] + v2[1] * v2[1] + v2[2] * v2[2]);
   alfa = acos(aux1 / aux2);
   cross(v3, v1, v2);
   normalize(v3);
   vecminus(v1, v3, FacFeq(LooLFace(HalWLoop(he))));
   aux1 = v1[0] + v1[1] + v1[2];
   if (comp(aux1, 0.0, EPS) == 0)
   {
      return(alfa);
   }
   else
   {
      alfa = (2 * PI) - alfa;
      return(alfa);
   }
}

int GetEdges(VPTYPE v, int flag)
{
   HPTYPE he;
   int    n = 0;

   he = VerVEdge(v);
   do
   {
      if (flag == -1)
      {
         n++;
      }
      if (flag == EdgFgRound(HalEdg(he)))
      {
         n++;
      }
      he = HalNxt(mate(he));
   } while (he != VerVEdge(v));
   return(n);
}

void CLC(SPTYPE s, HPTYPE he, vector p1, vector p2, vector c)
{
   vector t1, t2;

   maxct = getmaxct(s);
   lgeo_arc(c, p1, p2, t1, t2, vecd(c, p1), vecd(c, p2));
   MSD_highNewCurvePolyline(SolSolidNo(s), ++maxct);
   MSD_highAddPointToCurve(SolSolidNo(s), maxct, p1[0], p1[1], p1[2]);
   MSD_highAddPointToCurve(SolSolidNo(s), maxct, t1[0], t1[1], t1[2]);
   MSD_highAddPointToCurve(SolSolidNo(s), maxct, t2[0], t2[1], t2[2]);
   MSD_highAddPointToCurve(SolSolidNo(s), maxct, p2[0], p2[1], p2[2]);
   MSD_lowSetCurveAtributeToEdge(he, MSD_getCurve(s, maxct));
}

void lgeo_arc(vector c, vector p1, vector p2, vector t1, vector t2, real d1, real d2)
{
   vector n;

   vecminus(n, c, p1);
   normalize(n);
   t1[0] = p1[0] + d1 * 0.552 * n[0];
   t1[1] = p1[1] + d1 * 0.552 * n[1];
   t1[2] = p1[2] + d1 * 0.552 * n[2];
   vecminus(n, c, p2);
   normalize(n);
   t2[0] = p2[0] + d2 * 0.552 * n[0];
   t2[1] = p2[1] + d2 * 0.552 * n[1];
   t2[2] = p2[2] + d2 * 0.552 * n[2];
}

void _line(vector r1, real d1, vector r2, real d2, vector p1, vector p2, vector n)
{
   vector pv, r;

   veczer(pv);
   veczer(r);

   vecminus(r, r2, r1);
   cross(pv, n, r);
   normalize(pv);
   vecesc(pv, pv, d1);
   vecplus(p1, r1, pv);
   normalize(pv);
   vecesc(pv, pv, d2);
   vecplus(p2, r2, pv);
}

void line_parallel(vector r1, vector r2, vector p1, vector p2, vector n, real d)
{
   vector pv, r;

   veczer(pv);
   veczer(r);

   vecminus(r, r2, r1);
   cross(pv, n, r);
   normalize(pv);
   vecesc(pv, pv, d);
   vecplus(p2, r2, pv);
   vecplus(p1, r1, pv);
}

real calc_t(vector v1, vector v2, vector p)
{
   vector a1, a2;
   real   a3, a4, at;

   veczer(a1);
   veczer(a2);
   vecminus(a1, p, v1);
   vecminus(a2, v2, v1);
   a3 = sqrt(a1[0] * a1[0] + a1[1] * a1[1] + a1[2] * a1[2]);
   a4 = sqrt(a2[0] * a2[0] + a2[1] * a2[1] + a2[2] * a2[2]);
   at = (a3 * a4) / (a4 * a4);
   return(at);
}

void line_inter(vector v1, vector v2, vector v3, vector v4, vector p)
{
   double D, a1, a2, b1, b2, c1, c2, t1;
   vector n1, n2, n;
   int    i, drop;

   vecminus(n1, v2, v1);
   vecminus(n2, v4, v3);
   cross(n, n1, n2);
   for (i = 0; i < 3; i++)
   {
      if (comp(n[i], 0, EPS) == 0)
      {
         n[i] = 0;
      }
   }
   normalize(n);
   drop = getdrop(n);

   switch (drop)
   {
       case X:
       a1 = v2[1] - v1[1];
       a2 = v2[2] - v1[2];
       b1 = v3[1] - v4[1];
       b2 = v3[2] - v4[2];
       c1 = v1[1] - v3[1];
       c2 = v1[2] - v3[2];
       break;

       case Y:
       a1 = v2[0] - v1[0];
       a2 = v2[2] - v1[2];
       b1 = v3[0] - v4[0];
       b2 = v3[2] - v4[2];
       c1 = v1[0] - v3[0];
       c2 = v1[2] - v3[2];
       break;

       case Z:
       a1 = v2[0] - v1[0];
       a2 = v2[1] - v1[1];
       b1 = v3[0] - v4[0];
       b2 = v3[1] - v4[1];
       c1 = v1[0] - v3[0];
       c2 = v1[1] - v3[1];
       break;
   }

   if (comp(D = a1 * b2 - a2 * b1, 0.0, EPS) != 0)
   {
      t1   = (c2 * b1 - c1 * b2) / D;
      p[0] = v1[0] + t1 * (v2[0] - v1[0]);
      p[1] = v1[1] + t1 * (v2[1] - v1[1]);
      p[2] = v1[2] + t1 * (v2[2] - v1[2]);
   }
}

void point_perp(vector v1, vector v2, vector p, vector n, real d)
{
   vector pv, v;

   veczer(pv);
   veczer(v);

   vecminus(v, v2, v1);
   cross(pv, n, v);
   normalize(pv);
   vecesc(pv, pv, d);
   vecplus(p, v1, pv);
}
