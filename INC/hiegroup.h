/* 
  PROJETO USPDesigner
  MODULO: HIE (Estrutura Hierarquica)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: hiegroup.h
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

#ifndef __HieGroup_h
#define __HieGroup_h

#include <stdio.h>

#ifndef EXTERN
 #ifdef _DVS_
  #define EXTERN
 #else
  #define EXTERN    extern
 #endif
#endif

extern char *x_malloc(unsigned int a);
extern char *x_calloc(unsigned int a, unsigned int b);
extern char x_free(char *a);

#ifndef __double_real
 #define __double_real
typedef double            real;
#endif
#ifndef __double_matrix
 #define __double_matrix
typedef real              matrix[4][4];   /* transformation matrix */
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
#ifndef __struct_axistp
 #define __struct_axistp
typedef struct axistp     ATYPE;
#endif
#ifndef __struct_grouptp
 #define __struct_grouptp
typedef struct grouptp    GTYPE;
#endif
#ifndef __struct_eulerop
 #define __struct_eulerop
typedef struct eulerop    EulerOp;
#endif

typedef union  hierarq    QTYPE;

struct grouptp
{
   Id     groupno;
   char   gname[18];
   GPTYPE parent;
   APTYPE gaxs;
   GPTYPE nextg;
   GPTYPE prevg;
   int    gcolor;
};

union hierarq
{
   GPTYPE agroup;
   SPTYPE asolid;
};

struct axistp
{
   Id     axisno;
   char   aname[18];
   char   type;
#ifndef __VIRTUAL_MEM
   QTYPE  hierarq;
#else
   long   hierarq;
#endif
   APTYPE nexta;
   APTYPE preva;
   char   amode;
   vector acenter;
   vector avector;
};

EXTERN APTYPE axsary;
EXTERN int    axsused;
EXTERN Id     AxisNo;

EXTERN GPTYPE grpary;
EXTERN GPTYPE def_grpptr;
EXTERN int    grpused;
EXTERN Id     GroupNo;

GPTYPE MSD_getGroup(Id);
APTYPE MSD_getAxis(Id);
Id MSD_getGroupIdFromName(char *);
Id MSD_getAxisIdFromName(char *);
Id MSD_getMaxGroupId(void);


/* file hieassag.c */
void MSD_execNameAssociaGroupAxis(void);
int MSD_highNameAssociaGroupAxis(char *name1, char *name2);
void MSD_execAssociaGroupAxis(void);
int MSD_highAssociaGroupAxis(int sn, int an);
int MSD_lowAssociaGroupAxis(APTYPE aptr, GPTYPE gptr);

/* file hieassas.c */
void MSD_execNameAssociaSolidAxis(void);
int MSD_highNameAssociaSolidAxis(char *, char *);
void MSD_execAssociaSolidAxis(void);
int MSD_highAssociaSolidAxis(int sn, int an);
int MSD_lowAssociaSolidAxis(APTYPE aptr, SPTYPE optr);
int MSD_execStructAssociate(EulerOp *op);
void MSD_printStructAssociate(FILE *f, EulerOp *op);
int MSD_readStructAssociate(FILE *f, EulerOp *op);
int MSD_modifyStructAssociate(EulerOp *op);

/* file hieassgg.c */
void MSD_execNameAssociaGroupGroup(void);
int MSD_highNameAssociaGroupGroup(char *name1, char *name2);
void MSD_execAssociaGroupGroup(void);
int MSD_highAssociaGroupGroup(int gn1, int gn2);
int MSD_lowAssociaGroupGroup(GPTYPE gptr1, GPTYPE gptr2);

/* file hieassgs.c */
void MSD_execNameAssociaGroupSolid(void);
int MSD_highNameAssociaGroupSolid(char *name1, char *name2);
void MSD_execAssociaGroupSolid(void);
int MSD_highAssociaGroupSolid(int gn, int sn);
int MSD_lowAssociaGroupSolid(GPTYPE gptr, SPTYPE optr);

