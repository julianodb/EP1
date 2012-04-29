/* 
  PROJETO USPDesigner
  MODULO: DIS (Hidden Line Elimination)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: dishevis.cpp
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
#include "disdispl.h"
#include "gphgraph.h"

void MSD_lowSetEdgeVisibility(char t, FPTYPE f1ptr, FPTYPE f2ptr, vector edir)
{
   vector   mid;
   DEDGTYPE *p0, *p1;
   int      i;

   divedptr = divedge;
   for (i = 0; i < divednum - 1; ++divedptr, ++i)
   {
      p1 = (p0 = divedptr) + 1;
      calc_p(p0->devtx, p1->devtx, 0.5, mid);
      if (t == 'i')
      {
         if (MSD_lowIntersectionCheck(mid, f1ptr, f2ptr))
         {
            if (divedptr->devis != TRUE)
            {
               continue;
            }
         }
      }
      divedptr->devis = MSD_lowPositionalVisibility(t, mid, f1ptr, f2ptr, edir);
   }
   divedptr->devis = FALSE;
}

int MSD_lowIntersectionCheck(vector mid, FPTYPE f1ptr, FPTYPE f2ptr)
{
   if (MSD_lowIntersectionCheckStep1(mid, f1ptr, f2ptr))
   {
      return(TRUE);
   }
   if (MSD_lowIntersectionCheckStep1(mid, f2ptr, f1ptr))
   {
      return(TRUE);
   }
   if ((FacFVisi(f1ptr) == DONTSHOW) || (FacFVisi(f2ptr) == DONTSHOW))
   {
      divedptr->devis = FALSE;
      return(TRUE);
   }
   return(FALSE);
}

int MSD_lowIntersectionCheckStep1(vector mid, FPTYPE f1ptr, FPTYPE f2ptr)
{
   vector edir1, edir2, v1, v2;
   HPTYPE xe;

   switch (MSD_lowIsInsideFace(mid, f1ptr, &xe))
   {
       case DONTSHOW:
       divedptr->devis = -TRUE;
       return(TRUE);

       case INVISIBLE:
       divedptr->devis = FALSE;
       veccopy(v1, VerVCoord(HalVtx(HalNxt(xe))));
       veccopy(v2, VerVCoord(HalVtx(xe)));
       vecminus(edir1, v1, v2);
       if (normalize(edir2))
       {
          if (LooLFace(HalWLoop(mate(xe))) == f1ptr)
          {
             cross(edir2, FacFeq(f1ptr), edir1);
             divedptr->devis = dot(edir2, FacFeq(f2ptr)) > releps;
          }
          else
          {
             cross(edir2, FacFeq(f2ptr), edir1);
             divedptr->devis = dot(edir2, FacFeq(f1ptr)) > releps;
          }
       }
       return(TRUE);
   }
   return(FALSE);
}

int MSD_lowPositionalVisibility(char tp, vector v, FPTYPE f1, FPTYPE f2, vector edir)
{
   SPTYPE xo;
   DPTYPE xd;
   FPTYPE xf;
   FPTYPE lf;
   FPTYPE rf;
   HPTYPE xe;
   BTYPE  box;
   vector v1, pos, dir;
   real   t;
   int    i;

   if (camptr->mode == 1)
   {
      veccopy(v1, camptr->eye);
   }
   else
   {
      for (i = 0; i < 3; ++i)
      {
         v1[i] = v[i] + camptr->cdis * camptr->eyedir[i];
      }
   }
   for (i = 0; i < 3; ++i)
   {
      box.min[i] = -abseps + ((v[i] < v1[i]) ? v[i] : v1[i]);
      box.max[i] = abseps + ((v[i] > v1[i]) ? v[i] : v1[i]);
   }

   for (AllSolids(xo))
   {
      if (SolDsp(xo) != TRUE)
      {
         continue;
      }
      if (!MSD_lowBoxBoxIntersection(&box, &SolBox(xo)))
      {
         continue;
      }
      for (AllShellsSolid(xo, xd))
      {
         for (AllFacesShell(xd, xf))
         {
            if ((xf == f1) || (xf == f2) || !FacFVisi(xf))
            {
               continue;
            }
            if (!MSD_lowBoxBoxIntersection(&box, &FacBox(xf)))
            {
               continue;
            }
            if (!MSD_lowEdgePlaneIntersection(v, v1, xf, pos, &t))
            {
               continue;
            }
            if (t < -releps)
            {
               continue;
            }

            switch (MSD_lowIsInsideFace(pos, xf, &xe))
            {
                case INVISIBLE:
                if (t > releps)
                {
                   lf = LooLFace(HalWLoop(EdgHe1(HalEdg(xe))));
                   rf = LooLFace(HalWLoop(EdgHe2(HalEdg(xe))));
                   if (EdgEVis(HalEdg(xe)) == DONTSHOW)
                   {
                      if (lf == f1)
                      {
                         break;
                      }
                      if (rf == f1)
                      {
                         break;
                      }
                      if (lf == f2)
                      {
                         break;
                      }
                      if (rf == f2)
                      {
                         break;
                      }
                   }
                   return(FALSE);
                }
                break;

                case _VISIBLE:
                if (t > releps)
                {
                   return(FALSE);
                }
                if (tp == 'e')
                {
                   if (FacFVisi(f1) || FacFVisi(f2))
                   {
                      cross(dir, FacFeq(f1), edir);
                      if (dot(dir, FacFeq(xf)) < -releps)
                      {
                         break;
                      }
                      cross(dir, FacFeq(f2), edir);
                      if (dot(dir, FacFeq(xf)) > releps)
                      {
                         break;
                      }
                   }
                }
                return(FALSE);
            }
         }
      }
   }
   return(TRUE);
}

int MSD_lowEdgePlaneIntersection(vector v1, vector v2, FPTYPE xf, real *pos, real *t)
{
   real p1, p2;

   p1 = dot(FacFeq(xf), v1) + FacFeq(xf)[3];
   p2 = dot(FacFeq(xf), v2) + FacFeq(xf)[3];
   if (!(fabs(p1 - p2) < abseps))
   {
      calc_p(v2, v1, *t = p1 / (p1 - p2), pos);
      return(TRUE);
   }
   return(FALSE);
}
