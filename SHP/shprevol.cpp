/* 
  PROJETO USPDesigner
  MODULO: SHP (Criacao de Formas Primitivas)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: shprevol.cpp
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
#ifndef __UNIX_
 #include <stdlib.h>
#endif
#include "memvirtu.h"
#include "lowparam.h"
#include "lowmacro.h"
#include "lowsolid.h"
#include "eulerops.h"
#include "lowgeome.h"
#include "vectorop.h"
#include "mancommd.h"
#include "shpshape.h"

extern FILE *trace;

SPTYPE MSD_lowMakeRevolve(Id sn, XY *listvtx, int m, int n)
{
   SPTYPE        s;
   int           flag = 1, GOld;
   static vector vect = { 0.0, 1.0, 0.0, 0.0 };

   GOld         = Gdoinversion;
   Gdoinversion = FALSE;
   CommandCount++;
   if ((s = MSD_lowMakePolygon(sn, listvtx, n, 2)) == SNIL)
   {
      flag = 0;
   }
   if (MSD_lowMakeRotationalSweep(s, m, 2, 0, vect) == ERROR)
   {
      flag = 0;
   }
   if (flag == 1)
   {
      MSD_lowSetNormal(s, TRUE);
      MSD_lowSetEdgeAngle(s);
      MSD_lowSetInfo(s);
   }
   if (flag == 0)
   {
      MSD_lowHardRemove(s);
   }
   if (flag == 1)
   {
      MSD_lowAddEulerOp(s, HARDREM, 0, 0, 0, 0, 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0);
   }
   Gdoinversion = GOld;
   return(flag == 0 ? SNIL : s);
}

/* Rotational sweeping "swings" a two-dimensional figure around an */
/* axis to create a rotational solid. The accuracy of the          */
/* approximation is selected by parameter nfac that tell how many  */
/* points are used to approximate any circle of the solid.         */
/*   The algorithm developed performs the sweeping operation in    */
/* two phases. In the first phase the algorithm makes nfaces-1     */
/* scans along the wire, and creates one new face for each         */
/* original wire edge with an algorithm somewhat similar to the    */
/* translational sweeping algorithm. In the second phase, the most */
/* recent edges created will be joined with the edges of the       */
/* original wire.                                                  */
/*   The procedure implements the algorithm by first finding the   */
/* head first of the wire; variables cfirst e clast are used to    */
/* hold the leftmost edge of the current scan. The first while-    */
/* loop controls the nfaces-1 wire scans, while inner loop marches */
/* along the wire and creates the new edges; clast is used to      */
/* control the termination of the inner loop. In the last "mending"*/
/* scan, the remaining faces and edges are created.                */

char MSD_lowMakeRotationalSweep(SPTYPE s, int nfaces, int axs, int pln, vector vect)
{
   FPTYPE tailf;
   FPTYPE headf;
   HPTYPE cfirst;
   HPTYPE last;
   HPTYPE scan;
   HPTYPE first;
   Id     maxv, maxf;
   int    cut_headf, cut_tailf, closed_figure;
   real   phix, phiy, phiz;
   matrix m;
   vector v;

   if ((closed_figure = MSD_lowMakeOpenRSweep(s, pln)) == 2)
   {
      return(ERROR);
   }
   scan = LooLEdg(FacFLoops(SheSFaces(SolSShells(s))));
   if ((cfirst = MSD_lowTouchWire(scan, &cut_tailf, pln)) == HNIL)
   {
      return(ERROR);
   }
   if ((last = MSD_lowTouchWire(HalNxt(cfirst), &cut_headf, pln)) == HNIL)
   {
      return(ERROR);
   }

   maxv = MSD_getMaxVertexId(s);
   maxf = MSD_getMaxFaceId(s);
   MSD_lowMEF(HalNxt(cfirst), HalNxt(last), maxf++);
   MSD_lowFaceEq(HalWLoop(cfirst), FacFeq(LooLFace(HalWLoop(cfirst))));
   if (dot(FacFeq(LooLFace(HalWLoop(cfirst))), vect) < 0.0)
   {
      scan   = cfirst;
      cfirst = last;
      last   = scan;
   }
   MSD_lowKEF(HalNxt(cfirst), HalNxt(last));
   headf = SheSFaces(SolSShells(s));
   first = cfirst;

   if ((mate(cfirst) != last) && (HalNxt(HalNxt(cfirst)) != last))
   {
      for (scan = HalNxt(HalNxt(cfirst)); scan != last; scan = HalNxt(scan))
      {
         if (comp(VerVCoord(HalVtx(scan))[pln], 0.0, EPS) == 0)
         {
            return(ERROR);
         }
      }
   }

   matident(m);
   phix = axs == 0 ? 360.0 / (real)nfaces : 0.0;
   phiy = axs == 1 ? 360.0 / (real)nfaces : 0.0;
   phiz = axs == 2 ? 360.0 / (real)nfaces : 0.0;
   matrotat(m, phix, phiy, phiz);

   first = cfirst;
   while (--nfaces)
   {
      vecmult(v, VerVCoord(HalVtx(HalNxt(cfirst))), m);
      MSD_lowMEV(HalNxt(cfirst), HalNxt(cfirst), maxv++, v[0], v[1], v[2]);
      scan = HalNxt(cfirst);
      while (scan != HalNxt(last))
      {
         vecmult(v, VerVCoord(HalVtx(HalPrv(scan))), m);
         MSD_lowMEV(HalPrv(scan), HalPrv(scan), maxv++, v[0], v[1], v[2]);
         MSD_lowMEF(HalPrv(HalPrv(scan)), HalNxt(scan), maxf++);
         scan = HalNxt(HalNxt(scan));
         scan = mate(scan);
      }
      last   = scan;
      cfirst = HalNxt(HalNxt(cfirst));
      cfirst = mate(cfirst);
   }
   tailf = MSD_lowMEF(HalNxt(cfirst), mate(first), maxf++);
   while (cfirst != scan)
   {
      MSD_lowMEF(cfirst, HalNxt(HalNxt(HalNxt(cfirst))), maxf++);
      cfirst = HalPrv(mate(HalPrv(cfirst)));
   }

   if (closed_figure == 1)
   {
      MSD_lowKFMRH(headf, tailf);
      MSD_lowLoopGlue(headf);
   }
   if (cut_headf == 1)
   {
      MSD_lowMakeFaceToPoint(headf);
   }
   if (cut_tailf == 1)
   {
      MSD_lowMakeFaceToPoint(tailf);
   }
   return(SUCCESS);
}

