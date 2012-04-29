/* 
  PROJETO USPDesigner
  MODULO: SET (Operadores Booleanos)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: setopcon.cpp
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
#include <math.h>
#include "memvirtu.h"
#include "lowparam.h"
#include "lowmacro.h"
#include "lowsolid.h"
#include "eulerops.h"
#include "vectorop.h"
#include "sethmain.h"
#include "disdispl.h"
#include "gphgraph.h"

int MSD_checkBoolean(void);

extern SPTYPE sola;
extern SPTYPE solb;

/* Joining algorithm for set operations */
void MSD_lowSetOpConnect(void)
{
   EPTYPE nextedgea;
   EPTYPE nextedgeb;
   HPTYPE h1a;
   HPTYPE h2a;
   HPTYPE h1b;
   HPTYPE h2b;
   int    finished_ok = 0;

#ifdef TRACE_BOOLEAN
   vector feq;
   int    i;

   fprintf(trace, "\n\n\n");
   fprintf(trace, "*************************************************\n");
   fprintf(trace, "*                                               *\n");
   fprintf(trace, "*                 SETOPCONNECT                  *\n");
   fprintf(trace, "*                                               *\n");
   fprintf(trace, "*************************************************\n");
   fprintf(trace, "\n");
#endif

   nenda = nendb = 0;
   MSD_lowSetOpSortNullEdges();
   while (MSD_lowSetOpGetNextNullEdge(&nextedgea, &nextedgeb))
   {
      if (MSD_lowSetOpCanJoin(EdgHe1(nextedgea), EdgHe2(nextedgeb),
                              &h1a, &h2b))
      {
#ifdef TRACE_BOOLEAN3
         fprintf(trace, "*** Une as arestas do solido A ");
#endif
         MSD_lowSetOpJoin(h1a, EdgHe1(nextedgea));
         if (!MSD_lowSetOpIsLooseA(mate(h1a)))
         {
            MSD_lowSetOpCutA(h1a);
         }

#ifdef TRACE_BOOLEAN3
         fprintf(trace, "*** Une as arestas do solido B ");
#endif

         MSD_lowSetOpJoin(h2b, EdgHe2(nextedgeb));
         if (!MSD_lowSetOpIsLooseB(mate(h2b)))
         {
            MSD_lowSetOpCutB(h2b);
         }
      }
      if (MSD_lowSetOpCanJoin(EdgHe2(nextedgea), EdgHe1(nextedgeb),
                              &h2a, &h1b))
      {
#ifdef TRACE_BOOLEAN3
         fprintf(trace, "*** Une as arestas do solid A");
#endif

         MSD_lowSetOpJoin(h2a, EdgHe2(nextedgea));
         if (!MSD_lowSetOpIsLooseA(mate(h2a)))
         {
            MSD_lowSetOpCutA(h2a);
         }

#ifdef TRACE_BOOLEAN3
         fprintf(trace, "*** Une as arestas do solid B");
#endif

         MSD_lowSetOpJoin(h1b, EdgHe1(nextedgeb));
         if (!MSD_lowSetOpIsLooseB(mate(h1b)))
         {
            MSD_lowSetOpCutB(h1b);
         }
      }
      if ((h1a != HNIL) && (h1b != HNIL) && (h2a != HNIL) && (h2b != HNIL))
      {
         MSD_lowSetOpCutA(EdgHe1(nextedgea));
         MSD_lowSetOpCutB(EdgHe1(nextedgeb));
         finished_ok = 1;
      }
   }

#ifdef TRACE_BOOLEAN
   fprintf(trace, "\n");
   fprintf(trace, "*** setopconnect *** - nfaca = %2i\n", nfaca);
   fprintf(trace, "*** setopconnect *** - nfacb = %2i\n", nfacb);
   fprintf(trace, "*** estrutura sonfa ***\n");
   for (i = 0; i < nfaca; i++)
   {
      MSD_lowFaceEq(FacFLOut(SetSonfa(i)), feq);
      veccopy(FacFeq(SetSonfa(i)), feq);
      trace_f(SetSonfa(i));
      MSD_lowListFace(trace, SetSonfa(i), 3);
   }
   fprintf(trace, "*** estrutura sonfb ***\n");
   for (i = 0; i < nfacb; i++)
   {
      MSD_lowFaceEq(FacFLOut(SetSonfb(i)), feq);
      veccopy(feq, FacFeq(SetSonfb(i)));
      trace_f(SetSonfb(i));
      MSD_lowListFace(trace, SetSonfb(i), 3);
   }
#endif

   if (finished_ok == 0 /* || MSD_checkBoolean() == 1 */)
   {
      fprintf(stderr, "!!");

#ifdef TRACE_BOOLEAN
      fclose(trace);
#endif

      exit(1);
   }
}

