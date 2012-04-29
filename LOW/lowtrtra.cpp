/* 
  PROJETO USPDesigner
  MODULO: LOW (Operadores Basicos)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: lowtrtra.cpp
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
#include "shpshape.h"
#include "vectorop.h"
#include "mancommd.h"

#ifdef __Extended_Version
void MSD_execNameTranslate(void)
{
   char  name[30];
   float tx, ty, tz;

   while (4 != sscanf(restbuf, "%s %f %f %f", name, &tx, &ty, &tz))
   {
      printf("trans: name rx ry rz\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highNameTranslate(name, tx, ty, tz);
}

#endif

int MSD_highNameTranslate(char *name, real tx, real ty, real tz)
{
   int sn;

   if ((sn = MSD_getSolidIdFromName(name)) == -1)
   {
      fprintf(stderr, MEN_NaoEncontrouSolidNome, NAM_TranslateSolid, name);
      return(ERROR);
   }
   return(MSD_highTranslate(sn, tx, ty, tz));
}

#ifdef __Extended_Version
void MSD_execTranslate(void)
{
   int   s, op;
   float tx, ty, tz;

   while (5 != sscanf(restbuf, "%d %d %f %f %f", &s, &op, &tx, &ty, &tz))
   {
      printf("trans: Sid rx ry rz\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highTranslate(s, tx, ty, tz);
}

#endif

int MSD_highTranslate(Id sn, real tx, real ty, real tz)
{
   SPTYPE s;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
//        fprintf(stderr, MEN_NaoEncontrouSolido, NAM_TranslateSolid, sn) ;
      return(ERROR);
   }
   CommandCount++;
   MSD_lowTranslate(s, tx, ty, tz);
   return(SUCCESS);
}

void MSD_lowTranslate(SPTYPE s, real tx, real ty, real tz)
{
   DPTYPE d;
   FPTYPE f;
   HPTYPE he;
   HPTYPE efim;
   VPTYPE v;
   vector pc;
   int    length;

#ifdef __curved_env__
   matrix m;
#endif

   /* store translation record into undo log */
   if (Gdoinversion)
   {
      MSD_lowAddEulerOp(s, TRANS, 0, 0, 0, 0, 0, 0, 0, 0, -tx, -ty, -tz, 0.0);
   }
   if (Gdodirect)
   {
      MSD_lowAddEulerOpRedo(s, TRANS, 0, 0, 0, 0, 0, 0, 0, 0, tx, ty, tz, 0.0);
   }

   for (AllShellsSolid(s, d))
   {
      for (AllVerticesShell(d, v))
      {
         VerVCoord(v)[0] += tx;
         VerVCoord(v)[1] += ty;
         VerVCoord(v)[2] += tz;
      }
   }

   /* update face equations & boxes (if evaluated) */
   MSD_lowSetNormal(s, TRUE);
   for (AllShellsSolid(s, d))
   {
      for (AllFacesShell(d, f))
      {
         veczer(pc);
         length = 0;
         he     = efim = LooLEdg(FacFLOut(f));
         do
         {
            vecplus(pc, pc, VerVCoord(HalVtx(he)));
            length++;
         } while ((he = HalNxt(he)) != efim);
         FacFeq(f)[3] = length != 0 ? -dot(pc, FacFeq(f)) / length : 0.0;
      }
   }

#ifdef __curved_env__
   matident(m);
   mattrans(m, tx, ty, tz);
//    adjusttags(s, m) ;
#endif
}

int MSD_execStructTranslate(EulerOp *op)
{
   Id   sid;
   real fd1, fd2, fd3;

   sid = op->solidno;
   fd1 = op->fp1;
   fd2 = op->fp2;
   fd3 = op->fp3;
   MSD_highTranslate(sid, fd1, fd2, fd3);
   return(SUCCESS);
}

void MSD_printStructTranslate(FILE *f, EulerOp *op)
{
   Id   sid;
   real fd1, fd2, fd3;
   int  com;

   sid = op->solidno;
   fd1 = op->fp1;
   fd2 = op->fp2;
   fd3 = op->fp3;
   com = op->CommandCount;
   fprintf(f, "trans %d %d %f %f %f \n", sid, com, fd1, fd2, fd3);
}

int MSD_readStructTranslate(FILE *f, EulerOp *op)
{
   int   solidno, com;
   float fp1, fp2, fp3;

   if (fscanf(f, "%d %d %f %f %f\n", &solidno, &com, &fp1, &fp2, &fp3) != 5)
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
   op->fp1          = fp1;
   op->fp2          = fp2;
   op->fp3          = fp3;
   op->fp4          = 0.0;
   return(SUCCESS);
}

int MSD_modifyStructTranslate(EulerOp *op)
{
   int solidno;

   solidno     = MSD_lowCheckTranslatorTable(op->solidno, SOLID);
   op->solidno = solidno;
   return(SUCCESS);
}
