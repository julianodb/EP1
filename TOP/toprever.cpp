/* 
  PROJETO USPDesigner
  MODULO: TOP (Operadores de Nivel Intermediario)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: toprever.cpp
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
#include "mancommd.h"

#ifdef __Extended_Version
void MSD_execRevert(void)
{
   int sn, dn, op;

   while (sscanf(restbuf, "%d %d %d", &sn, &op, &dn) != 3)
   {
      printf("revert: Sid Shell");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highRevert(sn, dn);
}

#endif

int MSD_highRevert(Id sn, Id dn)
{
   SPTYPE s;
   DPTYPE d;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_Revert, sn);
      return(ERROR);
   }
   if ((d = MSD_getShell(s, dn)) == DNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouShell, NAM_Revert, sn, dn);
      return(ERROR);
   }
   return(MSD_lowRevert(d));
}

int MSD_lowRevert(DPTYPE d)
{
   SPTYPE s;
   FPTYPE f;
   LPTYPE l;
   HPTYPE l3;
   HPTYPE l3nxt;
   VPTYPE prev;
   VPTYPE sav;
   Id     dn;

   s = SheSSolids(d);
   if (Gdoinversion)
   {
      dn = SheShellNo(d);
      MSD_lowAddEulerOp(s, REVERT, dn, 0, 0, 0, 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0);
   }
   if (Gdodirect)
   {
      dn = SheShellNo(d);
      MSD_lowAddEulerOpRedo(s, REVERT, dn, 0, 0, 0, 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0);
   }

   for (AllFacesShell(d, f))
   {
      for (AllLoopsFace(f, l))
      {
         l3 = LooLEdg(l);
         do
         {
            l3nxt      = HalNxt(l3);
            HalNxt(l3) = HalPrv(l3);
            HalPrv(l3) = l3nxt;
         } while ((l3 = l3nxt) != LooLEdg(l));
         prev = HalVtx(HalPrv(l3));
         do
         {
            sav                  = HalVtx(l3);
            HalVtx(l3)           = prev;
            VerVEdge(HalVtx(l3)) = l3;
            prev                 = sav;
         } while ((l3 = HalNxt(l3)) != LooLEdg(l));
      }
   }

#ifdef __curved_env
   /* for polyline edges, switch he1 and he2 */
   for (AllEdgesSolid(s, e))
   {
      if (polyltag(e->he1->curv))
      {
         l3     = e->he1;
         e->he1 = e->he2;
         e->he2 = l3;
      }
   }
#endif
   if (SetTrace)
   {
      if (TraceSolid(s))
      {
         fprintf(stderr, MEN_MSDAbortou, NAM_Revert);
         exit(1);
      }
   }
   return(SUCCESS);
}

int MSD_execStructRevert(EulerOp *op)
{
   return(MSD_highRevert(op->solidno, op->ip1));
}

void MSD_printStructRevert(FILE *f, EulerOp *op)
{
   fprintf(f, "revert %d %d %d\n", op->solidno, op->CommandCount, op->ip1);
}

int MSD_readStructRevert(FILE *f, EulerOp *op)
{
   int solidno, ip1, com;

   if (fscanf(f, "%d %d %d\n", &solidno, &com, &ip1) != 3)
   {
      return(ERROR);
   }
   op->solidno      = solidno;
   op->CommandCount = com;
   op->ip1          = ip1;
   op->ip2          = 0;
   op->ip3          = 0;
   op->ip4          = 0;
   op->ip5          = 0;
   op->ip6          = 0;
   op->fp1          = 0.0;
   op->fp2          = 0.0;
   op->fp3          = 0.0;
   op->fp4          = 0.0;
   return(SUCCESS);
}

int MSD_modifyStructRevert(EulerOp *op)
{
   int solidno;

   solidno     = MSD_lowCheckTranslatorTable(op->solidno, SOLID);
   op->solidno = solidno;
   return(SUCCESS);
}
