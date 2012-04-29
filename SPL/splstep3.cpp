/* 
  PROJETO USPDesigner
  MODULO: SPL (Corte de Solido)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: splstep3.cpp
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
#include "vectorop.h"
#include "shpshape.h"
#include "splsplit.h"

/* the joining algorithm */
void MSD_lowSplitConnect(void)
{
   EPTYPE nextedge;
   HPTYPE h1;
   HPTYPE h2;

#ifdef TRACE_SPLIT
   int i;

   fprintf(trace, "\n\n\n");
   fprintf(trace, "*************************************************\n");
   fprintf(trace, "*                                               *\n");
   fprintf(trace, "*                SETSPLITCONNECT                *\n");
   fprintf(trace, "*                                               *\n");
   fprintf(trace, "*************************************************\n");
   fprintf(trace, "\n");
#endif

   spl_null_edge_pointer = spl_nend = spl_nfac = 0;
   MSD_lowSortNullEdge();
   while ((nextedge = MSD_lowGetNextNullEdge()) != ENIL)
   {
      if ((h1 = MSD_lowCanJoin(EdgHe1(nextedge))) != HNIL)
      {
#ifdef TRACE_SPLIT3
         fprintf(trace, "*** Une as arestas do solido ");
#endif
         MSD_lowJoin(h1, EdgHe1(nextedge));
         if (!MSD_lowIsLoose(mate(h1)))
         {
            MSD_lowCut(h1);
         }
      }
      if ((h2 = MSD_lowCanJoin(EdgHe2(nextedge))) != HNIL)
      {
#ifdef TRACE_SPLIT3
         fprintf(trace, "*** Une as arestas do solido ");
#endif
         MSD_lowJoin(h2, EdgHe2(nextedge));
         if (!MSD_lowIsLoose(mate(h2)))
         {
            MSD_lowCut(h2);
         }
      }
      if ((h1 != HNIL) && (h2 != HNIL))
      {
         MSD_lowCut(EdgHe1(nextedge));
      }
   }

#ifdef TRACE_SPLIT
   fprintf(trace, "\n");
   fprintf(trace, "*** setsplitconnect *** - nfac = %2i\n", spl_nfac);
   fprintf(trace, "*** estrutura sonf  ***\n");
   for (i = 0; i < spl_nfac; i++)
   {
      trace_f(SplSonf(i));
   }
#endif
}

HPTYPE MSD_lowCanJoin(HPTYPE he)
{
   HPTYPE ret;
   int    i, j;

#ifdef TRACE_SPLIT3
   fprintf(trace, "*** canjoin\n");
   trace_he(he);
#endif

   for (i = 0; i < spl_nend; ++i)
   {
      if (MSD_lowNeighbor(he, SplEnds(i)))
      {
         ret = SplEnds(i);
         for (j = i + 1; j < spl_nend; ++j)
         {
            SplEnds(j - 1) = SplEnds(j);
         }
         spl_nend--;
#ifdef TRACE_BOOLEAN3
         fprintf(trace, "*** canjoin - OK - join is possible\n");
         trace_he(ret);
#endif
         return(ret);
      }
   }
   SplEnds(spl_nend) = he;
   spl_nend++;
#ifdef TRACE_BOOLEAN3
   fprintf(trace, "*** canjoin - join is not possible\n");
#endif
   return(HNIL);
}

/* joining and cutting of null edges */
void MSD_lowJoin(HPTYPE h1, HPTYPE h2)
{
   FPTYPE oldf;
   FPTYPE newf;

#ifdef TRACE_SPLIT3
   int flag;
   flag = 0;
#endif

   oldf = LooLFace(HalWLoop(h1));
   newf = FNIL;
   if (HalWLoop(h1) == HalWLoop(h2))
   {
#ifdef TRACE_SPLIT3
      if (HalPrv(HalPrv(h1)) != h2)
      {
         fprintf(trace, " (MEF) ***\n");
         trace_he(h1);
         trace_he(HalNxt(h2));
         newf = MSD_lowMEF(h1, HalNxt(h2), maxf++);
         flag = 1;
      }
#else
      if (HalPrv(HalPrv(h1)) != h2)
      {
         newf = MSD_lowMEF(h1, HalNxt(h2), maxf++);
      }
#endif
   }
#ifdef TRACE_SPLIT3
   else
   {
      fprintf(trace, " (MEKR) ***\n");
      trace_he(h1);
      trace_he(HalNxt(h2));
      MSD_lowMEKR(h1, HalNxt(h2));
      flag = 1;
   }
#else
   else
   {
      MSD_lowMEKR(h1, HalNxt(h2));
   }
#endif

   if (HalNxt(HalNxt(h1)) != h2)
   {
#ifdef TRACE_SPLIT3
      if (flag == 1)
      {
         fprintf(trace, "*** Une as arestas novamente ");
      }
      fprintf(trace, " (MEF) \n");
      trace_he(h2);
      trace_he(HalNxt(h1));
#endif
      MSD_lowMEF(h2, HalNxt(h1), maxf++);

#ifdef TRACE_SPLIT3
      if ((newf != FNIL) && (LooNextL(FacFLoops(oldf)) != LNIL))
      {
         fprintf(trace, "*** Operador (LARINGMV) \n");
         MSD_lowAllRingMove(oldf, newf);
      }
#else
      if ((newf != FNIL) && (LooNextL(FacFLoops(oldf)) != LNIL))
      {
         MSD_lowAllRingMove(oldf, newf);
      }
#endif
   }
}

