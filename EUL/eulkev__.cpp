/* 
  PROJETO USPDesigner
  MODULO: EUL (Operadores de Euler)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: eulkev__.cpp
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
#include "mensagem.h"
#include "memvirtu.h"
#include "lowparam.h"
#include "lowmacro.h"
#include "lowsolid.h"
#include "eulerops.h"
#include "curvgeom.h"
#include "mancommd.h"
/* Operator kev is the inverse of lmev. It removes the edge pointed at by */
/* both he1 and he2, and "joins" the two vertices he1->vtx and he2->vtx   */
/* which must be distinct (but can of course, correspond with             */
/* geometrically identical points). Vertex he1->vtx is removed.           */

#ifdef __Extended_Version
void MSD_execKEV(void)
{
   int sn, fn, v1, v2, op;

   while (5 != sscanf(restbuf, "%d %d %d %d %d", &sn, &op, &fn, &v1, &v2))
   {
      printf("Solid F V1 V2\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highKEV(sn, fn, v1, v2);
}

#endif

int MSD_highKEV(Id sn, Id fn, Id v1, Id v2)
{
   SPTYPE s;
   FPTYPE f;
   HPTYPE h1;
   HPTYPE h2;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_KEV, sn);
      return(ERROR);
   }
   if ((f = MSD_getFace(s, fn)) == FNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouFace, NAM_KEV, fn, sn);
      return(ERROR);
   }
   if (MSD_getEdge(f, v1, v2, &h1) == LNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouArestaV, NAM_KEV, v1, v2, sn);
      return(ERROR);
   }
   if (MSD_getEdge(f, v2, v1, &h2) == LNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouArestaV, NAM_KEV, v2, v1, sn);
      return(ERROR);
   }
   if (HalWLoop(h1) != HalWLoop(h2))
   {
      fprintf(stderr, MEN_NaoArestaPonte, NAM_KEV, v1, v2, sn);
      return(ERROR);
   }
   if (HalNxt(h1) != h2)
   {
      fprintf(stderr, MEN_NaoVerticeTerminal, NAM_KEV, v2, sn);
      return(ERROR);
   }
   MSD_lowKEV(h2, h1);
   return(SUCCESS);
}

#ifdef __Extended_Version
void MSD_execJVKE(void)
{
   int sn, f, v1, v2;

   while (4 != sscanf(restbuf, "%d %d %d %d", &sn, &v1, &v2, &f))
   {
      printf("Solid V1 V2 F\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highJVKE(sn, v1, v2, f);
}

#endif

int MSD_highJVKE(Id sn, Id v1, Id v2, Id f)
{
   SPTYPE s;
   FPTYPE ff;
   HPTYPE he1;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_JVKE, sn);
      return(ERROR);
   }
   if ((ff = MSD_getFace(s, f)) == FNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouFace, NAM_JVKE, f, sn);
      return(ERROR);
   }
   if (MSD_getEdge(ff, v1, v2, &he1) == LNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouArestaV, NAM_JVKE, v1, v2, sn);
      return(ERROR);
   }
   if (HalNxt(he1) == he1)
   {
      fprintf(stderr, MEN_ArestaCircular, NAM_JVKE, v1, v2, sn);
      return(ERROR);
   }
   MSD_lowKEV(mate(he1), he1);
   return(SUCCESS);
}

#ifdef __Extended_Version
void MSD_execJEKV(void)
{
   int sn, v1, v2;

   while (3 != sscanf(restbuf, "%d %d %d", &sn, &v1, &v2))
   {
      printf("Solid V1 V2\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highJEKV(sn, v1, v2);
}

#endif

int MSD_highJEKV(Id sn, Id v1, Id v2)
{
   SPTYPE s;
   DPTYPE d;
   FPTYPE f;
   HPTYPE he1;
   short  flag_found;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_JEKV, sn);
      return(ERROR);
   }

   flag_found = 1;
   for (AllShellsSolid(s, d))
   {
      for (AllFacesShell(d, f))
      {
         if (MSD_getEdge(f, v1, v2, &he1))
         {
            flag_found = 0;
            break;
         }
      }
   }
   if (flag_found == 1)
   {
      fprintf(stderr, MEN_NaoEncontrouArestaV, NAM_JEKV, v1, v2, sn);
      return(ERROR);
   }
   MSD_lowKEV(HalNxt(he1), HalPrv(mate(he1)));
   return(SUCCESS);
}

