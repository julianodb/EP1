/* 
  PROJETO USPDesigner
  MODULO: DIS (Hidden Line Elimination)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: disdispl.h
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

#ifndef __CamDispc__h
#define __CamDispc__h

#ifndef EXTERN
 #ifdef _DVS_
  #define EXTERN
 #else
  #define EXTERN    extern
 #endif
#endif

#include <stdio.h>

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
#ifndef __struct_solid
 #define __struct_solid
typedef struct solid      STYPE;
#endif
#ifndef __struct_face
 #define __struct_face
typedef struct face       FTYPE;
#endif
#ifndef __struct_edge
 #define __struct_edge
typedef struct edge       ETYPE;
#endif
#ifndef __struct_halfedge
 #define __struct_halfedge
typedef struct halfedge   HTYPE;
#endif
#ifndef __struct_grouptp
 #define __struct_grouptp
typedef struct grouptp    GTYPE;
#endif
#ifndef __struct_box
 #define __struct_box
typedef struct box        BTYPE;
#endif
#ifndef __struct_cameratp
 #define __struct_cameratp
typedef struct cameratp   CTYPE;
#endif

EXTERN int MAX_COLOR_NUMBER;

struct cameratp
{
   Id     cn;
   char   cname[18];
   vector eye;
   vector ref;
   vector eyedir;
   real   scale;
   real   cdis;
   int    hid;
   int    dash;
   int    smooth;
   int    mode;
   matrix drot;
   CTYPE  *nxt;
};

typedef struct
{
   vector devtx;
   char   devis;
} DEDGTYPE;

EXTERN DEDGTYPE *divedge;
EXTERN DEDGTYPE *divedptr;
EXTERN int      divednum;
EXTERN vector   desvtx;
EXTERN vector   deevtx;
EXTERN BTYPE    debox;

EXTERN int   Draw;
EXTERN int   NumCinema;
EXTERN FILE  *dspfwptr;
EXTERN FILE  *dspfrptr;
EXTERN int   breakflg;
EXTERN real  abseps;
EXTERN real  xyratio;
EXTERN CTYPE camera3;
EXTERN CTYPE *camary;
EXTERN CTYPE *camptr;
EXTERN int   camused;
EXTERN real  angsmooth;
EXTERN char  monisw;
EXTERN CTYPE *monicptr;

EXTERN int defltcol;
EXTERN int framecol;
EXTERN int lncolor;
EXTERN int lntype;

/* file camaltcr.c */
int MSD_highAlteraCurrentCamera(char opt, real Ex, real Ey, real Ez,
                                real Rx, real Ry, real Rz, real scale,
                                int mode, int hidtype, int dash, int smooth);

/* file camaltpr.c */
int MSD_highNameAlteraCamera(char opt, char *name, real Ex, real Ey,
                             real Ez, real Rx, real Ry, real Rz,
                             real scale, int mode, int hidtype, int dash,
                             int smooth);
int MSD_highAlteraCamera(char opt, Id cn, real Ex, real Ey, real Ez,
                         real Rx, real Ry, real Rz, real scale,
                         int mode, int hidtype, int dash, int smooth);
int MSD_lowAlteraCamera(char opt, CTYPE *cptr, real Ex, real Ey,
                        real Ez, real Rx, real Ry, real Rz,
                        real scale, int mode, int hidtype, int dash,
                        int smooth);

/* file camcheck.c */
int MSD_highCheckCamera(real, real, real, real, real, real, real, int,
                        int, int, int);
int  MSD_lowCheckCamera(vector, vector, real, int, int, int, int);
void MSD_highSetCamera(CTYPE *, real, real, real, real, real, real,
                       real, int, int, int, int);
void MSD_lowSetCamera(CTYPE *, vector, vector, real, int, int, int, int);
void MSD_lowSetCameraTransformation(CTYPE *cptr);
void MSD_lowSetCamera3View(int type, CTYPE *cptr);


/* file camcopy_.c */
void MSD_execNameCopyCamera(void);
int MSD_highNameCopyCamera(char *name1, char *name2);
void MSD_execCopyCamera(void);
int MSD_highCopyCamera(int cn);
int MSD_lowCopyCamera(CTYPE *cptr, Id cn);

/* file camgetpr.c */
void MSD_execGetCurrentCameraParameters(void);
int MSD_getCurrentCameraParameters(real *Ex, real *Ey, real *Ez,
                                   real *Rx, real *Ry, real *Rz,
                                   real *ampl, int *mode, int *elim,
                                   int *hash, int *edge);

