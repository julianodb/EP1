/* 
  PROJETO USPDesigner
  MODULO: LOW (Operadores Basicos)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: lowsolid.h
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

#ifndef __LowSolid_h
#define __LowSolid_h

#ifndef __HieGroup_h
 #include "hiegroup.h"       /* include group and axis definitions */
#endif

/******************************************************************************/
/* Artificio para manter apenas um arquivo com definicoes de estruturas ----- */
#ifndef EXTERN
 #ifdef _DVS_
  #define EXTERN
 #else
  #define EXTERN    extern
 #endif
#endif

#define GRAVIDADE    9.8

/******************************************************************************/
/* Estruturas de Dados definidas em outros arquivos ------------------------- */
#ifndef __double_real
 #define __double_real
typedef double            real;
#endif
#ifndef __double_vector
 #define __double_vector
typedef real              vector[4];
#endif
#ifndef __double_vect2
 #define __double_vect2
typedef real              vect2[3];
#endif
#ifndef __double_matrix
 #define __double_matrix
typedef real              matrix[4][4];
#endif
#ifdef __curved_env__
 #ifndef __surface1_SUSURF
  #define __surface1_SUSURF
typedef struct surface    SSURF;
 #endif
 #ifndef __curve1_CUCURV
  #define __curve1_CUCURV
typedef struct curve      CURVE;
 #endif
#endif
#ifndef __int_id
 #define __int_id
typedef int               Id;
#endif
#ifndef __struct_eulerop
 #define __struct_eulerop
typedef struct eulerop    EulerOp;
#endif
#ifndef __struct_axistp
 #define __struct_axistp
typedef struct axistp     ATYPE;
#endif
#ifndef __struct_grouptp
 #define __struct_grouptp
typedef struct grouptp    GTYPE;
#endif
#ifndef __struct_cameratp
 #define __struct_cameratp
typedef struct cameratp   CTYPE;
#endif

/******************************************************************************/
/* Estruturas de Dados definidas neste arquivo ------------------------------ */
#ifndef __struct_solid
 #define __struct_solid
typedef struct solid      STYPE;
#endif
#ifndef __struct_shell
 #define __struct_shell
typedef struct shell      DTYPE;
#endif
#ifndef __struct_face
 #define __struct_face
typedef struct face       FTYPE;
#endif
#ifndef __struct_loop
 #define __struct_loop
typedef struct loop       LTYPE;
#endif
#ifndef __struct_halfedge
 #define __struct_halfedge
typedef struct halfedge   HTYPE;
#endif
#ifndef __struct_vertex
 #define __struct_vertex
typedef struct vertex     VTYPE;
#endif
#ifndef __struct_edge
 #define __struct_edge
typedef struct edge       ETYPE;
#endif
#ifndef __struct_box
 #define __struct_box
typedef struct box        BTYPE;
#endif
typedef struct eulerco    EulerCo;
typedef union nodes       NTYPE;

#ifdef __ROV__
EXTERN real densifluido;
#endif

struct box
{
   vector max;                        /* corners of the box - max    */
   vector min;                        /* corners of the box - min    */
};

struct eulerco
{
   int shelln;                        /* number of shells            */
   int facen;                         /* number of faces             */
   int ringn;                         /* number of rings             */
   int edgen;                         /* number of edges             */
   int vertexn;                       /* number of vertices          */
   int holen;                         /* number of holes             */
};

struct solid
{
   Id      solidno;                   /* solid identifier            */
   char    name[20];                  /* solid's name                */

#ifdef __ROV__
   real    densrel;                   /* relative density of solid   */
   real    densabs;                   /* absolut density of solid    */
   real    areaproj;                  /* actual projected area       */
   vector  PrincDir;                  /* main direction              */
   int     type;                      /* type of solid               */
   int     geometria;                 /* geometric characteristic    */
   int     fronteira;                 /* boundary type of solid      */
   int     bareaproj;                 /* flag for proj area of solid */
   real    param1;                    /* first parameter             */
   real    param2;                    /* second parameter            */
   real    param3;                    /* third parameter             */
   real    param4;                    /* fourth parameter            */
   vector  center;                    /* center of the solid         */
   real    aux;                       /* auxiliary variable          */
   int     pos;                       /* relative position           */
   Id      corte;                     /* identifier for the cut line */
#endif

