/* 
  PROJETO USPDesigner
  MODULO: SET (Operadores Booleanos)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: setsinsn.cpp
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
#include "lowgeome.h"
#include "eulerops.h"
#include "vectorop.h"
#include "sethmain.h"

/* Insertion of null edges */
void MSD_lowSetOpVVInsertNullEdges(int op)
{
   HPTYPE ha1;
   HPTYPE ha2;
   HPTYPE hb1;
   HPTYPE hb2;
   int    i, j, /* n1, n2, n3, n4, flag, */ InterCont;

   /*
    *  Verifica se existem mais que dois pontos para realizar a
    *  divisao.
    */
   for (InterCont = j = 0; j < nsectors; j++)
   {
      if (SetSecInter(j) == 1)
      {
         InterCont++;
      }
   }

   if ((InterCont != 0) && (InterCont != 2))
   {
      fprintf(stderr, "%d", InterCont);
   }

   i = /* n1 = n2 = n3 = n4 = */ 0;
   while (1)
   {
      while (SetSecInter(i) == 0)
      {
         if (++i > nsectors - 1)
         {
            return;
         }
      }

      ha1 = hb1 = ha2 = hb2 = HNIL;
      if (SetSecs1a(i) == USP_IN)
      {
         ha1 = SetANBHe(SetSecA(i));
      }
      else
      {
         ha2 = SetANBHe(SetSecA(i));
      }
      if (SetSecs1b(i) == USP_OUT)
      {
         hb1 = SetBNBHe(SetSecB(i));
      }
      else
      {
         hb2 = SetBNBHe(SetSecB(i));
      }

      if (++i > nsectors - 1)
      {
         return;
      }

      while (SetSecInter(i) == 0)
      {
         if (++i > nsectors - 1)
         {
            return;
         }
      }

      if (SetSecs1a(i) == USP_IN)
      {
         ha1 = SetANBHe(SetSecA(i));
      }
      else
      {
         ha2 = SetANBHe(SetSecA(i));
      }
      if (SetSecs1b(i) == USP_OUT)
      {
         hb1 = SetBNBHe(SetSecB(i));
      }
      else
      {
         hb2 = SetBNBHe(SetSecB(i));
      }
      i++;

      if ((ha1 == HNIL) || (ha2 == HNIL) || (hb1 == HNIL) || (hb2 == HNIL))
      {
         for (InterCont = j = 0; j < nsectors; j++)
         {
            if (SetSecInter(j) == 1)
            {
               InterCont++;
            }
         }
         fprintf(stderr, "*%d*", InterCont);
         if (i > nsectors - 1)
         {
            return;
         }
         fprintf(stderr, "+");
         continue;
      }

      if ((ha1 == ha2) && (hb1 == hb2))
      {
         continue;
      }
      if (ha1 == ha2)
      {
         MSD_lowSetOpSepar2(ha1, 0, MSD_lowSetOpGetOrient(ha1, hb1, hb2), op);
         MSD_lowSetOpSepar1(hb1, hb2, 1);
      }
      else
      if (hb1 == hb2)
      {
         MSD_lowSetOpSepar2(hb1, 1, MSD_lowSetOpGetOrient(hb1, ha2, ha1), op);
         MSD_lowSetOpSepar1(ha2, ha1, 0);
      }
      else
      {
         MSD_lowSetOpSepar1(ha2, ha1, 0);
         MSD_lowSetOpSepar1(hb1, hb2, 1);
      }
      if (i > nsectors - 1)
      {
         return;
      }
   }
}

