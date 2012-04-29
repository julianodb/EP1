/* 
  PROJETO USPDesigner
  MODULO: GEN (Funcoes Gerais de Nivel Superior)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: genfadjr.cpp
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
#include "genfunc_.h"
#include "mancommd.h"

#ifdef __Extended_Version
void MSD_execNameListFaceAdjacency(void)
{
   char name[30], fname[30];

   while (2 != sscanf(restbuf, "%s %s", name, fname))
   {
      printf("ListFaceAdjacency: Objeto Arquivo\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highNameListFaceAdjacency(name, fname);
}

#endif

int MSD_highNameListFaceAdjacency(char *name, char *fname)
{
   SPTYPE optr;

   if ((optr = MSD_lowFindSolidByName(name)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido,
              NAM_ListFaceAdjacency, name);
      return(ERROR);
   }
   return(MSD_lowListFaceAdjacency(optr, fname));
}

int MSD_lowListFaceAdjacency(SPTYPE optr, char *fname)
{
   STYPE  *ss;
   DPTYPE dptr;
   FPTYPE fptr;
   HPTYPE eptr;
   HPTYPE efim;
   int    fe;
   FILE   *f;

   if ((f = fopen(fname, "w")) == (FILE *)NIL)
   {
      fprintf(stderr, MEN_NaoAbriuArquivo, NAM_ListFaceAdjacency, fname);
      return(ERROR);
   }

#ifdef __VIRTUAL_MEM
   ss = SSTYPE(optr);
#else
   ss = optr;
#endif

   fprintf(f, "%d\n", ss->eucoef.facen);
   for (AllShellsSolid(optr, dptr))
   {
      for (AllFacesShell(dptr, fptr))
      {
         fprintf(f, "\n");
         fe   = 0;
         eptr = efim = LooLEdg(FacFLOut(fptr));
         do
         {
            fe++;
         } while (efim != (eptr = HalNxt(eptr)));
         fprintf(f, "%d \n%d 1\n", FacFaceNo(fptr), fe);

         eptr = efim;
         do
         {
            fprintf(f, "%d ", FacFaceNo(LooLFace(HalWLoop(mate(eptr)))));
         } while (efim != (eptr = HalNxt(eptr)));

         fprintf(f, "\n");
         eptr = efim;
         do
         {
            fprintf(f, "%f ", EdgAngle(HalEdg(eptr)));
         } while (efim != (eptr = HalNxt(eptr)));

         fprintf(f, "\n%d \n", FacFaceNo(fptr));
      }
   }
   fclose(f);
   return(TRUE);
}
