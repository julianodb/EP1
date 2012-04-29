/* 
  PROJETO USPDesigner
  MODULO: MAN (Modulo Principal)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: manlinin.cpp
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
#include <ctype.h>
#include "lowparam.h"
#include "mancommd.h"

char optin(void)
{
   char optstr[30];

   if (1 != sscanf(restbuf, "%s", optstr))
   {
      return('\0');
   }
   if (strlen(optstr) != 2)
   {
      return('\0');
   }
   if (optstr[0] != '-')
   {
      return('\0');
   }
   restbuf += 2;
   skipsp(&restbuf);
   return(optstr[1]);
}

char lineins(char *s)
{
   for ( ; ;)
   {
      restbuf = linebuf;
      if (instream == stdin)
      {
         printf("%s", s);
      }
      if (!fgets(linebuf, BUFLENG, instream))
      {
         if (instream != stdin)
         {
            fclose(instream);
         }
         instream = stdin;
         return(FALSE);
      }
      control();
      skipsp(&restbuf);
      if (strlen(restbuf) > 1)
      {
         return(*restbuf != '.');
      }
   }
}

void control(void)
{
   char *p;

   if ((p = strchr(linebuf, '#')) != NULL)
   {
      if ((p == linebuf) || (*(p - 1) != '\\'))
      {
         *p = '\0';
      }
   }
}
