/* 
  PROJETO USPDesigner
  MODULO: GEN (Funcoes Gerais de Nivel Superior)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: genfunc_.h
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

#ifndef __GenFunc__h
#define __GenFunc__h

#include <stdio.h>

#ifndef __double_real
 #define __double_real
typedef double            real;
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

/* file genaxsas.c */
void MSD_execNameAssociateAxisElement(void);
int MSD_highNameAssociateAxisElement(char *name1, char *name2);

/* file genaxsds.c */
void MSD_execNameDesassociateAxisElement(void);
int MSD_highNameDesassociateAxisElement(char *name);

/* file gencopy_.c */
void MSD_execNameCopyElement(void);
int MSD_highNameCopyElement(char *name1, char *name2);
void MSD_execNameCopySolid(void);
int MSD_highNameCopySolid(char *name1, char *name2);
void MSD_execCopySolid(void);
int MSD_highCopySolid(int sn);
int MSD_lowCopySolid(SPTYPE s, Id sn);

/* file gendelet.c */
void MSD_execNameDeleteElement(void);
int MSD_highNameDeleteElement(char *name);

/* file gendir__.c */
#ifndef __UNIX_
void MSD_execDir(void);
int MSD_lowDir(char *name);
void MSD_lowDisplayFiles(char *nam);

#endif

/* file genfadjr.c */
void MSD_execNameListFaceAdjacency(void);
int MSD_highNameListFaceAdjacency(char *name, char *fname);
int MSD_lowListFaceAdjacency(SPTYPE optr, char *fname);

/* file genfindn.c */
char MSD_getTypeByName(char *nam);
int MSD_getNumberOfSolid(void);
char **MSD_putSolidNameInList(char **list);

/* file gengrpas.c */
void MSD_execNameAssociateGroupElement(void);
int MSD_highNameAssociateGroupElement(char *name1, char *name2);

/* file gengrpds.c */
void MSD_execNameDesassociateGroupElement(void);
int MSD_highNameDesassociateGroupElement(char *name);

/* file genlists.c */
void MSD_execListElement(void);
void MSD_lowListAllSolidName(void);
void MSD_lowListNumberOfElements(void);

/* file genload_.c */
void MSD_execNameLoadSolid(void);
int MSD_highNameLoadSolid(char *oname, char *fname);
void MSD_execLoadSolid(void);
int MSD_lowLoadSolid(Id sn, char *fname);
int MSD_highLoadSolid(char *fname);

/* file genncopy.c */
void MSD_execNameNCopySolid(void);
int MSD_highNameNCopySolidTranslate(char *name, real Xi, real Yi, real Zi, real Xd, \
                                    real Yd, real Zd, int n);
int MSD_lowNameNCopySolidTranslate(char *name, vector omove, vector dmove, int n);
int MSD_highNameNCopySolidSpiral(char *name, real Xi, real Yi, real Zi, real Xd,
                                 real Yd, real Zd, real dth, real dmv, int n);
int MSD_lowNameNCopySolidSpiral(char *name, vector cpos, vector vect, real dth, real dmv, int n);
void MSD_lowRequestNewName(char *name);

/* file genrenam.c */
void MSD_execNameRenameElement(void);
int MSD_highNameRenameElement(char *name1, char *name2);
void MSD_execNameRenameSolid(void);
int MSD_highNameRenameSolid(char *name1, char *name2);
int MSD_lowNameRenameSolid(SPTYPE sptr, char *name2);

/* file genrotat.c */
void MSD_execNameRotateElement(void);
int MSD_highNameRotateElement(char *name, real x1, real y1, real z1,
                              real x2, real y2, real z2, real th);

/* file gensave_.c */
void MSD_execNameSaveSolid(void);
int MSD_highNameSaveSolid(char *oname, char *fname);
void MSD_execSaveSolid(void);
int MSD_highSaveSolid(int sn, char *fname);
int MSD_lowSaveSolid(SPTYPE s, char *fname);

/* file genshow_.c */
void MSD_execShow(void);
EPTYPE MSD_lowShowFindEdge(SPTYPE s, Id en);
VPTYPE MSD_lowShowFindVertex(SPTYPE s, Id vn);
void MSD_lowListAllSolid(void);
void MSD_lowListAllVertex(SPTYPE optr);
void MSD_lowListAllEdge(SPTYPE optr);
void MSD_lowListAllFace(SPTYPE optr);
void MSD_lowShowListSolid(SPTYPE optr);
void MSD_lowShowListVertex(VPTYPE vptr);
void MSD_lowShowListEdge(EPTYPE eptr);
void MSD_lowShowListFace(FPTYPE fptr);

/* file gentrans.c */
void MSD_execNameTranslateElement(void);
int MSD_highNameTranslateElement(char *name, real tx, real ty, real tz);

/* file genwait_.c */
void MSD_execWait(void);
void MSD_highWait(int waitsec);
void MSD_execTime(void);

#endif
