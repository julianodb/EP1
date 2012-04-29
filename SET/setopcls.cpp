/* 
  PROJETO USPDesigner
  MODULO: SET (Operadores Booleanos)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: setopcls.cpp
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

/* Vertex neighborhood classification */
void MSD_lowSetOpClassify(int op)
{
   int i;

#ifdef TRACE_BOOLEAN
   fprintf(trace, "\n\n\n");
   fprintf(trace, "*************************************************\n");
   fprintf(trace, "*                                               *\n");
   fprintf(trace, "*                 SETOPCLASSIFY                 *\n");
   fprintf(trace, "*                                               *\n");
   fprintf(trace, "*************************************************\n");
   fprintf(trace, "\n");
#endif

#ifdef __VIRTUAL_MEM
   /*
    *  Fecha temporariamente a area de memoria da analise Vertex-Vertex
    */
   fclose(SET03VirtualArray->file);

   /*
    *  Abre a area de memoria relativa a analise da vizinhanca Vertex-Face
    */
   if (MSD_lowSetOpInitiate3() == ERROR)
   {
      fprintf(stderr, "setop: 3 - nao conseguiu abrir arquivos de memoria virtual!\n");
      return;
   }

   /*
    *  Abre a area de memoria relativa a armazenamento de ponteiros
    */
   if (MSD_lowSetOpInitiate5() == ERROR)
   {
      fprintf(stderr, "setop: 5 - nao conseguiu abrir arquivos de memoria virtual!\n");
      return;
   }
#endif

   nedga = nedgb = 0;

   for (i = 0; i < nvtxa; ++i)
   {
      MSD_lowVtxFacClassify(SetAVFv(i), SetAVFf(i), op, 0);
   }
   for (i = 0; i < nvtxb; ++i)
   {
      MSD_lowVtxFacClassify(SetBVFv(i), SetBVFf(i), op, 1);
   }

#ifdef __VIRTUAL_MEM
   /*
    *  Fecha a area de memoria relativa a analise Vertex-Face
    */
   MSD_lowSetOpTerminate2();

   /*
    *  Fecha a area de memoria relativa a analise da vizinhanca Vertex-Face
    */
   MSD_lowSetOpTerminate3();

   /*
    *  Abre a area de memoria da analise Vertex-Vertex
    */
   if ((SET03VirtualArray->file = fopen("SETOP03.TMP", "r+b")) == (FILE *)NIL)
   {
      fprintf(stderr, "setop: 3!- nao conseguiu abrir arquivos de memoria virtual!\n");
      return;
   }

   /*
    *  Abre a area de memoria relativa a analise da vizinhanca Vertex-Vertex
    */
   if (MSD_lowSetOpInitiate4() == ERROR)
   {
      fprintf(stderr, "setop: 4 - nao conseguiu abrir arquivos de memoria virtual!\n");
      return;
   }
#endif

   for (i = 0; i < nvvtx; ++i)
   {
      MSD_lowVtxVtxClassify(SetVVva(i), SetVVvb(i), op);
   }

#ifdef __VIRTUAL_MEM
   /*
    *  Fecha a area de memoria relativa a analise da vizinhanca Vertex-Vertex
    */
   MSD_lowSetOpTerminate4();

   /*
    *  Fecha a area de memoria relativa a analise Vertex-Vertex
    */
   MSD_lowSetOpTerminate1();
#endif


#ifdef TRACE_BOOLEAN
   fprintf(trace, "\n");
   fprintf(trace, "*** setopclassify *** - nedga = %2i\n", nedga);
   fprintf(trace, "*** setopclassify *** - nedgb = %2i\n", nedgb);
   fprintf(trace, "*** estrutura sonea ***\n");
   for (i = 0; i < nedga; i++)
   {
      trace_e(SetSonea(i));
   }
   fprintf(trace, "*** estrutura soneb ***\n");
   for (i = 0; i < nedgb; i++)
   {
      trace_e(SetSoneb(i));
   }
#endif
}

