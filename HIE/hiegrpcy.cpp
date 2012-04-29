/* 
  PROJETO USPDesigner
  MODULO: HIE (Estrutura Hierarquica)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: hiegrpcy.cpp
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
#include "vectorop.h"
#include "mancommd.h"
#include "genfunc_.h"
#include "hiegroup.h"
#include "shpshape.h"

#ifdef __Extended_Version
void MSD_execNameCopyGroup(void)
{
   char gname1[30], gname2[30];

   while (2 != sscanf(restbuf, "%s %s", gname1, gname2))
   {
      printf("Copy Group: old new\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highNameCopyGroup(gname1, gname2);
}

#endif

int MSD_highNameCopyGroup(char *name1, char *name2)
{
   GPTYPE gptr;
   Id     gn1, gn2;

   if ((gn1 = MSD_getGroupIdFromName(name1)) == -1)
   {
      fprintf(stderr, MEN_NaoEncontrouGroupNome,
              NAM_CopyGroup, name1);
      return(ERROR);
   }
   if (MSD_getGroupIdFromName(name2) != -1)
   {
      fprintf(stderr, MEN_GroupJaExiste,
              NAM_CopyGroup, name2);
      return(ERROR);
   }
   if ((gn2 = MSD_highCopyGroup(gn1)) != -1)
   {
      gptr = MSD_getGroup(gn2);
      strcpy(GrpName(gptr), name2);
      return(SUCCESS);
   }
   return(ERROR);
}

#ifdef __Extended_Version
void MSD_execCopyGroup(void)
{
   int gn;

   while (1 != sscanf(restbuf, "%d", &gn))
   {
      printf("Copy Group: gn\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   printf("Id = %d\n", MSD_highCopyGroup(gn));
}

#endif

int MSD_highCopyGroup(int gn)
{
   GPTYPE gptr;
   Id     gn1;

   gn1 = MSD_getMaxGroupId();
   if ((gptr = MSD_getGroup(gn)) == GNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouGroupId,
              NAM_CopyGroup, gn);
      return(ERROR);
   }
   return(MSD_lowCopyGroup(gptr, gn1));
}

int MSD_lowCopyGroup(GPTYPE gptr, Id gn)
{
   PEulerOp StartOp;
   PEulerOp op;
   SPTYPE   optr;
   GPTYPE   goptr;
   int      cont, GdoInvOld;
   Id       gnold;

   StartOp      = OpHead;
   GdoInvOld    = Gdoinversion;
   Gdoinversion = TRUE;
   gnold        = GrpGroupNo(gptr);
   MSD_highRemoveGroup(GrpGroupNo(gptr));
   Gdoinversion = FALSE;

   for (op = OpHead; op != StartOp; EulerOpNext(op))
   {
      MSD_lowApplyEulerOp(EULEROPS(op), &cont);
      while (cont != 1)
      {
         EulerOpNext(op);
         cont--;
      }
   }

   MSD_lowIniciaTranslatorTable();
   MSD_lowSetTranslatorTable(gnold, gn, GROUP);
   for (op = OpHead; op != StartOp; EulerOpNext(op))
   {
      MSD_lowModifyEulerOp(EULEROPS(op));
      MSD_lowApplyEulerOp(EULEROPS(op), &cont);
      while (cont != 1)
      {
         EulerOpNext(op);
         cont--;
      }
   }

   MSD_lowFreeUndoLog(StartOp);
   Gdoinversion = GdoInvOld;

   goptr = MSD_getGroup(gn);
   for (AllSolids(optr))
   {
      if (MSD_lowIsAncestorGroup(gptr, SolGroup(optr)) ||
          MSD_lowIsAncestorGroup(goptr, SolGroup(optr)))
      {
         MSD_lowSetNormal(optr, TRUE);
         MSD_lowSetEdgeAngle(optr);
         MSD_lowSetInfo(optr);
      }
   }

   return(gn);
}
