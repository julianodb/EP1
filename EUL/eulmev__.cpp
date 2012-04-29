/* 
  PROJETO USPDesigner
  MODULO: EUL (Operadores de Euler)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: eulmev__.cpp
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
#include "lowsolid.h"
#include "eulerops.h"
#include "curvgeom.h"
#include "mancommd.h"
/* Operator lmev "splits" the vertex pointed at by he1 and he2, and adds */
/* a new edge between the resulting two vertices. The identifier v and   */
/* the coordinates x, y, and z are assigned to the new vertex. The case  */
/* that he1 and he2 are identical is allowed; in this case, the new      */
/* vertex and edge are added into the face of he1. The new edge is       */
/* oriented from the new vertex to the old one.                          */

#ifdef __Extended_Version
void MSD_execMEV(void)
{
   int   f, s, vn1, vn2, op;
   float xx, yy, zz;

   while (8 != sscanf(restbuf, "%d %d %d %d %d %f %f %f", &s, &op, &f, &vn1, &vn2,
                      &xx, &yy, &zz))
   {
      printf("mev: Sid F V1 V2 X Y Z \n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highMEV(s, f, vn1, vn2, xx, yy, zz);
}

#endif

int MSD_highMEV(Id sn, Id fn, Id v1, Id v2, real x, real y, real z)
{
   SPTYPE s;
   FPTYPE oldface;
   HPTYPE h;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_MEV, sn);
      return(ERROR);
   }
   if ((oldface = MSD_getFace(s, fn)) == FNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouFace, NAM_MEV, fn, sn);
      return(ERROR);
   }
   if (MSD_getLoop(oldface, v1, &h) == LNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouVertice, NAM_MEV, v1, sn);
      return(ERROR);
   }
   MSD_lowMEV(h, h, v2, x, y, z);
   return(SUCCESS);
}

#ifdef __Extended_Version
void MSD_execSEMV(void)
{
   int   sn, v1, v2, v3;
   float x, y, z;

   while (7 != sscanf(restbuf, "%d %d %d %d %f %f %f", &sn, &v1, &v2, &v3,
                      &x, &y, &z))
   {
      printf("semv: Sid V1 V2 V3 X Y Z\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highSEMV(sn, v1, v2, v3, x, y, z);
}

#endif

int MSD_highSEMV(Id sn, Id v1, Id v2, Id v3, real x, real y, real z)
{
   SPTYPE s;
   DPTYPE d;
   FPTYPE f;
   HPTYPE he1;
   short  flag_found;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_SEMV, sn);
      return(ERROR);
   }

   flag_found = 1;
   for (AllShellsSolid(s, d))
   {
      for (AllFacesShell(d, f))
      {
         if (MSD_getEdge(f, v1, v2, &he1))
         {
            flag_found = 0;
            break;
         }
      }
   }
   if (flag_found == 1)
   {
      fprintf(stderr, MEN_NaoEncontrouArestaV, NAM_SEMV, v1, v2, sn);
      return(ERROR);
   }

   MSD_lowMEV(mate(he1), HalNxt(he1), v3, x, y, z);
   return(SUCCESS);
}

#ifdef __Extended_Version
void MSD_execSVME(void)
{
   int   sn, v1, v2, f1, f2, op;
   float x, y, z;

   while (9 != sscanf(restbuf, "%d %d %d %d %d %d %f %f %f", &sn, &op, &v1, &v2, &f1,
                      &f2, &x, &y, &z))
   {
      printf("svme: Sid V1 V2 F1 F2 X Y Z\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highSVME(sn, v1, v2, f1, f2, x, y, z);
}

#endif

int MSD_highSVME(Id sn, Id v1, Id v2, Id f1, Id f2, real x, real y, real z)
{
   SPTYPE s;
   FPTYPE ff1;
   FPTYPE ff2;
   HPTYPE he1;
   HPTYPE he2;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_SVME, sn);
      return(ERROR);
   }
   if ((ff1 = MSD_getFace(s, f1)) == FNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouFace, NAM_SVME, f1, sn);
      return(ERROR);
   }
   if ((ff2 = f1 != f2 ? MSD_getFace(s, f2) : ff1) == FNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouFace, NAM_SVME, f2, sn);
      return(ERROR);
   }
   if (MSD_getLoop(ff1, v1, &he1) == LNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouVertice, NAM_SVME, v1, sn);
      return(ERROR);
   }
   if (MSD_getLoop(ff2, v1, &he2) == LNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouVertice, NAM_SVME, v1, sn);
      return(ERROR);
   }
   MSD_lowMEV(he1, he2, v2, x, y, z);
   return(SUCCESS);
}

