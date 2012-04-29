/* 
  PROJETO USPDesigner
  MODULO: RND (Arredondamento)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: rndfase2.cpp
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
#include <math.h>
#include "memvirtu.h"
#include "lowmacro.h"
#include "lowparam.h"
#include "lowsolid.h"
#include "eulerops.h"
#include "vectorop.h"
#include "mancommd.h"
#include "rndround.h"

int EdgeGeneration_faces(SPTYPE s)
{
   GenerationEdges_corners(s);
   GenerationEdges_nvertices(s);
   return(SUCCESS);
}

/* -------------------------------------------------------------------------- */
/* Algoritmo descrito na pagina 167 do livro Solid Modeling with Designbase   */
int GenerationEdges_corners(SPTYPE s)
{
   DPTYPE d;
   FPTYPE f;
   LPTYPE l;
   HPTYPE he;
   HPTYPE heaux;
   HPTYPE he3;
   real   th;
   int    flag;

   for (AllShellsSolid(s, d))
   {
      for (AllFacesShell(d, f))
      {
         for (AllLoopsFace(f, l))
         {
            he = he3 = LooLEdg(l);
            printf("*");
            do
            {
               if (VerFgMake(HalVtx(he)) == 1)
               {
                  he3 = he;
                  printf("ok");
                  break;
               }
            } while ((he = HalNxt(he)) != LooLEdg(l));

            he = he3;
            do
            {
               printf("<");
               flag = 0;
               if (VerFgMake(HalVtx(he)) == MSD_ROUND_V_ORIGINAL)
               {
                  printf(":");
                  heaux = he;
                  he    = HalNxt(he);
                  th    = angle_edge(heaux);
                  if (comp(th, PI, EPS) == -1)
                  {
                     printf("<1");
                     flag = 1;
                     MakeEdgeConvexCorner(s, heaux);
                  }
                  if (comp(th, PI, EPS) == 0)
                  {
                     printf("<2");
                     flag = 1;
                     MakeEdgeFlatCorner(s, heaux);
                  }
                  if (comp(th, PI, EPS) == 1)
                  {
                     printf("<3");
                     flag = 1;
                     MakeEdgeConcaveCorner(s, heaux);
                  }
                  if (flag == 0)
                  {
                     he = HalNxt(he);
                  }
               }
               else
               {
                  he = HalNxt(he);
               }
            } while (HalVtx(he) != HalVtx(he3));
         }
      }
   }

   printf("[saiu]");
   return(TRUE);
}

