/* 
  PROJETO USPDesigner
  MODULO: DIS (Hidden Line Elimination)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: dishsore.cpp
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
#include "lowmacro.h"
#include "lowsolid.h"
#include "vectorop.h"
#include "disdispl.h"

void MSD_lowSortDividedEdge(void)
{
   DEDGTYPE *p, *q, *minp;
   real     min, l, ll, w, x;
   int      i, j, idx, n;

   if (divednum == 2)
   {
      return;
   }
   ll  = (l = deevtx[0] - desvtx[0]) > 0.0 ? l : -l;
   idx = 0;
   for (i = 1; i < 3; ++i)
   {
      if ((l = (l = deevtx[i] - desvtx[i]) > 0.0 ? l : -l) > ll)
      {
         ll  = l;
         idx = i;
      }
   }

   for (p = divedge, i = 1; i < divednum; ++p, ++i)
   {
      min = (minp = p)->devtx[idx];
      for (q = p + 1, j = i; j < divednum; ++q, ++j)
      {
         if (q->devtx[idx] < min)
         {
            min = (minp = q)->devtx[idx];
         }
      }
      if (p != minp)
      {
         for (j = 0; j < 3; ++j)
         {
            w              = p->devtx[j];
            p->devtx[j]    = minp->devtx[j];
            minp->devtx[j] = w;
         }
      }
   }
   x = (p = divedge)->devtx[idx];
   n = 1;
   for (q = ++p, i = 1; i < divednum; ++q, ++i)
   {
      if ((q->devtx[idx] - x) > abseps)
      {
         if (p != q)
         {
            veccopy(p->devtx, q->devtx);
         }
         x = p->devtx[idx];
         ++n;
         ++p;
      }
   }
   divednum = n;
}
