/* 
  PROJETO USPDesigner
  MODULO: CUR (Curve Manipulation)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: cursettg.cpp
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
#include "lowmacro.h"
#include "lowparam.h"
#include "lowsolid.h"
#include "eulerops.h"
#include "mancommd.h"
#include "curvgeom.h"

#ifdef __curved_env__

/* tag halfedge v1->v2 of face fn and its mate with tag */
 #ifdef __Extended_Version
void MSD_execSetCurveAtributeToEdge(void)
{
   int  sn, tag, fn, v1, v2;
   char sname[20];

   while (5 != sscanf(restbuf, "%s %d %d %d %d", sname, &tag, &fn, &v1, &v2))
   {
      printf("PUTCTAG: sname tag fn v1 v2 \n");
      if (!lineins("? "))
      {
         return;
      }
   }
   if ((sn = MSD_getSolidIdFromName(sname)) == ERROR)
   {
      fprintf(stderr, MEN_NaoEncontrouSolidNome, NAM_PutCTag, sname);
      return;
   }
   MSD_highSetCurveAtributeToEdge(sn, tag, fn, v1, v2);
}

 #endif

int MSD_highSetCurveAtributeToEdge(Id sn, Id tag, Id fn, Id v1, Id v2)
{
   SPTYPE s;
   FPTYPE fac;
   HPTYPE hptr;
   CURVE  *curv;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_PutCTag, sn);
      return(ERROR);
   }
   if ((fac = MSD_getFace(s, fn)) == FNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouFace, NAM_PutCTag, fn, sn);
      return(ERROR);
   }
   if (MSD_getEdge(fac, v1, v2, &hptr) == LNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouArestaV, NAM_PutCTag, v1, v2, sn);
      return(ERROR);
   }
   if ((curv = MSD_getCurve(s, tag)) == (CURVE *)NIL)
   {
      fprintf(stderr, MEN_NaoEncontrouTag, NAM_PutCTag, tag, sn);
      return(ERROR);
   }
   MSD_lowSetCurveAtributeToEdge(hptr, curv);
   return(SUCCESS);
}

int MSD_highSetCurveAtribute(Id sn, Id tag, Id fn, Id v1, Id v2)
{
   SPTYPE s;
   FPTYPE fac;
   HPTYPE hptr;
   CURVE  *curv;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_PutCTag, sn);
      return(ERROR);
   }
   if ((fac = MSD_getFace(s, fn)) == FNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouFace, NAM_PutCTag, fn, sn);
      return(ERROR);
   }
   if (MSD_getEdge(fac, v1, v2, &hptr) == LNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouArestaV, NAM_PutCTag, v1, v2, sn);
      return(ERROR);
   }
   if ((curv = MSD_getCurve(s, tag)) == (CURVE *)NIL)
   {
      fprintf(stderr, MEN_NaoEncontrouTag, NAM_PutCTag, tag, sn);
      return(ERROR);
   }
   MSD_lowSetCurveAtribute(hptr, curv);
   return(SUCCESS);
}

/* Low-level tagging of halfedges */
int MSD_lowSetCurveAtributeToEdge(HPTYPE he, CURVE *tag)
{
   MSD_lowSetCurveAtribute(he, tag);
   MSD_lowSetCurveAtribute(mate(he), tag);
   return(SUCCESS);
}

/* Set tag on an halfedge */
int MSD_lowSetCurveAtribute(HPTYPE he, CURVE *t)
{
   SPTYPE s;
   FPTYPE f;
   CURVE  *old;
   Id     fid, v1d, v2d, cid;

   if (t == (old = HalCurv(he)))
   {
      return(ERROR);
   }
   f           = LooLFace(HalWLoop(he));
   HalCurv(he) = t;

   if (Gdoinversion)
   {
      s   = SheSSolids(FacFShell(f));
      fid = FacFaceNo(f);
      v1d = VerVertexNo(HalVtx(he));
      v2d = VerVertexNo(HalVtx(HalNxt(he)));
	  cid = old == (CURVE *)NIL ? -1 : old->tagno;
      MSD_lowAddEulerOp(s, SETCTAG, fid, v1d, v2d, cid, 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0);
   }
   if (Gdodirect)
   {
      s   = SheSSolids(FacFShell(f));
      fid = FacFaceNo(f);
      v1d = VerVertexNo(HalVtx(he));
      v2d = VerVertexNo(HalVtx(HalNxt(he)));
      cid = t->tagno;
      MSD_lowAddEulerOpRedo(s, SETCTAG, fid, v1d, v2d, cid, 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0);
   }

   if (old != (CURVE *)NIL)
   {
      if ((--old->times_used) == 0)
      {
         MSD_lowCurveDeleteElement(old, SheSSolids(FacFShell(f)));
      }
   }
   if (t != (CURVE *)NIL)
   {
      t->times_used++;
   }
   return(SUCCESS);
}

int MSD_execStructSETCTAG(EulerOp *op)
{
   Id sid, id1, id2, id3, id4;

   sid = op->solidno;
   id1 = op->ip1;
   id2 = op->ip2;
   id3 = op->ip3;
   id4 = op->ip4;
   return(MSD_highSetCurveAtribute(sid, id1, id2, id3, id4));
}

void MSD_printStructSETCTAG(FILE *f, EulerOp *op)
{
   Id  ids, id1, id2, id3, id4;
   int com;

   ids = op->solidno;
   id1 = op->ip1;
   id2 = op->ip2;
   id3 = op->ip3;
   id4 = op->ip4;
   com = op->CommandCount;
   fprintf(f, "setctag %d %d %d %d %d\n", ids, com, id1, id2, id3, id4);
}

int MSD_readStructSETCTAG(FILE *f, EulerOp *op)
{
   int solidno, ip1, ip2, ip3, ip4, com;

   if (fscanf(f, "%d %d %d %d %d\n", &solidno, &com, &ip1, &ip2, &ip3, &ip4) != 5)
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

int MSD_modifyStructSETCTAG(EulerOp *op)
{
   return(SUCCESS);
}

#endif
