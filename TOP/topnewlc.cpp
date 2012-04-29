/* 
  PROJETO USPDesigner
  MODULO: TOP (Operadores de Nivel Intermediario)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: topnewlc.cpp
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

#ifdef __Extended_Version
void MSD_execNewVertexLocation(void)
{
   int   sn, vn;
   float x, y, z;

   while (sscanf(restbuf, "%d %d %f %f %f", &sn, &vn, &x, &y, &z) != 5)
   {
      printf("newloc: Sid vn x y z");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highNewVertexLocation(sn, vn, x, y, z);
}

#endif

/* Change location of a vertex, generate inversion info */
int MSD_highNewVertexLocation(Id sn, Id vn, real x, real y, real z)
{
   SPTYPE s;
   DPTYPE d;
   VPTYPE v;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_NewLocation, sn);
      return(ERROR);
   }
   for (AllShellsSolid(s, d))
   {
      for (AllVerticesShell(d, v))
      {
         if (VerVertexNo(v) == vn)
         {
            MSD_lowNewVertexLocation(s, v, x, y, z);
            return(SUCCESS);
         }
      }
   }
   fprintf(stderr, MEN_NaoEncontrouVertice, NAM_NewLocation, vn);
   return(ERROR);
}

void MSD_lowNewVertexLocation(SPTYPE s, VPTYPE v, real x, real y, real z)
{
   if (Gdoinversion)
   {
      MSD_lowAddEulerOp(s, NEWLOC, VerVertexNo(v), 0, 0, 0, 0, 0, 0, 0,
                        VerVCoord(v)[0],
                        VerVCoord(v)[1],
                        VerVCoord(v)[2], 0.0);
   }
   if (Gdodirect)
   {
      MSD_lowAddEulerOpRedo(s, NEWLOC, VerVertexNo(v), 0, 0, 0, 0, 0, 0, 0,
                            x, y, z, 0.0);
   }

   VerVCoord(v)[0] = x;
   VerVCoord(v)[1] = y;
   VerVCoord(v)[2] = z;
   VerVCoord(v)[3] = 1.0;
}

int MSD_execStructNewVertexLocation(EulerOp *op)
{
   Id   sid, id1;
   real fd1, fd2, fd3;

   sid = op->solidno;
   id1 = op->ip1;
   fd1 = op->fp1;
   fd2 = op->fp2;
   fd3 = op->fp3;
   return(MSD_highNewVertexLocation(sid, id1, fd1, fd2, fd3));
}

void MSD_printStructNewVertexLocation(FILE *f, EulerOp *op)
{
   Id   sid, id1;
   real fd1, fd2, fd3;
   int  com;

   sid = op->solidno;
   id1 = op->ip1;
   fd1 = op->fp1;
   fd2 = op->fp2;
   fd3 = op->fp3;
   com = op->CommandCount;
   fprintf(f, "newloc %d %d %d %f %f %f\n", sid, com, id1, fd1, fd2, fd3);
}

int MSD_readStructNewVertexLocation(FILE *f, EulerOp *op)
{
   int   solidno, ip1, com;
   float fp1, fp2, fp3;

   if (fscanf(f, "%d %d %d %f %f %f\n", &solidno, &com, &ip1, &fp1, &fp2, &fp3) != 7)
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
   op->fp1          = fp1;
   op->fp2          = fp2;
   op->fp3          = fp3;
   op->fp4          = 0.0;
   return(SUCCESS);
}

int MSD_modifyStructNewVertexLocation(EulerOp *op)
{
   int solidno;

   solidno     = MSD_lowCheckTranslatorTable(op->solidno, SOLID);
   op->solidno = solidno;
   return(SUCCESS);
}
