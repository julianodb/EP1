/* 
  PROJETO USPDesigner
  MODULO: HIE (Estrutura Hierarquica)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: hiegrpcs.cpp
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
#include "hiegroup.h"

#ifdef __Extended_Version
void MSD_execNameCreateSetGroup(void)
{
   char gname[30], name[30];

   while (2 != sscanf(restbuf, "%s %s", gname, name))
   {
      printf("CreateSetGroup: name set\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highNameCreateSetGroup(gname, name);
}

#endif

int MSD_highNameCreateSetGroup(char *name1, char *name2)
{
   GPTYPE gptr;
   int    gn;

   if (MSD_getGroupIdFromName(name1) != -1)
   {
      fprintf(stderr, MEN_GroupJaExiste, NAM_CreateGroupSetCurrent, name1);
      return(ERROR);
   }
   if ((gn = MSD_highCreateSetGroup(!strcmp(name2, "TRUE"))) != -1)
   {
      gptr = MSD_getGroup(gn);
      strcpy(GrpName(gptr), name1);
      return(SUCCESS);
   }
   return(ERROR);
}

#ifdef __Extended_Version
void MSD_execCreateSetGroup(void)
{
   int flag;

   while (1 != sscanf(restbuf, "%d", &flag))
   {
      printf("CreateSetGroup: flag\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   printf("Id = %d\n", MSD_highCreateSetGroup(flag));
}

#endif

int MSD_highCreateSetGroup(int flag)
{
   Id gn;

   gn = MSD_getMaxGroupId();
   if (MSD_lowCreateGroup(gn) == ERROR)
   {
      return(-1);
   }
   if (flag)
   {
      MSD_highSetGroupCurrent(gn);
   }
   return(gn);
}
