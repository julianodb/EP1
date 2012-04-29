/* 
  PROJETO USPDesigner
  MODULO: CUR (Curve Manipulation)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: curmline.cpp
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
#include "lowparam.h"
#include "lowsolid.h"
#include "curvgeom.h"
#include "mancommd.h"
#include "eulerops.h"

#ifdef __curved_env__

 #ifdef __Extended_Version
void MSD_execNewCurveLine(void)
{
   int  tag, sn;
   char sname[20];

   while (2 != sscanf(restbuf, "%d %s", &tag, sname))
   {
      printf("NEW_LINE: CTag Sname\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   if ((sn = MSD_getSolidIdFromName(sname)) == ERROR)
   {
      fprintf(stderr, MEN_NaoEncontrouSolidNome, NAM_NewLine, sname);
      return;
   }
   MSD_highNewCurveLine(sn, tag);
}

 #endif

int MSD_highNewCurveLine(Id sn, Id tag)
{
   SPTYPE s;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_NewLine, sn);
      return(ERROR);
   }
   if (MSD_lowNewCurveLine(s, tag) == (CURVE *)NIL)
   {
      return(ERROR);
   }
   return(SUCCESS);
}

CURVE *MSD_lowNewCurveLine(SPTYPE s, Id tag)
{
   CURVE *curv;

   if ((curv = MSD_lowNewCurve(LINE1, s)) == (CURVE *)NIL)
   {
      fprintf(stderr, MEN_NaoPossuiMemoria, NAM_NewLine);
      return((CURVE *)NIL);
   }
   curv->tagno = tag;
   curv->lpnt  = (LPOINT *)NIL;
   if (Gdoinversion)
   {
      MSD_lowAddEulerOp(s, KILLLINE, tag, 0, 0, 0, 0, 0, 0, 0, 0, 0.0, 0.0, 0.0);
   }
   if (Gdodirect)
   {
      MSD_lowAddEulerOpRedo(s, NEWLINE, tag, 0, 0, 0, 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0);
   }
   return(curv);
}

int MSD_execStructNEWLINE(EulerOp *op)
{
   return(MSD_highNewCurveLine(op->ip1, op->solidno));
}

void MSD_printStructNEWLINE(FILE *f, EulerOp *op)
{
   Id  ids, id1, id2;
   int com;

   ids = op->solidno;
   id1 = op->ip1;
   id2 = op->ip2;
   com = op->CommandCount;
   fprintf(f, "newline %d %d %d %d\n", ids, com, id1, id2);
}

int MSD_readStructNEWLINE(FILE *f, EulerOp *op)
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

int MSD_modifyStructNEWLINE(EulerOp *op)
{
   return(SUCCESS);
}

#endif
