/* 
  PROJETO USPDesigner
  MODULO: GEN (Funcoes Gerais de Nivel Superior)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: genshow_.cpp
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
#include "memvirtu.h"
#include "lowparam.h"
#include "lowmacro.h"
#include "lowsolid.h"
#include "hiegroup.h"
#include "genfunc_.h"
#include "disdispl.h"
#include "mancommd.h"

void MSD_execShow(void)
{
   SPTYPE optr;
   int    n, opt, np;
   char   name[20];

   for (optr = SNIL; ; np = 0)
   {
      if (((opt = optin()) == 'v') || (opt == 'e') || (opt == 'f'))
      {
         if ((np = sscanf(restbuf, "%d", &n)) == 1)
         {
            switch (opt)
            {
                case 'v':
                MSD_lowShowListVertex(MSD_lowShowFindVertex(optr, n));
                break;

                case 'e':
                MSD_lowShowListEdge(MSD_lowShowFindEdge(optr, n));
                break;

                case 'f':
                MSD_lowShowListFace(MSD_getFace(optr, n));
                break;
            }
         }
      }
      if ((opt == 'c') || (opt == 'g') || (opt == 'a') || (opt == 'o'))
      {
         if ((np = sscanf(restbuf, "%s", name)) == 1)
         {
            switch (opt)
            {
                case 'c':
                MSD_lowNameListCameraName(MSD_lowFindCameraByName(name));
                break;

                case 'g':
                MSD_lowListGroup(MSD_lowFindGroupByName(name));
                break;

                case 'a':
                MSD_lowListAxisName(MSD_lowFindAxisByName(name));
                break;

                case 'o':
                MSD_lowShowListSolid(optr = MSD_lowFindSolidByName(name));
                break;
            }
         }
      }
      if (np != 1)
      {
         switch (opt)
         {
             case 'c':
             MSD_lowListAllCamera(    );
             break;

             case 'g':
             MSD_lowListAllGroup(    );
             break;

             case 'a':
             MSD_lowListAllAxis(    );
             break;

             case 'o':
             MSD_lowListAllSolid(    );
             break;

             case 'v':
             MSD_lowListAllVertex(optr);
             break;

             case 'e':
             MSD_lowListAllEdge(optr);
             break;

             case 'f':
             MSD_lowListAllFace(optr);
             break;
         }
      }
      printf("-cgao [Nome]  -vef[No.] \n");
      if (!lineins("? "))
      {
         return;
      }
   }
}

EPTYPE MSD_lowShowFindEdge(SPTYPE s, Id en)
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

VPTYPE MSD_lowShowFindVertex(SPTYPE s, Id vn)
{
   DPTYPE d;
   VPTYPE v;

   for (AllShellsSolid(s, d))
   {
      for (AllVerticesShell(d, v))
      {
         if (VerVertexNo(v) == vn)
         {
            return(v);
         }
      }
   }
   return(VNIL);
}

void MSD_lowListAllSolid(void)
{
   SPTYPE optr;

   for (AllSolids(optr))
   {
      MSD_lowShowListSolid(optr);
   }
}

void MSD_lowListAllVertex(SPTYPE optr)
{
   DPTYPE dptr;
   VPTYPE vptr;

   if (optr != SNIL)
   {
      for (AllShellsSolid(optr, dptr))
      {
         for (AllVerticesShell(dptr, vptr))
         {
            MSD_lowShowListVertex(vptr);
         }
      }
   }
}

void MSD_lowListAllEdge(SPTYPE optr)
{
   DPTYPE dptr;
   EPTYPE eptr;

   if (optr != SNIL)
   {
      for (AllShellsSolid(optr, dptr))
      {
         for (AllEdgesShell(dptr, eptr))
         {
            MSD_lowShowListEdge(eptr);
         }
      }
   }
}

void MSD_lowListAllFace(SPTYPE optr)
{
   DPTYPE dptr;
   FPTYPE fptr;

   if (optr != SNIL)
   {
      for (AllShellsSolid(optr, dptr))
      {
         for (AllFacesShell(dptr, fptr))
         {
            MSD_lowShowListFace(fptr);
         }
      }
   }
}

void MSD_lowShowListSolid(SPTYPE optr)
{
   static char nul[1] = { 0 };
   char        *grnamptr, *axnamptr;
   STYPE       *ss;

#ifdef __VIRTUAL_MEM
   ss = SSTYPE(optr);
#else
   ss = optr;
#endif

   if (optr == SNIL)
   {
      return;
   }
   grnamptr = SolGroup(optr) == GNIL ? nul : GrpName(SolGroup(optr));
   axnamptr = SolOAxs(optr) != ANIL ? AxsName(SolOAxs(optr)) :
              SolGroup(optr) == GNIL ? nul :
              GrpGAxs(SolGroup(optr)) != ANIL ? AxsName(GrpGAxs(SolGroup(optr))) :
              nul;
   printf("Objeto  Nome=%-17s group=%-17s axis=%-17s\n", SolName(optr),
          grnamptr, axnamptr);
   printf("          Cor=%-3d  N_Vert=%4d     N_Edge=%4d     N_Face=%4d\n",
          SolColor(optr), ss->eucoef.vertexn,
          ss->eucoef.edgen,
          ss->eucoef.facen);
}

void MSD_lowShowListVertex(VPTYPE vptr)
{
   if (vptr == VNIL)
   {
      return;
   }
   printf("Vertex %-4d", VerVertexNo(vptr));
   printf("v3(%12.5f,%12.5f,%12.5f)\n", VerVCoord(vptr)[0],
          VerVCoord(vptr)[1],
          VerVCoord(vptr)[2]);
}

void MSD_lowShowListEdge(EPTYPE eptr)
{
   if (eptr == ENIL)
   {
      return;
   }
   printf("Edge %-4d", EdgEdgeNo(eptr));
   printf("vertex(%3d,%3d) face(%3d,%3d) wing(%3d,%3d,%3d,%3d)",
          VerVertexNo(HalVtx(EdgHe1(eptr))),
          VerVertexNo(HalVtx(EdgHe2(eptr))),
          FacFaceNo(LooLFace(HalWLoop(EdgHe1(eptr)))),
          FacFaceNo(LooLFace(HalWLoop(EdgHe2(eptr)))),
          EdgEdgeNo(HalEdg(HalNxt(EdgHe1(eptr)))),
          EdgEdgeNo(HalEdg(HalPrv(EdgHe1(eptr)))),
          EdgEdgeNo(HalEdg(HalNxt(EdgHe2(eptr)))),
          EdgEdgeNo(HalEdg(HalPrv(EdgHe2(eptr)))));
   printf(" angle(%7.4g)\n", EdgAngle(eptr));
}

void MSD_lowShowListFace(FPTYPE fptr)
{
   HPTYPE eptr;
   HPTYPE efim;

   if (fptr == FNIL)
   {
      return;
   }
   printf("Face    %-4d", FacFaceNo(fptr));
   printf("normal(%8.5f,%8.5f,%8.5f)", FacFeq(fptr)[0],
          FacFeq(fptr)[1],
          FacFeq(fptr)[2]);
   printf("  dis(%9.5f)  nv(%4d)\n", -FacFeq(fptr)[3], LooLength(FacFLOut(fptr)));
   eptr = efim = LooLEdg(FacFLOut(fptr));
   do
   {
      printf(">%3d <---%3d ---", EdgEdgeNo(HalEdg(eptr)),
             VerVertexNo(HalVtx(eptr)));
   } while (efim != (eptr = HalNxt(eptr)));
   printf("\n");
}
