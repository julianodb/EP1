/* 
  PROJETO USPDesigner
  MODULO: HIE (Estrutura Hierarquica)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: hierotat.cpp
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
#include "mensagem.h"
#include "memvirtu.h"
#include "lowparam.h"
#include "lowmacro.h"
#include "lowsolid.h"
#include "vectorop.h"
#include "mancommd.h"
#include "disdispl.h"
#include "genfunc_.h"
#include "shpshape.h"
#include "hiegroup.h"

#ifdef __Extended_Version
void MSD_execNameRotateGroup(void)
{
   char  name[30];
   float th, x1, y1, z1, x2, y2, z2;

   if (8 != sscanf(restbuf, "%s %f %f %f %f %f %f %f", name,
                   &x1, &y1, &z1, &x2, &y2, &z2, &th))
   {
      printf("Grupo Pto_axis Vector Angulo\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highNameRotateGroup(name, x1, y1, z1, x2, y2, z2, th);
}

#endif

int MSD_highNameRotateGroup(char *name, real x1, real y1, real z1, \
                            real x2, real y2, real z2, real th)
{
   int gn;

   if ((gn = MSD_getGroupIdFromName(name)) == -1)
   {
      fprintf(stderr, MEN_NaoEncontrouGroupNome, NAM_RotateGroup, name);
      return(ERROR);
   }
   return(MSD_highRotateGroup(gn, x1, y1, z1, x2, y2, z2, th));
}

#ifdef __Extended_Version
void MSD_execRotateGroup(void)
{
   float th, x1, y1, z1, x2, y2, z2;
   int   gn;

   if (8 != sscanf(restbuf, "%d %f %f %f %f %f %f %f", &gn,
                   &x1, &y1, &z1, &x2, &y2, &z2, &th))
   {
      printf("Grupo Pto_axis Vector Angulo\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highRotateGroup(gn, x1, y1, z1, x2, y2, z2, th);
}

#endif

int MSD_highRotateGroup(Id gn, real x1, real y1, real z1, \
                        real x2, real y2, real z2, real th)
{
   GPTYPE gptr;
   vector center, vec;

   if ((gptr = MSD_getGroup(gn)) == GNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouGroupId, NAM_RotateGroup, gn);
      return(ERROR);
   }
   makevec(center, x1, y1, z1, 1.0);
   makevec(vec, x2, y2, z2, 1.0);
   normalize(vec);
   return(MSD_lowRotateGroup(gptr, center, vec, th));
}

int MSD_lowRotateGroup(GPTYPE gptr, vector center, vector vec, real th)
{
   SPTYPE optr;
   APTYPE aptr;

   for (AllSolids(optr))
   {
      if (MSD_lowIsAncestorGroup(gptr, SolGroup(optr)))
      {
         MSD_lowRotateSolidAxis(optr, center, vec, th);
      }
   }

   for (AllAxis(aptr))
   {
      if (MSD_lowIsAncestorGroup(gptr, MSD_getGroupAxis(aptr)))
      {
         MSD_lowRotateAxis(aptr, center, vec, th);
      }
   }

   return(SUCCESS);
}
