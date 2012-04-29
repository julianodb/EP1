/* 
  PROJETO USPDesigner
  MODULO: SET (Operadores Booleanos)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: sethmain.h
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

#ifndef __SetHmain_h
#define __SetHmain_h

#ifndef EXT
 #ifdef _BOOLEAN_
  #define EXT
 #else
  #define EXT    extern
 #endif
#endif

#ifndef __SplSplit_h

 #include <stdio.h>
 #ifndef __int_id
  #define __int_id
typedef int               Id;
 #endif
 #ifndef __double_vector
  #define __double_vector
typedef real              vector[4];
 #endif
 #ifndef __struct_solid
  #define __struct_solid
typedef struct solid      STYPE;
 #endif
 #ifndef __struct_face
  #define __struct_face
typedef struct face       FTYPE;
 #endif
 #ifndef __struct_loop
  #define __struct_loop
typedef struct loop       LTYPE;
 #endif
 #ifndef __struct_edge
  #define __struct_edge
typedef struct edge       ETYPE;
 #endif
 #ifndef __struct_halfedge
  #define __struct_halfedge
typedef struct halfedge   HTYPE;
 #endif
 #ifndef __struct_vertex
  #define __struct_vertex
typedef struct vertex     VTYPE;
 #endif


/*
 #define TRACE_BOOLEAN
 #define TRACE_BOOLEAN1
 #define TRACE_BOOLEAN2
 #define TRACE_BOOLEAN21
 #define TRACE_BOOLEAN22
 #define TRACE_BOOLEAN3
 #define TRACE_BOOLEAN4
 */

 #undef TRACE_BOOLEAN
 #undef TRACE_BOOLEAN1
 #undef TRACE_BOOLEAN2
 #undef TRACE_BOOLEAN21
 #undef TRACE_BOOLEAN22
 #undef TRACE_BOOLEAN3
 #undef TRACE_BOOLEAN4

 #define MAXONVERTICES        200
 #define MAXSECTORS           50
 #define MAXEDGESFORVERTEX    50
 #define MAXNULLEDGES         100
 #define MAXNULLFACES         50
 #define MAXENDEDGES          50
 #define MAXENDS              30

 #ifdef _Windows
namespace USPDesigner
{
 #endif

 #define USP_IN            1
 #define USP_ON            0
 #define USP_OUT           -1

 #define USP_UNION         1
 #define USP_INTERSECT     2
 #define USP_DIFFERENCE    3

 #ifdef _Windows
}
 #endif

typedef struct _nbr       NBR;
typedef struct _nb        NB;
typedef struct _sectors   SECTORS;
typedef struct _sonvv     SONVV;
typedef struct _sonvf     SONVF;

/* estrutura de dados para o modulo setopclassify. */
struct _nbr
{
   HPTYPE sector;
   int    c1;
};

struct _nb
{
   HPTYPE he;
   vector ref1, ref2, ref12;
};

struct _sectors
{
   int secta, sectb, s1a, s2a, s1b, s2b, intersect, onon;
};

 #ifndef __VIRTUAL_MEM
EXT NB      nbb[MAXSECTORS];
EXT NB      nba[MAXSECTORS];
EXT SECTORS sectors[MAXSECTORS];
 #endif
EXT int nnba, nnbb, nsectors;

 #ifndef __VIRTUAL_MEM
EXT NBR nbr[MAXEDGESFORVERTEX];
 #endif
EXT int nnbr;

/* Estrutura de dados para o modulo setopgenerate.c */
struct _sonvv
{
   VPTYPE va;
   VPTYPE vb;
};

struct _sonvf
{
   VPTYPE v;
   FPTYPE f;
};

 #ifndef MAXSHELLS
  #define MAXSHELLS          50
 #endif
typedef struct sff   SFF;
struct sff
{
   DPTYPE shell;
   int    sign;
};
 #ifndef __VIRTUAL_MEM
EXT SFF shellposa[MAXSHELLS], shellposb[MAXSHELLS];
 #endif
EXT int contshella, contshellb;

 #ifndef __VIRTUAL_MEM
EXT SONVV sonvv[MAXONVERTICES];
EXT SONVF sonva[MAXONVERTICES], sonvb[MAXONVERTICES];
 #endif
EXT int nvvtx, nvtxa, nvtxb;

 #ifndef __VIRTUAL_MEM
EXT EPTYPE sonea[MAXNULLEDGES];
EXT EPTYPE soneb[MAXNULLEDGES];
EXT FPTYPE sonfa[MAXNULLFACES * 2];
EXT FPTYPE sonfb[MAXNULLFACES * 2];
 #endif
EXT int nedga, nedgb, nfaca, nfacb;

/* Estrutura para o modulo setopconnect.c */
 #ifndef __VIRTUAL_MEM
EXT HPTYPE endsa[MAXENDS];
EXT HPTYPE endsb[MAXENDS];
 #endif
EXT int nenda, nendb, null_pnta, null_pntb;

#endif

/* Arquivo para trace. */
#ifdef TRACE_BOOLEAN
 #ifndef TRACE_SPLIT
EXT FILE *trace;
 #endif
#endif

/* file setdiffe.c */
void MSD_execNameDifference(void);
void MSD_highNameDifference(char *name1, char *name2, char *name);
void MSD_execDifference(void);
Id MSD_highDifference(Id sn1, Id sn2);

