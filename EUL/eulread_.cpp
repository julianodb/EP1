/* 
  PROJETO USPDesigner
  MODULO: EUL (Operadores de Euler)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: eulread_.cpp
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

/* Read an ASCII Euler Operator. Construct an Euler operator from a printed  */
/*  operator. The identical format to that of printop() is used.             */
int MSD_lowScanEulerOperator(FILE *f, EulerOp *op)
{
   EuCode *p;
   char   name[20];
   short  n;

   if (((n = fscanf(f, "%s", name)) == EOF) || (n != 1))
   {
      return(0);
   }
   strupper((unsigned char *)name);
   for (p = ematrix; *p->eop; ++p)
   {
      if (!strncmp(p->eop, name, strlen(name)))
      {
         break;
      }
   }

   op->opcode = p->ecode;
   if ((*p->reop)(f, op) == ERROR)
   {
      fprintf(stderr, MEN_ParametrosIncorretos, NAM_ScanEulerOp);
      return(ERROR);
   }
   return(SUCCESS);
}

int MSD_readStructError(FILE *f, EulerOp *op)
{
   MSD_lowNaoUtilizado(f);
   MSD_lowNaoUtilizado(op);

   fprintf(stderr, MEN_ParametrosIncorretos, NAM_ScanEulerOp);
   return(ERROR);
}
