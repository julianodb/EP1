/* 
  PROJETO USPDesigner
  MODULO: CAM (Camera Manipulation)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: cammanip.cpp
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
#include "disdispl.h"

int MSD_getNumberOfCameras(void)
{
   CTYPE *cptr;
   int   i = 0;

   for (AllCamera(cptr))
   {
      i++;
   }
   return(i);
}

char **MSD_putCameraNameInList(char **list)
{
   CTYPE *cptr;
   int   i = 0;

   for (AllCamera(cptr))
   {
      list[i] = cptr->cname;
      i++;
   }
   return(list);
}

CTYPE *MSD_lowFindCameraByName(char *nam)
{
   CTYPE *cptr;

   for (AllCamera(cptr))
   {
      if (!strcmp(nam, cptr->cname))
      {
         return(cptr);
      }
   }
   return((CTYPE *)NIL);
}

CTYPE *MSD_getCamera(Id cn)
{
   CTYPE *cptr;

   for (AllCamera(cptr))
   {
      if (cptr->cn == cn)
      {
         return(cptr);
      }
   }
   return((CTYPE *)NIL);
}

int MSD_getCameraIdFromName(char *cname)
{
   CTYPE *cptr;

   for (AllCamera(cptr))
   {
      if (!strcmp(cname, cptr->cname))
      {
         return(cptr->cn);
      }
   }
   return(-1);
}

int MSD_getMaxCameraId(void)
{
   CTYPE *cptr;
   Id    cn;

   if (camary == (CTYPE *)NIL)
   {
      return(1);
   }
   cn = camary->cn;
   for (AllCamera(cptr))
   {
      if (cptr->cn > cn)
      {
         cn = cptr->cn;
      }
   }
   return(cn + 1);
}
