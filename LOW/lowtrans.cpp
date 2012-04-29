/* 
  PROJETO USPDesigner
  MODULO: LOW (Operadores Basicos)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: lowtrans.cpp
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
#include <stdlib.h>
#include "mensagem.h"
#include "memvirtu.h"
#include "lowparam.h"
#include "lowmacro.h"
#include "lowsolid.h"
#include "genfunc_.h"
#include "eulerops.h"
#include "shpshape.h"
#include "vectorop.h"
#include "mancommd.h"

/* Transform solid s by matrix m */
#ifdef __Extended_Version
void MSD_execNameTransformation(void)
{
   int    i, ip, ap, bp, op;
   float  x, y, z, w;
   char   name[14];
   matrix mat;

   for (ip = ap = 0; ip == 0;)
   {
      if ((bp = sscanf(restbuf, "%s &op", name, &op)) == 2)
      {
         for (i = 0; i < 4; ++i)
         {
            if (!lineins("? "))
            {
               return;
            }
            ap        = sscanf(restbuf, " %f %f %f %f", &x, &y, &z, &w);
            mat[i][0] = x;
            mat[i][1] = y;
            mat[i][2] = z;
            mat[i][3] = w;
            if (ap != 4)
            {
               break;
            }
         }
      }
      if ((bp == 1) && (ap == 4))
      {
         ip = 1;
      }
      if (ip == 0)
      {
         printf("transf: name \nTransformation Matrix\n");
         if (!lineins("? "))
         {
            return;
         }
      }
   }
   MSD_highNameTransformation(name, mat);
}

#endif

int MSD_highNameTransformation(char *name, matrix m)
{
   int sn;

   if ((sn = MSD_getSolidIdFromName(name)) == -1)
   {
      fprintf(stderr, MEN_NaoEncontrouSolidNome, NAM_TransformSolid, name);
      return(ERROR);
   }
   return(MSD_highTransformation(sn, m));
}

#ifdef __Extended_Version
void MSD_execTransformation(void)
{
   int    i, ip, ap, bp, sn, op;
   float  x, y, z, w;
   matrix mat;
   char   name[15];

   for (ip = ap = 0; ip == 0;)
   {
      if ((bp = sscanf(restbuf, "%d %d", &sn, &op)) == 2)
      {
         for (i = 0; i < 4; ++i)
         {
            if (!lineins("? "))
            {
               return;
            }
            ap        = sscanf(restbuf, "%s %d %d %d %f %f %f %f", name, &op, &op, &op, &x, &y, &z, &w);
            mat[i][0] = x;
            mat[i][1] = y;
            mat[i][2] = z;
            mat[i][3] = w;
            if (ap != 8)
            {
               break;
            }
         }
      }
      if ((bp == 2) && (ap == 8))
      {
         ip = 1;
      }
      if (ip == 0)
      {
         printf("transf: sid \nTransformation Matrix\n");
         if (!lineins("? "))
         {
            return;
         }
      }
   }
   MSD_highTransformation(sn, mat);
}

#endif

int MSD_highTransformation(int sn, matrix m)
{
   SPTYPE s;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_TransformSolid, sn);
      return(ERROR);
   }
   CommandCount++;
   MSD_lowTransformation(s, m);
   return(SUCCESS);
}

void MSD_lowTransformation(SPTYPE s, matrix m)
{
   DPTYPE d;
   VPTYPE v;
   matrix minv;
   int    GdoinversionOld;

   if (Gdoinversion)
   {
      invmat(m, minv, 4);
      MSD_lowAddMatrix(s, TRANSFORM, -1, -1, 0, 0, 0, 0, minv);
      MSD_lowAddEulerOp(s, TRANSFORM, 1, 1, 0, 0, 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0);
   }
   if (Gdodirect)
   {
      MSD_lowAddEulerOpRedo(s, TRANSFORM, 1, 1, 0, 0, 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0);
      MSD_lowAddMatrixRedo(s, TRANSFORM, -1, -1, 0, 0, 0, 0, m);
   }

   for (AllShellsSolid(s, d))
   {
      for (AllVerticesShell(d, v))
      {
         vecmult(VerVCoord(v), VerVCoord(v), m);
      }
   }
   MSD_lowSetNormal(s, TRUE);

#ifdef __curved_env__
//    adjusttags(s, m) ;
#endif

   if (determ(m, 3) < 0.0)
   {
      GdoinversionOld = Gdoinversion;
      Gdoinversion    = 0;
      for (AllShellsSolid(s, d))
      {
         MSD_lowRevert(d);
      }
      MSD_lowSetNormal(s, TRUE);
      MSD_lowSetEdgeAngle(s);
      Gdoinversion = GdoinversionOld;
   }
}

int MSD_execStructTransformation(EulerOp *op)
{
   Id     sn;
   matrix m;

   sn = op->solidno;
   matident(m);
   MSD_lowGetMatrix(op, m);
   MSD_highTransformation(sn, m);
   return(SUCCESS);
}

void MSD_printStructTransformation(FILE *f, EulerOp *op)
{
   Id   sid, ip1;
   real fd1, fd2, fd3, fd4;
   int  com;

   sid = op->solidno;
   ip1 = op->ip1;
   com = op->CommandCount;
   if (ip1 == 1)
   {
      fprintf(f, "transform %d %d %d 0.0 0.0 0.0 0.0\n", sid, com, ip1);
   }
   else
   {
      fd1 = op->fp1;
      fd2 = op->fp2;
      fd3 = op->fp3;
      fd4 = op->fp4;
      fprintf(f, "transform %d %d %d %f %f %f %f\n", sid, com, ip1,
              fd1, fd2, fd3, fd4);
   }
}

int MSD_readStructTransformation(FILE *f, EulerOp *op)
{
   int   solidno, ip1, com;
   float fd1, fd2, fd3, fd4;

   if (fscanf(f, "%d %d %d %f %f %f %f\n", &solidno, &com, &ip1, &fd1, &fd2,
              &fd3, &fd4) != 7)
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
   op->ip7          = 0;
   op->ip8          = 0;
   op->fp1          = fd1;
   op->fp2          = fd2;
   op->fp3          = fd3;
   op->fp4          = fd4;
   return(SUCCESS);
}

int MSD_modifyStructTransformation(EulerOp *op)
{
   int solidno;

   solidno     = MSD_lowCheckTranslatorTable(op->solidno, SOLID);
   op->solidno = solidno;
   return(SUCCESS);
}
