/* 
  PROJETO USPDesigner
  MODULO: ANI (Animacao)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: anicreat.cpp
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
#include <math.h>
#include <string.h>
#ifndef __UNIX_
 #include <malloc.h>
 #include <stdlib.h>
#endif
#include "mensagem.h"
#include "memvirtu.h"
#include "lowparam.h"
#include "lowmacro.h"
#include "lowsolid.h"
#include "genfunc_.h"
#include "hiegroup.h"
#include "vectorop.h"
#include "disdispl.h"
#include "animatio.h"
#include "mancommd.h"

void animatio(void)
{
   char  aname[13], arqnam[13];
   int   ip, nframes, opcao;
   float edesl, x1, y1, z1, x2, y2, z2;

   for (ip = 0; ip == 0;)
   {
      switch (optin())
      {
          case 'i':
          MSD_clearAnimationList();
          ip = 1;
          break;

          case 'p':
          PrintListAni();
          ip = 1;
          break;

          case 'm':
          if (sscanf(restbuf, "%d %d %s %s %s", &opcao, &nframes, arqnam,
                     aname) == 4)
          {
             MSD_createAnimation(opcao, nframes, arqnam, aname);
             ip = 1;
          }
          break;

          case 'c':
          if (sscanf(restbuf, "%s %f %f %f %f %f %f %f", aname, &x1,
                     &y1, &z1, &x2, &y2, &z2, &edesl) == 8)
          {
             MSD_addCameraToMovementStructure(aname, x1, y1, z1, x2, y2, z2, edesl);
             ip = 1;
          }
          break;

          case 'a':
          if (sscanf(restbuf, "%s %f", aname, &edesl) == 2)
          {
             MSD_addAxisToMovementStructure(aname, edesl);
             ip = 1;
          }
      }
      if (ip == 0)
      {
         printf("-ip \n");
         printf("-a  eixo desl\n");
         printf("-c  camera olho[3] ref[3] scale\n");
         printf("-m  ini nframes arquivo camera objetos\n");
         if (!lineins("? "))
         {
            return;
         }
      }
   }
}

void MSD_selectAllObjects(void)
{
   STYPE *optr;

   for (AllSolids(optr))
   {
      SolDsp(optr) = TRUE;
   }
}

void MSD_resetAllObjects(void)
{
   STYPE *optr;

   for (AllSolids(optr))
   {
      SolDsp(optr) = FALSE;
   }
}

void MSD_selectElement(char *name)
{
   if (MSD_getTypeByName(name) == 'o')
   {
      MSD_selectObject(name);
      return;
   }
   if (MSD_getTypeByName(name) == 'g')
   {
      MSD_selectGroup(name);
      return;
   }
}

void MSD_resetElement(char *name)
{
   if (MSD_getTypeByName(name) == 'o')
   {
      MSD_resetObject(name);
      return;
   }
   if (MSD_getTypeByName(name) == 'g')
   {
      MSD_resetGroup(name);
      return;
   }
}

void MSD_selectGroup(char *name)
{
   SPTYPE optr;
   GPTYPE gptr;

   if ((gptr = MSD_lowFindGroupByName(name)) == GNIL)
   {
      return;
   }
   for (AllSolids(optr))
   {
      if (SolGroup(optr) != GNIL)
      {
         if (MSD_lowIsAncestorGroup(gptr, SolGroup(optr)))
         {
            SolDsp(optr) = TRUE;
         }
      }
   }
}

void MSD_resetGroup(char *name)
{
   SPTYPE optr;
   GPTYPE gptr;

   if ((gptr = MSD_lowFindGroupByName(name)) == GNIL)
   {
      return;
   }
   for (AllSolids(optr))
   {
      if (SolGroup(optr) != GNIL)
      {
         if (MSD_lowIsAncestorGroup(gptr, SolGroup(optr)))
         {
            SolDsp(optr) = FALSE;
         }
      }
   }
}

void MSD_selectObject(char *name)
{
   SPTYPE optr;

   if ((optr = MSD_lowFindSolidByName(name)) != SNIL)
   {
      SolDsp(optr) = TRUE;
   }
}

void MSD_resetObject(char *name)
{
   SPTYPE optr;

   if ((optr = MSD_lowFindSolidByName(name)) != SNIL)
   {
      SolDsp(optr) = FALSE;
   }
}

void MSD_addAxisToMovementStructure(char *eaname, real edesl)
{
   APTYPE axis;
   MOVE  *count;

   if ((axis = MSD_lowFindAxisByName(eaname)) == ANIL)
   {
      fprintf(stderr, "%s nao e o nome de nenhum eixo!\n", eaname);
      return;
   }
   if ((count = CreateElementAni(MovimentoAxis)) == (MOVE *)NIL)
   {
      fprintf(stderr, "AddAxisAni: no memory left!\n");
      return;
   }
   AddElementAni(count);
   count->mu.ma->axs  = axis;
   count->mu.ma->desl = edesl;
}

void MSD_addCameraToMovementStructure(char *aname, real ex, real ey, real ez,
                                      real rx, real ry, real rz, real desl)
{
   vector eeye, eref;

   makevec(eeye, ex, ey, ez, 1.0);
   makevec(eref, rx, ry, rz, 1.0);
   AddCamAni(aname, eeye, eref, desl);
}

void AddCamAni(char *aname, vector eeye, vector eref, real edesl)
{
   CTYPE *cam;
   MOVE  *count;

   if ((cam = MSD_lowFindCameraByName(aname)) == (CTYPE *)NIL)
   {
      fprintf(stderr, "%s nao e o nome de nenhuma camera!\n", aname);
      return;
   }
   if ((count = CreateElementAni(MovimentoCamera)) == (MOVE *)NIL)
   {
      fprintf(stderr, "AddCamAni: no memory left!\n");
      return;
   }
   AddElementAni(count);
   count->mu.mc->cam = cam;
   veccopy(count->mu.mc->eye, eeye);
   veccopy(count->mu.mc->ref, eref);
   count->mu.mc->scale = edesl;
}

FILE *OpenIndexAni(char *AnimeName)
{
   char FrameIndex[13];
   FILE *FrameIndexFile;

   strcpy(FrameIndex, AnimeName);
   strcat(FrameIndex, "000.TMP");
   if ((FrameIndexFile = fopen(FrameIndex, "w")) == (FILE *)NIL)
   {
      fprintf(stderr, "Nao obteve sucesso em abrir o arquivo %s!\n", FrameIndex);
      return((FILE *)NIL);
   }
   return(FrameIndexFile);
}

void CriaFrame(FILE *FrameIndexFile, char *AnimeName, int framect)
{
   SPTYPE  optr;
   CTYPE  *cptr;
   MOVE   *count;
   char   FrameFile[13];
   vector eeye, eref;
   real   edesl;

   for (count = movhead; count; count = count->next)
   {
      if (count->tipo == MovimentoCamera)
      {
         cptr = count->mu.mc->cam;
         vecplus(eeye, cptr->eye, count->mu.mc->auxeye);
         vecplus(eref, cptr->ref, count->mu.mc->auxref);
         edesl = cptr->scale + count->mu.mc->auxscale;
         MSD_lowSetCamera(cptr, eeye, eref, edesl, cptr->mode, cptr->hid, cptr->dash,
                          cptr->smooth);
      }
      else
      {
         edesl = count->mu.ma->auxdesl;
         MSD_highNameTransformUsingAxis(count->mu.ma->axs->aname, edesl);
      }
   }
   MSD_lowDisplayWriteFile(InitFrameCount(AnimeName, FrameFile, framect));
   for (AllSolids(optr))
   {
      SolDsp(optr) = TRUE;
   }
   g_clear();
   framenum = 0;
   Draw     = FALSE;
   switch (camptr->hid)
   {
       case 0:
       MSD_highDisplayAllEdges();
       break;

       case 1:
       MSD_highDisplayLocalHidden();
       break;

       case 2:
       MSD_lowCameraDisplayHidden();
       break;

       case 3:
       MSD_lowCameraDisplayIntersection();
       break;
   }
   Draw = TRUE;
   fclose(dspfwptr);
   dspfwptr = (FILE *)NIL;
   fprintf(FrameIndexFile, "%s %d\n", FrameFile, framenum);
}

void MSD_createAnimation(int opcao, int nframes, char *AnimeName, char *ecname)
{
   MOVE *count;
   char FrameFile[13];
   int  framect;
   FILE *FrameIndexFile;

   if ((camptr = MSD_lowFindCameraByName(ecname)) == (CTYPE *)NIL)
   {
      fprintf(stderr, "%s nao e o nome de nenhuma camera!\n", ecname);
      return;
   }
   if ((FrameIndexFile = OpenIndexAni(AnimeName)) == (FILE *)NIL)
   {
      return;
   }

   for (count = movhead; count; count = count->next)
   {
      if (count->tipo == MovimentoCamera)
      {
         vecminus(count->mu.mc->auxeye, count->mu.mc->eye, camptr->eye);
         vecesc(count->mu.mc->auxeye, count->mu.mc->auxeye, 1. / ((real)nframes));
         vecminus(count->mu.mc->auxref, count->mu.mc->ref, camptr->ref);
         vecesc(count->mu.mc->auxref, count->mu.mc->auxref, 1. / ((real)nframes));
         count->mu.mc->auxscale = (count->mu.mc->scale - camptr->scale) / ((real)nframes);
      }
      else
      {
         count->mu.ma->auxdesl = count->mu.ma->desl / ((real)nframes);
      }
   }

   if (opcao == 1)
   {
      opcao++;
      for (count = movhead; count; count = count->next)
      {
         if (count->tipo == MovimentoCamera)
         {
            camptr = count->mu.mc->cam;
         }
      }
      MSD_lowDisplayWriteFile(InitFrameCount(AnimeName, FrameFile, 1));
      /* for (AllSolids(optr)) optr->dsp = TRUE ; */
      g_clear();
      framenum = 0;
      Draw     = FALSE;
      switch (camptr->hid)
      {
          case 0:
          MSD_highDisplayAllEdges();
          break;

          case 1:
          MSD_highDisplayLocalHidden();
          break;

          case 2:
          MSD_lowCameraDisplayHidden();
          break;

          case 3:
          MSD_lowCameraDisplayIntersection();
          break;
      }
      Draw = TRUE;
      fclose(dspfwptr);
      dspfwptr = (FILE *)NIL;
      fprintf(FrameIndexFile, "%s %d\n", FrameFile, framenum);
   }

   for (framect = opcao; framect <= opcao + nframes - 1; framect++)
   {
      CriaFrame(FrameIndexFile, AnimeName, framect);
   }
   fclose(FrameIndexFile);
   PurificaAni(AnimeName, opcao);
}

