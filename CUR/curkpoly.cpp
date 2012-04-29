/* 
  PROJETO USPDesigner
  MODULO: CUR (Curve Manipulation)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: curkpoly.cpp
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
#include "vectorop.h"
#include "mancommd.h"
#include "curvgeom.h"

#ifdef __curved_env__

 #ifdef __Extended_Version
void MSD_execKillCurvePolyline(void)
{
   int  tag, sn;
   char sname[20];

   while (2 != sscanf(restbuf, "%s %d", sname, &tag))
   {
      printf("KILLPOLYLINE: sname tag \n");
      if (!lineins("? "))
      {
         return;
      }
   }
   if ((sn = MSD_getSolidIdFromName(sname)) == (ERROR))
   {
      fprintf(stderr, MEN_NaoEncontrouSolidNome, NAM_KillPolyLine, sname);
      return;
   }
   MSD_highKillCurvePolyline(sn, tag);
}

 #endif

int MSD_highKillCurvePolyline(Id sn, Id tag)
{
   SPTYPE s;
   CURVE  *curv;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_KillPolyLine, sn);
      return(ERROR);
   }
   if ((curv = MSD_getCurve(s, tag)) == (CURVE *)NIL)
   {
      fprintf(stderr, MEN_NaoEncontrouTag, NAM_KillPolyLine, tag, sn);
      return(ERROR);
   }
   MSD_lowKillCurvePolyline(s, curv);
   return(SUCCESS);
}

int MSD_lowKillCurvePolyline(SPTYPE s, CURVE *c)
{
   int Gdodirectsave;

   assert(c != (CURVE *)NIL);
   assert(s != SNIL);

   if (--c->times_used == 0)
   {
      if (Gdoinversion)
      {
         MSD_lowAddEulerOp(s, NEWPOLY, c->tagno, 0, 0, 0, 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0);
      }
      if (Gdodirect)
      {
         MSD_lowAddEulerOpRedo(s, KILLPOLY, c->tagno, 0, 0, 0, 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0);
      }

      Gdodirectsave = Gdodirect;
      Gdodirect     = FALSE;
      while (c->lpnt)
      {
         MSD_lowRemovePointFromCurve(s, c);
      }
      Gdodirect = Gdodirectsave;
      MSD_lowCurveDeleteElement(c, s);
   }
   return(SUCCESS);
}

int MSD_execStructKILLPOLY(EulerOp *op)
{
   return(MSD_highKillCurvePolyline(op->ip1, op->solidno));
}

void MSD_printStructKILLPOLY(FILE *f, EulerOp *op)
{
   Id ids, id1, id2, com;

   ids = op->solidno;
   id1 = op->ip1;
   id2 = op->ip2;
   com = op->CommandCount;
   fprintf(f, "killpoly %d %d %d %d\n", ids, com, id1, id2);
}

int MSD_readStructKILLPOLY(FILE *f, EulerOp *op)
{
   int solidno, ip1, ip2, com;

   if (fscanf(f, "%d %d %d %d\n", &solidno, &com, &ip1, &ip2) != 4)
   {
      return(ERROR);
   }
   op->solidno      = solidno;
   op->CommandCount = com;
   op->ip1          = ip1;
   op->ip2          = ip2;
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

int MSD_modifyStructKILLPOLY(EulerOp *op)
{
   return(SUCCESS);
}

#endif
