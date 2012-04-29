/* 
  PROJETO USPDesigner
  MODULO: TOP (Operadores de Nivel Intermediario)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: topmerge.cpp
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
#include "eulerops.h"
#include "genfunc_.h"
#include "disdispl.h"
#include "mancommd.h"

#ifdef __Extended_Version
void MSD_execNameMerge(void)
{
   char name1[30], name2[30];

   while (2 != sscanf(restbuf, "%s %s", name1, name2))
   {
      printf("merge: nome1 nome2\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highNameMerge(name1, name2);
}

#endif

int MSD_highNameMerge(char *name1, char *name2)
{
   int sn1, sn2;

   if ((sn1 = MSD_getSolidIdFromName(name1)) == -1)
   {
      fprintf(stderr, MEN_NaoEncontrouSolidNome, NAM_MergeSolid, name1);
      return(ERROR);
   }
   if ((sn2 = MSD_getSolidIdFromName(name2)) == -1)
   {
      fprintf(stderr, MEN_NaoEncontrouSolidNome, NAM_MergeSolid, name2);
      return(ERROR);
   }
   return(MSD_highMerge(sn1, sn2));
}

#ifdef __Extended_Version
void MSD_execMerge(void)
{
   int sn1, sn2;

   while (sscanf(restbuf, "%d %d", &sn1, &sn2) != 2)
   {
      printf("merge: sid1 sid2");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highMerge(sn1, sn2);
}

#endif

int MSD_highMerge(Id sn1, Id sn2)
{
   SPTYPE s1;
   SPTYPE s2;

   if ((s1 = MSD_getSolid(sn1)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_MergeSolid, sn1);
      return(ERROR);
   }
   if ((s2 = MSD_getSolid(sn2)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_MergeSolid, sn2);
      return(ERROR);
   }
   MSD_lowMerge(s1, s2);
   return(SUCCESS);
}

void MSD_lowMerge(SPTYPE s1, SPTYPE s2)
{
   DPTYPE d2;
   int    maxd, maxf, maxv;

   maxd = MSD_getMaxShellId(s1);
   maxf = MSD_getMaxFaceId(s1);
   maxv = MSD_getMaxVertexId(s1);
   while ((d2 = SolSShells(s2)) != DNIL)
   {
      MSD_lowShellDetach(s2, s1, d2, maxd++, maxf, maxv);
   }
}