#ifdef __Extended_Version
void MSD_execSVME2(void)
{
   int   sn, v1, v2, v3, v4, f1, f2;
   float x, y, z;

   while (10 != sscanf(restbuf, "%d %d %d %d %d %d %d %f %f %f", &sn, &v1,
                       &v2, &v3, &v4, &f1, &f2, &x, &y, &z))
   {
      printf("svme2: Sid V1 V2 V3 V4 F1 F2 X Y Z\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highSVME2(sn, v1, v2, v3, v4, f1, f2, x, y, z);
}

#endif

int MSD_highSVME2(Id sn, Id v1, Id v2, Id v3, Id v4, Id f1, Id f2, real x,
                  real y, real z)
{
   SPTYPE s;
   FPTYPE ff1;
   FPTYPE ff2;
   HPTYPE he1;
   HPTYPE he2;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_SVME2, sn);
      return(ERROR);
   }
   if ((ff1 = MSD_getFace(s, f1)) == FNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouFace, NAM_SVME2, f1, sn);
      return(ERROR);
   }
   if ((ff2 = f1 != f2 ? MSD_getFace(s, f2) : ff1) == FNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouFace, NAM_SVME2, f2, sn);
      return(ERROR);
   }
   if (MSD_getEdge(ff1, v1, v2, &he1) == LNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouArestaV, NAM_SVME2, v1, v2, sn);
      return(ERROR);
   }
   if (MSD_getEdge(ff2, v1, v3, &he2) == LNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouArestaV, NAM_SVME2, v1, v3, sn);
      return(ERROR);
   }
   MSD_lowMEV(he1, he2, v4, x, y, z);
   return(SUCCESS);
}

#ifdef __Extended_Version
void MSD_execSVME3(void)
{
   int   sn, v1, v2, v3, v4, f1, f2, f3, f4, op;
   float x, y, z;

   while (13 != sscanf(restbuf, "%d %d %d %d %d %d %d %d %d %d %f %f %f", &sn, &op,
                       &v1, &v2, &v3, &v4, &f1, &f2, &f3, &f4, &x, &y, &z))
   {
      printf("svme3: Sid V1 V2 V3 V4 F1 F2 F3 F4 X Y Z\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highSVME3(sn, v1, v2, v3, v4, f1, f2, f3, f4, x, y, z);
}

#endif

int MSD_highSVME3(Id sn, Id v1, Id v2, Id v3, Id v4, Id f1, Id f2, Id f3, Id f4,
                  real x, real y, real z)
{
   SPTYPE s;
   FPTYPE ff1;
   FPTYPE ff2;
   FPTYPE ff3;
   FPTYPE ff4;
   HPTYPE he1;
   HPTYPE he2;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_SVME3, sn);
      return(ERROR);
   }
   if ((ff1 = MSD_getFace(s, f1)) == FNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouFace, NAM_SVME3, f1, sn);
      return(ERROR);
   }
   if ((ff2 = MSD_getFace(s, f2)) == FNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouFace, NAM_SVME3, f2, sn);
      return(ERROR);
   }
   if ((ff3 = MSD_getFace(s, f3)) == FNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouFace, NAM_SVME3, f3, sn);
      return(ERROR);
   }
   if ((ff4 = MSD_getFace(s, f4)) == FNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouFace, NAM_SVME3, f4, sn);
      return(ERROR);
   }
   if (MSD_getHalfedge(ff1, ff3, v1, v2, &he1) == LNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouArestaF, NAM_SVME3, v1, v2, f1, f3, sn);
      return(ERROR);
   }
   if (MSD_getHalfedge(ff2, ff4, v1, v3, &he2) == LNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouArestaF, NAM_SVME3, v1, v3, f2, f4, sn);
      return(ERROR);
   }
   MSD_lowMEV(he1, he2, v4, x, y, z);
   return(SUCCESS);
}

