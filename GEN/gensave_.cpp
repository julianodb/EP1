/* 
  PROJETO USPDesigner
  MODULO: GEN (Funcoes Gerais de Nivel Superior)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: gensave_.cpp
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
#include "genfunc_.h"
#include "shpshape.h"
#include "disdispl.h"
#include "mancommd.h"

void MSD_execNameSaveSolid(void)
{
   char oname[30], fname[30];

   while (2 != sscanf(restbuf, "%s %s", oname, fname))
   {
      printf("SaveSolid: objeto arquivo\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highNameSaveSolid(oname, fname);
}

int MSD_highNameSaveSolid(char *oname, char *fname)
{
   Id sn;

   if ((sn = MSD_getSolidIdFromName(oname)) == -1)
   {
      fprintf(stderr, MEN_NaoEncontrouSolidNome, NAM_SaveSolido, oname);
      return(FALSE);
   }
   if (!MSD_highSaveSolid(sn, fname))
   {
      return(FALSE);
   }
   strcpy(SolName(MSD_getSolid(sn)), oname);
   return(TRUE);
}

#ifdef __Extended_Version
void MSD_execSaveSolid(void)
{
   char fname[30];
   int  sn;

   while (2 != sscanf(restbuf, "%d %s", &sn, fname))
   {
      printf("SaveSolid: sn arquivo\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highSaveSolid(sn, fname);
}

#endif

int MSD_highSaveSolid(int sn, char *fname)
{
   SPTYPE s;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_SaveSolido, sn);
      return(FALSE);
   }
   return(MSD_lowSaveSolid(s, fname));
}

int MSD_lowSaveSolid(SPTYPE s, char *fname)
{
   PEulerOp StartOp;
   PEulerOp op;
   Id       sn;
   int      cont;
   FILE     *arq;

   if ((arq = fopen(fname, "w")) == (FILE *)NIL)
   {
      fprintf(stderr, MEN_NaoAbriuArquivo, NAM_SaveSolido, fname);
      return(FALSE);
   }

   sn           = SolSolidNo(s);
   StartOp      = OpHead;
   Gdoinversion = TRUE;
   Gdodirect    = FALSE;
   MSD_lowSoftRemove(s);
   Gdoinversion = FALSE;

   for (op = OpHead; op != StartOp; EulerOpNext(op))
   {
      MSD_lowPrintEulerOp(arq, EULEROPS(op));
   }

   if ((s = MSD_lowApplyEulerOp(EULEROPS(OpHead), &cont)) != SNIL)
   {
      op = OpHead;
      for (EulerOpNext(op); op != StartOp; EulerOpNext(op))
      {
         MSD_lowApplyEulerOp(EULEROPS(op), &cont);
      }
      SolSolidNo(s) = sn;
      MSD_lowSetNormal(s, TRUE);
      MSD_lowSetEdgeAngle(s);
      MSD_lowSetInfo(s);
   }
   MSD_lowFreeUndoLog(StartOp);
   Gdodirect = TRUE;
   fclose(arq);
   return(TRUE);
}
