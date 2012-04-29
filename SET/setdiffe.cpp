/* 
  PROJETO USPDesigner
  MODULO: SET (Operadores Booleanos)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: setdiffe.cpp
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
#include "mancommd.h"
#include "genfunc_.h"
#include "shpshape.h"
#include "disdispl.h"
#include "sethmain.h"

void MSD_execNameDifference(void)
{
   char name1[15], name2[15], name3[15];

   while (3 != sscanf(restbuf, "%s %s %s", name1, name2, name3))
   {
      printf("Nome1 Nome2 NomeR\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highNameDifference(name1, name2, name3);
}

void MSD_highNameDifference(char *name1, char *name2, char *name)
{
   Id sn1, sn2, sn;

   if (MSD_getSolidIdFromName(name) != -1)
   {
      fprintf(stderr, MEN_SolidoJaExiste, NAM_SetDifference, name);
      return;
   }
   if ((sn1 = MSD_getSolidIdFromName(name1)) == -1)
   {
      fprintf(stderr, MEN_NaoEncontrouSolidNome, NAM_SetDifference, name1);
      return;
   }
   if ((sn2 = MSD_getSolidIdFromName(name2)) == -1)
   {
      fprintf(stderr, MEN_NaoEncontrouSolidNome, NAM_SetDifference, name2);
      return;
   }
   if ((sn = MSD_highDifference(sn1, sn2)) != -1)
   {
      MSD_highName(sn, name, SOLID);
   }
}

#ifdef __Extended_Version
void MSD_execDifference(void)
{
   int sn1, sn2, sn;

   while (3 != sscanf(restbuf, "%d %d %d", &sn1, &sn2, &sn))
   {
      printf("difference: Sid1 Sid2\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   maxs = sn;
   printf("Id = %d\n", MSD_highDifference(sn1, sn2));
}

#endif

Id MSD_highDifference(Id sn1, Id sn2)
{
   SPTYPE optr1;
   SPTYPE optr2;
   SPTYPE optr3;
   Id     sn;

   if ((optr1 = MSD_getSolid(sn1)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_SetDifference, sn1);
      return(-1);
   }
   if ((optr2 = MSD_getSolid(sn2)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_SetDifference, sn2);
      return(-1);
   }
   sn = MSD_getMaxSolidId();
   if (MSD_lowSetOp(optr1, optr2, sn, &optr3, USP_DIFFERENCE) == SUCCESS)
   {
      if (optr3 != SNIL)
      {
         MSD_lowCleanSolid(optr3);
         MSD_lowDisplayMonitor('o', optr3, GNIL);
         MSD_lowModifyIds(optr3, sn1);
      }
      return(sn1);
   }
   return(-1);
}
