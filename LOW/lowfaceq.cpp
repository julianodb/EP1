/* 
  PROJETO USPDesigner
  MODULO: LOW (Operadores Basicos)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: lowfaceq.cpp
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
#include "lowsolid.h"
#include "vectorop.h"

/*** evaluation of face equations ***/
int MSD_lowFaceEq(LPTYPE l, vector eq)
{
   HPTYPE he;
   double a, b, c, norm;
   double xi, yi, zi, xj, yj, zj, xc, yc, zc;
   int    len;

   if (FacFeq(LooLFace(l)) == eq)
   {
      SetFaceEqOK(LooLFace(l));
   }

   a   = b = c = xc = yc = zc = 0.0;
   len = 0;
   he  = LooLEdg(l);
   do
   {
      xc += VerVCoord(HalVtx(he))[0];
      yc += VerVCoord(HalVtx(he))[1];
      zc += VerVCoord(HalVtx(he))[2];
      len++;
   } while ((he = HalNxt(he)) != LooLEdg(l));

   xc = len != 0 ? xc / len : 0.0;
   yc = len != 0 ? yc / len : 0.0;
   zc = len != 0 ? zc / len : 0.0;

   do
   {
      xi = VerVCoord(HalVtx(he))[0] - xc;
      yi = VerVCoord(HalVtx(he))[1] - yc;
      zi = VerVCoord(HalVtx(he))[2] - zc;
      xj = VerVCoord(HalVtx(HalNxt(he)))[0] - xc;
      yj = VerVCoord(HalVtx(HalNxt(he)))[1] - yc;
      zj = VerVCoord(HalVtx(HalNxt(he)))[2] - zc;
      a += (yi - yj) * (zi + zj);
      b += (zi - zj) * (xi + xj);
      c += (xi - xj) * (yi + yj);
   } while ((he = HalNxt(he)) != LooLEdg(l));

   if (comp(norm = sqrt(a * a + b * b + c * c), 0.0, EPS) != 0)
   {
      eq[0] = a / norm;
      eq[1] = b / norm;
      eq[2] = c / norm;
      eq[3] = -eq[0] * xc - eq[1] * yc - eq[2] * zc;     /************ 01/03/95 */
      return(SUCCESS);
   }
   else
   {
      veczer(eq);
      return(ERROR);
   }
}
