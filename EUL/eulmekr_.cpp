/* 
  PROJETO USPDesigner
  MODULO: EUL (Operadores de Euler)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: eulmekr_.cpp
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
#include "mancommd.h"

#ifdef __Extended_Version
void MSD_execMEKR(void)
{
   int sn, fn, v1, v2, op;

   while (5 != sscanf(restbuf, "%d %d %d %d %d", &sn, &op, &fn, &v1, &v2))
   {
      printf("mekr: Sid F V1 V2\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highMEKR(sn, fn, v1, v2);
}

#endif

int MSD_highMEKR(Id sn, Id fn, Id v1, Id v2)
{
   SPTYPE s;
   FPTYPE f;
   LPTYPE l1;
   LPTYPE l2;
   HPTYPE he1;
   HPTYPE he2;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_MEKR, sn);
      return(ERROR);
   }
   if ((f = MSD_getFace(s, fn)) == FNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouFace, NAM_MEKR, fn, sn);
      return(ERROR);
   }
   if ((l1 = MSD_getLoop(f, v1, &he1)) == LNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouVertice, NAM_MEKR, v1, sn);
      return(ERROR);
   }
   if ((l2 = MSD_getLoop(f, v2, &he2)) == LNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouVertice, NAM_MEKR, v2, sn);
      return(ERROR);
   }
   if (l1 == l2)
   {
      fprintf(stderr, MEN_VerticesEmLoopsDistintos, NAM_MEKR, v1, v2, sn);
      return(ERROR);
   }
   MSD_lowMEKR(he1, he2);
   return(SUCCESS);
}

#ifdef __Extended_Version
void MSD_execMEKR2(void)
{
   int sn, v1, v2, v3, v4, f1, f2, f3, op;

   while (9 != sscanf(restbuf, "%d %d %d %d %d %d %d %d %d", &sn, &op, &v1, &v2,
                      &v3, &v4, &f1, &f2, &f3))
   {
      printf("smekr: Sid V1 V2 V3 V4 F1 F2 F3\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highMEKR2(sn, v1, v2, v3, v4, f1, f2, f3);
}

#endif

int MSD_highMEKR2(Id sn, Id v1, Id v2, Id v3, Id v4, Id f1, Id f2, Id f3)
{
   SPTYPE s;
   FPTYPE ff1;
   FPTYPE ff2;
   FPTYPE ff3;
   HPTYPE he1;
   HPTYPE he2;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_MEKR2, sn);
      return(ERROR);
   }
   if ((ff1 = MSD_getFace(s, f1)) == FNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouFace, NAM_MEKR2, f1, sn);
      return(ERROR);
   }
   if ((ff2 = MSD_getFace(s, f2)) == FNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouFace, NAM_MEKR2, f2, sn);
      return(ERROR);
   }
   if ((ff3 = MSD_getFace(s, f3)) == FNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouFace, NAM_MEKR2, f3, sn);
      return(ERROR);
   }
   if (MSD_getHalfedge(ff1, ff2, v1, v3, &he1) == LNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouArestaV, NAM_MEKR2, v1, v2, sn);
      return(ERROR);
   }
   if (MSD_getHalfedge(ff1, ff3, v2, v4, &he2) == LNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouArestaV, NAM_MEKR2, v1, v3, sn);
      return(ERROR);
   }
   MSD_lowMEKR(he1, he2);
   return(SUCCESS);
}

void MSD_lowMEKR(HPTYPE he1, HPTYPE he2)
{
   SPTYPE s;
   DPTYPE d;
   FPTYPE f;
   LPTYPE l1;
   LPTYPE l2;
   EPTYPE newedge;
   HPTYPE nhe1;
   HPTYPE nhe2;
   HPTYPE he;
   VPTYPE v1;
   VPTYPE v2;
   Id     va, vb, f1;
   Id     vc, vd, f2, f3;

   assert1((l1 = HalWLoop(he1)) != (l2 = HalWLoop(he2)));
   assert1((f = LooLFace(l1)) == LooLFace(l2));

   d = FacFShell(f);
   s = SheSSolids(d);
   if (Gdoinversion)
   {
      f1 = FacFaceNo(f);
      /* if (l1 != FacFLOut(f) && l2 != FacFLOut(f)) { */
      he = LooLEdg(FacFLOut(f));
      va = VerVertexNo(HalVtx(he));
      vb = HalNxt(he) != he ? VerVertexNo(HalVtx(HalNxt(he))) : 0;
      MSD_lowAddEulerOp(s, RMOVE, va, vb, f1, f1, TRUE, 0, 0, 0, 0.0, 0.0, 0.0, 0.0);
      /*  } */
      va = l1 == FacFLOut(f) ? VerVertexNo(HalVtx(he1)) : VerVertexNo(HalVtx(he2));
      vb = l1 == FacFLOut(f) ? VerVertexNo(HalVtx(he2)) : VerVertexNo(HalVtx(he1));
      MSD_lowAddEulerOp(s, KEMR, f1, vb, va, 0, 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0);
   }

   if (Gdodirect)
   {
      va = VerVertexNo(HalVtx(he1));
      vb = VerVertexNo(HalVtx(he2));
      vc = VerVertexNo(HalVtx(HalNxt(he1)));
      vd = VerVertexNo(HalVtx(HalNxt(he2)));
      f1 = FacFaceNo(LooLFace(HalWLoop(he1)));
      f2 = HalEdg(he1) == ENIL ? f1 : FacFaceNo(LooLFace(HalWLoop(mate(he1))));
      f3 = HalEdg(he1) == ENIL ? f1 : FacFaceNo(LooLFace(HalWLoop(mate(he2))));
      MSD_lowAddEulerOpRedo(s, MEKR, va, vb, vc, vd, f1, f2, f3, 0, 0.0, 0.0, 0.0, 0.0);
   }

   newedge            = (EPTYPE)MSD_lowNewElement(EDGE, (NPTYPE)d);
   EdgEdgeNo(newedge) = -1;
   he = LooLEdg(l2);
   do
   {
      HalWLoop(he) = l1;
   } while ((he = HalNxt(he)) != LooLEdg(l2));

   v1   = HalVtx(he1);
   v2   = HalVtx(he2);
   nhe1 = MSD_lowAddHE(newedge, v1, he1, PLUS);
   nhe2 = MSD_lowAddHE(newedge, v2, he2, MINUS);
   if (VerVEdge(v1) == HNIL)
   {
      VerVEdge(v1) = nhe1;
   }
   if (VerVEdge(v2) == HNIL)
   {
      VerVEdge(v2) = nhe2;
   }

   HalNxt(nhe1) = he2;
   HalNxt(nhe2) = he1;
   HalPrv(he2)  = nhe1;
   HalPrv(he1)  = nhe2;

   LooLength(l1) += LooLength(l2) + 2;
   if (FacFLOut(f) == l2)
   {
      FacFLOut(f) = l1;
   }
   MSD_lowDelElement(LOOP, (NPTYPE)l2, (NPTYPE)LooLFace(l2));

   MSD_addEulCoef(s, 0, 0, -1, 1, 0, 0);

   if (SetTrace)
   {
      if (TraceSolid(s))
      {
         fprintf(stderr, MEN_MSDAbortou, NAM_MEKR2);
         exit(1);
      }
   }
}

