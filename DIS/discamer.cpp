/* 
  PROJETO USPDesigner
  MODULO: DIS (Hidden Line Elimination)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: discamer.cpp
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
#include "gphgraph.h"
#include "mancommd.h"

void MSD_execCamera(void)
{
   char  name1[15];
   float f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0;
   int   i1 = 0, i2 = 0, i3 = 0, i4 = 0;
   int   ip;
   char  opt;

   for (ip = 0; ip == 0;)
   {
      switch (opt = optin())
      {
          case 'g':
          if (12 == sscanf(restbuf, "%s %f %f %f %f %f %f %f %d %d %d %d",
                           name1, &f1, &f2, &f3, &f4, &f5, &f6, &f7,
                           &i1, &i2, &i3, &i4))
          {
             ip = 1;
          }
          break;

          case 'e':
          if (4 == sscanf(restbuf, "%s %f %f %f", name1, &f1, &f2, &f3))
          {
             ip = 1;
          }
          break;

          case 'r':
          if (4 == sscanf(restbuf, "%s %f %f %f", name1, &f4, &f5, &f6))
          {
             ip = 1;
          }
          break;

          case 's':
          if (2 == sscanf(restbuf, "%s %f", name1, &f7))
          {
             ip = 1;
          }
          break;

          case 'm':
          if (2 == sscanf(restbuf, "%s %d", name1, &i1))
          {
             ip = 1;
          }
          break;

          case 'h':
          if (2 == sscanf(restbuf, "%s %d", name1, &i2))
          {
             ip = 1;
          }
          break;

          case 'd':
          if (2 == sscanf(restbuf, "%s %d", name1, &i3))
          {
             ip = 1;
          }
          break;

          case 'f':
          if (2 == sscanf(restbuf, "%s %d", name1, &i4))
          {
             ip = 1;
          }
          break;
      }
      if (ip == 0)
      {
         printf("-g       Camera Olho Referencia Escala Modo Eliminicao Hashura Planas\n");
         printf("-ersmhdf Camera Olho/Referencia/Escala/Modo/Eliminacao/Hashura/Planas\n");
         if (!lineins("? "))
         {
            return;
         }
      }
   }
   if (opt == 'g')
   {
      MSD_highNameCreateCamera(name1, f1, f2, f3, f4, f5, f6, f7, i1,
                               i2, i3, i4);
   }
   else
   {
      MSD_highNameAlteraCamera(opt, name1, f1, f2, f3, f4, f5, f6,
                               f7, i1, i2, i3, i4);
   }
}
