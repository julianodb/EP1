/* 
  PROJETO USPDesigner
  MODULO: HIE (Estrutura Hierarquica)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: hieaxsli.cpp
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
#include "vectorop.h"
#include "mancommd.h"
#include "hiegroup.h"

#ifdef __Extended_Version
void MSD_execNameListAxisName(void)
{
   char aname[30];

   while (1 != sscanf(restbuf, "%s", aname))
   {
      printf("ListAxisName: name\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highNameListAxisName(aname);
}

#endif

int MSD_highNameListAxisName(char *name)
{
   APTYPE aptr;

   if ((aptr = MSD_lowFindAxisByName(name)) == ANIL)
   {
      fprintf(stderr, MEN_NaoEncontrouAxisNome, NAM_ListAxis, name);
      return(ERROR);
   }
   return(MSD_lowListAxisName(aptr));
}

int MSD_lowListAxisName(APTYPE aptr)
{
   static char *strmode[] = { "\0", "Translacao", "Rotacao    " };
   int         modeidx;

   printf("Eixo  Nome=%-18s  ", AxsName(aptr));
   switch (AxsMode(aptr))
   {
       case AxisOfTranslation:
       modeidx = 1;
       break;

       case AxisOfRotation:
       modeidx = 2;
       break;

       default:
       modeidx = 0;
   }
   printf("mode=%-10s  ", strmode[modeidx]);
   if (AxsType(aptr) == -1)
   {
      printf("Nao possui grupo/objeto adjunto!\n");
   }
   if (AxsType(aptr) == SOLID)
   {
      printf("Grupo adjunto =%s\n", SolName(AxsASolid(aptr)));
   }
   if (AxsType(aptr) == GROUP)
   {
      printf("Grupo adjunto =%s\n", GrpName(AxsAGroup(aptr)));
   }
   if (AxsMode(aptr) == AxisOfRotation)
   {
      printf("\tcenter=(%10.3g,%10.3g,%10.3g)\n", AxsCenter(aptr)[0],
             AxsCenter(aptr)[1],
             AxsCenter(aptr)[2]);
   }
   printf("\tvector=(%10.3g,%10.3g,%10.3g)\n", AxsVector(aptr)[0],
          AxsVector(aptr)[1],
          AxsVector(aptr)[2]);
   return(TRUE);
}

void MSD_lowListAllAxis(void)
{
   APTYPE aptr;

   for (AllAxis(aptr))
   {
      MSD_lowListAxisName(aptr);
   }
}

void MSD_lowListAllAxisName(void)
{
   APTYPE aptr;

   for (AllAxis(aptr))
   {
      printf("%-20s", AxsName(aptr));
   }
   printf("\n");
}