/* Vertex-vertex neighborhood classifier */
void MSD_lowVtxVtxClassify(VPTYPE va, VPTYPE vb, int op)
{
#ifdef TRACE_BOOLEAN2
   int         i;
   static char *ff[] = { "OUT", "ON ", "IN " };

   fprintf(trace, "\n");
   if (op == USP_UNION)
   {
      fprintf(trace, "*** Caso vtxvtx - Operacao UNIAO\n");
   }
   if (op == USP_INTERSECT)
   {
      fprintf(trace, "*** Caso vtxvtx - Operacao INTERSECCAO\n");
   }
   if (op == USP_DIFFERENCE)
   {
      fprintf(trace, "*** Caso vtxvtx - Operacao DIFERENCA\n");
   }
#endif

   MSD_lowSetOpVVGetNeighborhood(va, vb);
#ifdef TRACE_BOOLEAN2
   fprintf(trace, "\n*** sgetneighborhood ***\n");
   fprintf(trace, "*** Possuimos apenas setores cuja interseccao tambem\n");
   fprintf(trace, "***  intersecciona os dois setores!!\n");
   for (i = 0; i < nsectors; ++i)
   {
      trace_sec(i);
   }
#endif

   MSD_lowSetOpVVReclassifySectors(op);
#ifdef TRACE_BOOLEAN2
   fprintf(trace, "\n\n*** sreclassifyonsectors ***\n");
   for (i = 0; i < nsectors; ++i)
   {
      trace_sec(i);
   }
#endif

   MSD_lowSetOpVVReclassifyEdges(op);
#ifdef TRACE_BOOLEAN2
   fprintf(trace, "\n\n*** sbreclasifyonedges ***\n");
   for (i = 0; i < nsectors; ++i)
   {
      trace_sec(i);
   }
#endif

   MSD_lowSetOpVVInsertNullEdges(op);
}

void MSD_lowVtxFacClassify(VPTYPE v, FPTYPE f, int op, int flag)
{
   vector feq;

#ifdef TRACE_BOOLEAN2
   int i;

   fprintf(trace, "\n");
   if (op == USP_UNION)
   {
      fprintf(trace, "*** Caso vtxfac - Operacao UNIAO\n");
   }
   if (op == USP_INTERSECT)
   {
      fprintf(trace, "*** Caso vtxfac - Operacao INTERSECCAO\n");
   }
   if (op == USP_DIFFERENCE)
   {
      fprintf(trace, "*** Caso vtxfac - Operacao DIFERENCA\n");
   }
   if (flag == 0)
   {
      fprintf(trace, "*** Vertice de A e Face de B\n");
   }
   if (flag == 1)
   {
      fprintf(trace, "*** Vertice de B e Face de A\n");
   }
#endif

   veccopy(feq, FacFeq(f));
   MSD_lowSetOpVFGetNeighborhood(v, feq);
#ifdef TRACE_BOOLEAN2
   fprintf(trace, "\n*** vizinhanca ***\n");
   trace_f(f);
   trace_v(v);
   for (i = 0; i < nnbr; ++i)
   {
      trace_he(SetNBRSector(i));
   }
   fprintf(trace, "\n*** getneighborhood ***\n");
   for (i = 0; i < nnbr; ++i)
   {
      trace_nbr(i);
   }
#endif

   MSD_lowSetOpVFReclassifyOnSectors(feq, op, flag);
#ifdef TRACE_BOOLEAN2
   fprintf(trace, "*** reclassifyonsectors ***\n");
   for (i = 0; i < nnbr; ++i)
   {
      trace_nbr(i);
   }
#endif

   MSD_lowSetOpVFReclassifyOnEdges();
#ifdef TRACE_BOOLEAN2
   fprintf(trace, "*** breclasifyonedges ***\n");
   for (i = 0; i < nnbr; ++i)
   {
      trace_nbr(i);
   }
#endif

   MSD_lowSetOpVFInsertNullEdges(f, flag);
}

