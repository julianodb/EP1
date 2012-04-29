/* 
  PROJETO USPDesigner
  MODULO: SET (Operadores Booleanos)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: setopgen.cpp
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
#include "lowgeome.h"
#include "vectorop.h"
#include "sethmain.h"
#include "splsplit.h"

/* generation of relevant vertices */
void MSD_lowSetOpGenerate(SPTYPE a, SPTYPE b)
{
   DPTYPE da;
   DPTYPE db;
   EPTYPE e;
   BTYPE  ebox;
   int    i, j, k;

#ifdef TRACE_BOOLEAN
   fprintf(trace, "*************************************************\n");
   fprintf(trace, "*                                               *\n");
   fprintf(trace, "*                 SETOPGENERATE                 *\n");
   fprintf(trace, "*                                               *\n");
   fprintf(trace, "*************************************************\n");
   fprintf(trace, "\n");
#endif

   nvvtx = nvtxa = nvtxb = 0;
   if (MSD_lowBoxBoxIntersection(&(SolBox(a)), &(SolBox(b))))
   {
      for (AllShellsSolid(a, da))
      {
         if (MSD_lowBoxBoxIntersection(&(SheBox(da)), &(SolBox(b))))
         {
            for (AllEdgesShell(da, e))
            {
               MSD_lowEdgeBox(&ebox, e);
               if (MSD_lowBoxBoxIntersection(&ebox, &(SolBox(b))))
               {
                  MSD_lowSetOpProcessEdge(e, SheSFaces(SolSShells(b)), SolSShells(b), 0);
               }
            }
         }
      }
      for (AllShellsSolid(b, db))
      {
         if (MSD_lowBoxBoxIntersection(&(SheBox(db)), &(SolBox(a))))
         {
            for (AllEdgesShell(db, e))
            {
               MSD_lowEdgeBox(&ebox, e);
               if (MSD_lowBoxBoxIntersection(&ebox, &(SolBox(a))))
               {
                  MSD_lowSetOpProcessEdge(e, SheSFaces(SolSShells(a)), SolSShells(a), 1);
               }
            }
         }
      }
   }

   for (i = 0; i < nvvtx; i++)
   {
      for (j = 0; j < nvtxa; j++)
      {
         if (SetVVva(i) == SetAVFv(j))
         {
            nvtxa--;
            j--;
            for (k = j + 1; k < nvtxa; k++)
            {
               SetAVFv(k) = SetAVFv(k + 1);
               SetAVFf(k) = SetAVFf(k + 1);
               fprintf(stderr, "*");
            }
         }
      }
      for (j = 0; j < nvtxb; j++)
      {
         if (SetVVvb(i) == SetBVFv(j))
         {
            nvtxb--;
            j--;
            for (k = j + 1; k < nvtxb; k++)
            {
               SetBVFv(k) = SetBVFv(k + 1);
               SetBVFf(k) = SetBVFf(k + 1);
               fprintf(stderr, "*");
            }
         }
      }
   }

#ifdef TRACE_BOOLEAN
   fprintf(trace, "*** setopgenerate *** - nvvtx = %3i\n", nvvtx);
   fprintf(trace, "*** setopgenerate *** - nvtxa = %3i\n", nvtxa);
   fprintf(trace, "*** setopgenerate *** - nvtxb = %3i\n", nvtxb);
   fprintf(trace, "*** estrutura sonvv ***\n");
   for (i = 0; i < nvvtx; i++)
   {
      trace_v(SetVVva(i));
      trace_v(SetVVvb(i));
   }
   fprintf(trace, "*** estrutura sonva ***\n");
   for (i = 0; i < nvtxa; i++)
   {
      trace_v(SetAVFv(i));
      trace_f(SetAVFf(i));
   }
   fprintf(trace, "*** estrutura sonvb ***\n");
   for (i = 0; i < nvtxb; i++)
   {
      trace_v(SetBVFv(i));
      trace_f(SetBVFf(i));
   }
#endif
}

