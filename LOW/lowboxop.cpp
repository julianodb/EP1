/* 
  PROJETO USPDesigner
  MODULO: LOW (Operadores Basicos)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: lowboxop.cpp
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
#include "memvirtu.h"
#include "lowparam.h"
#include "lowmacro.h"
#include "lowsolid.h"
#include "vectorop.h"
#include "disdispl.h"

/* Make box for solid from face boxes */
void MSD_lowSolidBoxFromBoxes(SPTYPE s, BTYPE *b)
{
   DPTYPE d;
   FPTYPE f;
   int    i;

   for (i = 0; i < 3; i++)
   {
      b->max[i] = INF;
      b->min[i] = -INF;
   }
   for (AllShellsSolid(s, d))
   {
      for (AllFacesShell(d, f))
      {
         MSD_lowUpdateBox(&(FacBox(f)), b);
      }
   }
}

int MSD_lowVertexInBox(BTYPE *b, real x, real y, real z)
{
   if ((x < b->min[0]) || (x > b->max[0]))
   {
      return(FALSE);
   }
   if ((y < b->min[1]) || (y > b->max[1]))
   {
      return(FALSE);
   }
   if ((z < b->min[2]) || (z > b->max[2]))
   {
      return(FALSE);
   }
   return(TRUE);
}

/* Does edge intersect box? */
int MSD_lowEdgeBoxIntersection(BTYPE *b, EPTYPE e)
{
   int    i;
   VPTYPE v1;
   VPTYPE v2;

   v1 = HalVtx(EdgHe1(e));
   v2 = HalVtx(EdgHe2(e));
   for (i = 0; i < 3; i++)
   {
      if (MSD_lowEdgeBoxIntersectionAux(VerVCoord(v1)[i], VerVCoord(v2)[i],
                                        b->min[i], b->max[i]) == 0)
      {
         return(0);
      }
   }
   return(1);
}

static int MSD_lowEdgeBoxIntersectionAux(real x1, real x2, real bx1, real bx2)
{
   if (x1 > x2)
   {
      if (x1 < bx1)
      {
         return(0);
      }
      if (x2 > bx2)
      {
         return(0);
      }
   }
   else
   {
      if (x2 < bx1)
      {
         return(0);
      }
      if (x1 < bx2)
      {
         return(0);
      }
   }
   return(1);
}

/* Check intersection of boxes */
int MSD_lowBoxBoxIntersection(BTYPE *b1, BTYPE *b2)
{
   int i;

   for (i = 0; i < 3; i++)
   {
      if (b1->min[i] > b2->max[i])
      {
         return(0);
      }
      if (b1->max[i] < b2->min[i])
      {
         return(0);
      }
   }
   return(1);
}

/* Calculate box for solid */
void MSD_lowSolidBox(SPTYPE s, BTYPE *bx)
{
   DPTYPE d;
   VPTYPE v;
   int    i;

   v = SheSVerts(SolSShells(s));
   veccopy(bx->min, VerVCoord(v));
   veccopy(bx->max, VerVCoord(v));
   for (AllShellsSolid(s, d))
   {
      for (AllVerticesShell(d, v))
      {
         for (i = 0; i < 3; i++)
         {
            if (VerVCoord(v)[i] < bx->min[i])
            {
               bx->min[i] = VerVCoord(v)[i];
            }
            if (VerVCoord(v)[i] > bx->max[i])
            {
               bx->max[i] = VerVCoord(v)[i];
            }
         }
      }
   }
   MSD_lowWidenBox(bx, CONTBVEPS);
}

void MSD_lowShellBox(DPTYPE d)
{
   VPTYPE v;
   BTYPE  *b;
   int    i;

   SetShellBoxOK(d);
   v = SheSVerts(d);
   b = &(SheBox(d));
   veccopy(b->min, VerVCoord(v));
   veccopy(b->max, VerVCoord(v));
   for (AllVerticesShell(d, v))
   {
      for (i = 0; i < 3; i++)
      {
         if (VerVCoord(v)[i] < b->min[i])
         {
            b->min[i] = VerVCoord(v)[i];
         }
         if (VerVCoord(v)[i] > b->max[i])
         {
            b->max[i] = VerVCoord(v)[i];
         }
      }
   }
   MSD_lowWidenBox(b, CONTBVEPS);
}