   SPTYPE  nxt;                       /* pointer to next solid       */
   SPTYPE  prevs;                     /* pointer to previous solid   */
   DPTYPE  sshells;                   /* pointer to list of shells   */
   GPTYPE  group;                     /* pointer to a group          */
   APTYPE  oaxs;                      /* pointer to an axis          */
   BTYPE   box;                       /* enclosing box               */
   short   sbits;                     /* status bits                 */
   EulerCo eucoef;                    /* Euler's exp. coefficients   */
   char    dsp;                       /* flag - display/not display  */
   int     color;                     /* color qualifier             */

#ifdef __curved_env__
   CURVE   *scurvs;                   /* curve information           */
#endif
};

struct shell
{
   Id     shellno;                    /* shell identifier            */
   DPTYPE nextd;                      /* pointer to next shell       */
   DPTYPE prevd;                      /* pointer to previous shell   */
   SPTYPE ssolids;                    /* back pointer to solid       */
   FPTYPE sfaces;                     /* pointer to list of faces    */
   EPTYPE sedges;                     /* pointer to list of edges    */
   VPTYPE sverts;                     /* pointer to list of vertices */
   BTYPE  box;                        /* enclosing box               */
   short  dbits;                      /* status bits                 */
   int    color;                      /* color qualifier             */
};

struct face
{
   Id     faceno;                     /* face identifier             */
   DPTYPE fshell;                     /* back pointer to shell       */
   FPTYPE nextf;                      /* pointer to next face        */
   FPTYPE prevf;                      /* pointer to previous face    */
   LPTYPE flout;                      /* pointer to outer loop       */
   LPTYPE floops;                     /* pointer to list of loops    */
   vector feq;                        /* face equation               */
   BTYPE  box;                        /* enclosing box               */
   short  fbits;                      /* status information          */
   char   fvisi;                      /* display qualifier (y/n)     */
   int    color;                      /* color qualifier             */
   int    flag;                       /* flag da face                */
};

struct loop
{
   Id     loopno;                     /* loop identifier             */
   FPTYPE lface;                      /* pointer to face             */
   LPTYPE nextl;                      /* pointer to next loop        */
   LPTYPE prevl;                      /* pointer to previous loop    */
   HPTYPE ledg;                       /* ring of halfedges           */
   short  length;                     /* length of loop              */
};

struct edge
{
   Id     edgeno;                     /* edge identifier             */
   EPTYPE nexte;                      /* pointer to next edge        */
   EPTYPE preve;                      /* pointer to previous edge    */
   HPTYPE he1;                        /* pointer to halfedge         */
   HPTYPE he2;                        /* pointer to halfedge         */
   real   angle;                      /* angle - adjacent faces      */
   char   evis;                       /* display qualifier (y/n)     */
   char   eclp;                       /* clipping qualifier (y/n)    */
   vector svclp3;                     /* 3D clipping coordinates     */
   vector evclp3;                     /* 3D clipping coordinates     */
   vect2  svclp2;                     /* 2D clipping coordinates     */
   vect2  evclp2;                     /* 2D clipping coordinates     */
   int    color;                      /* color qualifier             */

#ifdef __curved_env__
   int    fg_round;                   /* rounding value of edge      */
   int    fg_make;                    /*                             */
   real   t1;                         /* rounding parameter          */
   real   t2;                         /* rounding parameter          */
   real   t3;                         /* rounding parameter          */
   real   t4;                         /* rounding parameter          */
   real   d;
#endif
};

struct halfedge
{
   LPTYPE wloop;                      /* pointer to loop             */
   EPTYPE edg;                        /* edge of loop                */
   HPTYPE nxt;                        /* next halfedge               */
   HPTYPE prv;                        /* previous halfedge           */
   VPTYPE vtx;                        /* starting vertex             */
#ifdef __VIRTUAL_MEM
   HPTYPE nexth;                      /* pointer to the next halfedg */
#endif
#ifdef __curved_env__
   CURVE  *curv;                      /* curve information           */
   real   r;                          /* radius of starting vertex   */
#endif
};