/* file camlist_.c */
void MSD_execNameListCameraName(void);
int MSD_highNameListCameraName(char *name);
int MSD_lowNameListCameraName(CTYPE *cptr);
void MSD_lowListAllCamera(void);
void MSD_lowListAllCameraName(void);

/* file cammanip.c */
int MSD_getNumberOfCameras(void);
char **MSD_putCameraNameInList(char **list);
CTYPE *MSD_getCamera(Id cn);
Id MSD_getCameraIdFromName(char *nam);
CTYPE *MSD_lowFindCameraByName(char *nam);
CTYPE *MSD_lowFindCamera(Id cn);
Id MSD_getMaxCameraId(void);

/* file camnew__.c */
void MSD_execNameCreateCamera(void);
int MSD_highNameCreateCamera(char *name, real Ex, real Ey, real Ez,
                             real Rx, real Ry, real Rz, real scale,
                             int mode, int hidtype, int dash, int smooth);
void MSD_execCreateCamera(void);
int MSD_highCreateCamera(real Ex, real Ey, real Ez,
                         real Rx, real Ry, real Rz, real scale,
                         int mode, int hidtype, int dash, int smooth);
int MSD_middleCreateCamera(int cn, real Ex, real Ey, real Ez, real Rx,
                           real Ry, real Rz, real scale, int mode,
                           int hidtype, int dash, int smooth);
CTYPE *MSD_lowCreateCamera(Id cn, real Ex, real Ey, real Ez,
                           real Rx, real Ry, real Rz, real scale,
                           int mode, int hidtype, int dash, int smooth);

/* file camremov.c */
void MSD_execNameRemoveCamera(void);
int MSD_highNameRemoveCamera(char *name);
void MSD_execRemoveCamera(void);
int MSD_highRemoveCamera(Id cn);
int MSD_lowRemoveCamera(CTYPE *cptr);

/* file camrenam.c */
void MSD_execNameRenameCamera(void);
int MSD_highNameRenameCamera(char *name1, char *name2);
int MSD_lowNameRenameCamera(CTYPE *cptr, char *name2);

/* file camsetcr.c */
void MSD_execNameSetCurrentCamera(void);
int MSD_highNameSetCurrentCamera(char *name);
void MSD_execSetCurrentCamera(void);
int MSD_highSetCurrentCamera(Id cn);
int MSD_lowSetCurrentCamera(CTYPE *cptr);

/* file camtrncr.c */
void MSD_execTranslateCurrentCamera(void);
void MSD_lowTranslateCurrentCamera(real dEx, real dEy, real dEz);

/* file disbreak.c */
#if !defined (__UNIX_) && !defined (_Windows)
void MSD_lowSetBreakInterruption(void);
void interrupt far MSD_lowBreakFunction(...);
void MSD_lowSetBreakResident(void);

#endif

/* file discamer.c */
void MSD_execCamera(void);

/* file discaxis.c */
void MSD_execDisplayCoordinates(void);
int MSD_highDisplayCoordinates(int opt, int dir, real dis, real l, real dl, int acolor, int mcolor);
int MSD_lowDisplayAxis(real len, int acolor);
int MSD_lowDisplayMesh(int dir, real dis, real len, real dl, int acolor, int mcolor);
void MSD_lowDisplayLineCoordinates(int lt, vector sv, vector ev);

/* file disccolr.c */
void MSD_execColor(void);
int MSD_lowSetFrameColor(int color);
int MSD_lowSetDefaultColor(int color);
int MSD_lowGetIntersectionColor(SPTYPE o1ptr, SPTYPE o2ptr);
int MSD_lowGetSolidColor(SPTYPE optr);

/* file discdisp.c */
void MSD_execDisplay(void);
int MSD_highNameDisplay(char *cname, char *olist);
int MSD_highNameSetDisplay(char *oname);
void MSD_highDisplayAllEdges(void);
void MSD_lowDisplayAllEdges(void);
void MSD_highDisplayLocalHidden(void);
void MSD_lowDisplayLocalHidden(CTYPE *cptr);
void MSD_lowDisplayLine(int l, vector sv, vector ev);
void MSD_lowDisplayVertex(int l, vector v);

/* file discfile.c */
void MSD_execDisplayFile(void);
int MSD_highDisplayFile(char rw, char *fname);
int MSD_lowDisplayReadFile(char *fname);
int MSD_lowDisplayWriteFile(char *fname);

/* file discpli3.c */
int MSD_lowClip3Dimension(vector v1, vector v2);
int MSD_lowCheckClipDisplay(vector t1, vector t2);
int MSD_lowClipingPlane(vector v1, vector v2, vector normal, real d);
void MSD_lowDetermineVisibility(CTYPE *cptr);
void MSD_lowEdgeClip(EPTYPE eptr);

