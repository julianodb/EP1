/* 
  PROJETO USPDesigner
  MODULO: HIE (Estrutura Hierarquica)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: hieassgs.cpp
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
#include "vectorop.h"
#include "eulerops.h"
#include "mancommd.h"
#include "hiegroup.h"

#ifdef __Extended_Version
void MSD_execNameAssociaGroupSolid(void)
{
   char sname[30], gname[30];

   while (2 != sscanf(restbuf, "%s %s", gname, sname))
   {
      printf("AssGroupSolid: grupo solido\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highNameAssociaGroupSolid(gname, sname);
}

#endif

int MSD_highNameAssociaGroupSolid(char *name1, char *name2)
{
   int sn, gn;

   if ((gn = MSD_getGroupIdFromName(name1)) == -1)
   {
      fprintf(stderr, MEN_NaoEncontrouGroupNome,
              NAM_AssociaGroupSolid, name2);
      return(ERROR);
   }
   if ((sn = MSD_getSolidIdFromName(name2)) == -1)
   {
      fprintf(stderr, MEN_NaoEncontrouSolidNome,
              NAM_AssociaGroupSolid, name1);
      return(ERROR);
   }
   return(MSD_highAssociaGroupSolid(gn, sn));
}

#ifdef __Extended_Version
void MSD_execAssociaGroupSolid(void)
{
   int sn, gn;

   while (2 != sscanf(restbuf, "%d %d", &gn, &sn))
   {
      printf("AssGroupSolid: gn sn\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highAssociaGroupSolid(gn, sn);
}

#endif

int MSD_highAssociaGroupSolid(int gn, int sn)
{
   SPTYPE optr;
   GPTYPE gptr;

   if ((optr = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido,
              NAM_AssociaGroupSolid, sn);
      return(ERROR);
   }
   if (((gptr = MSD_getGroup(gn)) == GNIL) && (gn != -1))
   {
      fprintf(stderr, MEN_NaoEncontrouGroupId,
              NAM_AssociaGroupSolid, gn);
      return(ERROR);
   }
   return(MSD_lowAssociaGroupSolid(gptr, optr));
}

int MSD_lowAssociaGroupSolid(GPTYPE gptr, SPTYPE optr)
{
   Id code, gn, sn;

   if (Gdoinversion)
   {
      code = GroupSolid;
      sn   = SolSolidNo(optr);
      gn   = SolGroup(optr) != GNIL ? GrpGroupNo(SolGroup(optr)) : -1;
      MSD_lowAddEulerOp(SNIL, ASSOCIATE, code, gn, sn, 0, 0, 0, 0, 0,
                        0.0, 0.0, 0.0, 0.0);
   }
   SolGroup(optr) = gptr;
   return(SUCCESS);
}
