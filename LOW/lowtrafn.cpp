/* 
  PROJETO USPDesigner
  MODULO: LOW (Operadores Basicos)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: lowtrafn.cpp
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
#include "shpshape.h"
#include "vectorop.h"
#include "mancommd.h"

void MSD_execNameAffine(void)
{
   int   i, ip, ap, bp;
   char  name1[15];
   float a11, a12, a13, a21, a22, a23, a31, a32, a33, xc, yc, zc;

   for (ip = 0; ip == 0;)
   {
      bp = sscanf(restbuf, "%s %f %f %f", name1, &xc, &yc, &zc);
      if (bp == 4)
      {
         for (i = 0; i < 3; ++i)
         {
            if (!lineins("? "))
            {
               return;
            }
            if (i == 0)
            {
               ap = sscanf(restbuf, "%f %f %f", &a11, &a12, &a13);
            }
            if (i == 1)
            {
               ap = sscanf(restbuf, "%f %f %f", &a21, &a22, &a23);
            }
            if (i == 2)
            {
               ap = sscanf(restbuf, "%f %f %f", &a31, &a32, &a33);
            }
            if (ap != 3)
            {
               break;
            }
         }
      }
      if ((bp == 4) && (ap == 3))
      {
         ip = 1;
      }
      if ((bp != 4) || (ap != 3))
      {
         printf("Objeto Centro de Transf.\nMatriz de Transf.\n");
         if (!lineins("? "))
         {
            return;
         }
      }
   }
   MSD_highNameAffine(name1, a11, a12, a13, a21, a22, a23, a31, a32, a33, xc, yc, zc);
}

int MSD_highNameAffine(char *name, real a11, real a12, real a13, \
                       real a21, real a22, real a23,             \
                       real a31, real a32, real a33,             \
                       real xc, real yc, real zc)
{
   int sn;

   if ((sn = MSD_getSolidIdFromName(name)) == -1)
   {
      fprintf(stderr, MEN_NaoEncontrouSolidNome, NAM_AffineSolid, name);
      return(ERROR);
   }
   return(MSD_highAffine(sn, a11, a12, a13, a21, a22, a23, a31, a32,
                         a33, xc, yc, zc));
}

#ifdef __Extended_Version
void MSD_execAffine(void)
{
   int   sn, i, ip, ap, bp;
   float a11, a12, a13, a21, a22, a23, a31, a32, a33, xc, yc, zc;

   for (ip = 0; ip == 0;)
   {
      bp = sscanf(restbuf, "%d %f %f %f", &sn, &xc, &yc, &zc);
      if (bp == 4)
      {
         for (i = 0; i < 3; ++i)
         {
            if (!lineins("? "))
            {
               return;
            }
            if (i == 0)
            {
               ap = sscanf(restbuf, "%f %f %f", &a11, &a12, &a13);
            }
            if (i == 1)
            {
               ap = sscanf(restbuf, "%f %f %f", &a21, &a22, &a23);
            }
            if (i == 2)
            {
               ap = sscanf(restbuf, "%f %f %f", &a31, &a32, &a33);
            }
            if (ap != 3)
            {
               break;
            }
         }
      }
      if ((bp == 4) && (ap == 3))
      {
         ip = 1;
      }
      if ((bp != 4) || (ap != 3))
      {
         printf("Objeto Centro de Transf.\nMatriz de Transf.\n");
         if (!lineins("? "))
         {
            return;
         }
      }
   }
   MSD_highAffine(sn, a11, a12, a13, a21, a22, a23, a31, a32, a33, xc, yc, zc);
}

#endif

int MSD_highAffine(Id sn, real a11, real a12, real a13, \
                   real a21, real a22, real a23,        \
                   real a31, real a32, real a33,        \
                   real xc, real yc, real zc)
{
   SPTYPE s;
   matrix mat;
   vector vec;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_AffineSolid, sn);
      return(ERROR);
   }
   matident(mat);
   mat[0][0] = a11;
   mat[0][1] = a12;
   mat[0][2] = a13;
   mat[1][0] = a21;
   mat[1][1] = a22;
   mat[1][2] = a23;
   mat[2][0] = a31;
   mat[2][1] = a32;
   mat[2][2] = a33;
   makevec(vec, xc, yc, zc, 1.0);
   CommandCount++;
   return(MSD_lowAffine(s, vec, mat));
}

int MSD_lowAffine(SPTYPE sptr, vector center, matrix mat)
{
   matrix m;

   matident(m);
   mattrans(m, -center[0], -center[1], -center[2]);
   matmult(m, m, mat);
   mattrans(m, center[0], center[1], center[2]);
   MSD_lowTransformation(sptr, m);
   return(SUCCESS);
}
