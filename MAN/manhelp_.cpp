/* 
  PROJETO USPDesigner
  MODULO: MAN (Modulo Principal)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: manhelp_.cpp
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
#include "memvirtu.h"
#include "lowparam.h"
#include "lowsolid.h"
#include "mancommd.h"

void help(void)
{
   COMTYPE *p;
   int ip, pn, cont;
   char com[30];

   for (ip = 0; ip == 0;)
   {
      pn = sscanf(restbuf, "%s", com);
      if ((pn == 0) || (pn == EOF))
      {
         p  = (COMTYPE *)NIL;
         ip = 1;
      }
      if (ip == 0)
      {
         strupper((unsigned char *)com);
         for (p = comtable; *p->comname; ++p)
         {
            if (!strncmp(p->comname, com, strlen(com)))
            {
               break;
            }
         }
         if (*p->comname)
         {
            ip = 1;
         }
         else
         {
            printf("Comando %s nao existe!\n", com);
            return;
         }
      }
   }
   if (p)
   {
      helpman(p);
   }
   else
   {
      for (p = comtable, cont = 0; *p->comname; p++, cont++)
      {
         if (cont == 6)
         {
            printf("\n");
            cont = 0;
         }
         printf("%-13s", p->comname);
      }
      printf("\n");
   }
}

void helpman(COMTYPE *p)
{
   FILE *helpfptr;
   char filename[16], c;

   strcpy(filename, p->comname);
   filename[8] = '\0';
   strcat(filename, ".HLP");
   if ((helpfptr = fopen(filename, "r")) == NIL)
   {
      printf("Nao conseguiu abrir o arquivo HELP \"%s\"\n", filename);
      return;
   }
   while ((c = getc(helpfptr)) != EOF)
   {
      putchar(c);
   }
   fclose(helpfptr);
}
