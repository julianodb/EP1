/* 
  PROJETO USPDesigner
  MODULO: LOW (Operadores Basicos)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: lowcheck.cpp
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
#include "memvirtu.h"
#include "lowparam.h"
#include "lowmacro.h"
#include "lowsolid.h"

void MSD_execTrace(void)
{
   if (TraceBoundaryStructure())
   {
      fprintf(stderr, "Ocorreu Erro - trace\n");
   }
}

int TraceBoundaryStructure(void)
{
   SPTYPE s;

   if (error == TRUE)
   {
      return(TRUE);
   }
   for (AllSolids(s))
   {
      if (TraceSolid(s) == TRUE)
      {
         return(TRUE);
      }
   }
   return(FALSE);
}

int TraceSolid(SPTYPE s)
{
   if (checkf(s) == ERROR)
   {
      return(TRUE);
   }
   if (checke(s) == ERROR)
   {
      return(TRUE);
   }
   if (checkv(s) == ERROR)
   {
      return(TRUE);
   }
   return(FALSE);
}

int checkShell(FPTYPE f, DPTYPE d)
{
   FPTYPE f2;
   LPTYPE l;
   HPTYPE he;

   if (FacFShell(f) != d)
   {
      return(1);
   }
   SetFaceBitOK1(f);
   for (AllLoopsFace(f, l))
   {
      he = LooLEdg(l);
      if (HalEdg(he) == ENIL)
      {
         continue;
      }
      do
      {
         f2 = LooLFace(HalWLoop(mate(he)));
         if (FaceBitNOK1(f2))
         {
            if (checkShell(f2, d) == 1)
            {
               return(1);
            }
         }
      } while ((he = HalNxt(he)) != LooLEdg(l));
   }
   return(0);
}

int checkf(SPTYPE s)
{
   DPTYPE dd;
   DPTYPE d;
   FPTYPE f;
   LPTYPE l;
   LPTYPE out;
   HPTYPE he;

#ifdef __VIRTUAL_MEM
   MSD_checkSolid(s);
#endif

   assert(s != SNIL);
   assert(SolSShells(s) != DNIL);

#ifdef __VIRTUAL_MEM
   MSD_checkShell(SolSShells(s));
#endif

   for (AllShellsSolid(s, d))
   {
#ifdef __VIRTUAL_MEM
      MSD_checkShell(d);
#endif

      assert(SheSSolids(d) == s);
      assert(SheSFaces(d) != FNIL);

#ifdef __VIRTUAL_MEM
      MSD_checkFace(SheSFaces(d));
#endif

      for (AllShellsSolid(s, dd))
      {
         for (AllFacesShell(dd, f))
         {
            SetFaceBitNOK1(f);
         }
      }
      assert(checkShell(SheSFaces(d), d) == 0);

      for (AllFacesShell(d, f))
      {
#ifdef __VIRTUAL_MEM
         MSD_checkFace(f);
         MSD_checkLoop(FacFLoops(f));
         MSD_checkLoop(FacFLOut(f));
#endif

         assert(FacFLoops(f) != LNIL);
         assert(FacFLOut(f) != LNIL);
         assert(FacFShell(f) == d);
         out = LNIL;
         for (AllLoopsFace(f, l))
         {
#ifdef __VIRTUAL_MEM
            MSD_checkLoop(l);
            MSD_checkHalfedge(LooLEdg(l));
#endif

            assert(LooLFace(l) == f);
            assert(LooLEdg(l) != HNIL);
            he = LooLEdg(l);
            do
            {
#ifdef __VIRTUAL_MEM
               MSD_checkLoop(HalWLoop(he));
               MSD_checkHalfedge(HalPrv(he));
               MSD_checkHalfedge(HalNxt(he));
               // MSD_checkEdge(HalEdg(he)) ;
#endif

               assert(HalWLoop(he) == l);
               assert(HalPrv(he) != HNIL);
               assert(HalNxt(he) != HNIL);
               assert(findv(d, HalVtx(he)) == 1);
               if (HalEdg(he) != ENIL)
               {
                  assert(findes(d, HalEdg(he)) == 1);

#ifdef __VIRTUAL_MEM
                  MSD_checkVertex(HalVtx(he));
                  MSD_checkHalfedge(EdgHe1(HalEdg(he)));
                  MSD_checkHalfedge(EdgHe2(HalEdg(he)));
#endif

                  assert(VerVEdge(HalVtx(he)) != HNIL);
                  assert(findhes(d, VerVEdge(HalVtx(he))) == 1);
                  assert(EdgHe1(HalEdg(he)) != HNIL);
                  assert(EdgHe2(HalEdg(he)) != HNIL);
                  assert(he == EdgHe1(HalEdg(he)) || he == EdgHe2(HalEdg(he)));
                  assert(findhes(d, mate(he)) == 1);
               }
            } while ((he = HalNxt(he)) != LooLEdg(l));
            if (l == FacFLOut(f))
            {
               out = l;
            }
         }
         assert(out != LNIL);
      }
   }
   return(SUCCESS);
}

int findhes(DPTYPE d, HPTYPE he)
{
   FPTYPE f;
   LPTYPE l;
   HPTYPE h;
   int    v1, v2;

#ifdef __VIRTUAL_MEM
   MSD_checkHalfedge(he);
   MSD_checkShell(d);
   MSD_checkLoop(HalWLoop(he));
   MSD_checkFace(LooLFace(HalWLoop(he)));
#endif

   assert((l = HalWLoop(he)) != LNIL);
   assert((f = LooLFace(l)) != FNIL);
   assert(FacFShell(f) == d);

   for (AllFacesShell(d, f))
   {
      for (AllLoopsFace(f, l))
      {
         h = LooLEdg(l);
         do
         {
            if (h == he)
            {
               return(1);
            }
         } while ((h = HalNxt(h)) != LooLEdg(l));
      }
   }

#ifdef __VIRTUAL_MEM
   MSD_checkHalfedge(he);
   MSD_checkHalfedge(HalNxt(he));
   MSD_checkVertex(HalVtx(he));
   MSD_checkVertex(HalVtx(HalNxt(he)));
#endif

   v1 = VerVertexNo(HalVtx(he));
   v2 = VerVertexNo(HalVtx(HalNxt(he)));
   fprintf(stderr, "checkf: halfedge <%d %d> not found!\n", v1, v2);
   MSD_lowListSolid(stdout, SheSSolids(d), 3);
   return(0);
}

int checke(SPTYPE s)
{
   DPTYPE d;
   DPTYPE d1;
   EPTYPE e;
   HPTYPE he1;
   HPTYPE he2;
   LPTYPE l1;
   LPTYPE l2;
   FPTYPE f1;
   FPTYPE f2;
   VPTYPE v1;
   VPTYPE v2;

   assert(s != SNIL);
   assert(SolSShells(s) != DNIL);
   for (AllShellsSolid(s, d))
   {
      assert(SheSSolids(d) == s);
      for (AllEdgesShell(d, e))
      {
         assert((he1 = EdgHe1(e)) != HNIL);
         assert((he2 = EdgHe2(e)) != HNIL);
         assert(he1 != he2);
         assert(HalEdg(he1) == e);
         assert(HalEdg(he2) == e);
         assert(HalNxt(he1) != HNIL);
         assert(HalPrv(he1) != HNIL);
         assert(HalNxt(he2) != HNIL);
         assert(HalPrv(he2) != HNIL);
         assert(HalNxt(HalPrv(he2)) == he2);
         assert(HalPrv(HalNxt(he2)) == he2);
         assert(HalNxt(HalPrv(he1)) == he1);
         assert(HalPrv(HalNxt(he1)) == he1);

         assert((l1 = HalWLoop(he1)) != LNIL);
         assert((l2 = HalWLoop(he2)) != LNIL);
         assert(findhe(he1, l1) == 1);
         assert(findhe(he2, l2) == 1);

         assert((f1 = LooLFace(l1)) != FNIL);
         assert((f2 = LooLFace(l2)) != FNIL);
         assert(findl(l1, f1) == 1);
         assert(findl(l2, f2) == 1);
         assert(findf(d, f1) == 1);
         assert(findf(d, f2) == 1);

         assert((d1 = FacFShell(f1)) != DNIL);
         assert(findd(s, d1) == 1);

         assert((v1 = HalVtx(he1)) != VNIL);
         assert((v2 = HalVtx(he2)) != VNIL);
         assert(findv(d, v1) == 1);
         assert(findv(d, v2) == 1);
         assert(VerVEdge(v1) != HNIL);
         assert(VerVEdge(v2) != HNIL);
         assert(finde(v1, he1) == 1);
         assert(finde(v2, he2) == 1);
      }
   }
   return(SUCCESS);
}

int findhe(HPTYPE he, LPTYPE l)
{
   HPTYPE h;

   assert(l != LNIL);

   h = LooLEdg(l);
   do
   {
      if (h == he)
      {
         return(1);
      }
   } while ((h = HalNxt(h)) != LooLEdg(l));

   fprintf(stderr, "checke: wrong back ptr in halfedge <%d %d>\n",
           VerVertexNo(HalVtx(he)), VerVertexNo(HalVtx(HalNxt(he))));
   MSD_lowListFace(stdout, LooLFace(l), 3);
   return(0);
}

int findes(DPTYPE d, EPTYPE e)
{
   EPTYPE edg;

   assert(d != DNIL);
   assert(SheSEdges(d) != ENIL);

   for (AllEdgesShell(d, edg))
   {
      if (edg == e)
      {
         return(1);
      }
   }
   fprintf(stderr, "checkf: edge <%d %d> not found\n",
           VerVertexNo(HalVtx(EdgHe1(e))), VerVertexNo(HalVtx(EdgHe2(e))));
   MSD_lowListSolid(stdout, SheSSolids(d), 3);
   return(0);
}

int findl(LPTYPE l, FPTYPE f)
{
   LPTYPE lp;

   for (AllLoopsFace(f, lp))
   {
      if (lp == l)
      {
         return(1);
      }
   }

   fprintf(stderr, "checke: wrong back ptr in loop <%d %d>\n",
           VerVertexNo(HalVtx(LooLEdg(l))),
           VerVertexNo(HalVtx(HalNxt(LooLEdg(l)))));
   MSD_lowListFace(stdout, LooLFace(l), 3);
   return(0);
}

int finde(VPTYPE v, HPTYPE he)
{
   HPTYPE h;

   h = VerVEdge(v);
   do
   {
      if (h == he)
      {
         return(1);
      }
   } while ((h = HalNxt(mate(h))) != VerVEdge(v));

   fprintf(stderr, "checke: wrong vertex ptr in halfedge <%d %d>\n",
           VerVertexNo(HalVtx(he)), VerVertexNo(HalVtx(HalNxt(he))));
   MSD_lowListFace(stdout, LooLFace(HalWLoop(he)), 3);
   return(0);
}

int findf(DPTYPE d, FPTYPE f)
{
   FPTYPE ff;

   assert(d != DNIL);
   assert(SheSFaces(d) != FNIL);

   for (AllFacesShell(d, ff))
   {
      if (ff == f)
      {
         return(1);
      }
   }
   fprintf(stderr, "checke: face %d not found!\n", FacFaceNo(f));
   MSD_lowListFace(stdout, f, 3);
   MSD_lowListSolid(stdout, SheSSolids(d), 3);
   return(0);
}

int findd(SPTYPE s, DPTYPE d)
{
   DPTYPE dd;

   assert(s != SNIL);
   assert(SolSShells(s) != DNIL);

   for (AllShellsSolid(s, dd))
   {
      if (dd == d)
      {
         return(1);
      }
   }
   fprintf(stderr, "checke: shell %d not found!\n", SheShellNo(d));
   MSD_lowListSolid(stdout, s, 101);
   return(0);
}

int findv(DPTYPE d, VPTYPE v)
{
   VPTYPE vtx;

#ifdef __VIRTUAL_MEM
   MSD_checkVertex(v);
   MSD_checkShell(d);
#endif

   assert(d != DNIL);
   assert(SheSVerts(d) != VNIL);

   for (AllVerticesShell(d, vtx))
   {
      if (vtx == v)
      {
         return(1);
      }
   }
   fprintf(stderr, "checke: vertex %d not found in shell %d!\n",
           VerVertexNo(v), SheShellNo(d));
   MSD_lowListSolid(stdout, SheSSolids(d), 3);
   return(0);
}

int checkv(SPTYPE s)
{
   DPTYPE d;
   VPTYPE v;
   HPTYPE he;
   int    maxtry;

#ifdef __VIRTUAL_MEM
   MSD_checkSolid(s);
#endif

   assert(s != SNIL);
   assert(SolSShells(s) != DNIL);
   assert(SheSVerts(SolSShells(s)) != VNIL);

   for (AllShellsSolid(s, d))
   {
      assert(SheSSolids(d) == s);
      for (AllVerticesShell(d, v))
      {
         if ((he = VerVEdge(v)) != HNIL)
         {
            maxtry = 100;
            do
            {
               assert(HalEdg(he) != ENIL);
               assert(HalVtx(he) == v);
               assert(maxtry-- > 0);
            } while ((he = HalNxt(mate(he))) != VerVEdge(v));
         }
      }
   }
   return(SUCCESS);
}
