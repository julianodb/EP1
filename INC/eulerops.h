/* 
  PROJETO USPDesigner
  MODULO: EUL (Operadores de Euler)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: eulerops.h
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

#ifndef __Eulerops_h
#define __Eulerops_h

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
#ifndef __int_id
 #define __int_id
typedef int               Id;
#endif
#ifndef __double_matrix
 #define __double_matrix
typedef real              matrix[4][4];
#endif
#ifndef __struct_eulerop
 #define __struct_eulerop
typedef struct eulerop    EulerOp;
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

typedef enum   opcode     OpCode;

#ifndef __VIRTUAL_MEM
EXTERN EulerOp *OpHead;           /* head of undo operator sequence       */
EXTERN EulerOp *OpHeadRedo;       /* head of redo operator sequence       */
#else
EXTERN long OpHead;               /* head of undo operator sequence       */
EXTERN long OpHeadRedo;           /* head of redo operator sequence       */
#endif
EXTERN int OpCount;               /* length of undo operator sequence     */
EXTERN int OpCountRedo;           /* length of redo operator sequence     */
EXTERN int Gdoinversion;          /* generate Euler ops for undoing       */
EXTERN int Gdodirect;             /* generate Euler ops for redoing       */
EXTERN int CommandCount;          /* generate command counter             */

/* operation codes for EulerOp */
enum opcode
{
   MVSF,                               /* make vertex solid shell face */
   KVSF,                               /* kill vertex solid shell face */
   MEV,                                /* make edge vertex             */
   KEV,                                /* kill edge vertex             */
   MEF,                                /* make edge face               */
   KEF,                                /* kill edge face               */
   KEMR,                               /* kill edge make ring          */
   MEKR,                               /* make edge kill ring          */
   KFMRH,                              /* kill face make ring hole     */
   MFKRH,                              /* make face kill ring hole     */
   MSFKR,                              /* make shell face kill ring    */
   KSFMR,                              /* kill shell face make ring    */
   HARDREM,                            /* hard remove                  */
   RMOVE,                              /* ring move                    */
   NEWLOC,                             /* locate vertex coordinates    */
   REVERT,                             /* revert topology              */
   MODIFYNAMES,                        /* modify names for Boolean     */
   MERGE,                              /* merge operation              */
   DETACH,                             /* detach operation             */
   TRANS,                              /* translation                  */
   ROTAT,                              /* rotation                     */
   SCALE,                              /* scale                        */
   TRANSFORM,                          /* transformation               */
   ASSOCIATE,                          /* association between elements */
   CRTAXSTRS,                          /* create axis of translation   */
   CRTAXSROT,                          /* create axis of rotation      */
   CRTGRP,                             /* create group                 */
   REMAXS,                             /* remove axis                  */
   REMGRP,                             /* remove group                 */
   MOVAXIS,                            /* move axis of rotation        */
   ROTAXIS,                            /* rotate an axis               */
   SETGRP,                             /* set current group            */
   COLOR,                              /* set color of a solid/group   */
   NAME,                               /* set name of solid/group/axis */

#ifdef __curved_env__
   SETCTAG,                            /* define a tag curve           */
   SETSTAG,                            /* define a tag surface         */
   NEWLINE,                            /* create geometric line        */
   KILLLINE,                           /* kill geometric line          */
   NEWPOLY,                            /* create geom. polyline        */
   KILLPOLY,                           /* kill geometric polyline      */
   ADDLPOINT,                          /* add point to polyline        */
   REMLPOINT,                          /* remove point from polyline   */
   NEWPLANE,                           /* create geometric plane       */
   KILLPLANE,                          /* kill geometric plane         */
   NEWCYLINDER,                        /* create geom. cylinder        */
   KILLCYLINDER,                       /* kill geometric cylinder      */
   NEWCONE,                            /* create geometric cone        */
   KILLCONE,                           /* kill geometric cone          */
   NEWSPHERE,                          /* create geometric sphere      */
   KILLSPHERE,                         /* kill geometric sphere        */
   NEWTORUS,                           /* create geometric torus       */
   KILLTORUS,                          /* kill geometric torus         */
   NEWROTSURF,                         /* create rotational surf.      */
   KILLROTSURF,                        /* kill rotational surf.        */
   NEWBLENDSURF,                       /* create blend surface         */
   KILLBLENDSURF,                      /* kill blend surface           */
#endif
   MAXCODE,                            /* largest opcode assigned      */
};

