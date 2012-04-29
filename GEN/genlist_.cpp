/* 
  PROJETO USPDesigner
  MODULO: GEN (Funcoes Gerais de Nivel Superior)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: genlists.cpp
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
#include "hiegroup.h"
#include "genfunc_.h"
#include "disdispl.h"
#include "mancommd.h"

void MSD_execListElement(void)
{
   int rp;

   for (rp = 0; ; rp = 2)
   {
      switch (optin())
      {
          case 'a':
          MSD_lowListAllAxisName();
          rp++;
          break;

          case 'c':
          MSD_lowListAllCameraName();
          rp++;
          break;

          case 'o':
          MSD_lowListAllSolidName();
          rp++;
          break;

          case 'f':
          MSD_lowListNumberOfElements();
          rp++;
          break;

          case 'g':
          MSD_lowListAllGroupName();
          rp++;
          break;

          default:
          printf("-acfgo Nome\n");
      }
      if (rp == 1)
      {
         return;
      }
      if (!lineins("? "))
      {
         return;
      }
   }
}

void MSD_lowListAllSolidName(void)
{
   SPTYPE optr;

   for (AllSolids(optr))
   {
      printf("%-20s", SolName(optr));
   }
   printf("\n");
}

void MSD_lowListNumberOfElements(void)
{
   static char pattern[] = "  %-8s     %5d\n";

   printf("Tipo_Elem.  Elem._Utilizados\n");
   printf(pattern, "Camera", camused);
   printf(pattern, "Eixo  ", axsused);
   printf(pattern, "Grupo ", grpused);
   printf(pattern, "Vertex", vtxused);
   printf(pattern, "Edge  ", edgused);
   printf(pattern, "Face  ", facused);
   printf(pattern, "Solido", objused);
}