/* calculate box of a face */
void MSD_lowFaceBox(FPTYPE f)
{
   BTYPE  *b;
   VPTYPE v;
   LPTYPE l;
   HPTYPE he;
   int    i;

   SetFaceBoxOK(f);
   b = &(FacBox(f));
   veccopy(b->min, VerVCoord(HalVtx(LooLEdg(FacFLOut(f)))));
   veccopy(b->max, VerVCoord(HalVtx(LooLEdg(FacFLOut(f)))));
   for (AllLoopsFace(f, l))
   {
      he = LooLEdg(l);
      do
      {
         v = HalVtx(he);
         for (i = 0; i < 3; i++)
         {
            if (VerVCoord(v)[i] < b->min[i])
            {
               b->min[i] = VerVCoord(v)[i];
            }
            if (VerVCoord(v)[i] > b->max[i])
            {
               b->max[i] = VerVCoord(v)[i];
            }
         }
      } while ((he = HalNxt(he)) != LooLEdg(l));
   }
   MSD_lowWidenBox(b, CONTBVEPS);
}

void MSD_lowEdgeBox(BTYPE *b, EPTYPE e)
{
   VPTYPE v1;
   VPTYPE v2;
   int    i;

   v1 = HalVtx(EdgHe1(e));
   v2 = HalVtx(EdgHe2(e));
   for (i = 0; i < 3; i++)
   {
      if (VerVCoord(v1)[i] > VerVCoord(v2)[i])
      {
         b->min[i] = VerVCoord(v2)[i];
         b->max[i] = VerVCoord(v1)[i];
      }
      else
      {
         b->max[i] = VerVCoord(v2)[i];
         b->min[i] = VerVCoord(v1)[i];
      }
   }
   MSD_lowWidenBox(b, CONTBVEPS);
}

/* Calculate boxes of a solid */
void MSD_lowEvaluateSolidBoxes(SPTYPE s)
{
   DPTYPE d;
   FPTYPE f;

   for (AllShellsSolid(s, d))
   {
      MSD_lowShellBox(d);
      for (AllFacesShell(d, f))
      {
         MSD_lowFaceBox(f);
      }
   }
}

/* Widen box by eps */
void MSD_lowWidenBox(BTYPE *b, real eps)
{
   int i;

   for (i = 0; i < 3; i++)
   {
      b->min[i] -= eps;
      b->max[i] += eps;
   }
}

/* Update box b */
void MSD_lowUpdateBox(BTYPE *b1, BTYPE *b)
{
   int i;

   for (i = 0; i < 3; i++)
   {
      if (b1->min[i] < b->min[i])
      {
         b->min[i] = b1->min[i];
      }
      if (b1->max[i] > b->max[i])
      {
         b->max[i] = b1->max[i];
      }
   }
}

/* Translate box b by dx, dy, dz */
void MSD_lowTranslateBox(BTYPE *b, real dx, real dy, real dz)
{
   b->min[0] -= dx;
   b->min[1] -= dy;
   b->min[2] -= dz;
   b->max[0] += dx;
   b->max[1] += dy;
   b->max[2] += dz;
}

void MSD_lowUpdateDEBox(void)
{
   int i;

   for (i = 0; i < 3; ++i)
   {
      if (desvtx[i] < deevtx[i])
      {
         debox.min[i] = desvtx[i] - abseps;
         debox.max[i] = deevtx[i] + abseps;
      }
      else
      {
         debox.min[i] = deevtx[i] - abseps;
         debox.max[i] = desvtx[i] + abseps;
      }
   }
}
