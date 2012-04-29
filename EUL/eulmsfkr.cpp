/* 
  PROJETO USPDesigner
  MODULO: EUL (Operadores de Euler)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: eulmsfkr.cpp
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
void MSD_execMSFKR(void)
{
   int sn, ff, fn, dn, op;

   while (5 != sscanf(restbuf, "%d %d %d %d %d", &sn, &op, &ff, &fn, &dn))
   {
      printf("msfkr: Sid ff fn dn\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highMSFKR(sn, ff, fn, dn);
}

#endif

int MSD_highMSFKR(Id sn, Id ff, Id fn, Id dn)
{
   SPTYPE s;
   FPTYPE f;
   LPTYPE ring;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_MSFKR, sn);
      return(ERROR);
   }
   if ((f = MSD_getFace(s, ff)) == FNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouFace, NAM_MSFKR, ff, sn);
      return(ERROR);
   }
   for (ring = FacFLoops(f); ring == FacFLOut(f); ring = LooNextL(ring))
   {
      ;
   }
   MSD_lowMSFKR(ring, fn, dn);
   return(SUCCESS);
}

#ifdef __Extended_Version
void MSD_execMSFKR2(void)
{
   int sn, ff, fn, dn, v1, v2, op;

   while (7 != sscanf(restbuf, "%d %d %d %d %d %d %d", &sn, &op, &ff, &fn,
                      &dn, &v1, &v2))
   {
      printf("msfkr2: Sid ff fn dn v1 v2\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highMSFKR2(sn, ff, fn, dn, v1, v2);
}

#endif

int MSD_highMSFKR2(Id sn, Id ff, Id fn, Id dn, Id v1, Id v2)
{
   SPTYPE s;
   FPTYPE f;
   LPTYPE ring;
   HPTYPE he1;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_MSFKR2, sn);
      return(ERROR);
   }
   if ((f = MSD_getFace(s, ff)) == FNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouFace, NAM_MSFKR2, ff, sn);
      return(ERROR);
   }
   if ((ring = MSD_getEdge(f, v1, v2, &he1)) == LNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouArestaV, NAM_MSFKR2, v1, v2, sn);
      return(ERROR);
   }
   MSD_lowMSFKR(ring, fn, dn);
   return(SUCCESS);
}

FPTYPE MSD_lowMSFKR(LPTYPE l, Id fn, Id dn)
{
   SPTYPE s;
   DPTYPE newshell;
   DPTYPE d;
   FPTYPE ff;
   FPTYPE fa;
   FPTYPE newface;
   Id     f1, v1, v2;

   assert1(LooNextL(FacFLoops(LooLFace(l))) != LNIL);

   ff = LooLFace(l);
   d  = FacFShell(ff);
   s  = SheSSolids(d);
   if (Gdoinversion)
   {
      MSD_lowAddEulerOp(s, KSFMR, FacFaceNo(ff), fn, 0, 0, 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0);
   }

/*
 *  Deve acrescentar uma movimentacao de RING - no caso especial de UNDO!!
 */
   if (Gdodirect)
   {
      f1 = FacFaceNo(ff);
      v1 = VerVertexNo(HalVtx(LooLEdg(l)));
      v2 = VerVertexNo(HalVtx(HalNxt(LooLEdg(l))));
      MSD_lowAddEulerOpRedo(s, MSFKR, f1, fn, dn, v1, v2, 0, 0, 0, 0.0, 0.0, 0.0, 0.0);
   }

   newshell             = (DPTYPE)MSD_lowNewElement(SHELL, (NPTYPE)s);
   newface              = (FPTYPE)MSD_lowNewElement(FACE, (NPTYPE)d);
   SheShellNo(newshell) = dn;
   FacFaceNo(newface)   = fn;
   MSD_lowDelList(LOOP, (NPTYPE)l, (NPTYPE)ff);
   MSD_lowAddList(LOOP, (NPTYPE)l, (NPTYPE)newface);
   FacFLOut(newface) = l;
   if (FacFLOut(ff) == l)
   {
      FacFLOut(ff) = FacFLoops(ff);
   }

   markface(newface, newshell);
   ff = SheSFaces(d);
   while (ff != FNIL)
   {
      if (FacFShell(ff) == newshell)
      {
         fa = FacNextF(ff);
         MSD_lowDelList(FACE, (NPTYPE)ff, (NPTYPE)d);
         MSD_lowAddList(FACE, (NPTYPE)ff, (NPTYPE)newshell);
         ff = fa;
      }
      else
      {
         ff = FacNextF(ff);
      }
   }
   redo(newshell, d);

   MSD_addEulCoef(s, 1, 1, -1, 0, 0, 0);

   if (SetTrace)
   {
      if (TraceSolid(s))
      {
#ifdef TRACE_BOOLEAN4
         fprintf(trace, "*** SOLID B\n");
         MSD_lowListSolid(trace, s, 1);
#endif
         fprintf(stderr, MEN_MSDAbortou, NAM_MSFKR);
         exit(1);
      }
   }
   return(newface);
}

