/* 
  PROJETO USPDesigner
  MODULO: ANI (Animacao) 
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: animatio.h
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

#ifndef __Animatio_h
#define __Animatio_h

#ifndef EXTERN
 #ifdef _DVS_
  #define EXTERN
 #else
  #define EXTERN    extern
 #endif
#endif

#include <stdio.h>

#ifndef __double_vector
 #define __double_vector
typedef real              vector[4];
#endif
#ifndef __double_vect2
 #define __double_vect2
typedef real              vect2[3];
#endif
#ifndef __struct_cameratp
 #define __struct_cameratp
typedef struct cameratp   CTYPE;
#endif
#ifndef __struct_axistp
typedef struct axistp     ATYPE;
#endif

typedef enum movimento    Mov;

/* Parameter Definition */

#define MAXTIME              0.1
#define MAXTIME2             1
#define MAXVECTORS           600
#define MAXNUMFRAMES         150
#define FILTERPRECISION      0.003

typedef struct movcam      MOVCAM;
typedef struct movaxs      MOVAXS;
typedef union  movunion    MOVUNION;
typedef struct Move        MOVE;
typedef struct IntVector   INTVECTOR;
typedef struct FloVector   FLOVECTOR;
typedef struct FrameList   FRAMEPT;

struct IntVector
{
   int tlinha;
   int cor;
   int px1;
   int px2;
   int py1;
   int py2;
};

struct FloVector
{
   int  tlinha;
   int  cor;
   real px1;
   real px2;
   real py1;
   real py2;
};

struct movcam
{
   CTYPE  *cam;
   vector eye;
   vector ref;
   vector auxeye;
   vector auxref;
   real   scale;
   real   auxscale;
};

struct movaxs
{
   APTYPE axs;
   real   desl;
   real   auxdesl;
};

union movunion
{
   MOVCAM *mc;
   MOVAXS *ma;
};

enum movimento
{
   MovimentoAxis,
   MovimentoCamera,
};

struct Move
{
   Mov      tipo;
   MOVUNION mu;
   MOVE     *next;
};

struct FrameList
{
   int       nvec;
   INTVECTOR *vec;
};

EXTERN MOVE *movhead;
EXTERN int  framenum;
EXTERN int  NumeroFrames;

/* file anicreat.c */
void animatio(void);
void MSD_selectAllObjects(void);
void MSD_selectElement(char *name);
void MSD_selectGroup(char *name);
void MSD_selectObject(char *name);
void MSD_resetAllObjects(void);
void MSD_resetObject(char *name);
void MSD_resetGroup(char *name);
void MSD_resetElement(char *name);
void MSD_addAxisToMovementStructure(char *eaname, real edesl);
void MSD_addCameraToMovementStructure(char *aname, real ex, real ey, real ez,
                                      real rx, real ry, real rz, real desl);
void AddCamAni(char *aname, vector eeye, vector eref, real edesl);
FILE *OpenIndexAni(char *arqnam);
void CriaFrame(FILE *arqaux, char *arqnam, int framect);
void MSD_createAnimation(int opcao, int nframes, char *AnimeName, char *ecname);
char *InitFrameCount(char *nome, char *aux, int framect);

#ifdef __UNIX_
void itoaa(char *a, int b);
void reverse(char *c);

#endif
void PurificaAni(char *arqnam, int opcao);
void PurificaFrame(int num, char *arqname, char *filename, int opcao);
int compara(real x, real y, real eps);

/* file aniexibe.c */
void cinema(void);
void MSD_loadAnimation(char *AnimeName, int frini, int frfim, int frpasso);
void DrawLine(INTVECTOR *auxpt);
void ExibeFrame(int p);
void MSD_displayLoadedAnimation(int num);
void LoadFrame(int count, int num, char *filename);
int FindFrameNum(char *nome);
void MSD_freeCinemaStructure(void);

/* file animlist.c */
MOVE *CreateElementAni(Mov mov);
void AddElementAni(MOVE *e);
void RemElementAni(MOVE *p);
void MSD_clearAnimationList(void);
void PrintListAni(void);
void InitListAni(int nframes);

#endif