char *InitFrameCount(char *AnimeName, char *FrameFile, int framect)
{
   char Numero[4];

   strcpy(FrameFile, AnimeName);
   if (framect < 100)
   {
      strcat(FrameFile, "0");
   }
   if (framect < 10)
   {
      strcat(FrameFile, "0");
   }
#ifdef __UNIX_
   itoaa(Numero, framect);
#else
   _itoa(framect, Numero, 10);
#endif
   strcat(FrameFile, Numero);
   strcat(FrameFile, ".dsp");
   return(FrameFile);
}

#ifdef __UNIX_
void itoaa(char *s, int n)
{
   int i, sign;

   if ((sign = n) < 0)                   /* record positive */
   {
      n = -n;                            /* make n positive */
   }
   i = 0;
   do                                          /* generate digits in reverse order */
   {
      s[i++] = n % 10 + '0';                   /* get next digit */
   } while ((n /= 10) > 0);                    /* delete it */
   if (sign < 0)
   {
      s[i++] = '-';
   }
   s[i] = '\0';
   reverse(s);
}

/*
 * void reverse(char *s)
 * {
 *  int c, i, j ;
 *
 *  for (i = 0, j = strlen(s) - 1 ; i < j ; i++, j--) {
 *      c = s[i] ; s[i] = s[j] ; s[j] = c ;
 *  }
 * }
 */
