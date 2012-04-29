/* 
  PROJETO USPDesigner
  MODULO: DIS (Hidden Line Elimination)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: disbreak.cpp
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
#ifndef __UNIX_
 #include <dos.h>
#endif
#include "memvirtu.h"
#include "lowparam.h"
#include "lowsolid.h"
#include "disdispl.h"
#include "mancommd.h"

int breakchk = FALSE;

#if !defined (_Windows)
void MSD_lowSetBreakInterruption(void)
{
 #if !defined (__UNIX_)
   setvect(0x06, MSD_lowBreakFunction);
 #endif
   breakchk = TRUE;
   breakflg = FALSE;
}

#endif

#if !defined (_Windows)
void interrupt far MSD_lowBreakFunction(...)
{
   int c = 'n';

   breakflg = TRUE;
 #if !defined (__UNIX_)
   bdosptr(9, "break (y/n) $", 0);
   c = bdos(1, 0, 0) & 0x00ff;
 #endif
   if ((c == 'n') || (c == 'N'))
   {
      breakflg = FALSE;
   }
   printf("\n");
}

void MSD_lowSetBreakResident(void)
{
   if (!breakchk)
   {
      return;
   }
   breakchk = FALSE;
   if (instream == stdin)
   {
      breakflg = FALSE;
   }
}

#endif
