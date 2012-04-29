/* 
  PROJETO USPDesigner
  MODULO: LOW (Operadores Basicos)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: lowfinde.cpp
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
#include "lowsolid.h"
#include "lowmacro.h"

extern SPTYPE objary;                     /* declared in file lownewel.c */

SPTYPE MSD_lowFindSolidByName(char *nam)
{
   SPTYPE sptr;

   for (AllSolids(sptr))
   {
      if (!strcmp(nam, SolName(sptr)))
      {
         return(sptr);
      }
   }
   return(SNIL);
}

Id MSD_getSolidIdFromName(char *name)
{
   SPTYPE s;

   for (AllSolids(s))
   {
      if (!strcmp(name, SolName(s)))
      {
         return(SolSolidNo(s));
      }
   }
   return((Id) - 1);
}

SPTYPE MSD_getSolid(Id sn)
{
   SPTYPE s;

   if (objary == SNIL)
   {
      return(SNIL);
   }
   for (s = objary; s != SNIL; s = SolNxt(s))
   {
      if (SolSolidNo(s) == sn)
      {
         return(s);
      }
   }
   return(SNIL);
}

DPTYPE MSD_getShell(SPTYPE s, Id dn)
{
   DPTYPE d;

   for (AllShellsSolid(s, d))
   {
      if (SheShellNo(d) == dn)
      {
         return(d);
      }
   }
   return(DNIL);
}

LPTYPE MSD_getLoop(FPTYPE f, Id vn, HPTYPE *h)
{
   HPTYPE he;
   LPTYPE l;

   for (AllLoopsFace(f, l))
   {
      he = LooLEdg(l);
      do
      {
         if (VerVertexNo(HalVtx(he)) == vn)
         {
            *h = he;
            return(l);
         }
      } while ((he = HalNxt(he)) != LooLEdg(l));
   }
   return(LNIL);
}

LPTYPE MSD_getEdge(FPTYPE f, Id v1, Id v2, HPTYPE *ptr)
{
   HPTYPE he;
   LPTYPE l;

   for (AllLoopsFace(f, l))
   {
      he = LooLEdg(l);
      do
      {
         if ((VerVertexNo(HalVtx(he)) == v1) &&
             (VerVertexNo(HalVtx(HalNxt(he))) == v2))
         {
            *ptr = he;
            return(l);
         }
      } while ((he = HalNxt(he)) != LooLEdg(l));
   }
   return(LNIL);
}

LPTYPE MSD_getHalfedge(FPTYPE f1, FPTYPE f2, Id v1, Id v2, HPTYPE *ptr)
{
   HPTYPE he;
   LPTYPE l;

   for (AllLoopsFace(f1, l))
   {
      he = LooLEdg(l);
      do
      {
         if ((VerVertexNo(HalVtx(he)) == v1) &&
             (VerVertexNo(HalVtx(HalNxt(he))) == v2))
         {
            *ptr = he;
            if (HalEdg(he) == ENIL)
            {
               return(l);
            }
            if (LooLFace(HalWLoop(mate(he))) == f2)
            {
               return(l);
            }
         }
      } while ((he = HalNxt(he)) != LooLEdg(l));
   }
   return(LNIL);
}

FPTYPE MSD_getFace(SPTYPE s, Id fn)
{
   DPTYPE d;
   FPTYPE f;

   for (AllShellsSolid(s, d))
   {
      for (AllFacesShell(d, f))
      {
         if (FacFaceNo(f) == fn)
         {
            return(f);
         }
      }
   }
   return(FNIL);
}

Id MSD_getMaxSolidId(void)
{
   return(maxs++);
}

/* Alteracao para que todas as faces independentemente de solido possuam IDs
 *                                                              diferentes. */
Id MSD_getMaxFaceId(SPTYPE s)
{
   return(maxf);
}

/* Alteracao para que todas as faces independentemente de solido possuam IDs
 *                                                               diferentes. */
Id MSD_getMaxShellId(SPTYPE s)
{
   return(maxd);
}

/* Alteracao para que todas as faces independentemente de solido possuam IDs
 *                                                               diferentes. */
Id MSD_getMaxVertexId(SPTYPE s)
{
   return(maxv);
}