int GenerationEdges_nvertices(SPTYPE s)
{
   DPTYPE d;
   FPTYPE f;
   LPTYPE l;
   EPTYPE ee;
   HPTYPE he;
   HPTYPE he1;
   HPTYPE he2;
   HPTYPE heloop;
   VPTYPE v;
   int    flag, cont, VFgM, EFgM, EFgR;
   vector p1, p2, p3, p4, n, vaux, vau1;

   maxv = MSD_getMaxVertexId(s);
   maxf = MSD_getMaxFaceId(s);
   for (AllShellsSolid(s, d))
   {
      for (AllFacesShell(d, f))
      {
         for (AllLoopsFace(f, l))
         {
            printf("$");
#ifdef __TRACE_ROUND__
            printf("face = %d\n", FacFaceNo(f));
#endif

            he1 = he2 = LooLEdg(l);
            do
            {
               ee   = HalEdg(he1);
               VFgM = VerFgMake(HalVtx(he1));
               EFgM = EdgFgMake(ee);
               EFgR = EdgFgRound(ee);
               if (((VFgM == 1) && (EFgR == 1) && (EFgM != 2)) || (VFgM == 2))
               {
                  he2 = he1;
                  break;
               }
            } while ((he1 = HalNxt(he1)) != LooLEdg(l));

            flag = 0;
            he1  = he2;
            ee   = HalEdg(he1);
            VFgM = VerFgMake(HalVtx(he1));
            EFgM = EdgFgMake(ee);
            EFgR = EdgFgRound(ee);

            if (((VFgM == 1) && (EFgR == 1) && (EFgM != 2)) || (VFgM == 2))
            {
               cont = 0;
               if ((((EFgR == 1) && (EFgM == 1) && (EFgM != 2)) || (VFgM == 2)) &&
                   (EdgFgRound(HalEdg(HalNxt(he1))) != 1))
               {
                  flag = 1;
               }

               v = HalVtx(he1);
               do
               {
                  ee   = HalEdg(he2);
                  EFgM = EdgFgMake(ee);
                  EFgR = EdgFgRound(ee);
                  if ((EFgR != 1) && (EFgM == 0))
                  {
                     cont = 1;
                  }

                  he2  = HalNxt(he2);
                  ee   = HalEdg(he2);
                  VFgM = VerFgMake(HalVtx(he2));
                  EFgM = EdgFgMake(ee);
                  EFgR = EdgFgRound(ee);
                  if ((((VFgM == 1) && (EFgR == 1) && (EFgM != 2)) || (VFgM == 2)) &&
                      (flag == 1) &&
                      (HalPrv(HalPrv(he2)) != he1) && (he1 != he2) &&
                      (HalNxt(he2) != he1) && (cont == 1))
                  {
                     if (VFgM == 1)
                     {
                        if ((VerFgMake(HalVtx(HalNxt(he2))) == 2) &&
                            (he1 != HalNxt(he2)) &&
                            (HalNxt(HalNxt(he2)) != he1))
                        {
                           he2 = HalNxt(he2);
                        }
                     }
#ifdef __TRACE_ROUND__
                     printf("criou nova face!\n");
                     printf("v1=(%.2f,%.2f,%.2f) v2=(%.2f,%.2f,%.2f)\n",
                            VerVCoord(HalVtx(he1))[0],
                            VerVCoord(HalVtx(he1))[1],
                            VerVCoord(HalVtx(he1))[2],
                            VerVCoord(HalVtx(he2))[0],
                            VerVCoord(HalVtx(he2))[1],
                            VerVCoord(HalVtx(he2))[2]);
#endif
                     /* MEF */
                     MSD_lowMEF(he1, he2, ++maxf);
                     he             = HalPrv(he2);
                     ee             = HalEdg(he);
                     EdgFgMake(ee)  = MSD_ROUND_E_FASE2;
                     EdgFgRound(ee) = MSD_ROUND_FLAG_E1;
                     flag           = 0;
                     cont           = 0;
                     he1            = he2;
                  }

                  if ((((VFgM == 1) && (EFgR == 1) && (EFgM != 2)) || (VFgM == 2)) && (cont == 0))
                  {
                     he1  = he2;
                     flag = 1;
                  }
                  if (EFgR == 0)
                  {
                     cont = 1;
                  }
                  if ((VFgM != 1) || (EFgR != 1) || (VFgM != 2))
                  {
                     he = HalPrv(he2);
                     ee = HalEdg(he);
                     if ((EdgFgRound(ee) != 1) &&
                         (EdgFgMake(ee) == MSD_ROUND_E_ORIGINAL))
                     {
                        flag = 1;
                     }
                  }
               } while (HalVtx(he2) != v);
            }
         }
      }
   }

   for (AllShellsSolid(s, d))
   {
      for (AllFacesShell(d, f))
      {
         for (AllLoopsFace(f, l))
         {
            printf("!");
            he = heloop = LooLEdg(l);
            do
            {
               printf("+");
               if ((VerFgMake(HalVtx(he)) == 1) &&
                   (EdgFgRound(HalEdg(he)) != 1) &&
                   (comp(angle_edge(he), PI, EPS) == 0))
               {
                  veccopy(vaux, VerVCoord(HalVtx(HalPrv(HalPrv(he)))));
                  if (vecequal(vaux, VerVCoord(HalVtx(HalPrv(he)))) != 1)
                  {
                     veccopy(vaux, VerVCoord(HalVtx(he)));
                     vecminus(p3, vaux, VerVCoord(HalVtx(mate(he))));
                     veccopy(vaux, VerVCoord(HalVtx(HalPrv(HalPrv(he)))));
                     vecminus(p4, vaux, VerVCoord(HalVtx(HalPrv(he))));
                     cross(n, p4, p3);
                     normalize(n);
                  }
                  else
                  {
                     veccopy(vaux, VerVCoord(HalVtx(HalNxt(HalNxt(he)))));
                     vecminus(p3, vaux, VerVCoord(HalVtx(HalNxt(he))));
                     veccopy(vaux, VerVCoord(HalVtx(HalNxt(he))));
                     vecminus(p4, vaux, VerVCoord(HalVtx(he)));
                     cross(n, p4, p3);
                     normalize(n);
                  }

                  veccopy(vaux, VerVCoord(HalVtx(he)));
                  point_perp(vaux, VerVCoord(HalVtx(mate(he))), p1, n, 1);
                  veccopy(vau1, VerVCoord(HalVtx(HalPrv(HalPrv(he)))));
                  line_inter(vaux, p1, vau1,
                             VerVCoord(HalVtx(HalPrv(HalPrv(HalPrv(he))))), p2);
                  if (cont_point(HalVtx(HalPrv(HalPrv(he))),
                                 HalVtx(HalPrv(HalPrv(HalPrv(he)))), p2) == 1)
                  {
                     printf("1");
                     MSD_lowMEV(HalPrv(HalPrv(HalPrv(he))),
                                HalNxt(mate(HalPrv(HalPrv(HalPrv(he))))),
                                ++maxv, p2[0],
                                p2[1],
                                p2[2]);
                     VerFgMake(HalVtx(HalPrv(HalPrv(HalPrv(he))))) = MSD_ROUND_V_FASE2;
                     EdgFgMake(HalEdg(HalPrv(HalPrv(HalPrv(he))))) = MSD_ROUND_E_FASE2;
                     MSD_lowMEF(HalPrv(HalPrv(HalPrv(he))), he, ++maxf);
                     EdgFgMake(HalEdg(HalPrv(he))) = MSD_ROUND_E_FASE2;
                     break;        //heloop = he;
                  }
                  else
                  {
                     printf("2");
                     MSD_lowMEV(HalPrv(HalPrv(HalPrv(HalPrv(he)))),
                                HalNxt(mate(HalPrv(HalPrv(HalPrv(HalPrv(he)))))),
                                ++maxv, p2[0],
                                p2[1],
                                p2[2]);
                     VerFgMake(HalVtx(HalPrv(HalPrv(HalPrv(HalPrv(he)))))) = MSD_ROUND_V_FASE2;
                     EdgFgMake(HalEdg(HalPrv(HalPrv(HalPrv(HalPrv(he)))))) = MSD_ROUND_E_FASE2;
                     MSD_lowMEF(HalPrv(HalPrv(HalPrv(HalPrv(he)))), he, ++maxf);
                     EdgFgMake(HalEdg(HalPrv(he))) = MSD_ROUND_E_FASE2;
                     break;        //heloop = he;
                  }
               }
               he = HalNxt(he);
            } while (he != heloop);
         }
      }
   }
   return(TRUE);
}