void MSD_lowSetOpProcessEdge(EPTYPE e, FPTYPE fini, DPTYPE dini, int BvxA)
{
   DPTYPE d;
   FPTYPE f;
   BTYPE  ebox;

   MSD_lowEdgeBox(&ebox, e);
   for (d = dini, f = fini; d != DNIL; d = SheNextD(d),
        f = d != DNIL ? SheSFaces(d) : FNIL)
   {
      if (MSD_lowBoxBoxIntersection(&ebox, &(SheBox(d))))
      {
         for ( ; f != FNIL; f = FacNextF(f))
         {
            if (MSD_lowBoxBoxIntersection(&ebox, &(FacBox(f))))
            {
               MSD_lowSetOpDoGenerate(e, f, BvxA);
            }
         }
      }
   }
}

/* edge-face comparison */
void MSD_lowSetOpDoGenerate(EPTYPE e, FPTYPE f, int BvxA)
{
   VPTYPE v1;
   VPTYPE v2;
   double d1, d2, t, x, y, z;
   int    s1, s2, cont;

#ifdef TRACE_BOOLEAN1
   fprintf(trace, "\n");
   if (BvxA == 0)
   {
      fprintf(trace, "*** Aresta de Objeto A e Face de Objeto B\n");
   }
   if (BvxA == 1)
   {
      fprintf(trace, "*** Aresta de Objeto B e Face de Objeto A\n");
   }
   trace_e(e);
   trace_f(f);
#endif

   v1 = HalVtx(EdgHe1(e));
   v2 = HalVtx(EdgHe2(e));
   s1 = comp((d1 = MSD_lowDistancePlanePoint(FacFeq(f), v1)), 0.0, EPS);
   s2 = comp((d2 = MSD_lowDistancePlanePoint(FacFeq(f), v2)), 0.0, EPS);
   if (((s1 == -1) && (s2 == 1)) || ((s1 == 1) && (s2 == -1)))
   {
      t = d1 / (d1 - d2);
      x = VerVCoord(v1)[0] + t * (VerVCoord(v2)[0] - VerVCoord(v1)[0]);
      y = VerVCoord(v1)[1] + t * (VerVCoord(v2)[1] - VerVCoord(v1)[1]);
      z = VerVCoord(v1)[2] + t * (VerVCoord(v2)[2] - VerVCoord(v1)[2]);
      if (MSD_lowVertexInBox(&(FacBox(f)), x, y, z))
      {
         if ((cont = MSD_lowSetOpFacPntContainment(f, x, y, z)) > 0)
         {
            MSD_lowMEV(EdgHe1(e), HalNxt(EdgHe2(e)), maxv++, x, y, z);

#ifdef TRACE_BOOLEAN1
            if (cont == 1)
            {
               fprintf(trace, "*** Ocorre interseccao aresta-face\n");
               MSD_lowSetOpAddSOVF(HalVtx(EdgHe1(e)), f, BvxA);
            }
#else
            if (cont == 1)
            {
               MSD_lowSetOpAddSOVF(HalVtx(EdgHe1(e)), f, BvxA);
            }
#endif

            if (cont == 2)
            {
#ifdef TRACE_BOOLEAN1
               fprintf(trace, "*** Ocorre interseccao aresta-aresta\n");
#endif
               MSD_lowMEV(hithe, HalNxt(mate(hithe)), maxv++, x, y, z);
               MSD_lowSetOpAddSOVV(HalVtx(EdgHe1(e)), HalVtx(hithe), BvxA);
            }

#ifdef TRACE_BOOLEAN1
            if (cont == 3)
            {
               fprintf(trace, "*** Ocorre interseccao aresta-vertice\n");
               MSD_lowSetOpAddSOVV(HalVtx(EdgHe1(e)), hitvertex, BvxA);
            }
#else
            if (cont == 3)
            {
               MSD_lowSetOpAddSOVV(HalVtx(EdgHe1(e)), hitvertex, BvxA);
            }
#endif
            MSD_lowSetOpProcessEdge(HalEdg(HalPrv(EdgHe1(e))), f, FacFShell(f), BvxA);
         }

#ifdef TRACE_BOOLEAN1
         if (cont == 0)
         {
            fprintf(trace, "*** Nao ocorre interseccao\n");
         }
#endif
      }
   }
   else
   {
      if (s1 == 0)
      {
         MSD_lowSetOpDoVertexOnFace(v1, f, BvxA);
      }
      if (s2 == 0)
      {
         MSD_lowSetOpDoVertexOnFace(v2, f, BvxA);
      }
#ifdef TRACE_BOOLEAN1
      if ((s1 != 0) && (s2 != 0))
      {
         fprintf(trace, "*** Nao ocorre interseccao\n");
      }
#endif
   }
}

