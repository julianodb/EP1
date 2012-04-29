/* 
  PROJETO USPDesigner
  MODULO: ANA (Modulo de Analise)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: anaminrc.cpp
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

void MSD_execNameMatrizInercia(void)
{
   char   name1[12];
   matrix mat;

   while (1 != sscanf(restbuf, "%s", name1))
   {
      printf("matriz inercia: Nome");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highNameMatrizInercia(name1, mat);
   printf("solido %s tem matriz de inercia:\n  %10.4f %10.4f %10.4f\n", name1,
          mat[0][0], mat[0][1], mat[0][2]);
   printf("  %10.4f %10.4f %10.4f\n", mat[1][0], mat[1][1], mat[1][2]);
   printf("  %10.4f %10.4f %10.4f\n", mat[2][0], mat[2][1], mat[2][2]);
}

int MSD_highNameMatrizInercia(char *name, matrix mat)
{
   int sn;

   if ((sn = MSD_getSolidIdFromName(name)) == -1)
   {
      fprintf(stderr, MEN_NaoEncontrouSolidNome, NAM_Inercia, name);
      return(ERROR);
   }
   return(MSD_highMatrizInercia(sn, mat));
}

int MSD_highMatrizInercia(Id sn, matrix mat)
{
   SPTYPE s;
   vector cg;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_Inercia, sn);
      return(ERROR);
   }
   MSD_lowCentroDeGravidade(s, cg);
   return(MSD_lowMatrizInercia(s, mat, cg));
}

/* Calculate signed inerce matrix of a solid */
int MSD_lowMatrizInercia(SPTYPE s, matrix mat, vector cg)
{
   DPTYPE dg;
   FPTYPE f;
   LPTYPE l;
   HPTYPE he1;
   HPTYPE he2;
   vector a, b, c, d;
   double escalar, intx2, inty2, intz2;
   int    i, j;

   matzer(mat);
   intx2 = inty2 = intz2 = 0;

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
               vecminus(a, VerVCoord(HalVtx(he2)), cg);
               vecminus(b, VerVCoord(HalVtx(he1)), cg);
               cross(c, a, b);
               vecminus(d, VerVCoord(HalVtx(HalNxt(he2))), cg);
               escalar = dot(d, c);
               intx2   = escalar * (a[0] * a[0] + b[0] * b[0] + d[0] * d[0] +
                                    a[0] * b[0] + a[0] * d[0] + b[0] * d[0]);
               inty2 = escalar * (a[1] * a[1] + b[1] * b[1] + d[1] * d[1] +
                                  a[1] * b[1] + a[1] * d[1] + b[1] * d[1]);
               intz2 = escalar * (a[2] * a[2] + b[2] * b[2] + d[2] * d[2] +
                                  a[2] * b[2] + a[2] * d[2] + b[2] * d[2]);
               mat[0][0] += inty2 + intz2;
               mat[1][1] += intx2 + intz2;
               mat[2][2] += intx2 + inty2;
               mat[0][1] += escalar *
                            (a[0] * a[1] + b[0] * b[1] + d[0] * d[1] +
                             0.5 * (a[0] * b[1] + a[0] * d[1] + b[0] * a[1] +
                                    b[0] * d[1] + d[0] * a[1] + d[0] * b[1]));
               mat[0][2] += escalar *
                            (a[0] * a[2] + b[0] * b[2] + d[0] * d[2] +
                             0.5 * (a[0] * b[2] + a[0] * d[2] + b[0] * a[2] +
                                    b[0] * d[2] + d[0] * a[2] + d[0] * b[2]));
               mat[1][2] += escalar *
                            (a[1] * a[2] + b[1] * b[2] + d[1] * d[2] +
                             0.5 * (a[1] * b[2] + a[1] * d[2] + b[1] * a[2] +
                                    b[1] * d[2] + d[1] * a[2] + d[1] * b[2]));
            } while ((he2 = HalNxt(he2)) != he1);
         }
      }
   }
   mat[2][1] = mat[1][2];
   mat[1][0] = mat[0][1];
   mat[2][0] = mat[0][2];

   for (i = 0; i < 3; i++)
   {
      for (j = 0; j < 3; j++)
      {
         mat[i][j] *= SolDensRel(s) / 60.0;
         if (i == j)
         {
            mat[i][j] *= -1.0;
         }
      }
   }
   return(SUCCESS);
}