/**************************************************************************/
/* Estamos verificando a orientacao da halfedge.                          */
/*  .ref --> aresta do solido que criara uma dangling edge.               */
/*  .he1 --> aresta do outro solido.                                      */
/*  .he2 --> aresta do outro solido.                                      */
/* ---------------------------------------------------------------------- */
int MSD_lowSetOpGetOrient(HPTYPE ref, HPTYPE he1, HPTYPE he2)
{
   HPTYPE mhe1;
   HPTYPE mhe2;
   int    retcode;

   /* Proxima halfedge no ciclo de halfedges ao redor do vertice.        */
   mhe1 = HalNxt(mate(he1));
   mhe2 = HalNxt(mate(he2));

   /* Se a aresta do outro solido tambem for uma dangling edge, entao    */
   /*   verificar a convexidade                                          */
   if ((mhe1 != he2) && (mhe2 == he1))
   {
      retcode = MSD_lowSetOpConvexEdge(he2);
   }
   else
   {
      retcode = MSD_lowSetOpConvexEdge(he1);
   }

#ifdef TRACE_BOOLEAN2
   FILE *ttrace = fopen("trace.txt", "a");
   fprintf(ttrace, "--> *******\n");
   fprintf(ttrace, "--> mhe1 != he2 --> %s\n", mhe1 != he2 ? "TRUE" : "FALSE");
   fprintf(ttrace, "--> mhe2 == he1 --> %s\n", mhe2 == he1 ? "TRUE" : "FALSE");
   fprintf(ttrace, "--> MSD_lowSetOpConvexEdge(ref) = %d\n", MSD_lowSetOpConvexEdge(ref));
   fprintf(ttrace, "--> MSD_lowSetOpConvexEdge(he2) = %d\n", MSD_lowSetOpConvexEdge(he2));
   fprintf(ttrace, "--> MSD_lowSetOpConvexEdge(he1) = %d\n", MSD_lowSetOpConvexEdge(he1));
   fprintf(ttrace, "--> MSD_lowSetOpSectorWide(mhe1, 0) = %d\n", MSD_lowSetOpSectorWide(mhe1, 0));
   fprintf(ttrace, "--> MSD_lowSetOpSectorWide(mhe2, 0) = %d\n", MSD_lowSetOpSectorWide(mhe2, 0));
   fprintf(ttrace, "--> MSD_lowSetOpSectorWide(he2, 0) = %d\n", MSD_lowSetOpSectorWide(he2, 0));
   fprintf(ttrace, "--> MSD_lowSetOpSectorWide(he1, 0) = %d\n", MSD_lowSetOpSectorWide(he1, 0));
   fprintf(ttrace, "--> MSD_lowSetOpSectorWide(ref, 0) = %d\n", MSD_lowSetOpSectorWide(ref, 0));
   int ret = retcode;
   if ((MSD_lowSetOpSectorWide(mhe1, 0) == 1) && (MSD_lowSetOpSectorWide(ref, 0) == 1))
   {
      ret = 1 - retcode;
   }
   fprintf(ttrace, "--> return code = %d\n\n", 1 - ret);
   fclose(ttrace);
#endif

   if ((MSD_lowSetOpSectorWide(mhe1, 0) == 1) && (MSD_lowSetOpSectorWide(ref, 0) == 1))
   {
      retcode = 1 - retcode;
   }

   if ((mhe1 != he2) && (mhe2 == he1) && (MSD_lowSetOpConvexEdge(ref) == 0) &&
       (MSD_lowSetOpConvexEdge(he1) == 0) &&
       (MSD_lowSetOpConvexEdge(he2) == 0) &&
       (MSD_lowSetOpSectorWide(mhe1, 0) == 1) &&
       (MSD_lowSetOpSectorWide(mhe2, 0) == 1) &&
       (MSD_lowSetOpSectorWide(he2, 0) == 1) &&
       (MSD_lowSetOpSectorWide(he1, 0) == 1) &&
       (MSD_lowSetOpSectorWide(ref, 0) == 0))
   {
      return(0);
   }

   return(1 - retcode);
}