/* joining and cutting of null edges */
void MSD_lowSetOpJoin(HPTYPE h1, HPTYPE h2)
{
   FPTYPE oldf;
   FPTYPE newf;

#ifdef TRACE_BOOLEAN3
   int flag;
   flag = 0;
#endif

   oldf = LooLFace(HalWLoop(h1));
   newf = FNIL;
   if (HalWLoop(h1) == HalWLoop(h2))
   {
#ifdef TRACE_BOOLEAN3
      if (HalPrv(HalPrv(h1)) != h2)
      {
         if ((HalWLoop(h1) != FacFLOut(LooLFace(HalWLoop(h1)))) &&
             (h1 == EdgHe1(HalEdg(h1))))
         {
            fprintf(trace, " (MEF) *** case a\n");
            trace_he(HalNxt(h2));
            trace_he(h1);
            newf = MSD_lowMEF(HalNxt(h2), h1, maxf++);
         }
         else
         {
            fprintf(trace, " (MEF) *** case b\n");
            trace_he(h1);
            trace_he(HalNxt(h2));
            newf = MSD_lowMEF(h1, HalNxt(h2), maxf++);
         }
         flag = 1;
      }
#else
      if (HalPrv(HalPrv(h1)) != h2)
      {
         if ((HalWLoop(h1) != FacFLOut(LooLFace(HalWLoop(h1)))) &&
             (h1 == EdgHe1(HalEdg(h1))))
         {
            newf = MSD_lowMEF(HalNxt(h2), h1, maxf++);
         }
         else
         {
            newf = MSD_lowMEF(h1, HalNxt(h2), maxf++);
         }
      }
#endif
   }

#ifdef TRACE_BOOLEAN3
   else
   {
      fprintf(trace, " (MEKR) ***\n");
      trace_he(h1);
      trace_he(HalNxt(h2));
      MSD_lowMEKR(h1, HalNxt(h2));
      flag = 1;
   }
#else
   else
   {
      MSD_lowMEKR(h1, HalNxt(h2));
   }
#endif

   if (HalNxt(HalNxt(h1)) != h2)
   {
#ifdef TRACE_BOOLEAN3
      if (flag == 1)
      {
         fprintf(trace, "*** Une as arestas novamente ");
      }
      fprintf(trace, " (MEF) *** caso c\n");
      trace_he(h2);
      trace_he(HalNxt(h1));
#endif
      MSD_lowMEF(h2, HalNxt(h1), maxf++);

#ifdef TRACE_BOOLEAN3
      if ((newf != FNIL) && (LooNextL(FacFLoops(oldf)) != LNIL))
      {
         fprintf(trace, "*** Operador (LARINGMV) \n");
         MSD_lowAllRingMove(oldf, newf);
      }
#else
      if ((newf != FNIL) && (LooNextL(FacFLoops(oldf)) != LNIL))
      {
         MSD_lowAllRingMove(oldf, newf);
      }
#endif
   }
}

