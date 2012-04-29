/* 
  PROJETO USPDesigner
  MODULO: CAM (Camera Manipulation)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: camtrncr.cpp
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
#ifndef __UNIX_
 #include <malloc.h>
#endif
#include "memvirtu.h"
#include "lowparam.h"
#include "lowmacro.h"
#include "lowsolid.h"
#include "mancommd.h"
#include "disdispl.h"

#ifdef __Extended_Version
void MSD_execTranslateCurrentCamera(void)
{
   float f1, f2, f3;

   while (3 != sscanf(restbuf, "%f %f %f", &f1, &f2, &f3))
   {
      printf("TranslateCurrentCamera: dEx dEy dEz\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_lowTranslateCurrentCamera(f1, f2, f3);
}

#endif

void MSD_lowTranslateCurrentCamera(real dEx, real dEy, real dEz)
{
   CTYPE *cptr;
   real  Ex, Ey, Ez, Rx, Ry, Rz, ampl;
   int   mode, elim, hash, edge;

   cptr = camptr;
   Ex   = cptr->eye[0] + dEx;
   Ey   = cptr->eye[1] + dEy;
   Ez   = cptr->eye[2] + dEz;
   Rx   = cptr->ref[0];
   Ry   = cptr->ref[1];
   Rz   = cptr->ref[2];
   ampl = cptr->scale;
   mode = cptr->mode;
   hash = cptr->dash;
   elim = cptr->hid;
   edge = cptr->smooth;
   MSD_highSetCamera(cptr, Ex, Ey, Ez, Rx, Ry, Rz, ampl, mode, elim, hash, edge);
}
