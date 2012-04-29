/* 
  PROJETO USPDesigner
  MODULO: ANI (Animacao)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: aniexibe.cpp
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
#include <math.h>
#ifndef __UNIX_
 #ifndef _Windows
  #include <bios.h>
 #endif
 #include <stdlib.h>
 #ifndef _Windows
  #include <graphics.h>
 #else
  #include <windows.h>
 #endif
 #include <time.h>
 #include <malloc.h>
#else
#endif
#include "mensagem.h"
#include "memvirtu.h"
#include "lowparam.h"
#include "lowmacro.h"
#include "lowsolid.h"
#include "vectorop.h"
#include "disdispl.h"
#include "animatio.h"
#include "mancommd.h"
#include "gphgraph.h"

#ifdef __UNIX_
extern GC      XViewGraphContext;
extern Display *XDisplay;
extern Window  ViewWndw;
#endif

int       NumVec[100];
INTVECTOR *ListFrames[100];

void cinema(void)
{
   int  ip, frini, frfim, frpasso;
   char AnimeName[13];

   for (ip = 0; ip == 0;)
   {
      switch (optin())
      {
          case 'l':
          if ((sscanf(restbuf, "%s %d %d %d", AnimeName, &frini, &frfim,
                      &frpasso)) == 4)
          {
             MSD_loadAnimation(AnimeName, frini, frfim, frpasso);
             ip = 1;
          }
          break;

          case 'f':
          MSD_freeCinemaStructure();
          ip = 1;
          break;

          case 'd':
          if ((sscanf(restbuf, "%d", &frpasso)) == 1)
          {
             MSD_displayLoadedAnimation(frpasso);
             ip = 1;
          }
          break;
      }
      if (ip == 0)
      {
         printf("-l AnimacaoNome FrameInicial FrameFinal Passo\n");
         printf("-f (libera memoria)\n");
         printf("-d Passo (exibe animacao)\n");
         if (!lineins("? "))
         {
            return;
         }
      }
   }
}

void MSD_loadAnimation(char *AnimeName, int frini, int frfim, int frpasso)
{
   int  frnum, FrameNum, NumVectors, frct;
   char FrameIndex[13], FrameFileName[13];
   FILE *FrameIndexFile;

   if (NumeroFrames != 0)
   {
      fprintf(stderr, "Ja esta carregada uma animacao!\n");
      return;
   }

   frnum = (int)(floor((real)(frfim - frini) / (real)frpasso) + 1);

   if (frnum > MAXNUMFRAMES)
   {
      printf("ExibeCinema: no memory left!\n");
      return;
   }
   strcpy(FrameIndex, AnimeName);
   strcat(FrameIndex, "000.dsp");
   if ((FrameIndexFile = fopen(FrameIndex, "r")) == (FILE *)NIL)
   {
      fprintf(stderr, "ExibeCinema: erro ao abrir arquivo %s\n",
              FrameIndex);
      return;
   }

   FrameNum = 0;
   while (fscanf(FrameIndexFile, "%s %d", FrameFileName, &NumVectors) == 2)
   {
      frct = FindFrameNum(FrameFileName);
      if ((frct >= frini) && ((frct - frini) % frpasso == 0) && (frct <= frfim))
      {
         LoadFrame(FrameNum, NumVectors, FrameFileName);
         FrameNum++;
      }
   }
   fclose(FrameIndexFile);
   NumeroFrames = FrameNum;
}

void DrawLine(INTVECTOR *auxpt)
{
   int sv[2], ev[2];

   if ((auxpt->cor < 0) || (auxpt->cor > 15))
   {
      return;
   }
   g_pcolor(auxpt->cor);
   sv[0] = auxpt->px1;
   sv[1] = auxpt->py1;
   ev[0] = auxpt->px2;
   ev[1] = auxpt->py2;
   g_iline(auxpt->tlinha == 0 ? SOLID_LINE : DASHED_LINE, sv, ev);
}

void ExibeFrame(int p)
{
   time_t now;

   int       x_ap       = NumVec[p] - 4;
   INTVECTOR *auxptr_ap = ListFrames[p] + 4;

   while (x_ap-- != 0)
   {
      DrawLine(auxptr_ap++);
   }
   time(&now);
   while (difftime(time(0), now) < 1)
   {
      ;
   }
   g_clear();
}

void MSD_displayLoadedAnimation(int num)
{
   if (NumeroFrames == 0)
   {
      fprintf(stderr, "Nao esta carregada nenhuma animacao!\n");
      return;
   }

   g_clear();

   int       x    = 0;
   INTVECTOR *ptr = ListFrames[0];
   while (x < NumVec[0])
   {
      DrawLine(&ptr[x]);
      x++;
   }

   for (int p, i = 0; i < num; i++)
   {
      for (p = 0; p < NumeroFrames; p++)
      {
         ExibeFrame(p);
      }
      for (p = NumeroFrames - 1; p > -1; p--)
      {
         ExibeFrame(p);
      }
   }
}

void LoadFrame(int FrameNum, int NumVectors, char *FrameFileName)
{
   FILE      *FrameFile;
   INTVECTOR *vpptr, *listp;
   float     x1, y1, x2, y2;
   vect2     ev, sv;
   int       NewNumVectors, auxcor, auxtipo, gdcsv[2], gdcev[2], flag, k;

   if ((FrameFile = fopen(FrameFileName, "r")) == (FILE *)NIL)
   {
      fprintf(stderr, "LoadCinema: erro ao abrir arquivo %s\n", FrameFileName);
      return;
   }

   if ((ListFrames[FrameNum] = (INTVECTOR *)malloc(NumVectors
                                                   * sizeof(INTVECTOR))) == (INTVECTOR *)NIL)
   {
      fprintf(stderr, "LoadCinema: no memory left!\n");
      fclose(FrameFile);
      return;
   }

   listp = ListFrames[FrameNum];
   flag  = NewNumVectors = 0;
   while (fscanf(FrameFile, "%d %d %f %f %f %f", &auxcor, &auxtipo,
                 &x1, &y1, &x2, &y2) == 6)
   {
      sv[0] = x1;
      sv[1] = y1;
      ev[0] = x2;
      ev[1] = y2;
      gdcpos(sv, gdcsv);
      gdcpos(ev, gdcev);
      flag = 1;
      for (vpptr = listp, flag = k = 0; k < NewNumVectors; k++, vpptr++)
      {
         if (vpptr->px1 == gdcsv[0])
         {
            if (vpptr->py1 == gdcsv[1])
            {
               if (vpptr->px2 == gdcev[0])
               {
                  if (vpptr->py2 == gdcev[1])
                  {
                     flag = 0;
                     break;
                  }
               }
            }
         }
         if (vpptr->px2 == gdcsv[0])
         {
            if (vpptr->py2 == gdcsv[1])
            {
               if (vpptr->px1 == gdcev[0])
               {
                  if (vpptr->py1 == gdcev[1])
                  {
                     flag = 0;
                     break;
                  }
               }
            }
         }
      }
      if (flag == 0)
      {
         listp[NewNumVectors].cor    = auxcor;
         listp[NewNumVectors].tlinha = auxtipo;
         listp[NewNumVectors].px1    = gdcsv[0];
         listp[NewNumVectors].py1    = gdcsv[1];
         listp[NewNumVectors].px2    = gdcev[0];
         listp[NewNumVectors].py2    = gdcev[1];
         NewNumVectors++;
      }
   }
   fclose(FrameFile);
   fprintf(stderr, "Frame %s  %4d ---> %4d\n", FrameFileName, NumVectors,
           NewNumVectors);
   NumVec[FrameNum] = NewNumVectors;
}

int FindFrameNum(char *nome)
{
   return(atoi(strchr(nome, (char)NIL) - 7));
}

void MSD_freeCinemaStructure(void)
{
   if (NumeroFrames == 0)
   {
      fprintf(stderr, "Nao esta carregada nenhuma animacao!\n");
      return;
   }

   for (int t = 0; t == NumeroFrames; t++)
   {
      free((char *)ListFrames[t]);
   }
   NumeroFrames = 0;
}