int MSD_lowSetOpSectorWide(HPTYPE he, int ind)
{
   vector ref1[2], ref2[2], ref12[2];
   vector v1, v2, v3;

   veccopy(v1, VerVCoord(HalVtx(HalPrv(he))));
   veccopy(v2, VerVCoord(HalVtx(HalNxt(he))));
   veccopy(v3, VerVCoord(HalVtx(he)));
   vecminus(ref1[ind], v1, v3);
   vecminus(ref2[ind], v2, v3);
   cross(ref12[ind], ref1[ind], ref2[ind]);
   if (vecnull(ref12[ind], EPS))
   {
      return(1);
   }
   return((dot(ref12[ind], FacFeq(LooLFace(HalWLoop(he)))) > 0.0) ? 0 : 1);
}

int MSD_lowSetOpConvexEdge(HPTYPE he)
{
   HPTYPE h2;
   vector dir, cr, v1, v2, f1, f2;

   h2 = HalNxt(he);
   if (MSD_lowSetOpNullEdge(he))
   {
      h2 = HalNxt(h2);
   }
   veccopy(v1, VerVCoord(HalVtx(h2)));
   veccopy(v2, VerVCoord(HalVtx(he)));
   vecminus(dir, v1, v2);
   veccopy(f1, FacFeq(LooLFace(HalWLoop(he))));
   veccopy(f2, FacFeq(LooLFace(HalWLoop(mate(he)))));
   cross(cr, f1, f2);
   if (vecnull(cr, EPS))
   {
      return(1);
   }
   return(dot(dir, cr) < 0.0);
}

/* Separate edge-sequence from ... to */
void MSD_lowSetOpSepar1(HPTYPE from, HPTYPE to, int type)
{
   HPTYPE PrvFrom;


#ifdef TRACE_BOOLEAN2
   fprintf(trace, "*** separ1! ***\n");
#endif

   PrvFrom = HalPrv(from);

   /* recover from null edges already inserted */
   if (MSD_lowSetOpNullEdge(PrvFrom) && MSD_lowSetOpStrutNullEdge(PrvFrom))
   {
#ifdef TRACE_BOOLEAN2
      fprintf(trace, "*** existe um strut null edge na frente de from!\n");
#endif

      if (PrvFrom == EdgHe2(HalEdg(PrvFrom)))
      {
         from = HalPrv(PrvFrom);
      }
   }

   if (MSD_lowSetOpNullEdge(HalPrv(to)) && MSD_lowSetOpStrutNullEdge(HalPrv(to)))
   {
#ifdef TRACE_BOOLEAN2
      fprintf(trace, "*** existe um strut null edge na frente de to!\n");
#endif

      if (HalPrv(to) == EdgHe1(HalEdg(HalPrv(to))))
      {
         to = HalPrv(HalPrv(to));
      }
   }

   if (HalVtx(from) != HalVtx(to))
   {
      if (HalPrv(from) == mate(HalPrv(to)))
      {
         from = HalPrv(from);

#ifdef TRACE_BOOLEAN2
         fprintf(trace, "*** from e to possuem vertices diferentes\n");
#endif
      }
      else
      if (HalVtx(HalPrv(from)) == HalVtx(to))
      {
         from = HalPrv(from);

#ifdef TRACE_BOOLEAN2
         fprintf(trace, "*** from->prv e to possuem vertices iguais\n");
#endif
      }
      else
      if (HalVtx(HalPrv(to)) == HalVtx(from))
      {
         to = HalPrv(to);

#ifdef TRACE_BOOLEAN2
         fprintf(trace, "*** to->prv e from possuem vertices iguais\n");
#endif
      }
   }

#ifdef TRACE_BOOLEAN2
   fprintf(trace, "*** Aplicando o operador MEV\n");
   trace_he(from);
   trace_he(to);
#endif

   MSD_lowMEV(from, to, maxv++, VerVCoord(HalVtx(from))[0],
              VerVCoord(HalVtx(from))[1],
              VerVCoord(HalVtx(from))[2]);
   if (type == 0)
   {
#ifdef TRACE_BOOLEAN2
      fprintf(trace, "*** Aresta Solido A inserida na SONEA\n");
      trace_e(HalEdg(HalPrv(from)));
#endif

      SetSonea(nedga) = HalEdg(HalPrv(from));
      nedga++;

#ifndef __VIRTUAL_MEM
      if (nedga == MAXNULLEDGES)
      {
         fprintf(stderr, "sonea: falta memoria!\n");
         exit(1);
      }
#endif
   }
   else
   {
#ifdef TRACE_BOOLEAN2
      fprintf(trace, "*** Aresta Solido A inserida na SONEB\n");
      trace_e(HalEdg(HalPrv(from)));
#endif

      SetSoneb(nedgb) = HalEdg(HalPrv(from));
      nedgb++;

#ifndef __VIRTUAL_MEM
      if (nedgb == MAXNULLEDGES)
      {
         fprintf(stderr, "soneb: falta memoria!\n");
         exit(1);
      }
#endif
   }
}

