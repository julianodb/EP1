/* 
  PROJETO USPDesigner
  MODULO: SPL (Corte de Solido)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: splstep1.cpp
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
#include "memvirtu.h"
#include "lowparam.h"
#include "lowmacro.h"
#include "lowsolid.h"
#include "eulerops.h"
#include "lowgeome.h"
#include "vectorop.h"
#include "shpshape.h"
#include "splsplit.h"

void MSD_lowSplitGenerate(SPTYPE s, vector SP)
{
   DPTYPE d;
   EPTYPE e;
   HPTYPE hh;
   VPTYPE v1;
   VPTYPE v2;
   double d1, d2, t, x, y, z;
   int    s1, s2;

#ifdef TRACE_SPLIT
   int i;

   fprintf(trace, "*************************************************\n");
   fprintf(trace, "*                                               *\n");
   fprintf(trace, "*                SETSPLITGENERATE               *\n");
   fprintf(trace, "*                                               *\n");
   fprintf(trace, "*************************************************\n");
   fprintf(trace, "\n");
#endif

   spl_nvtx = 0;
   for (AllShellsSolid(s, d))
   {
      for (AllEdgesShell(d, e))
      {
#ifdef TRACE_SPLIT1
         fprintf(trace, "\n");
         fprintf(trace, "*** SplitGenerate - Aresta ***\n");
         trace_e(e);
#endif
         v1 = HalVtx(EdgHe1(e));
         v2 = HalVtx(EdgHe2(e));
         s1 = comp((d1 = MSD_lowDistancePlanePoint(SP, v1)), 0.0, EPS);
         s2 = comp((d2 = MSD_lowDistancePlanePoint(SP, v2)), 0.0, EPS);
         if (((s1 == -1) && (s2 == 1)) || ((s1 == 1) && (s2 == -1)))
         {
#ifdef TRACE_SPLIT1
            fprintf(trace, "*** Ocorre interseccao aresta-superficie\n");
#endif
            t = d1 / (d1 - d2);
            x = VerVCoord(v1)[0] + t * (VerVCoord(v2)[0] - VerVCoord(v1)[0]);
            y = VerVCoord(v1)[1] + t * (VerVCoord(v2)[1] - VerVCoord(v1)[1]);
            z = VerVCoord(v1)[2] + t * (VerVCoord(v2)[2] - VerVCoord(v1)[2]);
            MSD_lowMEV(EdgHe1(e), (hh = HalNxt(EdgHe2(e))), maxv++, x, y, z);
            MSD_lowAddSplitOnVertex(HalVtx(HalPrv(hh)));
         }
         else
         {
#ifdef TRACE_SPLIT1
            if (s1 == 0)
            {
               fprintf(trace, "*** Ocorre interseccao vertice-superficie\n");
               trace_v(v1);
               MSD_lowAddSplitOnVertex(v1);
            }
#else
            if (s1 == 0)
            {
               MSD_lowAddSplitOnVertex(v1);
            }
#endif
#ifdef TRACE_SPLIT1
            if (s2 == 0)
            {
               fprintf(trace, "*** Ocorre interseccao vertice-superficie\n");
               trace_v(v2);
               MSD_lowAddSplitOnVertex(v2);
            }
#else
            if (s2 == 0)
            {
               MSD_lowAddSplitOnVertex(v2);
            }
#endif
         }
      }
   }

#ifdef TRACE_SPLIT
   fprintf(trace, "*** setsplitgenerate *** - nvtx = %3i\n", spl_nvtx);
   fprintf(trace, "*** estrutura soov   ***\n");
   for (i = 0; i < spl_nvtx; i++)
   {
      trace_v(SplSoov(i));
   }
#endif
}

void MSD_lowAddSplitOnVertex(VPTYPE v)
{
   int i;

   for (i = 0; i < spl_nvtx; i++)
#ifdef TRACE_SPLIT1
   {
      if (SplSoov(i) == v)
      {
         fprintf(trace,
                 "*** Nao foi acrescentado a estrutura - (soov) repetido\n");
         return;
      }
   }
#else
   {
      if (SplSoov(i) == v)
      {
         return;
      }
   }
#endif

   SplSoov(spl_nvtx) = v;
   spl_nvtx++;
}
