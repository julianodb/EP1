/* 
  PROJETO USPDesigner
  MODULO: ANA (Modulo de Analise)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: anaaproj.cpp
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
 #ifndef _Windows
  #include <graphics.h>
 #endif
 #include <conio.h>
#endif
#include <math.h>
#include "mensagem.h"
#include "memvirtu.h"
#include "lowparam.h"
#include "lowmacro.h"
#include "lowsolid.h"
#include "genfunc_.h"
#include "vectorop.h"
#include "eulerops.h"
#include "mancommd.h"
#include "analise_.h"
#include "basrov__.h"
#include "shpshape.h"
#include "sethmain.h"

double MSD_lowSolidAreaProjetada(SPTYPE s);

void MSD_execNameAreaProjetada(void)
{
   char  name1[12];
   float dx, dy, dz;
   real  area, dim;

   while (4 != sscanf(restbuf, "%s %f %f %f", name1, &dx, &dy, &dz))
   {
      printf("area projetada: Nome dx dy dz");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highNameAreaProjetada(name1, dx, dy, dz, &area, &dim);
   printf("solido %s tem area proj. %f e dim. princ. %f\n", name1, area, dim);
}

int MSD_highNameAreaProjetada(char *name, real dx, real dy, real dz, real *area, real *dim)
{
   int sn;

   if ((sn = MSD_getSolidIdFromName(name)) == -1)
   {
      fprintf(stderr, MEN_NaoEncontrouSolidNome, NAM_AreaProjetada, name);
      *area = *dim = 0.0;
      return(ERROR);
   }
   return(MSD_highAreaProjetada(sn, dx, dy, dz, area, dim));
}

int MSD_highAreaProjetada(Id sn, real dx, real dy, real dz, real *area, real *dim)
{
   SPTYPE s;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_AreaProjetada, sn);
      *area = *dim = 0.0;
      return(ERROR);
   }
   return(MSD_lowAreaProjetada(s, dx, dy, dz, area, dim, 1));
}

int MSD_lowAreaProjetada(SPTYPE s, real dx, real dy, real dz, real *area,
                         real *dim, int flag)
{
   SPTYPE sp;
   vector dir, normal;

   makevec(normal, 0.0, 0.0, 1.0, 0.0);
   makevec(dir, dx, dy, dz, 0.0);
   sp    = MSD_CriaListaDePoligonosCoplanares(s, dir);
   *area = MSD_lowSolidAreaProjetada(sp);
   *dim  = MSD_lowSolidDimensaoPrincipal(sp, normal);

   if (flag == 1)
   {
      MSD_highNameSoftRemove("st");
   }
   return(SUCCESS);
}

void MSD_lowMakeUnion(FPTYPE f, vector dir)
{
   XY     xy[30];
   LPTYPE l;
   LPTYPE FaFOtf;
   HPTYPE he1;
   HPTYPE he2;
   vector a;

/*
 *  char aa[5] = "p1";
 *  char bb[5] = "st";
 */
   int j;

   if (dot(dir, FacFeq(f)) < EPS)
   {
      return;
   }
   FaFOtf = FacFLOut(f);
   he1    = LooLEdg(FaFOtf);
   he2    = he1;
   MSD_lowRebatePontoNoPlano(VerVCoord(HalVtx(he1)), a, dir);
   xy[0].x = a[0];
   xy[0].y = a[1];
   he1     = HalNxt(he1);
   j       = 1;
   do
   {
      MSD_lowRebatePontoNoPlano(VerVCoord(HalVtx(he1)), a, dir);
      xy[j].x   = a[0];
      xy[j++].y = a[1];
   } while ((he1 = HalNxt(he1)) != he2);
   MSD_highNameMakePrism("p1", j, 100.0, xy);

/*
 *  MSD_highNameDisplay("cam", aa);
 */

   for (AllLoopsFace(f, l))
   {
      if (l == FaFOtf)
      {
         continue;
      }
      he1 = LooLEdg(l);
      he2 = he1;
      MSD_lowRebatePontoNoPlano(VerVCoord(HalVtx(he1)), a, dir);
      xy[0].x = a[0];
      xy[0].y = a[1];
      he1     = HalNxt(he1);
      j       = 1;
      do
      {
         MSD_lowRebatePontoNoPlano(VerVCoord(HalVtx(he1)), a, dir);
         xy[j].x = a[0];
         xy[j].y = a[1];
      } while ((he1 = HalNxt(he1)) != he2);
      MSD_highNameMakePrism("p2", j, 1.0, xy);
      MSD_highNameDifference("p1", "p2", "df");
      MSD_highNameRenameSolid("df", "p1");
   }

   if (MSD_getSolidIdFromName("st") == -1)
   {
      MSD_highNameRenameSolid("p1", "st");
   }
   else
   {
      MSD_highNameUnion("st", "p1", "un");
      MSD_highNameRenameSolid("un", "st");

/*
 *      g_clear();
 *      MSD_highNameDisplay("cam", bb);
 */
   }
}