void MSD_lowSetOpVFGetNeighborhood(VPTYPE v, vector SP)
{
   HPTYPE he;

#ifndef __VIRTUAL_MEM
   VTYPE bisect;
#else
   int    c1;
   VPTYPE bisect;
   vector bisectcor;

   bisect = MSD_getMemVirtual(VERTEX);
#endif

   nnbr = 0;
   he   = VerVEdge(v);
   do
   {
      SetNBRC1(nnbr) =
         -comp(MSD_lowDistancePlanePoint(SP, HalVtx(HalNxt(he))), 0.0, EPS);
      SetNBRSector(nnbr) = he;
      nnbr++;

#ifndef __VIRTUAL_MEM
      if (nnbr == MAXEDGESFORVERTEX)
      {
         fprintf(stderr, "nbr: falta memoria!\n");
         exit(1);
      }
#endif

      if (MSD_lowCheckWideness(he) == 1)
      {
#ifndef __VIRTUAL_MEM
         MSD_lowBisector(he, bisect.vcoord);
         SetNBRSector(nnbr) = he;
         SetNBRC1(nnbr)     = SetNBRC1(nnbr - 1);
         SetNBRC1(nnbr - 1) =
            -comp(MSD_lowDistancePlanePoint(SP, &bisect), 0.0, EPS);
         nnbr++;

         if (nnbr == MAXEDGESFORVERTEX)
         {
            fprintf(stderr, "set_nbr: falta memoria!\n");
            exit(1);
         }
#else
         MSD_lowBisector(he, bisectcor);
         veccopy(VerVCoord(bisect), bisectcor);
         SetNBRSector(nnbr) = he;
         c1                 = SetNBRC1(nnbr - 1);
         SetNBRC1(nnbr)     = c1;
         SetNBRC1(nnbr - 1) =
            -comp(MSD_lowDistancePlanePoint(SP, bisect), 0.0, EPS);
         nnbr++;
#endif
      }
   } while ((he = HalNxt(mate(he))) != VerVEdge(v));

#ifdef __VIRTUAL_MEM
   MSD_putMemVirtual(bisect, VERTEX);
#endif
}

void MSD_lowSetOpVFReclassifyOnSectors(vector SP, int op, int flag)
{
   FPTYPE f;
   vector c;
   int    i;

   for (i = 0; i < nnbr; i++)
   {
      f = LooLFace(HalWLoop(mate(SetNBRSector(i))));
      cross(c, FacFeq(f), SP);
      if (comp(dot(c, c), 0.0, EPS * EPS) == 0)     /* test coplanarity */
      {
         if (comp(dot(FacFeq(f), SP), 0.0, EPS) == 1)
         {
            /* test orientation */
            if (flag)                                      /* identical */
            {
               SetNBRC1(i)              = (op == USP_UNION) ? USP_IN : USP_OUT;
               SetNBRC1((i + 1) % nnbr) = (op == USP_UNION) ? USP_IN : USP_OUT;
               if (SetNBRSector((i + 1) % nnbr) == SetNBRSector((i + 2) % nnbr))
               {
                  SetNBRC1((i + 2) % nnbr) = (op == USP_UNION) ? USP_IN : USP_OUT;
               }
            }
            else
            {
               SetNBRC1(i)              = (op == USP_UNION) ? USP_OUT : USP_IN;
               SetNBRC1((i + 1) % nnbr) = (op == USP_UNION) ? USP_OUT : USP_IN;
               if (SetNBRSector((i + 1) % nnbr) == SetNBRSector((i + 2) % nnbr))
               {
                  SetNBRC1((i + 2) % nnbr) = (op == USP_UNION) ? USP_OUT : USP_IN;
               }
            }
         }
         else                                              /* opposite */
         {
            if (flag)
            {
               SetNBRC1(i)              = (op == USP_UNION) ? USP_IN : USP_OUT;
               SetNBRC1((i + 1) % nnbr) = (op == USP_UNION) ? USP_IN : USP_OUT;
               if (SetNBRSector((i + 1) % nnbr) == SetNBRSector((i + 2) % nnbr))
               {
                  SetNBRC1((i + 2) % nnbr) = (op == USP_UNION) ? USP_IN : USP_OUT;
               }
            }
            else
            {
               SetNBRC1(i)              = (op == USP_UNION) ? USP_IN : USP_OUT;
               SetNBRC1((i + 1) % nnbr) = (op == USP_UNION) ? USP_IN : USP_OUT;
               if (SetNBRSector((i + 1) % nnbr) == SetNBRSector((i + 2) % nnbr))
               {
                  SetNBRC1((i + 2) % nnbr) = (op == USP_UNION) ? USP_IN : USP_OUT;
               }
            }
         }
      }
   }
}

void MSD_lowSetOpVFReclassifyOnEdges(void)
{
   int i;

   for (i = 0; i < nnbr; ++i)
   {
      if (SetNBRC1(i) == USP_ON)
      {
         if (SetNBRC1((nnbr + i - 1) % nnbr) == USP_IN)
         {
            if (SetNBRC1((i + 1) % nnbr) == USP_IN)
            {
               SetNBRC1(i) = USP_IN;
            }
            else
            {
               SetNBRC1(i) = USP_IN;
            }
         }
         else
         {
            if (SetNBRC1((i + 1) % nnbr) == USP_IN)
            {
               SetNBRC1(i) = USP_IN;
            }
            else
            {
               SetNBRC1(i) = USP_OUT;
            }
         }
      }
   }
}

