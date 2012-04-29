/* 
  PROJETO USPDesigner
  MODULO: EUL (Operadores de Euler)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: eulmfkrh.cpp
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
void MSD_execMFKRH(void)
{
   int sn, f1, f2, op;

   while (4 != sscanf(restbuf, "%d %d %d %d", &sn, &op, &f1, &f2))
   {
      printf("mfkrh: Sid F1 F2\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highMFKRH(sn, f1, f2);
}

#endif

int MSD_highMFKRH(Id sn, Id f1, Id f2)
{
   SPTYPE s;
   FPTYPE f;
   LPTYPE ring;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_MFKRH, sn);
      return(ERROR);
   }
   if ((f = MSD_getFace(s, f1)) == FNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouFace, NAM_MFKRH, f1, sn);
      return(ERROR);
   }
   for (ring = FacFLoops(f); ring == FacFLOut(f); ring = LooNextL(ring))
   {
      ;
   }
   MSD_lowMFKRH(f, ring, f2);
   return(SUCCESS);
}

#ifdef __Extended_Version
void MSD_execMFKRH2(void)
{
   int sn, f1, f2, v1, v2, op;

   while (6 != sscanf(restbuf, "%d %d %d %d %d %d", &sn, &op, &f1, &f2, &v1, &v2))
   {
      printf("mfkrh2: Sid F1 F2 V1 V2\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highMFKRH2(sn, f1, f2, v1, v2);
}

#endif

int MSD_highMFKRH2(Id sn, Id f1, Id f2, Id v1, Id v2)
{
   SPTYPE s;
   FPTYPE f;
   LPTYPE ring;
   HPTYPE he1;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_MFKRH2, sn);
      return(ERROR);
   }
   if ((f = MSD_getFace(s, f1)) == FNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouFace, NAM_MFKRH2, f1);
      return(ERROR);
   }
   if ((ring = MSD_getEdge(f, v1, v2, &he1)) == LNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouArestaV, NAM_MFKRH2, v1, v2, sn);
      return(ERROR);
   }
   MSD_lowMFKRH(f, ring, f2);
   return(SUCCESS);
}

FPTYPE MSD_lowMFKRH(FPTYPE f, LPTYPE l, Id fn)
{
   SPTYPE s;
   DPTYPE d;
   FPTYPE nf;
   Id     f1;
   Id     v1, v2;

   assert1(LooNextL(FacFLoops(f)) != LNIL);
   assert1(FacFLOut(f) != l);
   assert1(LooLFace(l) == f);

   d = FacFShell(f);
   s = SheSSolids(d);
   if (Gdoinversion)
   {
      f1 = FacFaceNo(f);
      MSD_lowAddEulerOp(s, KFMRH, f1, fn, 0, 0, 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0);
   }
   if (Gdodirect)
   {
      f1 = FacFaceNo(f);
      v1 = VerVertexNo(HalVtx(LooLEdg(l)));
      v2 = VerVertexNo(HalVtx(HalNxt(LooLEdg(l))));
      MSD_lowAddEulerOpRedo(s, MFKRH, f1, fn, v1, v2, 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0);
   }

   nf = (FPTYPE)MSD_lowNewElement(FACE, (NPTYPE)d);

   FacFaceNo(nf) = fn;
   MSD_lowDelList(LOOP, (NPTYPE)l, (NPTYPE)f);
   MSD_lowAddList(LOOP, (NPTYPE)l, (NPTYPE)nf);
   FacFLOut(nf) = l;
   if (FacFLOut(f) == l)
   {
      fprintf(stderr, "#");
      FacFLOut(f) = FacFLoops(f);
   }

   MSD_addEulCoef(s, 0, 1, -1, 0, 0, -1);

   if (SetTrace)
   {
      if (TraceSolid(s))
      {
         fprintf(stderr, MEN_MSDAbortou, NAM_MFKRH);
         exit(1);
      }
   }
   return(nf);
}

int MSD_execStructMFKRH(EulerOp *op)
{
   Id sid, id1, id2, id3, id4;

   sid = op->solidno;
   id1 = op->ip1;
   id2 = op->ip2;
   id3 = op->ip3;
   id4 = op->ip4;
   return(MSD_highMFKRH2(sid, id1, id2, id3, id4));
}

void MSD_printStructMFKRH(FILE *f, EulerOp *op)
{
   Id  sid, id1, id2, id3, id4;
   int com;

   sid = op->solidno;
   id1 = op->ip1;
   id2 = op->ip2;
   id3 = op->ip3;
   id4 = op->ip4;
   com = op->CommandCount;
   fprintf(f, "mfkrh %d %d %d %d %d %d\n", sid, com, id1, id2, id3, id4);
}

int MSD_readStructMFKRH(FILE *f, EulerOp *op)
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

int MSD_modifyStructMFKRH(EulerOp *op)
{
   int solidno;

   solidno     = MSD_lowCheckTranslatorTable(op->solidno, SOLID);
   op->solidno = solidno;
   return(SUCCESS);
}
