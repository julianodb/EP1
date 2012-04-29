/* 
  PROJETO USPDesigner
  MODULO: HIE (Estrutura Hierarquica)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: hieaxsdf.cpp
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
#include "genfunc_.h"
#include "vectorop.h"
#include "mancommd.h"
#include "hiegroup.h"

void MSD_execManipulateAxis(void)
{
   int   ip;
   char  onam[30], anam[30];
   float x1, y1, z1, x2, y2, z2, k;

   for (ip = 0; ip == 0;)
   {
      switch (optin())
      {
          case 'a':
          if (2 == sscanf(restbuf, "%s %s", anam, onam))
          {
             MSD_highNameAssociateAxisElement(anam, onam);
             ip = 1;
          }
          break;

          case 'd':
          if (1 == sscanf(restbuf, "%s", anam))
          {
             MSD_highNameDesassociateAxisElement(anam);
             ip = 1;
          }
          break;

          case 'e':
          if (1 == sscanf(restbuf, "%s", anam))
          {
             MSD_highNameRemoveAxis(anam);
             ip = 1;
          }
          break;

          case 'm':
          if (2 == sscanf(restbuf, "%s %f", anam, &k))
          {
             MSD_highNameTransformUsingAxis(anam, k);
             ip = 1;
          }
          break;

          case 'r':
          if (7 == sscanf(restbuf, "%s %f %f %f %f %f %f", anam, &x1,
                          &y1, &z1, &x2, &y2, &z2))
          {
             MSD_highNameCreateAxisRotation(anam, x1, y1, z1, x2, y2, z2);
             ip = 1;
          }
          break;

          case 's':
          if (4 == sscanf(restbuf, "%s %f %f %f", anam, &x1, &y1, &z1))
          {
             MSD_highNameCreateAxisTranslation(anam, x1, y1, z1);
             ip = 1;
          }
      }
      if (ip == 0)
      {
         printf("-rs  nome dos eixos [pto sobre os eixos] vector\n");
         printf("-m   nome dos eixos angulo/numero de vezes\n");
         printf("-ade nome dos eixos [nome do objeto/grupo]\n");
         if (!lineins("? "))
         {
            return;
         }
      }
   }
}