/* Joinability test for set operations */
int MSD_lowSetOpCanJoin(HPTYPE hea, HPTYPE heb, HPTYPE *reta, HPTYPE *retb)
{
   int i, j;

#ifdef TRACE_BOOLEAN3
   fprintf(trace, "*** -------------------------------------------------\n");
   fprintf(trace, "*** scanjoin\n");
   trace_he(hea);
   trace_he(heb);
#endif

   for (i = 0; i < nenda; ++i)
   {
      if (MSD_lowSetOpNeighbor(hea, SetEndsa(i)) &&
          MSD_lowSetOpNeighbor(heb, SetEndsb(i)))
      {
         *reta = SetEndsa(i);
         *retb = SetEndsb(i);
         for (j = i + 1; j < nenda; ++j)
         {
            SetEndsa(j - 1) = SetEndsa(j);
            SetEndsb(j - 1) = SetEndsb(j);
         }
         nenda--;
         nendb--;

#ifdef TRACE_BOOLEAN3
         fprintf(trace, "*** scanjoin - OK - join is possible\n");
         trace_he(*reta);
         trace_he(*retb);
         fprintf(trace, "*** -------------------------------------------------\n\n");
#endif

         return(1);
      }
   }
   SetEndsa(nenda) = hea;
   SetEndsb(nendb) = heb;
   nenda++;
   nendb++;

#ifndef __VIRTUAL_MEM
   if (nenda == MAXENDS)
   {
      fprintf(stderr, "ends: falta memoria!\n");
      exit(1);
   }
#endif

   *reta = HNIL;
   *retb = HNIL;

#ifdef TRACE_BOOLEAN3
   fprintf(trace, "*** scanjoin - join is not possible\n");
   fprintf(trace, "*** -------------------------------------------------\n\n");
#endif

   return(0);
}

void MSD_lowSetOpSortNullEdges(void)
{
   register int i, j;

   null_pnta = null_pntb = 0;

   /* Em seguida verifica as arestas que possuem as mesmas faces dos dois */
   /*   lados. Em ambos os solidos! Neste caso eles devem ser processados */
   /*   primeiro!                                                         */

   if (nedga < 5)
   {
      return;
   }
   for (i = j = 0; i < nedga; i++)
   {
      for (j = i + 1; j < nedga; j++)
      {
         if (MSD_lowSetOpGreater(i, j))
         {
            MSD_lowSetOpSwap(i, j);
         }
      }
   }
}

int MSD_lowSetOpGreater(int i, int j)
{
   VPTYPE v1;
   VPTYPE v2;
   int    com;

   v1 = HalVtx(EdgHe1(SetSonea(i)));
   v2 = HalVtx(EdgHe1(SetSonea(j)));
   if ((com = comp(VerVCoord(v1)[0], VerVCoord(v2)[0], EPS)) == -1)
   {
      return(0);
   }
   if (com == 0)
   {
      if ((com = comp(VerVCoord(v1)[1], VerVCoord(v2)[1], EPS)) == -1)
      {
         return(0);
      }
      if (com == 0)
      {
         if ((com = comp(VerVCoord(v1)[2], VerVCoord(v2)[2], EPS)) == -1)
         {
            return(0);
         }
      }
   }
   return(1);
}

void MSD_lowSetOpSwap(int i, int j)
{
   EPTYPE temp;

   temp        = SetSonea(i);
   SetSonea(i) = SetSonea(j);
   SetSonea(j) = temp;
   temp        = SetSoneb(i);
   SetSoneb(i) = SetSoneb(j);
   SetSoneb(j) = temp;
}

int MSD_lowSetOpIsLooseA(HPTYPE he)
{
   int i;

   for (i = 0; i < nenda; ++i)
   {
      if (SetEndsa(i) == he)
      {
         return(1);
      }
   }
   return(0);
}

int MSD_lowSetOpIsLooseB(HPTYPE he)
{
   int i;

   for (i = 0; i < nendb; ++i)
   {
      if (SetEndsb(i) == he)
      {
         return(1);
      }
   }
   return(0);
}

