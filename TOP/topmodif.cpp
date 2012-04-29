/* 
  PROJETO USPDesigner
  MODULO: TOP (Operadores de Nivel Intermediario)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: topmodif.cpp
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
#include "lowmacro.h"
#include "lowsolid.h"
#include "eulerops.h"
#include "mancommd.h"

/* Change (increment or decrement) face and vertex id's of a solid */
#ifdef __Extended_Version
void MSD_execModifyIds(void)
{
   int sn, snew, op;

   while (sscanf(restbuf, "%d %d %d", &sn, &op, &snew) != 3)
   {
      printf("ModifyIds: Sid Snew");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highModifyIds(sn, snew);
}

#endif

int MSD_highModifyIds(Id sn, Id snew)
{
   SPTYPE s;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_ModifyIds, sn);
      return(ERROR);
   }
   MSD_lowModifyIds(s, snew);
   return(SUCCESS);
}

void MSD_lowModifyIds(SPTYPE s, int sn)
{
   Id sno;

   sno = SolSolidNo(s);
   if (Gdoinversion)
   {
      MSD_lowAddEulerOp(s, MODIFYNAMES, sn, sno, 0, 0, 0, 0, 0, 0,
                        0.0, 0.0, 0.0, 0.0);
   }
   if (Gdodirect)
   {
      MSD_lowAddEulerOpRedo(s, MODIFYNAMES, sno, sn, 0, 0, 0, 0, 0, 0,
                            0.0, 0.0, 0.0, 0.0);
   }
   SolSolidNo(s) = sn;
}

int MSD_execStructModifyIds(EulerOp *op)
{
   Id sid, id1, id2;

   sid = op->solidno;
   id1 = op->ip1;
   id2 = op->ip2;
   return(MSD_highModifyIds(id1, id2));
}

void MSD_printStructModifyIds(FILE *f, EulerOp *op)
{
   Id  sid, id2;
   int com;

   sid = op->ip1;
   id2 = op->ip2;
   com = op->CommandCount;
   fprintf(f, "modifynames %d %d %d\n", sid, com, id2);
}

int MSD_readStructModifyIds(FILE *f, EulerOp *op)
{
   int solidno, ip2, com;

   if (fscanf(f, "%d %d %d\n", &solidno, &com, &ip2) != 3)
   {
      return(ERROR);
   }
   op->solidno      = solidno;
   op->CommandCount = com;
   op->ip1          = solidno;
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

int MSD_modifyStructModifyIds(EulerOp *op)
{
   int solidno;

   solidno     = MSD_lowCheckTranslatorTable(op->solidno, SOLID);
   op->solidno = solidno;
   return(SUCCESS);
}