/* file hieaxscd.c */
void MSD_execNameCreateAxisTranslation(void);
int MSD_highNameCreateAxisTranslation(char *name, real x, real y, real z);
void MSD_execCreateAxisTranslation(void);
int MSD_highCreateAxisTranslation(real x, real y, real z);
int MSD_middleCreateAxisTranslation(int an, real x, real y, real z);
int MSD_lowCreateAxisTranslation(int an, vector vec);
int MSD_execStructCrtAxsTranslation(EulerOp *op);
void MSD_printStructCrtAxsTranslation(FILE *f, EulerOp *op);
int MSD_readStructCrtAxsTranslation(FILE *f, EulerOp *op);
int MSD_modifyStructCrtAxsTranslation(EulerOp *op);

/* file hieaxscr.c */
void MSD_execNameCreateAxisRotation(void);
int MSD_highNameCreateAxisRotation(char *name, real x1, real y1, real z1,
                                   real x2, real y2, real z2);
void MSD_execCreateAxisRotation(void);
int MSD_highCreateAxisRotation(real x1, real y1, real z1,
                               real x2, real y2, real z2);
int MSD_middleCreateAxisRotation(int an, real x1, real y1, real z1,
                                 real x2, real y2, real z2);
int MSD_lowCreateAxisRotation(Id an, vector pos, vector vec);
int MSD_execStructCrtAxsRotation(EulerOp *op);
void MSD_printStructCrtAxsRotation(FILE *f, EulerOp *op);
int MSD_readStructCrtAxsRotation(FILE *f, EulerOp *op);
int MSD_modifyStructCrtAxsRotation(EulerOp *op);

/* file hieaxidf.c */
void MSD_execManipulateAxis(void);

/* file hieaxsli.c */
void MSD_execNameListAxisName(void);
int MSD_highNameListAxisName(char *name);
int MSD_lowListAxisName(APTYPE aptr);
void MSD_lowListAllAxis(void);
void MSD_lowListAllAxisName(void);

/* file hieaxsmn.c */
int MSD_getMaxAxisId(void);
int MSD_getNumberOfAxis(void);
char **MSD_putAxisNameInList(char **list);
APTYPE MSD_lowFindAxisByName(char *nam);
APTYPE MSD_getAxis(Id an);

/* file hieaxsmv.c */
void MSD_execNameMoveAxis(void);
int MSD_highNameMoveAxis(char *name, real x, real y, real z);
void MSD_execMoveAxis(void);
int MSD_highMoveAxis(int an, real x, real y, real z);
int MSD_lowMoveAxis(APTYPE aptr, vector vec);
int MSD_execStructMoveAxis(EulerOp *op);
void MSD_printStructMoveAxis(FILE *f, EulerOp *op);
int MSD_readStructMoveAxis(FILE *f, EulerOp *op);
int MSD_modifyStructMoveAxis(EulerOp *op);

/* file hieaxsrm.c */
void MSD_execNameRemoveAxis(void);
int MSD_highNameRemoveAxis(char *name);
void MSD_execRemoveAxis(void);
int MSD_highRemoveAxis(int an);
int MSD_lowRemoveAxis(APTYPE aptr);
int MSD_execStructRemoveAxis(EulerOp *op);
void MSD_printStructRemoveAxis(FILE *f, EulerOp *op);
int MSD_readStructRemoveAxis(FILE *f, EulerOp *op);
int MSD_modifyStructRemoveAxis(EulerOp *op);

/* file hieaxsrn.c */
void MSD_execNameRenameAxis(void);
int MSD_highNameRenameAxis(char *name1, char *name2);
int MSD_lowNameRenameAxis(APTYPE aptr, char *name2);

/* file hieaxsrt.c */
void MSD_execNameRotateAxis(void);
int MSD_highNameRotateAxis(char *name, real cx, real cy, real cz,
                           real ax, real ay, real az, real th);
void MSD_execRotateAxis(void);
int MSD_highRotateAxis(int an, real cx, real cy, real cz,
                       real ax, real ay, real az, real th);
