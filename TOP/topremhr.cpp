/* 
  PROJETO USPDesigner
  MODULO: TOP (Operadores de Nivel Intermediario)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: topremhr.cpp
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
#include "eulerops.h"
#include "mancommd.h"

/* Remove a solid from the workspace in a non-undoable way */
#ifdef __Extended_Version
void MSD_execHardRemove(void)
{
   int sn, op;

   while (sscanf(restbuf, "%d %d", &sn, &op) != 2)
   {
      printf("hardsolidrm: Sid");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highHardRemove(sn);
}

#endif

int MSD_highHardRemove(Id sn)
{
   SPTYPE s;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_HardRemove, sn);
      return(ERROR);
   }
   return(MSD_lowHardRemove(s));
}

int MSD_lowHardRemove(SPTYPE s)
{
   DPTYPE d;
   FPTYPE f;
   LPTYPE l;
   HPTYPE he;
   int    SetOld;

   SetOld   = SetTrace;
   SetTrace = FALSE;

   while ((d = SolSShells(s)) != DNIL)
   {
      while ((f = SheSFaces(d)) != FNIL)
      {
         while ((l = FacFLoops(f)) != LNIL)
         {
            he = LooLEdg(l);
            do
            {
               if (HalEdg(he) != ENIL)
               {
                  if (EdgHe1(HalEdg(he)) == he)
                  {
                     MSD_lowDelElement(EDGE, (NPTYPE)HalEdg(he), (NPTYPE)s);
                  }
                  if (VerVEdge(HalVtx(he)) == he)
                  {
                     MSD_lowDelElement(VERTEX, (NPTYPE)HalVtx(he), (NPTYPE)s);
                  }
               }
               else
               {
                  MSD_lowDelElement(VERTEX, (NPTYPE)HalVtx(he), (NPTYPE)s);
               }
               MSD_lowDelElement(HALFEDGE, (NPTYPE)he, NNIL);
            } while ((he = HalNxt(he)) != LooLEdg(l));
            MSD_lowDelElement(LOOP, (NPTYPE)l, (NPTYPE)f);
         }
         MSD_lowDelElement(FACE, (NPTYPE)f, (NPTYPE)d);
      }
      MSD_lowDelElement(SHELL, (NPTYPE)d, (NPTYPE)s);
   }
   MSD_lowDelElement(SOLID, (NPTYPE)s, NNIL);

   SetTrace = SetOld;
   return(SUCCESS);
}

int MSD_execStructHardRemove(EulerOp *op)
{
   Id sid;

   sid = op->solidno;
   return(MSD_highHardRemove(sid));
}

void MSD_printStructHardRemove(FILE *f, EulerOp *op)
{
   Id  sid;
   int com;

   sid = op->solidno;
   com = op->CommandCount;
   fprintf(f, "hardrem %d %d \n", sid, com);
}

int MSD_readStructHardRemove(FILE *f, EulerOp *op)
{
   int solidno, com;

   if (fscanf(f, "%d %d\n", &solidno, &com) != 2)
   {
      return(ERROR);
   }
   op->solidno      = solidno;
   op->CommandCount = com;
   op->ip1          = 0;
   op->ip2          = 0;
   op->ip3          = 0;
   op->ip4          = 0;
   op->ip5          = 0;
   op->ip6          = 0;
   op->ip7          = 0;
   op->ip8          = 0;
   op->fp1          = 0.0;
   op->fp2          = 0.0;
   op->fp3          = 0.0;
   op->fp4          = 0.0;
   return(SUCCESS);
}

int MSD_modifyStructHardRemove(EulerOp *op)
{
   int solidno;

   solidno     = MSD_lowCheckTranslatorTable(op->solidno, SOLID);
   op->solidno = solidno;
   return(SUCCESS);
}
