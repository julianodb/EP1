/* 
  PROJETO USPDesigner
  MODULO: CAM (Camera Manipulation)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: camlist_.cpp
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
#include "mancommd.h"
#include "disdispl.h"

#ifdef __Extended_Version
void MSD_execNameListCameraName(void)
{
   char name1[15];

   while (1 != sscanf(restbuf, "%s", name1))
   {
      printf(MEN_PerguntaNome, NAM_ListCamara);
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highNameListCameraName(name1);
}

#endif

int MSD_highNameListCameraName(char *name)
{
   CTYPE *cptr;

   if ((cptr = MSD_lowFindCameraByName(name)) == (CTYPE *)NIL)
   {
      fprintf(stderr, MEN_CamaraNomeNaoEncontrada,
              NAM_ListCamara, name);
      return(ERROR);
   }
   return(MSD_lowNameListCameraName(cptr));
}

int MSD_lowNameListCameraName(CTYPE *cptr)
{
   static char *strmode[] = { "Paralela", "Axonom. ", "3 vistas" };
   static char *strhid[]  =
   {
      "Todos os edges", "Elim. Local   ",
      "Elim. Completa", "Elim. e Int.  "
   };
   static char *strdisp[] = { "OFF", "ON " };

   printf("Camera  Nome=%-18s", cptr->cname);
   printf("  Modo=%8s", strmode[cptr->mode - 1]);
   printf("  Tipo=%-14s\n", strhid[cptr->hid]);
   printf("        Ampliacao=%10.3g   ", cptr->scale);
   printf("  Linhas Hash =%-3s    ", strdisp[cptr->dash]);
   printf("  Edges Planos=%-3s\n", strdisp[cptr->smooth]);
   printf("        P.V.=(%7.3g,%7.3g,%7.3g)", cptr->eye[0], cptr->eye[1],
          cptr->eye[2]);
   printf("  C.V.=(%7.3g,%7.3g,%7.3g)\n", cptr->ref[0], cptr->ref[1],
          cptr->ref[2]);
   return(TRUE);
}

void MSD_lowListAllCamera(void)
{
   CTYPE *cptr;

   for (AllCamera(cptr))
   {
      MSD_lowNameListCameraName(cptr);
   }
}

void MSD_lowListAllCameraName(void)
{
   CTYPE *cptr;

   for (AllCamera(cptr))
   {
      printf("%-20s", cptr->cname);
   }
   printf("\n");
}