struct vertex
{
   Id     vertexno;                   /* vertex identifier           */
   HPTYPE vedge;                      /* pointer to a halfedge       */
   VPTYPE nextv;                      /* pointer to next vertex      */
   VPTYPE prevv;                      /* pointer to previous vertex  */
   int    fg_round;                   /* rounding flag of vertex     */
   int    fg_make;
   vector vcoord;                     /* vertex coordinates          */
};

union nodes
{
   STYPE s;
   DTYPE d;
   FTYPE f;
   LTYPE l;
   ETYPE e;
   HTYPE h;
   VTYPE v;
   BTYPE b;
   ATYPE a;
   GTYPE g;
};

EXTERN int    maxs;                 /* largest solid no. given so far  */
EXTERN Id     maxf;                 /* largest face no. given so far   */
EXTERN Id     maxv;                 /* largest vertex no. given so far */
EXTERN Id     maxd;                 /* largest shell no. given so far  */
EXTERN int    error;                /* error flag - (SetTrace)         */
EXTERN VPTYPE hitvertex;            /* info from contlv(): vertex hit  */
EXTERN HPTYPE hithe;                /* info from contlv(): edge hit    */
EXTERN SPTYPE objary;               /* first object in the list        */
EXTERN int    SetTrace;
EXTERN int    objused;
EXTERN int    facused;
EXTERN int    looused;
EXTERN int    edgused;
EXTERN int    vtxused;
EXTERN long   defonum;

/* file lowaddel.c */
void MSD_lowAddList(int what, NPTYPE which, NPTYPE where);
void MSD_lowDelList(int what, NPTYPE which, NPTYPE where);

/* file lowaddhe.c */
HPTYPE MSD_lowAddHE(EPTYPE e, VPTYPE v, HPTYPE where, int orient);
HPTYPE MSD_lowDelHE(HPTYPE he);

/* file lowboxop.c */
void MSD_lowSolidBoxFromBoxes(SPTYPE s, BTYPE *b);
int MSD_lowEdgeBoxIntersection(BTYPE *b, EPTYPE e);
int MSD_lowEdgeBoxIntersectionAux(real x1, real x2, real bx1, real bx2);
int MSD_lowBoxBoxIntersection(BTYPE *b1, BTYPE *b2);
int MSD_lowVertexInBox(BTYPE *b, real x, real y, real z);
void MSD_lowSolidBox(SPTYPE s, BTYPE *bx);
void MSD_lowShellBox(DPTYPE d);
void MSD_lowFaceBox(FPTYPE f);
void MSD_lowEdgeBox(BTYPE *b, EPTYPE e);
void MSD_lowEvaluateSolidBoxes(SPTYPE s);
void MSD_lowWidenBox(BTYPE *b, real eps);
void MSD_lowUpdateBox(BTYPE *b1, BTYPE *b);
void MSD_lowTranslatedBox(BTYPE *b, real dx, real dy, real dz);
void MSD_lowUpdateDEBox(void);

/* file lowcheck.c */
void MSD_execTrace(void);
int TraceBoundaryStructure(void);
int TraceSolid(SPTYPE s);
int checkf(SPTYPE s);
int findhes(DPTYPE d, HPTYPE he);
int checke(SPTYPE s);
int findhe(HPTYPE he, LPTYPE l);
int findes(DPTYPE d, EPTYPE e);
int findl(LPTYPE l, FPTYPE f);
int finde(VPTYPE v, HPTYPE he);
int findf(DPTYPE d, FPTYPE f);
int findd(SPTYPE s, DPTYPE d);
int findv(DPTYPE d, VPTYPE v);
int checkv(SPTYPE s);

/* file lowfaceq.c */
int MSD_lowFaceEq(LPTYPE l, vector eq);

/* file lowfinde.c */
SPTYPE MSD_lowFindSolidByName(char *name);
Id MSD_getSolidIdFromName(char *name);
SPTYPE MSD_getSolid(Id sn);
DPTYPE MSD_getShell(SPTYPE s, Id dn);
LPTYPE MSD_getLoop(FPTYPE f, Id vn, HPTYPE *h);
LPTYPE MSD_getEdge(FPTYPE f, Id v1, Id v2, HPTYPE *ptr);
LPTYPE MSD_getHalfedge(FPTYPE f1, FPTYPE f2, Id v1, Id v2, HPTYPE *ptr);
FPTYPE MSD_getFace(SPTYPE s, Id fn);
Id MSD_getMaxSolidId(void);
Id MSD_getMaxShellId(SPTYPE s);
Id MSD_getMaxFaceId(SPTYPE s);
Id MSD_getMaxVertexId(SPTYPE s);

