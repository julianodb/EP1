/* 
  PROJETO USPDesigner
  MODULO: DIS (Hidden Line Elimination)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: disccolr.cpp
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
#include "hiegroup.h"
#include "disdispl.h"
#include "genfunc_.h"
#include "gphgraph.h"
#include "mancommd.h"
#include "eulerops.h"

extern int MaxColors;

void MSD_execColor(void)
{
   int  ip;
   char name1[12];
   int  i1;

   for (ip = 0; ip == 0;)
   {
      switch (optin())
      {
          case 'o':
          if (2 == sscanf(restbuf, "%s %d", name1, &i1))
          {
             MSD_highNameSolidColor(name1, i1);
             ip = 1;
          }
          break;

          case 'g':
          if (2 == sscanf(restbuf, "%s %d", name1, &i1))
          {
             MSD_highNameGroupColor(name1, i1);
             ip = 1;
          }
          break;

          case 'd':
          if (1 == sscanf(restbuf, "%d", &i1))
          {
             MSD_lowSetDefaultColor(i1);
             ip = 1;
          }
          break;

          case 'f':
          if (1 == sscanf(restbuf, "%d", &i1))
          {
             MSD_lowSetFrameColor(i1);
             ip = 1;
          }
          break;
      }
      if (ip == 0)
      {
         printf("-dfgo [Objeto/Grupo] cor\n");
         if (!lineins("? "))
         {
            return;
         }
      }
   }
}

int MSD_lowSetFrameColor(int color)
{
   if ((color > -1) && (color < MaxColors))
   {
      framecol = color;
      return(TRUE);
   }
   fprintf(stderr, MEN_CorInvalida, NAM_SetFrameColor);
   return(FALSE);
}

int MSD_lowSetDefaultColor(int color)
{
   if ((color > -2) && (color < MaxColors))
   {
      defltcol = color;
      return(TRUE);
   }
   fprintf(stderr, MEN_CorInvalida, NAM_SetDefaultColor);
   return(FALSE);
}

/*
 * int MSD_highNameSetElementColor(char *name, int color)
 * {
 *  if (color < -1 || color > MaxColors - 1) {
 *      fprintf(stderr, MEN_CorInvalida, NAM_SetElementColor) ;
 *      return(FALSE) ;
 *  }
 *  switch(MSD_getTypeByName(name)) {
 *      case 'o':
 *          return(MSD_highNameSetSolidColor(name, color)) ;
 *      case 'g':
 *          return(MSD_highNameSetGroupColor(name, color)) ;
 *  }
 *  fprintf(stderr, MEN_ElementoNaoEncontrado, NAM_SetElementColor, name) ;
 *  return(FALSE) ;
 * }
 *
 * int MSD_highNameSetSolidColor(char *name, int col)
 * {
 *  SPTYPE optr ;
 *
 *  if (col < -2 || col > MaxColors - 1) {
 *      fprintf(stderr, MEN_CorInvalida, NAM_SetElementColor) ;
 *      return(FALSE) ;
 *  }
 *  if ((optr = MSD_lowFindSolidByName(name)) != SNIL)
 *      return(MSD_lowSetSolidColor(optr, col)) ;
 *  fprintf(stderr, MEN_ElementoNaoEncontrado, NAM_SetElementColor, name) ;
 *  return(FALSE) ;
 * }
 *
 * int MSD_lowSetSolidColor(SPTYPE optr, int col)
 * {
 *  if (col < -2 || col > MaxColors - 1) {
 *      fprintf(stderr, MEN_CorInvalida, NAM_SetElementColor) ;
 *      return(FALSE) ;
 *  }
 *  SolColor(optr) = col ;
 *  return(TRUE) ;
 * }
 *
 * int MSD_highNameSetGroupColor(char *name, int col)
 * {
 *  GPTYPE gptr ;
 *
 *  if (col < -2 || col > MaxColors - 1) {
 *      fprintf(stderr, MEN_CorInvalida, NAM_SetElementColor) ;
 *      return(FALSE) ;
 *  }
 *  if ((gptr = MSD_lowFindGroupByName(name)) != GNIL)
 *      return(MSD_lowSetGroupColor(gptr, col)) ;
 *  fprintf(stderr, MEN_ElementoNaoEncontrado, NAM_SetElementColor, name) ;
 *  return(FALSE) ;
 * }
 *
 * int MSD_lowSetGroupColor(GPTYPE gptr, int col)
 * {
 *  if (col < -2 || col > MaxColors - 1) {
 *      fprintf(stderr, MEN_CorInvalida, NAM_SetElementColor) ;
 *      return(FALSE) ;
 *  }
 *  GrpGColor(gptr) = col ;
 *  return(TRUE) ;
 * }
 */

int MSD_lowGetIntersectionColor(SPTYPE o1ptr, SPTYPE o2ptr)
{
   int col1, col2;

   col1 = MSD_lowGetSolidColor(o1ptr);
   col2 = MSD_lowGetSolidColor(o2ptr);
   return(col1 > col2 ? col1 : col2);
}

int MSD_lowGetSolidColor(SPTYPE optr)
{
   GPTYPE gptr;

   if (SolColor(optr) > -1)
   {
      return(SolColor(optr));
   }
   for (gptr = SolGroup(optr); gptr != GNIL; gptr = GrpParent(gptr))
   {
      if (GrpGColor(gptr) > -1)
      {
         return(GrpGColor(gptr));
      }
   }
   return(defltcol);
}
