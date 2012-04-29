/* 
  PROJETO USPDesigner
  MODULO: HIE (Estrutura Hierarquica)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: hiegrpcr.cpp
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
#include "vectorop.h"
#include "mancommd.h"
#include "genfunc_.h"
#include "eulerops.h"
#include "hiegroup.h"

#ifdef __Extended_Version
void MSD_execNameCreateGroup(void)
{
   char name[30];

   while (1 != sscanf(restbuf, "%s", name))
   {
      printf("crtgrp: nome\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highNameCreateGroup(name);
}

#endif

int MSD_highNameCreateGroup(char *name)
{
   GPTYPE gptr;
   int    gn;

   if (MSD_getGroupIdFromName(name) != -1)
   {
      fprintf(stderr, MEN_GroupJaExiste, NAM_CreateGroup, name);
      return(ERROR);
   }
   if ((gn = MSD_highCreateGroup()) != -1)
   {
      gptr = MSD_getGroup(gn);
      strcpy(GrpName(gptr), name);
      return(SUCCESS);
   }
   return(ERROR);
}

#ifdef __Extended_Version
void MSD_execCreateGroup(void)
{
   printf("Id = %d\n", MSD_highCreateGroup());
}

#endif

int MSD_highCreateGroup(void)
{
   Id gn;

   gn = MSD_getMaxGroupId();
   return(MSD_lowCreateGroup(gn) == ERROR ? -1 : gn);
}

int MSD_middleCreateGroup(int gn)
{
   return(MSD_lowCreateGroup(gn));
}

int MSD_lowCreateGroup(Id gn)
{
   GPTYPE gptr;

   if ((gptr = (GPTYPE)MSD_lowNewElement(GROUP, NNIL)) == GNIL)
   {
      fprintf(stderr, MEN_NaoPossuiMemoria, NAM_CreateGroup);
      return(ERROR);
   }
   GrpGroupNo(gptr) = gn;

   if (Gdoinversion)
   {
      MSD_lowAddEulerOp(SNIL, REMGRP, gn, 0, 0, 0, 0, 0, 0, 0,
                        0.0, 0.0, 0.0, 0.0);
   }
   return(SUCCESS);
}

int MSD_execStructCreateGroup(EulerOp *op)
{
   return(MSD_middleCreateGroup(op->ip1));
}

void MSD_printStructCreateGroup(FILE *f, EulerOp *op)
{
   Id sid, ip1;

   sid = op->solidno;
   ip1 = op->ip1;
   fprintf(f, "crtgrp %d %d\n", sid, ip1);
}

int MSD_readStructCreateGroup(FILE *f, EulerOp *op)
{
   int solidno, ip1;

   if (fscanf(f, "%d %d\n", &solidno, &ip1) != 2)
   {
      return(ERROR);
   }
   op->solidno = solidno;
   op->ip1     = ip1;
   op->ip2     = 0;
   op->ip3     = 0;
   op->ip4     = 0;
   op->ip5     = 0;
   op->ip6     = 0;
   op->ip7     = 0;
   op->ip8     = 0;
   op->fp1     = 0.0;
   op->fp2     = 0.0;
   op->fp3     = 0.0;
   op->fp4     = 0.0;
   return(SUCCESS);
}

int MSD_modifyStructCreateGroup(EulerOp *op)
{
   int groupno;

   groupno = MSD_lowCheckTranslatorTable(op->ip1, GROUP);
   op->ip1 = groupno;
   return(SUCCESS);
}
