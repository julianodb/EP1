/* 
  PROJETO USPDesigner
  MODULO: DIS (Hidden Line Elimination)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: dishintr.cpp
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
#include "shpshape.h"
#include "disdispl.h"

void MSD_lowCameraDisplayIntersection(void)
{
   CTYPE *cptr;

   if (camptr->mode == 3)
   {
      cptr   = camptr;
      camptr = &camera3;
      MSD_lowSetCamera3View(1, cptr);
      MSD_lowDisplayHidden(camptr);
      MSD_lowDisplayIntersection();

      MSD_lowSetCamera3View(2, cptr);
      MSD_lowDisplayHidden(camptr);
      MSD_lowDisplayIntersection();

      MSD_lowSetCamera3View(3, cptr);
      MSD_lowDisplayHidden(camptr);
      MSD_lowDisplayIntersection();

      MSD_lowSetCamera3View(4, cptr);
      MSD_lowDisplayHidden(camptr);
      MSD_lowDisplayIntersection();

      MSD_lowSetCamera3View(0, cptr);
   }
   else
   {
      MSD_lowDisplayHidden(camptr);
      MSD_lowDisplayIntersection();
   }
}

void MSD_lowDisplayIntersection(void)
{
   SPTYPE o1;
   SPTYPE o2;
   DPTYPE d1;
   DPTYPE d2;
   FPTYPE f1;
   FPTYPE f2;

   for (AllSolids(o1))
   {
      if (SolDsp(o1) == FALSE)
      {
         continue;
      }
      for (o2 = SolNxt(o1); o2 != SNIL; o2 = SolNxt(o2))
      {
         if (SolDsp(o2) == FALSE)
         {
            continue;
         }
         if (!MSD_lowBoxBoxIntersection(&SolBox(o1), &SolBox(o2)))
         {
            continue;
         }
         g_pcolor(MSD_lowGetIntersectionColor(o1, o2));
         if (camptr->dash)
         {
            for (AllShellsSolid(o1, d1))
            {
               for (AllFacesShell(d1, f1))
               {
                  for (AllShellsSolid(o2, d2))
                  {
                     for (AllFacesShell(d2, f2))
                     {
                        if (breakflg)
                        {
                           return;
                        }
                        if (MSD_lowBoxBoxIntersection(&FacBox(f1), &FacBox(f2)))
                        {
                           MSD_lowIntersectionFaceFace(o1, f1, o2, f2);
                        }
                     }
                  }
               }
            }
         }
         else
         {
            for (AllShellsSolid(o1, d1))
            {
               for (AllFacesShell(d1, f1))
               {
                  if (FacFVisi(f1))
                  {
                     for (AllShellsSolid(o2, d2))
                     {
                        for (AllFacesShell(d2, f2))
                        {
                           if (FacFVisi(f2))
                           {
                              if (breakflg)
                              {
                                 return;
                              }
                              if (MSD_lowBoxBoxIntersection(&FacBox(f1), &FacBox(f2)))
                              {
                                 MSD_lowIntersectionFaceFace(o1, f1, o2, f2);
                              }
                           }
                        }
                     }
                  }
               }
            }
         }
      }
   }
}

/***************************************************************************/
/* Determina a interseccao entre duas faces f1ptr e f2ptr                  */
/***************************************************************************/
void MSD_lowIntersectionFaceFace(SPTYPE o1ptr, FPTYPE f1ptr,
                                 SPTYPE o2ptr, FPTYPE f2ptr)
{
   if (MSD_lowIntersectionParallelFaces(f1ptr, f2ptr) == FALSE)
   {
      divedptr = divedge = (DEDGTYPE *)workbuf;
      divednum = 0;
      MSD_lowIntersectionFaceEdge(f1ptr, f2ptr);
      MSD_lowIntersectionFaceEdge(f2ptr, f1ptr);
      if (divednum != 0)
      {
         if (MSD_lowIntersectionSeparateMinMax() == TRUE)
         {
            MSD_lowUpdateDEBox();
            MSD_lowEdgeEdgeIntersection(SNIL, ENIL, f1ptr, f2ptr);
            MSD_lowEdgeFaceIntersection(o1ptr, o2ptr, f1ptr, f2ptr);
            MSD_lowSortDividedEdge();
            MSD_lowSetEdgeVisibility('i', f1ptr, f2ptr, (real *)NIL);
            MSD_lowDisplayDividedEdge(camptr, ENIL);
         }
      }
   }
}

