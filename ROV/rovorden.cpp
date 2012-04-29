/* 
  PROJETO USPDesigner
  MODULO: ROV (Criacao de Formas Especificas)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: rovorden.cpp
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
 #include <malloc.h>
#endif
#include <math.h>
#include <stdlib.h>
#include "lowmacro.h"
#include "mensagem.h"
#include "memvirtu.h"
#include "lowparam.h"
#include "lowsolid.h"
#include "eulerops.h"
#include "vectorop.h"
#include "genfunc_.h"
#include "sethmain.h"
#include "mancommd.h"
#include "analise_.h"
#include "basrov__.h"
#include "gphgraph.h"
#include "disdispl.h"

char MSD_formatImpressao[] =
   "(%s %s %.2f %.2f MSD %s %s %s %s %s %s %s %s %.2f %.2f)\n";
int MSD_lowOrdena(char *nome, real raio, real dx, real dy, real dz);
int MSD_lowAreaProjetadaTotal(real dx, real dy, real dz, real *area, real *dim);
double MSD_lowSolidAreaProjetada(SPTYPE s);


void MSD_execProjetadaTotal(void)
{
   float dx, dy, dz;
   real  area, dim;

   while (3 != sscanf(restbuf, "%f %f %f ", &dx, &dy, &dz))
   {
      printf("ROV.projetada total  dx dy dz\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_lowAreaProjetadaTotal(dx, dy, dz, &area, &dim);
   printf(" area = %f dim = %f\n", area, dim);
}

void MSD_execOrdena(void)
{
   float dx, dy, dz, raio;
   char  nome[15];
   int   i;

   while (5 != sscanf(restbuf, "%s %f %f %f %f", nome, &raio, &dx, &dy, &dz))
   {
      printf("ROV.ordena  nomearq raio dx dy dz\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_lowOrdena(nome, raio, dx, dy, dz);
   for (i = 0; i < NumListaEscoamento; i++)
   {
      printf("..Solido %2d = %s\n", i, SolName(ListaEscoamento[i].optr));
      printf("..Solido Id = %2d dist = %10.4f\n",
             SolSolidNo(ListaEscoamento[i].optr),
             ListaEscoamento[i].param);
   }
}

int MSD_lowOrdenaListaEstudo(void)
{
   SPTYPE optr;
   vector vec;
   real   temp;
   int    sorted = FALSE, j;

   while (!sorted)
   {
      sorted = TRUE;                     /* assume list is sorted         */
      for (j = 0; j < NumListaEscoamento - 1; j++)
      {
         if (ListaEscoamento[j].param < ListaEscoamento[j + 1].param)
         {
            optr = ListaEscoamento[j].optr;
            ListaEscoamento[j].optr     = ListaEscoamento[j + 1].optr;
            ListaEscoamento[j + 1].optr = optr;
            temp = ListaEscoamento[j].param;
            ListaEscoamento[j].param     = ListaEscoamento[j + 1].param;
            ListaEscoamento[j + 1].param = temp;
            veccopy(vec, ListaEscoamento[j].cg);
            veccopy(ListaEscoamento[j].cg, ListaEscoamento[j + 1].cg);
            veccopy(ListaEscoamento[j + 1].cg, vec);
            sorted = FALSE;
         }
      }
   }
   return(TRUE);
}

int MSD_lowCriaListaEstudo(real dx, real dy, real dz)
{
   SPTYPE optr;
   vector dir, cg, ref;
   int    cont;

   cont = 0;
   for (AllSolids(optr))
   {
      if ((SolPerfil(optr) != -1) || (SolType(optr) == SOLIDO_TIPO_SOLID))
      {
         cont++;
      }
   }

   if (NumListaEscoamento != 0)
   {
      free((char *)ListaEscoamento);
   }
   if ((ListaEscoamento = (LOPType *)malloc(cont * sizeof(LOPType))) ==
       (LOPType *)NIL)
   {
      fprintf(stderr, MEN_NaoPossuiMemoria, NAM_RovOrdena);
      return(ERROR);
   }
   NumListaEscoamento = cont;
   makevec(dir, dx, dy, dz, 1.0);
   makevec(ref, 0.0, 0.0, 0.0, 1.0);

   cont = 0;
   for (AllSolids(optr))
   {
      if ((SolPerfil(optr) == -1) && (SolType(optr) != SOLIDO_TIPO_SOLID))
      {
         continue;
      }
      MSD_lowCentroDeGravidade(optr, cg);
      veccopy(ListaEscoamento[cont].cg, cg);
      ListaEscoamento[cont].optr    = optr;
      ListaEscoamento[cont++].param =
         MSD_determinaDistanciaParametrica(ref, dir, cg);
   }
   return(TRUE);
}

