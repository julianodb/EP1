/* 
  PROJETO USPDesigner
  MODULO: DIS (Hidden Line Elimination)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: discfile.cpp
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
#include "disdispl.h"
#include "mancommd.h"

void MSD_execDisplayFile(void)
{
   char rw;
   char name1[15];

   for ( ; ; )
   {
      rw = optin();
      if ((rw == 'r') || (rw == 'w'))
      {
         if (1 == sscanf(restbuf, "%s", name1))
         {
            MSD_highDisplayFile(rw, name1);
            return;
         }
      }
      printf("%s: -rw  arquivo\n", NAM_DisplayFile);
      if (!lineins("? "))
      {
         return;
      }
   }
}

int MSD_highDisplayFile(char rw, char *fname)
{
   switch (rw)
   {
       case 'r':
       return(MSD_lowDisplayReadFile(fname));

       case 'w':
       return(MSD_lowDisplayWriteFile(fname));
   }
   return(FALSE);
}

int MSD_lowDisplayReadFile(char *fname)
{
   char namedsp[30];

   if (dspfrptr)
   {
      fclose(dspfrptr);
   }
   if ((dspfrptr = fopen(fname, "r")) == (FILE *)NIL)
   {
      strcpy(namedsp, fname);
      strcat(namedsp, ".DSP");
      if ((dspfrptr = fopen(namedsp, "r")) == (FILE *)NIL)
      {
         fprintf(stderr, MEN_NaoAbriuArquivo,
                 NAM_DisplayFile, fname);
         return(FALSE);
      }
   }

   while (!feof(dspfrptr))
   {
      h_line();
   }
   fclose(dspfrptr);
   dspfrptr = (FILE *)NIL;
   return(TRUE);
}

int MSD_lowDisplayWriteFile(char *fname)
{
   char namedsp[30];

   if (dspfwptr)
   {
      fclose(dspfwptr);
   }

   if ((dspfwptr = fopen(fname, "w")) == (FILE *)NIL)
   {
      strcpy(namedsp, fname);
      strcat(namedsp, ".DSP");
      if ((dspfwptr = fopen(namedsp, "w")) == (FILE *)NIL)
      {
         fprintf(stderr, MEN_NaoAbriuArquivo,
                 NAM_DisplayFile, fname);
         return(FALSE);
      }
   }
   return(TRUE);
}
