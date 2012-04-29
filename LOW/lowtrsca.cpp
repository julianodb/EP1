/* 
  PROJETO USPDesigner
  MODULO: LOW (Operadores Basicos)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: lowtrsca.cpp
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

/* Scale solid by sx, sy, sz */
void MSD_execNameScale(void)
{
   char  name1[15];
   float tx, ty, tz;

   while (4 != sscanf(restbuf, "%s %f %f %f", name1, &tx, &ty, &tz))
   {
      printf("scale: name rx ry rz\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highNameScale(name1, tx, ty, tz);
}

int MSD_highNameScale(char *name, real tx, real ty, real tz)
{
   int sn;

   if ((sn = MSD_getSolidIdFromName(name)) == -1)
   {
      fprintf(stderr, MEN_NaoEncontrouSolidNome, NAM_ScaleSolid, name);
      return(ERROR);
   }
   return(MSD_highScale(sn, tx, ty, tz));
}

#ifdef __Extended_Version
void MSD_execScale(void)
{
   int   s;
   float sx, sy, sz;

   while (4 != sscanf(restbuf, "%d %f %f %f", &s, &sx, &sy, &sz))
   {
      printf("scale: Sid sx sy sz\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highScale(s, sx, sy, sz);
}

#endif

int MSD_highScale(Id sn, real sx, real sy, real sz)
{
   SPTYPE s;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_ScaleSolid, sn);
      return(ERROR);
   }
   CommandCount++;
   MSD_lowScale(s, sx, sy, sz);
   return(SUCCESS);
}

void MSD_lowScale(SPTYPE s, real sx, real sy, real sz)
{
   DPTYPE d;
   VPTYPE v;
   matrix m;
   int    GdoinversionOld;

   if ((comp(sx, 0.0, EPS) == 0) || (comp(sy, 0.0, EPS) == 0) ||
       (comp(sz, 0.0, EPS) == 0))
   {
      fprintf(stderr, MEN_FatorEscalaNulo, NAM_ScaleSolid);
      return;
   }

   if (Gdoinversion)
   {
      MSD_lowAddEulerOp(s, SCALE, 0, 0, 0, 0, 0, 0, 0, 0, 1.0 / sx, 1.0 / sy, 1.0 / sz, 0.0);
   }

   if (Gdodirect)
   {
      MSD_lowAddEulerOpRedo(s, SCALE, 0, 0, 0, 0, 0, 0, 0, 0, sx, sy, sz, 0.0);
   }

   matident(m);
   matscale(m, sx, sy, sz);
   for (AllShellsSolid(s, d))
   {
      for (AllVerticesShell(d, v))
      {
         vecmult(VerVCoord(v), VerVCoord(v), m);
      }
   }
   MSD_lowSetNormal(s, TRUE);
#ifdef __curved_env
   adjusttags(s, m);
#endif
   if (determ(m, 3) < 0.0)
   {
      GdoinversionOld = Gdoinversion;
      Gdoinversion    = 0;
      for (AllShellsSolid(s, d))
      {
         MSD_lowRevert(d);
      }
      Gdoinversion = GdoinversionOld;
   }
}

int MSD_execStructScale(EulerOp *op)
{
   Id   sid;
   real fd1, fd2, fd3;

   sid = op->solidno;
   fd1 = op->fp1;
   fd2 = op->fp2;
   fd3 = op->fp3;
   MSD_highScale(sid, fd1, fd2, fd3);
   return(SUCCESS);
}

void MSD_printStructScale(FILE *f, EulerOp *op)
{
   Id   sid;
   real fd1, fd2, fd3;
   int  com;

   sid = op->solidno;
   fd1 = op->fp1;
   fd2 = op->fp2;
   fd3 = op->fp3;
   com = op->CommandCount;
   fprintf(f, "scale %d %d %f %f %f \n", sid, com, fd1, fd2, fd3);
}

int MSD_readStructScale(FILE *f, EulerOp *op)
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

int MSD_modifyStructScale(EulerOp *op)
{
   int solidno;

   solidno     = MSD_lowCheckTranslatorTable(op->solidno, SOLID);
   op->solidno = solidno;
   return(SUCCESS);
}