/* ************************************************************************** */
/* Possui apenas funcoes de tipo MEV.                                         */
void MakeEdgeFlatCorner(SPTYPE s, HPTYPE he)
{
   vector n, p1, p2, pm, vaux, vmhe;
   HPTYPE prev  = HalPrv(he);
   HPTYPE mateh = mate(he);
   EPTYPE edge  = HalEdg(he);
   EPTYPE edgp  = HalEdg(prev);

   printf("MakeEdgeFlatCorner\n");
   maxv = MSD_getMaxVertexId(s);
   veccopy(n, FacFeq(LooLFace(HalWLoop(he))));
   veccopy(vaux, VerVCoord(HalVtx(he)));
   veccopy(vmhe, VerVCoord(HalVtx(mateh)));
   if (EdgFgMake(edge) < 2)
   {
      if ((EdgFgRound(edge) != 1) && (EdgFgRound(edgp) == 1))
      {
         point_perp(vaux, vmhe, pm, n, HalR(he));
         MSD_lowMEV(he, he, ++maxv, pm[0], pm[1], pm[2]);
         prev = HalPrv(he);                       /* alterado pela funcao MEV */
         edgp = HalEdg(prev);
         VerFgMake(HalVtx(prev)) = MSD_ROUND_V_FASE2;
         EdgFgMake(edgp)         = MSD_ROUND_E_FASE2;
         EdgFgRound(edgp)        = 1;
      }

      if ((EdgFgRound(edgp) != 1) && (EdgFgRound(edge) == 1))
      {
         point_perp(vaux, vmhe, pm, n, HalR(mate(prev)));
         MSD_lowMEV(he, he, ++maxv, pm[0], pm[1], pm[2]);
         prev = HalPrv(he);                       /* alterado pela funcao MEV */
         edgp = HalEdg(prev);
         VerFgMake(HalVtx(prev)) = MSD_ROUND_V_FASE2;
         EdgFgMake(edgp)         = MSD_ROUND_E_FASE2;
         EdgFgRound(edgp)        = 1;
      }

      if ((EdgFgRound(edge) != 1) && (EdgFgRound(edgp) != 1))
      {
         point_perp(vaux, vmhe, p1, n, HalR(he));
         point_perp(vaux, vmhe, p2, n, HalR(mate(prev)));
         vecplus(pm, p1, p2);
         vecesc(pm, pm, 0.5);
         MSD_lowMEV(he, he, ++maxv, pm[0], pm[1], pm[2]);
         prev = HalPrv(he);                       /* alterado pela funcao MEV */
         edgp = HalEdg(prev);
         VerFgMake(HalVtx(prev)) = MSD_ROUND_V_FASE2;
         EdgFgMake(edgp)         = MSD_ROUND_E_FASE2;
         EdgFgRound(edgp)        = 1;
      }
   }
}

