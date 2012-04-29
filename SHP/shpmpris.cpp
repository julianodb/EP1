/* 
  PROJETO USPDesigner
  MODULO: SHP (Criacao de Formas Primitivas)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: shpmpris.cpp
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
#include "mensagem.h"
#include "memvirtu.h"
#include "lowparam.h"
#include "lowsolid.h"
#include "mancommd.h"
#include "shpshape.h"
#include "disdispl.h"
#include "eulerops.h"

void MSD_execNameMakePrism(void)
{
   XY *listvtx;
   char name1[15];
   int n;
   float h;

   while (3 != sscanf(restbuf, "%s %d %f", name1, &n, &h))
   {
      printf("Objeto N_div Altura\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   if ((n > 2) && setvtpi(&listvtx, n))
   {
      MSD_highNameMakePrism(name1, n, h, listvtx);
   }
}

void MSD_highNameMakePrism(char *name, int n, real h, XY *listvtx)
{
   Id sn;

   if (MSD_getSolidIdFromName(name) != -1)
   {
      fprintf(stderr, MEN_SolidoJaExiste, NAM_MakePrism, name);
      return;
   }
   if ((sn = MSD_highMakePrism(n, h, listvtx)) != -1)
   {
      MSD_highName(sn, name, SOLID);
   }
}

#ifdef __Extended_Version
void MSD_execMakePrism(void)
{
   XY *listvtx;
   int n;
   float h;

   while (2 != sscanf(restbuf, "%d %f", &n, &h))
   {
      printf("N_div Altura\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   if ((n > 2) && setvtpi(&listvtx, n))
   {
      printf("Id = %d\n", MSD_highMakePrism(n, h, listvtx));
   }
}

#endif

Id MSD_highMakePrism(int n, real h, XY *listvtx)
{
   SPTYPE optr;
   Id sn;

   sn = MSD_getMaxSolidId();
   if (h != 0.0)
   {
      if ((optr = MSD_lowMakePrism(sn, listvtx, h, n)) != SNIL)
      {
         MSD_lowDisplayMonitor('o', optr, GNIL);
         return(sn);
      }
   }
   if (h == 0.0)
   {
      if ((optr = MSD_lowMakePrism0(sn, listvtx, n, 1)) != SNIL)
      {
         MSD_lowDisplayMonitor('o', optr, GNIL);
         return(sn);
      }
   }
   fprintf(stderr, MEN_ParametrosIncorretos, NAM_MakePrism);
   return(-1);
}

void MSD_execNameMakePyramid(void)
{
   XY *listvtx;
   char name1[15];
   int n;
   float h;

   while (3 != sscanf(restbuf, "%s %d %f", name1, &n, &h))
   {
      printf("Objeto N_div Altura\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   if ((n > 2) && setvtpi(&listvtx, n))
   {
      MSD_highNameMakePyramid(name1, n, h, listvtx);
   }
}

void MSD_highNameMakePyramid(char *name, int n, real h, XY *listvtx)
{
   Id sn;

   if (MSD_getSolidIdFromName(name) != -1)
   {
      fprintf(stderr, MEN_SolidoJaExiste, NAM_MakePyramid, name);
      return;
   }
   if ((sn = MSD_highMakePyramid(n, h, listvtx)) != -1)
   {
      MSD_highName(sn, name, SOLID);
   }
}

#ifdef __Extended_Version
void MSD_execMakePyramid(void)
{
   XY *listvtx;
   int n;
   float h;

   while (2 != sscanf(restbuf, "%d %f", &n, &h))
   {
      printf("N_div Altura\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   if ((n > 2) && setvtpi(&listvtx, n))
   {
      printf("Id = %d\n", MSD_highMakePyramid(n, h, listvtx));
   }
}

#endif

Id MSD_highMakePyramid(int n, real h, XY *listvtx)
{
   SPTYPE optr;
   Id sn;

   sn = MSD_getMaxSolidId();
   if (h != 0.0)
   {
      if ((optr = MSD_lowMakePyramid(sn, listvtx, h, n)) != SNIL)
      {
         MSD_lowDisplayMonitor('o', optr, GNIL);
         return(sn);
      }
   }
   if (h == 0.0)
   {
      if ((optr = MSD_lowMakePrism0(sn, listvtx, n, 1)) != SNIL)
      {
         MSD_lowDisplayMonitor('o', optr, GNIL);
         return(sn);
      }
   }
   fprintf(stderr, MEN_ParametrosIncorretos, NAM_MakePyramid);
   return(-1);
}

char setvtpi(XY **listxy, int n)
{
   char prompt[16];
   float x, y;
   int i;

   if (n > 100)
   {
      return(FALSE);
   }
   *listxy = (XY *)workbuf;
   for (i = 0; i < n; i++)
   {
      sprintf(prompt, "X%d Y%d ? ", i, i);
      do
      {
         if (!lineins(prompt))
         {
            return(FALSE);
         }
      } while (2 != sscanf(restbuf, "%f %f", &x, &y));

      ((*listxy) + i)->x = x;
      ((*listxy) + i)->y = y;
   }
   return(TRUE);
}