struct eulerop
{
   OpCode  opcode;                     /* operation code               */
   Id      solidno;                    /* solid Id                     */
   Id      ip1;                        /* integer arguments            */
   Id      ip2;                        /* integer arguments            */
   Id      ip3;                        /* integer arguments            */
   Id      ip4;                        /* integer arguments            */
   Id      ip5;                        /* integer arguments            */
   Id      ip6;                        /* integer arguments            */
   Id      ip7;                        /* integer arguments            */
   Id      ip8;                        /* integer arguments            */
   real    fp1;                        /* float arguments              */
   real    fp2;                        /* float arguments              */
   real    fp3;                        /* float arguments              */
   real    fp4;                        /* float arguments              */
   int     CommandCount;               /* contador de comandos         */
#ifndef __VIRTUAL_MEM
   EulerOp *opnext;                    /* pointer to next operator     */
#else
   long    pp;                         /* pointer to the present op.   */
#endif
};

/* file eulcolor.c */
int MSD_highNameColor(char *name, int color);
int MSD_highNameSolidColor(char *name, int col);
int MSD_highNameGroupColor(char *name, int col);
int MSD_highColor(Id sn, int color, int cod);
void MSD_lowSolidColor(SPTYPE s, int color);
void MSD_lowGroupColor(GPTYPE g, int color);
int MSD_execStructColor(EulerOp *op);
void MSD_printStructColor(FILE *f, EulerOp *op);
int MSD_readStructColor(FILE *f, EulerOp *op);
int MSD_modifyStructColor(EulerOp *op);

/* file eulname_.cpp */
void MSD_execNome(void);
int MSD_highName(Id sn, char *nome, int cod);
void MSD_lowSolidName(SPTYPE s, char *nome);
void MSD_lowGroupName(GPTYPE g, char *nome);
void MSD_lowAxisName(APTYPE a, char *nome);
int MSD_execStructName(EulerOp *op);
void MSD_printStructName(FILE *f, EulerOp *op);
int MSD_readStructName(FILE *f, EulerOp *op);
int MSD_modifyStructName(EulerOp *op);

/* file eulxundo.c */
void MSD_execSetDoInverse(void);
void MSD_lowSetDoInverse(void);

/* file eulerops.c */
void MSD_lowAddEulerOp(SPTYPE a, OpCode b, Id c, Id d, Id e, Id f, \
                       Id g, Id h, Id i, Id j,                     \
                       real k, real l, real m, real n);
void MSD_lowAddEulerOpRedo(SPTYPE a, OpCode b, Id c, Id d, Id e, Id f, \
                           Id g, Id h, Id i, Id j,                     \
                           real k, real l, real m, real n);
void MSD_lowAddMatrix(SPTYPE a, OpCode b, Id c, Id d, Id e, Id f, Id g, Id h, matrix i);
void MSD_lowAddMatrixRedo(SPTYPE s, OpCode opcode, Id ip1, Id ip2, Id ip3, \
                          Id ip4, Id ip5, Id ip6,                          \
                          matrix m);

/* file eulkef__.c */
void MSD_execKEF(void);
int MSD_highKEF(Id sn, Id v1, Id v2, Id fn);
void MSD_execKEF2(void);
int MSD_highKEF2(Id sn, Id v1, Id v2, Id f1, Id f2);
void MSD_lowKEF(HPTYPE he1, HPTYPE he2);
int MSD_execStructKEF(EulerOp *op);
void MSD_printStructKEF(FILE *f, EulerOp *op);
int MSD_readStructKEF(FILE *f, EulerOp *op);
int MSD_modifyStructKEF(EulerOp *op);

/* file eulkemr_.c */
void MSD_execKEMR(void);
int MSD_highKEMR(Id sn, Id fn, Id v1, Id v2);
void MSD_lowKEMR(HPTYPE he1, HPTYPE he2);
int MSD_execStructKEMR(EulerOp *op);
void MSD_printStructKEMR(FILE *f, EulerOp *op);
int MSD_readStructKEMR(FILE *f, EulerOp *op);
int MSD_modifyStructKEMR(EulerOp *op);