/***************************************************************************/
/* Determina se duas faces sao paralelas ou nao.                           */
/***************************************************************************/
int MSD_lowIntersectionParallelFaces(FPTYPE f1ptr, FPTYPE f2ptr)
{
   vector v, f1, f2;

   veccopy(f1, FacFeq(f1ptr));
   veccopy(f2, FacFeq(f2ptr));
   vecminus(v, f1, f2);
   if (!(sqrt(dot(v, v)) < abseps))
   {
      vecplus(v, f1, f2);
      if (!(sqrt(dot(v, v)) < abseps))
      {
         return(FALSE);
      }
   }
   return(TRUE);
}

/***************************************************************************/
/* Determina a aresta de interseccao entre duas faces. A aresta pertence   */
/*  a face f2ptr e deve ser armazenada na estrutura EDIV.                  */
/***************************************************************************/
void MSD_lowIntersectionFaceEdge(FPTYPE f1ptr, FPTYPE f2ptr)
{
   HPTYPE e2ptr;
   HPTYPE efim;
   HPTYPE dummy;
   real   ps, pe;
   vector pos, sv, ev;

   e2ptr = efim = LooLEdg(FacFLOut(f2ptr));
   do
   {
      veccopy(sv, VerVCoord(HalVtx(e2ptr)));
      veccopy(ev, VerVCoord(HalVtx(HalNxt(e2ptr))));
      ps = dot(sv, FacFeq(f1ptr)) + FacFeq(f1ptr)[3];
      pe = dot(ev, FacFeq(f1ptr)) + FacFeq(f1ptr)[3];
      if (fabs(ps - pe) > abseps)
      {
         if (!((ps < -abseps) && (pe < -abseps)))
         {
            if (!((ps > abseps) && (pe > abseps)))
            {
               calc_p(ev, sv, ps / (ps - pe), pos);
               if (MSD_lowIsInsideFace(pos, f1ptr, &dummy))
               {
                  MSD_lowAddEdgeDivision(pos);
               }
            }
         }
      }
   } while ((e2ptr = HalNxt(e2ptr)) != efim);
}

/***************************************************************************/
/* Determina a maior distancia entre os pontos de interseccao. Se for      */
/*  maior que abseps, entao o recorte deve ser processado.                 */
/***************************************************************************/
int MSD_lowIntersectionSeparateMinMax(void)
{
   real     maxdis, max, min;
   DEDGTYPE *minptr, *maxptr, *minp, *maxp, *dptr;
   int      idx, i, j;

   maxdis = 0.0;
   for (i = 0; i < 3; ++i)
   {
      maxp = minp = dptr = divedge;
      max  = min = dptr->devtx[i];
      for (j = 1; j < divednum; ++j)
      {
         ++dptr;
         if (max < dptr->devtx[i])
         {
            max = (maxp = dptr)->devtx[i];
         }
         if (min > dptr->devtx[i])
         {
            min = (minp = dptr)->devtx[i];
         }
      }
      if (max - min > maxdis)
      {
         maxdis = max - min;
         idx    = i;
         minptr = minp;
         maxptr = maxp;
      }
   }
   if (maxdis > abseps)
   {
      if (MSD_lowClipDisplayTransformation(minptr->devtx, maxptr->devtx, desvtx, deevtx))
      {
         MSD_lowAddEdgeDivision(desvtx);
         MSD_lowAddEdgeDivision(deevtx);
         MSD_lowIntersectionClipDropCoord(idx);
         return(TRUE);
      }
   }
   return(FALSE);
}

/***************************************************************************/
/* Realiza o recorte da aresta de interseccao de acordo com o eixo de      */
/*  maior distancia.                                                       */
/***************************************************************************/
void MSD_lowIntersectionClipDropCoord(int idx)
{
   DEDGTYPE *dptr;
   real     max, min;
   int      i;

   if ((max = desvtx[idx]) < (min = deevtx[idx]))
   {
      max = min;
      min = desvtx[idx];
   }
   for (dptr = divedge, i = 0; i < divednum - 2; ++dptr, ++i)
   {
      if ((dptr->devtx[idx] < min) || (dptr->devtx[idx] > max))
      {
         veccopy(dptr->devtx, desvtx);
      }
   }
}
