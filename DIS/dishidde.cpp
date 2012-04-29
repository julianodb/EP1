/* 
  PROJETO USPDesigner
  MODULO: DIS (Hidden Line Elimination)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: dishidde.cpp
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
#ifndef __UNIX_
 #ifndef _Windows
  #include <graphics.h>
 #else
  #include <windows.h>
 #endif
#endif
#include "memvirtu.h"
#include "lowparam.h"
#include "lowmacro.h"
#include "lowsolid.h"
#include "vectorop.h"
#include "shpshape.h"
#include "disdispl.h"
#include "genfunc_.h"
#include "gphgraph.h"

void MSD_lowCameraDisplayHidden(void)
{
   CTYPE *cptr;

   if (camptr->mode == 3)
   {
      cptr   = camptr;
      camptr = &camera3;
      MSD_lowSetCamera3View(1, cptr);
      MSD_lowDisplayHidden(camptr);

      MSD_lowSetCamera3View(2, cptr);
      MSD_lowDisplayHidden(camptr);

      MSD_lowSetCamera3View(3, cptr);
      MSD_lowDisplayHidden(camptr);

      MSD_lowSetCamera3View(4, cptr);
      MSD_lowDisplayHidden(camptr);

      MSD_lowSetCamera3View(0, cptr);
   }
   else
   {
      MSD_lowDisplayHidden(camptr);
   }
}

void MSD_lowDisplayHidden(CTYPE *cptr)
{
   SPTYPE optr;
   DPTYPE dptr;
   FPTYPE rf;
   FPTYPE lf;
   EPTYPE eptr;
   vector sv, ev, edir;

   g_frame();
   MSD_lowDetermineVisibility(cptr);
   divedge = (DEDGTYPE *)workbuf;
   for (AllSolids(optr))
   {
      if (SolDsp(optr) != TRUE)
      {
         continue;
      }
      g_pcolor(MSD_lowGetSolidColor(optr));
      for (AllShellsSolid(optr, dptr))
      {
         for (AllEdgesShell(dptr, eptr))
         {
            if (breakflg)
            {
               return;
            }
            switch (EdgEVis(eptr))
            {
                case INVISIBLE:
                if ((cptr->dash == TRUE) && (EdgEClp(eptr) == TRUE))
                {
                   veccopy(sv, EdgSVClp2(eptr));
                   veccopy(ev, EdgEVClp2(eptr));
                   g_tline(DASHED_LINE, sv, ev);
                }
                break;

                case _VISIBLE:
                if (EdgEClp(eptr) == TRUE)
                {
                   veccopy(desvtx, EdgSVClp3(eptr));
                   veccopy(deevtx, EdgEVClp3(eptr));
                   MSD_lowUpdateDEBox();
                   divednum = 0;
                   divedptr = divedge;
                   MSD_lowAddEdgeDivision(desvtx);
                   rf = LooLFace(HalWLoop(EdgHe1(eptr)));
                   lf = LooLFace(HalWLoop(EdgHe2(eptr)));
                   MSD_lowEdgeEdgeIntersection(optr, eptr, lf, rf);
                   MSD_lowEdgeFaceIntersection(optr, SNIL, lf, rf);
                   MSD_lowAddEdgeDivision(deevtx);
                   MSD_lowSortDividedEdge();
                   veccopy(sv, VerVCoord(HalVtx(EdgHe1(eptr))));
                   veccopy(ev, VerVCoord(HalVtx(EdgHe2(eptr))));
                   vecminus(edir, ev, sv);
                   if (normalize(edir) == TRUE)
                   {
                      MSD_lowSetEdgeVisibility('e', lf, rf, edir);
                      MSD_lowDisplayDividedEdge(cptr, eptr);
                   }
                }
                break;
            }
         }
      }
   }
}
