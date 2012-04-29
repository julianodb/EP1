/* 
  PROJETO USPDesigner
  MODULO: ANA (Modulo de Analise)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: anashow_.c
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
#include "genfunc_.h"
#include "vectorop.h"
#include "eulerops.h"
#include "analise_.h"
#include "mancommd.h"

void MSD_execAnaliseShow(void)
{
   char name1[12];

   while (1 != sscanf(restbuf, "%s", name1))
   {
      printf("show: nome");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_lowAnaliseShow(name1);
}

void MSD_lowAnaliseShow(char *name)
{
   matrix m;
   vector c;
   int    i, j;
   FILE   *fp;

   if (strcmp(name, "*"))
   {
      if ((fp = fopen("Rov1.ent", "w")) == (FILE *)NIL)
      {
         return;
      }
   }
   else
   {
      fp = stdout;
   }

   MSD_lowMatrizInerciaTotal(m);
   for (i = 0; i < 3; i++)
   {
      for (j = 0; j < 3; j++)
      {
         fprintf(fp, "%10.4f   ", m[i][j]);
      }
      fprintf(fp, "\n");
   }
   fprintf(fp, "%f\n", MSD_lowMassaTotal());
   MSD_lowCentroMassaTotal(c);
   fprintf(fp, "%10.4f  %10.4f  %10.4f\n", c[0], c[1], c[2]);
   MSD_lowCentroCarenaTotal(c);
   fprintf(fp, "%10.4f  %10.4f  %10.4f\n", c[0], c[1], c[2]);
   fprintf(fp, "%f\n", MSD_lowVolumeFlutuabilidadeTotal());
   fprintf(fp, "%f\n", MSD_lowVolumeTotal());
   fprintf(fp, "%f\n", densifluido);
   fclose(fp);
}