/* file setinter.c */
void MSD_execNameIntersection(void);
void MSD_highNameIntersection(char *name1, char *name2, char *name);
void MSD_execIntersection(void);
Id MSD_highIntersection(Id sn1, Id sn2);

/* file setomain.c */
int MSD_lowSetOp(SPTYPE a, SPTYPE b, Id sn, SPTYPE *res, int op);
int MSD_lowSetOpInitiate0(void);
void MSD_lowSetOpTerminate0(void);
int MSD_lowSetOpInitiate1(void);
void MSD_lowSetOpTerminate1(void);
int MSD_lowSetOpInitiate2(void);
void MSD_lowSetOpTerminate2(void);
int MSD_lowSetOpInitiate3(void);
void MSD_lowSetOpTerminate3(void);
int MSD_lowSetOpInitiate4(void);
void MSD_lowSetOpTerminate4(void);
int MSD_lowSetOpInitiate5(void);
void MSD_lowSetOpTerminate5(void);

/* file setopcls.c */
void MSD_lowSetOpClassify(int op);
void MSD_lowVtxVtxClassify(VPTYPE va, VPTYPE vb, int op);
void MSD_lowVtxFacClassify(VPTYPE v, FPTYPE f, int op, int flag);
void MSD_lowSetOpVFGetNeighborhood(VPTYPE v, vector SP);
void MSD_lowSetOpVFReclassifyOnSectors(vector SP, int op, int flag);
void MSD_lowSetOpVFReclassifyOnEdges(void);
void MSD_lowSetOpVFInsertNullEdges(FPTYPE f, int flag);
void MSD_lowSetOpMakeRing(FPTYPE f, double x, double y, double z, int flag);

/* file setopcon.c */
void MSD_lowSetOpConnect(void);

void MSD_lowSetOpJoin(HPTYPE, HPTYPE);
int  MSD_lowSetOpCanJoin(HPTYPE, HPTYPE, HPTYPE *, HPTYPE *);
void MSD_lowSetOpSortNullEdges(void);
int MSD_lowSetOpGreater(int i, int j);
void MSD_lowSetOpSwap(int i, int j);
int MSD_lowSetOpIsLooseA(HPTYPE he);
int MSD_lowSetOpIsLooseB(HPTYPE he);
void MSD_lowSetOpCutA(HPTYPE he);
void MSD_lowSetOpCutB(HPTYPE he);
int MSD_lowSetOpGetNextNullEdge(EPTYPE *, EPTYPE *);

int MSD_lowSetOpNeighbor(HPTYPE, HPTYPE);

/* file setopfin.c */
SPTYPE MSD_lowSetOpFinish(SPTYPE a, SPTYPE b, Id sn, int op);

/* file setopgen.c */
void MSD_lowSetOpGenerate(SPTYPE a, SPTYPE b);

void MSD_lowSetOpProcessEdge(EPTYPE, FPTYPE, DPTYPE, int BvxA);
void MSD_lowSetOpDoGenerate(EPTYPE, FPTYPE, int BvxA);
void MSD_lowSetOpDoVertexOnFace(VPTYPE, FPTYPE, int BvxA);
void MSD_lowSetOpAddSOVF(VPTYPE, FPTYPE, int flag);
void MSD_lowSetOpAddSOVV(VPTYPE, VPTYPE, int flag);
int  MSD_lowSetOpFacPntContainment(FPTYPE, real x, real y, real z);

/* file setopget.c */
void MSD_lowSetOpVVGetNeighborhood(VPTYPE, VPTYPE);
int  MSD_lowSetOpNbaPreProcess(VPTYPE);
int  MSD_lowSetOpNbbPreProcess(VPTYPE);
int MSD_lowSetOpSectorTest(int i, int j);
int MSD_lowSetOpSectorWithin(vector dir, vector ref1, vector ref2, vector ref12);

int MSD_lowSetOpOverlap(HPTYPE, HPTYPE);
int MSD_lowSetOpSectorOverlap(HPTYPE, HPTYPE);
void MSD_lowSetOpInside(HPTYPE he, vector in);

/* file setsinsn.c */
void MSD_lowSetOpVVInsertNullEdges(int);

int  MSD_lowSetOpGetOrient(HPTYPE, HPTYPE, HPTYPE);
int  MSD_lowSetOpSectorWide(HPTYPE, int);
int  MSD_lowSetOpConvexEdge(HPTYPE);
void MSD_lowSetOpSepar1(HPTYPE, HPTYPE, int);
void MSD_lowSetOpSepar2(HPTYPE, int, int, int);
int  MSD_lowSetOpStrutNullEdge(HPTYPE);
int  MSD_lowSetOpNullEdge(HPTYPE);

/* file setsrece.c */
void MSD_lowSetOpVVReclassifyEdges(int op);

/* file setsrecs.c */
void MSD_lowSetOpVVReclassifySectors(int op);

/* file setunion.c */
void MSD_execNameUnion(void);
void MSD_highNameUnion(char *name1, char *name2, char *name);
void MSD_execUnion(void);
Id MSD_highUnion(Id sn1, Id sn2);


void trace_he(HPTYPE he);
void trace_e(EPTYPE e);
void trace_nbr(int x);
void trace_nba(int x);
void trace_nbb(int x);

#endif
