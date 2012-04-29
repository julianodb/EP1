/* 
  PROJETO USPDesigner
  MODULO: GEN (Funcoes Gerais de Nivel Superior)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: genwait_.cpp
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
#include <time.h>
#include "memvirtu.h"
#include "genfunc_.h"
#include "mancommd.h"

void MSD_execWait(void)
{
   int waitsec;

   while (1 != sscanf(restbuf, "%d", &waitsec))
   {
      printf("Entre numero de segundos.\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highWait(waitsec);
}

void MSD_highWait(int waitsec)
{
   time_t now;

   time(&now);

#ifndef __UNIX_
   while (difftime(time(0), now) < waitsec)
   {
      ;
   }
#endif
}

void MSD_execTime(void)
{
   time_t now;

   time(&now);
#ifdef __UNIX_
   printf("time %s\n", ctime(localtime(now)));
#else
   now = time(NULL);
   printf("time %s\n", ctime(&now));
#endif
}
