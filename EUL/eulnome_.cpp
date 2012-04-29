/* 
  PROJETO USPDesigner
  MODULO: EUL (Operadores de Euler)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: eulnome_.cpp
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
#include <string.h>
#ifndef __UNIX_
 #include <stdlib.h>
#endif
#include "mensagem.h"
#include "memvirtu.h"
#include "lowparam.h"
#include "lowmacro.h"
#include "lowsolid.h"
#include "eulerops.h"
#include "genfunc_.h"
#include "curvgeom.h"
#include "mancommd.h"

#ifdef __Extended_Version
void MSD_execNome(void)
{
   int  s, cod, op;
   char nome[13];

   while (4 != sscanf(restbuf, "%d %d %d %s", &s, &op, &cod, nome))
   {
      printf("nome: Sid nome cod \n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highName(s, nome, cod);
}

#endif

int MSD_highName(Id sn, char *nome, int cod)
{
   SPTYPE s;
   GPTYPE g;
   APTYPE a;

   if (cod == SOLID)
   {
      if ((s = MSD_getSolid(sn)) == SNIL)
      {
         fprintf(stderr, MEN_NaoEncontrouSolido, NAM_Name, sn);
         return(ERROR);
      }
      MSD_lowSolidName(s, nome);
   }
   if (cod == GROUP)
   {
      if ((g = MSD_getGroup(sn)) == GNIL)
      {
         fprintf(stderr, MEN_NaoEncontrouGroupId, NAM_Name, sn);
         return(ERROR);
      }
      MSD_lowGroupName(g, nome);
   }
   if (cod == AXIS)
   {
      if ((a = MSD_getAxis(sn)) == ANIL)
      {
         fprintf(stderr, MEN_NaoEncontrouAxisId, NAM_Name, sn);
         return(ERROR);
      }
      MSD_lowAxisName(a, nome);
   }
   return(SUCCESS);
}

void MSD_lowSolidName(SPTYPE s, char *nome)
{
   int  sid;
   char name[13];

   sid = SolSolidNo(s);
   if (Gdoinversion)
   {
      strcpy(name, SolName(s));
      MSD_lowAddEulerOp(s, NAME, sid, -1, name[6], name[7],
                        name[8], name[9],
                        name[10], name[11], 0.0, 0.0, 0.0, 0.0);
      MSD_lowAddEulerOp(s, NAME, sid, SOLID, name[0], name[1],
                        name[2], name[3],
                        name[4], name[5], 0.0, 0.0, 0.0, 0.0);
   }
   if (Gdodirect)
   {
      MSD_lowAddEulerOpRedo(s, NAME, sid, -1, nome[6], nome[7],
                            nome[8], nome[9],
                            nome[10], nome[11], 0.0, 0.0, 0.0, 0.0);
      MSD_lowAddEulerOpRedo(s, NAME, sid, SOLID, nome[0], nome[1],
                            nome[2], nome[3],
                            nome[4], nome[5], 0.0, 0.0, 0.0, 0.0);
   }

   strcpy(SolName(s), nome);
}

void MSD_lowGroupName(GPTYPE g, char *nome)
{
   int  gid;
   char name[13];

   gid = GrpGroupNo(g);
   if (Gdoinversion)
   {
      strcpy(name, GrpName(g));
      MSD_lowAddEulerOp(SNIL, NAME, gid, -1, name[6], name[7],
                        name[8], name[9],
                        name[10], name[11], 0.0, 0.0, 0.0, 0.0);
      MSD_lowAddEulerOp(SNIL, NAME, gid, GROUP, name[0], name[1],
                        name[2], name[3],
                        name[4], name[5], 0.0, 0.0, 0.0, 0.0);
   }
   if (Gdodirect)
   {
      MSD_lowAddEulerOpRedo(SNIL, NAME, gid, -1, nome[6], nome[7],
                            nome[8], nome[9],
                            nome[10], nome[11], 0.0, 0.0, 0.0, 0.0);
      MSD_lowAddEulerOpRedo(SNIL, NAME, gid, GROUP, nome[0], nome[1],
                            nome[2], nome[3],
                            nome[4], nome[5], 0.0, 0.0, 0.0, 0.0);
   }
   strcpy(GrpName(g), nome);
}

void MSD_lowAxisName(APTYPE a, char *nome)
{
   int  aid;
   char name[13];

   if (Gdoinversion)
   {
      aid = AxsAxisNo(a);
      strcpy(name, AxsName(a));
      MSD_lowAddEulerOp(SNIL, NAME, aid, -1, name[6], name[7],
                        name[8], name[9],
                        name[10], name[11], 0.0, 0.0, 0.0, 0.0);
      MSD_lowAddEulerOp(SNIL, NAME, aid, AXIS, name[0], name[1],
                        name[2], name[3],
                        name[4], name[5], 0.0, 0.0, 0.0, 0.0);
   }
   if (Gdodirect)
   {
      MSD_lowAddEulerOpRedo(SNIL, NAME, aid, -1, nome[6], nome[7],
                            nome[8], nome[9],
                            nome[10], nome[11], 0.0, 0.0, 0.0, 0.0);
      MSD_lowAddEulerOpRedo(SNIL, NAME, aid, AXIS, nome[0], nome[1],
                            nome[2], nome[3],
                            nome[4], nome[5], 0.0, 0.0, 0.0, 0.0);
   }
   strcpy(AxsName(a), nome);
}

int MSD_execStructName(EulerOp *op)
{
   char     nome[13];
   PEulerOp eop;

   if (op->ip2 == -1)
   {
      return(TRUE);
   }
   nome[0] = (char)op->ip3;
   nome[1] = (char)op->ip4;
   nome[2] = (char)op->ip5;
   nome[3] = (char)op->ip6;
   nome[4] = (char)op->ip7;
   nome[5] = (char)op->ip8;

#ifndef __VIRTUAL_MEM
   eop = op;
#else
   eop = op->pp;
#endif
   EulerOpNext(eop);
   nome[6]  = (char)EulerIp3(eop);
   nome[7]  = (char)EulerIp4(eop);
   nome[8]  = (char)EulerIp5(eop);
   nome[9]  = (char)EulerIp6(eop);
   nome[10] = (char)EulerIp7(eop);
   nome[11] = (char)EulerIp8(eop);
   nome[12] = '\0';
   return(MSD_highName(op->ip1, nome, op->ip2));
}

void MSD_printStructName(FILE *f, EulerOp *op)
{
   static char nome[13];
   static Id   id2, id1;
   int         com;

   id1 = op->ip1;
   if (op->ip2 != -1)
   {
      id2     = op->ip2;
      nome[0] = op->ip3;
      nome[1] = op->ip4;
      nome[2] = op->ip5;
      nome[3] = op->ip6;
      nome[4] = op->ip7;
      nome[5] = op->ip8;
   }
   else
   {
      nome[6]  = op->ip3;
      nome[7]  = op->ip4;
      nome[8]  = op->ip5;
      nome[9]  = op->ip6;
      nome[10] = op->ip7;
      nome[11] = op->ip8;
      nome[12] = '\0';
      com      = op->CommandCount;
      fprintf(f, "name %d %d %d %s\n", id1, com, id2, nome);
   }
}

int MSD_readStructName(FILE *f, EulerOp *op)
{
   Id   ip1, ip2;
   char nome[13];
   int  com;

   if (fscanf(f, "%d %d %d %s\n", &ip1, &com, &ip2, nome) != 4)
   {
      return(ERROR);
   }
   op->solidno      = op->ip1 = ip1;
   op->CommandCount = com;
   op->ip2          = ip2;
   op->ip3          = nome[0];
   op->ip4          = nome[1];
   op->ip5          = nome[2];
   op->ip6          = nome[3];
   op->ip7          = nome[4];
   op->ip8          = nome[5];
   return(SUCCESS);
}

int MSD_modifyStructName(EulerOp *op)
{
   int solidno;

   if (op->ip2 == SOLID)
   {
      solidno     = MSD_lowCheckTranslatorTable(op->ip1, SOLID);
      op->solidno = op->ip1 = solidno;
   }
   if (op->ip2 == GROUP)
   {
      solidno = MSD_lowCheckTranslatorTable(op->ip1, GROUP);
      op->ip1 = solidno;
   }
   if (op->ip2 == AXIS)
   {
      solidno = MSD_lowCheckTranslatorTable(op->ip1, AXIS);
      op->ip1 = solidno;
   }
   return(SUCCESS);
}