int MSD_lowSetVariaveisImpressao(int i, int j, char *TSolido, char *TFronteira,
                                 char *TGeometria, char *TAtitude,
                                 real dx, real dy, real dz, int flag)
{
   vector dir, dirp1, dirp2;

   switch (SolType(ListaEscoamento[i].optr))
   {
       case SOLIDO_TIPO_BARRA:
       strcpy(TSolido, "BARRA");
       strcat(TSolido, flag == 1 ? "-V" : "-R");

       makevec(dir, dx, dy, dz, 1.0);

       veccopy(dirp1, SolPrincDir(ListaEscoamento[i].optr));
       MSD_lowRebatePontoNoPlano(dirp1, dirp1, dir);
       veccopy(dirp2, SolPrincDir(ListaEscoamento[j].optr));
       MSD_lowRebatePontoNoPlano(dirp2, dirp2, dir);
       cross(dir, dirp2, dirp1);
       if (vecnull(dir, EPS))
       {
          strcpy(TAtitude, "NORMAL");
          strcat(TAtitude, flag == 1 ? "-V" : "-R");
       }
       else
       {
          strcpy(TAtitude, "OBLIQUA");
          strcat(TAtitude, flag == 1 ? "-V" : "-R");
       }

       switch (SolGeometria(ListaEscoamento[i].optr))
       {
           case BARRA_GEOM_CIRCULO:
           strcpy(TGeometria, "CIRCULO");
           strcat(TGeometria, flag == 1 ? "-V" : "-R");
           strcpy(TFronteira, "ARREDONDADO");
           strcat(TFronteira, flag == 1 ? "-V" : "-R");
           break;

           case BARRA_GEOM_QUADRADO:
           strcpy(TGeometria, "QUADRADO");
           strcat(TGeometria, flag == 1 ? "-V" : "-R");
           strcpy(TFronteira, "CANTOS-VIVOS");
           strcat(TFronteira, flag == 1 ? "-V" : "-R");
           break;

           case BARRA_GEOM_TRIANGULO:
           strcpy(TGeometria, "TRIANGULO");
           strcat(TGeometria, flag == 1 ? "-V" : "-R");
           strcpy(TFronteira, "CANTOS-VIVOS");
           strcat(TFronteira, flag == 1 ? "-V" : "-R");
           break;

           case BARRA_GEOM_RETANGULO:
           strcpy(TGeometria, "RETANGULO");
           strcat(TGeometria, flag == 1 ? "-V" : "-R");
           strcpy(TFronteira, "CANTOS-VIVOS");
           strcat(TFronteira, flag == 1 ? "-V" : "-R");
           break;

           case BARRA_GEOM_ELIPSE:
           strcpy(TGeometria, "ELIPSE");
           strcat(TGeometria, flag == 1 ? "-V" : "-R");
           strcpy(TFronteira, "ARREDONDADO");
           strcat(TFronteira, flag == 1 ? "-V" : "-R");
           break;

           case BARRA_GEOM_PERFIL_L:
           strcpy(TGeometria, "PERFIL-L");
           strcat(TGeometria, flag == 1 ? "-V" : "-R");
           strcpy(TFronteira, "CANTOS-VIVOS");
           strcat(TFronteira, flag == 1 ? "-V" : "-R");
           break;

           case BARRA_GEOM_PERFIL_T:
           strcpy(TGeometria, "PERFIL-T");
           strcat(TGeometria, flag == 1 ? "-V" : "-R");
           strcpy(TFronteira, "CANTOS-VIVOS");
           strcat(TFronteira, flag == 1 ? "-V" : "-R");
           break;

           case BARRA_GEOM_PERFIL_I:
           strcpy(TGeometria, "PERFIL-I");
           strcat(TGeometria, flag == 1 ? "-V" : "-R");
           strcpy(TFronteira, "CANTOS-VIVOS");
           strcat(TFronteira, flag == 1 ? "-V" : "-R");
           break;

           default:
           return(ERROR);
       }
       break;

       case SOLIDO_TIPO_SOLID:
       strcpy(TSolido, "SOLIDO");
       strcat(TSolido, flag == 1 ? "-V" : "-R");
       strcpy(TAtitude, "");

       switch (SolGeometria(ListaEscoamento[i].optr))
       {
           case SOLIDO_GEOM_CUBO:
           strcpy(TGeometria, "CUBO");
           strcat(TGeometria, flag == 1 ? "-V" : "-R");
           strcpy(TFronteira, "CANTOS-VIVOS");
           strcat(TFronteira, flag == 1 ? "-V" : "-R");
           break;

           case SOLIDO_GEOM_CONE:
           strcpy(TGeometria, "CONE");
           strcat(TGeometria, flag == 1 ? "-V" : "-R");
           makevec(dir, dx, dy, dz, 1.0);
           cross(dir, dir, SolPrincDir(ListaEscoamento[i].optr));
           if (comp(dot(dir, dir), 0.0, 0.01) == 0)
           {
              strcpy(TFronteira, "CANTOS-VIVOS");
              strcat(TFronteira, flag == 1 ? "-V" : "-R");
           }
           else
           {
              strcpy(TFronteira, "ARREDONDADO");
              strcat(TFronteira, flag == 1 ? "-V" : "-R");
           }
           break;

           case SOLIDO_GEOM_ESFERA:
           strcpy(TGeometria, "ESFERA");
           strcat(TGeometria, flag == 1 ? "-V" : "-R");
           strcpy(TFronteira, "ARREDONDADO");
           strcat(TFronteira, flag == 1 ? "-V" : "-R");
           break;

           case SOLIDO_GEOM_PRISMA:
           strcpy(TGeometria, "PRISMA");
           strcat(TGeometria, flag == 1 ? "-V" : "-R");
           strcpy(TFronteira, "CANTOS-VIVOS");
           strcat(TFronteira, flag == 1 ? "-V" : "-R");
           break;

           case SOLIDO_GEOM_PIRAMIDE:
           strcpy(TGeometria, "PIRAMIDE");
           strcat(TGeometria, flag == 1 ? "-V" : "-R");
           strcpy(TFronteira, "CANTOS-VIVOS");
           strcat(TFronteira, flag == 1 ? "-V" : "-R");
           break;

           default:
           break;
       }
       break;

       case SOLIDO_TIPO_PLACA:
       strcpy(TSolido, "PLACA");
       strcat(TSolido, flag == 1 ? "-V" : "-R");

       makevec(dir, dx, dy, dz, 1.0);
       if (comp(dot(dir, SolPrincDir(ListaEscoamento[i].optr)), 0.0, 0.01) == 0)
       {
          strcpy(TAtitude, "TANGENTE");
          strcat(TAtitude, flag == 1 ? "-V" : "-R");
       }
       else
       {
          strcpy(TAtitude, "NORMAL");
          strcat(TAtitude, flag == 1 ? "-V" : "-R");
       }
       strcpy(TGeometria, "");
       strcpy(TFronteira, "CANTOS-VIVOS");
       strcat(TFronteira, flag == 1 ? "-V" : "-R");
       break;

       default:
       return(ERROR);
   }
   return(TRUE);
}