/* file eulkev__.c */
void MSD_execKEV(void);
int MSD_highKEV(Id sn, Id fn, Id v1, Id v2);
void MSD_execJVKE(void);
int MSD_highJVKE(Id sn, Id v1, Id v2, Id f);
void MSD_execJEKV(void);
int MSD_highJEKV(Id sn, Id v1, Id v2);
void MSD_execKEV2(void);
int MSD_highKEV2(Id sn, Id v1, Id v2, Id f1, Id f2);
void MSD_lowKEV(HPTYPE he1, HPTYPE he2);
int MSD_execStructKEV(EulerOp *op);
void MSD_printStructKEV(FILE *f, EulerOp *op);
int MSD_readStructKEV(FILE *f, EulerOp *op);
int MSD_modifyStructKEV(EulerOp *op);

/* file eulkfmrh.c */
void MSD_execKFMRH(void);
int MSD_highKFMRH(Id sn, Id fn1, Id fn2);
void MSD_lowKFMRH(FPTYPE f1, FPTYPE f2);
int MSD_execStructKFMRH(EulerOp *op);
void MSD_printStructKFMRH(FILE *f, EulerOp *op);
int MSD_readStructKFMRH(FILE *f, EulerOp *op);
int MSD_modifyStructKFMRH(EulerOp *op);

/* file eulksfmr.c */
void MSD_execKSFMR(void);
int MSD_highKSFMR(Id sn, Id fn1, Id fn2);
void MSD_lowKSFMR(FPTYPE f1, FPTYPE f2);
int MSD_execStructKSFMR(EulerOp *op);
void MSD_printStructKSFMR(FILE *f, EulerOp *op);
int MSD_readStructKSFMR(FILE *f, EulerOp *op);
int MSD_modifyStructKSFMR(EulerOp *op);

/* file eulkvsf_.c */
void MSD_execKVSF(void);
int MSD_highKVSF(Id sn, Id fn);
void MSD_lowKVSF(SPTYPE s);
int MSD_execStructKVSF(EulerOp *op);
void MSD_printStructKVSF(FILE *f, EulerOp *op);
int MSD_readStructKVSF(FILE *f, EulerOp *op);
int MSD_modifyStructKVSF(EulerOp *op);

/* file eulmef__.c */
void MSD_execMEF(void);
int MSD_highMEF(Id sn, Id v1, Id v2, Id f1, Id f2);
void MSD_execMEF2(void);
int MSD_highMEF2(Id sn, Id v1, Id v2, Id v3, Id v4, Id f1, Id f2, Id f3, Id f4);
FPTYPE MSD_lowMEF(HPTYPE he1, HPTYPE he2, Id fn);
int MSD_execStructMEF(EulerOp *op);
void MSD_printStructMEF(FILE *f, EulerOp *op);
int MSD_readStructMEF(FILE *f, EulerOp *op);
int MSD_modifyStructMEF(EulerOp *op);

/* file eulmekr_.c */
void MSD_execMEKR(void);
int MSD_highMEKR(Id sn, Id fn, Id v1, Id v2);
void MSD_execMEKR2(void);
int MSD_highMEKR2(Id sn, Id v1, Id v2, Id v3, Id v4, Id f1, Id f2, Id f3);
void MSD_lowMEKR(HPTYPE he1, HPTYPE he2);
int MSD_execStructMEKR(EulerOp *op);
void MSD_printStructMEKR(FILE *f, EulerOp *op);
int MSD_readStructMEKR(FILE *f, EulerOp *op);
int MSD_modifyStructMEKR(EulerOp *op);

/* file eulmev__.c */
void MSD_execMEV(void);
int MSD_highMEV(Id sn, Id fn, Id v1, Id v2, real x, real y, real z);
void MSD_execSEMV(void);
int MSD_highSEMV(Id sn, Id v1, Id v2, Id v3, real x, real y, real z);
void MSD_execSVME(void);
int MSD_highSVME(Id sn, Id v1, Id v2, Id f1, Id f2, real x, real y, real z);
void MSD_execSVME2(void);
int MSD_highSVME2(Id sn, Id v1, Id v2, Id v3, Id v4, Id f1, Id f2, real x,
                  real y, real z);
void MSD_execSVME3(void);
int MSD_highSVME3(Id sn, Id v1, Id v2, Id v3, Id v4, Id f1, Id f2, Id f3, Id f4, \
                  real x, real y, real z);