/* separate 'interior' of the face of he */
void MSD_lowSetOpSepar2(HPTYPE he, int type, int orient, int)
{
   HPTYPE tmp;

#ifdef TRACE_BOOLEAN2
   fprintf(trace, "*** separ2! ***\n");
#endif

   if (MSD_lowSetOpNullEdge(HalPrv(he)))
   {
      if (((HalPrv(he) == EdgHe1(HalEdg(HalPrv(he)))) && orient) ||
          ((HalPrv(he) == EdgHe2(HalEdg(HalPrv(he)))) && !orient))
      {
         he = HalPrv(he);
      }

#ifdef TRACE_BOOLEAN2
      fprintf(trace, "*** existe um null edge na frente de he!\n");
#endif
   }

#ifdef TRACE_BOOLEAN2
   fprintf(trace, "*** Aplicando o operador MEV\n");
   trace_he(he);
   trace_he(he);
#endif
   MSD_lowMEV(he, he, maxv++, VerVCoord(HalVtx(he))[0],
              VerVCoord(HalVtx(he))[1],
              VerVCoord(HalVtx(he))[2]);

   /* magica */
   if (orient)
   {
#ifdef TRACE_BOOLEAN2
      fprintf(trace, "*** Realizou magica\n");
#endif

      tmp = EdgHe1(HalEdg(HalPrv(he)));
      EdgHe1(HalEdg(HalPrv(he))) = EdgHe2(HalEdg(HalPrv(he)));
      EdgHe2(HalEdg(HalPrv(he))) = tmp;
   }

   if (type == 0)
   {
#ifdef TRACE_BOOLEAN2
      fprintf(trace, "*** Aresta Solido A inserida na SONEA\n");
      trace_e(HalEdg(HalPrv(he)));
#endif

      SetSonea(nedga) = HalEdg(HalPrv(he));
      nedga++;

#ifndef __VIRTUAL_MEM
      if (nedga == MAXNULLEDGES)
      {
         fprintf(stderr, "sonea: falta memoria!\n");
         exit(1);
      }
#endif
   }
   else
   {
#ifdef TRACE_BOOLEAN2
      fprintf(trace, "*** Aresta Solido A inserida na SONEB\n");
      trace_e(HalEdg(HalPrv(he)));
#endif

      SetSoneb(nedgb) = HalEdg(HalPrv(he));
      nedgb++;

#ifndef __VIRTUAL_MEM
      if (nedgb == MAXNULLEDGES)
      {
         fprintf(stderr, "soneb: falta memoria!\n");
         exit(1);
      }
#endif
   }
}

int MSD_lowSetOpStrutNullEdge(HPTYPE he)
{
   if ((he == HalNxt(mate(he))) || (he == HalPrv(mate(he))))
   {
      return(1);
   }
   return(0);
}

int MSD_lowSetOpNullEdge(HPTYPE he)
{
   return(MSD_lowVtxVtxContainment(HalVtx(he), HalVtx(HalNxt(he))));
}
