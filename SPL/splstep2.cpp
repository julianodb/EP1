/* 
  PROJETO USPDesigner
  MODULO: SPL (Corte de Solido)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: splstep2.cpp
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
#include <stdlib.h>
#include "memvirtu.h"
#include "lowparam.h"
#include "lowmacro.h"
#include "lowsolid.h"
#include "eulerops.h"
#include "lowgeome.h"
#include "vectorop.h"
#include "shpshape.h"
#include "splsplit.h"

/* vertex neighborhood classifier */
void MSD_lowSplitClassify(vector SP)
{
   int i;

#ifdef TRACE_SPLIT
 #ifdef TRACE_SPLIT2
   int         j;
   static char *ff[] = { "BELOW ", "ON ", "ABOVE" };
 #endif

   fprintf(trace, "\n\n\n");
   fprintf(trace, "*************************************************\n");
   fprintf(trace, "*                                               *\n");
   fprintf(trace, "*                SETSPLITCLASSIFY               *\n");
   fprintf(trace, "*                                               *\n");
   fprintf(trace, "*************************************************\n");
   fprintf(trace, "\n");
#endif

   for (spl_nedge = i = 0; i < spl_nvtx; ++i)
   {
#ifdef TRACE_SPLIT2
      fprintf(trace, "\n*** plano = %10.4f %10.4f %10.4f %10.4f ***\n", SP[0], SP[1], SP[2], SP[3]);
      trace_v(SplSoov(i));
#endif

      MSD_lowGetNeighborhood(SplSoov(i), SP);

#ifdef TRACE_SPLIT2
      fprintf(trace, "\n*** getneighborhood ***\n");
      for (j = 0; j < spl_nnbr; ++j)
      {
         trace_nbr(j);
      }
#endif

      MSD_lowReclassifyOnSectors(SP);

#ifdef TRACE_SPLIT2
      fprintf(trace, "\n*** reclassifyonsectors ***\n");
      for (j = 0; j < spl_nnbr; ++j)
      {
         trace_nbr(j);
      }
#endif

      MSD_lowReclassifyOnEdges();

#ifdef TRACE_SPLIT2
      fprintf(trace, "\n*** reclassifyonedges ***\n");
      for (j = 0; j < spl_nnbr; ++j)
      {
         trace_nbr(j);
      }
      fprintf(trace, "\n");
#endif

      MSD_lowInsertNullEdges();
   }

#ifdef TRACE_SPLIT
   fprintf(trace, "\n");
   fprintf(trace, "*** setsplitclassify *** - nedge = %2i\n", spl_nedge);
   fprintf(trace, "*** estrutura sone   ***\n");
   for (i = 0; i < spl_nedge; i++)
   {
      trace_e(SplSone(i));
   }
#endif
}

/* initial classification */
void MSD_lowGetNeighborhood(VPTYPE v, vector SP)
{
   HPTYPE he;
   VPTYPE vv;
   real   dist;

#ifndef __VIRTUAL_MEM
   VTYPE bisect;
#else
   VPTYPE bisect;

   bisect = MSD_getMemVirtual(VERTEX);
#endif

   spl_nnbr = 0;
   he       = VerVEdge(v);
   do
   {
      vv   = HalVtx(HalNxt(he));
      dist = MSD_lowDistancePlanePoint(SP, vv);
      SplNBRC1(spl_nnbr)     = comp(dist, 0.0, EPS);
      SplNBRSector(spl_nnbr) = he;
      spl_nnbr++;
      if (MSD_lowCheckWideness(he) == 1)
      {
#ifndef __VIRTUAL_MEM
         MSD_lowBisector(he, bisect.vcoord);
         SplNBRSector(spl_nnbr) = he;
         SplNBRC1(spl_nnbr)     = SplNBRC1(spl_nnbr - 1);
         dist = MSD_lowDistancePlanePoint(SP, &bisect);
         SplNBRC1(spl_nnbr - 1) = comp(dist, 0.0, EPS);
         spl_nnbr++;
         if (spl_nnbr == MAXEDGESFORVERTEX)
         {
            fprintf(stderr, "spl_nbr: falta memoria!\n");
            exit(1);
         }
#else
         MSD_lowBisector(he, VerVCoord(bisect));
         SplNBRSector(spl_nnbr) = he;
         SplNBRC1(spl_nnbr)     = SplNBRC1(spl_nnbr - 1);
         dist = MSD_lowDistancePlanePoint(SP, bisect);
         SplNBRC1(spl_nnbr - 1) = comp(dist, 0.0, EPS);
         spl_nnbr++;
#endif
      }
   } while ((he = HalNxt(mate(he))) != VerVEdge(v));

#ifdef __VIRTUAL_MEM
   MSD_putMemVirtual(bisect, VERTEX);
#endif
}