int MSD_execStructMSFKR(EulerOp *op)
{
   Id sid, id1, id2, id3, id4, id5;

   sid = op->solidno;
   id1 = op->ip1;
   id2 = op->ip2;
   id3 = op->ip3;
   id4 = op->ip4;
   id5 = op->ip5;
   MSD_highMSFKR2(sid, id1, id2, id3, id4, id5);
   return(SUCCESS);
}

void MSD_printStructMSFKR(FILE *f, EulerOp *op)
{
   Id  sid, id1, id2, id3, id4, id5;
   int command;

   sid     = op->solidno;
   id1     = op->ip1;
   id2     = op->ip2;
   id3     = op->ip3;
   id4     = op->ip4;
   id5     = op->ip5;
   command = op->CommandCount;
   fprintf(f, "msfkr %d %d %d %d %d %d %d\n", sid, command, id1, id2, id3, id4, id5);
}

int MSD_readStructMSFKR(FILE *f, EulerOp *op)
{
   int solidno, ip1, ip2, ip3, ip4, ip5, com;

   if (fscanf(f, "%d %d %d %d %d %d %d\n", &solidno, &com, &ip1, &ip2, &ip3,
              &ip4, &ip5) != 7)
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
   op->fp1          = 0.0;
   op->fp2          = 0.0;
   op->fp3          = 0.0;
   op->fp4          = 0.0;
   return(SUCCESS);
}

int MSD_modifyStructMSFKR(EulerOp *op)
{
   int solidno;

   solidno     = MSD_lowCheckTranslatorTable(op->solidno, SOLID);
   op->solidno = solidno;
   return(SUCCESS);
}

void markface(FPTYPE f, DPTYPE d)
{
   FPTYPE f2;
   LPTYPE l;
   HPTYPE he;


   FacFShell(f) = d;
   for (AllLoopsFace(f, l))
   {
      he = LooLEdg(l);
      do
      {
         f2 = LooLFace(HalWLoop(mate(he)));
         if (FacFShell(f2) != d)
         {
            markface(f2, d);
         }
      } while ((he = HalNxt(he)) != LooLEdg(l));
   }
}

void redo(DPTYPE shell, DPTYPE oldshell)
{
   FPTYPE f;
   LPTYPE l;
   HPTYPE h;

   for (AllFacesShell(shell, f))
   {
      for (AllLoopsFace(f, l))
      {
         h = LooLEdg(l);
         do
         {
            if (HalEdg(h) != ENIL)
            {
               if (h == EdgHe1(HalEdg(h)))
               {
                  MSD_lowDelList(EDGE, (NPTYPE)HalEdg(h), (NPTYPE)oldshell);
                  MSD_lowAddList(EDGE, (NPTYPE)HalEdg(h), (NPTYPE)shell);
               }
               if (h == VerVEdge(HalVtx(h)))
               {
                  MSD_lowDelList(VERTEX, (NPTYPE)HalVtx(h), (NPTYPE)oldshell);
                  MSD_lowAddList(VERTEX, (NPTYPE)HalVtx(h), (NPTYPE)shell);
               }
            }
            else
            if (h == VerVEdge(HalVtx(h)))
            {
               MSD_lowDelList(VERTEX, (NPTYPE)HalVtx(h), (NPTYPE)oldshell);
               MSD_lowAddList(VERTEX, (NPTYPE)HalVtx(h), (NPTYPE)shell);
            }
         } while ((h = HalNxt(h)) != LooLEdg(l));
      }
   }
}

int sfindv(DPTYPE d, VPTYPE v)
{
   VPTYPE vtx;

   for (AllVerticesShell(d, vtx))
   {
      if (vtx == v)
      {
         return(TRUE);
      }
   }
   return(FALSE);
}

int sfinde(DPTYPE d, EPTYPE e)
{
   EPTYPE edg;

   for (AllEdgesShell(d, edg))
   {
      if (edg == e)
      {
         return(TRUE);
      }
   }
   return(FALSE);
}
