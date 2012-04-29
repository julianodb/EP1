/* 
  PROJETO USPDesigner
  MODULO: HIE (Estrutura Hierarquica)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: hiedesgs.cpp
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
void MSD_execNameDesassociaGroupSolid(void)
{
   char gname[30], sname[30];

   while (2 != sscanf(restbuf, "%s %s", gname, sname))
   {
      printf("DesGroupSolid: group solid\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highNameDesassociaGroupSolid(gname, sname);
}

#endif

int MSD_highNameDesassociaGroupSolid(char *name1, char *name2)
{
   int sn, gn;

   if ((gn = MSD_getGroupIdFromName(name1)) == -1)
   {
      fprintf(stderr, MEN_NaoEncontrouGroupNome,
              NAM_DesassociaGroupSolid, name1);
      return(ERROR);
   }
   if ((sn = MSD_getSolidIdFromName(name2)) == -1)
   {
      fprintf(stderr, MEN_NaoEncontrouSolidNome,
              NAM_DesassociaGroupSolid, name2);
      return(ERROR);
   }
   return(MSD_highAssociaGroupSolid(gn, sn));
}

#ifdef __Extended_Version
void MSD_execDesassociaGroupSolid(void)
{
   int sn, gn;

   while (2 != sscanf(restbuf, "%d %d", &gn, &sn))
   {
      printf("DesGroupSolid: gn sn\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highDesassociaGroupSolid(gn, sn);
}

#endif

int MSD_highDesassociaGroupSolid(int gn, int sn)
{
   GPTYPE gptr;
   SPTYPE sptr;

   if ((gptr = MSD_getGroup(gn)) == GNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouGroupId,
              NAM_DesassociaGroupSolid, gn);
      return(ERROR);
   }
   if ((sptr = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido,
              NAM_DesassociaGroupSolid, sn);
      return(ERROR);
   }
   if (SolGroup(sptr) == gptr)
   {
      return(MSD_lowAssociaGroupSolid(GNIL, sptr));
   }
   fprintf(stderr, MEN_ParametrosIncorretos, NAM_DesassociaGroupSolid);
   return(ERROR);
}
