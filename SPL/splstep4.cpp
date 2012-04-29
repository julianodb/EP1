/* 
  PROJETO USPDesigner
  MODULO: SPL (Corte de Solido)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: splstep4.cpp
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
#include "memvirtu.h"
#include "lowparam.h"
#include "lowmacro.h"
#include "lowsolid.h"
#include "eulerops.h"
#include "lowgeome.h"
#include "vectorop.h"
#include "shpshape.h"
#include "splsplit.h"

void MSD_lowSplitFinish(SPTYPE s, Id sn1, Id sn2, SPTYPE *Above, SPTYPE *Below)
{
   int    i, j, cont;
   Id     maxd;
   FILE   *fv;
   HPTYPE he;

#ifdef TRACE_SPLIT
   fprintf(trace, "\n\n\n");
   fprintf(trace, "*************************************************\n");
   fprintf(trace, "*                                               *\n");
   fprintf(trace, "*                 SETSPLITFINISH                *\n");
   fprintf(trace, "*                                               *\n");
   fprintf(trace, "*************************************************\n");
   fprintf(trace, "\n");
#endif

   maxd = MSD_getMaxShellId(s);

   for (i = 0; i < spl_nfac; ++i)
   {
      for (j = 0; j < spl_contshell; j++)
      {
         if (SplShellShell(j) == FacFShell(SplSonf(i)))
         {
            SplShellSign(j) = -2;
         }
      }
      SplSonf(spl_nfac + i) = MSD_lowMSFKR(InnerLoop(SplSonf(i)), maxf++, maxd++);
   }

#ifdef TRACE_SPLIT
   for (i = 0; i < 2 * spl_nfac; i++)
   {
      trace_f(SplSonf(i));
   }
#endif

   for (i = 0; i < spl_nfac; ++i)
   {
      *Above = MSD_lowDetach(s, sn1, FacFShell(SplSonf(i)), -1, 0, 0);
      *Below = MSD_lowDetach(s, sn2, FacFShell(SplSonf(spl_nfac + i)), -1, 0, 0);
   }

   if (OutputPlanoCorte)
   {
      if ((fv = fopen(OutputFile, "w")) != (FILE *)NIL)
      {
         fprintf(fv, "numero de lacos: %d\n", spl_nfac);
         for (i = 0; i < spl_nfac; ++i)
         {
            he   = LooLEdg(FacFLOut(SplSonf(i)));
            cont = 0;
            do
            {
               cont++;
            } while ((he = HalNxt(he)) != LooLEdg(FacFLOut(SplSonf(i))));
            fprintf(fv, "face: %d \n", FacFaceNo(SplSonf(i)));
            fprintf(fv, "numero_de_vertices: %d\n", cont);
            he = LooLEdg(FacFLOut(SplSonf(i)));
            do
            {
               fprintf(fv, "vertice: %d %10.4f %10.4f %10.4f\n", VerVertexNo(HalVtx(he)),
                       VerVCoord(HalVtx(he))[0],
                       VerVCoord(HalVtx(he))[1],
                       VerVCoord(HalVtx(he))[2]);
            } while ((he = HalNxt(he)) != LooLEdg(FacFLOut(SplSonf(i))));
         }
         fclose(fv);
      }
   }

   for (i = 0; i < spl_contshell; i++)
   {
      if (SplShellSign(i) != -2)
      {
         if (SplShellSign(i) == 1)
         {
            *Below = MSD_lowDetach(s, sn2, SplShellShell(i), -1, 0, 0);
         }
         else
         {
            *Above = MSD_lowDetach(s, sn1, SplShellShell(i), -1, 0, 0);
         }
      }
   }
}
