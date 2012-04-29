/* 
  PROJETO USPDesigner
  MODULO: EUL (Operadores de Euler)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: eulksfmr.cpp
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
void MSD_execKSFMR(void)
{
   int sn, fn1, fn2, op;

   while (4 != sscanf(restbuf, "%d %d %d %d", &sn, &op, &fn1, &fn2))
   {
      printf("Sid f1 f2\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highKSFMR(sn, fn1, fn2);
}

#endif

int MSD_highKSFMR(Id sn, Id fn1, Id fn2)
{
   SPTYPE s;
   FPTYPE f1;
   FPTYPE f2;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_KSFMR, sn);
      return(ERROR);
   }
   if ((f1 = MSD_getFace(s, fn1)) == FNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouFace, NAM_KSFMR, fn1, sn);
      return(ERROR);
   }
   if ((f2 = MSD_getFace(s, fn2)) == FNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouFace, NAM_KSFMR, fn2, sn);
      return(ERROR);
   }
   if (LooNextL(FacFLoops(f2)) != LNIL)
   {
      fprintf(stderr, MEN_FaceSemLoop, NAM_KSFMR, fn2, sn);
      return(ERROR);
   }
   MSD_lowKSFMR(f1, f2);
   return(SUCCESS);
}

void MSD_lowKSFMR(FPTYPE f1, FPTYPE f2)
{
   SPTYPE s;
   DPTYPE d1;
   DPTYPE d2;
   FPTYPE f;
   LPTYPE newloop;
   EPTYPE e;
   VPTYPE v;
   Id     fa, fb, db, v1, v2;

   assert1(FacFShell(f1) != FacFShell(f2));
   assert1(LooNextL(FacFLoops(f2)) == LNIL);
   assert1(SheSSolids(FacFShell(f1)) == SheSSolids(FacFShell(f2)));

   d1 = FacFShell(f1);
   d2 = FacFShell(f2);
   s  = SheSSolids(d1);
   if (Gdoinversion)
   {
      fa = FacFaceNo(f1);
      fb = FacFaceNo(f2);
      db = SheShellNo(d2);
      v1 = VerVertexNo(HalVtx(LooLEdg(FacFLoops(f2))));
      v2 = VerVertexNo(HalVtx(HalNxt(LooLEdg(FacFLoops(f2)))));
      MSD_lowAddEulerOp(s, MSFKR, fa, fb, db, v1, v2, 0, 0, 0, 0.0, 0.0, 0.0, 0.0);
   }
   if (Gdodirect)
   {
      fa = FacFaceNo(f1);
      fb = FacFaceNo(f2);
      MSD_lowAddEulerOpRedo(s, KSFMR, fa, fb, 0, 0, 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0);
   }

   while ((v = SheSVerts(d2)) != VNIL)
   {
      MSD_lowDelList(VERTEX, (NPTYPE)v, (NPTYPE)d2);
      MSD_lowAddList(VERTEX, (NPTYPE)v, (NPTYPE)d1);
   }
   while ((e = SheSEdges(d2)) != ENIL)
   {
      MSD_lowDelList(EDGE, (NPTYPE)e, (NPTYPE)d2);
      MSD_lowAddList(EDGE, (NPTYPE)e, (NPTYPE)d1);
   }
   while ((f = SheSFaces(d2)) != FNIL)
   {
      MSD_lowDelList(FACE, (NPTYPE)f, (NPTYPE)d2);
      MSD_lowAddList(FACE, (NPTYPE)f, (NPTYPE)d1);
   }

   newloop = FacFLoops(f2);
   MSD_lowDelList(LOOP, (NPTYPE)newloop, (NPTYPE)f2);
   MSD_lowAddList(LOOP, (NPTYPE)newloop, (NPTYPE)f1);
   MSD_lowDelElement(FACE, (NPTYPE)f2, (NPTYPE)d1);
   MSD_lowDelElement(SHELL, (NPTYPE)d2, (NPTYPE)s);

   MSD_addEulCoef(s, -1, -1, 1, 0, 0, 0);

   if (SetTrace)
   {
      if (TraceSolid(s))
      {
         fprintf(stderr, MEN_MSDAbortou, NAM_KSFMR);
         exit(1);
      }
   }
}

int MSD_execStructKSFMR(EulerOp *op)
{
   Id sid, id1, id2;

   sid = op->solidno;
   id1 = op->ip1;
   id2 = op->ip2;
   return(MSD_highKSFMR(sid, id1, id2));
}

void MSD_printStructKSFMR(FILE *f, EulerOp *op)
{
   Id  sid, id1, id2;
   int com;

   sid = op->solidno;
   id1 = op->ip1;
   id2 = op->ip2;
   com = op->CommandCount;
   fprintf(f, "ksfmr %d %d %d %d \n", sid, com, id1, id2);
}

int MSD_readStructKSFMR(FILE *f, EulerOp *op)
{
   int solidno, ip1, ip2, com;

   if (fscanf(f, "%d %d %d %d\n", &solidno, &com, &ip1, &ip2) != 4)
   {
      return(ERROR);
   }
   op->solidno      = solidno;
   op->CommandCount = com;
   op->ip1          = ip1;
   op->ip2          = ip2;
   op->ip3          = 0;
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

int MSD_modifyStructKSFMR(EulerOp *op)
{
   int solidno;

   solidno     = MSD_lowCheckTranslatorTable(op->solidno, SOLID);
   op->solidno = solidno;
   return(SUCCESS);
}
