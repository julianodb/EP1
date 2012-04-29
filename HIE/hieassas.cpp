/* 
  PROJETO USPDesigner
  MODULO: HIE (Estrutura Hierarquica)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: hieassas.cpp
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
void MSD_execNameAssociaSolidAxis(void)
{
   char sname[30], aname[30];

   while (2 != sscanf(restbuf, "%s %s", sname, aname))
   {
      printf("AssAxisSolid: solido axis\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highNameAssociaSolidAxis(sname, aname);
}

#endif

int MSD_highNameAssociaSolidAxis(char *name1, char *name2)
{
   int sn, an;

   if ((sn = MSD_getSolidIdFromName(name1)) == -1)
   {
      fprintf(stderr, MEN_NaoEncontrouSolidNome, NAM_AssociaAxisSolid, name1);
      return(ERROR);
   }
   if ((an = MSD_getAxisIdFromName(name2)) == -1)
   {
      fprintf(stderr, MEN_NaoEncontrouAxisNome, NAM_AssociaAxisSolid, name2);
      return(ERROR);
   }
   return(MSD_highAssociaSolidAxis(sn, an));
}

#ifdef __Extended_Version
void MSD_execAssociaSolidAxis(void)
{
   int sn, an;

   while (2 != sscanf(restbuf, "%d %d", &sn, &an))
   {
      printf("AssAxisSolid: sn an\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highAssociaSolidAxis(sn, an);
}

#endif

int MSD_highAssociaSolidAxis(int sn, int an)
{
   SPTYPE optr;
   APTYPE aptr;

   if (((optr = MSD_getSolid(sn)) == SNIL) && (sn != -1))
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_AssociaAxisSolid, sn);
      return(ERROR);
   }
   if (((aptr = MSD_getAxis(sn)) == ANIL) && (an != -1))
   {
      fprintf(stderr, MEN_NaoEncontrouAxisId, NAM_AssociaAxisSolid, an);
      return(ERROR);
   }
   if ((sn == -1) && (an == -1))
   {
      fprintf(stderr, MEN_PonteirosNulos, NAM_AssociaAxisSolid);
      return(ERROR);
   }
   return(MSD_lowAssociaSolidAxis(aptr, optr));
}

int MSD_lowAssociaSolidAxis(APTYPE aptr, SPTYPE optr)
{
   Id code, sn, an;

   if (Gdoinversion)
   {
      /* Verifica se o eixo ja estava associado a algum solido ou
       * grupo. Neste caso, devemos recuperar a associacao com o eixo.
       */

      an   = AxsAxisNo(aptr);
      code = sn = -1;
      if (aptr != ANIL)
      {
         if (AxsType(aptr) == GROUP)
         {
            sn   = GrpGroupNo(AxsAGroup(aptr));
            code = AxisGroup;
         }
         if (AxsType(aptr) == SOLID)
         {
            sn   = SolSolidNo(AxsASolid(aptr));
            code = AxisSolid;
         }
         MSD_lowAddEulerOp(SNIL, ASSOCIATE, code, an, sn, 0, 0, 0, 0, 0,
                           0.0, 0.0, 0.0, 0.0);
      }

      /* Verifica se o solido ja estava associado a algum outro eixo.
       * neste caso, devemos recuperar a associacao com o solido.
       */

      code = -1;
      an   = sn = -1;
      if (optr != SNIL)
      {
         if (SolOAxs(optr) != ANIL)
         {
            sn   = SolSolidNo(optr);
            an   = AxsAxisNo(SolOAxs(optr));
            code = AxisSolid;
         }
         MSD_lowAddEulerOp(SNIL, ASSOCIATE, code, an, sn, 0, 0, 0, 0, 0,
                           0.0, 0.0, 0.0, 0.0);
      }
   }

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
      AxsType(aptr)   = SOLID;
      AxsASolid(aptr) = optr;
   }

   if (optr != SNIL)
   {
      if (SolOAxs(optr) != ANIL)
      {
         AxsASolid(SolOAxs(optr)) = SNIL;
      }
      SolOAxs(optr) = aptr;
   }
   return(SUCCESS);
}

int MSD_execStructAssociate(EulerOp *op)
{
   Id code, an, sn;

   code = op->ip1;
   an   = op->ip2;
   sn   = op->ip3;
   switch (code)
   {
       case AxisSolid:
       return(MSD_highAssociaSolidAxis(an, sn));

       case AxisGroup:
       return(MSD_highAssociaGroupAxis(an, sn));

       case GroupSolid:
       return(MSD_highAssociaGroupSolid(an, sn));

       case GroupGroup:
       return(MSD_highAssociaGroupGroup(an, sn));

       case - 1:
       return(SUCCESS);

       default:
       return(ERROR);
   }
}

void MSD_printStructAssociate(FILE *f, EulerOp *op)
{
   Id sid, id1, id2, id3;

   sid = op->solidno;
   id1 = op->ip1;
   id2 = op->ip2;
   id3 = op->ip3;
   fprintf(f, "associate %d %d %d %d\n", sid, id1, id2, id3);
}

int MSD_readStructAssociate(FILE *f, EulerOp *op)
{
   int solidno, ip1, ip2, ip3;

   if (fscanf(f, "%d %d %d %d\n", &solidno, &ip1, &ip2, &ip3) != 4)
   {
      return(ERROR);
   }
   op->solidno = solidno;
   op->ip1     = ip1;
   op->ip2     = ip2;
   op->ip3     = ip3;
   op->ip4     = 0;
   op->ip5     = 0;
   op->ip6     = 0;
   op->ip7     = 0;
   op->ip8     = 0;
   op->fp1     = 0.0;
   op->fp2     = 0.0;
   op->fp3     = 0.0;
   op->fp4     = 0.0;
   return(SUCCESS);
}

int MSD_modifyStructAssociate(EulerOp *op)
{
   Id code, an, sn;

   code = op->ip1;
   an   = op->ip2;
   sn   = op->ip3;
   switch (code)
   {
       case AxisSolid:
       an = MSD_lowCheckTranslatorTable(an, AXIS);
       sn = MSD_lowCheckTranslatorTable(sn, SOLID);
       break;

       case AxisGroup:
       an = MSD_lowCheckTranslatorTable(an, AXIS);
       sn = MSD_lowCheckTranslatorTable(sn, GROUP);
       break;

       case GroupSolid:
       an = MSD_lowCheckTranslatorTable(an, GROUP);
       sn = MSD_lowCheckTranslatorTable(sn, SOLID);
       break;

       case GroupGroup:
       an = MSD_lowCheckTranslatorTable(an, GROUP);
       sn = MSD_lowCheckTranslatorTable(sn, GROUP);
       break;
   }
   op->ip2 = an;
   op->ip3 = sn;
   return(SUCCESS);
}
