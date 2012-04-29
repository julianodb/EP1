/* 
  PROJETO USPDesigner
  MODULO: CUR (Operacao com curvas)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: curvgeom.h
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

#ifndef __curvgeom_h
#define __curvgeom_h

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
typedef real              vector[4];             /* vector of xyz-space   */
#endif
#ifndef __int_id
 #define __int_id
typedef int               Id;
#endif
#ifndef __double_matrix
 #define __double_matrix
typedef real              matrix[4][4];
#endif
#ifndef __struct_solid
 #define __struct_solid
typedef struct solid      STYPE;
#endif
#ifndef __struct_halfedge
 #define __struct_halfedge
typedef struct halfedge   HTYPE;
#endif
#ifndef __struct_eulerop
 #define __struct_eulerop
typedef struct eulerop    EulerOp;
#endif

#ifdef __curved_env__
typedef enum   cname      CNAME;
typedef struct lpoint     LPOINT;
 #ifndef __CURVE
  #define __CURVE
typedef struct curve      CURVE;
 #endif

enum cname
{
   LINE1,
   CURVE1,
};

struct lpoint
{
   vector pnt;                         /* point coordinates                */
   LPOINT *pnxt;                       /* pointer to next point            */
};

struct curve
{
   Id     tagno;                       /* curve identifier                 */
   CNAME  tag_type;                    /* curve type                       */
   short  times_used;                  /* times used curve                 */
   CURVE  *nextc;                      /* pointer to next curve            */
   CURVE  *prevc;                      /* pointer to previous curve        */
   LPOINT *lpnt;                       /* header to list of points         */
};

EXTERN Id maxct;
 #define MAXMEMO    10
EXTERN HPTYPE he_store[MAXMEMO];
EXTERN matrix Mc;


/* file curaddpt.c */
void MSD_execAddPointToCurve(void);
int MSD_highAddPointToCurve(Id s, Id tag, real x, real y, real z);
LPOINT *MSD_lowAddPointToCurve(SPTYPE s, CURVE *curv, real x, real y, real z);
int MSD_execStructADDLPOINT(EulerOp *op);
void MSD_printStructADDLPOINT(FILE *f, EulerOp *op);
int MSD_readStructADDLPOINT(FILE *f, EulerOp *op);
int MSD_modifyStructADDLPOINT(EulerOp *op);

/* file currempt.c */
void MSD_execRemovePointFromCurve(void);
int MSD_highRemovePointFromCurve(Id sn, Id tag);
LPOINT *MSD_lowRemovePointFromCurve(SPTYPE s, CURVE *curv);
int MSD_execStructREMLPOINT(EulerOp *op);
void MSD_printStructREMLPOINT(FILE *f, EulerOp *op);
int MSD_readStructREMLPOINT(FILE *f, EulerOp *op);
int MSD_modifyStructREMLPOINT(EulerOp *op);

/* file curaux__.c */
void hget_he(void);
int get_he(char *sn, Id v1, Id v2, Id fn, int memohe);
void lget_he(HPTYPE he, int memohe);
void hget_mate(void);
int get_mate(int memohe, int memomate);
void lget_mate(HPTYPE he, int memomate);
void hget_next(void);
int get_next(int memohe, int memonext);
void lget_next(HPTYPE he, int memonext);
void hget_prev(void);
int get_prev(int memohe, int memoprev);
void lget_prev(HPTYPE he, int memoprev);
void hcmef(void);
int cmef(int memohe1, int memohe2);
void hcmev(void);
int cmev(int memohe1, int memohe2, real x, real y, real z);
void cnewloc(void);
int ccnewloc(int memohe, real x, real y, real z);
void hshow_memo(void);
void lshow_memo(int memohe);
void cputctag(void);
void ceval_polyline(void);

/* file curdelem.c */
void  MSD_lowCurveDeleteElement(CURVE *curv, SPTYPE s);

