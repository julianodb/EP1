/* 
  PROJETO USPDesigner
  MODULO: LOW (Operadores Basicos)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: lowgeome.h
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

int MSD_lowEdgVtxContainment(VPTYPE, VPTYPE, VPTYPE);
int MSD_lowFacVtxContainment(FPTYPE, VPTYPE);
int MSD_lowFacVtxCoordContainment(FPTYPE, VPTYPE, int);
int MSD_lowLooVtxBoundary(LPTYPE, VPTYPE);
int MSD_lowLooVtxContainment(LPTYPE, VPTYPE, int);
int MSD_lowLooBoxContainment(LPTYPE, VPTYPE, int);
int MSD_lowSolVtxContainment(SPTYPE, VPTYPE);
int MSD_lowVtxVtxContainment(VPTYPE, VPTYPE);
int MSD_lowFacFacIdentical(FPTYPE, FPTYPE);
int MSD_lowFacFacContainment(FPTYPE, FPTYPE);
int MSD_lowFacFacEqualEqs(FPTYPE, FPTYPE);
int MSD_lowFacFacOppositeEqs(FPTYPE, FPTYPE);
int MSD_lowEdgEdgColinear(EPTYPE, EPTYPE);
int MSD_lowNullEdge(HPTYPE);
int MSD_lowEdgEdgIntersection(VPTYPE, VPTYPE, VPTYPE, VPTYPE, int, real *, real *);
int MSD_lowVtxEdgIntersection(VPTYPE, VPTYPE, VPTYPE, real *);