/* reclassification of ON-sectors */
void MSD_lowReclassifyOnSectors(vector SP)
{
   FPTYPE f;
   vector c;
   int    i, j;

   for (i = 0; i < spl_nnbr; ++i)
   {
      f = LooLFace(HalWLoop(SplNBRSector(i)));
      cross(c, FacFeq(f), SP);
      if (comp(dot(c, c), 0.0, EPS) == 0)
      {
         if (comp(dot(FacFeq(f), SP), 0.0, EPS) == 1)
         {
            SplNBRC1(i) = BELOW;
            j           = (i + spl_nnbr - 1) % spl_nnbr;
            SplNBRC1(j) = BELOW;
         }
         else
         {
            SplNBRC1(i) = ABOVE;
            j           = (i + spl_nnbr - 1) % spl_nnbr;
            SplNBRC1(j) = ABOVE;
         }
      }
   }
}

/* reclassification of ON-edges */
void MSD_lowReclassifyOnEdges(void)
{
   int i;

   for (i = 0; i < spl_nnbr; ++i)
   {
      if (SplNBRC1(i) == ON)
      {
         if (SplNBRC1((spl_nnbr + i - 1) % spl_nnbr) == BELOW)
         {
            SplNBRC1(i) = SplNBRC1((i + 1) % spl_nnbr) == BELOW ? ABOVE : BELOW;
         }
         else
         {
            SplNBRC1(i) = BELOW;
         }
      }
   }
}

/* insertion of NULL edges */
void MSD_lowInsertNullEdges(void)
{
   int    start, i;
   HPTYPE head;
   HPTYPE tail;

   i = 0;
   while (!(SplNBRC1(i) == /* BELOW */ BELOW && SplNBRC1((i + 1) % spl_nnbr) == /* ABOVE */ ABOVE))
   {
      if (++i == spl_nnbr)
      {
         return;
      }
   }
   start = i;
   head  = SplNBRSector((i + 1) % spl_nnbr);
   while (1)
   {
      while (!(SplNBRC1(i) == /* ABOVE */ ABOVE && SplNBRC1((i + 1) % spl_nnbr) == /* BELOW */ BELOW))
      {
         i = (i + 1) % spl_nnbr;
      }
      tail = SplNBRSector((i + 1) % spl_nnbr);

#ifdef TRACE_SPLIT2
      fprintf(trace, "*** Aplicando o operador MEV\n");
      trace_he(head);
      trace_he(tail);
#endif
      MSD_lowMEV(head, tail, maxv++, VerVCoord(HalVtx(head))[0],
                 VerVCoord(HalVtx(head))[1],
                 VerVCoord(HalVtx(head))[2]);
#ifdef TRACE_SPLIT2
      fprintf(trace, "*** Aresta inserida na SONE\n");
      trace_e(HalEdg(HalPrv(head)));
#endif
      SplSone(spl_nedge) = HalEdg(HalPrv(head));
      spl_nedge++;
      while (!(SplNBRC1(i) == /* BELOW */ BELOW && SplNBRC1((i + 1) % spl_nnbr) == /* ABOVE */ ABOVE))
      {
         if ((i = (i + 1) % spl_nnbr) == start)
         {
            return;
         }
      }
      head = SplNBRSector((i + 1) % spl_nnbr);
   }
}

/***    Write the  procedure that checks whether the edges he->prv ***/
/*** and  he  make  a  convex  (less than 180 degrees) or concave  ***/
/*** larger  than  180  degrees)  angle.  In  the first case, the  ***/
/*** procedure should return 1, and 0 otherwise.                   ***/
int MSD_lowCheckWideness(HPTYPE he)
{
   vector ref1, ref2, ref12;
   vector v1, v2, v3;

   veccopy(v1, VerVCoord(HalVtx(HalPrv(he))));
   veccopy(v2, VerVCoord(HalVtx(HalNxt(he))));
   veccopy(v3, VerVCoord(HalVtx(he)));
   vecminus(ref1, v1, v3);
   vecminus(ref2, v2, v3);
   cross(ref12, ref1, ref2);
   if (vecnull(ref12, EPS))
   {
      return(1);
   }
   return((dot(ref12, FacFeq(LooLFace(HalWLoop(he)))) > 0.0) ? 1 : 0);
}

void MSD_lowBisector(HPTYPE he, vector bisect)
{
   vector aa, bb, cc, vhe;

   veccopy(vhe, VerVCoord(HalVtx(he)));
   vecminus(aa, VerVCoord(HalVtx(HalPrv(he))), vhe);
   vecminus(bb, VerVCoord(HalVtx(HalNxt(he))), vhe);
   vecplus(cc, aa, bb);
   vecminus(bisect, vhe, cc);
}

double MSD_lowDistancePlanePoint(vector SP, VPTYPE p)
{
   double dis;

   dis = dot(VerVCoord(p), SP) + SP[3];
   if (comp(dis, 0.0, EPS) == 0)
   {
      return(0.0);
   }
   return(dis);
}