void MakeEdgeConcaveCorner(SPTYPE s, HPTYPE he)
{
   vector n, p1, p2, p3, p4, p_inter, p_nxt, p_prv, vaux;

   printf("MakeEdgeConcaveCorner\n");
   maxv = MSD_getMaxVertexId(s);
   if (EdgFgMake(HalEdg(he)) < 2)
   {
      if ((EdgFgRound(HalEdg(he)) != 1) && (EdgFgRound(HalEdg(HalPrv(he))) != 1))
      {
         if (VerFgRound(HalVtx(he)) == 1)
         {
#ifdef __TRACE_ROUND__
            printf("v=(%.0f,%.0f,%.0f) he.r=%f  mate.r=%f\n",
                   VerVCoord(HalVtx(he))[0],
                   VerVCoord(HalVtx(he))[1],
                   VerVCoord(HalVtx(he))[2],
                   HalR(he), HalR(mate(HalPrv(he))));
#endif
            GetNxtVertex(he, p_nxt);
            GetPrvVertex(he, p_prv);
            veccopy(n, FacFeq(LooLFace(HalWLoop(he))));
            veccopy(vaux, VerVCoord(HalVtx(he)));
            point_perp(vaux, VerVCoord(HalVtx(mate(he))), p1, n, HalR(he));
            veccopy(n, FacFeq(LooLFace(HalWLoop(he))));
            vecesc(n, n, -1);
            veccopy(vaux, VerVCoord(HalVtx(he)));
            point_perp(vaux, VerVCoord(HalVtx(HalPrv(he))), p2, n,
                       HalR(mate(HalPrv(he))));
#ifdef __TRACE_ROUND__
            printf("p1=(%.1f,%.1f,%.1f)\n", p1   [0], p1   [1], p1   [2]);
            printf("pnxt=(%.1f,%.1f,%.1f)\n", p_nxt[0], p_nxt[1], p_nxt[2]);
            printf("p2=(%.1f,%.1f,%.1f)\n", p2   [0], p2   [1], p2   [2]);
            printf("pprv=(%.1f,%.1f,%.1f)\n", p_prv[0], p_prv[1], p_prv[2]);
#endif

            line_inter(p1, p_nxt, p2, p_prv, p_inter);

#ifdef __TRACE_ROUND__
            printf("pinter=(%.1f,%.1f,%.1f)\n", p_inter[0],
                   p_inter[1],
                   p_inter[2]);
#endif

            MSD_lowMEV(he, he, ++maxv, p_inter[0], p_inter[1], p_inter[2]);
            VerFgMake(HalVtx(HalPrv(he))) = MSD_ROUND_V_FASE2;
            EdgFgMake(HalEdg(HalPrv(he))) = 3;
         }
         else
         {
            veccopy(n, FacFeq(LooLFace(HalWLoop(he))));
            veccopy(vaux, VerVCoord(HalVtx(he)));
            line_parallel(vaux, VerVCoord(HalVtx(mate(he))), p1, p2, n, HalR(he));
            line_parallel(VerVCoord(HalVtx(HalPrv(he))), vaux, p3, p4, n,
                          HalR(mate(HalPrv(he))));
            line_inter(p1, p2, p3, p4, p_inter);
            MSD_lowMEV(he, he, ++maxv, p_inter[0], p_inter[1], p_inter[2]);
            VerFgMake(HalVtx(HalPrv(he))) = MSD_ROUND_V_FASE2;
            EdgFgMake(HalEdg(HalPrv(he))) = 3;
         }
      }
      else
      {
         if ((EdgFgRound(HalEdg(he)) != 1) && (EdgFgRound(HalEdg(HalPrv(he))) == 1))
         {
            GetNxtVertex(he, p_nxt);
            veccopy(n, FacFeq(LooLFace(HalWLoop(he))));
            veccopy(vaux, VerVCoord(HalVtx(he)));
            point_perp(vaux, VerVCoord(HalVtx(mate(he))), p1, n, HalR(he));
            line_inter(p_nxt, p1, vaux, VerVCoord(HalVtx(HalPrv(he))), p_inter);
            MSD_lowMEV(he, he, ++maxv, p_inter[0], p_inter[1], p_inter[2]);
            VerFgMake(HalVtx(HalPrv(he))) = MSD_ROUND_V_FASE2;
            EdgFgMake(HalEdg(HalPrv(he))) = 3;
         }
         if ((EdgFgRound(HalEdg(he)) == 1) && (EdgFgRound(HalEdg(HalPrv(he))) != 1))
         {
            GetPrvVertex(he, p_prv);
            veccopy(n, FacFeq(LooLFace(HalWLoop(he))));
            veccopy(vaux, VerVCoord(HalVtx(he)));
            point_perp(vaux, VerVCoord(HalVtx(HalPrv(he))), p1, n,
                       HalR(mate(HalPrv(he))));
            line_inter(p1, p_prv, vaux, VerVCoord(HalVtx(mate(he))), p_inter);
            MSD_lowMEV(he, he, ++maxv, p_inter[0], p_inter[1], p_inter[2]);
            VerFgMake(HalVtx(HalPrv(he))) = MSD_ROUND_V_FASE2;
            EdgFgMake(HalEdg(HalPrv(he))) = 3;
         }
      }
   }
}