/* Vertex-face comparison */
void MSD_lowSetOpDoVertexOnFace(VPTYPE v, FPTYPE f, int BvxA)
{
   int cont;

   if (MSD_lowVertexInBox(&(FacBox(f)), VerVCoord(v)[0],
                          VerVCoord(v)[1],
                          VerVCoord(v)[2]))
   {
      cont = MSD_lowFacVtxContainment(f, v);
   }
   else
   {
      cont = 0;
   }

#ifdef TRACE_BOOLEAN1
   if (cont == 1)
   {
      fprintf(trace, "*** Ocorre interseccao aresta-face\n");
      MSD_lowSetOpAddSOVF(v, f, BvxA);
   }
#else
   if (cont == 1)
   {
      MSD_lowSetOpAddSOVF(v, f, BvxA);
   }
#endif

   if (cont == 2)
   {
#ifdef TRACE_BOOLEAN1
      fprintf(trace, "*** Ocorre interseccao aresta-aresta\n");
#endif
      MSD_lowMEV(hithe, HalNxt(mate(hithe)), maxv++,
                 VerVCoord(v)[0],
                 VerVCoord(v)[1],
                 VerVCoord(v)[2]);
      MSD_lowSetOpAddSOVV(v, HalVtx(hithe), BvxA);
   }

#ifdef TRACE_BOOLEAN1
   if (cont == 3)
   {
      fprintf(trace, "*** Ocorre interseccao aresta-vertice\n");
      MSD_lowSetOpAddSOVV(v, hitvertex, BvxA);
   }
#else
   if (cont == 3)
   {
      MSD_lowSetOpAddSOVV(v, hitvertex, BvxA);
   }
#endif

#ifdef TRACE_BOOLEAN1
   if (cont == 0)
   {
      fprintf(trace, "*** Nao ocorre interseccao\n");
   }
#endif
}

void MSD_lowSetOpAddSOVF(VPTYPE v, FPTYPE f, int flag)
{
   int i;

#ifdef TRACE_BOOLEAN1
   if (flag == 0)
   {
      fprintf(trace, "*** Vertice Objeto A e Face Objeto B\n");
   }
   if (flag == 1)
   {
      fprintf(trace, "*** Vertice Objeto B e Face Objeto A\n");
   }
   trace_v(v);
   trace_f(f);
#endif

   if (flag == 0)
   {
      for (i = 0; i < nvtxa; ++i)

#ifdef TRACE_BOOLEAN1
      {
         if ((SetAVFv(i) == v) && (SetAVFf(i) == f))
         {
            fprintf(trace,
                    "*** Nao foi acrescentado a estrutura - (sonva) repetido\n");
            return;
         }
      }
#else
      {
         if ((SetAVFv(i) == v) && (SetAVFf(i) == f))
         {
            return;
         }
      }
#endif

      SetAVFv(nvtxa) = v;
      SetAVFf(nvtxa) = f;
      nvtxa++;

#ifndef __VIRTUAL_MEM
      if (nvtxa == MAXONVERTICES)
      {
         fprintf(stderr, "sonva: falta memoria!\n");
         exit(1);
      }
#endif
   }
   else
   {
      for (i = 0; i < nvtxb; ++i)

#ifdef TRACE_BOOLEAN1
      {
         if ((SetBVFv(i) == v) && (SetBVFf(i) == f))
         {
            fprintf(trace,
                    "*** Nao foi acrescentado a estrutura - (sonvb) repetido\n");
            return;
         }
      }
#else
      {
         if ((SetBVFv(i) == v) && (SetBVFf(i) == f))
         {
            return;
         }
      }
#endif

      SetBVFv(nvtxb) = v;
      SetBVFf(nvtxb) = f;
      nvtxb++;

#ifndef __VIRTUAL_MEM
      if (nvtxb == MAXONVERTICES)
      {
         fprintf(stderr, "sonvb: falta memoria!\n");
         exit(1);
      }
#endif
   }
}

