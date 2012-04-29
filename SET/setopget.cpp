/* 
  PROJETO USPDesigner
  MODULO: SET (Operadores Booleanos)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: setopget.cpp
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
 #include <stdlib.h>
#endif
#include <math.h>
#include "memvirtu.h"
#include "lowparam.h"
#include "lowmacro.h"
#include "lowsolid.h"
#include "eulerops.h"
#include "vectorop.h"
#include "sethmain.h"

#ifdef _Windows
using namespace USPDesigner;
#endif

/* Search for intersecting sectors */
void MSD_lowSetOpVVGetNeighborhood(VPTYPE va, VPTYPE vb)
{
   HPTYPE ha;
   HPTYPE hb;
   double d1, d2, d3, d4;
   int    i, j;

#ifdef ESP
   int    s1a, s2a, s1b, s2b;
   vector dir;
#endif

   nnba = MSD_lowSetOpNbaPreProcess(va);
   nnbb = MSD_lowSetOpNbbPreProcess(vb);

#ifdef TRACE_BOOLEAN2
   fprintf(trace, "\n*** sgetneighborhood ***\n");
   for (i = 0; i < nnba; ++i)
   {
      trace_nba(i);
   }
   for (i = 0; i < nnbb; ++i)
   {
      trace_nbb(i);
   }
#endif

   nsectors = 0;
   for (i = 0; i < nnba; ++i)
   {
      for (j = 0; j < nnbb; ++j)
      {
#ifdef TRACE_BOOLEAN21
         fprintf(trace, "*** setopgetneighborhood ***\n");
         fprintf(trace, "    sector (solida) --> %2d\n", i);
         fprintf(trace, "    sector (solidb) --> %2d\n", j);
#endif

         if (MSD_lowSetOpSectorTest(i, j))
         {
#ifdef TRACE_BOOLEAN21
            fprintf(trace, "    criado par de sectors --> %2d\n", nsectors);
#endif

            ha = SetANBHe(i);
            hb = SetBNBHe(j);
            SetSecA(nsectors) = i;
            SetSecB(nsectors) = j;
            d1 = dot(FacFeq(LooLFace(HalWLoop(hb))), SetANB1(i));
            d2 = dot(FacFeq(LooLFace(HalWLoop(hb))), SetANB2(i));
            d3 = dot(FacFeq(LooLFace(HalWLoop(ha))), SetBNB1(j));
            d4 = dot(FacFeq(LooLFace(HalWLoop(ha))), SetBNB2(j));

            SetSecs1a(nsectors) = -comp(d1, 0.0, EPS);
            SetSecs2a(nsectors) = -comp(d2, 0.0, EPS);
            SetSecs1b(nsectors) = -comp(d3, 0.0, EPS);
            SetSecs2b(nsectors) = -comp(d4, 0.0, EPS);

            SetSecInter(nsectors) = 1;
            SetSecOnOn(nsectors)  = 0;
            if (SetSecs1a(nsectors) == USP_ON)
            {
               SetSecOnOn(nsectors) = 1;
            }
            if (SetSecs1b(nsectors) == USP_ON)
            {
               SetSecOnOn(nsectors) = SetSecOnOn(nsectors) + 2;
            }
            if (SetSecs2a(nsectors) == USP_ON)
            {
               SetSecOnOn(nsectors) = SetSecOnOn(nsectors) + 4;
            }
            if (SetSecs2b(nsectors) == USP_ON)
            {
               SetSecOnOn(nsectors) = SetSecOnOn(nsectors) + 8;
            }
            nsectors++;

#ifndef __VIRTUAL_MEM
            if (nsectors == MAXSECTORS)
            {
               fprintf(stderr, "sectors: falta memoria!\n");
               exit(1);
            }
#endif
         }
#ifdef ESP
         else
         {
 #ifdef TRACE_BOOLEAN21
            fprintf(trace, "    criado par de sectors --> %2d\n", nsectors);
 #endif

            ha = SetANBHe(i);
            hb = SetBNBHe(j);
            SetSecA(nsectors) = i;
            SetSecB(nsectors) = j;

            veccopy(dir, SetANB1(i));
            s1a = MSD_lowSetOpSectorWithin(dir, SetBNB1(j),
                                           SetBNB2(j), SetBNB12(j)) ? USP_ON : USP_OUT;

            veccopy(dir, SetANB2(i));
            s2a = MSD_lowSetOpSectorWithin(dir, SetBNB1(j),
                                           SetBNB2(j), SetBNB12(j)) ? USP_ON : USP_OUT;
            SetSecs1a(nsectors) = s1a;
            SetSecs2a(nsectors) = s2a;

            veccopy(dir, SetBNB1(i));
            s1b = MSD_lowSetOpSectorWithin(dir, SetANB1(j),
                                           SetANB2(j), SetANB12(j)) ? USP_ON : USP_OUT;

            veccopy(dir, SetBNB2(i));
            s2b = MSD_lowSetOpSectorWithin(dir, SetANB1(j),
                                           SetANB2(j), SetANB12(j)) ? USP_ON : USP_OUT;
            SetSecs1b(nsectors) = s1b;
            SetSecs2b(nsectors) = s2b;

            SetSecInter(nsectors) = 0;
            nsectors++;
         }
#endif
      }
   }
}