int MSD_lowRotateAxis(APTYPE aptr, vector center, vector axis, real th);
int MSD_execStructRotateAxis(EulerOp *op);
void MSD_printStructRotateAxis(FILE *f, EulerOp *op);
int MSD_readStructRotateAxis(FILE *f, EulerOp *op);
int MSD_modifyStructRotateAxis(EulerOp *op);

/* file hieaxstr.c */
void MSD_execNameTransformUsingAxis(void);
int MSD_highNameTransformUsingAxis(char *name, real desl);
void MSD_execTransformUsingAxis(void);
int MSD_highTransformUsingAxis(int an, real desl);
int MSD_lowTransformUsingAxis(APTYPE aptr, real desl);
void MSD_lowTranslateUsingAxis(APTYPE aptr, vector disp);
void MSD_lowRotateUsingAxis(APTYPE aptr, vector center, vector vec, real th);

/* file hiedesag.c */
void MSD_execNameDesassociaGroupAxis(void);
int MSD_highNameDesassociaGroupAxis(char *name1, char *name2);
void MSD_execDesassociaGroupAxis(void);
int MSD_highDesassociaGroupAxis(int gn, int an);

/* file hiedesas.c */
void MSD_execNameDesassociaSolidAxis(void);
int MSD_highNameDesassociaSolidAxis(char *name1, char *name2);
void MSD_execDesassociaSolidAxis(void);
int MSD_highDesassociaSolidAxis(int sn, int an);

/* file hiedesgg.c */
void MSD_execNameDesassociaGroupGroup(void);
int MSD_highNameDesassociaGroupGroup(char *name1, char *name2);
void MSD_execDesassociaGroupGroup(void);
int MSD_highDesassociaGroupGroup(int gn1, int gn2);

/* file hiedesgs.c */
int MSD_highNameDesassociaGroupSolid(char *name1, char *name2);
void MSD_execDesassociaGroupSolid(void);
int MSD_highDesassociaGroupSolid(int gn, int sn);
void MSD_execNameDesassociaGroupSolid(void);

/* file hiegrpcr.c */
void MSD_execNameCreateGroup(void);
int MSD_highNameCreateGroup(char *name);
void MSD_execCreateGroup(void);
int MSD_highCreateGroup(void);
int MSD_middleCreateGroup(int gn);
int MSD_lowCreateGroup(Id gn);
int MSD_execStructCreateGroup(EulerOp *op);
void MSD_printStructCreateGroup(FILE *f, EulerOp *op);
int MSD_readStructCreateGroup(FILE *f, EulerOp *op);
int MSD_modifyStructCreateGroup(EulerOp *op);

/* file hiegrpcs.c */
void MSD_execNameCreateSetGroup(void);
int MSD_highNameCreateSetGroup(char *name1, char *name2);
void MSD_execCreateSetGroup(void);
int MSD_highCreateSetGroup(int flag);

/* file hiegrpcy.c */
void MSD_execNameCopyGroup(void);
int MSD_highNameCopyGroup(char *name1, char *name2);
void MSD_execCopyGroup(void);
int MSD_highCopyGroup(int gn);
int MSD_lowCopyGroup(GPTYPE gptr, Id gn);

/* file hiegrpdf.c */
void MSD_execManipulateGroup(void);

/* file hiegrpli.c */
void MSD_execNameListGroupName(void);
int MSD_highNameListGroupName(char *name);
int MSD_lowNameListGroupName(GPTYPE gptr);
void MSD_lowListAllGroup(void);
void MSD_lowListGroup(GPTYPE gptr);
void MSD_lowListAllGroupName(void);