/* file lowinfos.c */
void MSD_lowSetInfo(SPTYPE optr);
void MSD_lowSetLoopEdge(SPTYPE optr);
void MSD_lowCountFaceEdges(SPTYPE optr);

/* file lowlists.c */
void MSD_execListSolid(void);
int MSD_highListSolid(Id sn, int sw);
void MSD_lowListSolid(FILE *file, SPTYPE s, int sw);
void MSD_lowListFace(FILE *file, FPTYPE f, int sw);

/* file lownewel.c */
NPTYPE MSD_lowNewElement(int what, NPTYPE where);
void MSD_lowDelElement(int what, NPTYPE node, NPTYPE where);
int bytesused(SPTYPE s);

/* file lowtrafn.c */
void MSD_execNameAffine(void);
int MSD_highNameAffine(char *name, real a11, real a12, real a13, \
                       real a21, real a22, real a23,             \
                       real a31, real a32, real a33,             \
                       real xc, real yc, real zc);
void MSD_execAffine(void);
int MSD_highAffine(Id sn, real a11, real a12, real a13, \
                   real a21, real a22, real a23,        \
                   real a31, real a32, real a33,        \
                   real xc, real yc, real zc);
int MSD_lowAffine(SPTYPE sptr, vector center, matrix mat);

/* file lowtrans.c */
void MSD_execNameTransformation(void);

int MSD_highNameTransformation(char *name, matrix);
void MSD_execTransformation(void);

int  MSD_highTransformation(Id sn, matrix);
void MSD_lowTransformation(SPTYPE s, matrix);
int MSD_execStructTransformation(EulerOp *op);
void MSD_printStructTransformation(FILE *f, EulerOp *op);
int MSD_readStructTransformation(FILE *f, EulerOp *op);
int MSD_modifyStructTransformation(EulerOp *op);

/* file lowtrrot.c */
void MSD_execNameRotate(void);
int MSD_highNameRotate(char *name, real tx, real ty, real tz);
void MSD_execRotate(void);
int MSD_highRotate(Id sn, real tx, real ty, real tz);
void MSD_lowRotate(SPTYPE s, real tx, real ty, real tz);
int MSD_execStructRotate(EulerOp *op);
void MSD_printStructRotate(FILE *f, EulerOp *op);
int MSD_readStructRotate(FILE *f, EulerOp *op);
int MSD_modifyStructRotate(EulerOp *op);

/* file lowtrrtg.c */
void MSD_execNameRotateSolidAxis(void);
int MSD_highNameRotateSolidAxis(char *name, real x1, real y1, real z1, \
                                real x2, real y2, real z2,             \
                                real th);
void MSD_execRotateSolidAxis(void);
int MSD_highRotateSolidAxis(Id sn, real x1, real y1, real z1, \
                            real x2, real y2, real z2, real th);
int MSD_lowRotateSolidAxis(SPTYPE sptr, vector center, vector axis, real th);

/* file lowtrsca.c */
void MSD_execNameScale(void);
int MSD_highNameScale(char *name, real tx, real ty, real tz);
void MSD_execScale(void);
int MSD_highScale(Id sn, real tx, real ty, real tz);
void MSD_lowScale(SPTYPE s, real tx, real ty, real tz);
int MSD_execStructScale(EulerOp *op);
void MSD_printStructScale(FILE *f, EulerOp *op);
int MSD_readStructScale(FILE *f, EulerOp *op);
int MSD_modifyStructScale(EulerOp *op);

/* file lowtrtra.c */
void MSD_execNameTranslate(void);
int MSD_highNameTranslate(char *name, real tx, real ty, real tz);
void MSD_execTranslate(void);
int MSD_highTranslate(Id sn, real tx, real ty, real tz);
void MSD_lowTranslate(SPTYPE s, real tx, real ty, real tz);
int MSD_execStructTranslate(EulerOp *op);
void MSD_printStructTranslate(FILE *f, EulerOp *op);
int MSD_readStructTranslate(FILE *f, EulerOp *op);
int MSD_modifyStructTranslate(EulerOp *op);

#endif