void MSD_lowMEV(HPTYPE he1, HPTYPE he2, Id vn, real x, real y, real z);
int MSD_execStructMEV(EulerOp *op);
void MSD_printStructMEV(FILE *f, EulerOp *op);
int MSD_readStructMEV(FILE *f, EulerOp *op);
int MSD_modifyStructMEV(EulerOp *op);

/* file eulmfkrh.c */
void MSD_execMFKRH(void);
int MSD_highMFKRH(Id sn, Id f1, Id f2);
void MSD_execMFKRH2(void);
int MSD_highMFKRH2(Id sn, Id f1, Id f2, Id v1, Id v2);
FPTYPE MSD_lowMFKRH(FPTYPE f, LPTYPE l, Id fn);
int MSD_execStructMFKRH(EulerOp *op);
void MSD_printStructMFKRH(FILE *f, EulerOp *op);
int MSD_readStructMFKRH(FILE *f, EulerOp *op);
int MSD_modifyStructMFKRH(EulerOp *op);

/* file eulmodif.c */
SPTYPE MSD_lowModifyEulerOp(EulerOp *op);
int MSD_modifyStructError(EulerOp *op);
void MSD_lowIniciaTranslatorTable(void);
int MSD_lowCheckTranslatorTable(int elemno, int code);
void MSD_lowSetTranslatorTable(int elemnoold, int elemno, int code);

/* file eulmsfkr.c */
void MSD_execMSFKR(void);
int MSD_highMSFKR(Id sn, Id ff, Id fn, Id dn);
void MSD_execMSFKR2(void);
int MSD_highMSFKR2(Id sn, Id ff, Id fn, Id dn, Id v1, Id v2);
FPTYPE MSD_lowMSFKR(LPTYPE l, Id fn, Id dn);
int MSD_execStructMSFKR(EulerOp *op);
void MSD_printStructMSFKR(FILE *f, EulerOp *op);
int MSD_readStructMSFKR(FILE *f, EulerOp *op);
int MSD_modifyStructMSFKR(EulerOp *op);
void markface(FPTYPE f, DPTYPE d);
void redo(DPTYPE shell, DPTYPE oldshell);
int sfindv(DPTYPE d, VPTYPE v);
int sfinde(DPTYPE d, EPTYPE e);

/* file eulmvsf_.c */
void MSD_execMVSF(void);
SPTYPE MSD_highMVSF(Id fn, Id vn, Id dn, real xx, real yy, real zz);
SPTYPE MSD_lowMVSF(Id sn, Id fn, Id vn, Id dn, real xx, real yy, real zz);
int MSD_execStructMVSF(EulerOp *op);
void MSD_printStructMVSF(FILE *f, EulerOp *op);
int MSD_readStructMVSF(FILE *f, EulerOp *op);
int MSD_modifyStructMVSF(EulerOp *op);

/* file eulprint.c */
void MSD_highPrintAllOps(void);
void MSD_highPrintAllRedoOps(void);
void MSD_lowPrintEulerOp(FILE *f, EulerOp *op);
void MSD_printStructError(FILE *f, EulerOp *op);

/* file eulread_.c */
int MSD_lowScanEulerOperator(FILE *f, EulerOp *op);
int MSD_readStructError(FILE *f, EulerOp *op);

/* file eulscoef.c */
void MSD_setEulCoef(SPTYPE s, int dn, int fn, int rn, int en, int vn, int hn);
void MSD_addEulCoef(SPTYPE s, int dn, int fn, int rn, int en, int vn, int hn);

/* file eulxundo.c */
void MSD_execUndoOp(void);
void MSD_highUndoOp(void);
SPTYPE MSD_lowUndoOp(void);
SPTYPE MSD_lowApplyEulerOp(EulerOp *op, int *cont);
int MSD_execStructError(EulerOp *op);
void MSD_lowFreeUndoLog(PEulerOp EndOp);
void MSD_lowGetMatrix(EulerOp *op, matrix m);

/* file topclean.c */
void MSD_execCleanSolid(void);
void MSD_execNameCleanSolid(void);
int MSD_highNameCleanSolid(char *);
int MSD_highCleanSolid(Id sn);
void MSD_lowCleanSolid(SPTYPE s);