HPTYPE MakeEdgeConvexCorner(SPTYPE s, HPTYPE he)
{
   vector n, p, p_nxt, p_prv, p1, p2, p3, p4, vaux, normal, vhe, vme, vpe;
   real   r1, r2, r3;
   HPTYPE mhe;
   HPTYPE phe;

   maxv = MSD_getMaxVertexId(s);
   maxf = MSD_getMaxFaceId(s);

   if ((EdgFgRound(HalEdg(he)) != 1) &&
       (EdgFgRound(HalEdg(HalPrv(he))) != 1) && (VerFgMake(HalVtx(he)) == 0))
   {
      r1 = HalR(he);
      r2 = HalR(mate(HalPrv(he)));


      if (EdgFgMake(HalEdg(HalNxt(mate(he)))) != 3)
      {
         r3 = HalR(HalNxt(mate(he)));
      }
      else
      {
         r3 = HalR(HalNxt(mate(HalNxt(mate(he)))));
      }
      printf("MakeEdgeConvexCorner r1 = %10.4f r2 = %10.4f r3 = %10.4f\n", r1, r2, r3);

      if ((r3 > r1) && (r3 > r2) && (VerFgRound(HalVtx(he)) == 0))
      {
         veccopy(n, FacFeq(LooLFace(HalWLoop(he))));
         veccopy(vaux, VerVCoord(HalVtx(he)));
         line_parallel(vaux, VerVCoord(HalVtx(mate(he))), p1, p2, n, HalR(he));
         veccopy(vaux, VerVCoord(HalVtx(HalPrv(he))));
         line_parallel(vaux, VerVCoord(HalVtx(mate(HalPrv(he)))), p3, p4, n,
                       HalR(mate(HalPrv(he))));
         line_inter(p1, p2, p3, p4, p);
         MSD_lowMEV(he, he, ++maxv, p[0], p[1], p[2]);
         VerFgMake(HalVtx(HalPrv(he)))  = MSD_ROUND_V_FASE2;
         EdgFgMake(HalEdg(HalPrv(he)))  = 3;
         EdgFgRound(HalEdg(HalPrv(he))) = EdgFgRound(HalEdg(he));
      }

      if (((comp(r3, r1, EPS) != 1) && (comp(r3, r2, EPS) != 1)) || (VerFgRound(HalVtx(he)) == 1))
      {
         if (VerFgRound(HalVtx(he)) == 1)
         {
            printf("d");
            GetNxtVertex(he, p_nxt);
            GetPrvVertex(he, p_prv);
            veccopy(vaux, VerVCoord(HalVtx(HalPrv(he))));
            line_inter(vaux, p_nxt, VerVCoord(HalVtx(mate(he))), p_prv, p);

            if (vecequal(p, VerVCoord(HalVtx(HalPrv(HalPrv(he))))) == 0)
            {
               printf("e");
               MSD_lowMEV(HalPrv(he), HalPrv(he), ++maxv, p[0], p[1], p[2]);
               VerFgMake(HalVtx(HalPrv(HalPrv(he)))) = MSD_ROUND_V_FASE2;
               EdgFgMake(HalEdg(HalPrv(HalPrv(he)))) = MSD_ROUND_E_FASE2;
               MSD_lowMEF(HalPrv(HalPrv(he)), HalNxt(he), ++maxf);
               EdgFgRound(HalEdg(HalNxt(he))) = MSD_ROUND_E_FASE2;
            }
            else
            if (vecequal(p, VerVCoord(HalVtx(HalNxt(HalNxt(he))))) == 0)
            {
               MSD_lowMEF(HalPrv(HalPrv(he)), HalNxt(he), ++maxf);
               EdgFgMake(HalEdg(HalNxt(he))) = MSD_ROUND_E_FASE2;
            }
         }
         else
         {
            printf("r");
            mhe = mate(he);
            phe = HalPrv(he);
            veccopy(normal, FacFeq(LooLFace(HalWLoop(he))));
            veccopy(vhe, VerVCoord(HalVtx(he)));
            veccopy(vme, VerVCoord(HalVtx(mhe)));
            veccopy(vpe, VerVCoord(HalVtx(phe)));
            line_parallel(vhe, vme, p1, p2, normal, r1);
            line_parallel(vpe, vhe, p3, p4, normal, r2);
            line_inter(p1, p2, p3, p4, p);
            if (vecequal(p, VerVCoord(HalVtx(HalPrv(phe)))) == 0)
            {
               printf("f");
               MSD_lowMEV(phe, phe, ++maxv, p[0], p[1], p[2]);
               phe = HalPrv(phe);
               VerFgMake(HalVtx(phe)) = MSD_ROUND_V_FASE2;
               EdgFgMake(HalEdg(phe)) = MSD_ROUND_E_FASE2;
               MSD_lowMEF(phe, HalNxt(he), ++maxf);
               EdgFgMake(HalEdg(HalNxt(he))) = MSD_ROUND_E_FASE2;
               printf("e");
               return(HalNxt(he));
            }
            else
            if (vecequal(p, VerVCoord(HalVtx(HalNxt(HalNxt(he))))) == 0)
            {
               MSD_lowMEF(HalPrv(HalPrv(he)), HalNxt(he), ++maxf);
               EdgFgMake(HalEdg(HalNxt(he))) = MSD_ROUND_E_FASE2;
            }
         }
      }

      if ((comp(r2, r3, EPS) == -1) && (comp(r3, r1, EPS) != 1) &&
          (VerFgRound(HalVtx(he)) == 0))
      {
         veccopy(vaux, VerVCoord(HalVtx(he)));
         point_perp(VerVCoord(HalVtx(HalPrv(he))), vaux, p,
                    FacFeq(LooLFace(HalWLoop(he))), HalR(mate(HalPrv(he))));
         MSD_lowMEV(HalPrv(he), HalPrv(he), ++maxv, p[0], p[1], p[2]);
         VerFgMake(HalVtx(HalPrv(HalPrv(he)))) = MSD_ROUND_V_FASE2;
         EdgFgMake(HalEdg(HalPrv(HalPrv(he)))) = MSD_ROUND_E_FASE2;
      }

      if ((comp(r1, r3, EPS) == -1) && (comp(r3, r2, EPS) != 1) &&
          (VerFgRound(HalVtx(he)) == 0))
      {
         veccopy(vaux, VerVCoord(HalVtx(HalNxt(he))));
         point_perp(vaux, VerVCoord(HalVtx(mate(HalNxt(he)))), p,
                    FacFeq(LooLFace(HalWLoop(he))), HalR(he));
         MSD_lowMEV(HalNxt(he), HalNxt(he), ++maxv, p[0], p[1], p[2]);
         VerFgMake(HalVtx(HalNxt(HalNxt(he)))) = MSD_ROUND_V_FASE2;
         EdgFgMake(HalEdg(HalNxt(he)))         = MSD_ROUND_E_FASE2;
      }
   }
   return(he);
}

