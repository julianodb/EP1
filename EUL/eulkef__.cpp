/* 
  PROJETO USPDesigner
  MODULO: EUL (Operadores de Euler)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: eulkef__.cpp
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

#ifdef __Extended_Version
void MSD_execKEF(void)
{
   int sn, v1, v2, fn, op;

   while (5 != sscanf(restbuf, "%d %d %d %d %d", &sn, &op, &v1, &v2, &fn))
   {
      printf("Solid V1 V2 F\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highKEF(sn, v1, v2, fn);
}

#endif

int MSD_highKEF(Id sn, Id v1, Id v2, Id fn)
{
   SPTYPE s;
   FPTYPE f1;
   HPTYPE he1;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_KEF, sn);
      return(ERROR);
   }
   if ((f1 = MSD_getFace(s, fn)) == FNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouFace, NAM_KEF, fn, sn);
      return(ERROR);
   }
   if (MSD_getEdge(f1, v1, v2, &he1) == LNIL)
   {
      if (MSD_getEdge(f1, v2, v1, &he1) == LNIL)
      {
         fprintf(stderr, MEN_NaoEncontrouArestaV, NAM_KEF, v1, v2, sn);
         return(ERROR);
      }
   }
   MSD_lowKEF(he1, mate(he1));
   return(SUCCESS);
}

#ifdef __Extended_Version
void MSD_execKEF2(void)
{
   int sn, v1, v2, f1, f2, op;

   while (6 != sscanf(restbuf, "%d %d %d %d %d %d", &sn, &op, &v1, &v2, &f1, &f2))
   {
      printf("kef2: Sid V1 V2 F1 F2\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highKEF2(sn, v1, v2, f1, f2);
}

#endif

int MSD_highKEF2(Id sn, Id v1, Id v2, Id f1, Id f2)
{
   SPTYPE s;
   FPTYPE ff1;
   FPTYPE ff2;
   HPTYPE he1;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_KEF2, sn);
      return(ERROR);
   }
   if ((ff1 = MSD_getFace(s, f1)) == FNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouFace, NAM_KEF2, f1, sn);
      return(ERROR);
   }
   if ((ff2 = MSD_getFace(s, f2)) == FNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouFace, NAM_KEF2, f2, sn);
      return(ERROR);
   }

   if (MSD_getHalfedge(ff1, ff2, v1, v2, &he1) == LNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouArestaV, NAM_KEF, v1, v2, sn);
      return(ERROR);
   }
   MSD_lowKEF(he1, mate(he1));
   return(SUCCESS);
}

void MSD_lowKEF(HPTYPE he1, HPTYPE he2)
{
   SPTYPE s;
   DPTYPE d;
   FPTYPE f1;
   FPTYPE f2;
   LPTYPE l1;
   LPTYPE l2;
   LPTYPE l;
   EPTYPE e;
   HPTYPE he;
   HPTYPE he1nxt;
   HPTYPE he2nxt;
   HPTYPE hetmp;
   VPTYPE vp1;
   VPTYPE vp2;
   Id     v1, v2, v3, v4, fa, fb, fc, fd, flag;

   assert1(HalEdg(he1) != ENIL);
   assert1(HalEdg(he2) != ENIL);
   assert1(he1 == mate(he2));
   assert1((l1 = HalWLoop(he1)) != (l2 = HalWLoop(he2)));
   assert1((f1 = LooLFace(l1)) != (f2 = LooLFace(l2)));
   assert1(HalVtx(he2) == HalVtx(HalNxt(he1)));
   assert1(HalVtx(he1) == HalVtx(HalNxt(he2)));

#ifdef __curved_env
   if (he1->curv != (CUCURV *)NIL)
   {
      lsettag(he1, (CUCURV *)NIL);
   }
   if (he2->curv != (CUCURV *)NIL)
   {
      lsettag(he2, (CUCURV *)NIL);
   }
   if (f2->surf != (SUSURF *)NIL)
   {
      lsettag(f2, (SUSURF *)NIL);
   }
#endif

   d = FacFShell(f1);
   s = SheSSolids(d);
   e = HalEdg(he1);

   if (Gdoinversion)
   {
      if ((HalNxt(he2) == he2) && (HalNxt(he1) != he1))
      {
         MSD_lowMEV(he1, HalNxt(he2), -1, 0.0, 0.0, 0.0);
         MSD_lowKEF(HalPrv(he1), mate(HalPrv(he1)));
         MSD_lowKEV(he1, mate(he1));
         return;
      }
      fa = FacFaceNo(f1);
      fd = FacFaceNo(f2);
      for (AllLoopsFace(f2, l))
      {
         if (l2 != l)
         {
            he   = LooLEdg(l);
            v1   = VerVertexNo(HalVtx(he));
            v2   = HalNxt(he) != he ? VerVertexNo(HalVtx(HalNxt(he))) : 0;
            flag = l == FacFLOut(f2) ? TRUE : FALSE;
            MSD_lowAddEulerOp(s, RMOVE, v1, v2, fa, fd, flag, 0, 0, 0, 0.0, 0.0, 0.0, 0.0);
         }
      }

      v1 = VerVertexNo(HalVtx(he1));
      v2 = VerVertexNo(HalVtx(he2));
      v3 = VerVertexNo(HalVtx(HalNxt(HalNxt(he2))));
      v4 = HalNxt(he1) == he1 ? v3 : VerVertexNo(HalVtx(HalNxt(HalNxt(he1))));
      fb = FacFaceNo(LooLFace(HalWLoop(mate(HalNxt(he2)))));
      fc = HalNxt(he1) == he1 ? fb : FacFaceNo(LooLFace(HalWLoop(mate(HalNxt(he1)))));
      if (fb == fd)
      {
         fb = fa;
      }
      if (fc == fd)
      {
         fc = fa;
      }
      MSD_lowAddEulerOp(s, MEF, v1, v2, v3, v4, fa, fb, fc, fd, 0.0, 0.0, 0.0, 0.0);
   }
   if (Gdodirect)
   {
      v1 = VerVertexNo(HalVtx(he1));
      v2 = VerVertexNo(HalVtx(he2));
      fa = FacFaceNo(LooLFace(HalWLoop(HalNxt(he1))));
      fb = FacFaceNo(LooLFace(HalWLoop(HalNxt(he2))));
      MSD_lowAddEulerOpRedo(s, KEF, v1, v2, fa, fb, 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0);
   }

   he = LooLEdg(l2);
   do
   {
      HalWLoop(he) = l1;
   } while ((he = HalNxt(he)) != LooLEdg(l2));
   LooLength(l1) += LooLength(l2) - 2;

   while ((l = FacFLoops(f2)) != LNIL)
   {
      MSD_lowDelList(LOOP, (NPTYPE)l, (NPTYPE)f2);
      MSD_lowAddList(LOOP, (NPTYPE)l, (NPTYPE)f1);
   }

   he2nxt = HalNxt(he2) == he2 ? HalNxt(he1) : HalNxt(he2);
   he1nxt = HalNxt(he1) == he1 ? HalNxt(he2) : HalNxt(he1);

   HalNxt(HalPrv(he1)) = he2;
   HalNxt(HalPrv(he2)) = he1;
   he          = HalPrv(he2);
   hetmp       = HalPrv(he1);
   HalPrv(he2) = hetmp;
   HalPrv(he1) = he;

   vp1 = HalVtx(he1);
   vp2 = HalVtx(he2);

   MSD_lowDelHE(he2);
   MSD_lowDelHE(he1);

   VerVEdge(vp2) = he1nxt;
   if (HalEdg(VerVEdge(vp2)) == ENIL)
   {
      VerVEdge(vp2) = HNIL;
   }
   VerVEdge(vp1) = he2nxt;
   if (HalEdg(VerVEdge(vp1)) == ENIL)
   {
      VerVEdge(vp1) = HNIL;
   }

   LooLEdg(l1) = he1nxt;

   MSD_lowDelElement(LOOP, (NPTYPE)l2, (NPTYPE)f1);
   MSD_lowDelElement(FACE, (NPTYPE)f2, (NPTYPE)d);
   MSD_lowDelElement(EDGE, (NPTYPE)e, (NPTYPE)d);

   MSD_addEulCoef(s, 0, -1, 0, -1, 0, 0);

   if (SetTrace)
   {
      if (TraceSolid(s))
      {
         fprintf(stderr, MEN_MSDAbortou, NAM_KEF);
         exit(1);
      }
   }
}

int MSD_execStructKEF(EulerOp *op)
{
   Id sid, id1, id2, id3, id4;

   sid = op->solidno;
   id1 = op->ip1;
   id2 = op->ip2;
   id3 = op->ip3;
   id4 = op->ip4;
   return(MSD_highKEF2(sid, id1, id2, id3, id4));
}

void MSD_printStructKEF(FILE *f, EulerOp *op)
{
   Id  sid, id1, id2, id3, id4;
   int com;

   sid = op->solidno;
   id1 = op->ip1;
   id2 = op->ip2;
   id3 = op->ip3;
   id4 = op->ip4;
   com = op->CommandCount;
   fprintf(f, "kef %d %d %d %d %d %d\n", sid, com, id1, id2, id3, id4);
}

int MSD_readStructKEF(FILE *f, EulerOp *op)
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

int MSD_modifyStructKEF(EulerOp *op)
{
   int solidno;

   solidno     = MSD_lowCheckTranslatorTable(op->solidno, SOLID);
   op->solidno = solidno;
   return(SUCCESS);
}
