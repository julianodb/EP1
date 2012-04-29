/* 
  PROJETO USPDesigner
  MODULO: DIS (Hidden Line Elimination)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: dishinfc.cpp
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

int MSD_lowIsInsideFace(vector pos, FPTYPE xfptr, HPTYPE *ei)
{
   LPTYPE lptr;
   int    opi, op;

   if ((op = MSD_lowIsInsideLoop(pos, FacFLOut(xfptr), ei)) != DONTSHOW)
   {
      for (AllLoopsFace(xfptr, lptr))
      {
         if (lptr != FacFLOut(xfptr))
         {
            opi = MSD_lowIsInsideLoop(pos, lptr, ei);
            if (opi == _VISIBLE)
            {
               return(DONTSHOW);
            }
            if (opi == INVISIBLE)
            {
               return(INVISIBLE);
            }
         }
      }
   }
   return(op);
}

int MSD_lowIsInsideLoop(vector pos, LPTYPE lptr, HPTYPE *ei)
{
   static char chktable[9][9] =
   {
      { 0, 0, 1, 0, 2, 1, 0, 0, 3 },
      { 0, 0, 1, 0, 2, 1, 0, 2, 1 },
      { 1, 1, 0, 1, 2, 0, 3, 0, 0 },
      { 0, 0, 1, 0, 2, 2, 0, 0, 0 },
      { 2, 2, 2, 2, 2, 2, 2, 2, 2 },
      { 1, 1, 0, 2, 2, 0, 0, 0, 0 },
      { 0, 0, 3, 0, 2, 0, 0, 0, 0 },
      { 0, 2, 0, 0, 2, 0, 0, 0, 0 },
      { 3, 1, 0, 0, 2, 0, 0, 0, 0 }
   };

   HPTYPE efim;
   HPTYPE eptr;
   VPTYPE xv;
   real   n1, n2, n3, po, x10, x11, x20, x21;
   real   pos2l, pos2r, pos2b, pos2t, xpoint;
   int    idx1, idx2, count, chkidx1, chkidx2;

   n1    = fabs(FacFeq(LooLFace(lptr))[0]);
   n2    = fabs(FacFeq(LooLFace(lptr))[1]);
   n3    = fabs(FacFeq(LooLFace(lptr))[2]);
   idx1  = (n1 >= n2 && n1 >= n3) ? 1 : 0;
   idx2  = ((n1 >= n2 && n1 >= n3) || n2 >= n3) ? 2 : 1;
   pos2l = (pos[idx1]) - abseps;
   pos2r = (pos[idx1]) + abseps;
   pos2b = (po = pos[idx2]) - abseps;
   pos2t = (pos[idx2]) + abseps;

   *ei      = LooLEdg(lptr);
   xv       = HalVtx(*ei);
   x10      = VerVCoord(xv)[idx1];
   x11      = VerVCoord(xv)[idx2];
   chkidx1  = (x10 > pos2r) ? 0 : ((x10 < pos2l) ? 6 : 3);
   chkidx1 += (x11 > pos2t) ? 0 : ((x11 < pos2b) ? 2 : 1);
   efim     = eptr = HalNxt(*ei);
   count    = 0;
   do
   {
      x20      = x10;
      x21      = x11;
      chkidx2  = chkidx1;
      xv       = HalVtx(eptr);
      chkidx1  = ((x10 = VerVCoord(xv)[idx1]) > pos2r) ? 0 : ((x10 < pos2l) ? 6 : 3);
      chkidx1 += ((x11 = VerVCoord(xv)[idx2]) > pos2t) ? 0 : ((x11 < pos2b) ? 2 : 1);
      switch (chktable[chkidx1][chkidx2])
      {
          case 1:
          ++count;
          break;

          case 2:
          return(INVISIBLE);

          case 3:
          xpoint = x10 + (po - x11) / (x21 - x11) * (x20 - x10);
          if (xpoint > pos2r)
          {
             ++count;
          }
          else
          if (xpoint < pos2l)
          {
             ;
          }
          else
          {
             return(INVISIBLE);
          }
      }
      *ei = eptr;
   } while ((eptr = HalNxt(eptr)) != efim);
   return((count & 1) ? _VISIBLE : DONTSHOW);
}
