/* 
  PROJETO USPDesigner
  MODULO: RND (Arredondamento)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: rndround.h
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

#ifndef EXTERN
 #ifdef _DVS_
  #define EXTERN
 #else
  #define EXTERN    extern
 #endif
#endif

typedef struct
{
   int  edgeno;
   int  fg_edge;
   real r;
   real r1;
   real r2;
   real d1;
   real d2;
} RND;

#define MSD_ROUND_E_ORIGINAL    0
#define MSD_ROUND_E_FASE1       1
#define MSD_ROUND_E_FASE2       2
#define MSD_ROUND_E_FASE3       3
#define MSD_ROUND_V_ORIGINAL    0
#define MSD_ROUND_V_FASE1       1
#define MSD_ROUND_V_FASE2       2
#define MSD_ROUND_V_FASE3       3

#define MSD_ROUND_FLAG_E0       0
#define MSD_ROUND_FLAG_E1       1
#define MSD_ROUND_FLAG_E2       2
#define MSD_ROUND_FLAG_E3       3
#define MSD_ROUND_FLAG_E4       4
#define MSD_ROUND_FLAG_V0       0
#define MSD_ROUND_FLAG_V1       1

/* file rndround.c */
EPTYPE sedge(SPTYPE, Id);
void MSD_execRound(void);
int MSD_highRound(char *, int, int);

int MSD_lowRound(SPTYPE);
Id  getmaxct(SPTYPE);
int leval_edge(SPTYPE, int);
void MSD_execEvalEdge(void);

int lrnd_alledges(SPTYPE, real);
void MSD_execRndAllEdges(void);

/* file rndfase1.c */
int GeneratingVertices_edges(SPTYPE);
int MakingEdges_tables(SPTYPE);
int MakingVertices_edges(SPTYPE);

/* file rndfase2.c */
int    EdgeGeneration_faces(SPTYPE);
int    GenerationEdges_corners(SPTYPE);
int    GenerationEdges_nvertices(SPTYPE);
void   MakeEdgeFlatCorner(SPTYPE, HPTYPE);
HPTYPE MakeEdgeConvexCorner(SPTYPE, HPTYPE);
void   MakeEdgeConcaveCorner(SPTYPE, HPTYPE);
void   GetNxtVertex(HPTYPE, vector);
void   GetPrvVertex(HPTYPE, vector);
int    cont_point(VPTYPE, VPTYPE, vector);

/* file rndfase3.c */
int    GenerationCurved_edges(SPTYPE);
int    EdgesAttachedOriginal_vertex(SPTYPE);
VPTYPE edv1(VPTYPE);
VPTYPE edv2(SPTYPE, VPTYPE);
VPTYPE edv3(SPTYPE, VPTYPE);
VPTYPE edv4(SPTYPE, VPTYPE);
int    EdgeAttached_vertexfase1(SPTYPE);

/* file rndaux__.c */
void calc_point(vector, vector, real, vector);
real angle_edge(HPTYPE);
int  GetEdges(VPTYPE, int);
void CLC(SPTYPE, HPTYPE, vector, vector, vector);
void lgeo_arc(vector, vector, vector, vector, vector, real, real);
void _line(vector, real, vector, real, vector, vector, vector);
void line_parallel(vector, vector, vector, vector, vector, real);
real calc_t(vector, vector, vector);
void line_inter(vector, vector, vector, vector, vector);
void point_perp(vector, vector, vector, vector, real);