/* file cureline.c */
void MSD_execEvaluateCurveLine(void);
int MSD_highEvaluateCurveLine(Id sn, Id fn, Id v1, Id v2, int nsegm);
void MSD_lowEvaluateCurveLine(HPTYPE he, int nsegm);

/* file curepoly.c */
void MSD_execEvaluateCurvePolyline(void);
int MSD_highEvaluateCurvePolyline(Id sn, Id fn, Id v1, Id v2, int nsegm);
void MSD_lowEvaluateCurvePolyline(HPTYPE he, int nsegm);

/* file curfinde.c */
CURVE *MSD_getCurve(SPTYPE s, Id tag);

/* file curkline.c */
void MSD_execKillCurveLine(void);
int MSD_highKillCurveLine(Id sn, Id tag);
int MSD_lowKillCurveLine(SPTYPE s, CURVE *l);
int MSD_execStructKILLLINE(EulerOp *op);
void MSD_printStructKILLLINE(FILE *f, EulerOp *op);
int MSD_readStructKILLLINE(FILE *f, EulerOp *op);
int MSD_modifyStructKILLLINE(EulerOp *op);

/* file curkpoly.c */
void MSD_execKillCurvePolyline(void);
int MSD_highKillCurvePolyline(Id sn, Id tag);
int MSD_lowKillCurvePolyline(SPTYPE s, CURVE *c);
int MSD_execStructKILLPOLY(EulerOp *op);
void MSD_printStructKILLPOLY(FILE *f, EulerOp *op);
int MSD_readStructKILLPOLY(FILE *f, EulerOp *op);
int MSD_modifyStructKILLPOLY(EulerOp *op);

/* file curmline.c */
void MSD_execNewCurveLine(void);
int MSD_highNewCurveLine(Id sn, Id tag);
CURVE *MSD_lowNewCurveLine(SPTYPE s, Id tag);
int MSD_execStructNEWLINE(EulerOp *op);
void MSD_printStructNEWLINE(FILE *f, EulerOp *op);
int MSD_readStructNEWLINE(FILE *f, EulerOp *op);
int MSD_modifyStructNEWLINE(EulerOp *op);

/* file curmnewe.c */
CURVE *MSD_lowNewCurve(CNAME what, SPTYPE s);

/* file curmpoly.c */
void MSD_execNewCurvePolyline(void);
int MSD_highNewCurvePolylineWithPoint(Id sn, Id tag, real x, real y, real z);
int MSD_highNewCurvePolyline(Id sn, Id tag);
CURVE *MSD_lowNewCurvePolyline(SPTYPE s, Id tag);
int MSD_execStructNEWPOLY(EulerOp *op);
void MSD_printStructNEWPOLY(FILE *f, EulerOp *op);
int MSD_readStructNEWPOLY(FILE *f, EulerOp *op);
int MSD_modifyStructNEWPOLY(EulerOp *op);

/* file curmplen.c */
void MSD_execGetNumberPoints(void);
int MSD_highGetNumberPoints(Id sn, Id tag);
int MSD_lowGetNumberPoints(LPOINT *plist);

/* file cursettg.c */
void MSD_execSetCurveAtributeToEdge(void);
int MSD_highSetCurveAtributeToEdge(Id sn, Id tag, Id fn, Id v1, Id v2);
int MSD_lowSetCurveAtributeToEdge(HPTYPE he, CURVE *tag);
int MSD_highSetCurveAtribute(Id sn, Id tag, Id fn, Id v1, Id v2);
int MSD_lowSetCurveAtribute(HPTYPE he, CURVE *tag);
int MSD_execStructSETCTAG(EulerOp *op);
void MSD_printStructSETCTAG(FILE *f, EulerOp *op);
int MSD_readStructSETCTAG(FILE *f, EulerOp *op);
int MSD_modifyStructSETCTAG(EulerOp *op);

#endif
#endif