int MSD_lowCalculaDimensoesRelativas(int i, int j, real *LD, real *TD,
                                     real dx, real dy, real dz)
{
   real   L, T, D, param;
   vector dir, dir_L;

   makevec(dir, dx, dy, dz, 1.0);
   param = MSD_determinaDistanciaParametrica(ListaEscoamento[j].cg, dir,
                                             ListaEscoamento[i].cg);
   vecesc(dir_L, dir, param);
   L = sqrt(dot(dir_L, dir_L));

   vecplus(dir_L, dir_L, ListaEscoamento[j].cg);
   vecminus(dir_L, dir_L, ListaEscoamento[i].cg);
   T = sqrt(dot(dir_L, dir_L));

   D   = (ListaEscoamento[i].D + ListaEscoamento[j].D) * 0.5;
   *LD = L / D;
   *TD = T / D;
   return(TRUE);
}

int MSD_lowImprimeListaEstudo(char *nome, real raio, real dx, real dy, real dz)
{
   SPTYPE sp;
   char   nome1[20], nome2[20];
   char   TSolidoInterfere[20];
   char   TSolidoEstudo[20];
   char   TFronteiraInterfere[20];
   char   TFronteiraEstudo[20];
   char   TGeometriaInterfere[20];
   char   TGeometriaEstudo[20];
   char   TAtitudeInterfere[20];
   char   TAtitudeEstudo[20];
   real   LD, TD, area, dim, alfa, beta, gama;
   FILE   *fp, *fpin;
   vector dir, axs;
   int    i, j, flag, linecount = 0;

   if ((fp = fopen("lista.tmp", "w")) == (FILE *)NIL)
   {
      fprintf(stderr, "Nao conseguiu abrir arquivo lista.tmp!\n");
      return(ERROR);
   }

   for (i = 0; i < NumListaEscoamento; i++)
   {
      flag = 0;
      if (SolType(ListaEscoamento[i].optr) == SOLIDO_TIPO_BARRA)
      {
         makevec(dir, dx, dy, dz, 1.0);
         if ((sp = MSD_getSolid(SolPerfil(ListaEscoamento[i].optr))) == SNIL)
         {
            fprintf(stderr, "Nao encontrou o perfil\n");
            exit(1);
         }
         cross(dir, FacFeq(MSD_getFace(sp, 1)), dir);
         if (vecnull(dir, EPS))
         {
            flag = 1;
         }
      }
      if (flag == 1)
      {
         continue;
      }

      for (flag = j = 0; j < NumListaEscoamento; j++)
      {
         /*
          *         fprintf(stderr, "caso %2d - %2d\n", i, j) ;
          */
         if (i == j)
         {
            continue;
         }
         if (comp(ListaEscoamento[i].param, ListaEscoamento[j].param, 0.01) == -1)
         {
            continue;
         }
         if (vecd(ListaEscoamento[i].cg, ListaEscoamento[j].cg) >
             raio * ListaEscoamento[i].D)
         {
            continue;
         }

         MSD_lowSetVariaveisImpressao(i, j, TSolidoEstudo, TFronteiraEstudo,
                                      TGeometriaEstudo, TAtitudeEstudo, dx, dy, dz, 0);

         MSD_lowCalculaDimensoesRelativas(i, j, &LD, &TD, dx, dy, dz);
         MSD_lowSetVariaveisImpressao(j, i, TSolidoInterfere, TFronteiraInterfere,
                                      TGeometriaInterfere, TAtitudeInterfere,
                                      dx, dy, dz, 1);
         strcpy(nome1, SolName(ListaEscoamento[i].optr));
         strcpy(nome2, SolName(ListaEscoamento[j].optr));
         fprintf(fp, MSD_formatImpressao, nome1, nome2,
                 ListaEscoamento[i].area,
                 ListaEscoamento[i].H_D,
                 TSolidoInterfere, TFronteiraInterfere,
                 TGeometriaInterfere, TAtitudeInterfere,
                 TSolidoEstudo, TFronteiraEstudo,
                 TGeometriaEstudo, TAtitudeEstudo,
                 LD, TD);
         linecount++;
         flag = 1;
      }
      if (flag == 0)
      {
         MSD_lowSetVariaveisImpressao(i, i, TSolidoEstudo, TFronteiraEstudo,
                                      TGeometriaEstudo, TAtitudeEstudo, dx, dy, dz, 0);

         fprintf(fp, "(%s %s %.2f %.2f MSD SOZINHO %s %s %s %s)\n",
                 SolName(ListaEscoamento[i].optr),
                 SolName(ListaEscoamento[i].optr),
                 ListaEscoamento[i].area,
                 ListaEscoamento[i].H_D,
                 TSolidoEstudo, TFronteiraEstudo,
                 TGeometriaEstudo, TAtitudeEstudo);
         linecount++;
      }
   }
   fclose(fp);

   area = 0;
   dim  = 0;

   if ((fp = fopen(nome, "w")) == (FILE *)NIL)
   {
      fprintf(stderr, "Nao conseguiu abrir arquivo %s!\n", nome);
      return(ERROR);
   }
   if ((fpin = fopen("lista.tmp", "r")) == (FILE *)NIL)
   {
      fprintf(stderr, "Nao conseguiu abrir arquivo lista.tmp!\n");
      return(ERROR);
   }
   fprintf(fp, "%d\n", linecount);

   makevec(dir, dx, dy, 0.0, 1.0);
   normalize(dir);
   makevec(axs, 1.0, 0.0, 0.0, 1.0);
   alfa = acos(dot(dir, axs)) * 180.0 / 3.1416;
   makevec(dir, 0.0, dy, dz, 1.0);
   normalize(dir);
   makevec(axs, 0.0, 1.0, 0.0, 1.0);
   beta = acos(dot(dir, axs)) * 180.0 / 3.1416;
   makevec(dir, dx, 0.0, dz, 1.0);
   normalize(dir);
   makevec(axs, 0.0, 0.0, 1.0, 1.0);
   gama = acos(dot(dir, axs)) * 180.0 / 3.1416;

   fprintf(fp, "%.2f %.2f %.2f\n", alfa, beta, gama);
   fprintf(fp, "%.2f %.2f\n", area, dim);

   while (!feof(fpin))
   {
      fputc(fgetc(fpin), fp);
   }

   fclose(fpin);
   fclose(fp);
   remove("lista.tmp");

   return(TRUE);
}