void MSD_lowSetOpAddSOVV(VPTYPE v1, VPTYPE v2, int flag)
{
   int i;

#ifdef TRACE_BOOLEAN1
   if (flag == 0)
   {
      fprintf(trace, "*** Vertice Objeto A e Vertice Objeto B\n");
   }
   if (flag == 1)
   {
      fprintf(trace, "*** Vertice Objeto B e Vertice Objeto A\n");
   }
   trace_v(v1);
   trace_v(v2);
#endif

   if (flag == 0)
   {
      for (i = 0; i < nvvtx; ++i)

#ifdef TRACE_BOOLEAN1
      {
         if ((SetVVva(i) == v1) && (SetVVvb(i) == v2))
         {
            fprintf(trace,
                    "*** Nao foi acrescentado a estrutura - (sonvv) repetido\n");
            return;
         }
      }
#else
      {
         if ((SetVVva(i) == v1) && (SetVVvb(i) == v2))
         {
            return;
         }
      }
#endif

      SetVVva(nvvtx) = v1;
      SetVVvb(nvvtx) = v2;
      nvvtx++;

#ifndef __VIRTUAL_MEM
      if (nvvtx == MAXONVERTICES)
      {
         fprintf(stderr, "sonvv: falta memoria!\n");
         exit(1);
      }
#endif
   }
   else
   {
      for (i = 0; i < nvvtx; ++i)

#ifdef TRACE_BOOLEAN1
      {
         if ((SetVVva(i) == v2) && (SetVVvb(i) == v1))
         {
            fprintf(trace,
                    "*** Nao foi acrescentado a estrutura - (sonvv) repetido\n");
            return;
         }
      }
#else
      {
         if ((SetVVva(i) == v2) && (SetVVvb(i) == v1))
         {
            return;
         }
      }
#endif

      SetVVva(nvvtx) = v2;
      SetVVvb(nvvtx) = v1;
      nvvtx++;

#ifndef __VIRTUAL_MEM
      if (nvvtx == MAXONVERTICES)
      {
         fprintf(stderr, "sonvv: falta memoria!\n");
         exit(1);
      }
#endif
   }
}

/***    Implement  the shorthand  procedure contfp. The procedure ***/
/*** should test for the inclusion of the  point (x, y, z) in the ***/
/*** face f and return similar values as contlv.                  ***/
int MSD_lowSetOpFacPntContainment(FPTYPE f, real x, real y, real z)
{
#ifndef __VIRTUAL_MEM
   VTYPE v;

   v.vcoord[0] = x;
   v.vcoord[1] = y;
   v.vcoord[2] = z;
   return(MSD_lowFacVtxContainment(f, &v));

#else
   VPTYPE v;
   int    ip;

   v = MSD_getMemVirtual(VERTEX);

   VerVCoord(v)[0] = x;
   VerVCoord(v)[1] = y;
   VerVCoord(v)[2] = z;
   ip = MSD_lowFacVtxContainment(f, v);

   MSD_putMemVirtual(v, VERTEX);

   return(ip);
#endif

}
