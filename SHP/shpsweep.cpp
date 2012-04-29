/* 
  PROJETO USPDesigner
  MODULO: SHP (Criacao de Formas Primitivas)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: shpsweep.cpp
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
#include <string.h>
#include <math.h>
#include "memvirtu.h"
#include "lowparam.h"
#include "lowmacro.h"
#include "lowsolid.h"
#include "eulerops.h"
#include "vectorop.h"
#include "mancommd.h"
#include "shpshape.h"
#include "disdispl.h"
#include "genfunc_.h"
#include "mensagem.h"

#define LinePTYPE    LineTYPE *

typedef struct
{
   SPTYPE line;
   real   *ampl;
   real   *rot;
} LineTYPE;

LinePTYPE MSD_highSetLine(char *name2, int n);
void MSD_highNameMakeSWEEP(char *name1, int pn, XY *listvtx, LinePTYPE line);
void MSD_execMakeSWEEP(void);
Id MSD_highMakeSWEEP(int pn, XY *listvtx, LinePTYPE line);
SPTYPE MSD_lowMakeSWEEP(Id sn, int pn, XY *listvtx, LinePTYPE line);
int MSD_lowMakeSweepNew(FPTYPE fac, LinePTYPE line);
int MSD_lowMakeNewPoint(vector nc, vector oc, vector li, vector U, vector UpV, real den, real ampl, real rot);
void MSD_lowFaceCG(FPTYPE fac, vector cg);
real MSD_lowLoopCG(LPTYPE l, vector cg, vector norm);

void MSD_execNameMakeSweep(void)
{
   LinePTYPE line;
   XY        *listvtx;
   char      name1[15], name2[15];
   int       pn, ln;

   while (4 != sscanf(restbuf, "%s %s %d %d", name1, name2, &pn, &ln))
   {
      printf("Objeto Linha N_divSecao N_divLinha\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   if ((pn > 2) && (ln > 1) && setvtpi(&listvtx, pn) && ((line = MSD_highSetLine(name2, ln)) != NULL))
   {
      MSD_highNameMakeSWEEP(name1, pn, listvtx, line);
   }
}

LinePTYPE MSD_highSetLine(char *name, int n)
{
   LinePTYPE line;
   Id        sn;
   char      prompt[31];
   real      x, y, z;
   int       i, flag = 1;

   sn = MSD_getMaxSolidId();
   if ((line = (LinePTYPE)malloc(sizeof(LineTYPE))) == NULL)
   {
      return(NULL);
   }
   if ((line->ampl = (real *)malloc(n * sizeof(real))) == NULL)
   {
      free((void *)line);
      printf("\nErro na alocacao de amp[]\n"); /* mensagem provisoria */
      return(NULL);
   }
   if ((line->rot = (real *)malloc(n * sizeof(real))) == NULL)
   {
      free((void *)line->ampl);
      free((void *)line);
      printf("\nErro na alocacao de rot[]\n"); /* mensagem provisoria */
      return(NULL);
   }
   for (i = 0; i < n; i++)
   {
      sprintf(prompt, "X%d Y%d Z%d ? ", i, i, i);
      do
      {
         if (!lineins(prompt))
         {
            return(NULL);
         }
      } while (3 != sscanf(restbuf, "%f %f %f", &x, &y, &z));
      if (i == 0)
      {
         if ((line->line = MSD_lowMVSF(sn, 1, 1, 1, x, y, z)) == SNIL)
         {
            flag = 0;
         }
      }
      else
      if (MSD_highMEV(sn, 1, i, i + 1, x, y, z) == ERROR)
      {
         flag = 0;
      }
   }
   for (i = 0; i < n; i++)
   {
      sprintf(prompt, "A%d ? ", i);
      do
      {
         if (!lineins(prompt))
         {
            return(NULL);
         }
      } while (1 != sscanf(restbuf, "%f", line->ampl + i));
   }
   for (i = n - 1; i > 0; i--)
   {
      line->ampl[i] /= line->ampl[i - 1];
   }
   for (i = 0; i < n; i++)
   {
      sprintf(prompt, "R%d ? ", i);
      do
      {
         if (!lineins(prompt))
         {
            return(NULL);
         }
      } while (1 != sscanf(restbuf, "%f", line->rot + i));
      line->rot[i] *= PI / 180.0;
   }
   for (i = n - 1; i > 0; i--)
   {
      line->rot[i] -= line->rot[i - 1];
   }
   if (flag == 1)
   {
      MSD_lowSetNormal(line->line, TRUE);
   }
   if (flag == 0)
   {
      MSD_lowHardRemove(line->line);
   }
   strcpy(SolName(line->line), name);
   return(flag == 0 ? NULL : line);
}