int MSD_CalculaParametrosInterferencia(real dx, real dy, real dz)
{
   int    i;
   real   H, D, area, dim;
   vector dir;

   for (i = 0; i < NumListaEscoamento; i++)
   {
      /* Calcula Area Projetada */
      MSD_lowAreaProjetada(ListaEscoamento[i].optr, dx, dy, dz, &area, &dim, 1);
      ListaEscoamento[i].area = area;

      /* Calcula H_D */
      makevec(dir, dx, dy, dz, 1.0);
      D = MSD_determinaDimensaoPrincipal(ListaEscoamento[i].optr, dir, dim);
      H = MSD_determinaMaiorDimensao(ListaEscoamento[i].optr);
      ListaEscoamento[i].D   = D;
      ListaEscoamento[i].H_D = H / D;
   }
   return(TRUE);
}

int MSD_lowAreaProjetadaTotal(real dx, real dy, real dz, real *area, real *dim)
{
   SPTYPE optr;
   SPTYPE sp;
   real   sa, sd;
   vector normal;
   char   aa[5] = "st";
   char   bb[5] = "er";

   makevec(normal, 0.0, 0.0, 1.0, 0.0);
   for (AllSolids(optr))
   {
      if ((SolPerfil(optr) != -1) || (SolType(optr) == SOLIDO_TIPO_SOLID))
      {
         MSD_lowAreaProjetada(optr, dx, dy, dz, &sa, &sd, 0);

         MSD_highNameDisplay("cam", aa);
         fprintf(stderr, "--> solid %s\n", SolName(optr));
         if (MSD_getSolidIdFromName("er") == -1)
         {
            MSD_highNameRenameSolid("st", "er");
         }
         else
         {
            MSD_highNameUnion("st", "er", "un");
            MSD_highNameRenameSolid("un", "er");
         }
         g_clear();
         MSD_highNameDisplay("cam", bb);
      }
   }

   sp    = MSD_lowFindSolidByName("er");
   *area = MSD_lowSolidAreaProjetada(sp);
   *dim  = MSD_lowSolidDimensaoPrincipal(sp, normal);
   MSD_highNameSoftRemove("er");
   return(TRUE);
}

int MSD_lowOrdena(char *nome, real raio, real dx, real dy, real dz)
{
   if (MSD_lowCriaListaEstudo(dx, dy, dz) == ERROR)
   {
      return(ERROR);
   }
   MSD_lowOrdenaListaEstudo();
   MSD_CalculaParametrosInterferencia(dx, dy, dz);
   MSD_lowImprimeListaEstudo(nome, raio, dx, dy, dz);
   return(TRUE);
}