void MSD_lowMoveFace(FPTYPE f, vector dir)
{
   FPTYPE f2;
   LPTYPE l;
   HPTYPE he;
   HPTYPE LoLEdl;

   if (FacFlag(f) == 0)
   {
      MSD_lowMakeUnion(f, dir);
   }
   FacFlag(f) = 2;

   l  = FacFLOut(f);
   he = LoLEdl = LooLEdg(l);
   do
   {
      f2 = LooLFace(HalWLoop(mate(he)));
      if (FacFlag(f2) == 0)
      {
         MSD_lowMakeUnion(f2, dir);
         FacFlag(f2) = 1;
      }
   } while ((he = HalNxt(he)) != LoLEdl);

   for (AllLoopsFace(f, l))
   {
      LoLEdl = LooLEdg(l);
      he     = LooLEdg(l);
      do
      {
         f2 = LooLFace(HalWLoop(mate(he)));
         if (FacFlag(f2) != 2)
         {
            MSD_lowMoveFace(f2, dir);
         }
      } while ((he = HalNxt(he)) != LoLEdl);
   }
}

SPTYPE MSD_CriaListaDePoligonosCoplanares(SPTYPE s, vector dir)
{
   DPTYPE dd;
   FPTYPE ff;

   for (AllShellsSolid(s, dd))
   {
      for (AllFacesShell(dd, ff))
      {
         FacFlag(ff) = 0;
      }
      MSD_lowMoveFace(SheSFaces(dd), dir);
   }
   return(MSD_lowFindSolidByName("st"));
}

/*calcula para onde o vetor normal ao plano de projecao esta mais
 * perpendicular*/
int MSD_lowMaximaDirecao(vector p)
{
   if ((MSD_lowModuloReal(p[0]) >= MSD_lowModuloReal(p[1])) &&
       (MSD_lowModuloReal(p[0]) >= MSD_lowModuloReal(p[2])))
   {
      return(0);
   }
   else
   {
      return(MSD_lowModuloReal(p[1]) >= MSD_lowModuloReal(p[2]) ? 1 : 2);
   }
}

/* Rebate o ponto do solido sobre o plano de projecao     */
/*  vector p2  - ponto a ser rebatido                     */
/*  vector pp  - ponto rebatido                           */
/*  vector dir - normal ao plano de projecao              */
void MSD_lowRebatePontoNoPlano(vector p2, vector pp, vector dir)
{
   real   raz, ang;
   vector dirz, axis;
   matrix mat;

   makevec(dirz, 0.0, 0.0, 1.0, 1.0);
   normalize(dirz);
   normalize(dir);
   cross(axis, dirz, dir);
   normalize(axis);
   ang = acos(dot(dirz, dir));
   rotmat(axis, -ang, mat);

   raz   = dot(p2, dir);
   pp[0] = p2[0] - raz * dir[0];
   pp[1] = p2[1] - raz * dir[1];
   pp[2] = p2[2] - raz * dir[2];
   pp[3] = 1.0;
   vecmult(pp, pp, mat);
}

real MSD_lowModuloReal(real num)
{
   return(num <= 0 ? -num : num);
}

double MSD_lowSolidAreaProjetada(SPTYPE s)
{
   DPTYPE d;
   FPTYPE f;
   double varea;
   vector normal;

   makevec(normal, 0.0, 0.0, 1.0, 0.0);
   varea = 0.0;
   for (AllShellsSolid(s, d))
   {
      for (AllFacesShell(d, f))
      {
         if (dot(normal, FacFeq(f)) > 0)
         {
            varea += MSD_lowFaceArea(f);
         }
      }
   }
   return(-varea);
}

double MSD_lowSolidDimensaoPrincipal(SPTYPE s, vector normal)
{
   DPTYPE d;
   FPTYPE f;
   HPTYPE he1;
   HPTYPE he2;
   double dim, dd;
   vector v1, v2;
   HPTYPE LLEFFLF;

   dim = 0.0;
   for (AllShellsSolid(s, d))
   {
      for (AllFacesShell(d, f))
      {
         if (dot(normal, FacFeq(f)) > 0)
         {
            he1 = LLEFFLF = LooLEdg(FacFLoops(f));
            do
            {
               he2 = LLEFFLF;
               do
               {
                  if (he1 != he2)
                  {
                     veccopy(v1, VerVCoord(HalVtx(he1)));
                     veccopy(v2, VerVCoord(HalVtx(he2)));
                     dd = vecd(v1, v2);
                     if (dd > dim)
                     {
                        dim = dd;
                     }
                  }
               } while ((he2 = HalNxt(he2)) != LLEFFLF);
            } while ((he1 = HalNxt(he1)) != LLEFFLF);
         }
      }
   }
   return(dim);
}
