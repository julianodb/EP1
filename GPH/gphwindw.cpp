/* 
  PROJETO USPDesigner
  MODULO: GPH (Interface Grafica)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: gphwindw.cpp
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
#include "lowparam.h"
#include "mancommd.h"
#include "gphgraph.h"

void MSD_execWindow(void)
{
   float x, y, r;

   while (3 != sscanf(restbuf, "%f %f %f", &x, &y, &r))
   {
      printf("X Y  Propor.\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highWindow(x, y, r);
}

int MSD_highWindow(real x, real y, real r)
{
   if (!MSD_lowCheckWindow(x, y, r))
   {
      fprintf(stderr, "window: Parametros incorretos!\n");
      return(ERROR);
   }
   dwindbox[0] = windbox[0] = x;
   dwindbox[1] = windbox[1] = y;
   dwindbox[2] = windbox[2] = r;
   return(SUCCESS);
}

char MSD_lowCheckWindow(real x, real y, real r)
{
   if ((x < 0.0) || (x > 1.0))
   {
      return(FALSE);
   }
   if ((y < 0.0) || (y > 1.0))
   {
      return(FALSE);
   }
   if (r < releps)
   {
      return(FALSE);
   }
   if ((x + r > 1.0 + releps) || (y + r > 1.0 + releps))
   {
      return(FALSE);
   }
   return(TRUE);
}
