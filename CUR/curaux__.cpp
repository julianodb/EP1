/* 
  PROJETO USPDesigner
  MODULO: CUR (Curve Manipulation)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: curaux__.cpp
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
#include "mensagem.h"
#include "memvirtu.h"
#include "lowparam.h"
#include "lowmacro.h"
#include "curvgeom.h"
#include "lowsolid.h"
#include "eulerops.h"
#include "mancommd.h"
#include "vectorop.h"
#include "genfunc_.h"
#include "shpshape.h"

#ifdef __curved_env__

 #ifdef __Extended_Version
void hget_he(void)
{
   int  v1, v2, fn, memohe;
   char sn[20];

   while (5 != sscanf(restbuf, "%s %d %d %d %d", sn, &v1, &v2, &fn,
                      &memohe))
   {
      printf("Solidname V1 V2 F Memo\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   get_he(sn, v1, v2, fn, memohe);
}

int get_he(char *sn, Id v1, Id v2, Id fn, int memohe)
{
   SPTYPE s;
   FPTYPE f1;
   HPTYPE he;

   if ((s = MSD_getSolid(MSD_getSolidIdFromName(sn))) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_GetHE, sn);
      return(ERROR);
   }
   if ((f1 = MSD_getFace(s, fn)) == FNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouFace, NAM_GetHE, fn, sn);
      return(ERROR);
   }
   if (MSD_getEdge(f1, v1, v2, &he) == LNIL)
   {
      if (MSD_getEdge(f1, v2, v1, &he) == LNIL)
      {
         fprintf(stderr, MEN_NaoEncontrouArestaV, NAM_GetHE, v1, v2, sn);
         return(ERROR);
      }
   }
   lget_he(he, memohe);
   return(SUCCESS);
}

void lget_he(HPTYPE he, int memohe)
{
   HPTYPE heaux;

   heaux            = he;
   he_store[memohe] = heaux;
}

void hget_mate(void)
{
   int memohe, memomate;

   while (2 != sscanf(restbuf, "%d %d", &memohe, &memomate))
   {
      printf("Memo_he Memo_mate\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   get_mate(memohe, memomate);
}

int get_mate(int memohe, int memomate)
{
   HPTYPE he;

   if ((memohe > MAXMEMO) || (memohe < 0))
   {
      fprintf(stderr, MEN_MemoriaInvalida, NAM_GetMate, memohe);
      return(ERROR);
   }
   if ((memomate > MAXMEMO) || (memomate < 0))
   {
      fprintf(stderr, MEN_MemoriaInvalida, NAM_GetMate, memomate);
      return(ERROR);
   }

   if ((he = he_store[memohe]) == HNIL)
   {
      fprintf(stderr, MEN_MemoriaInvalida, NAM_GetMate, memohe);
      return(ERROR);
   }

   lget_mate(he, memomate);
   return(SUCCESS);
}

void lget_mate(HPTYPE he, int memomate)
{
   HPTYPE heaux;

   heaux              = mate(he);
   he_store[memomate] = heaux;
}

void hget_next(void)
{
   int memohe, memonext;

   while (2 != sscanf(restbuf, "%d %d", &memohe, &memonext))
   {
      printf("Memo_he Memo_next\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   get_next(memohe, memonext);
}

int get_next(int memohe, int memonext)
{
   HPTYPE he;

   if ((memohe > MAXMEMO) || (memohe < 0))
   {
      fprintf(stderr, MEN_MemoriaInvalida, NAM_GetNext, memohe);
      return(ERROR);
   }

   if ((memonext > MAXMEMO) || (memonext < 0))
   {
      fprintf(stderr, MEN_MemoriaInvalida, NAM_GetNext, memonext);
      return(ERROR);
   }

   if ((he = he_store[memohe]) == HNIL)
   {
      fprintf(stderr, MEN_MemoriaInvalida, NAM_GetNext, memohe);
      return(ERROR);
   }

   lget_next(he, memonext);
   return(SUCCESS);
}

void lget_next(HPTYPE he, int memonext)
{
   HPTYPE heaux;

   heaux              = HalNxt(he);
   he_store[memonext] = heaux;
}

void hget_prev(void)
{
   int memohe, memoprev;

   while (2 != sscanf(restbuf, "%d %d", &memohe, &memoprev))
   {
      printf("Memo_he Memo_prev\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   get_prev(memohe, memoprev);
}

int get_prev(int memohe, int memoprev)
{
   HPTYPE he;

   if ((memohe > MAXMEMO) || (memohe < 0))
   {
      fprintf(stderr, MEN_MemoriaInvalida, NAM_GetPrev, memohe);
      return(ERROR);
   }

   if ((memoprev > MAXMEMO) || (memoprev < 0))
   {
      fprintf(stderr, MEN_MemoriaInvalida, NAM_GetPrev, memoprev);
      return(ERROR);
   }

   if ((he = he_store[memohe]) == HNIL)
   {
      fprintf(stderr, MEN_MemoriaInvalida, NAM_GetPrev, memohe);
      return(ERROR);
   }

   lget_prev(he, memoprev);
   return(SUCCESS);
}

void lget_prev(HPTYPE he, int memoprev)
{
   HPTYPE heaux;

   heaux              = HalPrv(he);
   he_store[memoprev] = heaux;
}

void hcmef(void)
{
   int memohe1, memohe2;

   while (2 != sscanf(restbuf, "%d %d", &memohe1, &memohe2))
   {
      printf("Memo_he1 Memo_he2\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   cmef(memohe1, memohe2);
}

int cmef(int memohe1, int memohe2)
{
   SPTYPE s;
   HPTYPE he1;
   HPTYPE he2;
   Id     fn;

   if ((memohe1 > MAXMEMO) || (memohe1 < 0))
   {
      fprintf(stderr, MEN_MemoriaInvalida, NAM_DoMEF, memohe1);
      return(ERROR);
   }

   if ((memohe2 > MAXMEMO) || (memohe2 < 0))
   {
      fprintf(stderr, MEN_MemoriaInvalida, NAM_DoMEF, memohe2);
      return(ERROR);
   }

   if ((he1 = he_store[memohe1]) == HNIL)
   {
      fprintf(stderr, MEN_MemoriaInvalida, NAM_DoMEF, memohe1);
      return(ERROR);
   }

   if ((he2 = he_store[memohe2]) == HNIL)
   {
      fprintf(stderr, MEN_MemoriaInvalida, NAM_DoMEF, memohe2);
      return(ERROR);
   }

   s  = SheSSolids(FacFShell(LooLFace(HalWLoop(he1))));
   fn = MSD_getMaxFaceId(s);

   MSD_lowMEF(he1, he2, fn);
   MSD_lowSetNormal(s, TRUE);
   MSD_lowSetEdgeAngle(s);
   MSD_lowSetInfo(s);

   return(SUCCESS);
}

void hcmev(void)
{
   int   memohe1, memohe2;
   float x, y, z;

   while (5 != sscanf(restbuf, "%d %d %f %f %f", &memohe1, &memohe2,
                      &x, &y, &z))
   {
      printf("Memo_he1 Memo_he2 Xnew Ynew Znew\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   cmev(memohe1, memohe2, x, y, z);
}

int cmev(int memohe1, int memohe2, real x, real y, real z)
{
   SPTYPE s;
   HPTYPE he1;
   HPTYPE he2;
   Id     vn;

   if ((memohe1 > MAXMEMO) || (memohe1 < 0))
   {
      fprintf(stderr, MEN_MemoriaInvalida, NAM_DoMEV, memohe1);
      return(ERROR);
   }

   if ((memohe2 > MAXMEMO) || (memohe2 < 0))
   {
      fprintf(stderr, MEN_MemoriaInvalida, NAM_DoMEV, memohe2);
      return(ERROR);
   }

   if ((he1 = he_store[memohe1]) == HNIL)
   {
      fprintf(stderr, MEN_MemoriaInvalida, NAM_DoMEV, memohe1);
      return(ERROR);
   }

   if ((he2 = he_store[memohe2]) == HNIL)
   {
      fprintf(stderr, MEN_MemoriaInvalida, NAM_DoMEV, memohe2);
      return(ERROR);
   }

   s  = SheSSolids(FacFShell(LooLFace(HalWLoop(he1))));
   vn = MSD_getMaxVertexId(s) + 1;

   MSD_lowMEV(he1, he2, vn, x, y, z);
   MSD_lowSetNormal(s, TRUE);
   MSD_lowSetEdgeAngle(s);
   MSD_lowSetInfo(s);

   return(SUCCESS);
}

void cnewloc(void)
{
   int   memohe;
   float x, y, z;

   while (sscanf(restbuf, "%d %f %f %f", &memohe, &x, &y, &z) != 4)
   {
      printf("cur_newloc: Memohe x y z");
      if (!lineins("? "))
      {
         return;
      }
   }
   ccnewloc(memohe, x, y, z);
}

int ccnewloc(int memohe, real x, real y, real z)
{
   SPTYPE s;
   HPTYPE he;
   VPTYPE v;

   if ((memohe > MAXMEMO) || (memohe < 0))
   {
      fprintf(stderr, MEN_MemoriaInvalida, NAM_DoNewLoc, memohe);
      return(ERROR);
   }

   if ((he = he_store[memohe]) == HNIL)
   {
      fprintf(stderr, MEN_MemoriaInvalida, NAM_DoNewLoc, memohe);
      return(ERROR);
   }

   s = SheSSolids(FacFShell(LooLFace(HalWLoop(he))));
   v = HalVtx(he);
   MSD_lowNewVertexLocation(s, v, x, y, z);
   return(SUCCESS);
}

void hshow_memo(void)
{
   int memohe;

   while (1 != sscanf(restbuf, "%d", &memohe))
   {
      printf("Memo_he, -1: show all\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   lshow_memo(memohe);
}

void lshow_memo(int memohe)
{
   HPTYPE heaux;
   Id     v1, v2, fn;
   int    i;

   printf("Halfedge Memory:\n");
   if (memohe == -1)
   {
      for (i = 0; i < MAXMEMO; i++)
      {
         if ((heaux = he_store[i]) == HNIL)
         {
            printf("   Memo[%d] = empty\n", i);
         }
         else
         {
            v1 = VerVertexNo(HalVtx(heaux));
            v2 = VerVertexNo(HalVtx(HalNxt(heaux)));
            fn = FacFaceNo(LooLFace(HalWLoop(heaux)));
            printf("   Memo[%d] = v%d->v%d on face %d\n", i, v1, v2, fn);
         }
      }
   }
   else
   {
      if ((heaux = he_store[memohe]) == HNIL)
      {
         printf("   Memo[%d] = empty\n", memohe);
      }
      else
      {
         v1 = VerVertexNo(HalVtx(heaux));
         v2 = VerVertexNo(HalVtx(HalNxt(heaux)));
         fn = FacFaceNo(LooLFace(HalWLoop(heaux)));
         printf("   Memo[%d] = v%d->v%d on face %d\n", memohe, v1, v2, fn);
      }
   }
}

void cputctag(void)
{
   int    tag, memohe;
   char   sn[20];
   SPTYPE s;
   HPTYPE he;
   CURVE  *curv;

   while (3 != sscanf(restbuf, "%s %d %d", sn, &tag, &memohe))
   {
      printf("PUTCTAG: sname tag memo \n");
      if (!lineins("? "))
      {
         return;
      }
   }
   if ((s = MSD_lowFindSolidByName(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_DoPutTag, sn);
      return;
   }
   if ((curv = MSD_getCurve(s, tag)) == (CURVE *)NIL)
   {
      fprintf(stderr, MEN_NaoEncontrouTag, NAM_DoPutTag, tag, sn);
      return;
   }
   if ((he = he_store[memohe]) == HNIL)
   {
      fprintf(stderr, MEN_MemoriaInvalida, NAM_DoPutTag, memohe);
      return;
   }
   MSD_lowSetCurveAtributeToEdge(he, curv);
}

void ceval_polyline(void)
{
   int    memohe, nsegm;
   HPTYPE he;
   SPTYPE s;

   while (2 != sscanf(restbuf, "%d %d", &memohe, &nsegm))
   {
      printf("CEVAL_POLYLINE: memohe nsegm\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   if ((he = he_store[memohe]) == HNIL)
   {
      fprintf(stderr, MEN_MemoriaInvalida, NAM_DoEvalEdge, memohe);
      return;
   }
   if (nsegm < 2)
   {
      fprintf(stderr, MEN_NumeroDeSegmentosInvalido, NAM_DoEvalEdge);
      return;
   }

   MSD_lowEvaluateCurvePolyline(he, nsegm);
   s = SheSSolids(FacFShell(LooLFace(HalWLoop(he))));
   MSD_lowSetNormal(s, TRUE);
   MSD_lowSetEdgeAngle(s);
   MSD_lowSetInfo(s);

   if (TraceBoundaryStructure())
   {
      fprintf(stderr, MEN_MSDAbortou, NAM_DoEvalEdge);
      exit(1);
   }
}

 #endif

#endif
