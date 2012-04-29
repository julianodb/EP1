/* 
  PROJETO USPDesigner
  MODULO: LOW (Operadores Basicos)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: lowgeome.cpp
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
#include <math.h>
#include "memvirtu.h"
#include "lowparam.h"
#include "lowmacro.h"
#include "lowsolid.h"
#include "vectorop.h"
#include "splsplit.h"
#include "lowgeome.h"

/*** vertex equality ***/
int MSD_lowVtxVtxContainment(VPTYPE v1, VPTYPE v2)
{
   vector vv1, vv2;

   veccopy(vv1, VerVCoord(v1));
   veccopy(vv2, VerVCoord(v2));
   return(vecequal(vv1, vv2));
}

/*** Vertex-edge intersection ***/
int MSD_lowEdgVtxIntersection(VPTYPE v1, VPTYPE v2, VPTYPE v3, real *t)
{
#ifndef __VIRTUAL_MEM
   VTYPE v4;
#else
   VPTYPE v4;
   int    flag;
#endif
   vector r1, r2, vv1, vv2;
   real   r1r1;

   veccopy(vv1, VerVCoord(v1));
   veccopy(vv2, VerVCoord(v2));
   vecminus(r1, vv2, vv1);
   if ((r1r1 = dot(r1, r1)) < EPS * EPS)
   {
      *t = 0.0;
      return(MSD_lowVtxVtxContainment(v1, v3));
   }
   else
   {
      vecminus(r2, VerVCoord(v3), vv1);
#ifndef __VIRTUAL_MEM
      vecpe(r1, vv1, v4.vcoord, (*t = dot(r1, r2) / r1r1));
      return(MSD_lowVtxVtxContainment(&v4, v3));

#else
      v4 = MSD_getMemVirtual(VERTEX);
      vecpe(r1, vv1, VerVCoord(v4), (*t = dot(r1, r2) / r1r1));
      flag = MSD_lowVtxVtxContainment(v4, v3);
      MSD_putMemVirtual(v4, VERTEX);
      return(flag);
#endif

   }
}

/* Test for containment of v3 in edge v1-v2 */
int MSD_lowEdgVtxContainment(VPTYPE v1, VPTYPE v2, VPTYPE v3)
{
   real t;

   if (MSD_lowEdgVtxIntersection(v1, v2, v3, &t))
   {
      if ((t >= -EPS) && (t <= 1.0 + EPS))
      {
         return(TRUE);
      }
   }
   return(FALSE);
}

/***    Implement the procedure contfv. The procedure should test ***/
/*** for the  inclusion of the vertex v  in the face f and return ***/
/*** similar values as contlv.                                    ***/
int MSD_lowFacVtxContainment(FPTYPE f, VPTYPE v)
{
   if (FaceEqNOK(f))
   {
      MSD_lowFaceEq(FacFLOut(f), FacFeq(f));
   }
   return(MSD_lowFacVtxCoordContainment(f, v, getdrop(FacFeq(f))));
}

int MSD_lowFacVtxCoordContainment(FPTYPE f, VPTYPE v, int drop)
{
   LPTYPE l;
   int    cont;

   if ((cont = MSD_lowLooVtxContainment(FacFLOut(f), v, drop)) != 1)
   {
      return(cont);
   }
   for (AllLoopsFace(f, l))
   {
      if (l != FacFLOut(f))
      {
         if ((cont = MSD_lowLooVtxContainment(l, v, drop)) == 1)
         {
            return(0);
         }
         if (cont != 0)
         {
            return(cont);
         }
      }
   }
   return(1);
}

/*** Boundary cases ***/
int MSD_lowLooVtxBoundary(LPTYPE l, VPTYPE v)
{
   HPTYPE he;

   he = LooLEdg(l);
   do
   {
      if (MSD_lowVtxVtxContainment(HalVtx(he), v))
      {
         hitvertex = HalVtx(he);
         hithe     = HNIL;
         return(3);
      }
   } while ((he = HalNxt(he)) != LooLEdg(l));
   he = LooLEdg(l);
   do
   {
      if (MSD_lowEdgVtxContainment(HalVtx(he), HalVtx(HalNxt(he)), v))
      {
         hitvertex = VNIL;
         hithe     = he;
         return(2);
      }
   } while ((he = HalNxt(he)) != LooLEdg(l));
   return(0);
}