/* file hiegrpmn.c */
int MSD_getNumberOfGroups(void);
char **MSD_putGroupNameInList(char **list);
GPTYPE MSD_lowFindGroupByName(char *nam);
GPTYPE MSD_getGroup(Id gn);
char MSD_lowIsEmptyGroup(GPTYPE grpptr);
char MSD_lowIsAncestorGroup(GPTYPE ancgptr, GPTYPE desgptr);
char MSD_lowIsAncestorAxis(APTYPE ancaptr, APTYPE desaptr);
GPTYPE MSD_getGroupAxis(APTYPE axis);

/* file hiegrpmv.c */
void MSD_execNameMoveGroup(void);
int MSD_highNameMoveGroup(char *name, real x, real y, real z);
void MSD_execMoveGroup(void);
int MSD_highMoveGroup(int gn, real x, real y, real z);
int MSD_lowMoveGroup(GPTYPE gptr, vector disp);

/* file hiegrprg.c */
void MSD_execNameRemoveOneGroup(void);
int MSD_highNameRemoveOneGroup(char *name);
void MSD_execRemoveOneGroup(void);
int MSD_highRemoveOneGroup(int gn);
int MSD_lowRemoveOneGroup(GPTYPE gptr);
int MSD_execStructRemoveOneGroup(EulerOp *op);
void MSD_printStructRemoveOneGroup(FILE *f, EulerOp *op);
int MSD_readStructRemoveOneGroup(FILE *f, EulerOp *op);
int MSD_modifyStructRemoveOneGroup(EulerOp *op);

/* file hiegrprm.c */
void MSD_execNameRemoveGroup(void);
int MSD_highNameRemoveGroup(char *name);
void MSD_execRemoveGroup(void);
int MSD_highRemoveGroup(int gn);
int MSD_lowRemoveGroup(GPTYPE gptr);

/* file hiegrprn.c */
void MSD_execNameRenameGroup(void);
int MSD_highNameRenameGroup(char *name1, char *name2);
int MSD_lowNameRenameGroup(GPTYPE gptr, char *name2);

/* file hiegrprt.c */
void MSD_execNameRotateGroup(void);
int MSD_highNameRotateGroup(char *name, real x1, real y1, real z1, \
                            real x2, real y2, real z2, real th);
void MSD_execRotateGroup(void);
int MSD_highRotateGroup(Id gn, real x1, real y1, real z1, \
                        real x2, real y2, real z2, real th);
int MSD_lowRotateGroup(GPTYPE gptr, vector center, vector vec, real th);

/* file hiegrpsc.c */
void MSD_execNameSetGroupCurrent(void);
void MSD_highNameSetGroupCurrent(char *nam);
void MSD_execSetGroupCurrent(void);
int MSD_highSetGroupCurrent(int gn);
int MSD_lowSetGroupCurrent(GPTYPE gptr);
int MSD_execStructSetGroupCurrent(EulerOp *op);
void MSD_printStructSetGroupCurrent(FILE *f, EulerOp *op);
int MSD_readStructSetGroupCurrent(FILE *f, EulerOp *op);
int MSD_modifyStructSetGroupCurrent(EulerOp *op);

/* file hiegrptf.c */
void MSD_execNameTransferGroup(void);
int MSD_highNameTransferGroup(char *name1, char *name2);
void MSD_execTransferGroup(void);
int MSD_highTransferGroup(int gn1, int gn2);

/* file hiemove_.c */
void MSD_readExecuteMoveElement(void);
void MSD_executeMoveElement(char *name, real dx, real dy, real dz);
int _move(char *name, vector disp, int moni);

/* file hierotat.c */
void MSD_readExecuteRotateElement(void);
void MSD_executeRotateElement(char *name, real x1, real y1, real z1, \
                              real x2, real y2, real z2, real th);
int _rotate(char *name, vector center, vector vector, real th, int moni);
void MSD_readExecuteAffineElement(void);
char _affine(char *name, vector center, matrix mat, int moni);
void objrot(STYPE *optr, vector center, vector axis, real th);
void MSD_executeAffineElement(char *name, real a11, real a12, real a13, \
                              real a21, real a22, real a23,             \
                              real a31, real a32, real a33,             \
                              real xc, real yc, real zc);

#endif
