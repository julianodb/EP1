/* 
  PROJETO USPDesigner
  MODULO: DIS (Hidden Line Elimination)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: dishvisb.cpp
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

int MSD_lowIsEdgeVisible(CTYPE *cptr, EPTYPE eptr)
{
   int    lfvisi, rfvisi;
   vector dir, edir, ev, sv, vfrom;
   FPTYPE lfptr;
   FPTYPE rfptr;
   VPTYPE svptr;
   VPTYPE evptr;

   svptr = HalVtx(EdgHe1(eptr));
   evptr = HalVtx(EdgHe2(eptr));
   if (cptr->mode == 1)
   {
      veccopy(vfrom, VerVCoord(svptr));
   }
   else
   {
      veccopy(vfrom, cptr->ref);
   }
   vecminus(dir, cptr->eye, vfrom);
   normalize(dir);
   lfptr  = LooLFace(HalWLoop(EdgHe1(eptr)));
   rfptr  = LooLFace(HalWLoop(EdgHe2(eptr)));
   lfvisi = FacFVisi(lfptr);
   rfvisi = FacFVisi(rfptr);
   veccopy(ev, VerVCoord(evptr));
   veccopy(sv, VerVCoord(svptr));
   vecminus(edir, ev, sv);
   if ((lfvisi == _VISIBLE) && MSD_lowIsInvertedEdge(TRUE, dir, lfptr, edir))
   {
      return(DONTSHOW);
   }
   if ((rfvisi == _VISIBLE) && MSD_lowIsInvertedEdge(FALSE, dir, rfptr, edir))
   {
      return(DONTSHOW);
   }
   if ((lfvisi != rfvisi) && ((lfvisi == INVISIBLE) || (rfvisi == INVISIBLE)))
   {
      return(EdgAngle(eptr) > 0.0 ? _VISIBLE : INVISIBLE);
   }
   if ((cptr->smooth == 0) && (fabs(EdgAngle(eptr)) < angsmooth))
   {
      return(DONTSHOW);
   }
   if ((lfvisi == DONTSHOW) && (rfvisi == DONTSHOW))
   {
      return(INVISIBLE);
   }
   if ((EdgAngle(eptr) < 0.0) && ((lfvisi == DONTSHOW) || (rfvisi == DONTSHOW)))
   {
      return(INVISIBLE);
   }
   return(_VISIBLE);
}

int MSD_lowIsFaceVisible(CTYPE *cptr, FPTYPE fptr)
{
   real   dis;
   vector feq;

   veccopy(feq, FacFeq(fptr));
   if (cptr->mode == 1)
   {
      dis = dot(feq, cptr->eye) + feq[3];
      return(dis > abseps ? INVISIBLE : dis < -abseps ? DONTSHOW : _VISIBLE);
   }
   else
   {
      dis = dot(cptr->drot[2], feq);
      return(dis < -abseps ? INVISIBLE : dis > abseps ? DONTSHOW : _VISIBLE);
   }
}

int MSD_lowIsInvertedEdge(int isleft, vector dir, FPTYPE fp, vector edir)
{
   real   det;
   vector direction;

   if (normalize(edir) == TRUE)
   {
      det = dot(cross(direction, edir, FacFeq(fp)), dir);
      det = (isleft) ? det : -det;
      return(det < releps);
   }
   return(TRUE);
}
