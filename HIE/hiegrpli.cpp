/* 
  PROJETO USPDesigner
  MODULO: HIE (Estrutura Hierarquica)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: hiegrpli.cpp
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
void MSD_execNameListGroupName(void)
{
   char gname[30];

   while (1 != sscanf(restbuf, "%s", gname))
   {
      printf("ListGroupName: name\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highNameListGroupName(gname);
}

#endif

int MSD_highNameListGroupName(char *name)
{
   GPTYPE gptr;

   if ((gptr = MSD_lowFindGroupByName(name)) == GNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouGroupNome, NAM_ListGroup, name);
      return(ERROR);
   }
   return(MSD_lowNameListGroupName(gptr));
}

int MSD_lowNameListGroupName(GPTYPE gptr)
{
   SPTYPE optr;

   printf("GRUPO = %-20s", GrpName(gptr));
   if (GrpParent(gptr) != GNIL)
   {
      printf("Grupo Ancestral = %-20s\n", GrpName(GrpParent(gptr)));
   }
   else
   {
      printf("Nao possui grupo ancestral!\n");
   }

   for (AllSolids(optr))
   {
      if (gptr == SolGroup(optr))
      {
         printf("%-20s", SolName(optr));
      }
   }
   printf("\n");
   return(SUCCESS);
}

void MSD_lowListAllGroup(void)
{
   GPTYPE gptr;

   for (AllGroup(gptr))
   {
      MSD_lowListGroup(gptr);
   }
}

void MSD_lowListGroup(GPTYPE gptr)
{
   printf("Grupo  Nome=%-18s  Cor=%2d  ", GrpName(gptr), GrpGColor(gptr));
   if (GrpParent(gptr) == GNIL)
   {
      printf("Grupo Ancestral = NULL\n");
   }
   else
   {
      printf("Grupo Ancestral = %-18s\n", GrpName(GrpParent(gptr)));
   }
   if (GrpGAxs(gptr) != ANIL)
   {
      printf("           axis = %-17s\n", AxsName(GrpGAxs(gptr)));
   }
}

void MSD_lowListAllGroupName(void)
{
   GPTYPE gptr;

   for (AllGroup(gptr))
   {
      printf("%-20s", GrpName(gptr));
   }
   printf("\n");
}