#ifdef __Extended_Version
void MSD_execKEV2(void)
{
   int sn, v1, v2, f1, f2, op;

   while (6 != sscanf(restbuf, "%d %d %d %d %d %d", &sn, &op, &v1, &v2, &f1, &f2))
   {
      printf("kev2: Sid V1 V2 F1 F2\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highKEV2(sn, v1, v2, f1, f2);
}

#endif

int MSD_highKEV2(Id sn, Id v1, Id v2, Id f1, Id f2)
{
   SPTYPE s;
   FPTYPE ff1;
   FPTYPE ff2;
   HPTYPE he1;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_KEV2, sn);
      return(ERROR);
   }
   if ((ff1 = MSD_getFace(s, f1)) == FNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouFace, NAM_KEV2, f1, sn);
      return(ERROR);
   }
   if ((ff2 = MSD_getFace(s, f2)) == FNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouFace, NAM_KEV2, f2, sn);
      return(ERROR);
   }
   if (MSD_getHalfedge(ff1, ff2, v1, v2, &he1) == LNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouArestaV, NAM_KEV2, v1, v2, sn);
      return(ERROR);
   }
   MSD_lowKEV(he1, mate(he1));
   return(SUCCESS);
}

void MSD_lowKEV(HPTYPE he1, HPTYPE he2)
{
   SPTYPE s;
   DPTYPE d;
   LPTYPE loop1;
   LPTYPE loop2;
   HPTYPE he;
   HPTYPE nh1;
   HPTYPE nh2;
   VPTYPE vp1;
   VPTYPE vp2;
   Id     v1, v2 = 0, v3, v4, f1, f2, f3, f4;

   assert1(he1 == mate(he2));
   assert1(HalVtx(he1) != HalVtx(he2));
   d = FacFShell(LooLFace(HalWLoop(he1)));
   s = SheSSolids(d);

   if (Gdoinversion)
   {
      if ((HalNxt(he1) == he2) && (HalNxt(he2) != he1))
      {
         MSD_lowMEV(he2, he2, -1, 0.0, 0.0, 0.0);
         nh1 = HalNxt(he1);
         nh2 = HalPrv(he2);
         MSD_lowKEV(he1, he2);
         MSD_lowKEV(nh2, nh1);
         return;
      }
      v1 = VerVertexNo(HalVtx(he2));
      v4 = VerVertexNo(HalVtx(he1));
      v3 = VerVertexNo(HalVtx(HalNxt(HalNxt(he1))));
      v2 = (HalNxt(he2) == he1 ? v3 : VerVertexNo(HalVtx(HalNxt(HalNxt(he2)))));
      if (HalNxt(he1) == he2)
      {
         v2 = v3 = v1;
      }
      if (v2 == v4)
      {
         v2 = v1;
      }
      if (v3 == v4)
      {
         v3 = v1;
      }
      f1 = FacFaceNo(LooLFace(HalWLoop(he2)));
      f2 = FacFaceNo(LooLFace(HalWLoop(he1)));
      f4 = FacFaceNo(LooLFace(HalWLoop(mate(HalNxt(he1)))));
      f3 = HalNxt(he2) == he1 ? f4 : FacFaceNo(LooLFace(HalWLoop(mate(HalNxt(he2)))));
      MSD_lowAddEulerOp(s, MEV, v1, v2, v3, v4, f1, f2, f3, f4, VerVCoord(HalVtx(he1))[0],
                        VerVCoord(HalVtx(he1))[1],
                        VerVCoord(HalVtx(he1))[2], 1.0);
   }
   if (Gdodirect)
   {
      v1 = VerVertexNo(HalVtx(he1));
      v2 = VerVertexNo(HalVtx(he2));
      f1 = FacFaceNo(LooLFace(HalWLoop(he1)));
      f2 = FacFaceNo(LooLFace(HalWLoop(he2)));
      MSD_lowAddEulerOpRedo(s, KEV, v1, v2, f1, f2, 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0);
   }

#ifdef __curved_env
   if (he1->curv != (CUCURV *)NIL)
   {
      lputctag(he1, (CUCURV *)NIL);
   }
   if (he2->curv != (CUCURV *)NIL)
   {
      lputctag(he2, (CUCURV *)NIL);
   }
#endif

   for (he = HalNxt(he2); he != he1; he = HalNxt(mate(he)))
   {
      HalVtx(he) = HalVtx(he2);
   }

   vp2           = HalVtx(he2);
   vp1           = HalVtx(he1);
   VerVEdge(vp1) = HalNxt(he1) == he2 ? HalNxt(he2) : HalNxt(he1);
   VerVEdge(vp2) = HalNxt(he2) == he1 ? HalNxt(he1) : HalNxt(he2);
   LooLength((loop1 = HalWLoop(he1)))--;
   LooLength((loop2 = HalWLoop(he2)))--;

   MSD_lowDelElement(EDGE, (NPTYPE)HalEdg(he1), (NPTYPE)d);
   MSD_lowDelElement(VERTEX, (NPTYPE)HalVtx(he1), (NPTYPE)d);
   nh2 = HalNxt(he2);

   LooLEdg(loop1) = MSD_lowDelHE(he1);
   LooLEdg(loop2) = MSD_lowDelHE(he2);
   if (HalEdg(VerVEdge(vp2)) == ENIL)
   {
      VerVEdge(vp2) = HNIL;
   }

   MSD_addEulCoef(s, 0, 0, 0, -1, -1, 0);

   if (SetTrace)
   {
      if (TraceSolid(s))
      {
         fprintf(stderr, MEN_MSDAbortou, NAM_KEV);
         exit(1);
      }
   }
}