void MSD_lowSetOpCutA(HPTYPE he)
{
   if (HalWLoop(EdgHe1(HalEdg(he))) == HalWLoop(EdgHe2(HalEdg(he))))
   {
#ifdef TRACE_BOOLEAN3
      fprintf(trace, "*** Corta Solido A (KEMR) ***\n");
      trace_he(EdgHe2(HalEdg(he)));
      fprintf(trace, "*** Acrescenta Face Solido A (sonfa)\n");
      trace_f(LooLFace(HalWLoop(he)));
#endif

      SetSonfa(nfaca) = LooLFace(HalWLoop(he));
      nfaca++;

#ifndef __VIRTUAL_MEM
      if (nfaca == MAXNULLFACES)
      {
         fprintf(stderr, "sonfa: falta memoria!\n");
         exit(1);
      }
#endif

      MSD_lowKEMR(EdgHe2(HalEdg(he)), EdgHe1(HalEdg(he)));
   }

#ifdef TRACE_BOOLEAN3
   else
   {
      fprintf(trace, "*** Corta Solido A (KEF) ***\n");
      trace_he(EdgHe1(HalEdg(he)));
      MSD_lowKEF(EdgHe1(HalEdg(he)), EdgHe2(HalEdg(he)));
   }
#else
   else
   {
      MSD_lowKEF(EdgHe1(HalEdg(he)), EdgHe2(HalEdg(he)));
   }
#endif
}

void MSD_lowSetOpCutB(HPTYPE he)
{
   if (HalWLoop(EdgHe1(HalEdg(he))) == HalWLoop(EdgHe2(HalEdg(he))))
   {
#ifdef TRACE_BOOLEAN3
      fprintf(trace, "*** Corta Solido B (KEMR) ***\n");
      trace_he(EdgHe2(HalEdg(he)));
      fprintf(trace, "*** Acrescenta Face Solido B (sonfb)\n");
      trace_f(LooLFace(HalWLoop(he)));
#endif

      SetSonfb(nfacb) = LooLFace(HalWLoop(he));
      nfacb++;

#ifndef __VIRTUAL_MEM
      if (nfacb == MAXNULLFACES)
      {
         fprintf(stderr, "sonfb: falta memoria!\n");
         exit(1);
      }
#endif

      MSD_lowKEMR(EdgHe2(HalEdg(he)), EdgHe1(HalEdg(he)));
   }

#ifdef TRACE_BOOLEAN3
   else
   {
      fprintf(trace, "*** Corta Solido B (KEF) ***\n");
      trace_he(EdgHe1(HalEdg(he)));
      MSD_lowKEF(EdgHe1(HalEdg(he)), EdgHe2(HalEdg(he)));
   }
#else
   else
   {
      MSD_lowKEF(EdgHe1(HalEdg(he)), EdgHe2(HalEdg(he)));
   }
#endif
}

int MSD_lowSetOpGetNextNullEdge(EPTYPE *nexta, EPTYPE *nextb)
{
#ifdef TRACE_BOOLEAN3
   fprintf(trace, "\n*** sgetnextnulledge\n");
#endif

   *nexta = null_pnta < nedga ? SetSonea(null_pnta) : ENIL;
   *nextb = null_pntb < nedgb ? SetSoneb(null_pntb) : ENIL;
   null_pnta++;
   null_pntb++;
   if ((*nextb == ENIL) && (*nexta == ENIL))
   {
      return(FALSE);
   }

#ifdef TRACE_BOOLEAN3
   trace_he(EdgHe1(*nexta));
   trace_he(EdgHe1(*nextb));
#endif

   return(TRUE);
}

int MSD_lowSetOpNeighbor(HPTYPE h1, HPTYPE h2)
{
   if (HalVtx(h1) == HalVtx(h2))
   {
      return(FALSE);
   }
   if (HalVtx(h1) == HalVtx(mate(h2)))
   {
      return(FALSE);
   }
   if (HalVtx(mate(h1)) == HalVtx(h2))
   {
      return(FALSE);
   }
   if (HalVtx(mate(h1)) == HalVtx(mate(h2)))
   {
      return(FALSE);
   }
   return(LooLFace(HalWLoop(h1)) == LooLFace(HalWLoop(h2)) &&
          ((h1 == EdgHe1(HalEdg(h1)) && h2 == EdgHe2(HalEdg(h2))) ||
           (h1 == EdgHe2(HalEdg(h1)) && h2 == EdgHe1(HalEdg(h2)))));
}
