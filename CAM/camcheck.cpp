/* 
  PROJETO USPDesigner
  MODULO: CAM (Camera Manipulation)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: camcheck.cpp
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
#include "gphgraph.h"
#include "vectorop.h"

int MSD_highCheckCamera(real Ex, real Ey, real Ez, real Rx, real Ry,
                        real Rz, real scale, int mode, int hidtype,
                        int dash, int smooth)
{
   vector eye, ref;

   makevec(eye, Ex, Ey, Ez, 1.0);
   makevec(ref, Rx, Ry, Rz, 1.0);
   return(MSD_lowCheckCamera(eye, ref, scale, mode, hidtype, dash, smooth));
}

int MSD_lowCheckCamera(vector eye, vector ref, real scale, int mode,
                       int hidtype, int dash, int smooth)
{
   if (vecd(eye, ref) < abseps * 10.0)
   {
      return(FALSE);
   }
   if (scale < releps * 100.0)
   {
      return(FALSE);
   }
   if ((mode < 1) || (mode > 3))
   {
      return(FALSE);
   }
   if ((hidtype < 0) || (hidtype > 3))
   {
      return(FALSE);
   }
   if ((dash < 0) || (dash > 1))
   {
      return(FALSE);
   }
   if ((smooth < 0) || (smooth > 1))
   {
      return(FALSE);
   }
   return(TRUE);
}

void MSD_highSetCamera(CTYPE *cptr, real Ex, real Ey, real Ez,
                       real Rx, real Ry, real Rz, real scale,
                       int mode, int hidtype, int dash, int smooth)
{
   vector eye, ref;

   makevec(eye, Ex, Ey, Ez, 1.0);
   makevec(ref, Rx, Ry, Rz, 1.0);
   MSD_lowSetCamera(cptr, eye, ref, scale, mode, hidtype, dash, smooth);
}

void MSD_lowSetCamera(CTYPE *cptr, vector eye, vector ref, real scale,
                      int mode, int hidtype, int dash, int smooth)
{
   veccopy(cptr->eye, eye);
   veccopy(cptr->ref, ref);
   cptr->scale  = scale;
   cptr->mode   = mode;
   cptr->hid    = hidtype;
   cptr->dash   = dash;
   cptr->smooth = smooth;
   MSD_lowSetCameraTransformation(cptr);
}

void MSD_lowSetCameraTransformation(CTYPE *cptr)
{
   real sint, cost, sinp, cosp, al, als;

   cptr->cdis = al = vecd(cptr->eye, cptr->ref);
   if ((als = vecd2(cptr->eye, cptr->ref)) > abseps * 10.0)
   {
      sint = (cptr->eye[1] - cptr->ref[1]) / als;
      cost = (cptr->eye[0] - cptr->ref[0]) / als;
      sinp = (cptr->eye[2] - cptr->ref[2]) / al;
      cosp = als / al;
   }
   else
   {
      sint = 0.0;
      cost = 1.0;
      sinp = 1.0;
      cosp = 0.0;
   }
   matident(cptr->drot);
   cptr->drot[0][0] = -sint;
   cptr->drot[0][1] = cost;
   cptr->drot[0][2] = 0.0;
   cptr->drot[1][0] = -sinp * cost;
   cptr->drot[1][1] = -sinp * sint;
   cptr->drot[1][2] = cosp;
   cptr->drot[2][0] = -cosp * cost;
   cptr->drot[2][1] = -cosp * sint;
   cptr->drot[2][2] = -sinp;
   vecminus(cptr->eyedir, cptr->eye, cptr->ref);
   normalize(cptr->eyedir);
}

void MSD_lowSetCamera3View(int type, CTYPE *cptr)
{
   vector eye3;

   switch (type)
   {
       case 0:
       veccopy(dwindbox, windbox);
       break;

       case 1:
       eye3[0] = cptr->ref[0];
       eye3[1] = cptr->ref[1];
       eye3[2] = cptr->ref[2] + cptr->cdis;
       MSD_lowSetCamera(camptr, eye3, cptr->ref, cptr->scale, 2,
                        cptr->hid, cptr->dash, cptr->smooth);
       dwindbox[0] = windbox[0];
       dwindbox[1] = windbox[1] + windbox[2] / 2.0;
       dwindbox[2] = windbox[2] / 2.0;
       break;

       case 2:
       eye3[0] = cptr->ref[0] + cptr->cdis;
       eye3[1] = cptr->ref[1];
       eye3[2] = cptr->ref[2];
       MSD_lowSetCamera(camptr, eye3, cptr->ref, cptr->scale, 2,
                        cptr->hid, cptr->dash, cptr->smooth);
       dwindbox[0] = windbox[0];
       dwindbox[1] = windbox[1];
       dwindbox[2] = windbox[2] / 2.0;
       break;

       case 3:
       eye3[0] = cptr->ref[0];
       eye3[1] = cptr->ref[1] + cptr->cdis;
       eye3[2] = cptr->ref[2];
       MSD_lowSetCamera(camptr, eye3, cptr->ref, cptr->scale, 2,
                        cptr->hid, cptr->dash, cptr->smooth);
       dwindbox[0] = windbox[0] + windbox[2] / 2.0;
       dwindbox[1] = windbox[1];
       dwindbox[2] = windbox[2] / 2.0;
       break;

       case 4:
       dwindbox[0] = windbox[0] + windbox[2] / 2.0;
       dwindbox[1] = windbox[1] + windbox[2] / 2.0;
       dwindbox[2] = windbox[2] / 2.0;
       camptr      = cptr;
   }
}
