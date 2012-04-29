/* 
  PROJETO USPDesigner
  MODULO: GEN (Funcoes Gerais de Nivel Superior)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: genrotat.cpp
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
#include "lowmacro.h"
#include "lowsolid.h"
#include "hiegroup.h"
#include "genfunc_.h"
#include "disdispl.h"
#include "mancommd.h"

void MSD_execNameRotateElement(void)
{
   char  name1[15];
   float th, x1, y1, z1, x2, y2, z2;

   if (8 != sscanf(restbuf, "%s %f %f %f %f %f %f %f", name1,
                   &x1, &y1, &z1, &x2, &y2, &z2, &th))
   {
      printf("nome Pto_axis Vector Angulo\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highNameRotateElement(name1, x1, y1, z1, x2, y2, z2, th);
}

int MSD_highNameRotateElement(char *name, real x1, real y1, real z1,
                              real x2, real y2, real z2, real th)
{
   switch (MSD_getTypeByName(name))
   {
       case 'o':
       return(MSD_highNameRotateSolidAxis(name, x1, y1, z1, x2, y2, z2, th));

       case 'g':
       return(MSD_highNameRotateGroup(name, x1, y1, z1, x2, y2, z2, th));

       case 'a':
       return(MSD_highNameRotateAxis(name, x1, y1, z1, x2, y2, z2, th));
   }
   return(FALSE);
}