/* Reduces a face to a single vertex.                                        */
void MSD_lowMakeFaceToPoint(FPTYPE f)
{
   HPTYPE he;
   HPTYPE henxt;
   LPTYPE l1;
   LPTYPE l2;
   int    flag, drop;

   he = LooLEdg(FacFLoops(f));
   if (HalEdg(he) != ENIL)
   {
      while (he != HalNxt(he))
      {
         henxt = HalNxt(he);
         MSD_lowKEV(henxt, mate(henxt));
      }
      MSD_lowKEF(mate(he), he);
   }
   else
   {
      for (AllLoopsFace(f, l1))
      {
         flag = 0;
         for (AllLoopsFace(f, l2))
         {
            if (l1 == l2)
            {
               continue;
            }
            drop = getdrop(FacFeq(f));
            if (MSD_lowLooVtxContainment(l1, HalVtx(LooLEdg(l2)), drop))
            {
               flag = 1;
            }
         }
         if (flag == 0)
         {
            FacFLOut(f) = l1;
            break;
         }
      }
      FacFLOut(f) = InnerLoop(f);
   }
}

/* The rotational sweeping algorithm was designed so as to operate */
/* on an open polygonal string of edges, a wire. This routine will */
/* open the lamina at an arbitrary vertex by duplicating it with a */
/* MEV, and removing the null edge connecting the resulting two    */
/* vertex with a KEF. This gives us an open wire that can be swung */
/* with the procedure rsweep.                                      */
/*    The rotational sweeping algorithm works correctly with       */
/* closed figures that have an edge lying on the sweeping axis.    */
int MSD_lowMakeOpenRSweep(SPTYPE s, int pln)
{
   HPTYPE scan;
   HPTYPE h;

   /* verifica para o caso de ser uma face fechada. */
   if (FacNextF(SheSFaces(SolSShells(s))) != FNIL)
   {
      h = LooLEdg(FacFLoops(SheSFaces(SolSShells(s))));
      if ((scan = MSD_lowTouchAxis(h, pln)) != HNIL)
      {
         if (comp(VerVCoord(HalVtx(HalNxt(scan)))[pln], 0.0, EPS) == 0)
         {
            MSD_lowKEF(scan, mate(scan));
            return(0);
         }
         if (comp(VerVCoord(HalVtx(HalPrv(scan)))[pln], 0.0, EPS) == 0)
         {
            MSD_lowKEF(HalPrv(scan), mate(HalPrv(scan)));
            return(0);
         }
         return(2);
      }
      else
      {
         maxv = MSD_getMaxVertexId(s);
         MSD_lowMEV(h, HalNxt(mate(h)), maxv++, VerVCoord(HalVtx(h))[0],
                    VerVCoord(HalVtx(h))[1],
                    VerVCoord(HalVtx(h))[2]);
         MSD_lowKEF(HalPrv(h), mate(HalPrv(h)));
      }
      return(1);
   }
   return(0);
}

HPTYPE MSD_lowTouchAxis(HPTYPE eptr, int pln)
{
   HPTYPE scan;

   scan = eptr;
   do
   {
      if (comp(VerVCoord(HalVtx(scan))[pln], 0.0, EPS) == 0)
      {
         return(scan);
      }
   } while ((scan = HalNxt(scan)) != eptr);
   return(HNIL);
}

HPTYPE MSD_lowTouchWire(HPTYPE eptr, int *cut, int pln)
{
   *cut = 0;
   while (mate(eptr) != HalNxt(eptr))
   {
      eptr = HalNxt(eptr);
   }
   if (comp(VerVCoord(HalVtx(HalNxt(eptr)))[pln], 0.0, EPS) == 0)
   {
      while (comp(VerVCoord(HalVtx(eptr))[pln], 0.0, EPS) == 0)
      {
         eptr = HalPrv(eptr);
         MSD_lowKEV(mate(HalNxt(eptr)), HalNxt(eptr));
      }
      *cut = 1;
   }
   return(eptr);
}