/* Preprocessing of sectors */
int MSD_lowSetOpNbaPreProcess(VPTYPE v)
{
   HPTYPE he;
   vector v1, v2, v3, vv, bisec;
   int    i, j;

   i  = 0;
   he = VerVEdge(v);
   do
   {
      SetANBHe(i) = he;
      veccopy(v1, VerVCoord(HalVtx(HalPrv(he))));
      veccopy(v2, VerVCoord(HalVtx(he)));
      veccopy(v3, VerVCoord(HalVtx(HalNxt(he))));
      vecminus(SetANB1(i), v1, v2);
      vecminus(SetANB2(i), v3, v2);
      cross(SetANB12(i), SetANB1(i), SetANB2(i));
      if (vecnull(SetANB12(i), EPS) ||
          (dot(SetANB12(i), FacFeq(LooLFace(HalWLoop(he)))) > 0.0))
      {
         if (vecnull(SetANB12(i), EPS))
         {
            MSD_lowSetOpInside(he, bisec);
         }
         else
         {
            vecplus(bisec, SetANB1(i), SetANB2(i));
            bisec[0] = -bisec[0];
            bisec[1] = -bisec[1];
            bisec[2] = -bisec[2];
         }
         j           = i + 1;
         SetANBHe(j) = he;
         veccopy(vv, SetANB2(i));
         veccopy(SetANB2(j), vv);
         veccopy(SetANB2(i), bisec);
         veccopy(SetANB1(j), bisec);
         cross(SetANB12(i), SetANB1(i), SetANB2(i));
         cross(SetANB12(j), SetANB1(j), SetANB2(j));
         ++i;

#ifndef __VIRTUAL_MEM
         if (i == MAXSECTORS)
         {
            fprintf(stderr, "nb: falta memoria!\n");
            exit(1);
         }
#endif
      }
      ++i;

#ifndef __VIRTUAL_MEM
      if (i == MAXSECTORS)
      {
         fprintf(stderr, "nb: falta memoria!\n");
         exit(1);
      }
#endif
   } while ((he = HalNxt(mate(he))) != VerVEdge(v));
   return(i);
}

/* Preprocessing of sectors */
int MSD_lowSetOpNbbPreProcess(VPTYPE v)
{
   HPTYPE he;
   vector v1, v2, v3, vv, bisec;
   int    i, j;

   i  = 0;
   he = VerVEdge(v);
   do
   {
      SetBNBHe(i) = he;
      veccopy(v1, VerVCoord(HalVtx(HalPrv(he))));
      veccopy(v2, VerVCoord(HalVtx(he)));
      veccopy(v3, VerVCoord(HalVtx(HalNxt(he))));
      vecminus(SetBNB1(i), v1, v2);
      vecminus(SetBNB2(i), v3, v2);
      cross(SetBNB12(i), SetBNB1(i), SetBNB2(i));
      if (vecnull(SetBNB12(i), EPS) ||
          (dot(SetBNB12(i), FacFeq(LooLFace(HalWLoop(he)))) > 0.0))
      {
         if (vecnull(SetBNB12(i), EPS))
         {
            MSD_lowSetOpInside(he, bisec);
         }
         else
         {
            vecplus(bisec, SetBNB1(i), SetBNB2(i));
            bisec[0] = -bisec[0];
            bisec[1] = -bisec[1];
            bisec[2] = -bisec[2];
         }
         j           = i + 1;
         SetBNBHe(j) = he;
         veccopy(vv, SetBNB2(i));
         veccopy(SetBNB2(j), vv);
         veccopy(SetBNB2(i), bisec);
         veccopy(SetBNB1(j), bisec);
         cross(SetBNB12(i), SetBNB1(i), SetBNB2(i));
         cross(SetBNB12(j), SetBNB1(j), SetBNB2(j));
         ++i;

#ifndef __VIRTUAL_MEM
         if (i == MAXSECTORS)
         {
            fprintf(stderr, "nb: falta memoria!\n");
            exit(1);
         }
#endif
      }
      ++i;

#ifndef __VIRTUAL_MEM
      if (i == MAXSECTORS)
      {
         fprintf(stderr, "nb: falta memoria!\n");
         exit(1);
      }
#endif
   } while ((he = HalNxt(mate(he))) != VerVEdge(v));
   return(i);
}