int MSD_lowEdgEdgIntersection(VPTYPE v1, VPTYPE v2, VPTYPE v3, VPTYPE v4,
                              int drop, real *t1, real *t2)
{
   real D, a1, a2, b1, b2, c1, c2;

   switch (drop)
   {
       case X:
       a1 = VerVCoord(v2)[1] - VerVCoord(v1)[1];
       a2 = VerVCoord(v2)[2] - VerVCoord(v1)[2];
       b1 = VerVCoord(v3)[1] - VerVCoord(v4)[1];
       b2 = VerVCoord(v3)[2] - VerVCoord(v4)[2];
       c1 = VerVCoord(v1)[1] - VerVCoord(v3)[1];
       c2 = VerVCoord(v1)[2] - VerVCoord(v3)[2];
       break;

       case Y:
       a1 = VerVCoord(v2)[0] - VerVCoord(v1)[0];
       a2 = VerVCoord(v2)[2] - VerVCoord(v1)[2];
       b1 = VerVCoord(v3)[0] - VerVCoord(v4)[0];
       b2 = VerVCoord(v3)[2] - VerVCoord(v4)[2];
       c1 = VerVCoord(v1)[0] - VerVCoord(v3)[0];
       c2 = VerVCoord(v1)[2] - VerVCoord(v3)[2];
       break;

       case Z:
       a1 = VerVCoord(v2)[0] - VerVCoord(v1)[0];
       a2 = VerVCoord(v2)[1] - VerVCoord(v1)[1];
       b1 = VerVCoord(v3)[0] - VerVCoord(v4)[0];
       b2 = VerVCoord(v3)[1] - VerVCoord(v4)[1];
       c1 = VerVCoord(v1)[0] - VerVCoord(v3)[0];
       c2 = VerVCoord(v1)[1] - VerVCoord(v3)[1];
       break;
   }

   if (comp(D = a1 * b2 - a2 * b1, 0.0, EPS) == 0)
   {
      return(0);
   }
   *t1 = (c2 * b1 - c1 * b2) / D;
   *t2 = (a2 * c1 - a1 * c2) / D;
   return(1);
}

