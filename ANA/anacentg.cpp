/* 
  PROJETO USPDesigner
  MODULO: ANA (Modulo de Analise)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: anacentg.cpp
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
#include "mancommd.h"
#include "analise_.h"

void MSD_execNameCentroDeGravidade(void)
{
   char   name[12];
   vector centro;

   while (1 != sscanf(restbuf, "%s", name))
   {
      printf("CentroG: Nome");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highNameCentroDeGravidade(name, centro);
   printf("Solido tem centro de gravidade %f %f %f\n", centro[0],
          centro[1],
          centro[2]);
}

int MSD_highNameCentroDeGravidade(char *name, vector vec)
{
   int sn;

   if ((sn = MSD_getSolidIdFromName(name)) == -1)
   {
      fprintf(stderr, MEN_NaoEncontrouSolidNome, NAM_CentroGravidade, name);
      return(ERROR);
   }
   return(MSD_highCentroCarena(sn, vec));
}

int MSD_highCentroDeGravidade(Id sn, vector vec)
{
   SPTYPE s;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_CentroGravidade, sn);
      return(ERROR);
   }
   return(MSD_lowCentroDeGravidade(s, vec));
}

/* Calculate signed cent of mass of a solid */
int MSD_lowCentroDeGravidade(SPTYPE s, vector centg)
{
   DPTYPE dg;
   FPTYPE f;
   LPTYPE l;
   HPTYPE he1;
   HPTYPE he2;
   vector a, b, c, d, cen;
   real   volume;

   veczer(centg);
   veczer(cen);
   veczer(a);
   veczer(b);
   veczer(c);

   for (AllShellsSolid(s, dg))
   {
      for (AllFacesShell(dg, f))
      {
         for (AllLoopsFace(f, l))
         {
            he1 = LooLEdg(l);
            he2 = HalNxt(he1);
            do
            {
               vecminus(a, VerVCoord(HalVtx(he2)), cen);
               vecminus(b, VerVCoord(HalVtx(he1)), cen);
               cross(c, a, b);
               vecminus(d, VerVCoord(HalVtx(HalNxt(he2))), cen);
               centg[0] += dot(d, c) * (a[0] + b[0] + d[0]);
               centg[1] += dot(d, c) * (a[1] + b[1] + d[1]);
               centg[2] += dot(d, c) * (a[2] + b[2] + d[2]);
            } while ((he2 = HalNxt(he2)) != he1);
         }
      }
   }

   volume   = MSD_lowVolume(s) * 24.0;
   centg[0] = -centg[0] / volume;
   centg[1] = -centg[1] / volume;
   centg[2] = -centg[2] / volume;
   return(SUCCESS);
}
