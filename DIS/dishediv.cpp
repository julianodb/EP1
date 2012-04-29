/* 
  PROJETO USPDesigner
  MODULO: DIS (Hidden Line Elimination)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: dishediv.cpp
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
#include "memvirtu.h"
#include "lowparam.h"
#include "lowmacro.h"
#include "lowsolid.h"
#include "vectorop.h"
#include "gphgraph.h"
#include "disdispl.h"

void MSD_lowEdgeEdgeIntersection(SPTYPE optr, EPTYPE eptr, FPTYPE f1ptr, FPTYPE f2ptr)
{
   real   sbox[2][2], t0, t1, d1, d2;
   vector sv2, ev2;
   vector sv3, ev3;
   vector vsv, esv, xpos, vs0, ve0, pos;
   vect2  s0, e0;
   SPTYPE xoptr;
   DPTYPE xdptr;
   EPTYPE xeptr;
   int    i;

   MSD_lowExecDisplayTransformation(desvtx, deevtx, s0, e0);
   for (i = 0; i < 2; ++i)
   {
      if (s0[i] < e0[i])
      {
         sbox[0][i] = s0[i] - releps;
         sbox[1][i] = e0[i] + releps;
      }
      else
      {
         sbox[0][i] = e0[i] - releps;
         sbox[1][i] = s0[i] + releps;
      }
   }
   MSD_lowExecInverseDisplayTransformation(s0, e0, vs0, ve0);
   for (AllSolids(xoptr))
   {
      if (SolDsp(xoptr) != TRUE)
      {
         continue;
      }
      for (AllShellsSolid(xoptr, xdptr))
      {
         for (AllEdgesShell(xdptr, xeptr))
         {
            if (EdgEVis(xeptr) != _VISIBLE)
            {
               continue;
            }
            if (!MSD_lowEdgeEdgeCheck(xoptr, optr, xeptr, eptr,
                                      sbox, f1ptr, f2ptr))
            {
               continue;
            }
            veccopy(sv2, EdgSVClp2(xeptr));
            veccopy(ev2, EdgEVClp2(xeptr));
            if (MSD_lowEdgeEdgeCheckStep2(s0, e0, sv2, ev2, &t0, &t1) == TRUE)
            {
               if (MSD_lowEdgeEdgeCheckStep3(vs0, ve0, desvtx, deevtx, t0, pos))
               {
                  MSD_lowExecInverseDisplayTransformation(sv2, ev2, vsv, esv);
                  veccopy(sv3, EdgSVClp3(xeptr));
                  veccopy(ev3, EdgEVClp3(xeptr));
                  if (MSD_lowEdgeEdgeCheckStep3(vsv, esv, sv3, ev3, t1, xpos))
                  {
                     d1 = vecd(pos, camptr->eye);
                     d2 = vecd(xpos, camptr->eye);
                     if (!(d1 < d2 - abseps))
                     {
                        MSD_lowAddEdgeDivision(pos);
                     }
                  }
                  veccopy(EdgSVClp3(xeptr), sv3);
                  veccopy(EdgEVClp3(xeptr), ev3);
               }
            }
            vec2cp(EdgSVClp2(xeptr), sv2);
            vec2cp(EdgEVClp2(xeptr), ev2);
         }
      }
   }
}

int MSD_lowEdgeEdgeCheck(SPTYPE xoptr, SPTYPE optr, EPTYPE xeptr, EPTYPE eptr, \
                         real sbox[][2], FPTYPE f1ptr, FPTYPE f2ptr)
{
   real   a0, a1;
   FPTYPE fptr;

   if (FacFVisi(LooLFace(HalWLoop(EdgHe1(xeptr)))) ==
       FacFVisi(LooLFace(HalWLoop(EdgHe2(xeptr)))))
   {
      return(FALSE);
   }
   if (optr == xoptr)
   {
      if (eptr == xeptr)
      {
         return(FALSE);
      }
      if (HalVtx(EdgHe1(eptr)) == HalVtx(EdgHe1(xeptr)))
      {
         return(FALSE);
      }
      if (HalVtx(EdgHe1(eptr)) == HalVtx(EdgHe2(xeptr)))
      {
         return(FALSE);
      }
      if (HalVtx(EdgHe2(eptr)) == HalVtx(EdgHe1(xeptr)))
      {
         return(FALSE);
      }
      if (HalVtx(EdgHe2(eptr)) == HalVtx(EdgHe2(xeptr)))
      {
         return(FALSE);
      }
   }
   fptr = LooLFace(HalWLoop(EdgHe1(xeptr)));
   if ((f1ptr == fptr) || (f2ptr == fptr))
   {
      return(FALSE);
   }
   fptr = LooLFace(HalWLoop(EdgHe2(xeptr)));
   if ((f1ptr == fptr) || (f2ptr == fptr))
   {
      return(FALSE);
   }
   if (EdgEClp(xeptr) == FALSE)
   {
      return(FALSE);
   }
   a1 = EdgSVClp2(xeptr)[0];
   a0 = EdgEVClp2(xeptr)[0];
   if (a1 > a0)
   {
      if (a1 < sbox[0][0])
      {
         return(FALSE);
      }
      if (a0 > sbox[1][0])
      {
         return(FALSE);
      }
   }
   else
   {
      if (a0 < sbox[0][0])
      {
         return(FALSE);
      }
      if (a1 > sbox[1][0])
      {
         return(FALSE);
      }
   }
   a1 = EdgSVClp2(xeptr)[1];
   a0 = EdgEVClp2(xeptr)[1];
   if (a1 > a0)
   {
      if (a1 < sbox[0][1])
      {
         return(FALSE);
      }
      if (a0 > sbox[1][1])
      {
         return(FALSE);
      }
   }
   else
   {
      if (a0 < sbox[0][1])
      {
         return(FALSE);
      }
      if (a1 > sbox[1][1])
      {
         return(FALSE);
      }
   }
   return(TRUE);
}

int MSD_lowEdgeEdgeCheckStep2(vector a1, vector a2, vector b1, vector b2, real *u, real *v)
{
   vect2 a, b, ab;
   real  cz, tmp;
   int   i;

   for (i = 0; i < 2; ++i)
   {
      a[i]  = a2[i] - a1[i];
      b[i]  = b2[i] - b1[i];
      ab[i] = b1[i] - a1[i];
   }
   if (!(fabs(cz = a[0] * b[1] - a[1] * b[0]) < releps))
   {
      *u = tmp = (ab[0] * b[1] - ab[1] * b[0]) / cz;
      if (!((tmp < -releps) || (1.0 + releps < tmp)))
      {
         *v = tmp = (ab[0] * a[1] - ab[1] * a[0]) / cz;
         if ((tmp < -releps) || (1.0 + releps < tmp))
         {
            return(FALSE);
         }
         return(TRUE);
      }
   }
   return(FALSE);
}

int MSD_lowEdgeEdgeCheckStep3(vector s0, vector s1, vector v0, vector v1, real u, vector p)
{
   vector v01, es, ev0, n;
   real   cz, n0, n1, n2, t;
   int    idx1, idx2;

   if (camptr->mode == 1)
   {
      vecminus(v01, v1, v0);
      vecminus(ev0, camptr->eye, v0);
      calc_p(s1, s0, u, es);
      vecminus(es, es, camptr->eye);
      cross(n, v01, es);
      n0   = fabs(n[0]);
      n1   = fabs(n[1]);
      n2   = fabs(n[2]);
      idx1 = (n0 >= n1 && n0 >= n2) ? 1 : 0;
      idx2 = ((n0 >= n1 && n0 >= n2) || n1 >= n2) ? 2 : 1;
      cz   = v01[idx1] * es[idx2] - v01[idx2] * es[idx1];
      if (fabs(cz) < releps)
      {
         return(FALSE);
      }
      t = (ev0[idx1] * es[idx2] - ev0[idx2] * es[idx1]) / cz;
      calc_p(v1, v0, t, p);
   }
   else
   {
      calc_p(v1, v0, u, p);
   }
   return(TRUE);
}

void MSD_lowEdgeFaceIntersection(SPTYPE o1ptr, SPTYPE o2ptr, FPTYPE f1ptr, FPTYPE f2ptr)
{
   SPTYPE xop;
   DPTYPE xdp;
   FPTYPE xfp;
   HPTYPE dummy;
   real   ps, pe;
   vector pos;

   for (AllSolids(xop))
   {
      if ((SolDsp(xop) != TRUE) || (xop == o1ptr) || (xop == o2ptr))
      {
         continue;
      }
      if (!MSD_lowBoxBoxIntersection(&SolBox(xop), &debox))
      {
         continue;
      }
      for (AllShellsSolid(xop, xdp))
      {
         for (AllFacesShell(xdp, xfp))
         {
            if ((FacFVisi(xfp) == DONTSHOW) || (xfp == f1ptr) || (xfp == f2ptr))
            {
               continue;
            }
            if (!MSD_lowBoxBoxIntersection(&FacBox(xfp), &debox))
            {
               continue;
            }
            ps = dot(desvtx, FacFeq(xfp)) + FacFeq(xfp)[3];
            pe = dot(deevtx, FacFeq(xfp)) + FacFeq(xfp)[3];
            if ((ps > -abseps) && (pe > -abseps))
            {
               continue;
            }
            if ((ps < abseps) && (pe < abseps))
            {
               continue;
            }
            calc_p(deevtx, desvtx, -ps / (pe - ps), pos);
            if (MSD_lowIsInsideFace(pos, xfp, &dummy) != DONTSHOW)
            {
               MSD_lowAddEdgeDivision(pos);
            }
         }
      }
   }
}

void MSD_lowAddEdgeDivision(vector pos)
{
   divedptr->devis = 2;
   veccopy((divedptr++)->devtx, pos);
   ++divednum;
}
