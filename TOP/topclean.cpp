/* 
  PROJETO USPDesigner
  MODULO: TOP (Operadores de Nivel Intermediario)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: topclean.cpp
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
#include "mensagem.h"
#include "memvirtu.h"
#include "lowparam.h"
#include "lowmacro.h"
#include "lowsolid.h"
#include "vectorop.h"
#include "eulerops.h"
#include "lowgeome.h"
#include "shpshape.h"
#include "mancommd.h"

#ifdef __Extended_Version
void MSD_execNameCleanSolid(void)
{
   char name[30];

   while (1 != sscanf(restbuf, "%s", name))
   {
      printf("Nome\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highNameCleanSolid(name);
}

#endif

int MSD_highNameCleanSolid(char *name)
{
   int sn;

   if ((sn = MSD_getSolidIdFromName(name)) == -1)
   {
      fprintf(stderr, MEN_NaoEncontrouSolidNome, NAM_CleanSolid, name);
      return(ERROR);
   }
   return(MSD_highCleanSolid(sn));
}

#ifdef __Extended_Version
void MSD_execCleanSolid(void)
{
   int sn;

   while (1 != sscanf(restbuf, "%d", &sn))
   {
      printf("clean: sn");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highCleanSolid(sn);
}

#endif

int MSD_highCleanSolid(Id sn)
{
   SPTYPE optr;

   if ((optr = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_CleanSolid, sn);
      return(ERROR);
   }
   MSD_lowCleanSolid(optr);
   return(SUCCESS);
}

void MSD_lowCleanSolid(SPTYPE s)
{
   DPTYPE d;
   FPTYPE f1;
   FPTYPE f2;
   FPTYPE f3;
   LPTYPE l1;
   LPTYPE l3;
   EPTYPE e1;
   EPTYPE e3;
   HPTYPE he1;
   HPTYPE he2;
   VPTYPE v1;
   VPTYPE v2;
   VPTYPE v3;
   int    flag1;

   /* step 0: remove null edges */
   for (AllShellsSolid(s, d))
   {
      for (e1 = SheSEdges(d); e1 != ENIL;)
      {
         flag1 = 0;
         if (MSD_lowVtxVtxContainment(HalVtx(EdgHe1(e1)), HalVtx(EdgHe2(e1))))
         {
            if (HalVtx(EdgHe1(e1)) != HalVtx(EdgHe2(e1)))
            {
               e3    = EdgNextE(e1);
               flag1 = 1;
               MSD_lowKEV(EdgHe1(e1), EdgHe2(e1));
            }
         }
         e1 = (flag1 == 0) ? (EdgNextE(e1)) : e3;
      }
   }

   /* step 0c: remove null faces */
   for (AllShellsSolid(s, d))
   {
      for (f1 = SheSFaces(d); f1 != FNIL;)
      {
         flag1 = 0;
         if (vecnull(FacFeq(f1), EPS))
         {
            f3    = FacNextF(f1);
            flag1 = 1;
            MSD_lowMakeFaceToPoint(f1);
         }
         f1 = (flag1 == 0) ? (FacNextF(f1)) : f3;
      }
   }

   /* step 1: remove edges that belong to faces with same normals */
   for (AllShellsSolid(s, d))
   {
      for (e1 = SheSEdges(d); e1 != ENIL;)
      {
         flag1 = 0;
         f1    = LooLFace(HalWLoop(EdgHe1(e1)));
         f2    = LooLFace(HalWLoop(EdgHe2(e1)));
         if ((f1 != f2) && MSD_lowFacFacEqualEqs(f1, f2))
         {
            e3    = EdgNextE(e1);
            flag1 = 1;
            MSD_lowKEF(EdgHe1(e1), EdgHe2(e1));
         }
         e1 = (flag1 == 0) ? (EdgNextE(e1)) : e3;
      }
   }

   /* step 2: remove edges occurring twice at a face */
   for (AllShellsSolid(s, d))
   {
      for (e1 = SheSEdges(d); e1 != ENIL;)
      {
         flag1 = 0;
         if (HalWLoop(EdgHe1(e1)) == HalWLoop(EdgHe2(e1)))
         {
            if (HalNxt(EdgHe2(e1)) == EdgHe1(e1))
            {
               e3    = EdgNextE(e1);
               flag1 = 1;
               MSD_lowKEV(EdgHe1(e1), EdgHe2(e1));
            }
            else
            if (HalNxt(EdgHe1(e1)) == EdgHe2(e1))
            {
               e3    = EdgNextE(e1);
               flag1 = 1;
               MSD_lowKEV(EdgHe2(e1), EdgHe1(e1));
            }
            else
            {
               e3    = EdgNextE(e1);
               flag1 = 1;
               v2    = HalVtx(EdgHe2(e1));
               l1    = HalWLoop(EdgHe1(e1));
               MSD_lowKEMR(EdgHe1(e1), EdgHe2(e1));
               if (FacFLOut(LooLFace(l1)) == l1)
               {
                  if (MSD_lowLooVtxContainment(l1, v2, getdrop(FacFeq(LooLFace(l1)))) != 1)
                  {
                     FacFLOut(LooLFace(l1)) = HalWLoop(VerVEdge(v2));
                  }
               }
            }
         }
         e1 = (flag1 == 0) ? (EdgNextE(e1)) : e3;
      }
   }

   /* step 2a: remove null rings */
   for (AllShellsSolid(s, d))
   {
      for (AllFacesShell(d, f1))
      {
         for (l1 = FacFLoops(f1); l1 != LNIL;)
         {
            he1   = LooLEdg(l1);
            flag1 = 0;
            if ((HalNxt(he1) == he1) && (FacFLOut(f1) != l1))
            {
               l3    = LooNextL(l1);
               flag1 = 1;
               he2   = LooLEdg(FacFLOut(f1));
               MSD_lowMEKR(he2, he1);
               MSD_lowKEV(HalPrv(he2), mate(HalPrv(he2)));
            }
            l1 = (flag1 == 0) ? (LooNextL(l1)) : l3;
         }
      }
   }

   /* step 3: remove vertices connecting two colinear edges */
   for (AllShellsSolid(s, d))
   {
      for (v1 = SheSVerts(d); v1 != VNIL;)
      {
         flag1 = 0;
         he1   = VerVEdge(v1);
         if (he1 != HNIL)
         {
            he2 = HalNxt(mate(he1));
            if ((he1 != he2) && (he1 == HalNxt(mate(he2))))
            {
               if (MSD_lowEdgEdgColinear(HalEdg(he1), HalEdg(he2)))
               {
                  v3    = VerNextV(v1);
                  flag1 = 1;
                  MSD_lowKEV(he1, mate(he1));
               }
            }
            v1 = (flag1 == 0) ? (VerNextV(v1)) : v3;
         }
         else
         {
            v1 = VerNextV(v1);
         }
      }
   }

   MSD_lowSetNormal(s, TRUE);
   MSD_lowSetEdgeAngle(s);
}