int MSD_execStructMEKR(EulerOp *op)
{
   Id sid, id1, id2, id3, id4, id5, id6, id7;

   sid = op->solidno;
   id1 = op->ip1;
   id2 = op->ip2;
   id3 = op->ip3;
   id4 = op->ip4;
   id5 = op->ip5;
   id6 = op->ip6;
   id7 = op->ip7;
   return(MSD_highMEKR2(sid, id1, id2, id3, id4, id5, id6, id7));
}

void MSD_printStructMEKR(FILE *f, EulerOp *op)
{
   Id  sid, id1, id2, id3, id4, id5, id6, id7;
   int com;

   sid = op->solidno;
   id1 = op->ip1;
   id2 = op->ip2;
   id3 = op->ip3;
   id4 = op->ip4;
   id5 = op->ip5;
   id6 = op->ip6;
   id7 = op->ip7;
   com = op->CommandCount;
   fprintf(f, "mekr %d %d %d %d %d %d %d %d %d\n", sid, com, id1, id2,
           id3, id4, id5, id6, id7);
}

int MSD_readStructMEKR(FILE *f, EulerOp *op)
{
   int solidno, ip1, ip2, ip3, ip4, ip5, ip6, ip7, com;

   if (fscanf(f, "%d %d %d %d %d %d %d %d %d\n", &solidno, &com, &ip1, &ip2,
              &ip3, &ip4, &ip5, &ip6, &ip7) != 9)
   {
      return(ERROR);
   }
   op->solidno      = solidno;
   op->CommandCount = com;
   op->ip1          = ip1;
   op->ip2          = ip2;
   op->ip3          = ip3;
   op->ip4          = ip4;
   op->ip5          = ip5;
   op->ip6          = ip6;
   op->ip7          = ip7;
   op->ip8          = 0;
   op->fp1          = 0.0;
   op->fp2          = 0.0;
   op->fp3          = 0.0;
   op->fp4          = 0.0;
   return(SUCCESS);
}

int MSD_modifyStructMEKR(EulerOp *op)
{
   int solidno;

   solidno     = MSD_lowCheckTranslatorTable(op->solidno, SOLID);
   op->solidno = solidno;
   return(SUCCESS);
}
