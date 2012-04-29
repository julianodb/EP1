/* 
  PROJETO USPDesigner
  MODULO: SET (Operadores Booleanos)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: setsrecs.cpp
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
#include <math.h>
#include "memvirtu.h"
#include "lowparam.h"
#include "lowmacro.h"
#include "lowsolid.h"
#include "vectorop.h"
#include "sethmain.h"

/* Reclassification of "on-"sectors */
void MSD_lowSetOpVVReclassifySectors(int op)
{
   HPTYPE ha;
   HPTYPE hb;
   int    i, j, nonopposite, newsa, newsb;
   int    secta, prevsecta, nextsecta;
   int    sectb, prevsectb, nextsectb;
   vector vha, vhb;

#ifdef TRACE_BOOLEAN22
   static char *ff[] = { "OUT", "ON ", "IN " };
#endif

   for (i = 0; i < nsectors; ++i)
   {
      if ((SetSecs1a(i) == USP_ON) && (SetSecs2a(i) == USP_ON) &&
          (SetSecs1b(i) == USP_ON) && (SetSecs2b(i) == USP_ON))
      {
         secta     = SetSecA(i);
         sectb     = SetSecB(i);
         prevsecta = (secta == 0) ? nnba - 1 : secta - 1;
         prevsectb = (sectb == 0) ? nnbb - 1 : sectb - 1;
         nextsecta = (secta == nnba - 1) ? 0 : secta + 1;
         nextsectb = (sectb == nnbb - 1) ? 0 : sectb + 1;
         ha        = SetANBHe(secta);
         hb        = SetBNBHe(sectb);

#ifdef TRACE_BOOLEAN22
         fprintf(trace, "\nPASSO 1 ***\n");
         fprintf(trace, "par numero %2d  - sec.s1a == ON && sec.s1b == ON\n", i);
         fprintf(trace, "                 sec.s1b == ON && sec.s2b == ON\n");
         fprintf(trace, "               - secta     = %2d\n", secta);
         fprintf(trace, "               - sectb     = %2d\n", sectb);
         fprintf(trace, "               - prevsecta = %2d\n", prevsecta);
         fprintf(trace, "               - prevsectb = %2d\n", prevsectb);
         fprintf(trace, "               - nextsecta = %2d\n", nextsecta);
         fprintf(trace, "               - nextsectb = %2d\n", nextsectb);
#endif

         veccopy(vha, FacFeq(LooLFace(HalWLoop(ha))));
         veccopy(vhb, FacFeq(LooLFace(HalWLoop(hb))));
         nonopposite = vecequal(vha, vhb);
         if (nonopposite)
         {
#ifdef TRACE_BOOLEAN22
            fprintf(trace, "               - faces nao opostas!\n");
#endif

            newsa = (op == USP_UNION) ? USP_OUT : USP_IN;
            newsb = (op == USP_UNION) ? USP_IN  : USP_OUT;
         }
         else
         {
#ifdef TRACE_BOOLEAN22
            fprintf(trace, "               - faces opostas!\n");
#endif

            newsa = (op == USP_UNION) ? USP_IN : USP_OUT;
            newsb = (op == USP_UNION) ? USP_IN : USP_OUT;
         }

         for (j = 0; j < nsectors; ++j)
         {
#ifdef TRACE_BOOLEAN22
            fprintf(trace, "*** j = %2d\n", j);
#endif
            if ((SetSecA(j) == prevsecta) && (SetSecB(j) == sectb))
            {
               if (SetSecs1a(j) != USP_ON)
               {
#ifdef TRACE_BOOLEAN22
                  fprintf(trace, "*** sec.secta == prevsecta && sec.sectb == sectb\n");
                  fprintf(trace, "--> sec[%2d].s2a = %4s\n", j, ff[newsa + 1]);
#endif
                  SetSecs2a(j) = newsa;
               }
            }

            if ((SetSecA(j) == nextsecta) && (SetSecB(j) == sectb))
            {
               if (SetSecs2a(j) != USP_ON)
               {
#ifdef TRACE_BOOLEAN22
                  fprintf(trace, "*** sec.nextsecta == prevsecta && sec.sectb == sectb\n");
                  fprintf(trace, "--> sec[%2d].s1a = %4s\n", j, ff[newsa + 1]);
#endif
                  SetSecs1a(j) = newsa;
               }
            }

            if ((SetSecA(j) == secta) && (SetSecB(j) == prevsectb))
            {
               if (SetSecs1b(j) != USP_ON)
               {
#ifdef TRACE_BOOLEAN22
                  fprintf(trace, "*** sec.secta == secta && sec.sectb == prevsectb\n");
                  fprintf(trace, "--> sec[%2d].s2b = %4s\n", j, ff[newsb + 1]);
#endif
                  SetSecs2b(j) = newsb;
               }
            }
            if ((SetSecA(j) == secta) && (SetSecB(j) == nextsectb))
            {
               if (SetSecs2b(j) != USP_ON)
               {
#ifdef TRACE_BOOLEAN22
                  fprintf(trace, "*** sec.secta == secta && sec.sectb == nextsectb\n");
                  fprintf(trace, "--> sec[%2d].s1b = %4s\n", j, ff[newsb + 1]);
#endif
                  SetSecs1b(j) = newsb;
               }
            }
            if ((SetSecs1a(j) == SetSecs2a(j)) &&
                ((SetSecs1a(j) == USP_IN) || (SetSecs1a(j) == USP_OUT)))
            {
#ifdef TRACE_BOOLEAN22
               fprintf(trace, "*** sec.s1a == sec.s2a != ON\n");
               fprintf(trace, "--> sec[%2d].intersect = 0\n", j);
#endif
               SetSecInter(j) = 0;
            }
            if ((SetSecs1b(j) == SetSecs2b(j)) &&
                ((SetSecs1b(j) == USP_IN) || (SetSecs1b(j) == USP_OUT)))
            {
#ifdef TRACE_BOOLEAN22
               fprintf(trace, "*** sec.s1b == sec.s2b != ON\n");
               fprintf(trace, "--> sec[%2d].intersect = 0\n", j);
#endif
               SetSecInter(j) = 0;
            }
         }
#ifdef TRACE_BOOLEAN22
         fprintf(trace, "--> sec[%2d].s1a = sec[%2d].s2a = %4s\n", i, i, ff[newsa + 1]);
         fprintf(trace, "--> sec[%2d].s1b = sec[%2d].s2b = %4s\n", i, i, ff[newsb + 1]);
         fprintf(trace, "--> sec[%2d].intersect = 0\n", i);
#endif
         SetSecs1a(i)   = SetSecs2a(i) = newsa;
         SetSecs1b(i)   = SetSecs2b(i) = newsb;
         SetSecInter(i) = 0;
      }
   }
}
