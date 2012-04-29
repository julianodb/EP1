/* 
  PROJETO USPDesigner
  MODULO: ANA (Modulo de Analise)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: anavolum.cpp
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

void MSD_execNameVolume(void)
{
   char name1[15];

   while (1 != sscanf(restbuf, "%s", name1))
   {
      printf("volume: Nome");
      if (!lineins("? "))
      {
         return;
      }
   }
   printf("solido %s tem volume de %f\n", name1, MSD_highNameVolume(name1));
}

real MSD_highNameVolume(char *name)
{
   int sn;

   if ((sn = MSD_getSolidIdFromName(name)) == -1)
   {
      fprintf(stderr, MEN_NaoEncontrouSolidNome, NAM_Volume, name);
      return(0.0);
   }
   return(MSD_highVolume(sn));
}

#ifdef __Extended_Version
void MSD_execVolume(void)
{
   int sn;

   while (1 != sscanf(restbuf, "%d", &sn))
   {
      printf("volume: sn");
      if (!lineins("? "))
      {
         return;
      }
   }
   printf("solido %d tem volume de %f\n", sn, MSD_highVolume(sn));
}

#endif

real MSD_highVolume(Id sn)
{
   SPTYPE s;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_Volume, sn);
      return(0.0);
   }
   return(MSD_lowVolume(s));
}

/* Calculate signed volume of a solid */
real MSD_lowVolume(SPTYPE s)
{
   DPTYPE d;
   FPTYPE f;
   LPTYPE l;
   HPTYPE he1;
   HPTYPE he2;
   VPTYPE v;
   vector a, b, c, cent;
   double res;
   int    len;

   cent[0] = cent[1] = cent[2] = 0.0;
   len     = 0;
   for (AllShellsSolid(s, d))
   {
      for (AllVerticesShell(d, v))
      {
         vecplus(cent, cent, VerVCoord(v));
         len++;
      }
   }
   cent[0] = cent[0] / len;
   cent[1] = cent[1] / len;
   cent[2] = cent[2] / len;
   cent[3] = 0.0;

   res = 0.0;
   for (AllShellsSolid(s, d))
   {
      for (AllFacesShell(d, f))
      {
         for (AllLoopsFace(f, l))
         {
            he1 = LooLEdg(l);
            he2 = HalNxt(he1);
            do
            {
               vecminus(a, VerVCoord(HalVtx(he2)), cent);
               vecminus(b, VerVCoord(HalVtx(he1)), cent);
               cross(c, a, b);
               vecminus(b, VerVCoord(HalVtx(HalNxt(he2))), cent);
               res += dot(b, c);
            } while ((he2 = HalNxt(he2)) != he1);
         }
      }
   }
   return(-res / 6.0);
}
