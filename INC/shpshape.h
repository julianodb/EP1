/* 
  PROJETO USPDesigner
  MODULO: SHP (Criacao de Formas Primitivas)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: shpshape.h
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

#ifndef __ShpShape_h
#define __ShpShape_h

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

typedef struct
{
   real x;
   real y;
} XY;

EXTERN unsigned char workbuf[2000];

/* file shpcylin.c */
void MSD_execNameMakeCylinder(void);
void MSD_highNameMakeCylinder(char *name, int n, real r, real h);
void MSD_execMakeCylinder(void);
Id MSD_highMakeCylinder(int n, real r, real h);
void MSD_execNameMakeCone(void);
void MSD_highNameMakeCone(char *name, int n, real r, real h);
void MSD_execMakeCone(void);
Id MSD_highMakeCone(int n, real r, real h);
SPTYPE MSD_lowMakeCylinder(Id sn, real rad, real h, int n);
SPTYPE MSD_lowMakeCylinder0(Id sn, real rad, int n);
SPTYPE MSD_lowMakeCone(Id sn, real rad, real h, int n);
void MSD_lowRot90Dg(SPTYPE s);

/* file shpgeoms.c */
void MSD_lowSetNormal(SPTYPE optr, int op);
void MSD_lowSetEdgeAngle(SPTYPE optr);

/* file shpmpris.c */
void MSD_execNameMakePrism(void);

void MSD_highNameMakePrism(char *, int, real, XY *);
void MSD_execMakePrism(void);

Id MSD_highMakePrism(int, real, XY *);
void MSD_execNameMakePyramid(void);

void MSD_highNameMakePyramid(char *, int, real, XY *);
void MSD_execMakePyramid(void);

Id MSD_highMakePyramid(int, real, XY *);
char setvtpi(XY **, int);

/* file shpmrevo.c */
void MSD_execNameMakeRevolve(void);
void MSD_highNameMakeRevolve(char *, int, int, XY *);
void MSD_execMakeRevolve(void);
Id MSD_highMakeRevolve(int, int, XY *);

/* file shppris0.c */
SPTYPE MSD_lowMakePrism0(Id sn, XY *listvtx, int n, int pln);
SPTYPE MSD_lowMakePolygon(Id sn, XY *listvtx, int n, int pln);

/* file shpprism.c */
SPTYPE MSD_lowMakePrism(Id sn, XY *listvtx, real h, int n);
int MSD_lowMakeSweep(FPTYPE fac, real dx, real dy, real dz);

/* file shppyram.c */
SPTYPE MSD_lowMakePyramid(Id sn, XY *listvtx, real h, int n);
void MSD_lowSetFaceToPoint(FPTYPE fptr, real x, real y, real z);

/* file shprevol.c */
SPTYPE MSD_lowMakeRevolve(Id sn, XY *listvtx, int m, int n);
char MSD_lowMakeRotationalSweep(SPTYPE s, int nfaces, int axs, int pln, vector vect);
void MSD_lowMakeFaceToPoint(FPTYPE f);
int MSD_lowMakeOpenRSweep(SPTYPE s, int pln);
HPTYPE MSD_lowTouchAxis(HPTYPE eptr, int pln);
HPTYPE MSD_lowTouchWire(HPTYPE eptr, int *cut, int pln);

/* file shpspher.c */
void MSD_execNameMakeSphere(void);
void MSD_highNameMakeSphere(char *name, int n, real r);
void MSD_execMakeSphere(void);
Id MSD_highMakeSphere(int n, real r);
SPTYPE MSD_lowMakeSphere(Id sn, real r, int nver, int nhor);
char MSD_lowMakeArc(Id s, Id f, Id v, real cx, real cz, real rad, real \
                    h, real phi1, real phi2, int n);
SPTYPE MSD_lowMakeCircle(Id sn, real cx, real cz, real rad, real h, int n);

/* file shpsweep.c */
void MSD_execNameMakeSweep(void);

/* file shptorus.c */
void MSD_execNameMakeTorus(void);
void MSD_highNameMakeTorus(char *name, int n, real r0, real r1);
void MSD_execMakeTorus(void);
Id MSD_highMakeTorus(int n, real r0, real r1);
SPTYPE MSD_lowMakeTorus(Id sn, real r1, real r2, int nf1, int nf2);

/* file shpxcube.c */
void MSD_execNameMakeCube(void);
void MSD_highNameMakeCube(char *name, real a, real b, real c);
void MSD_execMakeCube(void);
Id MSD_highMakeCube(real a, real b, real c);
SPTYPE MSD_lowMakeCube(Id sn, real dx, real dy, real dz);
SPTYPE MSD_lowMakeCube0(Id sn, real dx, real dy);

#endif
