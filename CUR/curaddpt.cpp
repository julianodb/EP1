/* 
  PROJETO USPDesigner
  MODULO: CUR (Curve Manipulation)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: curaddpt.cpp
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
#include <malloc.h>
#include "mensagem.h"
#include "memvirtu.h"
#include "lowparam.h"
#include "lowmacro.h"
#include "curvgeom.h"
#include "lowsolid.h"
#include "mancommd.h"
#include "vectorop.h"
#include "eulerops.h"

#ifdef __curved_env__

 #ifdef __Extended_Version
void MSD_execAddPointToCurve(void)
{
   int   tag, sn;
   char  sname[20];
   float x, y, z;

   while (5 != sscanf(restbuf, "%d %s %f %f %f", &tag, sname, &x, &y, &z))
   {
      printf("ADDLPOINT: CTag Sname x y z\n");
      if (!lineins("? "))
      {
         ;
      }
      return;
   }

   if ((sn = MSD_getSolidIdFromName(sname)) == ERROR)
   {
      fprintf(stderr, MEN_NaoEncontrouSolidNome, NAM_AddLPoint, sname);
      return;
   }

   MSD_highAddPointToCurve(sn, tag, x, y, z);
}

 #endif

int MSD_highAddPointToCurve(Id sn, Id tag, real x, real y, real z)
{
   SPTYPE s;
   CURVE  *curv;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_AddLPoint);
      return(ERROR);
   }
   if ((curv = MSD_getCurve(s, tag)) == (CURVE *)NIL)
   {
      fprintf(stderr, MEN_NaoEncontrouTag, NAM_AddLPoint, tag, sn);
      return(ERROR);
   }
   if ((curv->lpnt = MSD_lowAddPointToCurve(s, curv, x, y, z)) == (LPOINT *)NIL)
   {
      return(ERROR);
   }
   return(SUCCESS);
}

LPOINT *MSD_lowAddPointToCurve(SPTYPE s, CURVE *curv, real x, real y, real z)
{
   LPOINT *p;
   LPOINT *pstart = curv->lpnt;

   if (Gdoinversion)
   {
      MSD_lowAddEulerOp(s, REMLPOINT, curv->tagno, 0, 0, 0, 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0);
   }
   if (Gdodirect)
   {
      MSD_lowAddEulerOpRedo(s, ADDLPOINT, curv->tagno, 0, 0, 0, 0, 0, 0, 0, 0.0, x, y, z);
   }

   if ((p = (LPOINT *)malloc(sizeof(LPOINT))) == (LPOINT *)NIL)
   {
      fprintf(stderr, MEN_NaoPossuiMemoria, NAM_AddLPoint);
      return((LPOINT *)NIL);
   }
   makevec(p->pnt, x, y, z, 0.0);
   p->pnxt    = pstart;
   curv->lpnt = p;
   return(p);
}

int MSD_execStructADDLPOINT(EulerOp *op)
{
   return(MSD_highAddPointToCurve(op->solidno, op->ip1, op->fp1, op->fp2, op->fp3));
}

void MSD_printStructADDLPOINT(FILE *f, EulerOp *op)
{
   int  ip1, ip2, com;
   real fp1, fp2, fp3;

   ip1 = op->solidno;
   ip2 = op->ip1;
   fp1 = op->fp1;
   fp2 = op->fp2;
   fp3 = op->fp3;
   com = op->CommandCount;
   fprintf(f, "addlpoint %d %d %d %f %f %f\n", ip1, com, ip2, fp1, fp2, fp3);
}

int MSD_readStructADDLPOINT(FILE *f, EulerOp *op)
{
   int   solidno, ip1, com;
   float fp1, fp2, fp3;

   if (fscanf(f, "%d %d %d %f %f %f", &solidno, &com, &ip1, &fp1, &fp2, &fp3) != 6)
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
   op->fp1          = fp1;
   op->fp2          = fp2;
   op->fp3          = fp3;
   op->fp4          = 0.0;
   return(SUCCESS);
}

int MSD_modifyStructADDLPOINT(EulerOp *op)
{
   return(SUCCESS);
}

#endif
