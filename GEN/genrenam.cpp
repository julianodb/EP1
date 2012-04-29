/* 
  PROJETO USPDesigner
  MODULO: GEN (Funcoes Gerais de Nivel Superior)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: genrenam.cpp
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
#include "hiegroup.h"
#include "genfunc_.h"
#include "disdispl.h"
#include "mancommd.h"
#include "eulerops.h"

void MSD_execNameRenameElement(void)
{
   char name1[15], name2[15];

   while (2 != sscanf(restbuf, "%s %s", name1, name2))
   {
      printf("rename: Nome antigo  Nome novo\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highNameRenameElement(name1, name2);
}

int MSD_highNameRenameElement(char *name1, char *name2)
{
   switch (MSD_getTypeByName(name1))
   {
       case 'o':
       return(MSD_highNameRenameSolid(name1, name2));

       case 'g':
       return(MSD_highNameRenameGroup(name1, name2));

       case 'a':
       return(MSD_highNameRenameAxis(name1, name2));

       case 'c':
       return(MSD_highNameRenameCamera(name1, name2));
   }
   return(FALSE);
}

#ifdef __Extended_Version
void MSD_execNameRenameSolid(void)
{
   char sname1[30], sname2[30];

   while (2 != sscanf(restbuf, "%s %s", sname1, sname2))
   {
      printf("Rename Solid: old new\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highNameRenameSolid(sname1, sname2);
}

#endif

int MSD_highNameRenameSolid(char *name1, char *name2)
{
   SPTYPE sptr;

   if ((sptr = MSD_lowFindSolidByName(name1)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolidNome, NAM_RenameSolido, name1);
      return(ERROR);
   }
   if (MSD_lowFindSolidByName(name2) != SNIL)
   {
      fprintf(stderr, MEN_SolidoJaExiste, NAM_RenameSolido, name2);
      return(ERROR);
   }
   return(MSD_lowNameRenameSolid(sptr, name2));
}

int MSD_lowNameRenameSolid(SPTYPE sptr, char *name2)
{
   MSD_highName(SolSolidNo(sptr), name2, SOLID);
   return(SUCCESS);
}
