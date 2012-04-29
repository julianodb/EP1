/* 
  PROJETO USPDesigner
  MODULO: ANA (Modulo de Analise)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: anacaren.cpp
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
#include "genfunc_.h"
#include "vectorop.h"
#include "eulerops.h"
#include "analise_.h"
#include "mancommd.h"

void MSD_execNameCentroCarena(void)
{
   char   name1[12];
   vector vec;

   while (1 != sscanf(restbuf, "%s", name1))
   {
      printf("centro carena: Nome");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highNameCentroCarena(name1, vec);
   printf("solido %s tem centro de carena %f %f %f\n", name1,
          vec[0], vec[1], vec[2]);
}

int MSD_highNameCentroCarena(char *name, vector vec)
{
   int sn;

   if ((sn = MSD_getSolidIdFromName(name)) == -1)
   {
      fprintf(stderr, MEN_NaoEncontrouSolidNome, NAM_CentroCarena, name);
      return(ERROR);
   }
   return(MSD_highCentroCarena(sn, vec));
}

int MSD_highCentroCarena(Id sn, vector vec)
{
   SPTYPE s;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_CentroCarena, sn);
      return(ERROR);
   }
   return(MSD_lowCentroCarena(s, vec));
}

int MSD_lowCentroCarena(SPTYPE s, vector ctotal)
{
   MSD_lowCentroDeGravidade(s, ctotal);
   return(SUCCESS);
}
