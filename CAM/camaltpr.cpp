/* 
  PROJETO USPDesigner
  MODULO: CAM (Camera Manipulation)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: camaltpr.cpp
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
#include "mancommd.h"
#include "disdispl.h"
#include "vectorop.h"

int MSD_highNameAlteraCamera(char opt, char *name, real Ex, real Ey,
                             real Ez, real Rx, real Ry, real Rz,
                             real scale, int mode, int hidtype, int dash,
                             int smooth)
{
   int cn;

   if ((cn = MSD_getCameraIdFromName(name)) == -1)
   {
      fprintf(stderr, MEN_CamaraNomeNaoEncontrada,
              NAM_AlteraCamara, name);
      return(ERROR);
   }
   return(MSD_highAlteraCamera(opt, cn, Ex, Ey, Ez, Rx, Ry, Rz, scale,
                               mode, hidtype, dash, smooth));
}

int MSD_highAlteraCamera(char opt, Id cn, real Ex, real Ey, real Ez,
                         real Rx, real Ry, real Rz, real scale,
                         int mode, int hidtype, int dash, int smooth)
{
   CTYPE *cptr;

   if ((cptr = MSD_getCamera(cn)) == (CTYPE *)NIL)
   {
      fprintf(stderr, MEN_CamaraIdNaoEncontrada,
              NAM_AlteraCamara, cn);
      return(ERROR);
   }
   return(MSD_lowAlteraCamera(opt, cptr, Ex, Ey, Ez, Rx, Ry, Rz, scale,
                              mode, hidtype, dash, smooth));
}

int MSD_lowAlteraCamera(char opt, CTYPE *cptr, real Ex, real Ey,
                        real Ez, real Rx, real Ry, real Rz,
                        real scale, int mode, int hidtype, int dash,
                        int smooth)
{
   int    xmode, xhidtype, xdash, xsmooth;
   vector xeye, xref;
   real   xscale;

   veccopy(xeye, cptr->eye);
   veccopy(xref, cptr->ref);
   xscale   = cptr->scale;
   xmode    = cptr->mode;
   xhidtype = cptr->hid;
   xdash    = cptr->dash;
   xsmooth  = cptr->smooth;
   switch (opt)
   {
       case 'e':
       makevec(xeye, Ex, Ey, Ez, 1.0);
       break;

       case 'r':
       makevec(xref, Rx, Ry, Rz, 1.0);
       break;

       case 's':
       xscale = scale;
       break;

       case 'm':
       xmode = mode;
       break;

       case 'h':
       xhidtype = hidtype;
       break;

       case 'd':
       xdash = dash;
       break;

       case 'f':
       xsmooth = smooth;
       break;
   }
   if (!MSD_lowCheckCamera(xeye, xref, xscale, xmode, xhidtype, xdash, xsmooth))
   {
      fprintf(stderr, MEN_CamaraParametroIncorreto,
              NAM_AlteraCamara);
      return(FALSE);
   }
   MSD_lowSetCamera(cptr, xeye, xref, xscale, xmode, xhidtype, xdash, xsmooth);
   return(TRUE);
}
