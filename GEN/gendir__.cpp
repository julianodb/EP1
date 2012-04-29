/* 
  PROJETO USPDesigner
  MODULO: GEN (Funcoes Gerais de Nivel Superior)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: gendir__.cpp
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
#if !defined (__UNIX_) && !defined (_Windows)
 #include <dir.h>
#endif
#include "memvirtu.h"
#include "lowparam.h"
#include "lowsolid.h"
#include "shpshape.h"
#include "genfunc_.h"
#include "mancommd.h"

#if !defined (__UNIX_) && !defined (_Windows)

 #define NORMAL    0x16 /* FILE TYPE */

void MSD_execDir(void)
{
   char name[30];

   while (1 != sscanf(restbuf, "%s", name))
   {
      printf("Dir: name\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_lowDir(name);
}

int MSD_lowDir(char *name)
{
   struct ffblk a;
   int          retcode;

   retcode = findfirst(name, &a, NORMAL);
   while (retcode != -1)
   {
      MSD_lowDisplayFiles(a.ff_name);
      retcode = findnext(&a);
   }
   printf("\n");
   return(TRUE);
}

void MSD_lowDisplayFiles(char *nam)
{
   int i;

   for (i = 0; *nam && *nam != '.'; ++i, ++nam)
   {
      putchar(*nam);
   }
   for ( ; i < 9; ++i)
   {
      putchar(' ');
   }
   if (*nam == '.')
   {
      ++nam;
   }
   printf("%-7s", nam);
}

#endif