int MSD_execStructKEV(EulerOp *op)
{
   Id sid, id1, id2, id3, id4;

   sid = op->solidno;
   id1 = op->ip1;
   id2 = op->ip2;
   id3 = op->ip3;
   id4 = op->ip4;
   return(MSD_highKEV2(sid, id1, id2, id3, id4));
}

void MSD_printStructKEV(FILE *f, EulerOp *op)
{
   Id  sid, id1, id2, id3, id4;
   int com;

   sid = op->solidno;
   id1 = op->ip1;
   id2 = op->ip2;
   id3 = op->ip3;
   id4 = op->ip4;
   com = op->CommandCount;
   fprintf(f, "kev %d %d %d %d %d %d\n", sid, com, id1, id2, id3, id4);
}

int MSD_readStructKEV(FILE *f, EulerOp *op)
{
   int solidno, ip1, ip2, ip3, ip4, com;

   if (fscanf(f, "%d %d %d %d %d %d\n", &solidno, &com, &ip1, &ip2, &ip3, &ip4) != 6)
   {
      return(ERROR);
   }
   op->solidno      = solidno;
   op->CommandCount = com;
   op->ip1          = ip1;
   op->ip2          = ip2;
   op->ip3          = ip3;
   op->ip4          = ip4;
   op->ip5          = 0;
   op->ip6          = 0;
   op->ip7          = 0;
   op->ip8          = 0;
   op->fp1          = 0.0;
   op->fp2          = 0.0;
   op->fp3          = 0.0;
   op->fp4          = 0.0;
   return(SUCCESS);
}

int MSD_modifyStructKEV(EulerOp *op)
{
   int solidno;

   solidno     = MSD_lowCheckTranslatorTable(op->solidno, SOLID);
   op->solidno = solidno;
   return(SUCCESS);
}