void GetNxtVertex(HPTYPE he, vector p)
{
   HPTYPE heaux;
   real   th;
   int    flag = 1;
   vector n, vaux;

   heaux = he;
   do
   {
      printf("n");
      heaux = HalNxt(heaux);
      if (VerFgMake(HalVtx(heaux)) == MSD_ROUND_V_FASE2)
      {
         veccopy(p, VerVCoord(HalVtx(heaux)));
         flag = 0;
      }
      if (VerFgMake(HalVtx(heaux)) == MSD_ROUND_V_ORIGINAL)
      {
         th = angle_edge(heaux);
         if (comp(th, PI, EPS) == 0)
         {
            if ((EdgFgRound(HalEdg(HalPrv(heaux))) != 1) &&
                (EdgFgRound(HalEdg(heaux)) != 1))
            {
               veccopy(vaux, VerVCoord(HalVtx(heaux)));
               point_perp(vaux, VerVCoord(HalVtx(mate(heaux))), p,
                          FacFeq(LooLFace(HalWLoop(heaux))),
                          (HalR(heaux) + HalR(mate(HalPrv(heaux)))) * 0.5);
               flag = 0;
            }
            if (EdgFgRound(HalEdg(HalPrv(heaux))) == 1)
            {
               veccopy(vaux, VerVCoord(HalVtx(heaux)));
               point_perp(vaux, VerVCoord(HalVtx(mate(heaux))), p,
                          FacFeq(LooLFace(HalWLoop(heaux))), HalR(heaux));
               flag = 0;
            }
            if (EdgFgRound(HalEdg(heaux)) == 1)
            {
               veccopy(vaux, VerVCoord(HalVtx(heaux)));
               point_perp(vaux, VerVCoord(HalVtx(mate(heaux))), p,
                          FacFeq(LooLFace(HalWLoop(heaux))),
                          HalR(mate(HalPrv(heaux))));
               flag = 0;
            }
         }
         if (comp(th, PI, EPS) == 1)
         {
            veccopy(n, FacFeq(LooLFace(HalWLoop(heaux))));
            vecesc(n, n, -1);
            veccopy(vaux, VerVCoord(HalVtx(heaux)));
            point_perp(vaux, VerVCoord(HalVtx(HalPrv(heaux))), p, n,
                       HalR(mate(HalPrv(heaux))));
            flag = 0;
         }
         if (comp(th, PI, EPS) == -1)
         {
            veccopy(p, VerVCoord(HalVtx(HalNxt(heaux))));
            flag = 0;
         }
      }
   } while (flag == 1 && heaux != he);
}

