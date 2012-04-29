/* 
  PROJETO USPDesigner
  MODULO: HIE (Estrutura Hierarquica)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: hiegrpmn.cpp
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
#include "memvirtu.h"
#include "lowparam.h"
#include "lowmacro.h"
#include "lowsolid.h"
#include "vectorop.h"
#include "mancommd.h"
#include "genfunc_.h"
#include "hiegroup.h"

Id MSD_getGroupIdFromName(char *name)
{
   GPTYPE g;
   int    i = 0;

   for (AllGroup(g))
   {
      if (!strcmp(name, GrpName(g)))
      {
         return(GrpGroupNo(g));
      }
      i++;
      if (i == 100)
      {
         break;
      }
   }
   return((Id) - 1);
}

Id MSD_getMaxGroupId(void)
{
   GPTYPE gptr;
   Id     gn;

   if (grpary == GNIL)
   {
      return(1);
   }
   gn = GrpGroupNo(grpary);
   for (AllGroup(gptr))
   {
      if (GrpGroupNo(gptr) > gn)
      {
         gn = GrpGroupNo(gptr);
      }
   }
   return(gn + 1);
}

int MSD_getNumberOfGroups(void)
{
   GPTYPE gptr;
   int    i = 0;

   for (AllGroup(gptr))
   {
      i++;
   }
   return(i);
}

char **MSD_putGroupNameInList(char **list)
{
   GPTYPE gptr;
   int    i = 0;

   for (AllGroup(gptr))
   {
      list[i] = GrpName(gptr);
      i++;
   }
   return(list);
}

GPTYPE MSD_lowFindGroupByName(char *nam)
{
   GPTYPE gptr;

   for (AllGroup(gptr))
   {
      if (!strcmp(nam, GrpName(gptr)))
      {
         return(gptr);
      }
   }
   return(GNIL);
}

GPTYPE MSD_getGroup(Id gn)
{
   GPTYPE gptr;

   for (AllGroup(gptr))
   {
      if (GrpGroupNo(gptr) == gn)
      {
         return(gptr);
      }
   }
   return(GNIL);
}

char MSD_lowIsEmptyGroup(GPTYPE grpptr)
{
   GPTYPE gptr;
   GPTYPE gfptr;
   SPTYPE optr;

   for (AllSolids(optr))
   {
      if (SolGroup(optr) == grpptr)
      {
         return(FALSE);
      }
   }
   for (AllGroup(gptr))
   {
      if ((gfptr = gptr) != grpptr)
      {
         while ((gfptr = GrpParent(gfptr)) != GNIL)
         {
            if (gfptr == grpptr)
            {
               return(FALSE);
            }
         }
      }
   }
   return(TRUE);
}

char MSD_lowIsAncestorGroup(GPTYPE ancgptr, GPTYPE desgptr)
{
   GPTYPE gptr;

   for (gptr = desgptr; gptr != GNIL; gptr = GrpParent(gptr))
   {
      if (ancgptr == gptr)
      {
         return(TRUE);
      }
   }
   return(FALSE);
}

char MSD_lowIsAncestorAxis(APTYPE ancaptr, APTYPE desaptr)
{
   GPTYPE gptr;

   for (gptr = MSD_getGroupAxis(desaptr); gptr != GNIL; gptr = GrpParent(gptr))
   {
      if (GrpGAxs(gptr) == ancaptr)
      {
         return(TRUE);
      }
   }
   return(FALSE);
}

GPTYPE MSD_getGroupAxis(APTYPE axis)
{
   if (AxsType(axis) == SOLID)
   {
      return(SolGroup(AxsASolid(axis)));
   }
   if (AxsType(axis) == GROUP)
   {
      return(AxsAGroup(axis));
   }
   return(GNIL);
}
