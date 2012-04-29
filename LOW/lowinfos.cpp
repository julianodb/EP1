/* 
  PROJETO USPDesigner
  MODULO: LOW (Operadores Basicos)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: lowinfos.cpp
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
#include "memvirtu.h"
#include "lowparam.h"
#include "lowmacro.h"
#include "lowsolid.h"
#include "genfunc_.h"

void MSD_lowSetInfo(SPTYPE optr)
{
   MSD_lowSetLoopEdge(optr);
   MSD_lowCountFaceEdges(optr);
}

void MSD_lowSetLoopEdge(SPTYPE optr)
{
   DPTYPE dptr;
   FPTYPE fptr;
   LPTYPE lptr;
   EPTYPE eptr;
   int    cont;

   cont = 1;
   for (AllShellsSolid(optr, dptr))
   {
      for (AllFacesShell(dptr, fptr))
      {
         LooLoopNo(FacFLOut(fptr)) = cont++;
         for (AllLoopsFace(fptr, lptr))
         {
            if (lptr != FacFLOut(fptr))
            {
               LooLoopNo(lptr) = cont++;
            }
         }
      }
   }
   cont = 1;
   for (AllShellsSolid(optr, dptr))
   {
      for (AllEdgesShell(dptr, eptr))
      {
         EdgEdgeNo(eptr) = cont++;
      }
   }
}

void MSD_lowCountFaceEdges(SPTYPE optr)
{
   DPTYPE dptr;
   FPTYPE fptr;
   LPTYPE lptr;
   HPTYPE efim;
   HPTYPE eptr;
   int    cont;

   for (AllShellsSolid(optr, dptr))
   {
      for (AllFacesShell(dptr, fptr))
      {
         for (AllLoopsFace(fptr, lptr))
         {
            cont = 0;
            efim = eptr = LooLEdg(FacFLOut(fptr));
            do
            {
               cont++;
            } while ((eptr = HalNxt(eptr)) != efim);
            LooLength(lptr) = cont;
         }
      }
   }
}