void GetPrvVertex(HPTYPE he, vector p)
{
   HPTYPE heaux;
   real   th;
   int    flag = 1;
   vector n, vaux;

   heaux = he;
   do
   {
      printf("p");
      heaux = HalPrv(heaux);
      if (VerFgMake(HalVtx(heaux)) == MSD_ROUND_V_FASE2)
      {
         veccopy(p, VerVCoord(HalVtx(heaux)));
         flag = 0;
      }
      if (VerFgMake(HalVtx(heaux)) == MSD_ROUND_V_ORIGINAL)
      {
         th = angle_edge(heaux);
         if (comp(th, PI, EPS) == 0)
         {
            if ((EdgFgRound(HalEdg(HalPrv(heaux))) != 1) &&
                (EdgFgRound(HalEdg(heaux)) != 1))
            {
               veccopy(vaux, VerVCoord(HalVtx(heaux)));
               point_perp(vaux, VerVCoord(HalVtx(mate(heaux))), p,
                          FacFeq(LooLFace(HalWLoop(heaux))),
                          (HalR(heaux) + HalR(mate(HalPrv(heaux)))) * 0.5);
               flag = 0;
            }
            if (EdgFgRound(HalEdg(HalPrv(heaux))) == 1)
            {
               veccopy(vaux, VerVCoord(HalVtx(heaux)));
               point_perp(vaux, VerVCoord(HalVtx(mate(heaux))), p,
                          FacFeq(LooLFace(HalWLoop(heaux))), HalR(heaux));
               flag = 0;
            }
            if (EdgFgRound(HalEdg(heaux)) == 1)
            {
               veccopy(vaux, VerVCoord(HalVtx(heaux)));
               point_perp(vaux, VerVCoord(HalVtx(mate(heaux))), p,
                          FacFeq(LooLFace(HalWLoop(heaux))),
                          HalR(mate(HalPrv(heaux))));
               flag = 0;
            }
         }
         if (comp(th, PI, EPS) == 1)
         {
            veccopy(n, FacFeq(LooLFace(HalWLoop(heaux))));
            veccopy(vaux, VerVCoord(HalVtx(heaux)));
            point_perp(vaux, VerVCoord(HalVtx(mate(heaux))), p, n, HalR(heaux));
            flag = 0;
         }
         if (comp(th, PI, EPS) == -1)
         {
            veccopy(p, VerVCoord(HalVtx(HalPrv(heaux))));
            flag = 0;
         }
      }
   } while (flag == 1 && heaux != he);
}

int cont_point(VPTYPE v1, VPTYPE v2, vector p)
{
   real   d12, dp1, dp2;
   vector vaux;

   veccopy(vaux, VerVCoord(v1));
   d12 = vecd(vaux, VerVCoord(v2));
   dp1 = vecd(vaux, p);
   dp2 = vecd(VerVCoord(v2), p);
   if ((dp1 > d12) || (dp2 > d12))
   {
      return(0);
   }
   return(1);
}