void MSD_highNameMakeSWEEP(char *name, int pn, XY *listvtx, LinePTYPE line)
{
   SPTYPE optr;
   Id     sn;

   if (MSD_getSolidIdFromName(name) != -1)
   {
      fprintf(stderr, MEN_SolidoJaExiste, NAM_MakeSweep, name);
      return;
   }
   if ((sn = MSD_highMakeSWEEP(pn, listvtx, line)) != -1)
   {
      optr = MSD_getSolid(sn);
      strcpy(SolName(optr), name);
   }
}

Id MSD_highMakeSWEEP(int pn, XY *listvtx, LinePTYPE line)
{
   SPTYPE optr;
   Id     sn;

   sn = MSD_getMaxSolidId();
   if ((optr = MSD_lowMakeSWEEP(sn, pn, listvtx, line)) != SNIL)
   {
      MSD_lowDisplayMonitor('o', optr, GNIL);
      return(sn);
   }
   fprintf(stderr, MEN_ParametrosIncorretos, NAM_MakeSweep);
   return(-1);
}

SPTYPE MSD_lowMakeSWEEP(Id sn, int pn, XY *listvtx, LinePTYPE line)
{
   SPTYPE s;
   FPTYPE f;
   HPTYPE he;
   Id     face;
   int    flag = 1, GOld;
   real   a;
   vector li, lf, vec;

   GOld         = Gdoinversion;
   Gdoinversion = FALSE;
   if ((s = MSD_lowMakePolygon(sn, listvtx, pn, 1)) == SNIL)
   {
      flag = 0;
   }
   he = LooLEdg(FacFLoops(SheSFaces(SolSShells(line->line))));
   veccopy(li, VerVCoord(HalVtx(he)));
   veccopy(lf, VerVCoord(HalVtx(HalNxt(he))));
   vecminus(vec, lf, li);
   f = MSD_getFace(s, 1);
   if (FaceEqNOK(f))
   {
      MSD_lowFaceEq(FacFLOut(f), FacFeq(f));
   }
   if ((a = dot(FacFeq(f), vec)) == 0.0)
   {
      flag = 0;
   }
   face = a > 0.0 ? 1 : 2;
   if (MSD_lowMakeSweepNew(MSD_getFace(s, face), line) == ERROR)
   {
      flag = 0;
   }
   if (flag == 1)
   {
      MSD_lowSetNormal(s, TRUE);
      MSD_lowSetEdgeAngle(s);
      MSD_lowSetInfo(s);
   }
   if (flag == 0)
   {
      MSD_lowHardRemove(s);
   }
   if (flag == 1)
   {
      MSD_lowAddEulerOp(s, HARDREM, 0, 0, 0, 0, 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0);
   }
   Gdoinversion = GOld;
   return(flag == 0 ? SNIL : s);
}