void MSD_lowCut(HPTYPE he)
{
   if (HalWLoop(EdgHe1(HalEdg(he))) == HalWLoop(EdgHe2(HalEdg(he))))
   {
#ifdef TRACE_SPLIT3
      fprintf(trace, "*** Corta Solido (KEMR) ***\n");
      trace_he(EdgHe1(HalEdg(he)));
      fprintf(trace, "*** Acrescenta Face Solido A (sonfa)\n");
      trace_f(LooLFace(HalWLoop(he)));
#endif
      SplSonf(spl_nfac) = LooLFace(HalWLoop(he));
      spl_nfac++;
      MSD_lowKEMR(EdgHe1(HalEdg(he)), EdgHe2(HalEdg(he)));
   }

#ifdef TRACE_SPLIT3
   else
   {
      fprintf(trace, "*** Corta Solido (KEF) ***\n");
      trace_he(EdgHe1(HalEdg(he)));
      MSD_lowKEF(EdgHe1(HalEdg(he)), EdgHe2(HalEdg(he)));
   }
#else
   else
   {
      MSD_lowKEF(EdgHe1(HalEdg(he)), EdgHe2(HalEdg(he)));
   }
#endif
}

/* neighborship test */
int MSD_lowNeighbor(HPTYPE he1, HPTYPE he2)
{
   return(LooLFace(HalWLoop(he1)) == LooLFace(HalWLoop(he2)) &&
          ((he1 == EdgHe1(HalEdg(he1)) && he2 == EdgHe2(HalEdg(he2))) ||
           (he1 == EdgHe2(HalEdg(he1)) && he2 == EdgHe1(HalEdg(he2)))));
}

int MSD_lowIsLoose(HPTYPE he)
{
   int i;

   for (i = 0; i < spl_nend; i++)
   {
      if (SplEnds(i) == he)
      {
         return(TRUE);
      }
   }
   return(FALSE);
}

void MSD_lowSortNullEdge(void)
{
   register int i, j;

   if (spl_nedge < 5)
   {
      return;
   }
   for (i = 0; i < spl_nedge; i++)
   {
      for (j = i + 1; j < spl_nedge; j++)
      {
         if (MSD_lowGreater(i, j))
         {
            MSD_lowSwap(i, j);
         }
      }
   }
}

int MSD_lowGreater(int i, int j)
{
   vector       v1, v2;
   register int com;

   veccopy(v1, VerVCoord(HalVtx(EdgHe1(SplSone(i)))));
   veccopy(v2, VerVCoord(HalVtx(EdgHe2(SplSone(j)))));
   if ((com = comp(v1[0], v2[0], EPS)) == -1)
   {
      return(0);
   }
   if (com == 0)
   {
      if ((com = comp(v1[1], v2[1], EPS)) == -1)
      {
         return(0);
      }
      if (com == 0)
      {
         if (comp(v1[2], v2[2], EPS) == -1)
         {
            return(0);
         }
      }
   }
   return(1);
}

void MSD_lowSwap(int i, int j)
{
   register EPTYPE tmp;

   tmp        = SplSone(i);
   SplSone(i) = SplSone(j);
   SplSone(j) = tmp;
}

EPTYPE MSD_lowGetNextNullEdge(void)
{
   EPTYPE e;

   if (spl_null_edge_pointer < spl_nedge)
   {
      e = SplSone(spl_null_edge_pointer);
      spl_null_edge_pointer++;
      return(e);
   }
   else
   {
      return(ENIL);
   }
}
