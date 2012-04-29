/* 
  PROJETO USPDesigner
  MODULO: EUL (Operadores de Euler)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: eulmodif.cpp
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

typedef struct TransTable   TRANSTABLE;

struct TransTable
{
   Id OldNo;
   Id NewNo;
};

static TRANSTABLE STable[100], GTable[100], ATable[100];
static int        STableMax, GTableMax, ATableMax;

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

/* Evaluate the Euler Operator and modify the Ids */
SPTYPE MSD_lowModifyEulerOp(EulerOp *op)
{
   EuCode *p;

   for (p = ematrix; *p->eop; ++p)
   {
      if (p->ecode == op->opcode)
      {
         break;
      }
   }
   if ((*p->meop)(op) == ERROR)
   {
      fprintf(stderr, MEN_ParametrosIncorretos, NAM_ModifyEulerOp);
      return(SNIL);
   }
   return(MSD_getSolid(op->solidno));
}

int MSD_modifyStructError(EulerOp *op)
{
   MSD_lowNaoUtilizado(op);

   fprintf(stderr, MEN_ParametrosIncorretos, NAM_ModifyEulerOp);
   return(ERROR);
}

void MSD_lowIniciaTranslatorTable(void)
{
   STableMax = ATableMax = GTableMax = 0;
}

int MSD_lowCheckTranslatorTable(int elemno, int code)
{
   int i;

   if (elemno == -1)
   {
      return(-1);
   }

   switch (code)
   {
       case SOLID:
       for (i = 0; i < STableMax; i++)
       {
          if (STable[i].OldNo == elemno)
          {
             return(STable[i].NewNo);
          }
       }
       STable[STableMax].OldNo   = elemno;
       STable[STableMax++].NewNo = MSD_getMaxSolidId();
       return(STable[STableMax - 1].NewNo);

       case AXIS:
       for (i = 0; i < ATableMax; i++)
       {
          if (ATable[i].OldNo == elemno)
          {
             return(ATable[i].NewNo);
          }
       }
       ATable[ATableMax].OldNo   = elemno;
       ATable[ATableMax++].NewNo = MSD_getMaxAxisId();
       return(ATable[ATableMax - 1].NewNo);

       case GROUP:
       for (i = 0; i < GTableMax; i++)
       {
          if (GTable[i].OldNo == elemno)
          {
             return(GTable[i].NewNo);
          }
       }
       GTable[GTableMax].OldNo   = elemno;
       GTable[GTableMax++].NewNo = MSD_getMaxGroupId();
       return(GTable[GTableMax - 1].NewNo);
   }
   return(-1);
}

void MSD_lowSetTranslatorTable(int elemnoold, int elemno, int code)
{
   if ((elemnoold == -1) || (elemno == -1))
   {
      return;
   }
   switch (code)
   {
       case SOLID:
       STable[STableMax].OldNo   = elemnoold;
       STable[STableMax++].NewNo = elemno;
       return;

       case GROUP:
       GTable[GTableMax].OldNo   = elemnoold;
       GTable[GTableMax++].NewNo = elemno;
       return;

       case AXIS:
       ATable[ATableMax].OldNo   = elemnoold;
       ATable[ATableMax++].NewNo = elemno;
       return;
   }
}