#endif

void PurificaAni(char *AnimeName, int opcao)
{
   int  NumVectors;
   char FrameIndex[13], FrameFileName[13];
   FILE *FrameIndexFile;

   strcpy(FrameIndex, AnimeName);
   strcat(FrameIndex, "000.TMP");
   if ((FrameIndexFile = fopen(FrameIndex, "r")) == (FILE *)NIL)
   {
      fprintf(stderr, "erro ao abrir arquivo %s\n", FrameIndex);
      return;
   }
   while (fscanf(FrameIndexFile, "%s %d", FrameFileName, &NumVectors) == 2)
   {
      PurificaFrame(NumVectors, AnimeName, FrameFileName, opcao);
      opcao = 2;
   }
   fclose(FrameIndexFile);
}

void PurificaFrame(int NumVectors, char *AnimeName, char *FrameFileName, int opcao)
{
   FLOVECTOR Vectors[MAXVECTORS], *auxvp, *vp;
   int       filt, flag, k, NumVectorsNew, auxcor, auxtipo;
   float     ev1, ev2, sv1, sv2;
   char      FrameIndexFileName[13];
   FILE      *FrameFile, *FrameIndexFile;
   real      eps; 

   if (NumVectors > MAXVECTORS)
   {
      fprintf(stderr, "CreateFrameArray: falta memoria!\n");
      return;
   }
   if ((FrameFile = fopen(FrameFileName, "r")) == (FILE *)NIL)
   {
      fprintf(stderr, "CreateFrameArray: erro ao abrir arquivo %s\n",
              FrameFileName);
      return;
   }
   eps  = FILTERPRECISION;
   flag = filt = NumVectorsNew = 0;
   while (fscanf(FrameFile, "%d %d %f %f %f %f", &auxcor, &auxtipo,
                 &sv1, &sv2, &ev1, &ev2) == 6)
   {
      for (auxvp = Vectors, k = 0; k < NumVectorsNew; k++)
      {
         flag = 0;
         if (compara(auxvp->px1, sv1, eps))
         {
            if (compara(auxvp->py1, sv2, eps))
            {
               if (compara(auxvp->px2, ev1, eps))
               {
                  if (compara(auxvp->py2, ev2, eps))
                  {
                     flag = filt = 1;
                     break;
                  }
               }
            }
         }
         if (compara(auxvp->px2, sv1, eps))
         {
            if (compara(auxvp->py2, sv2, eps))
            {
               if (compara(auxvp->px1, ev1, eps))
               {
                  if (compara(auxvp->py1, ev2, eps))
                  {
                     flag = filt = 1;
                     break;
                  }
               }
            }
         }
         auxvp++;
      }
      if (flag == 0)
      {
         Vectors[NumVectorsNew].cor    = auxcor;
         Vectors[NumVectorsNew].tlinha = auxtipo;
         Vectors[NumVectorsNew].px1    = sv1;
         Vectors[NumVectorsNew].py1    = sv2;
         Vectors[NumVectorsNew].px2    = ev1;
         Vectors[NumVectorsNew].py2    = ev2;
         NumVectorsNew++;
      }
   }
   fclose(FrameFile);

   if (filt == 1)
   {
      fprintf(stderr, "Reescrevendo o arquivo %s %d --> %d\n",
              FrameFileName, NumVectors, NumVectorsNew);
      if ((FrameFile = fopen(FrameFileName, "w")) == (FILE *)NIL)
      {
         fprintf(stderr, "CreateFrameArray: erro ao abrir arquivo %s\n",
                 FrameFileName);
         return;
      }
      for (vp = Vectors, k = 0; k < NumVectorsNew; k++, vp++)
      {
         fprintf(FrameFile, "%2d %2d %10.6f %10.6f %10.6f %10.6f\n",
                 vp->cor, vp->tlinha, vp->px1, vp->py1, vp->px2, vp->py2);
      }
      fclose(FrameFile);
   }
   else
   {
      NumVectorsNew = NumVectors;
      fprintf(stderr, "Arquivo %s %d\n", FrameFileName, NumVectorsNew);
   }

   strcpy(FrameIndexFileName, AnimeName);
   strcat(FrameIndexFileName, "000.dsp");
   if ((FrameIndexFile = fopen(FrameIndexFileName, opcao == 1 ? "w" : "a")) ==
       (FILE *)NIL)
   {
      fprintf(stderr, "CreateFrameArray: erro ao abrir arquivo %s!\n",
              FrameIndexFileName);
      return;
   }
   fprintf(FrameIndexFile, "%s %d\n", FrameFileName, NumVectorsNew);
   fclose(FrameIndexFile);
}

int compara(real x, real y, real eps)
{
   if (fabs(x - y) < eps)
   {
      return(TRUE);
   }
   return(FALSE);
}