/*** Vertex-loop containment ***/
int MSD_lowLooVtxContainment(LPTYPE l, VPTYPE v, int drop)
{
   HPTYPE he1;
   HPTYPE he2;
   VPTYPE v1;
   VPTYPE v2;

#ifndef __VIRTUAL_MEM
   VTYPE aux;
#else
   VPTYPE aux;
#endif
   real   t1, t2;
   int    count, intr, c1, c2, ip;
   vector vv1, vv2;

   if ((intr = MSD_lowLooVtxBoundary(l, v)) > 0)
   {
      return(intr);
   }
   he2 = LooLEdg(l);

#ifdef __VIRTUAL_MEM
   aux = MSD_getMemVirtual(VERTEX);
#endif

   do
   {
      he1 = LooLEdg(l);
      v1  = HalVtx(he2);
      v2  = HalVtx(HalNxt(he2));
      veccopy(vv1, VerVCoord(v1));
      veccopy(vv2, VerVCoord(v2));

#ifndef __VIRTUAL_MEM
      vecplus(aux.vcoord, vv1, vv2);
      vecesc(aux.vcoord, aux.vcoord, 0.5);
#else
      vecplus(VerVCoord(aux), vv1, vv2);
      vecesc(VerVCoord(aux), VerVCoord(aux), 0.5);
#endif

      count = 0;
      do
      {
         ip = 1;
         v1 = HalVtx(he1);
         v2 = HalVtx(HalNxt(he1));

#ifndef __VIRTUAL_MEM
         if ((intr = MSD_lowEdgEdgIntersection(v, &aux, v1, v2, drop, &t1, &t2)) == 1)
         {
#else
         if ((intr = MSD_lowEdgEdgIntersection(v, aux, v1, v2, drop, &t1, &t2)) == 1)
         {
#endif

            c1 = comp(t2, 0.0, EPS);
            c2 = comp(t2, 1.0, EPS);
            if ((c1 == 0) || (c2 == 0))
            {
               ip = 0;
            }
            if ((c1 == 1) && (c2 == -1))
            {
               if (t1 >= 0.0)
               {
                  count++;
               }
            }
         }
         if (ip == 1)
         {
            he1 = HalNxt(he1);
         }
      } while (he1 != LooLEdg(l) && ip == 1);
   } while ((he2 = HalNxt(he2)) != LooLEdg(l) && ip == 0);

#ifdef __VIRTUAL_MEM
   MSD_putMemVirtual(aux, VERTEX);
#endif

   if (ip == 0)
   {
      return(ERROR);
   }
   return(count % 2);
}

/* Do a box test */
int MSD_lowLooBoxContainment(LPTYPE l, VPTYPE v, int drop)
{
   HPTYPE he;
   VPTYPE vtx;
   real   umin, umax, vmin, vmax;

   umin = vmin = 1e10;
   umax = vmax = -1e10;

   he = LooLEdg(l);
   do
   {
      vtx = HalVtx(he);
      switch (drop)
      {
          case X:
          if (umin < VerVCoord(vtx)[1])
          {
             umin = VerVCoord(vtx)[1];
          }
          if (vmin < VerVCoord(vtx)[2])
          {
             vmin = VerVCoord(vtx)[2];
          }
          if (umax > VerVCoord(vtx)[1])
          {
             umax = VerVCoord(vtx)[1];
          }
          if (vmax > VerVCoord(vtx)[2])
          {
             vmax = VerVCoord(vtx)[2];
          }
          break;

          case Y:
          if (umin < VerVCoord(vtx)[0])
          {
             umin = VerVCoord(vtx)[0];
          }
          if (vmin < VerVCoord(vtx)[2])
          {
             vmin = VerVCoord(vtx)[2];
          }
          if (umax > VerVCoord(vtx)[0])
          {
             umax = VerVCoord(vtx)[0];
          }
          if (vmax > VerVCoord(vtx)[2])
          {
             vmax = VerVCoord(vtx)[2];
          }
          break;

          case Z:
          if (umin < VerVCoord(vtx)[0])
          {
             umin = VerVCoord(vtx)[0];
          }
          if (vmin < VerVCoord(vtx)[1])
          {
             vmin = VerVCoord(vtx)[1];
          }
          if (umax > VerVCoord(vtx)[0])
          {
             umax = VerVCoord(vtx)[0];
          }
          if (vmax > VerVCoord(vtx)[1])
          {
             vmax = VerVCoord(vtx)[1];
          }
          break;
      }
   } while ((he = HalNxt(he)) != LooLEdg(l));

   umin -= CONTBVEPS;
   umax += CONTBVEPS;
   vmin -= CONTBVEPS;
   vmax += CONTBVEPS;

   switch (drop)
   {
       case X:
       if ((VerVCoord(v)[1] < umin) || (VerVCoord(v)[1] > umax))
       {
          return(0);
       }
       if ((VerVCoord(v)[2] < vmin) || (VerVCoord(v)[2] > vmax))
       {
          return(0);
       }
       break;

       case Y:
       if ((VerVCoord(v)[0] < umin) || (VerVCoord(v)[0] > umax))
       {
          return(0);
       }
       if ((VerVCoord(v)[2] < vmin) || (VerVCoord(v)[2] > vmax))
       {
          return(0);
       }
       break;

       case Z:
       if ((VerVCoord(v)[0] < umin) || (VerVCoord(v)[0] > umax))
       {
          return(0);
       }
       if ((VerVCoord(v)[1] < vmin) || (VerVCoord(v)[1] > vmax))
       {
          return(0);
       }
       break;
   }
   return(1);
}

int MSD_lowSolVtxContainment(SPTYPE s, VPTYPE v)
{
   DPTYPE d;
   FPTYPE f;

#ifndef __VIRTUAL_MEM
   VTYPE auxv, testv;
#else
   VPTYPE auxv, testv;
#endif

   real t, d1, d2, min_t, min_d;

#ifndef __VIRTUAL_MEM
   auxv.vcoord[0] = VerVCoord(v)[0];
   auxv.vcoord[1] = VerVCoord(v)[1];
   auxv.vcoord[2] = VerVCoord(v)[2] + 1.0;
#else
   auxv               = MSD_getMemVirtual(VERTEX);
   testv              = MSD_getMemVirtual(VERTEX);
   VerVCoord(auxv)[0] = VerVCoord(v)[0];
   VerVCoord(auxv)[1] = VerVCoord(v)[1];
   VerVCoord(auxv)[2] = VerVCoord(v)[2] + 1.0;
#endif

   min_t = 1e6;
   min_d = 1.0;


   for (AllShellsSolid(s, d))
   {
      for (AllFacesShell(d, f))
      {
         if (comp(MSD_lowDistancePlanePoint(FacFeq(f), v), 0.0, EPS) == 0)
         {
            if (MSD_lowFacVtxContainment(f, v) > 0)
            {
               return(0);
            }
         }
      }
   }

   for (AllShellsSolid(s, d))
   {
      for (AllFacesShell(d, f))
      {
         if (FaceEqNOK(f))
         {
            MSD_lowFaceEq(FacFLOut(f), FacFeq(f));
         }
         d1 = MSD_lowDistancePlanePoint(FacFeq(f), v);

#ifndef __VIRTUAL_MEM
         d2 = MSD_lowDistancePlanePoint(FacFeq(f), &auxv);
#else
         d2 = MSD_lowDistancePlanePoint(FacFeq(f), auxv);
#endif

         if (comp(d1 - d2, 0.0, EPS) != 0)
         {
            t = d1 / (d1 - d2);
            if ((t >= 0.0) && (t < min_t))
            {
#ifndef __VIRTUAL_MEM
               testv.vcoord[0] = VerVCoord(v)[0];
               testv.vcoord[1] = VerVCoord(v)[1];
               testv.vcoord[2] = VerVCoord(v)[2] +
                                 t * (auxv.vcoord[2] - VerVCoord(v)[2]);
               if (MSD_lowFacVtxContainment(f, &testv))
               {
#else
               VerVCoord(testv)[0] = VerVCoord(v)[0];
               VerVCoord(testv)[1] = VerVCoord(v)[1];
               VerVCoord(testv)[2] = VerVCoord(v)[2] +
                                     t * (VerVCoord(auxv)[2] - VerVCoord(v)[2]);
               if (MSD_lowFacVtxContainment(f, testv))
               {
#endif

                  if ((comp(t, min_t, EPS) == 0) && (min_d > d1))
                  {
                     min_d = d1;
                  }
                  if (t < min_t)
                  {
                     min_t = t;
                     min_d = d1;
                  }
               }
            }
         }
      }
   }
#ifdef __VIRTUAL_MEM
   MSD_putMemVirtual(testv, VERTEX);
   MSD_putMemVirtual(auxv, VERTEX);
#endif

   return(comp(-min_d, 0.0, EPS));
}

/* Various operations for face equations are faces identical? */
int MSD_lowFacFacIdentical(FPTYPE f1, FPTYPE f2)
{
   LPTYPE l1;
   LPTYPE l2;
   HPTYPE he1;
   HPTYPE he2;
   int    flag;

   l1 = FacFLOut(f1);
   l2 = FacFLOut(f2);
   if (LooLength(l1) != LooLength(l2))
   {
      return(0);
   }

   flag = 0;
   he1  = LooLEdg(l1);
   he2  = LooLEdg(l2);
   do
   {
      if (MSD_lowVtxVtxContainment(HalVtx(he1), HalVtx(he2)))
      {
         flag = 1;
      }
   } while ((he2 = HalNxt(he2)) != LooLEdg(l2));
   if (flag == 0)
   {
      return(0);
   }

   do
   {
      if (!MSD_lowVtxVtxContainment(HalVtx(he1), HalVtx(he2)))
      {
         return(0);
      }
      he2 = HalPrv(he2);
   } while ((he1 = HalNxt(he1)) != LooLEdg(l1));
   return(1);
}

/* does f1 contain f2 ? */
int MSD_lowFacFacContainment(FPTYPE f1, FPTYPE f2)
{
   LPTYPE l;
   HPTYPE he;

   for (AllLoopsFace(f2, l))
   {
      he = LooLEdg(l);
      do
      {
         if (MSD_lowLooVtxContainment(FacFLOut(f1), HalVtx(he), getdrop(FacFeq(f1))) == FALSE)
         {
            return(FALSE);
         }
      } while ((he = HalNxt(he)) != LooLEdg(l));
   }
   return(TRUE);
}

/* are two faces coplanar ? */
int MSD_lowFacFacEqualEqs(FPTYPE f1, FPTYPE f2)
{
   vector vf1, vf2;

   veccopy(vf1, FacFeq(f1));
   veccopy(vf2, FacFeq(f2));
   if (comp(vf1[3], vf2[3], EPS) == 0)
   {
      return(vecequal(vf1, vf2));
   }
   return(FALSE);
}

/* are equations (almost) opposite ? */
int MSD_lowFacFacOppositeEqs(FPTYPE f1, FPTYPE f2)
{
   vector tmp, vf1, vf2;

   veccopy(vf1, FacFeq(f1));
   veccopy(vf2, FacFeq(f2));
   if (comp(vf1[3], -vf2[3], EPS) == 0)
   {
      vecplus(tmp, vf1, vf2);
      return(vecnull(tmp, EPS));
   }
   return(0);
}

/* are two edges colinear */
int MSD_lowEdgEdgColinear(EPTYPE e1, EPTYPE e2)
{
   vector dir1, dir2, cr, ve11, ve12, ve21, ve22;

   veccopy(ve21, VerVCoord(HalVtx(EdgHe2(e1))));
   veccopy(ve11, VerVCoord(HalVtx(EdgHe1(e1))));
   veccopy(ve22, VerVCoord(HalVtx(EdgHe2(e2))));
   veccopy(ve12, VerVCoord(HalVtx(EdgHe1(e2))));
   vecminus(dir1, ve21, ve11);
   vecminus(dir2, ve22, ve12);
   cross(cr, dir1, dir2);
   if (vecnull(cr, EPS))
   {
      return(1);
   }
   return(0);
}

/* is h a null edge */
int MSD_lowNullEdge(HPTYPE h)
{
   return(MSD_lowVtxVtxContainment(HalVtx(h), HalVtx(HalNxt(h))));
}
