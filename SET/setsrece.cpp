/* 
  PROJETO USPDesigner
  MODULO: SET (Operadores Booleanos)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: setsrece.cpp
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
#include "sethmain.h"

#define antigo

void MSD_lowSetOpVVReclassifyEdges(int op)
{
   int i, j, k1, k2, k3, newsa, newsb;
   int secta, prevsecta;
   int sectb, prevsectb;
   int s1a, s1b;

#ifdef TRACE_BOOLEAN22
   static char *ff[] = { "OUT", "ON ", "IN " };
#endif

   for (i = 0; i < nsectors; i++)
   {
      /* --- verifica se a dupla de setores -i- se intercepta e a aresta de A e B estao ON --- */
      if (SetSecInter(i) && (SetSecs1a(i) == USP_ON) && (SetSecs1b(i) == USP_ON))
      {
         secta     = SetSecA(i);
         sectb     = SetSecB(i);
         prevsecta = (secta == 0) ? nnba - 1 : secta - 1;
         prevsectb = (sectb == 0) ? nnbb - 1 : sectb - 1;

         /* --- determina os setores anteriores e posteriores ao setor em estudo --- */
         /* --- a classificacao da diferenca foi acertada para o caso do livro do Mantyla --- */
         newsa = (op == USP_UNION) ? USP_OUT : (op == USP_INTERSECT ? USP_IN : USP_IN);
         newsb = (op == USP_UNION) ? USP_IN : (op == USP_INTERSECT ? USP_OUT : USP_OUT);

         if (op == USP_DIFFERENCE)
         {
            for (k1 = 0; k1 < nsectors; k1++)
            {
               if ((SetSecA(k1) == secta) && (SetSecB(k1) == prevsectb))
               {
                  break;
               }
            }
            for (k2 = 0; k2 < nsectors; k2++)
            {
               if ((SetSecA(k2) == prevsecta) && (SetSecB(k2) == sectb))
               {
                  break;
               }
            }
            for (k3 = 0; k3 < nsectors; k3++)
            {
               if ((SetSecA(k3) == prevsecta) && (SetSecB(k3) == prevsectb))
               {
                  break;
               }
            }

            if ((SetSecs2b(i) == SetSecs1b(k1)) && (SetSecs2b(i) == SetSecs2b(k2)) &&
                (SetSecs2b(i) == SetSecs1b(k3)))
            {
               // fprintf(stderr, "!");
               newsb = SetSecs2b(i);
            }
            if ((SetSecs2a(i) == SetSecs2a(k1)) && (SetSecs2a(i) == SetSecs1a(k2)) &&
                (SetSecs2a(i) == SetSecs1a(k3)))
            {
               // fprintf(stderr, "#");
               newsa = -SetSecs2a(i);
            }
         }

         /* com esta classificacao para a diferenca o arquivo Itano1.cad funciona IN solido b !! */


#ifdef TRACE_BOOLEAN22
         fprintf(trace, "\nPASSO 1 ***\n");
         fprintf(trace, "par numero %2d  - sec.s1a == ON && sec.s1b == ON\n", i);
         fprintf(trace, "               - secta     = %2d --> %4s\n", secta, ff[SetSecs1a(i) + 1]);
         fprintf(trace, "               - sectb     = %2d --> %4s\n", sectb, ff[SetSecs1b(i) + 1]);
         fprintf(trace, "               - prevsecta = %2d --> %4s\n", prevsecta, ff[SetSecs2a(i) + 1]);
         fprintf(trace, "               - prevsectb = %2d --> %4s\n", prevsectb, ff[SetSecs2b(i) + 1]);
#endif

         for (j = 0; j < nsectors; j++)
         {
            /* --- verifica se a dupla de setores -j- se intercepta --- */
            if (SetSecInter(j))
            {
#ifdef TRACE_BOOLEAN22
               fprintf(trace, "*** j = %2d\n", j);
               fprintf(trace, "               - secta     = %2d --> %4s\n", SetSecA(j), ff[SetSecs1a(j) + 1]);
               fprintf(trace, "               - sectb     = %2d --> %4s\n", SetSecB(j), ff[SetSecs1b(j) + 1]);
               fprintf(trace, "               - prevsecta = %2d --> %4s\n", (SetSecA(j) == 0) ? nnba - 1 : SetSecA(j) - 1, ff[SetSecs2a(j) + 1]);
               fprintf(trace, "               - prevsectb = %2d --> %4s\n", (SetSecB(j) == 0) ? nnbb - 1 : SetSecB(j) - 1, ff[SetSecs2b(j) + 1]);
#endif

               /* ---   o setor A da dupla -i- e equivalente ao setor A da dupla -j-   --- */
               /* --- e o setor B da dupla -i- e equivalente ao setor B da dupla -j- ? --- */
               /* --- Entao neste caso temos duas duplas equivalentes??                --- */
               if ((SetSecA(j) == secta) && (SetSecB(j) == sectb))
               {
#ifdef TRACE_BOOLEAN22
                  fprintf(trace, "*** sec.secta == secta && sec.sectb == sectb\n");
                  fprintf(trace, "--> sec[%2d].s1a = %4s\n", j, ff[newsa + 1]);
                  fprintf(trace, "--> sec[%2d].s1b = %4s\n", j, ff[newsb + 1]);
#endif
                  SetSecs1a(j) = newsa;
                  SetSecs1b(j) = newsb;
               }

               /* --- o setor A da dupla -j- e equivalente ao anterior A da dupla -i-  --- */
               /* --- e o setor B da dupla -i- e equivalente ao setor B da dupla -j- ? --- */
               if ((SetSecA(j) == prevsecta) && (SetSecB(j) == sectb))
               {
#ifdef TRACE_BOOLEAN22
                  fprintf(trace, "**** sec.secta == prevsecta && sec.sectb == sectb\n");
                  fprintf(trace, "--> sec[%2d].s2a = %4s\n", j, ff[newsa + 1]);
                  fprintf(trace, "--> sec[%2d].s1b = %4s\n", j, ff[newsb + 1]);
#endif
                  SetSecs2a(j) = newsa;
                  SetSecs1b(j) = newsb;
               }

               if ((SetSecA(j) == secta) && (SetSecB(j) == prevsectb))
               {
#ifdef TRACE_BOOLEAN22
                  fprintf(trace, "*** sec.secta == secta && sec.sectb == prevsectb\n");
                  fprintf(trace, "--> sec[%2d].s1a = %4s\n", j, ff[newsa + 1]);
                  fprintf(trace, "--> sec[%2d].s2b = %4s\n", j, ff[newsb + 1]);
#endif
                  SetSecs1a(j) = newsa;
                  SetSecs2b(j) = newsb;
               }

               if ((SetSecA(j) == prevsecta) && (SetSecB(j) == prevsectb))
               {
#ifdef TRACE_BOOLEAN22
                  fprintf(trace, "*** sec.secta == prevsecta && sec.sectb == prevsectb\n");
                  fprintf(trace, "--> sec[%2d].s2a = %4s\n", j, ff[newsa + 1]);
                  fprintf(trace, "--> sec[%2d].s2b = %4s\n", j, ff[newsb + 1]);
#endif
                  SetSecs2a(j) = newsa;
                  SetSecs2b(j) = newsb;
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
         }
      }
   }

   for (i = 0; i < nsectors; i++)
   {
      s1a = SetSecs1a(i);
      s1b = SetSecs1b(i);

      /* --- verifica se a dupla de setores -i- se intercepta e a aresta de A esta ON --- */
      if (SetSecInter(i) && (s1a == USP_ON))
      {
         secta = SetSecA(i);
         sectb = SetSecB(i);

         /* --- determina os setores anteriores e posteriores ao setor em estudo --- */
         prevsecta = (secta == 0) ? nnba - 1 : secta - 1;
         prevsectb = (sectb == 0) ? nnbb - 1 : sectb - 1;

#ifndef antigo
         nextsecta = (secta == nnba - 1) ? 0 : secta + 1;
         if (SetANBHe(nextsecta) == SetANBHe(secta))
         {
            nextsecta = (nextsecta == nnba - 1) ? 0 : nextsecta + 1;
         }
         nextsectb = (sectb == nnbb - 1) ? 0 : sectb + 1;
         if (SetBNBHe(nextsectb) == SetBNBHe(sectb))
         {
            nextsectb = (nextsectb == nnbb - 1) ? 0 : nextsectb + 1;
         }
#endif

         newsa = op == USP_UNION ? USP_OUT : op == USP_DIFFERENCE ? USP_OUT : USP_IN;

#ifdef TRACE_BOOLEAN22
         fprintf(trace, "\nPASSO 2 ***\n");
         fprintf(trace, "par numero %2d  - sec.s1a == ON\n", i);
         fprintf(trace, "               - secta     = %2d --> %4s\n", secta, ff[SetSecs1a(i) + 1]);
         fprintf(trace, "               - sectb     = %2d --> %4s\n", sectb, ff[SetSecs1b(i) + 1]);
         fprintf(trace, "               - prevsecta = %2d --> %4s\n", prevsecta, ff[SetSecs2a(i) + 1]);
         fprintf(trace, "               - prevsectb = %2d --> %4s\n", prevsectb, ff[SetSecs2b(i) + 1]);
#endif

         for (j = 0; j < nsectors; j++)
         {
            /* --- verifica se a dupla de setores -j- se intercepta --- */
            if (SetSecInter(j))
            {
#ifdef TRACE_BOOLEAN22
               fprintf(trace, "*** j = %2d\n", j);
               fprintf(trace, "               - secta     = %2d --> %4s\n", SetSecA(j), ff[SetSecs1a(j) + 1]);
               fprintf(trace, "               - sectb     = %2d --> %4s\n", SetSecB(j), ff[SetSecs1b(j) + 1]);
               fprintf(trace, "               - prevsecta = %2d --> %4s\n", (SetSecA(j) == 0) ? nnba - 1 : SetSecA(j) - 1, ff[SetSecs2a(j) + 1]);
               fprintf(trace, "               - prevsectb = %2d --> %4s\n", (SetSecB(j) == 0) ? nnbb - 1 : SetSecB(j) - 1, ff[SetSecs2b(j) + 1]);
#endif

#ifdef antigo
               /* ---   o setor A da dupla -i- e equivalente ao setor A da dupla -j-   --- */
               /* --- e o setor B da dupla -i- e equivalente ao setor B da dupla -j- ? --- */
               /* --- Entao neste caso temos duas duplas equivalentes??                --- */
               if ((SetSecA(j) == secta) && (SetSecB(j) == sectb))
               {
 #ifdef TRACE_BOOLEAN22
                  fprintf(trace, "*** sec.secta == secta && sec.sectb == sectb\n");
                  fprintf(trace, "--> sec[%2d].s1a = %4s\n", j, ff[newsa + 1]);
 #endif
                  SetSecs1a(j) = newsa;
               }

               /* --- o setor A da dupla -j- e equivalente ao anterior A da dupla -i-  --- */
               /* --- e o setor B da dupla -i- e equivalente ao setor B da dupla -j- ? --- */
               if ((SetSecA(j) == prevsecta) && (SetSecB(j) == sectb))
               {
 #ifdef TRACE_BOOLEAN22
                  fprintf(trace, "*** sec.secta == prevsecta && sec.sectb == sectb && sec.s1a == ON\n");
                  fprintf(trace, "--> sec[%2d].s1a = %4s\n", i, ff[SetSecs1a(i) + 1]);
                  fprintf(trace, "--> sec[%2d].s2a = %4s\n", j, ff[SetSecs2a(j) + 1]);
 #endif
                  SetSecs2a(j) = newsa;
               }
#else   /* antigo */
               /* ---   o setor A da dupla -j- e equivalente ao setor A da dupla -i-      --- */
               /* --- e o setor B da dupla -j- e equivalente ao anterior B da dupla -j- ? --- */
               /* ---                                              lembrar que s1a == ON. --- */
               if ((SetSecA(j) == secta) && (SetSecB(j) == prevsectb))
               {
                  if (s2a == SetSecs1a(j))
                  {
                     SetSecs1a(i) = (op == USP_UNION) ? newsa : -newsa;
                     SetSecs2a(j) = (op == USP_UNION) ? newsa : -newsa;
                  }
                  else
                  {
                     SetSecs1a(i) = (op == USP_UNION) ? USP_OUT : USP_IN;
                     SetSecs2a(j) = (op == USP_UNION) ? USP_OUT : USP_IN;
                  }
 #ifdef TRACE_BOOLEAN22
                  fprintf(trace, "*** sec.secta == prevsecta && sec.sectb == sectb && sec.s1a == ON\n");
                  fprintf(trace, "--> sec[%2d].s1a = %4s\n", i, ff[SetSecs1a(i) + 1]);
                  fprintf(trace, "--> sec[%2d].s2a = %4s\n", j, ff[SetSecs2a(j) + 1]);
 #endif
               }
               if ((SetSecA(j) == nextsecta) && (SetSecB(j) == sectb) && (s2a == USP_ON))
               {
                  if (s1a == SetSecs2a(j))
                  {
                     SetSecs2a(i) = (op == USP_UNION) ? newsa : -newsa;
                     SetSecs1a(j) = (op == USP_UNION) ? newsa : -newsa;
                  }
                  else
                  {
                     SetSecs2a(i) = (op == USP_UNION) ? USP_OUT : USP_IN;
                     SetSecs1a(j) = (op == USP_UNION) ? USP_OUT : USP_IN;
                  }
 #ifdef TRACE_BOOLEAN22
                  fprintf(trace, "*** sec.secta == nextsecta && sec.sectb == sectb && sec.s2a == ON\n");
                  fprintf(trace, "--> sec[%2d].s2a = %4s\n", i, ff[SetSecs2a(i) + 1]);
                  fprintf(trace, "--> sec[%2d].s1a = %4s\n", j, ff[SetSecs1a(j) + 1]);
 #endif
               }
#endif
               if ((SetSecs1a(j) == SetSecs2a(j)) &&
                   ((SetSecs1a(j) == USP_IN) || (SetSecs1a(j) == USP_OUT)))
               {
#ifdef TRACE_BOOLEAN22
                  fprintf(trace, "*** sec.s1a == sec.s2a != ON\n");
                  fprintf(trace, "--> sec[%2d].intersect == 0\n", j);
#endif
                  SetSecInter(j) = 0;
               }
            }
         }
      }
      else
      if (SetSecInter(i) && s1b == USP_ON)
      {
         secta     = SetSecA(i);
         sectb     = SetSecB(i);
         prevsecta = (secta == 0) ? nnba - 1 : secta - 1;
         prevsectb = (sectb == 0) ? nnbb - 1 : sectb - 1;
         newsb     = (op == USP_UNION) ? USP_IN : (op == USP_DIFFERENCE) ? USP_IN : USP_OUT;

#ifdef TRACE_BOOLEAN22
         fprintf(trace, "\nPASSO 3 ***\n");
         fprintf(trace, "par numero %2d  - sec.s1b == ON\n", i);
         fprintf(trace, "               - secta     = %2d\n", secta);
         fprintf(trace, "               - sectb     = %2d\n", sectb);
         fprintf(trace, "               - prevsecta = %2d\n", prevsecta);
         fprintf(trace, "               - prevsectb = %2d\n", prevsectb);
#endif

         for (j = 0; j < nsectors; j++)
         {
            if (SetSecInter(j))
            {
#ifdef TRACE_BOOLEAN22
               fprintf(trace, "*** j = %2d\n", j);
               fprintf(trace, "               - secta     = %2d\n", SetSecA(j));
               fprintf(trace, "               - sectb     = %2d\n", SetSecB(j));
               fprintf(trace, "               - prevsecta = %2d\n", (SetSecA(j) == 0) ? nnba - 1 : SetSecA(j) - 1);
               fprintf(trace, "               - prevsectb = %2d\n", (SetSecB(j) == 0) ? nnbb - 1 : SetSecB(j) - 1);
#endif
               if ((SetSecA(j) == secta) && (SetSecB(j) == sectb))
               {
#ifdef TRACE_BOOLEAN22
                  fprintf(trace, "*** sec.secta == secta && sec.sectb == sectb\n");
                  fprintf(trace, "--> sec[%2d].s1b = %4s\n", j, ff[newsb + 1]);
#endif
                  SetSecs1b(j) = newsb;
               }

               if ((SetSecA(j) == secta) && (SetSecB(j) == prevsectb))
               {
#ifdef TRACE_BOOLEAN22
                  fprintf(trace, "*** sec.secta == secta && sec.sectb == prevsectb && sec.s1b == ON\n");
                  fprintf(trace, "--> sec[%2d].s1a = %4s\n", i, ff[SetSecs1b(i) + 1]);
                  fprintf(trace, "--> sec[%2d].s2a = %4s\n", j, ff[SetSecs2b(j) + 1]);
#endif
                  SetSecs2b(j) = newsb;
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
         }
      }
   }
}
