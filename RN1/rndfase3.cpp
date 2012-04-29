/* 
  PROJETO USPDesigner
  MODULO: RND (Arredondamento)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: rndfase3.cpp
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
#include "lowmacro.h"
#include "lowparam.h"
#include "lowsolid.h"
#include "eulerops.h"
#include "vectorop.h"
#include "mancommd.h"
#include "rndround.h"

int GenerationCurved_edges(SPTYPE s)
{
   EdgesAttachedOriginal_vertex(s);
   EdgeAttached_vertexfase1(s);
   return(SUCCESS);
}

int EdgesAttachedOriginal_vertex(SPTYPE s)
{
   DPTYPE d;
   VPTYPE v;
   VPTYPE v1;
   EPTYPE e;
   int    n0, nt, n1, flag;

   for (AllShellsSolid(s, d))
   {
      for (AllEdgesShell(d, e))
      {
         if ((EdgFgRound(e) != 1) || (EdgFgMake(e) == 3))
         {
            EdgFgRound(e) = 0;
         }
      }
   }

   for (AllShellsSolid(s, d))
   {
      for (v = SheSVerts(d); v != VNIL;)
      {
         if (VerFgMake(v) == 0)
         {
            flag = 0;
            nt   = GetEdges(v, -1);
            n0   = GetEdges(v, 0);
            n1   = GetEdges(v, 1);

//#ifdef __TRACE_ROUND__
            printf("v = (%f,%f,%f) n0=%d n1=%d nt=%d\n", VerVCoord(v)[0],
                   VerVCoord(v)[1],
                   VerVCoord(v)[2], n0, n1, nt);
//#endif

            if (n0 == nt)
            {
               v1   = v;
               v    = VerNextV(v);
               flag = 1;
               if (edv1(v1) == v)
               {
                  v = VerNextV(v1);
               }
            }
            if ((n0 == 1) && (nt > 3))
            {
               v1   = v;
               v    = VerNextV(v);
               flag = 1;
               edv3(s, v1);
            }
            if (n1 == 2)
            {
               v1   = v;
               v    = VerNextV(v);
               flag = 1;
               if (edv2(s, v1) == v)
               {
                  v = VerNextV(v1);
               }
            }
            if ((nt == 3) && (n0 == 2) && (n1 == 1))
            {
               v1   = v;
               v    = VerNextV(v);
               flag = 1;
               edv4(s, v1);
            }
            if (flag == 0)
            {
               v = VerNextV(v);
            }
         }
         else
         {
            v = VerNextV(v);
         }
      }
   }
   printf("[saiu]");
   return(SUCCESS);
}

VPTYPE edv1(VPTYPE v)
{
   HPTYPE he;
   HPTYPE he1;
   HPTYPE heaux;

#ifdef __TRACE_ROUND__
   printf("iedv1\n");
#endif

   he = heaux = VerVEdge(v);
   do
   {
      if (he != heaux)
      {
         he1 = HalNxt(mate(he));
         MSD_lowKEF(he, mate(he));
         he = he1;
      }
      else
      {
         he = HalNxt(mate(he));
      }
   } while (he != heaux);

   MSD_lowKEV(he, mate(he));
   return(v);
}

VPTYPE edv2(SPTYPE s, VPTYPE v)
{
   HPTYPE he;
   HPTYPE he2;
   HPTYPE he1;
   VPTYPE v1;
   vector p, vaux;

#ifdef __TRACE_ROUND__
   printf("iedv2\n");
#endif

   he = he2 = VerVEdge(v);
   do
   {
      if (EdgFgRound(HalEdg(he)) == 1)
      {
         he2 = he;
         break;
      }
   } while ((he = HalNxt(mate(he))) != VerVEdge(v));

   he = he2;
   do
   {
      printf("!");
      if (EdgFgRound(HalEdg(he)) == 0)
      {
         he1 = HalNxt(mate(he));
         MSD_lowKEF(he, mate(he));
         he = he1;
      }
      else
      {
         he = HalNxt(mate(he));
      }
   } while (he != he2);

   veccopy(p, VerVCoord(HalVtx(he)));
   VerFgMake(HalVtx(he)) = 3;
   he2 = HalPrv(he);
   MSD_lowNewVertexLocation(s, HalVtx(he), VerVCoord(HalVtx(he2))[0],
                            VerVCoord(HalVtx(he2))[1],
                            VerVCoord(HalVtx(he2))[2]);

   v1 = HalVtx(he2);
   MSD_lowKEV(he2, mate(he2));
   veccopy(vaux, VerVCoord(HalVtx(he)));
   CLC(s, he, vaux, VerVCoord(HalVtx(mate(he))), p);

   return(v1);
}

VPTYPE edv3(SPTYPE s, VPTYPE v)
{
   HPTYPE he;
   HPTYPE heaux;
   HPTYPE he1;
   HPTYPE he2;
   vector vaux, vau1;

#ifdef __TRACE_ROUND__
   printf("iedv3\n");
#endif

   maxf  = MSD_getMaxFaceId(s);
   heaux = VerVEdge(v);
   he    = heaux;
   do
   {
      if (EdgFgRound(HalEdg(he)) == 0)
      {
         heaux = he;
         he    = VerVEdge(v);
      }
      else
      {
         he = HalNxt(mate(he));
      }
   } while (he != VerVEdge(v));
   he1 = HalPrv(heaux);
   MSD_lowKEF(heaux, mate(heaux));
   he2 = HalNxt(HalNxt(he1));
   MSD_lowMEF(he1, he2, ++maxf);
   veccopy(vaux, VerVCoord(HalVtx(he1)));
   veccopy(vau1, VerVCoord(HalVtx(HalPrv(he1))));
   CLC(s, HalPrv(he1), vaux, vau1, VerVCoord(HalVtx(HalNxt(he1))));

   return(v);
}

VPTYPE edv4(SPTYPE s, VPTYPE v)
{
   HPTYPE he;
   HPTYPE he1;
   HPTYPE he2;
   vector vaux, vau1;

#ifdef __TRACE_ROUND__
   printf("iedv4\n");
#endif

   he = he1 = VerVEdge(v);
   do
   {
      if (EdgFgRound(HalEdg(he)) == 1)
      {
         he1 = he;
         break;
      }
   } while ((he = HalNxt(mate(he))) != VerVEdge(v));

   he2 = HalPrv(mate(he1));
   he  = he1;
   he1 = HalNxt(HalNxt(he1));
   MSD_lowKEF(he, mate(he));
   MSD_lowMEF(he2, he1, ++maxf);
   veccopy(vaux, VerVCoord(HalVtx(he2)));
   veccopy(vau1, VerVCoord(HalVtx(HalPrv(he2))));
   CLC(s, HalPrv(he1), vaux, vau1, VerVCoord(HalVtx(HalNxt(he2))));
   he = VerVEdge(v);
   MSD_lowKEF(HalNxt(mate(he)), mate(HalNxt(mate(he))));
   MSD_lowKEV(VerVEdge(v), mate(VerVEdge(v)));
   return(v);
}

int EdgeAttached_vertexfase1(SPTYPE s)
{
   DPTYPE d;
   EPTYPE e;
   EPTYPE eb;
   VPTYPE v;
   VPTYPE vb;
   vector p, p_aux, vaux, paux;

   for (AllShellsSolid(s, d))
   {
      for (e = SheSEdges(d); e != ENIL;)
      {
         printf("[");
         if ((VerFgMake(HalVtx(EdgHe1(e))) == 1) &&
             (VerFgMake(HalVtx(EdgHe2(e))) == 1) &&
             ((EdgFgMake(e) == 0) || (EdgFgMake(e) == 1)) &&
             (EdgFgRound(e) != 1))
         {
            eb = e;
            e  = EdgNextE(e);
            MSD_lowKEF(EdgHe1(eb), EdgHe2(eb));
         }
         else
         {
            e = EdgNextE(e);
         }
      }
   }

   for (AllShellsSolid(s, d))
   {
      for (v = SheSVerts(d); v != VNIL;)
      {
         printf("(");
         if ((VerFgMake(v) == 1) && (GetEdges(v, -1) == 2))
         {
            veccopy(p, VerVCoord(v));
            veccopy(p_aux, VerVCoord(HalVtx(HalNxt(VerVEdge(v)))));
            MSD_lowNewVertexLocation(s, v, p_aux[0], p_aux[1], p_aux[2]);
            veccopy(vaux, VerVCoord(v));
            veccopy(paux, VerVCoord(HalVtx(HalPrv(VerVEdge(v)))));
            CLC(s, HalPrv(VerVEdge(v)), vaux, paux, p);
            vb = v;
            v  = VerNextV(v);
            MSD_lowKEV(VerVEdge(vb), mate(VerVEdge(vb)));
         }
         else
         {
            v = VerNextV(v);
         }
      }
   }

   for (AllShellsSolid(s, d))
   {
      for (v = SheSVerts(d); v != VNIL;)
      {
         printf("{");
         if (GetEdges(v, -1) == 2)
         {
            veccopy(p, VerVCoord(v));
            veccopy(p_aux, VerVCoord(HalVtx(HalNxt(VerVEdge(v)))));
            MSD_lowNewVertexLocation(s, v, p_aux[0], p_aux[1], p_aux[2]);
            veccopy(vaux, VerVCoord(v));
            veccopy(paux, VerVCoord(HalVtx(HalPrv(VerVEdge(v)))));
            CLC(s, HalPrv(VerVEdge(v)), vaux, paux, p);
            vb = v;
            v  = VerNextV(v);
            printf("ke");
            MSD_lowKEV(VerVEdge(vb), mate(VerVEdge(vb)));
            printf("v");
         }
         else
         {
            v = VerNextV(v);
         }
      }
   }
   return(SUCCESS);
}
