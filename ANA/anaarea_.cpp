/* 
  PROJETO USPDesigner
  MODULO: ANA (Modulo de Analise)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: anaarea_.cpp
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
#include "vectorop.h"
#include "genfunc_.h"
#include "analise_.h"
#include "mancommd.h"

void MSD_execNameArea(void)
{
   char name1[15];

   while (1 != sscanf(restbuf, "%s", name1))
   {
      printf("area: Nome");
      if (!lineins("? "))
      {
         return;
      }
   }
   printf("solido %s tem area de %f\n", name1, MSD_highNameArea(name1));
}

real MSD_highNameArea(char *name)
{
   int sn;

   if ((sn = MSD_getSolidIdFromName(name)) == -1)
   {
      fprintf(stderr, MEN_NaoEncontrouSolidNome, NAM_Area, name);
      return(ERROR);
   }
   return(MSD_highArea(sn));
}

#ifdef __Extended_Version
void MSD_execArea(void)
{
   int sn;

   while (1 != sscanf(restbuf, "%d", &sn))
   {
      printf("area: sn");
      if (!lineins("? "))
      {
         return;
      }
   }
   printf("solido %d tem area de %f\n", sn, MSD_highArea(sn));
}

#endif

real MSD_highArea(Id sn)
{
   SPTYPE s;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_Area, sn);
      return(0.0);
   }
   return(MSD_lowSolidArea(s));
}

/*** Loop area evaluation ***/
real MSD_lowLoopArea(LPTYPE l)
{
   HPTYPE he;
   VPTYPE v1;
   vector aa, bb, cc, dd, vv1;
   HPTYPE LoLEdl;

   veczer(dd);
   he = LoLEdl = LooLEdg(l);
   v1 = HalVtx(he);
   he = HalNxt(he);
   do
   {
      veccopy(vv1, VerVCoord(v1));
      vecminus(aa, VerVCoord(HalVtx(he)), vv1);
      vecminus(bb, VerVCoord(HalVtx(HalNxt(he))), vv1);
      cross(cc, aa, bb);
      vecplus(dd, dd, cc);
   } while ((he = HalNxt(he)) != LoLEdl);
   return(-0.5 * dot(FacFeq(LooLFace(l)), dd));
}

real MSD_lowFaceArea(FPTYPE f)
{
   LPTYPE l;
   real   varea;

   for (varea = 0.0, l = FacFLoops(f); l != LNIL; l = LooNextL(l))
   {
      varea += MSD_lowLoopArea(l);
   }
   return(varea);
}

real MSD_lowSolidArea(SPTYPE s)
{
   DPTYPE d;
   FPTYPE f;
   real   varea;

   varea = 0.0;
   for (AllShellsSolid(s, d))
   {
      for (AllFacesShell(d, f))
      {
         varea += MSD_lowFaceArea(f);
      }
   }
   return(varea);
}
