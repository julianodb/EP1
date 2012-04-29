/* 
  PROJETO USPDesigner
  MODULO: TOP (Operadores de Nivel Intermediario)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: toplglue.cpp
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
#include <stdlib.h>
#include "mensagem.h"
#include "memvirtu.h"
#include "lowparam.h"
#include "lowmacro.h"
#include "lowsolid.h"
#include "eulerops.h"
#include "lowgeome.h"
#include "mancommd.h"

#ifdef __Extended_Version
void MSD_execGlue(void)
{
   int s1, s2;

   while (sscanf(restbuf, "%d %d", &s1, &s2) != 2)
   {
      printf("glue: s1 s2\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highGlue(s1, s2);
}

#endif

int MSD_highGlue(Id sn1, Id sn2)
{
   SPTYPE s1;
   SPTYPE s2;

   if ((s1 = MSD_getSolid(sn1)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_Glue, sn1);
      return(ERROR);
   }
   if ((s2 = MSD_getSolid(sn2)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_Glue, sn2);
      return(ERROR);
   }
   MSD_lowGlue(s1, s2);
   return(SUCCESS);
}

void MSD_lowGlue(SPTYPE s1, SPTYPE s2)
{
   DPTYPE d1;
   DPTYPE d2;
   FPTYPE f1;
   FPTYPE f2;
   FPTYPE f3;
   int    flag1, flag2;

   MSD_lowMerge(s1, s2);
   for (AllShellsSolid(s1, d1))
   {
      for (AllFacesShell(d1, f1))
      {
         d2 = d1;
         f2 = FacNextF(f1);
         for ( ; d2 != DNIL; d2 = SheNextD(d2),
               f2 = d2 != DNIL ? SheSFaces(d2) : FNIL)
         {
            for ( ; f2 != FNIL;)
            {
               if (MSD_lowFacFacOppositeEqs(f1, f2) &&
                   MSD_lowFacFacIdentical(f1, f2))
               {
                  f3 = FacNextF(f2);
                  if (FacFShell(f1) == FacFShell(f2))
                  {
                     MSD_lowKFMRH(f1, f2);
                  }
                  else
                  {
                     MSD_lowKSFMR(f1, f2);
                  }
                  f2 = f3;
                  MSD_lowLoopGlue(f1);
               }
               else
               {
                  f2 = FacNextF(f2);
               }
            }
         }
      }
   }

   for (AllShellsSolid(s1, d1))
   {
      for (f1 = SheSFaces(d1); f1;)
      {
         flag1 = 0;
         for (AllShellsSolid(s1, d2))
         {
            for (f2 = SheSFaces(d2); f2;)
            {
               flag2 = 0;
               if (f2 == f1)
               {
                  f2    = FacNextF(f2);
                  flag2 = 1;
                  continue;
               }
               if (MSD_lowFacFacOppositeEqs(f1, f2))
               {
                  if (MSD_lowFacFacContainment(f1, f2) == 1)
                  {
                     f3    = FacNextF(f2);
                     flag2 = 1;
                     if (FacFShell(f1) == FacFShell(f2))
                     {
                        MSD_lowKFMRH(f1, f2);
                     }
                     else
                     {
                        MSD_lowKSFMR(f1, f2);
                     }
                     f2 = f3;
                  }
                  else
                  if (MSD_lowFacFacContainment(f2, f1) == 1)
                  {
                     f3    = FacNextF(f1);
                     flag1 = 1;
                     if (FacFShell(f1) == FacFShell(f2))
                     {
                        MSD_lowKFMRH(f2, f1);
                     }
                     else
                     {
                        MSD_lowKSFMR(f2, f1);
                     }
                     f1 = f3;
                  }
               }
               if (flag2 == 0)
               {
                  f2 = FacNextF(f2);
               }
            }
         }
         if (flag1 == 0)
         {
            f1 = FacNextF(f1);
         }
      }
   }
}

void MSD_lowFaceGlue(SPTYPE s1, SPTYPE s2, FPTYPE f1, FPTYPE f2)
{
   MSD_lowMerge(s1, s2);
   MSD_lowKSFMR(f1, f2);
   MSD_lowLoopGlue(f1);
}

int MSD_lowLoopGlue(FPTYPE f)
{
   HPTYPE h1;
   HPTYPE h2;
   HPTYPE h1next;

   assert1(LooNextL(FacFLoops(f)) != LNIL);
   assert1(LooNextL(LooNextL(FacFLoops(f))) == LNIL);

   h1 = LooLEdg(FacFLoops(f));
   h2 = LooLEdg(LooNextL(FacFLoops(f)));
   if ((h1 == HalNxt(h1)) && (h1 == HalPrv(h1)))
   {
      return(FALSE);
   }
   while (!MSD_lowVtxVtxContainment(HalVtx(h1), HalVtx(h2)))
   {
      h2 = HalNxt(h2);
   }

   MSD_lowMEKR(h1, h2);
   MSD_lowKEV(HalPrv(h1), HalPrv(h2));
   while (HalNxt(h1) != h2)
   {
      h1next = HalNxt(h1);
      MSD_lowMEF(HalNxt(h1), HalPrv(h1), maxf++);
      assert(HalVtx(HalNxt(h1)) != HalVtx(mate(HalNxt(h1))));
      MSD_lowKEV(HalNxt(h1), mate(HalNxt(h1)));
      MSD_lowKEF(mate(h1), h1);
      h1 = h1next;
   }
   MSD_lowKEF(mate(h1), h1);
   return(SUCCESS);
}
