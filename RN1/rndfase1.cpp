/* 
  PROJETO USPDesigner
  MODULO: RND (Arredondamento)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: rndfase1.cpp
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
#include "curvgeom.h"

int GeneratingVertices_edges(SPTYPE s)
{
   if (MakingEdges_tables(s) == ERROR)
   {
      return(ERROR);
   }
   if (MakingVertices_edges(s) == ERROR)
   {
      return(ERROR);
   }
   return(SUCCESS);
}

int MakingEdges_tables(SPTYPE s)
{
   DPTYPE d;
   EPTYPE e;
   HPTYPE he1;
   HPTYPE he2;
   HPTYPE phe1;
   HPTYPE phe2;
   HPTYPE nhe1;
   HPTYPE nhe2;
   VPTYPE vhe1;
   VPTYPE vhe2;
   real   t;
   vector p, p1, p2, va, vche1, vche2, fche1, fche2;

   for (AllShellsSolid(s, d))
   {
      for (AllEdgesShell(d, e))
      {
         veccopy(va, VerVCoord(HalVtx(EdgHe1(e))));
         vecminus(p, va, VerVCoord(HalVtx(EdgHe2(e))));
         EdgD(e) = sqrt(dot(p, p));
      }
   }

   for (AllShellsSolid(s, d))
   {
      for (AllEdgesShell(d, e))
      {
#ifdef __TRACE_ROUND__
         printf("e=%d fgp=%d  fgn=%d ", EdgEdgeNo(e),
                EdgFgRound(HalEdg(HalPrv(EdgHe1(e)))),
                EdgFgRound(HalEdg(HalPrv(EdgHe2(e)))));
         printf("aprv=%f ", angle_edge(EdgHe1(e)));
         printf("anxt=%f ", angle_edge(HalNxt(EdgHe1(e))));
#endif

         he1  = EdgHe1(e);
         phe1 = HalPrv(he1);
         vhe1 = HalVtx(he1);
         nhe1 = HalNxt(he1);
         he2  = EdgHe2(e);
         phe2 = HalPrv(he2);
         vhe2 = HalVtx(he2);
         nhe2 = HalNxt(he2);
         veccopy(vche1, VerVCoord(vhe1));
         veccopy(vche2, VerVCoord(vhe2));
         veccopy(fche1, FacFeq(LooLFace(HalWLoop(he1))));
         veccopy(fche2, FacFeq(LooLFace(HalWLoop(he2))));

         if (EdgFgRound(HalEdg(phe1)) > 1)       // --> condicao 1 -------------
         {
            if (comp(angle_edge(he1), PI, EPS) == -1)
            {
               veccopy(va, VerVCoord(HalVtx(phe1)));
               _line(va, HalR(phe1), vche1, HalR(mate(phe1)), p1, p2,
                     fche1);
               line_inter(p1, p2, vche1, vche2, p);
               t = calc_t(vche1, vche2, p);

#ifdef __TRACE_ROUND__
               printf("t1= %f\n", t);
#endif
               if ((t < 0) || (t > 1))
               {
                  printf("Raio incompativel!\n");
                  return(ERROR);
               }
               EdgT1(e) = t;
            }
            else
            {
               EdgT1(e) = 2;
            }
         }
         else
         {
            EdgT1(e) = 2;
         }

         if (EdgFgRound(HalEdg(nhe1)) > 1)       // --> condicao 2 -------------
         {
            if (comp(angle_edge(nhe1), PI, EPS) == -1)
            {
               veccopy(va, VerVCoord(HalVtx(nhe1)));
               _line(va, HalR(nhe1), VerVCoord(HalVtx(mate(nhe1))),
                     HalR(mate(nhe1)), p1, p2, fche1);
               line_inter(p1, p2, vche1, vche2, p);
               t = calc_t(VerVCoord(HalVtx(nhe1)), vche1, p);

#ifdef __TRACE_ROUND__
               printf("t3= %f\n", t);
#endif
               if ((t < 0) || (t > 1))
               {
                  printf("Raio incompativel!\n");
                  return(ERROR);
               }
               EdgT3(e) = t;
            }
            else
            {
               EdgT3(e) = 3;
            }
         }
         else
         {
            EdgT3(e) = 3;
         }


         if (EdgFgRound(HalEdg(phe2)) > 1)
         {
            if (comp(angle_edge(he2), PI, EPS) == -1)
            {
               veccopy(va, VerVCoord(HalVtx(phe2)));
               _line(va, HalR(phe2), vche2,
                     HalR(mate(phe2)), p1, p2, fche2);
               line_inter(p1, p2, vche2, vche1, p);
               t = calc_t(vche2, vche1, p);

#ifdef __TRACE_ROUND__
               printf("t4= %f\n", t);
#endif
               if ((t < 0) || (t > 1))
               {
                  printf("Raio incompativel!\n");
                  return(ERROR);
               }
               EdgT4(e) = t;
            }
            else
            {
               EdgT4(e) = 3;
            }
         }
         else
         {
            EdgT4(e) = 3;
         }

         if (EdgFgRound(HalEdg(nhe2)) > 1)
         {
            if (comp(angle_edge(nhe2), PI, EPS) == -1)
            {
               veccopy(va, VerVCoord(HalVtx(nhe2)));
               _line(va, HalR(nhe2),
                     VerVCoord(HalVtx(mate(nhe2))),
                     HalR(mate(nhe2)), p1, p2,
                     fche2);
               line_inter(p1, p2, vche2, vche1, p);
               t = calc_t(VerVCoord(HalVtx(nhe2)), vche2, p);

#ifdef __TRACE_ROUND__
               printf("t2= %f\n", t);
#endif
               if ((t < 0) || (t > 1))
               {
                  printf("Raio incompativel!\n");
                  return(ERROR);
               }
               EdgT2(e) = t;
            }
            else
            {
               EdgT2(e) = 2;
            }
         }
         else
         {
            EdgT2(e) = 2;
         }

#ifdef __TRACE_ROUND__
         printf("e= %d, t1= %f, t2= %f, t3= %f, t4= %f\n\n", EdgEdgeNo(e),
                EdgT1(e),
                EdgT2(e),
                EdgT3(e),
                EdgT4(e));
#endif
      }
   }
   return(SUCCESS);
}

int MakingVertices_edges(SPTYPE s)
{
   DPTYPE d;
   EPTYPE e;
   vector p, va;
   int    flag;
   real   t;

   for (AllShellsSolid(s, d))
   {
      for (AllEdgesShell(d, e))
      {
         flag = 0;
         if (EdgFgMake(e) == 0)
         {
            if (((EdgT1(e) > fabs(1 - EdgT3(e))) && (EdgT1(e) != 2)) ||
                ((EdgT1(e) > fabs(1 - EdgT4(e))) && (EdgT1(e) != 2)) ||
                ((EdgT2(e) > fabs(1 - EdgT3(e))) && (EdgT2(e) != 2)) ||
                ((EdgT2(e) > fabs(1 - EdgT4(e))) && (EdgT2(e) != 2)))
            {
               printf("Parametros Incompativeis!\n");
               return(ERROR);
            }
            if ((EdgFgRound(HalEdg(HalNxt(EdgHe1(e)))) == MSD_ROUND_FLAG_E0) &&
                (comp(angle_edge(HalNxt(EdgHe1(e))), PI, EPS) == -1))
            {
               t = 0.5;
               HalR(EdgHe1(e)) = 0;
               HalR(EdgHe2(e)) = 0;
               flag            = 1;
            }
            if ((EdgFgRound(HalEdg(HalPrv(EdgHe1(e)))) == MSD_ROUND_FLAG_E0) &&
                (comp(angle_edge(EdgHe1(e)), PI, EPS) == -1))
            {
               t = 0.5;
               HalR(EdgHe1(e)) = 0;
               HalR(EdgHe2(e)) = 0;
               flag            = 1;
            }
            else
            {
               if (VerFgRound(HalVtx(EdgHe1(e))) == MSD_ROUND_FLAG_V0)
               {
                  if (((comp(EdgT2(e), EdgT1(e), EPS) != -1) ||
                       (EdgT1(e) == 2)) && (EdgT2(e) > 0) && (EdgT2(e) < 1))
                  {
                     t    = EdgT2(e);
                     flag = 1;
                  }
                  if (((EdgT1(e) > EdgT2(e)) || (EdgT2(e) == 2)) &&
                      (EdgT1(e) > 0) && (EdgT1(e) < 1))
                  {
                     t    = EdgT1(e);
                     flag = 1;
                  }
               }
               else
               {
                  if ((EdgT1(e) > 0) && (EdgT1(e) < 1) &&
                      (EdgT2(e) > 0) && (EdgT2(e) < 1))
                  {
                     t    = (EdgT1(e) + EdgT2(e)) * 0.5;
                     flag = 1;
                  }
                  if ((EdgT1(e) == 2) && (EdgT2(e) > 0) && (EdgT2(e) < 1))
                  {
                     t    = EdgT2(e);
                     flag = 1;
                  }
                  if ((EdgT2(e) == 2) && (EdgT1(e) > 0) && (EdgT1(e) < 1))
                  {
                     t    = EdgT1(e);
                     flag = 1;
                  }
               }
            }
            if (flag == 1)
            {
               veczer(p);

#ifdef __TRACE_ROUND__
               //printf("t= %f\n", t);
#endif

               veccopy(va, VerVCoord(HalVtx(EdgHe2(e))));
               vecminus(p, va, VerVCoord(HalVtx(EdgHe1(e))));
               normalize(p);
               vecesc(p, p, EdgD(e) * t);
               vecplus(p, p, VerVCoord(HalVtx(EdgHe1(e))));
               maxv = MSD_getMaxVertexId(s);
               if (vecequal(p, VerVCoord(HalVtx(HalNxt(EdgHe1(e))))) == 0)
               {
                  MSD_lowMEV(EdgHe1(e), HalNxt(EdgHe2(e)), ++maxv, p[0],
                             p[1],
                             p[2]);
#ifdef __TRACE_ROUND__
                  printf("V1 = ( %f, %f, %f)\n",
                         VerVCoord(HalVtx(EdgHe1(e)))[0],
                         VerVCoord(HalVtx(EdgHe1(e)))[1],
                         VerVCoord(HalVtx(EdgHe1(e)))[2]);
#endif

                  VerFgMake(HalVtx(EdgHe1(e)))          = MSD_ROUND_V_FASE1;
                  EdgFgMake(HalEdg(HalPrv(EdgHe1(e))))  = MSD_ROUND_E_FASE1;
                  EdgFgRound(HalEdg(HalPrv(EdgHe1(e)))) = EdgFgRound(e);
                  HalR(HalPrv(EdgHe1(e)))         = HalR(EdgHe1(e));
                  HalR(HalNxt(EdgHe2(e)))         = HalR(EdgHe2(e));
                  EdgD(HalEdg(HalPrv(EdgHe1(e)))) = EdgD(e);
               }
            }

            flag = 0;
            t    = 0;
            if ((EdgFgRound(HalEdg(HalNxt(EdgHe2(e)))) == MSD_ROUND_FLAG_E0) &&
                (comp(angle_edge(HalNxt(EdgHe2(e))), PI, EPS) == -1))
            {
               t    = 0.5;
               flag = 1;
            }
            if ((EdgFgRound(HalEdg(HalPrv(EdgHe2(e)))) == MSD_ROUND_FLAG_E0) &&
                (comp(angle_edge(EdgHe2(e)), PI, EPS) == -1))
            {
               t    = 0.5;
               flag = 1;
            }
            else
            {
               if (VerFgRound(HalVtx(EdgHe2(e))) == MSD_ROUND_FLAG_V0)
               {
                  if (((EdgT3(e) >= EdgT4(e)) || (EdgT4(e) == 3)) &&
                      (EdgT3(e) > 0) && (EdgT3(e) < 1))
                  {
                     t    = EdgT3(e);
                     flag = 1;
                  }
                  if (((EdgT4(e) > EdgT3(e)) || (EdgT3(e) == 3)) &&
                      (EdgT4(e) > 0) && (EdgT4(e) < 1))
                  {
                     t    = EdgT4(e);
                     flag = 1;
                  }
               }
               else
               {
                  if ((EdgT3(e) > 0) && (EdgT3(e) < 1) &&
                      (EdgT4(e) > 0) && (EdgT4(e) < 1))
                  {
                     t    = (EdgT3(e) + EdgT4(e)) * 0.5;
                     flag = 1;
                  }
                  if ((EdgT3(e) == 3) && (EdgT4(e) > 0) && (EdgT4(e) < 1))
                  {
                     t    = EdgT4(e);
                     flag = 1;
                  }
                  if ((EdgT4(e) == 3) && (EdgT3(e) > 0) && (EdgT3(e) < 1))
                  {
                     t    = EdgT3(e);
                     flag = 1;
                  }
               }
            }
            if (flag == 1)
            {
               veczer(p);

#ifdef __TRACE_ROUND__
               printf("t= %f\n", t);
#endif

               veccopy(va, VerVCoord(HalVtx(EdgHe1(e))));
               vecminus(p, va, VerVCoord(HalVtx(EdgHe2(e))));
               normalize(p);
               vecesc(p, p, EdgD(e) * t);
               vecplus(p, p, VerVCoord(HalVtx(EdgHe2(e))));
               maxv = MSD_getMaxVertexId(s);
               if (vecequal(p, VerVCoord(HalVtx(HalNxt(EdgHe2(e))))) == 0)
               {
                  MSD_lowMEV(EdgHe2(e), HalNxt(EdgHe1(e)), ++maxv, p[0],
                             p[1],
                             p[2]);

#ifdef __TRACE_ROUND__
                  printf("V2 = ( %f, %f, %f)\n",
                         VerVCoord(HalVtx(EdgHe2(e)))[0],
                         VerVCoord(HalVtx(EdgHe2(e)))[1],
                         VerVCoord(HalVtx(EdgHe2(e)))[2]);
#endif

                  VerFgMake(HalVtx(EdgHe2(e)))          = MSD_ROUND_V_FASE1;
                  EdgFgMake(HalEdg(HalPrv(EdgHe2(e))))  = MSD_ROUND_E_FASE1;
                  EdgFgRound(HalEdg(HalPrv(EdgHe2(e)))) = EdgFgRound(e);
                  HalR(HalPrv(EdgHe2(e)))         = HalR(EdgHe1(e));
                  HalR(HalNxt(EdgHe1(e)))         = HalR(EdgHe2(e));
                  EdgD(HalEdg(HalPrv(EdgHe2(e)))) = EdgD(e);
               }
            }
         }
      }
   }

   return(SUCCESS);
}