void MSD_lowSetOpVFInsertNullEdges(FPTYPE f, int flag)
{
   HPTYPE tail;
   HPTYPE head;
   int    i, start;

   i = 0;
   while (!(SetNBRC1(i) == USP_OUT && SetNBRC1((i + 1) % nnbr) == USP_IN))
   {
      if (++i == nnbr)
      {
         return;
      }
   }
   start = i;

   head = SetNBRSector((i + 1) % nnbr);

   while (1)
   {
      while (!(SetNBRC1(i) == USP_IN && SetNBRC1((i + 1) % nnbr) == USP_OUT))
      {
         i = (i + 1) % nnbr;
      }
      tail = SetNBRSector((i + 1) % nnbr);


#ifdef TRACE_BOOLEAN2
      fprintf(trace, "*** Aplicando o operador MEV\n");
      trace_he(head);
      trace_he(tail);
#endif
      MSD_lowMEV(head, tail, maxv++, VerVCoord(HalVtx(head))[0],
                 VerVCoord(HalVtx(head))[1],
                 VerVCoord(HalVtx(head))[2]);

      if (flag == 0)
      {
#ifdef TRACE_BOOLEAN2
         fprintf(trace, "*** Aresta Solido A inserida na SONEA\n");
         trace_e(HalEdg(HalPrv(head)));
#endif

         SetSonea(nedga) = HalEdg(HalPrv(head));
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
         fprintf(trace, "*** Aresta Solido B inserida na SONEB\n");
         trace_e(HalEdg(HalPrv(head)));
#endif

         SetSoneb(nedgb) = HalEdg(HalPrv(head));
         nedgb++;

#ifndef __VIRTUAL_MEM
         if (nedgb == MAXNULLEDGES)
         {
            fprintf(stderr, "soneb: falta memoria!\n");
            exit(1);
         }
#endif
      }

      MSD_lowSetOpMakeRing(f, VerVCoord(HalVtx(head))[0],
                           VerVCoord(HalVtx(head))[1],
                           VerVCoord(HalVtx(head))[2], flag);

      /*
       *      while (!(SetNBRC1(i) == USP_IN && SetNBRC1((i+1)%nnbr) == USP_OUT))
       */
      while (!(SetNBRC1(i) == USP_OUT && SetNBRC1((i + 1) % nnbr) == USP_IN))
      {
         if ((i = (i + 1) % nnbr) == start)
         {
            return;
         }
      }
   }
}

void MSD_lowSetOpMakeRing(FPTYPE f, double x, double y, double z, int flag)
{
   HPTYPE he1;
   HPTYPE he2;

#ifdef TRACE_BOOLEAN2
   fprintf(trace, "*** Aplicando o operador MEV\n");
   trace_he(LooLEdg(FacFLOut(f)));
   trace_he(LooLEdg(FacFLOut(f)));
#endif

   MSD_lowMEV(LooLEdg(FacFLOut(f)), LooLEdg(FacFLOut(f)), maxv++, x, y, z);
   he1 = HalPrv(LooLEdg(FacFLOut(f)));

#ifdef TRACE_BOOLEAN2
   fprintf(trace, "*** Aplicando o operador MEV\n");
   trace_he(he1);
   trace_he(he1);
#endif

   MSD_lowMEV(he1, he1, maxv++, x, y, z);
   he2 = HalPrv(he1);

#ifdef TRACE_BOOLEAN2
   fprintf(trace, "*** Aplicando o operador KEMR\n");
   trace_he(HalPrv(HalPrv(HalPrv(he1))));
   trace_he(he1);
#endif

   MSD_lowKEMR(HalPrv(HalPrv(HalPrv(he1))), he1);

   if (flag == 1)
   {
#ifdef TRACE_BOOLEAN2
      fprintf(trace, "*** Aresta Solido A inserida na SONEA\n");
      trace_e(HalEdg(he2));
#endif

      SetSonea(nedga) = HalEdg(he2);
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
      fprintf(trace, "*** Aresta Solido B inserida na SONEB\n");
      trace_e(HalEdg(he2));
#endif

      SetSoneb(nedgb) = HalEdg(he2);
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
