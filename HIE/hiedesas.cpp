/* 
  PROJETO USPDesigner
  MODULO: HIE (Estrutura Hierarquica)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: hiedesas.cpp
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
void MSD_execNameDesassociaSolidAxis(void)
{
   char sname[30], aname[30];

   while (2 != sscanf(restbuf, "%s %s", sname, aname))
   {
      printf("DesAxisSolid: solid axis\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highNameDesassociaSolidAxis(sname, aname);
}

#endif

int MSD_highNameDesassociaSolidAxis(char *name1, char *name2)
{
   int sn, an;

   if ((sn = MSD_getSolidIdFromName(name1)) == -1)
   {
      fprintf(stderr, MEN_NaoEncontrouSolidNome,
              NAM_DesassociaAxisSolid, name1);
      return(ERROR);
   }
   if ((an = MSD_getAxisIdFromName(name2)) == -1)
   {
      fprintf(stderr, MEN_NaoEncontrouAxisNome,
              NAM_DesassociaAxisSolid, name2);
      return(ERROR);
   }
   return(MSD_highAssociaSolidAxis(sn, an));
}

#ifdef __Extended_Version
void MSD_execDesassociaSolidAxis(void)
{
   int sn, an;

   while (2 != sscanf(restbuf, "%d %d", &sn, &an))
   {
      printf("DesAxisSolid: sn an\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highDesassociaSolidAxis(sn, an);
}

#endif

int MSD_highDesassociaSolidAxis(int sn, int an)
{
   SPTYPE sptr;
   APTYPE aptr;

   if ((sptr = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido,
              NAM_DesassociaAxisSolid, sn);
      return(ERROR);
   }
   if ((aptr = MSD_getAxis(sn)) == ANIL)
   {
      fprintf(stderr, MEN_NaoEncontrouAxisId,
              NAM_DesassociaAxisSolid, an);
      return(ERROR);
   }
   if (AxsType(aptr) == SOLID)
   {
      if (AxsASolid(aptr) == sptr)
      {
         return(MSD_lowAssociaSolidAxis(aptr, SNIL));
      }
   }
   fprintf(stderr, MEN_ParametrosIncorretos, NAM_DesassociaAxisSolid);
   return(ERROR);
}
