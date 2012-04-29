/* 
  PROJETO USPDesigner
  MODULO: CAM (Camera Manipulation)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: camnew__.cpp
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
#include "mensagem.h"
#include "memvirtu.h"
#include "lowparam.h"
#include "lowmacro.h"
#include "lowsolid.h"
#include "mancommd.h"
#include "disdispl.h"

#ifdef __Extended_Version
void MSD_execNameCreateCamera(void)
{
   char  name1[15];
   float f1, f2, f3, f4, f5, f6, f7;
   int   i1, i2, i3, i4;

   while (12 != sscanf(restbuf, "%s %f %f %f %f %f %f %f %d %d %d %d",
                       name1, &f1, &f2, &f3, &f4, &f5, &f6, &f7,
                       &i1, &i2, &i3, &i4))
   {
      printf(MEN_CamaraParametros, NAM_CreateCamara, "nome");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highNameCreateCamera(name1, f1, f2, f3, f4, f5, f6, f7, i1, i2, i3, i4);
}

#endif

int MSD_highNameCreateCamera(char *name, real Ex, real Ey, real Ez,
                             real Rx, real Ry, real Rz, real scale,
                             int mode, int hidtype, int dash, int smooth)
{
   CTYPE *cptr;
   int   cn;

   if (MSD_getCameraIdFromName(name) != -1)
   {
      fprintf(stderr, MEN_CamaraJaExiste,
              NAM_CreateCamara, name);
      return(ERROR);
   }
   if ((cn = MSD_highCreateCamera(Ex, Ey, Ez, Rx, Ry, Rz, scale, mode,
                                  hidtype, dash, smooth)) != -1)
   {
      cptr = MSD_getCamera(cn);
      strcpy(cptr->cname, name);
      return(SUCCESS);
   }
   return(ERROR);
}

#ifdef __Extended_Version
void MSD_execCreateCamera(void)
{
   float f1, f2, f3, f4, f5, f6, f7;
   int   i1, i2, i3, i4;

   while (11 != sscanf(restbuf, "%f %f %f %f %f %f %f %d %d %d %d",
                       &f1, &f2, &f3, &f4, &f5, &f6, &f7, &i1, &i2, &i3, &i4))
   {
      printf(MEN_CamaraParametros, NAM_CreateCamara, "");
      if (!lineins("? "))
      {
         return;
      }
   }
   printf("Id = %d\n", MSD_highCreateCamera(f1, f2, f3, f4, f5, f6, f7, i1,
                                            i2, i3, i4));
}

#endif

int MSD_highCreateCamera(real Ex, real Ey, real Ez,
                         real Rx, real Ry, real Rz, real scale,
                         int mode, int hidtype, int dash, int smooth)
{
   Id cn;

   cn = MSD_getMaxCameraId();
   return(MSD_lowCreateCamera(cn, Ex, Ey, Ez, Rx, Ry, Rz, scale, mode,
                              hidtype, dash, smooth) == (CTYPE *)NIL ? -1 : cn);
}

int MSD_middleCreateCamera(Id cn, real Ex, real Ey, real Ez, real Rx,
                           real Ry, real Rz, real scale, int mode,
                           int hidtype, int dash, int smooth)
{
   return(MSD_lowCreateCamera(cn, Ex, Ey, Ez, Rx, Ry, Rz, scale, mode,
                              hidtype, dash, smooth) != (CTYPE *)NIL);
}

CTYPE *MSD_lowCreateCamera(Id cn, real Ex, real Ey, real Ez,
                           real Rx, real Ry, real Rz, real scale,
                           int mode, int hidtype, int dash, int smooth)
{
   CTYPE *cptr;

   if ((cptr = (CTYPE *)malloc(sizeof(CTYPE))) != (CTYPE *)NIL)
   {
      ++camused;
      cptr->nxt = camary;
      cptr->cn  = cn;
      if (!MSD_highCheckCamera(Ex, Ey, Ez, Rx, Ry, Rz, scale, mode, hidtype,
                               dash, smooth))
      {
         fprintf(stderr, MEN_CamaraParametroIncorreto,
                 NAM_CreateCamara);
         return((CTYPE *)NIL);
      }
      MSD_highSetCamera(cptr, Ex, Ey, Ez, Rx, Ry, Rz, scale, mode, hidtype,
                        dash, smooth);
      return(camary = cptr);
   }
   return((CTYPE *)NIL);
}
