/* 
  PROJETO USPDesigner
  MODULO: CAM (Camera Manipulation)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: camsetcr.cpp
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
void MSD_execNameSetCurrentCamera(void)
{
   char name1[15];

   while (1 != sscanf(restbuf, "%s", name1))
   {
      printf(MEN_PerguntaNome, NAM_SetCamaraCorrente);
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highNameSetCurrentCamera(name1);
}

#endif

int MSD_highNameSetCurrentCamera(char *name)
{
   CTYPE *cptr;

   if ((cptr = MSD_lowFindCameraByName(name)) == (CTYPE *)NIL)
   {
      fprintf(stderr, MEN_CamaraNomeNaoEncontrada,
              NAM_SetCamaraCorrente, name);
      return(FALSE);
   }
   return(MSD_lowSetCurrentCamera(cptr));
}

#ifdef __Extended_Version
void MSD_execSetCurrentCamera(void)
{
   int i1;

   while (1 != sscanf(restbuf, "%d", &i1))
   {
      printf(MEN_PerguntaId, NAM_SetCamaraCorrente);
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highSetCurrentCamera(i1);
}

#endif

int MSD_highSetCurrentCamera(Id cn)
{
   CTYPE *cptr;

   if ((cptr = MSD_getCamera(cn)) == (CTYPE *)NIL)
   {
      fprintf(stderr, MEN_CamaraIdNaoEncontrada,
              NAM_SetCamaraCorrente, cn);
      return(FALSE);
   }
   return(MSD_lowSetCurrentCamera(cptr));
}

int MSD_lowSetCurrentCamera(CTYPE *cptr)
{
   camptr = cptr;
   return(TRUE);
}
