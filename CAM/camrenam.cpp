/* 
  PROJETO USPDesigner
  MODULO: CAM (Camera Manipulation)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: camrenam.cpp
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
void MSD_execNameRenameCamera(void)
{
   char name1[15], name2[15];

   while (2 != sscanf(restbuf, "%s %s", name1, name2))
   {
      printf(MEN_PerguntaNovoAntigo, NAM_RenameCamara);
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highNameRenameCamera(name1, name2);
}

#endif

int MSD_highNameRenameCamera(char *name1, char *name2)
{
   CTYPE *cptr;

   if ((cptr = MSD_lowFindCameraByName(name1)) == (CTYPE *)NIL)
   {
      fprintf(stderr, MEN_CamaraNomeNaoEncontrada,
              NAM_RenameCamara, name1);
      return(ERROR);
   }
   if (MSD_lowFindCameraByName(name2) != (CTYPE *)NIL)
   {
      fprintf(stderr, MEN_CamaraJaExiste,
              NAM_RenameCamara, name2);
      return(ERROR);
   }
   return(MSD_lowNameRenameCamera(cptr, name2));
}

int MSD_lowNameRenameCamera(CTYPE *cptr, char *name2)
{
   strcpy(cptr->cname, name2);
   return(SUCCESS);
}
