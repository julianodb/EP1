/* 
  PROJETO USPDesigner
  MODULO: ROV (Criacao de Formas Especificas)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: rovdmprc.cpp
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
#include "eulerops.h"
#include "vectorop.h"
#include "mancommd.h"
#include "shpshape.h"
#include "analise_.h"
#include "basrov__.h"

real MSD_highNameDimensaoPrincipal(char *name, real dx, real dy, real dz);
real MSD_highDimensaoPrincipal(Id sn, real dx, real dy, real dz);

void MSD_execNameDimensaoPrincipal(void)
{
   char  name[30];
   float dx, dy, dz;

   while (4 != sscanf(restbuf, "%s %f %f %f", name, &dx, &dy, &dz))
   {
      printf("ROV.dimensaoPrincipal  nome dx dy dz\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   printf("dimensao principal do solido %s = %f\n", name,
          MSD_highNameDimensaoPrincipal(name, dx, dy, dz));
}

real MSD_highNameDimensaoPrincipal(char *name, real dx, real dy, real dz)
{
   int sn;

   if ((sn = MSD_getSolidIdFromName(name)) == -1)
   {
      fprintf(stderr, MEN_NaoEncontrouSolidNome, NAM_DimPrinc, name);
      return(ERROR);
   }
   return(MSD_highDimensaoPrincipal(sn, dx, dy, dz));
}

real MSD_highDimensaoPrincipal(Id sn, real dx, real dy, real dz)
{
   SPTYPE s;
   vector dir;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_DimPrinc, sn);
      return(0.0);
   }
   makevec(dir, dx, dy, dz, 1.0);
   return(MSD_determinaDimensaoPrincipal(s, dir, 0.0));
}
