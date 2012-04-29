/* 
  PROJETO USPDesigner
  MODULO: ROV (Criacao de Formas Especificas)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: rovquad_.cpp
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
#include "basrov__.h"

void MSD_highNameMakeQuadrado(char *name, real a, real h);

void MSD_execNameMakeQuadrado(void)
{
   char  name[30];
   float a, h;

   while (3 != sscanf(restbuf, "%s %f %f", name, &a, &h))
   {
      printf("ROV.quadrado  nome a h\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highNameMakeQuadrado(name, a, h);
}

void MSD_highNameMakeQuadrado(char *name, real a, real h)
{
   SPTYPE optr;
   Id     sn;

   if (MSD_getSolidIdFromName(name) != -1)
   {
      fprintf(stderr, MEN_SolidoJaExiste, NAM_RovQuadrado, name);
      return;
   }

   if ((sn = MSD_highMakeCube(a, a, h)) != -1)
   {
      optr = MSD_getSolid(sn);
      strcpy(SolName(optr), name);
      SolType(optr)      = SOLIDO_TIPO_BARRA;
      SolGeometria(optr) = BARRA_GEOM_QUADRADO;
      SolFronteira(optr) = FRONTEIRA_CV;
      SolParam1(optr)    = a;
      SolParam2(optr)    = h;
      makevec(SolPrincDir(optr), 0.0, 0.0, 1.0, 0.0);
   }
   if ((sn = MSD_highMakeCube(a, a, 0.0)) != -1)
   {
      SolPerfil(optr) = sn;
      strcpy(SolName(MSD_getSolid(sn)), "PerQuadrado");
   }
}
