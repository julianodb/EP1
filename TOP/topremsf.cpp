/* 
  PROJETO USPDesigner
  MODULO: TOP (Operadores de Nivel Intermediario)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: topremsf.cpp
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
#include "mensagem.h"
#include "memvirtu.h"
#include "lowparam.h"
#include "lowmacro.h"
#include "lowsolid.h"
#include "eulerops.h"
#include "mancommd.h"

/* Remove a solid */
#ifdef __Extended_Version
void MSD_execNameSoftRemove(void)
{
   char name[30];

   while (1 != sscanf(restbuf, "%s", name))
   {
      printf("SoftRemove: name\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highNameSoftRemove(name);
}

#endif

int MSD_highNameSoftRemove(char *name)
{
   SPTYPE sptr;

   if ((sptr = MSD_lowFindSolidByName(name)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolidNome, NAM_SoftRemove, name);
      return(FALSE);
   }
   return(MSD_lowSoftRemove(sptr));
}

#ifdef __Extended_Version
void MSD_execSoftRemove(void)
{
   int sn;

   while (sscanf(restbuf, "%d", &sn) != 1)
   {
      printf("solidrm: Sid");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highSoftRemove(sn);
}

#endif

int MSD_highSoftRemove(Id sn)
{
   SPTYPE s;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_SoftRemove, sn);
      return(ERROR);
   }
   return(MSD_lowSoftRemove(s));
}

int MSD_lowSoftRemove(SPTYPE s)
{
   char nome[13];

   if (SolGroup(s) != GNIL)
   {
      MSD_highDesassociaGroupSolid(GrpGroupNo(SolGroup(s)), SolSolidNo(s));
   }

   strcpy(nome, SolName(s));
   MSD_lowSolidColor(s, SolColor(s));
   MSD_lowSolidName(s, nome);


   if (SolSShells(s) != DNIL)
   {
      MSD_lowShellRemove(s);
      MSD_lowEdgeRemove(s);
      MSD_lowFaceRemove(s);
      MSD_lowKVSF(s);
   }
   return(SUCCESS);
}

void MSD_lowShellRemove(SPTYPE s)
{
   DPTYPE d;

   while ((d = SheNextD(SolSShells(s))) != DNIL)
   {
      MSD_lowKSFMR(SheSFaces(SolSShells(s)), SheSFaces(d));
   }
}

void MSD_lowEdgeRemove(SPTYPE s)
{
   EPTYPE e;
   LPTYPE l1;
   LPTYPE l2;

   maxf = MSD_getMaxFaceId(s);
   while ((e = SheSEdges(SolSShells(s))) != ENIL)
   {
      if ((l1 = HalWLoop(EdgHe1(e))) == (l2 = HalWLoop(EdgHe2(e))))
      {
         if (HalNxt(EdgHe1(e)) == EdgHe2(e))
         {
            MSD_lowKEV(EdgHe2(e), EdgHe1(e));
         }
         else
         if (HalNxt(EdgHe2(e)) == EdgHe1(e))
         {
            MSD_lowKEV(EdgHe1(e), EdgHe2(e));
         }
         else
         {
            MSD_lowKEMR(EdgHe1(e), EdgHe2(e));
         }
      }
      else
      if (LooLFace(l1) != LooLFace(l2))
      {
         MSD_lowKEF(EdgHe1(e), EdgHe2(e));
      }
      else
      {
         MSD_lowMFKRH(LooLFace(l1), (l1 == FacFLOut(LooLFace(l1)))
                      ? l2 : l1, ++maxf);
      }
   }
}

void MSD_lowFaceRemove(SPTYPE s)
{
   FPTYPE f1;
   FPTYPE f2;
   LPTYPE out;
   LPTYPE ring;

   for (AllFacesShell(SolSShells(s), f1))
   {
      out  = FacFLOut(f1);
      ring = FacFLoops(f1);
      while (LooNextL(FacFLoops(f1)) != LNIL)
      {
         if (ring != out)
         {
            HPTYPE he1 = LooLEdg(out);
            HPTYPE he2 = LooLEdg(ring);
            MSD_lowMEKR(he1, he2);
            MSD_lowKEV(he1, he2);
            ring = FacFLoops(f1);
         }
         else
         {
            ring = LooNextL(ring);
         }
      }
   }
   f1 = SheSFaces(SolSShells(s));
   while ((f2 = FacNextF(f1)) != FNIL)
   {
      MSD_lowKFMRH(f1, f2);
      MSD_lowMEKR(LooLEdg(FacFLoops(f1)), LooLEdg(LooNextL(FacFLoops(f1))));
      MSD_lowKEV(LooLEdg(FacFLoops(f1)), mate(LooLEdg(FacFLoops(f1))));
   }
}
