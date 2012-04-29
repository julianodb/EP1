/* 
  PROJETO USPDesigner
  MODULO: DIS (Hidden Line Elimination)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: disledge.cpp
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
#include "disdispl.h"
#include "gphgraph.h"

void MSD_lowDisplayDividedEdge(CTYPE *cptr, EPTYPE eptr)
{
   vect2    s1, s2;
   DEDGTYPE *spr, *epr;
   int      i, dsp, evis;

   evis = FALSE;
   spr  = divedptr = divedge;
   dsp  = divedptr->devis;
   for (++divedptr, i = 1; i < divednum; ++divedptr, ++i)
   {
      for ( ; i < divednum; ++divedptr, ++i)
      {
         if (dsp != divedptr->devis)
         {
            break;
         }
      }
      epr = (i < divednum) ? divedptr : divedptr - 1;
      MSD_lowExecDisplayTransformation(spr->devtx, epr->devtx, s1, s2);
      switch (dsp)
      {
          case FALSE:
          if (cptr->dash)
          {
             g_tline(DASHED_LINE, s1, s2);
          }
          break;

          case TRUE:
          evis = TRUE;
          g_tline(SOLID_LINE, s1, s2);
          break;
      }
      spr = epr;
      dsp = divedptr->devis;
   }
   if ((eptr != ENIL) && !evis)
   {
      EdgEVis(eptr) = INVISIBLE;
   }
}
