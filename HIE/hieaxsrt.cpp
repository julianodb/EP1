/* 
  PROJETO USPDesigner
  MODULO: HIE (Estrutura Hierarquica)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: hieaxsrt.cpp
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
 #include <malloc.h>
#endif
#include "mensagem.h"
#include "memvirtu.h"
#include "lowparam.h"
#include "lowmacro.h"
#include "lowsolid.h"
#include "vectorop.h"
#include "disdispl.h"
#include "eulerops.h"
#include "mancommd.h"
#include "hiegroup.h"

#ifdef __Extended_Version
void MSD_execNameRotateAxis(void)
{
   char  aname[30];
   float cx, cy, cz, ax, ay, az, th;

   while (8 != sscanf(restbuf, "%s %f %f %f %f %f %f %f",
                      aname, &cx, &cy, &cz,
                      &ax, &ay, &az, &th))
   {
      printf("RotateAxis: axis cx cy cz ax ay az th\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highNameRotateAxis(aname, cx, cy, cz, ax, ay, az, th);
}

#endif

int MSD_highNameRotateAxis(char *name, real cx, real cy, real cz,
                           real ax, real ay, real az, real th)
{
   int an;

   if ((an = MSD_getAxisIdFromName(name)) == -1)
   {
      fprintf(stderr, MEN_NaoEncontrouAxisNome, NAM_RotateAxis, name);
      return(ERROR);
   }
   return(MSD_highRotateAxis(an, cx, cy, cz, ax, ay, az, th));
}

#ifdef __Extended_Version
void MSD_execRotateAxis(void)
{
   float cx, cy, cz, ax, ay, az, th;
   int   an;

   while (8 != sscanf(restbuf, "%d %f %f %f %f %f %f %f",
                      &an, &cx, &cy, &cz,
                      &ax, &ay, &az, &th))
   {
      printf("RotateAxis: an cx cy cz ax ay az th\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highRotateAxis(an, cx, cy, cz, ax, ay, az, th);
}

#endif

int MSD_highRotateAxis(int an, real cx, real cy, real cz,
                       real ax, real ay, real az, real th)
{
   APTYPE aptr;
   vector center, axis;

   if ((aptr = MSD_getAxis(an)) == ANIL)
   {
      fprintf(stderr, MEN_NaoEncontrouAxisId, NAM_RotateAxis, an);
      return(ERROR);
   }
   makevec(center, cx, cy, cz, 1.0);
   makevec(axis, ax, ay, az, 1.0);
   return(MSD_lowRotateAxis(aptr, center, axis, th));
}

int MSD_lowRotateAxis(APTYPE aptr, vector center, vector axis, real th)
{
   real   fd1, fd2, fd3, fd4;
   matrix rot;
   vector dis;
   Id     an;

   if (Gdoinversion)
   {
      an  = AxsAxisNo(aptr);
      fd1 = center[0];
      fd2 = center[1];
      fd3 = center[2];
      fd4 = center[3];
      MSD_lowAddEulerOp(SNIL, ROTAXIS, -1, 0, 0, 0, 0, 0, 0, 0,
                        fd1, fd2, fd3, fd4);
      fd1 = axis[0];
      fd2 = axis[1];
      fd3 = axis[2];
      fd4 = -th;
      MSD_lowAddEulerOp(SNIL, ROTAXIS, an, 0, 0, 0, 0, 0, 0, 0,
                        fd1, fd2, fd3, fd4);
   }

   rotmat(axis, th, rot);
   if (AxsMode(aptr) == AxisOfRotation)
   {
      vecmult(dis, center, rot);
      vecminus(dis, center, dis);
      vecmult(AxsCenter(aptr), AxsCenter(aptr), rot);
      vecplus(AxsCenter(aptr), dis, AxsCenter(aptr));
   }
   vecmult(AxsVector(aptr), AxsVector(aptr), rot);
   return(SUCCESS);
}

int MSD_execStructRotateAxis(EulerOp *op)
{
   EulerOp *op1;
   real    fd1, fd2, fd3, fd4, fd5, fd6, fd7;
   Id      an;

   an  = op->ip1;
   fd1 = op->fp1;
   fd2 = op->fp2;
   fd3 = op->fp3;
   fd7 = op->fp4;

#ifndef __VIRTUAL_MEM
   op1 = op->opnext;
#else
   op1 = EULEROPS(op->pp - 1);
#endif

   fd4 = op1->fp1;
   fd5 = op1->fp2;
   fd6 = op1->fp3;
   return(MSD_highRotateAxis(an, fd1, fd2, fd3, fd4, fd5, fd6, fd7));
}

void MSD_printStructRotateAxis(FILE *f, EulerOp *op)
{
   Id   sid, ip1;
   real fd1, fd2, fd3, fd4;

   sid = op->solidno;
   ip1 = op->ip1;
   fd1 = op->fp1;
   fd2 = op->fp2;
   fd3 = op->fp3;
   fd4 = op->fp4;
   fprintf(f, "rotaxs %d %d %f %f %f %f\n", sid, ip1, fd1, fd2, fd3, fd4);
}

int MSD_readStructRotateAxis(FILE *f, EulerOp *op)
{
   int  solidno, ip1;
   real fd1, fd2, fd3, fd4;

   if (fscanf(f, "%d %d %f %f %f\n", &solidno, &ip1,
              &fd1, &fd2, &fd3, &fd4) != 6)
   {
      return(ERROR);
   }
   op->solidno = solidno;
   op->ip1     = ip1;
   op->ip2     = 0;
   op->ip3     = 0;
   op->ip4     = 0;
   op->ip5     = 0;
   op->ip6     = 0;
   op->ip7     = 0;
   op->ip8     = 0;
   op->fp1     = fd1;
   op->fp2     = fd2;
   op->fp3     = fd3;
   op->fp4     = fd4;
   return(SUCCESS);
}

int MSD_modifyStructRotateAxis(EulerOp *op)
{
   int axisno;

   axisno  = MSD_lowCheckTranslatorTable(op->ip1, AXIS);
   op->ip1 = axisno;
   return(SUCCESS);
}
