/* 
  PROJETO USPDesigner
  MODULO: TOP (Operadores de Nivel Intermediario)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: topringm.cpp
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
#include "vectorop.h"
#include "lowgeome.h"

#ifdef __Extended_Version
void MSD_execRingMove(void)
{
   int sn, v1, v2, f1, f2, out, op;

   while (sscanf(restbuf, "%d %d %d %d %d %d %d", &sn, &op, &v1, &v2,
                 &f1, &f2, &out) != 7)
   {
      printf("ringmv: Sid v1 v2 f1 f2 out");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highRingMove(sn, v1, v2, f1, f2, out);
}

#endif

int MSD_highRingMove(Id sn, Id v1, Id v2, Id f1, Id f2, int out)
{
   SPTYPE s;
   FPTYPE ff1;
   FPTYPE ff2;
   LPTYPE move_l;
   HPTYPE dum;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_RingMove, sn);
      return(ERROR);
   }
   if ((ff1 = MSD_getFace(s, f1)) == FNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouFace, NAM_RingMove, f1, sn);
      return(ERROR);
   }
   if ((ff2 = MSD_getFace(s, f2)) == FNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouFace, NAM_RingMove, f2, sn);
      return(ERROR);
   }

   move_l = v2 != 0 ? MSD_getEdge(ff1, v1, v2, &dum) : MSD_getLoop(ff1, v1, &dum);
   if (move_l == LNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouArestaV, NAM_RingMove, v1, v2, sn);
      return(ERROR);
   }
   return(MSD_lowRingMove(ff1, ff2, move_l, out));
}

int MSD_lowRingMove(FPTYPE f1, FPTYPE f2, LPTYPE l, int out)
{
   SPTYPE s;
   Id     v1, v2, ff1, ff2;
   int    flag;

   assert(LooLFace(l) == f1);

   s = SheSSolids(FacFShell(f1));
   if (Gdoinversion)
   {
      s    = SheSSolids(FacFShell(f1));
      v1   = VerVertexNo(HalVtx(LooLEdg(l)));
      v2   = HalEdg(LooLEdg(l)) != ENIL ? VerVertexNo(HalVtx(HalNxt(LooLEdg(l)))) : 0;
      ff2  = FacFaceNo(f2);
      ff1  = FacFaceNo(f1);
      flag = l == FacFLOut(f1) ? TRUE : FALSE;
      MSD_lowAddEulerOp(s, RMOVE, v1, v2, ff2, ff1, flag, 0, 0, 0, 0.0, 0.0, 0.0, 0.0);
   }
   if (Gdodirect)
   {
      ff2 = FacFaceNo(f2);
      ff1 = FacFaceNo(f1);
      v1  = VerVertexNo(HalVtx(LooLEdg(l)));
      v2  = HalEdg(LooLEdg(l)) != ENIL ? VerVertexNo(HalVtx(HalNxt(LooLEdg(l)))) : 0;
      MSD_lowAddEulerOpRedo(s, RMOVE, v1, v2, ff1, ff2, out, 0, 0, 0,
                            0.0, 0.0, 0.0, 0.0);
   }

   if (FacFLOut(f1) == l)
   {
      FacFLOut(f1) = InnerLoop(f1);
   }
   MSD_lowDelList(LOOP, (NPTYPE)l, (NPTYPE)f1);
   MSD_lowAddList(LOOP, (NPTYPE)l, (NPTYPE)f2);
   if (out)
   {
      FacFLOut(f2) = l;
   }

   if (SetTrace)
   {
      if (TraceSolid(s))
      {
         fprintf(stderr, MEN_MSDAbortou, NAM_RingMove);
         fprintf(stderr, "solid = %ld", SolSolidNo(s));
         fprintf(stderr, "ArrayBottom[SOLID] = %ld", ArrayBottom[SOLID]);
         exit(1);
      }
   }
   return(SUCCESS);
}

int MSD_execStructRingMove(EulerOp *op)
{
   Id sid, id1, id2, id3, id4, id5;

   sid = op->solidno;
   id1 = op->ip1;
   id2 = op->ip2;
   id3 = op->ip3;
   id4 = op->ip4;
   id5 = op->ip5;
   return(MSD_highRingMove(sid, id1, id2, id3, id4, id5));
}

void MSD_printStructRingMove(FILE *f, EulerOp *op)
{
   Id  sid, id1, id2, id3, id4, id5;
   int com;

   sid = op->solidno;
   id1 = op->ip1;
   id2 = op->ip2;
   id3 = op->ip3;
   id4 = op->ip4;
   id5 = op->ip5;
   com = op->CommandCount;
   fprintf(f, "rmove %d %d %d %d %d %d %d \n", sid, com, id1, id2, id3, id4, id5);
}

int MSD_readStructRingMove(FILE *f, EulerOp *op)
{
   int solidno, ip1, ip2, ip3, ip4, ip5, com;

   if (fscanf(f, "%d %d %d %d %d %d\n", &solidno, &com, &ip1, &ip2, &ip3, &ip4,
              &ip5) != 7)
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
   op->ip6          = 0;
   op->ip7          = 0;
   op->ip8          = 0;
   op->fp1          = 0.0;
   op->fp2          = 0.0;
   op->fp3          = 0.0;
   op->fp4          = 0.0;
   return(SUCCESS);
}

int MSD_modifyStructRingMove(EulerOp *op)
{
   int solidno;

   solidno     = MSD_lowCheckTranslatorTable(op->solidno, SOLID);
   op->solidno = solidno;
   return(SUCCESS);
}

#ifdef __Extended_Version
void MSD_execAllRingMove(void)
{
   int sn, f1, f2;

   while (sscanf(restbuf, "%d %d %d", &sn, &f1, &f2) != 3)
   {
      printf("aringmv: Sid f1 f2");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highAllRingMove(sn, f1, f2);
}

#endif

int MSD_highAllRingMove(Id sn, Id f1, Id f2)
{
   SPTYPE s;
   FPTYPE ff1;
   FPTYPE ff2;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_AllRingMove, sn);
      return(ERROR);
   }
   if ((ff1 = MSD_getFace(s, f1)) == FNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouFace, NAM_AllRingMove, f1, sn);
      return(ERROR);
   }
   if ((ff2 = MSD_getFace(s, f2)) == FNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouFace, NAM_AllRingMove, f2, sn);
      return(ERROR);
   }
   return(MSD_lowAllRingMove(ff1, ff2));
}

int MSD_lowAllRingMove(FPTYPE f1, FPTYPE f2)
{
   SPTYPE s;
   LPTYPE l;
   LPTYPE ll;
   Id     v1, v2, ff1, ff2;
   int    drop, flag;

   s = SheSSolids(FacFShell(f1));
   MSD_lowFaceEq(FacFLOut(f1), FacFeq(f1));
   drop = getdrop(FacFeq(f1));

   for (l = ll = FacFLoops(f1); l != LNIL; l = ll)
   {
      ll = LooNextL(l);
      if (l != FacFLOut(f1))
      {
         if (MSD_lowFacVtxCoordContainment(f1, HalVtx(LooLEdg(l)), drop) == 0)
         {
            if (Gdoinversion)
            {
               v1   = VerVertexNo(HalVtx(LooLEdg(l)));
               v2   = HalEdg(LooLEdg(l)) ? VerVertexNo(HalVtx(HalNxt(LooLEdg(l)))) : 0;
               ff2  = FacFaceNo(f2);
               ff1  = FacFaceNo(f1);
               flag = l == FacFLOut(f1) ? TRUE : FALSE;
               MSD_lowAddEulerOp(s, RMOVE, v1, v2, ff2, ff1, flag,
                                 0, 0, 0, 0.0, 0.0, 0.0, 0.0);
            }
            MSD_lowDelList(LOOP, (NPTYPE)l, (NPTYPE)f1);
            MSD_lowAddList(LOOP, (NPTYPE)l, (NPTYPE)f2);
         }
      }
   }

   if (SetTrace)
   {
      if (TraceSolid(s))
      {
         fprintf(stderr, MEN_MSDAbortou, NAM_AllRingMove);
         exit(1);
      }
   }
   return(SUCCESS);
}
