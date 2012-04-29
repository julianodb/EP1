/* 
  PROJETO USPDesigner
  MODULO: SET (Operadores Booleanos)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: settrace.cpp
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
#include "splsplit.h"

extern SPTYPE sola;
extern SPTYPE solb;

#ifdef TRACE_BOOLEAN
void trace_he(HPTYPE h)
{
   HPTYPE      hm;
   HPTYPE      hn;
   EPTYPE      e;
   LPTYPE      lo;
   LPTYPE      lm;
   VPTYPE      vv;
   extern FILE *trace;

   e  = HalEdg(h);
   hm = e == ENIL ? HNIL : mate(h);
   lo = HalWLoop(h);
   lm = e == ENIL ? LNIL : HalWLoop(hm);
   hn = e == ENIL ? HNIL : HalNxt(h);
   vv = HalVtx(h);

   fprintf(trace, "*** halfedge     f%3d-%3d v%3d-%3d    %10.4f %10.4f %10.4f \n",
           FacFaceNo(LooLFace(lo)), e == ENIL ? -1 : FacFaceNo(LooLFace(lm)),
           VerVertexNo(HalVtx(h)), e == ENIL ? -1 : VerVertexNo(HalVtx(hn)),
           VerVCoord(vv)[0], VerVCoord(vv)[1], VerVCoord(vv)[2]);
   fprintf(trace, "                                      %10.4f %10.4f %10.4f \n",
           e == ENIL ? 9999 : VerVCoord(HalVtx(hn))[0],
           e == ENIL ? 9999 : VerVCoord(HalVtx(hn))[1],
           e == ENIL ? 9999 : VerVCoord(HalVtx(hn))[2]);
}

void trace_e(EPTYPE e)
{
   HPTYPE      he1;
   HPTYPE      he2;
   VPTYPE      v1;
   VPTYPE      v2;
   LPTYPE      l1;
   LPTYPE      l2;
   extern FILE *trace;

   he1 = EdgHe1(e);
   he2 = EdgHe2(e);
   v1  = HalVtx(he1);
   v2  = HalVtx(he2);
   l1  = HalWLoop(he1);
   l2  = HalWLoop(he2);
   fprintf(trace, "*** edge    %3d  f%3d-%3d v%3d-%3d    %10.4f %10.4f %10.4f \n",
           EdgEdgeNo(e), FacFaceNo(LooLFace(l1)), FacFaceNo(LooLFace(l2)), \
           VerVertexNo(v1), VerVertexNo(v2),
           VerVCoord(v1)[0], VerVCoord(v1)[1], VerVCoord(v1)[2]);
   fprintf(trace, "                                      %10.4f %10.4f %10.4f \n",
           VerVCoord(v2)[0], VerVCoord(v2)[1], VerVCoord(v2)[2]);
}

void trace_nbr(int x)
{
   HPTYPE      he;
   HPTYPE      hm;
   LPTYPE      lo;
   LPTYPE      lm;
   extern FILE *trace;
   static char *ff[] = { "OUT", "ON ", "IN " };

   he = SetNBRSector(x);
   hm = HalEdg(he) == ENIL ? HNIL : mate(he);
   lo = HalWLoop(he);
   lm = hm == HNIL ? LNIL : HalWLoop(hm);
   fprintf(trace, "*** nbr[%2i] = f%3d-%3d v%3d-%3d %4s\n",
           x, FacFaceNo(LooLFace(lo)),
           lm == LNIL ? -1 : FacFaceNo(LooLFace(lm)),
           VerVertexNo(HalVtx(he)),
           hm == HNIL ? -1 : VerVertexNo(HalVtx(hm)),
           ff[SetNBRC1(x) + 1]);
}

void trace_nba(int h)
{
   extern FILE *trace;
   EPTYPE      e;
   HPTYPE      he;
   HPTYPE      hnxt;

   he   = SetANBHe(h);
   e    = HalEdg(he);
   hnxt = HalNxt(he);
   fprintf(trace, "*** nba[%2d].he   f%3d-%3d v%3d-%3d     %10.4f %10.4f %10.4f ***\n",
           h, e == ENIL ? -1 : FacFaceNo(LooLFace(HalWLoop(mate(he)))),
           FacFaceNo(LooLFace(HalWLoop(he))),
           VerVertexNo(HalVtx(he)),
           e == ENIL ? -1 : VerVertexNo(HalVtx(hnxt)),
           VerVCoord(HalVtx(he))[0],
           VerVCoord(HalVtx(he))[1],
           VerVCoord(HalVtx(he))[2]);
   fprintf(trace, "                                       %10.4f %10.4f %10.4f \n",
           e == ENIL ? 9999 : VerVCoord(HalVtx(hnxt))[0],
           e == ENIL ? 9999 : VerVCoord(HalVtx(hnxt))[1],
           e == ENIL ? 9999 : VerVCoord(HalVtx(hnxt))[2]);
   fprintf(trace, "    ref1                               %10.4f %10.4f %10.4f \n",
           SetANB1(h) [0], SetANB1(h) [1], SetANB1(h) [2]);
   fprintf(trace, "    ref2                               %10.4f %10.4f %10.4f \n",
           SetANB2(h) [0], SetANB2(h) [1], SetANB2(h) [2]);
   fprintf(trace, "    ref12                              %10.4f %10.4f %10.4f \n",
           SetANB12(h)[0], SetANB12(h)[1], SetANB12(h)[2]);
}

void trace_nbb(int h)
{
   extern FILE *trace;
   EPTYPE      e;
   HPTYPE      he;
   HPTYPE      hnxt;

   he   = SetBNBHe(h);
   e    = HalEdg(he);
   hnxt = HalNxt(he);
   fprintf(trace, "*** nbb[%2d].he   f%3d-%3d v%3d-%3d     %10.4f %10.4f %10.4f ***\n",
           h, e == ENIL ? -1 : FacFaceNo(LooLFace(HalWLoop(mate(he)))),
           FacFaceNo(LooLFace(HalWLoop(he))),
           VerVertexNo(HalVtx(he)),
           e == ENIL ? -1 : VerVertexNo(HalVtx(hnxt)),
           VerVCoord(HalVtx(he))[0],
           VerVCoord(HalVtx(he))[1],
           VerVCoord(HalVtx(he))[2]);
   fprintf(trace, "                                       %10.4f %10.4f %10.4f \n",
           e == ENIL ? 9999 : VerVCoord(HalVtx(hnxt))[0],
           e == ENIL ? 9999 : VerVCoord(HalVtx(hnxt))[1],
           e == ENIL ? 9999 : VerVCoord(HalVtx(hnxt))[2]);
   fprintf(trace, "    ref1                               %10.4f %10.4f %10.4f \n",
           SetBNB1(h) [0], SetBNB1(h) [1], SetBNB1(h) [2]);
   fprintf(trace, "    ref2                               %10.4f %10.4f %10.4f \n",
           SetBNB2(h) [0], SetBNB2(h) [1], SetBNB2(h) [2]);
   fprintf(trace, "    ref12                              %10.4f %10.4f %10.4f \n",
           SetANB12(h)[0], SetANB12(h)[1], SetANB12(h)[2]);
}

int MSD_booleanBorderFace(FPTYPE f, int flag)
{
   int i;

   for (i = 0; i < (flag == 1 ? nfaca : nfacb); i++)
   {
      if (flag == 1)
      {
         if (f == SetSonfa(i))
         {
            return(TRUE);
         }
      }
      else
      if (f == SetSonfb(i))
      {
         return(TRUE);
      }
   }
   return(FALSE);
}

int MSD_checkSetLoopShell(FPTYPE f, int flag)
{
   FPTYPE f2;
   LPTYPE l;
   HPTYPE he;

   if (MSD_booleanBorderFace(f, flag))
   {
      return(0);
   }
   if (!FaceBitNOK1(f))
   {
      return(1);
   }
   if (!FaceBitNOK2(f))
   {
      return(0);
   }
   SetFaceBitOK2(f);
   for (AllLoopsFace(f, l))
   {
      he = LooLEdg(l);
      do
      {
         f2 = LooLFace(HalWLoop(mate(he)));
         if (FaceBitNOK2(f2))
         {
            if (MSD_checkSetLoopShell(f2, flag) == 1)
            {
               return(1);
            }
         }
      } while ((he = HalNxt(he)) != LooLEdg(l));
   }
   return(0);
}

void MSD_setLoopShell(FPTYPE f, int flag)
{
   FPTYPE f2;
   LPTYPE l;
   HPTYPE he;

   if (MSD_booleanBorderFace(f, flag))
   {
      return;
   }
   if (!FaceBitNOK1(f))
   {
      return;
   }
   SetFaceBitOK1(f);
   for (AllLoopsFace(f, l))
   {
      he = LooLEdg(l);
      do
      {
         f2 = LooLFace(HalWLoop(mate(he)));
         if (FaceBitNOK1(f2))
         {
            MSD_setLoopShell(f2, flag);
         }
      } while ((he = HalNxt(he)) != LooLEdg(l));
   }
}

int MSD_checkBoolean(void)
{
   DPTYPE d;
   FPTYPE f;
   LPTYPE l;
   HPTYPE he;
   int    i;

   fprintf(trace, "SOLID A: ------------------------>\n");
   for (i = 0; i < nfaca; i++)
   {
      /* Reseta o flag de todas as faces                                  */
      for (AllShellsSolid(sola, d))
      {
         for (AllFacesShell(d, f))
         {
            SetFaceBitNOK1(f);
            SetFaceBitNOK2(f);
         }
      }
      d = FacFShell(SetSonfa(i));

      /* Seleciona o laco interno para analise                            */
      l = InnerLoop(SetSonfa(i));
      SetFaceBitOK1(SetSonfa(i));

      /* Atualiza o flag de todas as faces vizinhas a face selecionada    */
      he = LooLEdg(l);
      do
      {
         f = LooLFace(HalWLoop(mate(he)));
         MSD_setLoopShell(f, 1);
      } while ((he = HalNxt(he)) != LooLEdg(l));

      /* Seleciona o laco externo para analise                            */
      l = FacFLOut(SetSonfa(i));
      SetFaceBitOK2(SetSonfa(i));

      /* Verifica o flag de todas as faces vizinhas a face selecionada    */
      he = LooLEdg(l);
      do
      {
         f = LooLFace(HalWLoop(mate(he)));
         if (MSD_checkSetLoopShell(f, 1) == 1)
         {
            return(1);
         }
      } while ((he = HalNxt(he)) != LooLEdg(l));
   }

   fprintf(trace, "SOLID B: ------------------------>\n");
   for (i = 0; i < nfacb; i++)
   {
      /* Reseta o flag de todas as faces                                  */
      for (AllShellsSolid(solb, d))
      {
         for (AllFacesShell(d, f))
         {
            SetFaceBitNOK1(f);
            SetFaceBitNOK2(f);
         }
      }
      d = FacFShell(SetSonfb(i));

      /* Seleciona o laco interno para analise                            */
      l = InnerLoop(SetSonfb(i));
      SetFaceBitOK1(SetSonfb(i));

      /* Atualiza o flag de todas as faces vizinhas a face selecionada    */
      he = LooLEdg(l);
      do
      {
         f = LooLFace(HalWLoop(mate(he)));
         MSD_setLoopShell(f, 2);
      } while ((he = HalNxt(he)) != LooLEdg(l));

      /* Seleciona o laco externo para analise                            */
      l = FacFLOut(SetSonfb(i));
      SetFaceBitOK2(SetSonfb(i));

      /* Verifica o flag de todas as faces vizinhas a face selecionada    */
      he = LooLEdg(l);
      do
      {
         f = LooLFace(HalWLoop(mate(he)));
         if (MSD_checkSetLoopShell(f, 2) == 1)
         {
            return(1);
         }
      } while ((he = HalNxt(he)) != LooLEdg(l));
   }
   return(0);
}

#endif
