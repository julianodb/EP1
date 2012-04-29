/* 
  PROJETO USPDesigner
  MODULO: EUL (Operadores de Euler)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: eulxundo.cpp
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
#include <string.h>
#ifndef __UNIX_
 #include <malloc.h>
#endif
#include "mensagem.h"
#include "memvirtu.h"
#include "lowparam.h"
#include "lowsolid.h"
#include "curvgeom.h"
#include "eulerops.h"
#include "mancommd.h"

typedef struct eulercod   EuCode;

struct eulercod
{
   OpCode ecode;
   char   *eop;
   int    (*xeop)(EulerOp *op);
   void   (*peop)(FILE *f, EulerOp *op);
   int    (*reop)(FILE *f, EulerOp *op);
   int    (*meop)(EulerOp *op);
   int    num;
};

extern EuCode ematrix[];

/* Undo most recent op of the undo log */
void MSD_execUndoOp(void)
{
   MSD_highUndoOp();
}

void MSD_highUndoOp(void)
{
   MSD_lowUndoOp();
}

SPTYPE MSD_lowUndoOp(void)
{
   SPTYPE  s;
   EulerOp *op;
   int     Gsave, cont, i, com;

   Gsave        = Gdoinversion;
   Gdoinversion = 0;
   s            = SNIL;
   if (OpHead != EuNIL)
   {
#ifndef __VIRTUAL_MEM
      op = OpHead;
#else
      op = EULEROPS(OpHead);
#endif
      com = op->CommandCount;
      while (com == op->CommandCount)
      {
         s = MSD_lowApplyEulerOp(op, &cont);
         for (i = 0; i < cont;  i++)
         {
            EulerOpNext(OpHead);
            OpCount--;

#ifndef __VIRTUAL_MEM
            free((char *)op);
            op = OpHead;
#else
            op = EULEROPS(OpHead);
#endif
         }
      }
   }
   Gdoinversion = Gsave;
   return(s);
}

/* Evaluate an Euler Operator, undo last Euler Operator */
SPTYPE MSD_lowApplyEulerOp(EulerOp *op, int *cont)
{
   EuCode *p;

   for (p = ematrix; *p->eop; ++p)
   {
      if (p->ecode == op->opcode)
      {
         break;
      }
   }
   if ((*p->xeop)(op) == ERROR)
   {
      fprintf(stderr, MEN_ParametrosIncorretos, NAM_ApplyEulerOp);
      return(SNIL);
   }
   *cont = p->num;
   return(MSD_getSolid(op->solidno));
}

int MSD_execStructError(EulerOp *op)
{
   MSD_lowNaoUtilizado(op);

   fprintf(stderr, MEN_ParametrosIncorretos, NAM_ApplyEulerOp);
   return(ERROR);
}

void MSD_lowFreeUndoLog(PEulerOp EndOp)
{
#ifndef __VIRTUAL_MEM
   EulerOp *op;

   while (OpHead != EndOp)
   {
      if ((op = OpHead) != (EulerOp *)NIL)
      {
         OpHead = op->opnext;
         OpCount--;
         free((char *)op);
      }
   }
#else
   OpHead = EndOp;
#endif
}

/* grab a matrix from the log */
void MSD_lowGetMatrix(EulerOp *op, matrix m)
{
   PEulerOp eop;

#ifndef __VIRTUAL_MEM
   eop = op;
#else
   eop = op->pp;
#endif

   EulerOpNext(eop);
   m[0][0] = EulerFp1(eop);
   m[0][1] = EulerFp2(eop);
   m[0][2] = EulerFp3(eop);
   m[0][3] = EulerFp4(eop);

   EulerOpNext(eop);
   m[1][0] = EulerFp1(eop);
   m[1][1] = EulerFp2(eop);
   m[1][2] = EulerFp3(eop);
   m[1][3] = EulerFp4(eop);

   EulerOpNext(eop);
   m[2][0] = EulerFp1(eop);
   m[2][1] = EulerFp2(eop);
   m[2][2] = EulerFp3(eop);
   m[2][3] = EulerFp4(eop);

   EulerOpNext(eop);
   m[3][0] = EulerFp1(eop);
   m[3][1] = EulerFp2(eop);
   m[3][2] = EulerFp3(eop);
   m[3][3] = EulerFp4(eop);
}
