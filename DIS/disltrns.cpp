/* 
  PROJETO USPDesigner
  MODULO: DIS (Hidden Line Elimination)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: disltrns.cpp
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
#include "memvirtu.h"
#include "lowparam.h"
#include "lowmacro.h"
#include "lowsolid.h"
#include "vectorop.h"
#include "disdispl.h"

void MSD_lowDisplayTransformation(vector wpos, vector cpos)
{
   vector temp_m;

   vecminus(temp_m, wpos, camptr->eye);
   MSD_lowApplyDisplayTransformation(camptr->drot, temp_m, cpos);
}

void MSD_lowInverseDisplayTransformation(vector cpos, vector wpos)
{
   vecmult(wpos, cpos, camptr->drot);
   vecplus(wpos, camptr->eye, wpos);

/*
 *  w_trans(camptr->drot, cpos, wpos, camptr->eye) ;
 */
}

void MSD_lowApplyDisplayTransformation(matrix mat, vector v, vector res)
{
   int i;

   for (i = 0; i < 3; i++)
   {
      res[i] = mat[i][0] * v[0] + mat[i][1] * v[1] + mat[i][2] * v[2];
   }
   res[3] = 1.0;
}

void MSD_lowExecInverseDisplayTransformation(vect2 s1, vector s2,
                                             vect2 v1, vector v2)
{
   vector w1, w2;
   real   k;

   if (camptr->mode == 1)
   {
      w1[0] = s1[0];
      w1[1] = s1[1];
      w2[0] = s2[0];
      w2[1] = s2[1];
      w1[2] = w2[2] = camptr->scale;
   }
   else
   {
      k     = camptr->cdis / camptr->scale;
      w1[0] = k * s1[0];
      w1[1] = k * s1[1];
      w2[0] = k * s2[0];
      w2[1] = k * s2[1];
      w1[2] = w2[2] = 0.0;
   }
   MSD_lowInverseDisplayTransformation(w1, v1);
   MSD_lowInverseDisplayTransformation(w2, v2);
}

char MSD_lowExecDisplayTransformation(vector v1, vector v2,
                                      vect2 s1, vect2 s2)
{
   vector t1, t2;
   real   d1, d2;

   MSD_lowDisplayTransformation(v1, t1);
   MSD_lowDisplayTransformation(v2, t2);
   if (MSD_lowClip3Dimension(t1, t2) == TRUE)
   {
      if (camptr->mode == 1)
      {
         d1 = t1[2];
         d2 = t2[2];
      }
      else
      {
         d1 = d2 = camptr->cdis;
      }
      s1[0] = t1[0] / d1 * camptr->scale;
      s1[1] = t1[1] / d1 * camptr->scale;
      s2[0] = t2[0] / d2 * camptr->scale;
      s2[1] = t2[1] / d2 * camptr->scale;
      return(TRUE);
   }
   return(FALSE);
}

void MSD_lowDisplayTransformationScale(vector v1, vector v2, vector s1, vector s2)
{
   vector t1, t2;
   real   d1, d2;

   MSD_lowDisplayTransformation(v1, t1);
   MSD_lowDisplayTransformation(v2, t2);
   if (camptr->mode == 1)
   {
      d1 = t1[2];
      d2 = t2[2];
   }
   else
   {
      d1 = d2 = camptr->cdis;
   }
   s1[0] = t1[0] / d1 * camptr->scale;
   s1[1] = t1[1] / d1 * camptr->scale;
   s2[0] = t2[0] / d2 * camptr->scale;
   s2[1] = t2[1] / d2 * camptr->scale;
}

char MSD_lowClipDisplayTransformation(vector vs, vector ve, vector dvs, vector dve)
{
   vector w1, w2, c1, c2;

   veccopy(w1, vs);
   veccopy(w2, ve);
   MSD_lowDisplayTransformation(w1, c1);
   MSD_lowDisplayTransformation(w2, c2);
   if (MSD_lowClip3Dimension(c1, c2) == TRUE)
   {
      MSD_lowInverseDisplayTransformation(c1, dvs);
      MSD_lowInverseDisplayTransformation(c2, dve);
      return(TRUE);
   }
   return(FALSE);
}
