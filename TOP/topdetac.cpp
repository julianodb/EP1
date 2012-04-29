/* 
  PROJETO USPDesigner
  MODULO: TOP (Operadores de Nivel Intermediario)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: topdetac.cpp
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
void MSD_execDetach(void)
{
   int sn, sn1, dn, newd, addf, addv, op;

   while (sscanf(restbuf, "%d %d %d %d %d %d %d", &sn, &op, &sn1, &dn, &newd,
                 &addf, &addv) != 7)
   {
      printf("detach: Sid sn1 dn newd addf addv\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highDetach(sn, sn1, dn, newd, addf, addv);
}

#endif

int MSD_highDetach(Id sn, Id sn1, Id dn, Id newd, int addf, int addv)
{
   SPTYPE s;
   DPTYPE d;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_Detach, sn);
      return(ERROR);
   }
   if ((d = MSD_getShell(s, dn)) == DNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouShell, NAM_Detach, dn, sn);
      return(ERROR);
   }
   return(MSD_lowDetach(s, sn1, d, newd, addf, addv) != SNIL);
}

SPTYPE MSD_lowDetach(SPTYPE s, Id sn1, DPTYPE d, Id newd, int addf, int addv)
{
   SPTYPE s1;

   if ((s1 = MSD_getSolid(sn1)) == SNIL)
   {
      s1             = (SPTYPE)MSD_lowNewElement(SOLID, NNIL);
      SolSolidNo(s1) = sn1;
   }
   if (SheSSolids(d) == s1)
   {
      return(s1);
   }
   MSD_lowShellDetach(s, s1, d, newd, addf, addv);
   return(s1);
}

void MSD_lowShellDetach(SPTYPE s, SPTYPE s1, DPTYPE d, Id newd, int addf, int addv)
{
   FPTYPE f;
   LPTYPE l;
   EPTYPE e;
   VPTYPE v;
   Id     sn, dn, dd;
   int    facn, rngn, edgn, vtxn, holn;

   if (Gdoinversion)
   {
      sn = SolSolidNo(s);
      dn = SheShellNo(d);
      dd = newd == -1 ? SheShellNo(d) : newd;
      MSD_lowAddEulerOp(s1, DETACH, sn, dd, dn, -addf, -addv, 0, 0, 0,
                        0.0, 0.0, 0.0, 0.0);
   }

   if (Gdodirect)
   {
      sn = SolSolidNo(s1);
      dn = SheShellNo(d);
      MSD_lowAddEulerOpRedo(s, DETACH, sn, dn, newd, addf, addv, 0, 0, 0,
                            0.0, 0.0, 0.0, 0.0);
   }

   MSD_lowDelList(SHELL, (NPTYPE)d, (NPTYPE)s);
   MSD_lowAddList(SHELL, (NPTYPE)d, (NPTYPE)s1);
   if (newd != -1)
   {
      SheShellNo(d) = newd;
   }
   for (AllFacesShell(d, f))
   {
      FacFaceNo(f) += addf;
   }
   for (AllVerticesShell(d, v))
   {
      VerVertexNo(v) += addv;
   }

   if (SolSShells(s) == DNIL)
   {
      MSD_addEulCoef(s1, 1, SolEuCoef(s).facen, SolEuCoef(s).ringn,
                     SolEuCoef(s).edgen, SolEuCoef(s).vertexn,
                     SolEuCoef(s).holen);
      MSD_lowDelElement(SOLID, (NPTYPE)s, NNIL);

      if (SetTrace)
      {
         if (TraceSolid(s1))
         {
            fprintf(stderr, MEN_MSDAbortou, NAM_Detach);
            exit(1);
         }
      }
   }
   else
   {
      facn = rngn = edgn = vtxn = 0;
      for (AllFacesShell(d, f))
      {
         for (AllLoopsFace(f, l))
         {
            rngn++;
         }
         facn++;
      }
      rngn -= facn;
      for (AllEdgesShell(d, e))
      {
         edgn++;
      }
      for (AllVerticesShell(d, v))
      {
         vtxn++;
      }
      holn = facn + vtxn + 2 * (rngn - 1 - edgn);
      MSD_addEulCoef(s1, 1, facn, rngn, edgn, vtxn, holn);
      MSD_addEulCoef(s, -1, -facn, -rngn, -edgn, -vtxn, -holn);

      if (SetTrace)
      {
         if (TraceSolid(s1))
         {
            fprintf(stderr, MEN_MSDAbortou, NAM_Detach);
            exit(1);
         }
         if (TraceSolid(s))
         {
            fprintf(stderr, MEN_MSDAbortou, NAM_Detach);
            exit(1);
         }
      }
   }
}

int MSD_execStructDetach(EulerOp *op)
{
   Id sid, ip1, ip2, ip3, ip4, ip5;

   sid = op->solidno;
   ip1 = op->ip1;
   ip2 = op->ip2;
   ip3 = op->ip3;
   ip4 = op->ip4;
   ip5 = op->ip5;
   return(MSD_highDetach(sid, ip1, ip2, ip3, ip4, ip5));
}

void MSD_printStructDetach(FILE *f, EulerOp *op)
{
   Id  sid, ip1, ip2, ip3, ip4, ip5;
   int com;

   sid = op->solidno;
   ip1 = op->ip1;
   ip2 = op->ip2;
   ip3 = op->ip3;
   ip4 = op->ip4;
   ip5 = op->ip5;
   com = op->CommandCount;
   fprintf(f, "detach %d %d %d %d %d %d %d\n", sid, com, ip1, ip2, ip3, ip4, ip5);
}

int MSD_readStructDetach(FILE *f, EulerOp *op)
{
   int solidno, ip1, ip2, ip3, ip4, ip5, com;

   if (fscanf(f, "%d %d %d %d %d %d %d\n", &solidno, &com, &ip1, &ip2, &ip3,
              &ip4, &ip5) != 7)
   {
      return(ERROR);
   }
   op->solidno      = solidno;
   op->CommandCount = com;
   op->ip1          = ip1;
   op->ip2          = ip2;
   op->ip3          = ip3;
   op->ip4          = ip4;
   op->ip5          = ip5;
   op->ip6          = 0;
   op->ip7          = 0;
   op->ip8          = 0;
   op->fp1          = 0.0;
   op->fp2          = 0.0;
   op->fp3          = 0.0;
   op->fp4          = 0.0;
   return(SUCCESS);
}

int MSD_modifyStructDetach(EulerOp *op)
{
   int solidno1, solidno2;

   solidno1    = MSD_lowCheckTranslatorTable(op->solidno, SOLID);
   solidno2    = MSD_lowCheckTranslatorTable(op->ip1, SOLID);
   op->solidno = solidno1;
   op->ip1     = solidno2;
   return(SUCCESS);
}
