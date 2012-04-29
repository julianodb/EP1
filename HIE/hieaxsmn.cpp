/* 
  PROJETO USPDesigner
  MODULO: HIE (Estrutura Hierarquica)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: hieaxsmn.cpp
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
#include "lowmacro.h"
#include "lowsolid.h"
#include "vectorop.h"
#include "hiegroup.h"

Id MSD_getAxisIdFromName(char *name)
{
   APTYPE a;

   for (AllAxis(a))
   {
      if (!strcmp(name, AxsName(a)))
      {
         return(AxsAxisNo(a));
      }
   }
   return((Id) - 1);
}

Id MSD_getMaxAxisId(void)
{
   APTYPE aptr;
   Id     an;

   if (axsary == ANIL)
   {
      return(1);
   }
   an = AxsAxisNo(axsary);
   for (AllAxis(aptr))
   {
      if (AxsAxisNo(aptr) > an)
      {
         an = AxsAxisNo(aptr);
      }
   }
   return(an + 1);
}

int MSD_getNumberOfAxis(void)
{
   APTYPE aptr;
   int    i = 0;

   for (AllAxis(aptr))
   {
      i++;
   }
   return(i);
}

char **MSD_putAxisNameInList(char **list)
{
   APTYPE aptr;
   int    i = 0;

   for (AllAxis(aptr))
   {
      list[i] = AxsName(aptr);
      i++;
   }
   return(list);
}

APTYPE MSD_lowFindAxisByName(char *nam)
{
   APTYPE aptr;

   for (AllAxis(aptr))
   {
      if (!strcmp(nam, AxsName(aptr)))
      {
         return(aptr);
      }
   }
   return(ANIL);
}

APTYPE MSD_getAxis(Id an)
{
   APTYPE aptr;

   for (AllAxis(aptr))
   {
      if (AxsAxisNo(aptr) == an)
      {
         return(aptr);
      }
   }
   return(ANIL);
}
