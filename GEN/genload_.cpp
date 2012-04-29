/* 
  PROJETO USPDesigner
  MODULO: GEN (Funcoes Gerais de Nivel Superior)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: genload_.cpp
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
#include "lowmacro.h"
#include "lowsolid.h"
#include "eulerops.h"
#include "shpshape.h"
#include "genfunc_.h"
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

void MSD_execNameLoadSolid(void)
{
   char oname[30], fname[30];

   while (2 != sscanf(restbuf, "%s %s", oname, fname))
   {
      printf("LoadSolid: solido arquivo\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highNameLoadSolid(oname, fname);
}

int MSD_highNameLoadSolid(char *oname, char *fname)
{
   SPTYPE sptr;
   Id     sn;

   if (MSD_getSolidIdFromName(oname) != -1)
   {
      fprintf(stderr, "LoadSolid: solid %s already exists!\n", oname);
      return(ERROR);
   }
   if ((sn = MSD_highLoadSolid(fname)) != -1)
   {
      sptr = MSD_getSolid(sn);
      strcpy(SolName(sptr), oname);
      return(SUCCESS);
   }
   return(ERROR);
}

#ifdef __Extended_Version
void MSD_execLoadSolid(void)
{
   char fname[30];

   while (1 != sscanf(restbuf, "%s", fname))
   {
      printf("LoadSolid: sn arquivo\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   printf("Id = %d\n", MSD_highLoadSolid(fname));
}

#endif

int MSD_highLoadSolid(char *fname)
{
   Id sn;

   sn = MSD_getMaxSolidId();
   return(MSD_lowLoadSolid(sn, fname) ? sn : -1);
}

int MSD_lowLoadSolid(Id sn, char *fname)
{
   SPTYPE   s;
   SPTYPE   sret;
   EulerOp  eop;
   PEulerOp StartOp;
   EuCode   *p;
   int      cont;
   FILE     *arq;

   if ((arq = fopen(fname, "r")) == (FILE *)NIL)
   {
      fprintf(stderr, MEN_NaoAbriuArquivo, NAM_LoadSolido, fname);
      return(FALSE);
   }

   while (MSD_lowScanEulerOperator(arq, &eop) == SUCCESS)
   {
      eop.solidno = sn;

      for (p = ematrix; *p->eop; ++p)
      {
         if (p->ecode == eop.opcode)
         {
            break;
         }
      }
      if (p->num != 1)
      {
         cont    = p->num;
         StartOp = OpHead;
         MSD_lowAddEulerOp(SNIL, eop.opcode, eop.ip1, eop.ip2, eop.ip3,
                           eop.ip4, eop.ip5, eop.ip6,
                           eop.ip7, eop.ip8, eop.fp1,
                           eop.fp2, eop.fp3, eop.fp4);
         do
         {
            MSD_lowScanEulerOperator(arq, &eop);
            MSD_lowAddEulerOp(SNIL, eop.opcode, eop.ip1, eop.ip2, eop.ip3,
                              eop.ip4, eop.ip5, eop.ip6,
                              eop.ip7, eop.ip8, eop.fp1,
                              eop.fp2, eop.fp3, eop.fp4);
         } while (--cont != 1);
         if ((sret = MSD_lowApplyEulerOp(&eop, &cont)) != SNIL)
         {
            s = sret;
         }
         MSD_lowFreeUndoLog(StartOp);
      }
      else
      if ((sret = MSD_lowApplyEulerOp(&eop, &cont)) != SNIL)
      {
         s = sret;
      }
   }
   if (s != SNIL)
   {
      MSD_lowSetNormal(s, TRUE);
      MSD_lowSetEdgeAngle(s);
      MSD_lowSetInfo(s);
   }

   fclose(arq);
   return(TRUE);
}
