/* 
  PROJETO USPDesigner
  MODULO: EUL (Operadores de Euler)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: eulmef__.cpp
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
void MSD_execMEF(void)
{
   int sn, v1, v2, f1, f2, op;

   while (6 != sscanf(restbuf, "%d %d %d %d %d %d", &sn, &op, &v1, &v2, &f1, &f2))
   {
      printf("mef: Sid V1 V2 F1 F2\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highMEF(sn, v1, v2, f1, f2);
}

#endif

int MSD_highMEF(Id sn, Id v1, Id v2, Id f1, Id f2)
{
   SPTYPE s;
   FPTYPE oldface;
   LPTYPE oldloop;
   HPTYPE he1;
   HPTYPE he2;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_MEF, sn);
      return(ERROR);
   }
   if ((oldface = MSD_getFace(s, f1)) == FNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouFace, NAM_MEF, f1, sn);
      return(ERROR);
   }
   if ((oldloop = MSD_getLoop(oldface, v1, &he1)) == LNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouVertice, NAM_MEF, v1, sn);
      return(ERROR);
   }
   if (MSD_getLoop(oldface, v2, &he2) != oldloop)
   {
      fprintf(stderr, MEN_NaoEncontrouVertice, NAM_MEF, v2, sn);
      return(ERROR);
   }
   MSD_lowMEF(he1, he2, f2);
   return(SUCCESS);
}

#ifdef __Extended_Version
void MSD_execMEF2(void)
{
   int sn, v1, v2, v3, v4, f1, f2, f3, f4, op;

   while (10 != sscanf(restbuf, "%d %d %d %d %d %d %d %d %d %d", &sn, &op, &v1,
                       &v2, &v3, &v4, &f1, &f2, &f3, &f4))
   {
      printf("mef2: Sid V1 V2 V3 V4 F1 F2 F3 F4\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highMEF2(sn, v1, v2, v3, v4, f1, f2, f3, f4);
}

#endif

int MSD_highMEF2(Id sn, Id v1, Id v2, Id v3, Id v4, Id f1, Id f2, Id f3, Id f4)
{
   SPTYPE s;
   FPTYPE ff1;
   FPTYPE ff2;
   FPTYPE ff3;
   HPTYPE he1;
   HPTYPE he2;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_MEF2, sn);
      return(ERROR);
   }
   if ((ff1 = MSD_getFace(s, f1)) == FNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouFace, NAM_MEF2, f1, sn);
      return(ERROR);
   }
   if ((ff2 = MSD_getFace(s, f2)) == FNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouFace, NAM_MEF2, f2, sn);
      return(ERROR);
   }
   if ((ff3 = MSD_getFace(s, f3)) == FNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouFace, NAM_MEF2, f3, sn);
      return(ERROR);
   }
   if (MSD_getHalfedge(ff1, ff2, v1, v3, &he1) == LNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouArestaF, NAM_MEF2, v1, v3, f1, f2);
      return(ERROR);
   }
   if (MSD_getHalfedge(ff1, ff3, v2, v4, &he2) == LNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouArestaF, NAM_MEF2, v2, v4, f1, f3);
      return(ERROR);
   }
   MSD_lowMEF(he1, he2, f4);
   return(SUCCESS);
}

FPTYPE MSD_lowMEF(HPTYPE he1, HPTYPE he2, Id fn)
{
   SPTYPE s;
   DPTYPE d;
   FPTYPE newface;
   FPTYPE oldface;
   LPTYPE newloop;
   LPTYPE oldloop;
   EPTYPE newedge;
   HPTYPE nhe1;
   HPTYPE nhe2;
   HPTYPE he;
   Id     v1, v2, f1;
   Id     v3, v4, f2, f3;

#ifdef __curved_env
   short  copy_tag, is_wire;
   vector v1;
#endif

#ifdef __VIRTUAL_MEM
   MSD_checkHalfedge(he1);
   MSD_checkHalfedge(he2);
#endif

   oldloop = HalWLoop(he1);
   oldface = LooLFace(oldloop);
   d       = FacFShell(oldface);
   s       = SheSSolids(d);

   assert1(HalWLoop(he1) == HalWLoop(he2));
   assert1(findl(oldloop, oldface) == 1);
   assert1(findf(d, oldface) == 1);
#ifdef __curved_env
   copy_tag = he1 == he2 ? 0 : 1;
   is_wire  = 0;
   if ((he1->prv == mate(he1)) && (he2->prv == mate(he2)) &&
       (s->sfaces->nextf == (FTYPE *)NIL))
   {
      is_wire = 1;
   }
#endif

   if (Gdoinversion)
   {
      v1 = VerVertexNo(HalVtx(he1));
      v2 = VerVertexNo(HalVtx(he2));
      f1 = FacFaceNo(oldface);
      MSD_lowAddEulerOp(s, KEF, v1, v2, f1, fn, 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0);
   }

   if (Gdodirect)
   {
      v1 = VerVertexNo(HalVtx(he1));
      v2 = VerVertexNo(HalVtx(he2));
      v3 = VerVertexNo(HalVtx(HalNxt(he1)));
      v4 = VerVertexNo(HalVtx(HalNxt(he2)));
      f1 = FacFaceNo(oldface);
      f2 = HalEdg(he1) == ENIL ? f1 : FacFaceNo(LooLFace(HalWLoop(mate(he1))));
      f3 = HalEdg(he2) == ENIL ? f1 : FacFaceNo(LooLFace(HalWLoop(mate(he2))));
      MSD_lowAddEulerOpRedo(s, MEF, v1, v2, v3, v4, f1, f2, f3, fn, 0.0, 0.0, 0.0, 0.0);
   }

   newface            = (FPTYPE)MSD_lowNewElement(FACE, (NPTYPE)d);
   newloop            = (LPTYPE)MSD_lowNewElement(LOOP, (NPTYPE)newface);
   newedge            = (EPTYPE)MSD_lowNewElement(EDGE, (NPTYPE)d);
   EdgEdgeNo(newedge) = -1;
   FacFaceNo(newface) = fn;
   FacFLOut(newface)  = newloop;

   for (LooLength(newloop) = 0, he = he1; he != he2; he = HalNxt(he))
   {
      HalWLoop(he) = newloop;
      LooLength(newloop)++;
      LooLength(oldloop)--;
   }

   nhe1 = MSD_lowAddHE(newedge, HalVtx(he2), he1, MINUS);
   nhe2 = MSD_lowAddHE(newedge, HalVtx(he1), he2, PLUS);

   HalNxt(HalPrv(nhe1)) = nhe2;
   HalNxt(HalPrv(nhe2)) = nhe1;
   he           = HalPrv(nhe1);
   HalPrv(nhe1) = HalPrv(nhe2);
   HalPrv(nhe2) = he;

   LooLEdg(newloop) = nhe1;
   LooLEdg(oldloop) = nhe2;

#ifdef __curved_env
   if (!is_wire && (copy_tag == 1) && (oldface->surf != (SUSURF *)NIL))
   {
      lsettag(newface, oldface->surf);
   }
   if (is_wire)
   {
      faceeq(newface->flout, v1);
      lsettag(newface, newplane(++maxst, v1[0], v1[1], v1[2], v1[3]));
      faceeq(oldface->flout, v1);
      lsettag(oldface, newplane(++maxst, v1[0], v1[1], v1[2], v1[3]));
   }
#endif

   MSD_addEulCoef(s, 0, 1, 0, 1, 0, 0);

   if (SetTrace)
   {
      if (TraceSolid(s))
      {
         fprintf(stderr, MEN_MSDAbortou, NAM_MEF);
         exit(1);
      }
   }
   return(newface);
}

int MSD_execStructMEF(EulerOp *op)
{
   Id sid, id1, id2, id3, id4, id5, id6, id7, id8;

   sid = op->solidno;
   id1 = op->ip1;
   id2 = op->ip2;
   id3 = op->ip3;
   id4 = op->ip4;
   id5 = op->ip5;
   id6 = op->ip6;
   id7 = op->ip7;
   id8 = op->ip8;
   return(MSD_highMEF2(sid, id1, id2, id3, id4, id5, id6, id7, id8));
}

void MSD_printStructMEF(FILE *f, EulerOp *op)
{
   Id  sid, id1, id2, id3, id4, id5, id6, id7, id8;
   int com;

   sid = op->solidno;
   id1 = op->ip1;
   id2 = op->ip2;
   id3 = op->ip3;
   id4 = op->ip4;
   id5 = op->ip5;
   id6 = op->ip6;
   id7 = op->ip7;
   id8 = op->ip8;
   com = op->CommandCount;
   fprintf(f, "mef %d %d %d %d %d %d %d %d %d %d\n", sid,
           com, id1, id2, id3, id4, id5, id6, id7, id8);
}

int MSD_readStructMEF(FILE *f, EulerOp *op)
{
   int solidno, ip1, ip2, ip3, ip4, ip5, ip6, ip7, ip8, com;

   if (fscanf(f, "%d %d %d %d %d %d %d %d %d %d\n", &solidno, &com, &ip1, &ip2, &ip3,
              &ip4, &ip5, &ip6, &ip7, &ip8) != 10)
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
   op->ip8          = ip8;
   op->fp1          = 0.0;
   op->fp2          = 0.0;
   op->fp3          = 0.0;
   op->fp4          = 0.0;
   return(SUCCESS);
}

int MSD_modifyStructMEF(EulerOp *op)
{
   int solidno;

   solidno     = MSD_lowCheckTranslatorTable(op->solidno, SOLID);
   op->solidno = solidno;
   return(SUCCESS);
}
