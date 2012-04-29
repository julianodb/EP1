/* 
  PROJETO USPDesigner
  MODULO: EUL (Operadores de Euler)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: eulmvsf_.cpp
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
#include "lowsolid.h"
#include "eulerops.h"
#include "mancommd.h"
/* Operator mvsf creates a new solid named sn that consists of one face */
/* f and one vertex v with coordinates x, y, and z. It returns a pointer*/
/* to the new solid created. Note that the operator mvsf is the only one*/
/* that has just one level of implementation: there are no separated    */
/* low-level and high-level operators.                                  */

#ifdef __Extended_Version
void MSD_execMVSF(void)
{
   int   s, f, v, d, op;
   float x, y, z;

   while (8 != sscanf(restbuf, "%d %d %d %d %d %f %f %f", &s, &op, &f, &v, &d,
                      &x, &y, &z))
   {
      printf("mvsf: Sid F V D X Y Z\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_lowMVSF(s, f, v, d, x, y, z);
}

#endif

SPTYPE MSD_highMVSF(Id fn, Id vn, Id dn, real xx, real yy, real zz)
{
   return(MSD_lowMVSF(MSD_getMaxSolidId(), fn, vn, dn, xx, yy, zz));
}

SPTYPE MSD_lowMVSF(Id sn, Id fn, Id vn, Id dn, real xx, real yy, real zz)
{
   SPTYPE newsolid;
   DPTYPE newshell;
   FPTYPE newface;
   LPTYPE newloop;
   HPTYPE newhe;
   VPTYPE newvertex;

   newsolid  = (SPTYPE)MSD_lowNewElement(SOLID, NNIL);
   newshell  = (DPTYPE)MSD_lowNewElement(SHELL, (NPTYPE)newsolid);
   newface   = (FPTYPE)MSD_lowNewElement(FACE, (NPTYPE)newshell);
   newloop   = (LPTYPE)MSD_lowNewElement(LOOP, (NPTYPE)newface);
   newhe     = (HPTYPE)MSD_lowNewElement(HALFEDGE, NNIL);
   newvertex = (VPTYPE)MSD_lowNewElement(VERTEX, (NPTYPE)newshell);

   SolSolidNo(newsolid)    = sn;
   SheShellNo(newshell)    = dn;
   FacFaceNo(newface)      = fn;
   FacFLOut(newface)       = newloop;
   LooLength(newloop)      = 0;
   LooLEdg(newloop)        = newhe;
   HalNxt(newhe)           = HalPrv(newhe) = newhe;
   HalEdg(newhe)           = ENIL;
   HalWLoop(newhe)         = newloop;
   HalVtx(newhe)           = newvertex;
   VerVertexNo(newvertex)  = vn;
   VerVCoord(newvertex)[0] = xx;
   VerVCoord(newvertex)[1] = yy;
   VerVCoord(newvertex)[2] = zz;
   VerVCoord(newvertex)[3] = 1.0;

   if (Gdoinversion)
   {
      MSD_lowAddEulerOp(newsolid, KVSF, fn, 0, 0, 0, 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0);
   }

   if (Gdodirect)
   {
      MSD_lowAddEulerOpRedo(newsolid, MVSF, fn, vn, dn, 0, 0, 0, 0, 0, xx, yy, zz, 0.0);
   }

   MSD_setEulCoef(newsolid, 1, 1, 0, 0, 1, 0);

   if (SetTrace)
   {
      if (TraceSolid(newsolid))
      {
         fprintf(stderr, MEN_MSDAbortou, NAM_MVSF);
         exit(1);
      }
   }

   return(newsolid);
}

int MSD_execStructMVSF(EulerOp *op)
{
   SPTYPE sh;
   int    bits;
   Id     sid, id1, id2, id3;
   real   fd1, fd2, fd3;

   sid  = op->solidno;
   id1  = op->ip1;
   id2  = op->ip2;
   id3  = op->ip3;
   fd1  = op->fp1;
   fd2  = op->fp2;
   fd3  = op->fp3;
   bits = op->ip4;

   sh           = MSD_lowMVSF(sid, id1, id2, id3, fd1, fd2, fd3);
   SolSBits(sh) = bits;
   return(SUCCESS);
}

void MSD_printStructMVSF(FILE *f, EulerOp *op)
{
   Id   sid, id1, id2, id3;
   real fd1, fd2, fd3;
   int  com;

   sid = op->solidno;
   id1 = op->ip1;
   id2 = op->ip2;
   id3 = op->ip3;
   fd1 = op->fp1;
   fd2 = op->fp2;
   fd3 = op->fp3;
   com = op->CommandCount;
   fprintf(f, "mvsf %d %d %d %d %d %f %f %f\n", sid,
           com, id1, id2, id3, fd1, fd2, fd3);
}

int MSD_readStructMVSF(FILE *f, EulerOp *op)
{
   int   solidno, ip1, ip2, ip3, com;
   float fp1, fp2, fp3;

   if (fscanf(f, "%d %d %d %d %d %f %f %f\n", &solidno, &com, &ip1, &ip2, &ip3,
              &fp1, &fp2, &fp3) != 8)
   {
      return(ERROR);
   }
   op->solidno      = solidno;
   op->CommandCount = com;
   op->ip1          = ip1;
   op->ip2          = ip2;
   op->ip3          = ip3;
   op->ip4          = 0;
   op->ip5          = 0;
   op->ip6          = 0;
   op->fp1          = fp1;
   op->fp2          = fp2;
   op->fp3          = fp3;
   op->fp4          = 0.0;
   return(SUCCESS);
}

int MSD_modifyStructMVSF(EulerOp *op)
{
   int solidno;

   solidno     = MSD_lowCheckTranslatorTable(op->solidno, SOLID);
   op->solidno = solidno;
   return(SUCCESS);
}
