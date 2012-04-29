/* 
  PROJETO USPDesigner
  MODULO: GEN (Funcoes Gerais de Nivel Superior)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: gencopy_.cpp
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
#include "hiegroup.h"
#include "shpshape.h"
#include "genfunc_.h"
#include "mancommd.h"

void MSD_execNameCopyElement(void)
{
   char name1[30], name2[30];

   while (2 != sscanf(restbuf, "%s %s", name1, name2))
   {
      printf("copy: Nome1 Nome2");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highNameCopyElement(name1, name2);
}

int MSD_highNameCopyElement(char *name1, char *name2)
{
   switch (MSD_getTypeByName(name1))
   {
       case 'o':
       return(MSD_highNameCopySolid(name1, name2));

       case 'g':
       return(MSD_highNameCopyGroup(name1, name2));
   }
   return(FALSE);
}

#ifdef __Extended_Version
void MSD_execNameCopySolid(void)
{
   char oname1[30], oname2[30];

   while (2 != sscanf(restbuf, "%s %s", oname1, oname2))
   {
      printf("CopySolid: old new\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highNameCopySolid(oname1, oname2);
}

#endif

int MSD_highNameCopySolid(char *name1, char *name2)
{
   SPTYPE sptr;
   Id     sn1, sn2;

   if ((sn1 = MSD_getSolidIdFromName(name1)) == -1)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_CopySolid, name1);
      return(ERROR);
   }
   if (MSD_getSolidIdFromName(name2) != -1)
   {
      fprintf(stderr, MEN_SolidoJaExiste, NAM_CopySolid, name2);
      return(ERROR);
   }
   if ((sn2 = MSD_highCopySolid(sn1)) != -1)
   {
      sptr = MSD_getSolid(sn2);
      strcpy(SolName(sptr), name2);
      sptr = MSD_getSolid(sn1);
      strcpy(SolName(sptr), name1);
      return(SUCCESS);
   }
   return(ERROR);
}

#ifdef __Extended_Version
void MSD_execCopySolid(void)
{
   int sn;

   while (1 != sscanf(restbuf, "%d", &sn))
   {
      printf("CopySolid: sn\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   printf("Id = %d\n", MSD_highCopySolid(sn));
}

#endif

int MSD_highCopySolid(int sn)
{
   SPTYPE sptr;
   Id     sn1;

   sn1 = MSD_getMaxSolidId();
   if ((sptr = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_CopySolid, sn);
      return(ERROR);
   }
   return(MSD_lowCopySolid(sptr, sn1) == SUCCESS ? sn1 : -1);
}

int MSD_lowCopySolid(SPTYPE s, Id sn)
{
   PEulerOp StartOp;
   PEulerOp op;
   int      cont;

   StartOp      = OpHead;
   Gdoinversion = TRUE;
   MSD_lowSoftRemove(s);
   Gdoinversion = FALSE;

   if ((s = MSD_lowApplyEulerOp(EULEROPS(OpHead), &cont)) != SNIL)
   {
      op = OpHead;
      for (EulerOpNext(op); op != StartOp; EulerOpNext(op))
      {
         MSD_lowApplyEulerOp(EULEROPS(op), &cont);
         while (cont != 1)
         {
            EulerOpNext(op);
            cont--;
         }
      }
      MSD_lowSetNormal(s, TRUE);
      MSD_lowSetEdgeAngle(s);
      MSD_lowSetInfo(s);
   }

   EulerSolidNo(OpHead) = sn;
   if ((s = MSD_lowApplyEulerOp(EULEROPS(OpHead), &cont)) != SNIL)
   {
      op = OpHead;
      for (EulerOpNext(op); op != StartOp; EulerOpNext(op))
      {
         EulerSolidNo(op) = sn;
         MSD_lowApplyEulerOp(EULEROPS(op), &cont);
         while (cont != 1)
         {
            EulerOpNext(op);
            cont--;
         }
      }
      MSD_lowSetNormal(s, TRUE);
      MSD_lowSetEdgeAngle(s);
      MSD_lowSetInfo(s);
   }

   MSD_lowFreeUndoLog(StartOp);
   return(SUCCESS);
}
