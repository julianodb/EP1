/* 
  PROJETO USPDesigner
  MODULO: EUL (Operadores de Euler)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: eulkvsf_.cpp
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
/* Operator lkvsf is the inverse of mvsf, and removes the solid pointed */
/* at by s. The solid is assumed to consist of a single face and vertex */
/* only.                                                                */

#ifdef __Extended_Version
void MSD_execKVSF(void)
{
   int s, f, op;

   while (3 != sscanf(restbuf, "%d %d %d", &s, &op, &f))
   {
      printf("kvsf: Sid F\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highKVSF(s, f);
}

#endif

int MSD_highKVSF(Id sn, Id fn)
{
   SPTYPE s;
   FPTYPE f;

   if ((s = MSD_getSolid(sn)) == (SNIL))
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_KVSF, sn);
      return(ERROR);
   }
   if ((f = MSD_getFace(s, fn)) == (FNIL))
   {
      fprintf(stderr, MEN_NaoEncontrouFace, NAM_KVSF, fn, sn);
      return(ERROR);
   }
   if (LooNextL(FacFLoops(f)) != LNIL)
   {
      fprintf(stderr, MEN_FaceSemLoop, NAM_KVSF, fn, sn);
      return(ERROR);
   }
   if (HalEdg(LooLEdg(FacFLoops(f))) != (ENIL))
   {
      fprintf(stderr, MEN_FaceSemVertice, NAM_KVSF, fn, sn);
      return(ERROR);
   }
   MSD_lowKVSF(s);
   return(SUCCESS);
}

void MSD_lowKVSF(SPTYPE s)
{
   DPTYPE d;
   FPTYPE f;
   VPTYPE v;
   Id     f1, v1, dn, flag;

   d = SolSShells(s);
   f = SheSFaces(d);
   assert1(SheNextD(d) == DNIL);
   assert1(f != FNIL);
   assert1(FacNextF(f) == FNIL);
   assert1(FacFLoops(f) != LNIL);
   assert1(FacFLoops(f) == FacFLOut(f));
   assert1(LooNextL(FacFLoops(f)) == LNIL);
   assert1(SheSEdges(d) == ENIL);
   assert1(SheSVerts(d) != VNIL);
   assert1(VerNextV(SheSVerts(d)) == VNIL);

#ifdef __curved_env
   if (s->sfaces->surf != (SUSURF *)NIL)
   {
      lsettag(f, (SUSURF *)NIL);
   }
#endif

   if (Gdoinversion)
   {
      v    = SheSVerts(d);
      dn   = SheShellNo(d);
      f1   = FacFaceNo(f);
      v1   = VerVertexNo(v);
      flag = SolSBits(s);
      MSD_lowAddEulerOp(s, MVSF, f1, v1, dn, flag, 0, 0, 0, 0, VerVCoord(v)[0],
                        VerVCoord(v)[1],
                        VerVCoord(v)[2], 1.0);
   }
   if (Gdodirect)
   {
      f1 = FacFaceNo(f);
      MSD_lowAddEulerOpRedo(s, KVSF, f1, 0, 0, 0, 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0);
   }

   MSD_addEulCoef(s, -1, -1, 0, 0, -1, 0);

   MSD_lowDelElement(VERTEX, (NPTYPE)SheSVerts(d), (NPTYPE)d);
   MSD_lowDelElement(HALFEDGE, (NPTYPE)LooLEdg(FacFLoops(f)), NNIL);
   MSD_lowDelElement(LOOP, (NPTYPE)FacFLoops(f), (NPTYPE)f);
   MSD_lowDelElement(FACE, (NPTYPE)f, (NPTYPE)d);
   MSD_lowDelElement(SHELL, (NPTYPE)d, (NPTYPE)s);
   MSD_lowDelElement(SOLID, (NPTYPE)s, NNIL);

   if (SetTrace)
      if (TraceSolid(s)) 
      {
         fprintf(stderr, MEN_MSDAbortou, NAM_KVSF) ;
         exit(1) ;
      }
}

int MSD_execStructKVSF(EulerOp *op)
{
   return(MSD_highKVSF(op->solidno, op->ip1));
}

void MSD_printStructKVSF(FILE *f, EulerOp *op)
{
   fprintf(f, "kvsf %d %d %d \n", op->solidno, op->CommandCount, op->ip1);
}

int MSD_readStructKVSF(FILE *f, EulerOp *op)
{
   int solidno, ip1, com;

   if (fscanf(f, "%d %d %d\n", &solidno, &com, &ip1) != 3)
   {
      return(ERROR);
   }
   op->solidno      = solidno;
   op->CommandCount = com;
   op->ip1          = ip1;
   op->ip2          = 0;
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

int MSD_modifyStructKVSF(EulerOp *op)
{
   int solidno;

   solidno     = MSD_lowCheckTranslatorTable(op->solidno, SOLID);
   op->solidno = solidno;
   return(SUCCESS);
}
