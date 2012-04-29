/* 
  PROJETO USPDesigner
  MODULO: RND (Arredondamento)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: rndround.cpp
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
#include "mensagem.h"
#include "memvirtu.h"
#include "lowmacro.h"
#include "lowparam.h"
#include "lowsolid.h"
#include "eulerops.h"
#include "vectorop.h"
#include "mancommd.h"
#include "rndround.h"
#include "curvgeom.h"
#include "disdispl.h"
#include "shpshape.h"
#include "genfunc_.h"

EPTYPE sedge(SPTYPE s, Id en)
{
   DPTYPE d;
   EPTYPE e;

   for (AllShellsSolid(s, d))
   {
      for (AllEdgesShell(d, e))
      {
         if (EdgEdgeNo(e) == en)
         {
            return(e);
         }
      }
   }
   return(ENIL);
}

void MSD_execRound(void)
{
   char sn[20];
   int  ne, flag_vertex;

   while (3 != sscanf(restbuf, "%s %d %d ", sn, &ne, &flag_vertex))
   {
      printf("Solidname Numero_Arestas Flag_Vertices\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   if (MSD_highRound(sn, ne, flag_vertex) == ERROR)
   {
      fprintf(stderr, MEN_OperacaoNaoRealizada, NAM_Round);
   }
}

int MSD_highRound(char *sn, int ne, int flag_vertex)
{
   SPTYPE s;
   EPTYPE e;
   int    i, ip, edgeno, fg_edge;
   RND    *listrnd;
   float  r, r1, r2, d1, d2;

   if ((s = MSD_lowFindSolidByName(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolidNome, NAM_Round, sn);
      return(FALSE);
   }
   if (ne > 100)
   {
      return(FALSE);
   }
   listrnd = (RND *)workbuf;

   char str[100];
   for (i = 0; i < ne; i++)
   {
      ip = 0;
      while (ip == 0)
      {
         do
         {
            sprintf(str, "Aresta%2d 01234 r d1/d2 r1/r2: ", i + 1);
            if (!lineins(str))
            {
               return(FALSE);
            }
         } while (2 != sscanf(restbuf, "%d %d", &edgeno, &fg_edge));
         (listrnd + i)->edgeno  = edgeno;
         (listrnd + i)->fg_edge = fg_edge;
         switch (fg_edge)
         {
             case 0:
             ip = 1;
             break;

             case 1:
             ip = 1;
             break;

             case 2:
             skipnext(&restbuf);
             skipnext(&restbuf);
             if (1 == sscanf(restbuf, "%f", &r))
             {
                (listrnd + i)->r = r;
                ip = 1;
             }
             break;

             case 3:
             skipnext(&restbuf);
             skipnext(&restbuf);
             if (2 == sscanf(restbuf, "%f %f", &d1, &d2))
             {
                (listrnd + i)->d1 = d1;
                (listrnd + i)->d2 = d2;
                ip = 1;
             }
             break;

             case 4:
             skipnext(&restbuf);
             skipnext(&restbuf);
             if (2 == sscanf(restbuf, "%f %f", &r1, &r2))
             {
                (listrnd + i)->r1 = r1;
                (listrnd + i)->r2 = r2;
                ip = 1;
             }
             break;
         }
      }
   }

   for (i = 0; i < ne; i++)
   {
      if ((e = sedge(s, (listrnd + i)->edgeno)) == ENIL)
      {
         fprintf(stderr, MEN_NaoEncontrouEdgeNo, NAM_Round,
                 (listrnd + i)->edgeno, SolSolidNo(s));
         return(FALSE);
      }
      switch ((listrnd + i)->fg_edge)
      {
          case 0:
          VerFgRound(HalVtx(EdgHe1(e))) = flag_vertex;
          VerFgRound(HalVtx(EdgHe2(e))) = flag_vertex;
          EdgFgRound(e)   = 0;
          HalR(EdgHe1(e)) = 0;
          HalR(EdgHe2(e)) = 0;
          break;

          case 1:
          VerFgRound(HalVtx(EdgHe1(e))) = flag_vertex;
          VerFgRound(HalVtx(EdgHe2(e))) = flag_vertex;
          EdgFgRound(e) = 1;
          break;

          case 2:
          VerFgRound(HalVtx(EdgHe1(e))) = flag_vertex;
          VerFgRound(HalVtx(EdgHe2(e))) = flag_vertex;
          EdgFgRound(e)   = 2;
          HalR(EdgHe1(e)) = (listrnd + i)->r;
          HalR(EdgHe2(e)) = (listrnd + i)->r;
          break;

          case 3:
          VerFgRound(HalVtx(EdgHe1(e))) = flag_vertex;
          VerFgRound(HalVtx(EdgHe2(e))) = flag_vertex;
          EdgFgRound(e)   = 3;
          HalR(EdgHe1(e)) = (listrnd + i)->d1;
          HalR(EdgHe2(e)) = (listrnd + i)->d2;
          break;

          case 4:
          VerFgRound(HalVtx(EdgHe1(e))) = flag_vertex;
          VerFgRound(HalVtx(EdgHe2(e))) = flag_vertex;
          EdgFgRound(e)   = 4;
          HalR(EdgHe1(e)) = (listrnd + i)->r1;
          HalR(EdgHe2(e)) = (listrnd + i)->r2;
          break;
      }
   }

   if (MSD_lowRound(s) == ERROR)
   {
      return(ERROR);
   }
   return(SUCCESS);
}

int MSD_lowRound(SPTYPE s)
{
   if (GeneratingVertices_edges(s) == ERROR)
   {
      return(ERROR);
   }
   EdgeGeneration_faces(s);
   GenerationCurved_edges(s);
   leval_edge(s, 10);
   MSD_lowSetNormal(s, TRUE);
   MSD_lowSetEdgeAngle(s);
   MSD_lowSetInfo(s);
   return(SUCCESS);
}

Id getmaxct(SPTYPE s)
{
   CURVE *c;
   Id    tagno;

   if (SolCurv(s) == (CURVE *)NIL)
   {
      return(0);
   }
   tagno = SolCurv(s)->tagno;
   for (AllCurvesSolid(s, c))
   {
      if (c->tagno > tagno)
      {
         tagno = c->tagno;
      }
   }
   return(tagno);
}

int leval_edge(SPTYPE s, int nsegm)
{
   EPTYPE e;
   DPTYPE d;

   for (AllShellsSolid(s, d))
   {
      for (AllEdgesShell(d, e))
      {
         if (HalCurv(EdgHe1(e)) != (CURVE *)NIL)
         {
            MSD_lowEvaluateCurvePolyline(EdgHe1(e), nsegm);
         }
      }
   }
   return(1);
}

#ifdef __Extended_Version
void MSD_execEvalEdge(void)
{
   int    nsegm;
   char   sn[20];
   SPTYPE s;

   while (2 != sscanf(restbuf, "%s %d  ", sn, &nsegm))
   {
      printf("Solidname Nsegm\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   if ((s = MSD_lowFindSolidByName(sn)) == SNIL)
   {
      fprintf(stderr, "MSD_execEvalEdge: solid %s not found\n", sn);
      return;
   }
   leval_edge(s, nsegm);
}

#endif

int lrnd_alledges(SPTYPE s, real r)
{
   DPTYPE d;
   EPTYPE e;
   VPTYPE v;

   for (AllShellsSolid(s, d))
   {
      for (AllEdgesShell(d, e))
      {
         EdgFgRound(e)   = 2;
         HalR(EdgHe1(e)) = r;
         HalR(EdgHe2(e)) = r;
      }
   }

   for (AllShellsSolid(s, d))
   {
      for (AllVerticesShell(d, v))
      {
         VerFgRound(v) = 1;
      }
   }

   if (MSD_lowRound(s) == ERROR)
   {
      return(ERROR);
   }
   return(SUCCESS);
}

void MSD_execRndAllEdges(void)
{
   char   sn[20];
   SPTYPE s;
   float  r;

   while (2 != sscanf(restbuf, "%s %f ", sn, &r))
   {
      printf("Solidname Raio\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   if ((s = MSD_lowFindSolidByName(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolidNome, NAM_RoundAll, sn);
      return;
   }
   if (lrnd_alledges(s, r) == ERROR)
   {
      printf(MEN_OperacaoNaoRealizada, NAM_RoundAll);
   }
}
