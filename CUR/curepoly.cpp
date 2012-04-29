/* 
  PROJETO USPDesigner
  MODULO: CUR (Curve Manipulation)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: curepoly.cpp
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
#include <stdlib.h>
#include "mensagem.h"
#include "memvirtu.h"
#include "lowparam.h"
#include "lowmacro.h"
#include "lowsolid.h"
#include "eulerops.h"
#include "mancommd.h"
#include "vectorop.h"
#include "curvgeom.h"
#include "shpshape.h"
#include "lowgeome.h"

#ifdef __curved_env__

 #ifdef __Extended_Version
void MSD_execEvaluateCurvePolyline(void)
{
   int  sn, fn, v1, v2, nsegm;
   char sname[20];

   while (5 != sscanf(restbuf, "%s %d %d %d %d", sname, &fn, &v1, &v2, &nsegm))
   {
      printf("EVAL_POLYLINE: sname fn v1 v2 nsegm\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   if ((sn = MSD_getSolidIdFromName(sname)) == ERROR)
   {
      fprintf(stderr, MEN_NaoEncontrouSolidNome, NAM_EvalEdge, sname);
      return;
   }
   MSD_highEvaluateCurvePolyline(sn, fn, v1, v2, nsegm);
}

int MSD_highEvaluateCurvePolyline(Id sn, Id fn, Id v1, Id v2, int nsegm)
{
   SPTYPE s;
   FPTYPE f;
   HPTYPE he;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_EvalEdge, sn);
      return(ERROR);
   }
   if ((f = MSD_getFace(s, fn)) == FNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouFace, NAM_EvalEdge, fn, sn);
      return(ERROR);
   }
   if (MSD_getEdge(f, v1, v2, &he) == LNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouArestaV, NAM_EvalEdge, v1, v2, sn);
      return(ERROR);
   }
   if (nsegm < 2)
   {
      fprintf(stderr, MEN_NumeroDeSegmentosInvalido, NAM_EvalEdge);
      return(ERROR);
   }
   MSD_lowEvaluateCurvePolyline(he, nsegm);
   MSD_lowSetNormal(s, TRUE);
   MSD_lowSetEdgeAngle(s);
   MSD_lowSetInfo(s);

   if (TraceBoundaryStructure())
   {
      fprintf(stderr, "eval: *** aborted ***\n", NAM_EvalEdge);
      exit(1);
   }

   return(SUCCESS);
}

 #endif

void MSD_lowEvaluateCurvePolyline(HPTYPE he, int nsegm)
{
   CURVE  *cu;
   LPOINT *p;
   int    j;
   real   del, del2, del3, x, y, z, deltax, delta2x, delta3x,
          deltay, delta2y, delta3y, deltaz, delta2z, delta3z;
   vector MGx, MGy, MGz, coefx, coefy, coefz;

 #ifndef __VIRTUAL_MEM
   VTYPE v;
 #else
   VPTYPE v;
 #endif

   maxv = MSD_getMaxVertexId(SheSSolids(FacFShell(LooLFace(HalWLoop(he)))));
   cu   = HalCurv(he);

 #ifndef __VIRTUAL_MEM
   veccopy(v.vcoord, cu->lpnt->pnt);
   if (MSD_lowVtxVtxContainment(&v, HalVtx(he)) == 0)
   {
      he = mate(he);
   }
 #else
   v = MSD_getMemVirtual(VERTEX);
   veccopy(VerVCoord(v), cu->lpnt->pnt);
   if (MSD_lowVtxVtxContainment(v, HalVtx(he)) == 0)
   {
      he = mate(he);
   }
   MSD_putMemVirtual(v, VERTEX);
 #endif
   he = HalNxt(he);

   p   = cu->lpnt;
   del = 1 / (real)nsegm;

   Mc[0][0] = -1.0;
   Mc[0][1] = 3.0;
   Mc[0][2] = -3.0;
   Mc[0][3] = 1.0;
   Mc[1][0] = 3.0;
   Mc[1][1] = -6.0;
   Mc[1][2] = 3.0;
   Mc[1][3] = 0.0;
   Mc[2][0] = -3.0;
   Mc[2][1] = 3.0;
   Mc[2][2] = 0.0;
   Mc[2][3] = 0.0;
   Mc[3][0] = 1.0;
   Mc[3][1] = 0.0;
   Mc[3][2] = 0.0;
   Mc[3][3] = 0.0;

   mattranspose(Mc, Mc);

   MGx[0] = p->pnt[0];
   MGx[1] = p->pnxt->pnt[0];
   MGx[2] = p->pnxt->pnxt->pnt[0];
   MGx[3] = p->pnxt->pnxt->pnxt->pnt[0];
   vecmult(coefx, MGx, Mc);

   MGy[0] = p->pnt[1];
   MGy[1] = p->pnxt->pnt[1];
   MGy[2] = p->pnxt->pnxt->pnt[1];
   MGy[3] = p->pnxt->pnxt->pnxt->pnt[1];
   vecmult(coefy, MGy, Mc);

   MGz[0] = p->pnt[2];
   MGz[1] = p->pnxt->pnt[2];
   MGz[2] = p->pnxt->pnxt->pnt[2];
   MGz[3] = p->pnxt->pnxt->pnxt->pnt[2];
   vecmult(coefz, MGz, Mc);

   del2 = del * del;
   del3 = del2 * del;

   x       = coefx[3];
   deltax  = coefx[0] * del3 + coefx[1] * del2 + coefx[2] * del;
   delta2x = 6 * coefx[0] * del3 + 2 * coefx[1] * del2;
   delta3x = 6 * coefx[0] * del3;

   y       = coefy[3];
   deltay  = coefy[0] * del3 + coefy[1] * del2 + coefy[2] * del;
   delta2y = 6 * coefy[0] * del3 + 2 * coefy[1] * del2;
   delta3y = 6 * coefy[0] * del3;

   z       = coefz[3];
   deltaz  = coefz[0] * del3 + coefz[1] * del2 + coefz[2] * del;
   delta2z = 6 * coefz[0] * del3 + 2 * coefz[1] * del2;
   delta3z = 6 * coefz[0] * del3;

   for (j = 1; j <= nsegm - 1; j++)
   {
      x       += deltax;
      deltax  += delta2x;
      delta2x += delta3x;

      y       += deltay;
      deltay  += delta2y;
      delta2y += delta3y;

      z       += deltaz;
      deltaz  += delta2z;
      delta2z += delta3z;

      MSD_lowMEV(mate(HalPrv(he)), he, ++maxv, x, y, z);
      MSD_lowSetCurveAtributeToEdge(HalPrv(he), cu);
   }
}

#endif
