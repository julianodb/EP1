/* 
  PROJETO USPDesigner
  MODULO: SHP (Criacao de Formas Primitivas)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: shpmrevo.cpp
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
#include "lowsolid.h"
#include "mancommd.h"
#include "shpshape.h"
#include "disdispl.h"
#include "eulerops.h"

void MSD_execNameMakeRevolve(void)
{
   XY *listvtx;
   char name1[15];
   int m, n;

   while (3 != sscanf(restbuf, "%s %d %d", name1, &m, &n))
   {
      printf("Objeto N_div N_pontos\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   if ((n > 2) && setvtpi(&listvtx, n))
   {
      MSD_highNameMakeRevolve(name1, m, n, listvtx);
   }
}

void MSD_highNameMakeRevolve(char *name, int m, int n, XY *listvtx)
{
   Id sn;

   if (MSD_getSolidIdFromName(name) != -1)
   {
      fprintf(stderr, MEN_SolidoJaExiste, NAM_MakeRevolve, name);
      return;
   }
   if ((sn = MSD_highMakeRevolve(m, n, listvtx)) != -1)
   {
      MSD_highName(sn, name, SOLID);
   }
}

#ifdef __Extended_Version
void MSD_execMakeRevolve(void)
{
   XY *listvtx;
   int m, n;

   while (2 != sscanf(restbuf, "%d %d", &m, &n))
   {
      printf("N_div N_pontos\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   if ((n > 2) && setvtpi(&listvtx, n))
   {
      printf("Id = %d\n", MSD_highMakeRevolve(m, n, listvtx));
   }
}

#endif

Id MSD_highMakeRevolve(int m, int n, XY *listvtx)
{
   SPTYPE optr;
   Id sn;

   sn = MSD_getMaxSolidId();
   if ((m > 2) && (n > 2))
   {
      if ((optr = MSD_lowMakeRevolve(sn, listvtx, m, n)) != SNIL)
      {
         MSD_lowDisplayMonitor('o', optr, GNIL);
         return(sn);
      }
   }
   fprintf(stderr, MEN_ParametrosIncorretos, NAM_MakeRevolve);
   return(-1);
}
