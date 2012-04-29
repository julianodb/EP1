/* 
  PROJETO USPDesigner
  MODULO: ANI (Animacao)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: animlist.cpp
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
#include "hiegroup.h"
#include "vectorop.h"
#include "disdispl.h"
#include "animatio.h"

MOVE *CreateElementAni(Mov cod)
{
   MOVE *p;

   if ((p = (MOVE *)malloc(sizeof(MOVE))) == (MOVE *)NIL)
   {
      fprintf(stderr, "CreateElementAni: no memory left!\n");
      return((MOVE *)NIL);
   }

   if (cod == MovimentoCamera)
   {
      if ((p->mu.mc = (MOVCAM *)malloc(sizeof(MOVCAM))) == (MOVCAM *)NIL)
      {
         fprintf(stderr, "AddCamAni: no memory left!\n");
         RemElementAni(p);
         return((MOVE *)NIL);
      }
   }
   if (cod == MovimentoAxis)
   {
      if ((p->mu.ma = (MOVAXS *)malloc(sizeof(MOVAXS))) == (MOVAXS *)NIL)
      {
         fprintf(stderr, "CreateElementAni: no memory left!\n");
         RemElementAni(p);
         return((MOVE *)NIL);
      }
   }

   p->tipo = cod;
   p->next = (MOVE *)NIL;
   return(p);
}

void AddElementAni(MOVE *e)
{
   MOVE *p;

   if (movhead == (MOVE *)NIL)
   {
      movhead = e;
      return;
   }
   for (p = movhead; p->next; p = p->next)
   {
      ;
   }
   p->next = e;
   e->next = (MOVE *)NIL;
}

void RemElementAni(MOVE *p)
{
   MOVE *e;

   if (p == movhead)
   {
      movhead = (MOVE *)NIL;
      return;
   }
   for (e = movhead; e->next != p; e = e->next)
   {
      ;
   }
   e->next = (MOVE *)NIL;
   free((char *)p);
}

void MSD_AnimationReset(int cod)
{
   if (cod == 0)
   {
      MSD_clearAnimationList();
      return;
   }
   MSD_freeCinemaStructure();
}

void MSD_clearAnimationList(void)
{
   MOVE *p, *aux;

   if (movhead == (MOVE *)NIL)
   {
      printf("Lista ja esta limpa!\n");
      return;
   }

   for (p = movhead; p != (MOVE *)NIL;)
   {
      aux = p;
      p   = p->next;
      if (aux->tipo == MovimentoCamera)
      {
         free((char *)aux->mu.mc);
      }
      if (aux->tipo == MovimentoAxis)
      {
         free((char *)aux->mu.ma);
      }
      free((char *)aux);
   }
   movhead = (MOVE *)NIL;
}

void PrintListAni(void)
{
   MOVE *e;

   if (movhead == (MOVE *)NIL)
   {
      printf("Lista esta limpa!\n");
      return;
   }
   for (e = movhead; e; e = e->next)
   {
      if (e->tipo == MovimentoCamera)
      {
         printf("Movimento de camera. nome: %s scale: %10.4f\n",
                e->mu.mc->cam->cname, e->mu.mc->scale);
         printf("posicao final do olho: %10.4f %10.4f %10.4f\n",
                e->mu.mc->eye[0], e->mu.mc->eye[1], e->mu.mc->eye[2]);
         printf("posicao final da ref : %10.4f %10.4f %10.4f\n",
                e->mu.mc->ref[0], e->mu.mc->ref[1], e->mu.mc->ref[2]);
      }
      else
      {
         printf("Movimento de Eixo. nome: %s  deslocamento: %10.4f \n",
                e->mu.ma->axs->aname, e->mu.ma->desl);
      }
   }
}

void InitListAni(int nframes)
{
   MOVE *e;

   if (movhead == (MOVE *)NIL)
   {
      printf("Lista esta limpa!\n");
      return;
   }
   for (e = movhead; e; e = e->next)
   {
      if (e->tipo == MovimentoCamera)
      {
         vecminus(e->mu.mc->auxeye, e->mu.mc->eye, e->mu.mc->cam->eye);
         vecminus(e->mu.mc->auxref, e->mu.mc->ref, e->mu.mc->cam->ref);
         vecesc(e->mu.mc->auxeye, e->mu.mc->auxeye, 1. / (real)nframes);
         vecesc(e->mu.mc->auxref, e->mu.mc->auxref, 1. / (real)nframes);
         e->mu.mc->scale = (e->mu.mc->scale - e->mu.mc->cam->scale) / nframes;
      }
      else
      {
         e->mu.ma->auxdesl = e->mu.ma->desl / nframes;
      }
   }
}
