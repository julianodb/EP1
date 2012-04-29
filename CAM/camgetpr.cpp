/* 
  PROJETO USPDesigner
  MODULO: CAM (Camera Manipulation)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: camgetpr.cpp
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

#ifdef __Extended_Version
void MSD_execGetCurrentCameraParameters(void)
{
   real Ex, Ey, Ez, Rx, Ry, Rz, ampl;
   int  mode, elim, hash, edge;

   if (MSD_getCurrentCameraParameters(&Ex, &Ey, &Ez, &Rx, &Ry, &Rz,
                                      &ampl, &mode, &elim, &hash, &edge))
   {
      printf("Ex = %10.4f Ey = %10.4f Ez = %10.4f\n", Ex, Ey, Ez);
      printf("Rx = %10.4f Ry = %10.4f Rz = %10.4f\n", Rx, Ry, Rz);
      printf("mode = %s\n", mode == 1 ? "a" : mode == 2 ? "b" : "c");
      printf("elim = %s\n", elim == 0 ? "nenhum processamento" :
             elim == 1 ? "processamento local"  :
             elim == 2 ? "processamento global" :
             "global + interseccao");
      printf("hash = %s\n", hash == 1 ? "exibe linhas escondidas" :
             "nao exibe linhas escondidas");
      printf("edge = %s\n", edge == 1 ? "exibe edges paralelos" :
             "nao exibe edges paralelos");
   }
   else
   {
      printf(MEN_NenhumaCamaraCorrente, NAM_GetCamaraCorrenteParametros);
   }
}

#endif

int MSD_getCurrentCameraParameters(real *Ex, real *Ey, real *Ez,
                                   real *Rx, real *Ry, real *Rz,
                                   real *ampl, int *mode, int *elim,
                                   int *hash, int *edge)
{
   *Ex   = *Ey = *Ez = *Rx = *Ry = *Rz = *ampl = 0.0;
   *elim = *hash = *edge = *mode = 0;
   if (camptr == (CTYPE *)NIL)
   {
      return(FALSE);
   }

   *Ex   = camptr->eye[0];
   *Ey   = camptr->eye[1];
   *Ez   = camptr->eye[2];
   *Rx   = camptr->ref[0];
   *Ry   = camptr->ref[1];
   *Rz   = camptr->ref[2];
   *ampl = camptr->scale;
   *mode = camptr->mode;
   *elim = camptr->hid;
   *hash = camptr->dash;
   *edge = camptr->smooth;
   return(TRUE);
}
