/* 
  PROJETO USPDesigner
  MODULO: EUL (Operadores de Euler)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: eulkfmrh.cpp
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
#include "lowsolid.h"
#include "eulerops.h"
#include "curvgeom.h"
#include "mancommd.h"

#ifdef __Extended_Version
void MSD_execKFMRH(void)
{
   int sn, fn1, fn2, op;

   while (4 != sscanf(restbuf, "%d %d %d %d", &sn, &op, &fn1, &fn2))
   {
      printf("Solid F1 F2\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highKFMRH(sn, fn1, fn2);
}

#endif

int MSD_highKFMRH(Id sn, Id fn1, Id fn2)
{
   SPTYPE s;
   FPTYPE f1;
   FPTYPE f2;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_KFMRH, sn);
      return(ERROR);
   }
   if ((f1 = MSD_getFace(s, fn1)) == FNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouFace, NAM_KFMRH, fn1, sn);
      return(ERROR);
   }
   if ((f2 = MSD_getFace(s, fn2)) == FNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouFace, NAM_KFMRH, fn2, sn);
      return(ERROR);
   }
   if (LooNextL(FacFLoops(f2)) != LNIL)
   {
      fprintf(stderr, MEN_FaceSemLoop, NAM_KFMRH, fn2, sn);
      return(ERROR);
   }
   MSD_lowKFMRH(f1, f2);
   return(SUCCESS);
}

void MSD_lowKFMRH(FPTYPE f1, FPTYPE f2)
{
   SPTYPE s;
   DPTYPE d;
   LPTYPE newloop;
   Id     fa, fb, v1, v2;

   assert1(FacFLOut(f2) == FacFLoops(f2));
   assert1(LooNextL(FacFLoops(f2)) == LNIL);

#ifdef __curved_env
   if (f2->surf != (SUSURF *)NIL)
   {
      lsettag(f2, (SUSURF *)NIL);
   }
#endif

   d = FacFShell(f1);
   s = SheSSolids(d);
   if (Gdoinversion)
   {
      fa = FacFaceNo(f1);
      fb = FacFaceNo(f2);
      v1 = VerVertexNo(HalVtx(LooLEdg(FacFLoops(f2))));
      v2 = VerVertexNo(HalVtx(HalNxt(LooLEdg(FacFLoops(f2)))));
      MSD_lowAddEulerOp(s, MFKRH, fa, fb, v1, v2, 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0);
   }
   if (Gdodirect)
   {
      fa = FacFaceNo(f1);
      fb = FacFaceNo(f2);
      MSD_lowAddEulerOpRedo(s, KFMRH, fa, fb, 0, 0, 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0);
   }


   while ((newloop = FacFLoops(f2)) != LNIL)
   {
      MSD_lowDelList(LOOP, (NPTYPE)newloop, (NPTYPE)f2);
      MSD_lowAddList(LOOP, (NPTYPE)newloop, (NPTYPE)f1);
   }
   MSD_lowDelElement(FACE, (NPTYPE)f2, (NPTYPE)d);

   MSD_addEulCoef(s, 0, -1, 1, 0, 0, 1);

   if (SetTrace)
   {
      if (TraceSolid(s))
      {
         fprintf(stderr, MEN_MSDAbortou, NAM_KFMRH);
         exit(1);
      }
   }
}

int MSD_execStructKFMRH(EulerOp *op)
{
   Id sid, id1, id2;

   sid = op->solidno;
   id1 = op->ip1;
   id2 = op->ip2;
   return(MSD_highKFMRH(sid, id1, id2));
}

void MSD_printStructKFMRH(FILE *f, EulerOp *op)
{
   Id  sid, id1, id2;
   int com;

   sid = op->solidno;
   id1 = op->ip1;
   id2 = op->ip2;
   com = op->CommandCount;
   fprintf(f, "kfmrh %d %d %d %d \n", sid, com, id1, id2);
}

int MSD_readStructKFMRH(FILE *f, EulerOp *op)
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

int MSD_modifyStructKFMRH(EulerOp *op)
{
   int solidno;

   solidno     = MSD_lowCheckTranslatorTable(op->solidno, SOLID);
   op->solidno = solidno;
   return(SUCCESS);
}