/* file topdetac.c */
void MSD_execDetach(void);
int MSD_highDetach(Id sn, Id sn1, Id dn, Id newd, int addf, int addv);
SPTYPE MSD_lowDetach(SPTYPE s, Id sn, DPTYPE d, Id newd, int addf, int addv);
void MSD_lowShellDetach(SPTYPE s, SPTYPE s1, DPTYPE d, Id newd, int addf, int addv);
int MSD_execStructDetach(EulerOp *op);
void MSD_printStructDetach(FILE *f, EulerOp *op);
int MSD_readStructDetach(FILE *f, EulerOp *op);
int MSD_modifyStructDetach(EulerOp *op);

/* file toplglue.c */
void MSD_execGlue(void);
int MSD_highGlue(Id sn1, Id sn2);
void MSD_lowGlue(SPTYPE s1, SPTYPE s2);
void MSD_lowFaceGlue(SPTYPE s1, SPTYPE s2, FPTYPE f1, FPTYPE f2);
int MSD_lowLoopGlue(FPTYPE f);

/* file topmerge.c */
void MSD_execNameMerge(void);
void MSD_execMerge(void);
int MSD_highNameMerge(char *, char *);
int MSD_highMerge(Id sn1, Id sn2);
void MSD_lowMerge(SPTYPE s1, SPTYPE s2);

/* file topmodif.c */
void MSD_execModifyIds(void);
int MSD_highModifyIds(Id sn, Id snew);
void MSD_lowModifyIds(SPTYPE s, Id snew);
int MSD_execStructModifyIds(EulerOp *op);
void MSD_printStructModifyIds(FILE *f, EulerOp *op);
int MSD_readStructModifyIds(FILE *f, EulerOp *op);
int MSD_modifyStructModifyIds(EulerOp *op);

/* file topnewlc.c */
void MSD_execNewVertexLocation(void);
int MSD_highNewVertexLocation(Id sn, Id vn, real x, real y, real z);
void MSD_lowNewVertexLocation(SPTYPE s, VPTYPE v, real x, real y, real z);
int MSD_execStructNewVertexLocation(EulerOp *op);
void MSD_printStructNewVertexLocation(FILE *f, EulerOp *op);
int MSD_readStructNewVertexLocation(FILE *f, EulerOp *op);
int MSD_modifyStructNewVertexLocation(EulerOp *op);

/* file toprever.c */
void MSD_execRevert(void);
int MSD_highRevert(Id sn, Id dn);
int MSD_lowRevert(DPTYPE d);
int MSD_execStructRevert(EulerOp *op);
void MSD_printStructRevert(FILE *f, EulerOp *op);
int MSD_readStructRevert(FILE *f, EulerOp *op);
int MSD_modifyStructRevert(EulerOp *op);

/* file topringm.c */
void MSD_execRingMove(void);
int MSD_highRingMove(Id sn, Id v1, Id v2, Id f1, Id f2, int out);
int MSD_lowRingMove(FPTYPE f1, FPTYPE f2, LPTYPE l, int out);
int MSD_execStructRingMove(EulerOp *op);
void MSD_printStructRingMove(FILE *f, EulerOp *op);
int MSD_readStructRingMove(FILE *f, EulerOp *op);
int MSD_modifyStructRingMove(EulerOp *op);
void MSD_execAllRingMove(void);
int MSD_highAllRingMove(Id sn, Id f1, Id f2);
int MSD_lowAllRingMove(FPTYPE f1, FPTYPE f2);

/* file topremhd.c */
void MSD_execHardRemove(void);
int MSD_highHardRemove(Id sn);
int MSD_lowHardRemove(SPTYPE s);
int MSD_execStructHardRemove(EulerOp *op);
void MSD_printStructHardRemove(FILE *f, EulerOp *op);
int MSD_readStructHardRemove(FILE *f, EulerOp *op);
int MSD_modifyStructHardRemove(EulerOp *op);

/* file topremsf.c */
void MSD_execNameSoftRemove(void);
int MSD_highNameSoftRemove(char *name);
void MSD_execSoftRemove(void);
int MSD_highSoftRemove(Id sn);
int MSD_lowSoftRemove(SPTYPE s);
void MSD_lowShellRemove(SPTYPE s);
void MSD_lowEdgeRemove(SPTYPE s);
void MSD_lowFaceRemove(SPTYPE s);

#endif
