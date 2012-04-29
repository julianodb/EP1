/* 
  PROJETO USPDesigner
  MODULO: EUL (Operadores de Euler)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: eulkemr_.cpp
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
void MSD_execKEMR(void)
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
   MSD_highKEMR(sn, fn, v1, v2);
}

#endif

int MSD_highKEMR(Id sn, Id fn, Id v1, Id v2)
{
   SPTYPE s;
   FPTYPE f;
   HPTYPE he1;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_KEMR, sn);
      return(ERROR);
   }
   if ((f = MSD_getFace(s, fn)) == FNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouFace, NAM_KEMR, fn, sn);
      return(ERROR);
   }
   if (MSD_getEdge(f, v1, v2, &he1) == LNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouArestaV, NAM_KEMR, v1, v2, sn);
      return(ERROR);
   }
   if (HalWLoop(mate(he1)) != HalWLoop(he1))
   {
      fprintf(stderr, MEN_NaoArestaPonte, NAM_KEMR, v1, v2, sn);
      return(ERROR);
   }
   MSD_lowKEMR(he1, mate(he1));
   return(SUCCESS);
}

void MSD_lowKEMR(HPTYPE he1, HPTYPE he2)
{
   SPTYPE s;
   DPTYPE d;
   LPTYPE nl;
   LPTYPE o1;
   EPTYPE killedge;
   HPTYPE he3;
   HPTYPE he4;
   HPTYPE he5;
   Id     v1, v2, v3, v4, f1, f2, f3;

   assert1(he1 == mate(he2));
   assert1((o1 = HalWLoop(he1)) == HalWLoop(he2));

#ifdef __curved_env
   if (he1->curv != (CUCURV *)NIL)
   {
      lsettag(he1, (CUCURV *)NIL);
   }
   if (he2->curv != (CUCURV *)NIL)
   {
      lsettag(he2, (CUCURV *)NIL);
   }
#endif

   d = FacFShell(LooLFace(o1));
   s = SheSSolids(d);
   if (Gdoinversion)
   {
      v1 = VerVertexNo(HalVtx(he1));
      v2 = VerVertexNo(HalVtx(he2));
      v3 = (HalNxt(he2) == he1) ? v1 : VerVertexNo(HalVtx(HalNxt(HalNxt(he2))));
      v4 = (HalNxt(he2) == he2) ? v2 : VerVertexNo(HalVtx(HalNxt(HalNxt(he1))));
      f1 = FacFaceNo(LooLFace(o1));
      f2 = (HalNxt(he2) == he1) ? f1 : FacFaceNo(LooLFace(HalWLoop(mate(HalNxt(he2)))));
      f3 = (HalNxt(he1) == he2) ? f1 : FacFaceNo(LooLFace(HalWLoop(mate(HalNxt(he1)))));
      MSD_lowAddEulerOp(s, MEKR, v2, v1, v4, v3, f1, f3, f2, 0, 0.0, 0.0, 0.0, 0.0);
   }

   if (Gdodirect)
   {
      v1 = VerVertexNo(HalVtx(he1));
      v2 = VerVertexNo(HalVtx(he2));
      f1 = FacFaceNo(LooLFace(HalWLoop(he1)));
      MSD_lowAddEulerOpRedo(s, KEMR, f1, v1, v2, 0, 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0);
   }

   nl       = (LPTYPE)MSD_lowNewElement(LOOP, (NPTYPE)LooLFace(o1));
   killedge = HalEdg(he1);

   he3                 = HalNxt(he1);
   HalNxt(he1)         = HalNxt(he2);
   HalPrv(HalNxt(he2)) = he1;
   HalNxt(he2)         = he3;
   HalPrv(he3)         = he2;

   he4           = he2;
   LooLength(nl) = -1;

   do
   {
      HalWLoop(he4) = nl;
      LooLength(nl)++;
   } while ((he4 = HalNxt(he4)) != he2);

   he4           = he1;
   LooLength(o1) = -1;
   do
   {
      LooLength(o1)++;
   } while ((he4 = HalNxt(he4)) != he1);

   LooLEdg(o1) = he3 = MSD_lowDelHE(he1);
   LooLEdg(nl) = he4 = MSD_lowDelHE(he2);

   he5 = (HalEdg(he3) != ENIL) ? he3 : HNIL;
   VerVEdge(HalVtx(he3)) = he5;
   he5 = (HalEdg(he4) != ENIL) ? he4 : HNIL;
   VerVEdge(HalVtx(he4)) = he5;
   he5 = (HalEdg(HalNxt(he3)) != ENIL) ? HalNxt(he3) : HNIL;
   VerVEdge(HalVtx(HalNxt(he3))) = he5;
   he5 = (HalEdg(HalNxt(he4)) != ENIL) ? HalNxt(he4) : HNIL;
   VerVEdge(HalVtx(HalNxt(he4))) = he5;

   assert1(findes(d, killedge) == 1);
   MSD_lowDelElement(EDGE, (NPTYPE)killedge, (NPTYPE)d);
   MSD_addEulCoef(s, 0, 0, 1, -1, 0, 0);

   if (SetTrace)
   {
      if (TraceSolid(s))
      {
         fprintf(stderr, MEN_MSDAbortou, NAM_KEMR);
         exit(1);
      }
   }
}

int MSD_execStructKEMR(EulerOp *op)
{
   Id sid, id1, id2, id3;

   sid = op->solidno;
   id1 = op->ip1;
   id2 = op->ip2;
   id3 = op->ip3;
   return(MSD_highKEMR(sid, id1, id2, id3));
}

void MSD_printStructKEMR(FILE *f, EulerOp *op)
{
   Id  sid, id1, id2, id3;
   int com;

   sid = op->solidno;
   id1 = op->ip1;
   id2 = op->ip2;
   id3 = op->ip3;
   com = op->CommandCount;
   fprintf(f, "kemr %d %d %d %d %d\n", sid, com, id1, id2, id3);
}

int MSD_readStructKEMR(FILE *f, EulerOp *op)
{
   int solidno, ip1, ip2, ip3, com;

   if (fscanf(f, "%d %d %d %d %d\n", &solidno, &com, &ip1, &ip2, &ip3) != 5)
   {
      return(ERROR);
   }
   op->solidno      = solidno;
   op->CommandCount = com;
   op->ip1          = ip1;
   op->ip2          = ip2;
   op->ip3          = ip3;
   op->ip4          = 0;
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

int MSD_modifyStructKEMR(EulerOp *op)
{
   int solidno;

   solidno     = MSD_lowCheckTranslatorTable(op->solidno, SOLID);
   op->solidno = solidno;
   return(SUCCESS);
}
