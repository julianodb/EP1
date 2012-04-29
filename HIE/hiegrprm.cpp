/* 
  PROJETO USPDesigner
  MODULO: HIE (Estrutura Hierarquica)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: hiegrprm.cpp
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
#include "genfunc_.h"
#include "eulerops.h"
#include "hiegroup.h"

#ifdef __Extended_Version
void MSD_execNameRemoveGroup(void)
{
   char gname[30];

   while (1 != sscanf(restbuf, "%s", gname))
   {
      printf("Remove Group: group\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highNameRemoveGroup(gname);
}

#endif

int MSD_highNameRemoveGroup(char *name)
{
   Id gn;

   if ((gn = MSD_getGroupIdFromName(name)) == -1)
   {
      fprintf(stderr, MEN_NaoEncontrouGroupNome, NAM_RemoveGroup, name);
      return(ERROR);
   }
   return(MSD_highRemoveGroup(gn));
}

#ifdef __Extended_Version
void MSD_execRemoveGroup(void)
{
   int gn;

   while (1 != sscanf(restbuf, "%d", &gn))
   {
      printf("Remove Group: gn\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highRemoveGroup(gn);
}

#endif

int MSD_highRemoveGroup(int gn)
{
   GPTYPE gptr;

   if ((gptr = MSD_getGroup(gn)) == GNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouGroupId, NAM_RemoveGroup, gn);
      return(ERROR);
   }
   if (MSD_lowIsAncestorGroup(gptr, def_grpptr))
   {
      fprintf(stderr, MEN_GrupoCorrenteEliminado, NAM_RemoveGroup);
      return(ERROR);
   }
   return(MSD_lowRemoveGroup(gptr));
}

int MSD_lowRemoveGroup(GPTYPE gptr)
{
   GPTYPE eragrptr;
   GPTYPE gptrn;
   GPTYPE gptrp;
   APTYPE aptrn;
   APTYPE aptrp;
   SPTYPE optrn;
   SPTYPE optrp;

   eragrptr = gptr;
   for (optrn = objary; optrn != SNIL;)
   {
      if (MSD_lowIsAncestorGroup(eragrptr, SolGroup(optrn)))
      {
         optrp = SolNxt(optrn);
         MSD_lowSoftRemove(optrn);
         optrn = optrp;
      }
      else
      {
         optrn = SolNxt(optrn);
      }
   }

   for (aptrn = axsary; aptrn != ANIL;)
   {
      if (MSD_lowIsAncestorGroup(eragrptr, MSD_getGroupAxis(aptrn)))
      {
         aptrp = AxsNextA(aptrn);
         MSD_lowRemoveAxis(aptrn);
         aptrn = aptrp;
      }
      else
      {
         aptrn = AxsNextA(aptrn);
      }
   }

   for (gptrn = grpary; gptrn != GNIL;)
   {
      if (MSD_lowIsAncestorGroup(eragrptr, gptrn) && (gptrn != eragrptr))
      {
         gptrp = GrpNextG(gptrn);
         MSD_lowRemoveOneGroup(gptrn);
         gptrn = gptrp;
      }
      else
      {
         gptrn = GrpNextG(gptrn);
      }
   }

   MSD_lowRemoveOneGroup(eragrptr);
   return(SUCCESS);
}