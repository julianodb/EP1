/* 
  PROJETO USPDesigner
  MODULO: DIS (Hidden Line Elimination)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: discplip3.cpp
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

int MSD_lowClip3Dimension(vector v1, vector v2)
{
   vector normal;
   real   h0, v0;

   if (MSD_lowCheckClipDisplay(v1, v2) == TRUE)
   {
      if (camptr->mode == 1)
      {
         normal[0] = camptr->scale;
         normal[1] = 0.0;
         normal[2] = 1.0;
         normalize(normal);
         if (MSD_lowClipingPlane(v1, v2, normal, 0.0))
         {
            normal[0] = -normal[0];
            if (MSD_lowClipingPlane(v1, v2, normal, 0.0))
            {
               normal[0] = 0.0;
               normal[1] = camptr->scale;
               normal[2] = xyratio;
               normalize(normal);
               if (MSD_lowClipingPlane(v1, v2, normal, 0.0))
               {
                  normal[1] = -normal[1];
                  if (MSD_lowClipingPlane(v1, v2, normal, 0.0))
                  {
                     return(TRUE);
                  }
               }
            }
         }
      }
      else
      {
         h0        = camptr->cdis / camptr->scale;
         v0        = xyratio * h0;
         normal[0] = 1.0;
         normal[2] = normal[1] = 0.0;
         if (MSD_lowClipingPlane(v1, v2, normal, h0))
         {
            normal[0] = -1.0;
            if (MSD_lowClipingPlane(v1, v2, normal, h0))
            {
               normal[0] = 0.0;
               normal[1] = 1.0;
               if (MSD_lowClipingPlane(v1, v2, normal, v0))
               {
                  normal[1] = -1.0;
                  if (MSD_lowClipingPlane(v1, v2, normal, v0))
                  {
                     return(TRUE);
                  }
               }
            }
         }
      }
   }
   return(FALSE);
}

int MSD_lowCheckClipDisplay(vector t1, vector t2)
{
   vector s;
   real   eps10, t;
   real   *tp;

   eps10 = abseps * 10;
   if ((t1[2] > eps10) && (t2[2] > eps10))
   {
      return(TRUE);
   }
   if (!((t1[2] > eps10) || (t2[2] > eps10)))
   {
      return(FALSE);
   }
   if (fabs(t1[2] - t2[2]) < abseps)
   {
      return(FALSE);
   }

   t    = (eps10 - t1[2]) / (t2[2] - t1[2]);
   s[0] = t1[0] + t * (t2[0] - t1[0]);
   s[1] = t1[1] + t * (t2[1] - t1[1]);
   s[2] = eps10;

   tp = (t1[2] > eps10) ? t2 : t1;
   veccopy(tp, s);
   return(TRUE);
}

int MSD_lowClipingPlane(vector v1, vector v2, vector normal, real d)
{
   real p1, p2;
   real *midp;

   p1 = d + dot(normal, v1);
   p2 = d + dot(normal, v2);
   if ((p1 <= 0.0) && (p2 <= 0.0))
   {
      return(FALSE);
   }
   if ((p1 >= 0.0) && (p2 >= 0.0))
   {
      return(TRUE);
   }
   midp = (p1 >= 0.0) ? v2 : v1;
   calc_p(v2, v1, p1 / (p1 - p2), midp);
   return(TRUE);
}

void MSD_lowEdgeClip(EPTYPE eptr)
{
   vector sv, ev, svc, evc, sv2, ev2;

   veccopy(sv, VerVCoord(HalVtx(EdgHe1(eptr))));
   veccopy(ev, VerVCoord(HalVtx(EdgHe2(eptr))));
   veccopy(svc, EdgSVClp3(eptr));
   veccopy(evc, EdgEVClp3(eptr));
   EdgEClp(eptr) = MSD_lowClipDisplayTransformation(sv, ev, svc, evc);
   if (EdgEClp(eptr) == TRUE)
   {
      veccopy(sv2, EdgSVClp2(eptr));
      veccopy(ev2, EdgEVClp2(eptr));
      MSD_lowExecDisplayTransformation(svc, evc, sv2, ev2);
      vec2cp(EdgSVClp2(eptr), sv2);
      vec2cp(EdgEVClp2(eptr), ev2);
   }
   veccopy(EdgSVClp3(eptr), svc);
   veccopy(EdgEVClp3(eptr), evc);
}

void MSD_lowDetermineVisibility(CTYPE *cptr)
{
   SPTYPE optr;
   DPTYPE dptr;
   FPTYPE fptr;
   EPTYPE eptr;

   for (AllSolids(optr))
   {
      if (SolDsp(optr) != TRUE)
      {
         continue;
      }
      if (breakflg)
      {
         return;
      }
      for (AllShellsSolid(optr, dptr))
      {
         for (AllFacesShell(dptr, fptr))
         {
            FacFVisi(fptr) = MSD_lowIsFaceVisible(cptr, fptr);
         }
      }
      for (AllShellsSolid(optr, dptr))
      {
         for (AllEdgesShell(dptr, eptr))
         {
            EdgEVis(eptr) = MSD_lowIsEdgeVisible(cptr, eptr);
            MSD_lowEdgeClip(eptr);
         }
      }
   }
}