/* Sector intersection test */
int MSD_lowSetOpSectorTest(int i, int j)
{
   HPTYPE h1;
   HPTYPE h2;
   vector intrs, nh1, nh2;
   int    c1, c2;

   h1 = SetANBHe(i);
   h2 = SetBNBHe(j);
   veccopy(nh1, FacFeq(LooLFace(HalWLoop(h1))));
   veccopy(nh2, FacFeq(LooLFace(HalWLoop(h2))));
   cross(intrs, nh1, nh2);
   if (vecnull(intrs, EPS))
   {
#ifdef TRACE_BOOLEAN21
      fprintf(trace, "    Sectors overlap!\n");
#endif

      return(MSD_lowSetOpOverlap(h1, h2));
   }

#ifdef TRACE_BOOLEAN21
   fprintf(trace, "--- Analise da interseccao e solida!\n");
#endif

   c1 = MSD_lowSetOpSectorWithin(intrs, SetANB1(i), SetANB2(i), SetANB12(i));

#ifdef TRACE_BOOLEAN21
   fprintf(trace, "--- Analise da interseccao e solidb!\n");
#endif

   c2 = MSD_lowSetOpSectorWithin(intrs, SetBNB1(j), SetBNB2(j), SetBNB12(j));

   if (c1 && c2)
   {
#ifdef TRACE_BOOLEAN21
      fprintf(trace, "    Interseccao interna aos sectores dos solida e solidb!\n");
#endif

      return(TRUE);
   }
   else
   {
      intrs[0] = -intrs[0];
      intrs[1] = -intrs[1];
      intrs[2] = -intrs[2];

#ifdef TRACE_BOOLEAN21
      fprintf(trace, "--- Analise da interseccao (invertida) e solida!\n");
#endif

      c1 = MSD_lowSetOpSectorWithin(intrs, SetANB1(i), SetANB2(i), SetANB12(i));

#ifdef TRACE_BOOLEAN21
      fprintf(trace, "--- Analise da interseccao (invertida) e solidb!\n");
#endif

      c2 = MSD_lowSetOpSectorWithin(intrs, SetBNB1(j), SetBNB2(j), SetBNB12(j));
      if (c1 && c2)
      {
#ifdef TRACE_BOOLEAN21
         fprintf(trace, "    Interseccao interna aos sectores dos solida e solidb!\n");
#endif

         return(TRUE);
      }
   }

#ifdef TRACE_BOOLEAN21
   fprintf(trace, "    Interseccao nao e interna a ambos os sectores dos solida e solidb.\n");
#endif

   return(FALSE);
}

int MSD_lowSetOpSectorWithin(vector dir, vector ref1, vector ref2, vector ref12)
{
   vector c1, c2;
   int    t1, t2;

   cross(c1, ref1, dir);
   if (vecnull(c1, EPS))
   {
      if (dot(ref1, dir) > 0.0)
      {
#ifdef TRACE_BOOLEAN21
         fprintf(trace, "    Interseccao coincide com uma aresta do setor\n");
#endif

         return(TRUE);
      }
      else
      {
#ifdef TRACE_BOOLEAN21
         fprintf(trace, "    ref1 = %f %f %f\n", ref1[0], ref1[1], ref1[2]);
         fprintf(trace, "    dir = %f %f %f\n", dir[0], dir[1], dir[2]);
         fprintf(trace, "    Interseccao e aresta sao colineares, mas de direcoes distintas\n");
#endif

         return(FALSE);
      }
   }

   cross(c2, dir, ref2);
   if (vecnull(c2, EPS))
   {
      if (dot(ref2, dir) > 0.0)
      {
#ifdef TRACE_BOOLEAN21
         fprintf(trace, "    Interseccao coincide com uma aresta do setor\n");
#endif

         return(TRUE);
      }
      else
      {
#ifdef TRACE_BOOLEAN21
         fprintf(trace, "    ref2 = %f %f %f\n", ref2[0], ref2[1], ref2[2]);
         fprintf(trace, "    dir = %f %f %f\n", dir[0], dir[1], dir[2]);
         fprintf(trace, "    Interseccao e aresta sao colineares, mas de direcoes distintas\n");
#endif

         return(FALSE);
      }
   }
   t1 = comp(dot(c1, ref12), 0.0, EPS);
   t2 = comp(dot(c2, ref12), 0.0, EPS);
   if ((t1 > 0) && (t2 > 0))
   {
#ifdef TRACE_BOOLEAN21
      fprintf(trace, "    Interseccao e interna ao setor\n");
#endif

      return(TRUE);
   }

#ifdef TRACE_BOOLEAN21
   fprintf(trace, "    ref12 = %f %f %f\n", ref12[0], ref12[1], ref12[2]);
   fprintf(trace, "    c1    = %f %f %f\n", c1[0], c1[1], c1[2]);
   fprintf(trace, "    c2    = %f %f %f\n", c2[0], c2[1], c2[2]);
   fprintf(trace, "    Interseccao e externa ao setor\n");
#endif

   return(FALSE);
}

int MSD_lowSetOpOverlap(HPTYPE he1, HPTYPE he2)
{
   MSD_lowNaoUtilizado(he2);
   MSD_lowNaoUtilizado(he1);

   return(TRUE);
}

void MSD_lowSetOpInside(HPTYPE he, vector in)
{
   vector v1, v2, eq, dir;

   veccopy(v1, VerVCoord(HalVtx(HalNxt(he))));
   veccopy(v2, VerVCoord(HalVtx(he)));
   vecminus(dir, v1, v2);
   MSD_lowFaceEq(FacFLOut(LooLFace(HalWLoop(he))), eq);
   cross(in, eq, dir);
}
