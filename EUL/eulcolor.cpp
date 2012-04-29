/* 
  PROJETO USPDesigner
  MODULO: EUL (Operadores de Euler)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: eulcolor.cpp
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
 #include <stdlib.h>
#endif
#include "mensagem.h"
#include "memvirtu.h"
#include "lowparam.h"
#include "lowmacro.h"
#include "lowsolid.h"
#include "eulerops.h"
#include "genfunc_.h"
#include "curvgeom.h"
#include "mancommd.h"

extern int MaxColors;

int MSD_highNameColor(char *name, int color)
{
   /* Verifica se a cor e correta. */
   if ((color < -1) || (color > MaxColors - 1))
   {
      fprintf(stderr, MEN_CorInvalida, NAM_Color);
      return(FALSE);
   }

   /* Identifica o tipo de elemento que esta sendo manipulado. */
   switch (MSD_getTypeByName(name))
   {
       case 'o':
       return(MSD_highNameSolidColor(name, color));

       case 'g':
       return(MSD_highNameGroupColor(name, color));
   }

   fprintf(stderr, MEN_ElementoNaoEncontrado, NAM_Color, name);
   return(FALSE);
}

int MSD_highNameSolidColor(char *name, int col)
{
   SPTYPE optr;

   if ((col < -2) || (col > MaxColors - 1))
   {
      fprintf(stderr, MEN_CorInvalida, NAM_SetElementColor);
      return(FALSE);
   }
   if ((optr = MSD_lowFindSolidByName(name)) != SNIL)
   {
      MSD_lowSolidColor(optr, col);
      return(TRUE);
   }

   fprintf(stderr, MEN_ElementoNaoEncontrado, NAM_Color, name);
   return(FALSE);
}

int MSD_highNameGroupColor(char *name, int col)
{
   GPTYPE gptr;

   if ((col < -2) || (col > MaxColors - 1))
   {
      fprintf(stderr, MEN_CorInvalida, NAM_SetElementColor);
      return(FALSE);
   }
   if ((gptr = MSD_lowFindGroupByName(name)) != GNIL)
   {
      MSD_lowGroupColor(gptr, col);
      return(TRUE);
   }
   fprintf(stderr, MEN_ElementoNaoEncontrado, NAM_Color, name);
   return(FALSE);
}

int MSD_highColor(Id sn, int color, int cod)
{
   SPTYPE s;
   GPTYPE g;

   if (cod == SOLID)
   {
      if ((s = MSD_getSolid(sn)) == SNIL)
      {
         fprintf(stderr, MEN_NaoEncontrouSolido, NAM_Color, sn);
         return(ERROR);
      }
      MSD_lowSolidColor(s, color);
   }
   if (cod == GROUP)
   {
      if ((g = MSD_getGroup(sn)) == GNIL)
      {
         fprintf(stderr, MEN_NaoEncontrouGroupId, NAM_Color, sn);
         return(ERROR);
      }
      MSD_lowGroupColor(g, color);
   }
   return(SUCCESS);
}

void MSD_lowSolidColor(SPTYPE s, int color)
{
   int sid, col;

   if (Gdoinversion)
   {
      sid = SolSolidNo(s);
      col = SolColor(s);
      MSD_lowAddEulerOp(s, COLOR, sid, col, SOLID, 0, 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0);
   }

   SolColor(s) = color;
}

void MSD_lowGroupColor(GPTYPE g, int color)
{
   int gid, col;

   if (Gdoinversion)
   {
      gid = GrpGroupNo(g);
      col = GrpGColor(g);
      MSD_lowAddEulerOp(SNIL, COLOR, gid, col, GROUP, 0, 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0);
   }

   GrpGColor(g) = color;
}

int MSD_execStructColor(EulerOp *op)
{
   Id id1, id2, id3;

   id1 = op->ip3 == SOLID ? op->solidno : op->ip1;
   id2 = op->ip2;
   id3 = op->ip3;
   return(MSD_highColor(id1, id2, id3));
}

void MSD_printStructColor(FILE *f, EulerOp *op)
{
   Id  id1, id2, id3;
   int com;

   id1 = op->ip1;
   id2 = op->ip2;
   id3 = op->ip3;
   com = op->CommandCount;
   fprintf(f, "color %d %d %d %d\n", id1, com, id2, id3);
}

int MSD_readStructColor(FILE *f, EulerOp *op)
{
   int ip1, ip2, ip3, com;

   if (fscanf(f, "%d %d %d %d\n", &ip1, &com, &ip2, &ip3) != 4)
   {
      return(ERROR);
   }
   op->CommandCount = com;
   op->ip1          = ip1;
   op->ip2          = ip2;
   op->ip3          = ip3;
   return(SUCCESS);
}

int MSD_modifyStructColor(EulerOp *op)
{
   int solidno;

   if (op->ip3 == SOLID)
   {
      solidno = MSD_lowCheckTranslatorTable(op->ip1, SOLID);
      op->ip1 = solidno;
   }
   if (op->ip3 == GROUP)
   {
      solidno = MSD_lowCheckTranslatorTable(op->ip1, GROUP);
      op->ip1 = solidno;
   }
   return(SUCCESS);
}
