/* 
  PROJETO USPDesigner
  MODULO: DIS (Hidden Line Elimination)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: disedge_.cpp
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
 #ifndef _Windows
  #include <graphics.h>
 #else
  #include <windows.h>
 #endif
#endif
#include "mensagem.h"
#include "memvirtu.h"
#include "lowparam.h"
#include "lowmacro.h"
#include "lowsolid.h"
#include "hiegroup.h"
#include "disdispl.h"
#include "gphgraph.h"
#include "genfunc_.h"
#include "shpshape.h"
#include "mancommd.h"

#ifdef __Extended_Version
void MSD_execDisplayEdge(void)
{
   int s, e, color;

   while (3 != sscanf(restbuf, "%d %d %d", &s, &e, &color))
   {
      printf("%s: Sid Eid color\n", NAM_DisplayEdge);
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highDisplayEdge(s, e, color);
}

#endif

void MSD_highDisplayEdge(Id s, Id e, int color)
{
   EPTYPE eptr;

   if ((eptr = MSD_lowShowFindEdge(MSD_getSolid(s), e)) == ENIL)
   {
      fprintf(stderr, MEN_NaoEncontrouAresta,
              NAM_DisplayEdge, e, s);
      return;
   }
   MSD_lowDisplayEdge(eptr, color);
}

void MSD_lowDisplayEdge(EPTYPE eptr, int color)
{
/*
 *  static char *vis[] = {"DONTSHOW", "INVISIBLE", "VISIBLE"} ;
 */
   g_pcolor(color);
   MSD_lowDisplayLine(SOLID_LINE, VerVCoord(HalVtx(EdgHe1(eptr))),
                      VerVCoord(HalVtx(EdgHe2(eptr))));

/*
 *  printf("eptr->he1->wloop->lface->fvis = %s\n",
 *               vis[FacFVisi(LooLFace(HalWLoop(EdgHe1(eptr))))]) ;
 *  printf("eptr->he2->wloop->lface->fvis = %s\n",
 *               vis[FacFVisi(LooLFace(HalWLoop(EdgHe2(eptr))))]) ;
 *  printf("eptr->evis = %s\n", vis[EdgEVis(eptr)]) ;
 */
}