/* file disedge_.c */
void MSD_execDisplayEdge(void);
void MSD_highDisplayEdge(Id s, Id e, int color);
void MSD_lowDisplayEdge(EPTYPE eptr, int color);

/* file dishediv.c */
void MSD_lowEdgeEdgeIntersection(SPTYPE optr, EPTYPE eptr, FPTYPE f1ptr, FPTYPE f2ptr);

int MSD_lowEdgeEdgeCheck(SPTYPE xoptr, SPTYPE optr, EPTYPE xeptr, EPTYPE eptr, \
                         real sbox[][2], FPTYPE f1ptr, FPTYPE f2ptr);
int MSD_lowEdgeEdgeCheckStep2(vector a1, vector a2, vector b1, vector b2, real *u, real *v);
int MSD_lowEdgeEdgeCheckStep3(vector s0, vector s1, vector v0, vector v1, real u, vector p);
void MSD_lowEdgeFaceIntersection(SPTYPE o1ptr, SPTYPE o2ptr, FPTYPE f1ptr, FPTYPE f2ptr);
void MSD_lowAddEdgeDivision(vector pos);

/* file dishevis.c */
void MSD_lowSetEdgeVisibility(char, FPTYPE, FPTYPE, vector);
int  MSD_lowIntersectionCheck(vector, FPTYPE, FPTYPE);
int  MSD_lowIntersectionCheckStep1(vector, FPTYPE, FPTYPE);
int  MSD_lowPositionalVisibility(char, vector, FPTYPE, FPTYPE, vector);
int  MSD_lowEdgePlaneIntersection(vector, vector, FPTYPE, real *, real *);

/* file dishidde.c */
void MSD_lowCameraDisplayHidden(void);
void MSD_lowDisplayHidden(CTYPE *cptr);

/* file dishinfc.c */
int MSD_lowIsInsideFace(vector pos, FPTYPE xfptr, HPTYPE *ei);
int MSD_lowIsInsideLoop(vector pos, LPTYPE lptr, HPTYPE *ei);

/* file dishintr.c */
void MSD_lowCameraDisplayIntersection(void);
void MSD_lowDisplayIntersection(void);

void MSD_lowIntersectionFaceFace(SPTYPE, FPTYPE, SPTYPE, FPTYPE);
int MSD_lowIntersectionParallelFaces(FPTYPE f1ptr, FPTYPE f2ptr);
void MSD_lowIntersectionFaceEdge(FPTYPE f1ptr, FPTYPE f2ptr);
int MSD_lowIntersectionSeparateMinMax(void);
void MSD_lowIntersectionClipDropCoord(int idx);

/* file dishsore.c */
void MSD_lowSortDividedEdge(void);

/* file dishvisb.c */
int MSD_lowIsEdgeVisible(CTYPE *cptr, EPTYPE eptr);
int MSD_lowIsFaceVisible(CTYPE *cptr, FPTYPE fptr);
int MSD_lowIsInvertedEdge(int isleft, vector dir, FPTYPE fp, vector edir);

/* file disledge.c */
void MSD_lowDisplayDividedEdge(CTYPE *cptr, EPTYPE eptr);

/* file dislintf.c */
void g_frame(void);
void g_tpoint(int lt, vect2 v);
void g_tline(int lt, vect2 sv, vect2 ev);
void g_iline(int lt, int *sv, int *ev);
void h_line(void);
void g_open(void);
void g_filbox(int color, vect2 v0, vect2 v1);
void g_clear(void);
void g_pcolor(int color);
void g_close(void);
void gdcpos(vect2 npos, int *dpos);

/* file disltrns.c */
void MSD_lowDisplayTransformation(vector, vector);
void MSD_lowInverseDisplayTransformation(vector, vector);
void MSD_lowApplyDisplayTransformation(matrix, vector, vector);
void MSD_lowExecInverseDisplayTransformation(vect2, vect2, vector, vector);
char MSD_lowExecDisplayTransformation(vector, vector, vect2, vect2);
void MSD_lowDisplayTransformationScale(vector, vector, vector, vector);
char MSD_lowClipDisplayTransformation(vector, vector, vector, vector);

/* file dismonit.c */
void MSD_execMonitor(void);
int MSD_highMonitor(char *name);
int MSD_lowMonitor(CTYPE *cptr, int flag);
void MSD_lowDisplayMonitor(int t, SPTYPE optr, GPTYPE gptr);
void MSD_lowDisplayMonitorSolid(SPTYPE op);
void MSD_lowDisplayMonitorGroup(GPTYPE gptr);
void MSD_lowDisplayMonitorAxis(void);

#endif
