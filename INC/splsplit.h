/* 
  PROJETO USPDesigner
  MODULO: SPL (Corte de Solido)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: splsplit.h
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

#ifndef __SplSplit_h
#define __SplSplit_h

#undef TRACE_SPLIT
#undef TRACE_SPLIT1
#undef TRACE_SPLIT2
#undef TRACE_SPLIT3
#undef TRACE_SPLIT4

#include <stdio.h>

#ifndef EXTERN
 #ifdef _DVS_
  #define EXTERN
 #else
  #define EXTERN    extern
 #endif
#endif

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

#define ON       0
#define BELOW    -1
#define ABOVE    1

#ifdef TRACE_SPLIT
EXTERN FILE *trace;
#endif

EXTERN int  OutputPlanoCorte;
EXTERN char OutputFile[12];

/* estrutura de dados para o passo 1 */
#ifndef MAXSHELLS
 #define MAXSHELLS          50
#endif
typedef struct sf   SF;
struct sf
{
   DPTYPE shell;
   int    sign;
};
#ifndef __VIRTUAL_MEM
EXTERN SF spl_shellpos[MAXSHELLS];
#endif
EXTERN int spl_contshell;

#ifndef MAXONVERTICES
 #define MAXONVERTICES     100
#endif
#ifndef __VIRTUAL_MEM
EXTERN VPTYPE spl_soov[MAXONVERTICES];
#endif
EXTERN int spl_nvtx;

/* estrutura de dados para o passo 2 */
#ifndef MAXNULLEDGES
 #define MAXNULLEDGES         100
#endif
#ifndef MAXEDGESFORVERTEX
 #define MAXEDGESFORVERTEX    100
#endif
#ifndef __VIRTUAL_MEM
EXTERN EPTYPE spl_sone[MAXNULLEDGES];
#endif
typedef  struct _spl_nbr   spl_NBR;
struct _spl_nbr
{
   HPTYPE sector;
   int    c1;
};
#ifndef __VIRTUAL_MEM
EXTERN spl_NBR spl_nbr[MAXEDGESFORVERTEX];
#endif
EXTERN int spl_nnbr;
EXTERN int spl_nedge;

/* estrutura de dados para o passo 3 */
#ifndef MAXNULLFACES
 #define MAXNULLFACES      100
#endif
#ifndef __VIRTUAL_MEM
EXTERN FPTYPE spl_sonf[MAXNULLFACES * 2];
EXTERN HPTYPE spl_ends[30];
#endif
EXTERN int spl_nfac;
EXTERN int spl_nend;
EXTERN int spl_null_edge_pointer;

/* file splmain_.c */
void MSD_execNameSplit(void);

void MSD_highNameSplit(char *, char *, char *, real, real, real, real);
void MSD_execSplit(void);

Id  MSD_highSplit(Id, real, real, real, real);
int MSD_lowSplit(SPTYPE, vector, Id, Id, SPTYPE *, SPTYPE *);
void MSD_lowSplitInitiate(void);
void MSD_lowSplitTerminate(void);

/* file splstep1.c */
void MSD_lowSplitGenerate(SPTYPE, vector);
void MSD_lowAddSplitOnVertex(VPTYPE);

/* file splstep2.c */
void MSD_lowSplitClassify(vector);
void MSD_lowGetNeighborhood(VPTYPE, vector);
void MSD_lowReclassifyOnSectors(vector);
void MSD_lowReclassifyOnEdges(void);
void MSD_lowInsertNullEdges(void);

int  MSD_lowCheckWideness(HPTYPE);
void MSD_lowBisector(HPTYPE, vector);
real MSD_lowDistancePlanePoint(vector, VPTYPE);

/* file splstep3.c */
void MSD_lowSplitConnect(void);

HPTYPE MSD_lowCanJoin(HPTYPE);
void   MSD_lowJoin(HPTYPE, HPTYPE);
void   MSD_lowCut(HPTYPE);
int    MSD_lowNeighbor(HPTYPE, HPTYPE);
int    MSD_lowIsLoose(HPTYPE);
void MSD_lowSortNullEdge(void);
int MSD_lowGreater(int i, int j);
void MSD_lowSwap(int i, int j);
EPTYPE MSD_lowGetNextNullEdge(void);

/* file splstep4.c */
void MSD_lowSplitFinish(SPTYPE, Id, Id, SPTYPE *, SPTYPE *);

/* file splvoron.c */
void MSD_execVoronoi(void);
void MSD_lowVoronoi(char *name);

#endif