int MSD_lowMakeSweepNew(FPTYPE fac, LinePTYPE line)
{
   LPTYPE l;
   HPTYPE first;
   HPTYPE scan;
   HPTYPE he;
   HPTYPE h;
   VPTYPE v;
   Id     maxvert, maxface;
   int    i = 0;
   real   den;
   vector dl, ls, li, lf, nc, CG, U, V, UpV;

   maxvert = MSD_getMaxVertexId(SheSSolids(FacFShell(fac)));
   maxface = MSD_getMaxFaceId(SheSSolids(FacFShell(fac)));
   MSD_lowFaceCG(fac, CG);
   he = LooLEdg(FacFLoops(SheSFaces(SolSShells(line->line))));
   veccopy(ls, VerVCoord(HalVtx(he)));
   vecminus(dl, CG, ls);
   h = he;
   do
   {
      vecplus(VerVCoord(HalVtx(h)), VerVCoord(HalVtx(h)), dl);
      h = HalNxt(h);
   } while (h != he);
   veccopy(li, VerVCoord(HalVtx(he)));
   veccopy(lf, VerVCoord(HalVtx(HalNxt(he))));
   normalize(vecminus(V, lf, li));
   do
   {
      veccopy(U, V);
      he = HalNxt(he), i++;
      veccopy(li, VerVCoord(HalVtx(he)));
      if (mate(he) != HalPrv(he))
      {
         veccopy(lf, VerVCoord(HalVtx(HalNxt(he))));
         normalize(vecminus(V, lf, li));
      }
      vecplus(UpV, U, V);
      if ((den = 1 + dot(U, V)) == 0.0)
      {
         return(ERROR);
      }
      for (AllLoopsFace(fac, l))
      {
         first = LooLEdg(l);
         scan  = HalNxt(first);
         v     = HalVtx(scan);
         if (MSD_lowMakeNewPoint(nc, VerVCoord(v), li, U, UpV, den, line->ampl[i], line->rot[i]) == ERROR)
         {
            return(ERROR);
         }
         MSD_lowMEV(scan, scan, ++maxvert, nc[0], nc[1], nc[2]);
         while (scan != first)
         {
            v = HalVtx(HalNxt(scan));
            if (MSD_lowMakeNewPoint(nc, VerVCoord(v), li, U, UpV, den, line->ampl[i], line->rot[i]) == ERROR)
            {
               return(ERROR);
            }
            MSD_lowMEV(HalNxt(scan), HalNxt(scan), ++maxvert, nc[0], nc[1], nc[2]);
            MSD_lowMEF(HalPrv(scan), HalNxt(HalNxt(scan)), ++maxface);
            scan = HalNxt(mate(HalNxt(scan)));
         }
         MSD_lowMEF(HalPrv(scan), HalNxt(HalNxt(scan)), ++maxface);
      }
   } while (mate(he) != HalPrv(he));
   return(SUCCESS);
}

int MSD_lowMakeNewPoint(vector nc, vector pc, vector li, vector U, vector UpV, real den, real ampl, real rot)
{
   real   alpha;
   vector aux1, aux2, dl, lmf, n;

   vecminus(lmf, li, pc);
   if ((alpha = dot(UpV, lmf) / den) < 0.0)
   {
      return(ERROR);
   }
   veccopy(n, UpV);
   normalize(n);
   vecesc(dl, U, alpha);
   vecplus(aux1, pc, dl);
   vecminus(aux1, aux1, li);
   vecesc(aux1, aux1, ampl);
   if (rot)
   {
      veccopy(aux2, aux1);
      vecesc(aux2, aux2, cos(rot));
      cross(aux1, aux1, n);
      printf("sfia");
      vecesc(aux1, aux1, sin(rot));
      vecplus(aux1, aux1, aux2);
   }
   vecplus(nc, li, aux1);
   return(SUCCESS);
}

/* Esta funcao calcula o CG de uma face */
void MSD_lowFaceCG(FPTYPE fac, vector cg)
{
   LPTYPE l;
   real   area = 0;
   vector gg;

   veczer(cg);
   if (FaceEqNOK(fac))
   {
      MSD_lowFaceEq(FacFLOut(fac), FacFeq(fac));
   }
   l = FacFLoops(fac);
   while (l != LNIL)
   {
      area += MSD_lowLoopCG(l, gg, FacFeq(fac));
      vecplus(cg, cg, gg);
      l = LooNextL(l);
   }
   vecesc(cg, cg, 1 / area);
   veczer(cg);   /* temporario */
}

/* Esta funcao calcula o CG de um loop MULTIPLICADO pela sua area */
/* e retorna a area do loop                                       */
real MSD_lowLoopCG(LPTYPE l, vector cg, vector norm)
{
   HPTYPE he;
   VPTYPE v1;
   VPTYPE v2;
   VPTYPE v3;
   real   area = 0, ar;
   vector aa, bb, cc, gg;

   veczer(cg);
   he = LooLEdg(l);
   v1 = HalVtx(he);
   he = HalNxt(HalNxt(he));
   do
   {
      v2 = HalVtx(HalPrv(he));
      v3 = HalVtx(he);
      vecminus(aa, VerVCoord(v2), VerVCoord(v1));
      vecminus(bb, VerVCoord(v3), VerVCoord(v1));
      cross(cc, aa, bb);
      ar    = 0.5 * dot(norm, cc);
      area += ar;
      veccopy(gg, VerVCoord(v1));
      vecplus(gg, gg, VerVCoord(v2));
      vecplus(gg, gg, VerVCoord(v3));
      vecesc(gg, gg, ar / 3);
      vecplus(cg, cg, gg);
   } while ((he = HalNxt(he)) != LooLEdg(l));
   return(area);
}
