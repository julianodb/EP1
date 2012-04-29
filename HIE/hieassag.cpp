/* 
  PROJETO USPDesigner
  MODULO: HIE (Estrutura Hierarquica)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: hieassag.cpp
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
#include "eulerops.h"
#include "mancommd.h"
#include "hiegroup.h"

#ifdef __Extended_Version
void MSD_execNameAssociaGroupAxis(void)
{
   char sname[30], aname[30];

   while (2 != sscanf(restbuf, "%s %s", sname, aname))
   {
      printf("AssAxisGroup: grupo axis\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highNameAssociaGroupAxis(sname, aname);
}

#endif

int MSD_highNameAssociaGroupAxis(char *name1, char *name2)
{
   int sn, an;

   if ((sn = MSD_getGroupIdFromName(name1)) == -1)
   {
      fprintf(stderr, MEN_NaoEncontrouGroupNome, NAM_AssociaAxisGroup, name1);
      return(ERROR);
   }
   if ((an = MSD_getAxisIdFromName(name2)) == -1)
   {
      fprintf(stderr, MEN_NaoEncontrouAxisNome, NAM_AssociaAxisGroup, name2);
      return(ERROR);
   }
   return(MSD_highAssociaGroupAxis(sn, an));
}

#ifdef __Extended_Version
void MSD_execAssociaGroupAxis(void)
{
   int sn, an;

   while (2 != sscanf(restbuf, "%d %d", &sn, &an))
   {
      printf("AssAxisGroup: gn an\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highAssociaGroupAxis(sn, an);
}

#endif

int MSD_highAssociaGroupAxis(int sn, int an)
{
   GPTYPE gptr;
   APTYPE aptr;

   if (((gptr = MSD_getGroup(sn)) == GNIL) && (sn != -1))
   {
      fprintf(stderr, MEN_NaoEncontrouGroupId, NAM_AssociaAxisGroup, sn);
      return(ERROR);
   }
   if (((aptr = MSD_getAxis(sn)) == ANIL) && (an != -1))
   {
      fprintf(stderr, MEN_NaoEncontrouAxisId, NAM_AssociaAxisGroup, an);
      return(ERROR);
   }
   if ((sn == -1) && (an == -1))
   {
      fprintf(stderr, MEN_PonteirosNulos, NAM_AssociaAxisGroup);
      return(ERROR);
   }
   return(MSD_lowAssociaGroupAxis(aptr, gptr));
}

int MSD_lowAssociaGroupAxis(APTYPE aptr, GPTYPE gptr)
{
   Id code, gn, an;

   if (Gdoinversion)
   {
      /* Verifica se o eixo ja estava associado a algum solido ou
       * grupo. Neste caso, devemos recuperar a associacao com o eixo.
       */

      code = gn = -1;
      if (aptr != ANIL)
      {
         an = AxsAxisNo(aptr);
         if (AxsType(aptr) == GROUP)
         {
            gn   = GrpGroupNo(AxsAGroup(aptr));
            code = AxisGroup;
         }
         if (AxsType(aptr) == SOLID)
         {
            gn   = SolSolidNo(AxsASolid(aptr));
            code = AxisSolid;
         }
         MSD_lowAddEulerOp(SNIL, ASSOCIATE, code, an, gn, 0, 0, 0, 0, 0,
                           0.0, 0.0, 0.0, 0.0);
      }

      /* Verifica se o solido ja estava associado a algum outro eixo.
       * neste caso, devemos recuperar a associacao com o solido.
       */

      code = -1;
      an   = gn = -1;
      if (gptr != GNIL)
      {
         if (GrpGAxs(gptr) != ANIL)
         {
            gn   = GrpGroupNo(gptr);
            an   = AxsAxisNo(GrpGAxs(gptr));
            code = AxisGroup;
         }
         MSD_lowAddEulerOp(SNIL, ASSOCIATE, code, an, gn, 0, 0, 0, 0, 0,
                           0.0, 0.0, 0.0, 0.0);
      }
   }

   /*  Se o eixo ja estiver conectado a um solido ou grupo. Entao devemos
    *  desconecta-lo - zerando o ponteiro do solido ou grupo.
    */
   if (aptr != ANIL)
   {
      if (AxsType(aptr) == SOLID)
      {
         SolOAxs(AxsASolid(aptr)) = ANIL;
      }
      if (AxsType(aptr) == GROUP)
      {
         GrpGAxs(AxsAGroup(aptr)) = ANIL;
      }
      AxsType(aptr)   = GROUP;
      AxsAGroup(aptr) = gptr;
   }

   /*  Se o grupo ja estiver conectado a um eixo. Entao devemos
    *  desconecta-lo - zerando o ponteiro do eixo.
    */
   if (gptr != GNIL)
   {
      if (GrpGAxs(gptr) != ANIL)
      {
         AxsAGroup(GrpGAxs(gptr)) = GNIL;
      }
      GrpGAxs(gptr) = aptr;
   }
   return(SUCCESS);
}
