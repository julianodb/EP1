/* 
  PROJETO USPDesigner
  MODULO: SHP (Criacao de Formas Primitivas)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: shpgeoms.cpp
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
#include "shpshape.h"

void MSD_lowSetNormal(SPTYPE optr, int op)
{
   DPTYPE dptr;
   FPTYPE fptr;

   if ((op == TRUE) || ((op == FALSE) && SolidBoxNOK(optr)))
   {
      MSD_lowSolidBox(optr, &(SolBox(optr)));
   }
   for (AllShellsSolid(optr, dptr))
   {
      if ((op == TRUE) || ((op == FALSE) && ShellBoxNOK(dptr)))
      {
         MSD_lowShellBox(dptr);
      }
      for (AllFacesShell(dptr, fptr))
      {
         if (op == TRUE)
         {
            MSD_lowFaceEq(FacFLOut(fptr), FacFeq(fptr));
            MSD_lowFaceBox(fptr);
         }
         else
         {
            if (FaceEqNOK(fptr))
            {
               MSD_lowFaceEq(FacFLOut(fptr), FacFeq(fptr));
            }
            if (FaceBoxNOK(fptr))
            {
               MSD_lowFaceBox(fptr);
            }
         }
      }
   }
}

void MSD_lowSetEdgeAngle(SPTYPE optr)
{
   DPTYPE dptr;
   EPTYPE eptr;
   vector edir1, edir2;
   vector spnt, epnt, lfac, rfac;
   double th, costh;

   for (AllShellsSolid(optr, dptr))
   {
      for (AllEdgesShell(dptr, eptr))
      {
         EdgAngle(eptr) = 0.0;
         veccopy(spnt, VerVCoord(HalVtx(EdgHe1(eptr))));
         veccopy(epnt, VerVCoord(HalVtx(EdgHe2(eptr))));
         vecminus(edir1, epnt, spnt);
         if (normalize(edir1))
         {
            veccopy(lfac, FacFeq(LooLFace(HalWLoop(EdgHe1(eptr)))));
            veccopy(rfac, FacFeq(LooLFace(HalWLoop(EdgHe2(eptr)))));
            costh = dot(lfac, rfac);
            if ((costh >= -1.0) && (costh <= 1.0))
            {
               th = acos(costh);
               if (dot(cross(edir2, lfac, rfac), edir1) < 0.0)
               {
                  th = -th;
               }
               EdgAngle(eptr) = th;
            }
         }
      }
   }
}
