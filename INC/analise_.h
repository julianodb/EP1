/* 
  PROJETO USPDesigner
  MODULO: ANA (Modulo de Analise)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: analise_.h
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

#ifndef __Analise__h
#define __Analise__h

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

/* file anaarea_.c */
void MSD_execArea(void);
void MSD_execNameArea(void);
real MSD_highArea(Id sn);
real MSD_highNameArea(char *name);
real MSD_lowLoopArea(LPTYPE l);
real MSD_lowFaceArea(FPTYPE f);
real MSD_lowSolidArea(SPTYPE s);

/* file anaaproj.c */
void MSD_execNameAreaProjetada(void);
int MSD_highNameAreaProjetada(char *name, real dx, real dy, real dz, real *area, real *dim);
int MSD_highAreaProjetada(Id sn, real dx, real dy, real dz, real *area, real *dim);
int MSD_lowAreaProjetada(SPTYPE s, real dx, real dy, real dz, real *area, real *dim, int flag);
void MSD_lowMakeUnion(FPTYPE f, vector dir);
void MSD_lowMoveFace(FPTYPE f, vector dir);
SPTYPE MSD_CriaListaDePoligonosCoplanares(SPTYPE s, vector dir);
real MSD_Calcula_Area_da_Uniao(SPTYPE s);
void MSD_lowRebatePontoNoPlano(vector p2, vector pp, vector dir);
real MSD_lowModuloReal(real num);
int MSD_lowMaximaDirecao(vector p);
real MSD_lowSolidDimensaoPrincipal(SPTYPE s, vector normal);

/* file anacaren.c */
void MSD_execNameCentroCarena(void);
int MSD_highNameCentroCarena(char *name, vector vec);
int MSD_highCentroCarena(Id sn, vector vec);
int MSD_lowCentroCarena(SPTYPE s, vector ctotal);

/* file anacatot.c */
void MSD_execCentroCarenaTotal(void);
void MSD_lowCentroCarenaTotal(vector ctotal);

/* file anacentg.c */
void MSD_execNameCentroDeGravidade(void);
int MSD_highNameCentroDeGravidade(char *name, vector centro);
int MSD_highCentroDeGravidade(Id sn, vector vec);
int MSD_lowCentroDeGravidade(SPTYPE s, vector centro);

/* file anacgtot.c */
void MSD_execCentroMassaTotal(void);
void MSD_lowCentroMassaTotal(vector ctotal);

/* file anadensa.c */
void MSD_execNameDensidadeAbsoluta(void);
int MSD_highNameDensidadeAbsoluta(char *name, real densrel);
int MSD_highDensidadeAbsoluta(Id sn, real densrel);
int MSD_lowDensidadeAbsoluta(SPTYPE s, real densrel);

/* file anadensi.c */
void MSD_execNameDensidadeRelativa(void);
int MSD_highNameDensidadeRelativa(char *name, real densrel);
int MSD_highDensidadeRelativa(Id sn, real densrel);
int MSD_lowDensidadeRelativa(SPTYPE s, real densrel);

/* file anadflui.c */
void MSD_execDensidadeFluido(void);
void MSD_lowDensidadeFluido(real dens);

/* file anaemtot.c */
void MSD_execEmpuxoTotal(void);
real MSD_lowEmpuxoTotal(void);

/* file anaintot.c */
void MSD_execMatrizInerciaTotal(void);
void MSD_lowMatrizInerciaTotal(matrix mat);

/* file anamassa.c */
void MSD_execNameMassa(void);
real MSD_highNameMassa(char *name);
real MSD_highMassa(Id sn);
real MSD_lowMassa(SPTYPE s);

/* file anaminrc.c */
void MSD_execNameMatrizInercia(void);
int MSD_highNameMatrizInercia(char *name, matrix mat);
int MSD_highMatrizInercia(Id sn, matrix mat);
int MSD_lowMatrizInercia(SPTYPE s, matrix mat, vector cg);

/* file anamtot_.c */
void MSD_execMassaTotal(void);
real MSD_lowMassaTotal(void);

/* file anapesot.c */
void MSD_execPesoTotal(void);
real MSD_lowPesoTotal(void);

/* file anashow_.c */
void MSD_execAnaliseShow(void);
void MSD_lowAnaliseShow(char *name);

/* file anavftot.c */
void MSD_execVolumeFlutuabilidadeTotal(void);
real MSD_lowVolumeFlutuabilidadeTotal(void);

/* file anavolum.c */
void MSD_execNameVolume(void);
void MSD_execVolume(void);
real MSD_highNameVolume(char *);
real MSD_highVolume(Id sn);
real MSD_lowVolume(SPTYPE s);

/* file anavtot_.c */
void MSD_execVolumeTotal(void);
real MSD_lowVolumeTotal(void);

#endif