void MSD_lowMEV(HPTYPE he1, HPTYPE he2, Id vn, real x, real y, real z)
{
   SPTYPE s;
   DPTYPE d;
   EPTYPE newedge;
   HPTYPE he;
   VPTYPE newvertex;

   Id v1, v2, v3, f1, f2, f3, f4;

#ifdef __curved_env
   HTYPE *nhe1, *nhe2;
   int   copy_tag;
#endif

   assert1(HalVtx(he1) == HalVtx(he2));
#ifdef __curved_env
   copy_tag = he2 == mate(he1)->nxt ? 0 : 1;
#endif

   d         = FacFShell(LooLFace(HalWLoop(he1)));
   s         = SheSSolids(d);
   newedge   = (EPTYPE)MSD_lowNewElement(EDGE, (NPTYPE)d);
   newvertex = (VPTYPE)MSD_lowNewElement(VERTEX, (NPTYPE)d);

   if (Gdoinversion)
   {
      v1 = VerVertexNo(HalVtx(he1));
      f1 = FacFaceNo(LooLFace(HalWLoop(he2)));
      f2 = FacFaceNo(LooLFace(HalWLoop(he1)));
      MSD_lowAddEulerOp(s, KEV, vn, v1, f1, f2, 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0);
   }
   if (Gdodirect)
   {
      // Linha inserida por Takase 16.06.97
      v1 = VerVertexNo(HalVtx(he1));
      v2 = VerVertexNo(HalVtx(HalNxt(he1)));
      v3 = VerVertexNo(HalVtx(HalNxt(he2)));
      f1 = FacFaceNo(LooLFace(HalWLoop(he1)));
      f2 = FacFaceNo(LooLFace(HalWLoop(he2)));
      f3 = HalEdg(he1) == ENIL ? f1 : FacFaceNo(LooLFace(HalWLoop(mate(he1))));
      f4 = HalEdg(he2) == ENIL ? f2 : FacFaceNo(LooLFace(HalWLoop(mate(he2))));
      MSD_lowAddEulerOpRedo(s, MEV, v1, v2, v3, vn, f1, f2, f3, f4, x, y, z, 0.0);
   }

   EdgEdgeNo(newedge)      = -1;
   VerVertexNo(newvertex)  = vn;
   VerVCoord(newvertex)[0] = x;
   VerVCoord(newvertex)[1] = y;
   VerVCoord(newvertex)[2] = z;
   VerVCoord(newvertex)[3] = 1.0;

   for (he = he1; he != he2; he = HalNxt(mate(he)))
   {
      HalVtx(he) = newvertex;
   }

#ifdef __curved_env
   nhe2 = addhe(newedge, he2->vtx, he1, MINUS);
   nhe1 = addhe(newedge, newvertex, he2, PLUS);
#else
   MSD_lowAddHE(newedge, HalVtx(he2), he1, MINUS);
   MSD_lowAddHE(newedge, newvertex, he2, PLUS);
#endif
   VerVEdge(newvertex)   = HalPrv(he2);
   VerVEdge(HalVtx(he2)) = he2;

   MSD_addEulCoef(s, 0, 0, 0, 1, 1, 0);

#ifdef __curved_env
   if ((copy_tag == 0) && (he1->curv != (CUCURV *)NIL))
   {
      lputctag(nhe1, he1->curv);
      lputctag(nhe2, he1->curv);
   }
#endif

   if (SetTrace)
   {
      if (TraceSolid(s))
      {
         fprintf(stderr, MEN_MSDAbortou, NAM_MEV);
         exit(1);
      }
   }
}

int MSD_execStructMEV(EulerOp *op)
{
   Id   sid, id1, id2, id3, id4, id5, id6, id7, id8;
   real fd1, fd2, fd3;

   sid = op->solidno;
   id1 = op->ip1;
   id2 = op->ip2;
   id3 = op->ip3;
   id4 = op->ip4;
   id5 = op->ip5;
   id6 = op->ip6;
   id7 = op->ip7;
   id8 = op->ip8;
   fd1 = op->fp1;
   fd2 = op->fp2;
   fd3 = op->fp3;
   return(MSD_highSVME3(sid, id1, id2, id3, id4, id5, id6, id7, id8, fd1,
                        fd2, fd3));
}

void MSD_printStructMEV(FILE *f, EulerOp *op)
{
   Id   sid, id1, id2, id3, id4, id5, id6, id7, id8;
   real fd1, fd2, fd3;
   int  com;

   sid = op->solidno;
   id1 = op->ip1;
   id2 = op->ip2;
   id3 = op->ip3;
   id4 = op->ip4;
   id5 = op->ip5;
   id6 = op->ip6;
   id7 = op->ip7;
   id8 = op->ip8;
   fd1 = op->fp1;
   fd2 = op->fp2;
   fd3 = op->fp3;
   com = op->CommandCount;
   fprintf(f, "svme %d %d %d %d %d %d %d %d %d %d %f %f %f\n", sid,
           com, id1, id2, id3, id4, id5, id6, id7, id8, fd1, fd2, fd3);
}

int MSD_readStructMEV(FILE *f, EulerOp *op)
{
   int   solidno, ip1, ip2, ip3, ip4, ip5, ip6, ip7, ip8, com;
   float fp1, fp2, fp3;

   if (fscanf(f, "%d %d %d %d %d %d %d %d %d %d %f %f %f\n", &solidno, &com, &ip1,
              &ip2, &ip3, &ip4, &ip5, &ip6, &ip7, &ip8, &fp1, &fp2, &fp3) != 13)
   {
      return(ERROR);
   }
   op->solidno      = solidno;
   op->CommandCount = com;
   op->ip1          = ip1;
   op->ip2          = ip2;
   op->ip3          = ip3;
   op->ip4          = ip4;
   op->ip5          = ip5;
   op->ip6          = ip6;
   op->ip7          = ip7;
   op->ip8          = ip8;
   op->fp1          = fp1;
   op->fp2          = fp2;
   op->fp3          = fp3;
   op->fp4          = 1.0;
   return(SUCCESS);
}

int MSD_modifyStructMEV(EulerOp *op)
{
   int solidno;

   solidno     = MSD_lowCheckTranslatorTable(op->solidno, SOLID);
   op->solidno = solidno;
   return(SUCCESS);
}
