/* 
  PROJETO USPDesigner
  MODULO: BAS (Criacao de Formas Especificas)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: basrov__.h
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

#ifndef __BasRov___h
#define __BasRov___h

#ifndef EXTERN
 #ifdef _DVS_
  #define EXTERN
 #else
  #define EXTERN    extern
 #endif
#endif

#define SOLIDO_TIPO_BARRA       0x01
#define SOLIDO_TIPO_SOLID       0x02
#define SOLIDO_TIPO_PLACA       0x03

#define BARRA_GEOM_CIRCULO      0x01
#define BARRA_GEOM_QUADRADO     0x02
#define BARRA_GEOM_TRIANGULO    0x03
#define BARRA_GEOM_RETANGULO    0x04
#define BARRA_GEOM_ELIPSE       0x05
#define BARRA_GEOM_PERFIL_L     0x06
#define BARRA_GEOM_PERFIL_T     0x07
#define BARRA_GEOM_PERFIL_I     0x08

#define SOLIDO_GEOM_ESFERA      0x20
#define SOLIDO_GEOM_CUBO        0x21
#define SOLIDO_GEOM_PRISMA      0x22
#define SOLIDO_GEOM_PIRAMIDE    0x23
#define SOLIDO_GEOM_CONE        0x24

#define FRONTEIRA_A             0x01
#define FRONTEIRA_CV            0x02

typedef struct ListaOrdenada   LOPType;

struct ListaOrdenada
{
   SPTYPE optr;                              /* ponteiro para o solido   */
   vector cg;                                /* centro de gravidade      */
   real   area;                              /* area projetada           */
   real   H_D;                               /* maior dim / dim princ.   */
   real   D;                                 /* dimensao principal       */
   real   param;                             /* valor a ser ordenado     */
};

EXTERN vector  DirecaoEscoamento;             /* Direcao de Escoamento    */
EXTERN LOPType *ListaEscoamento;              /* lista de elementos       */
EXTERN int     NumListaEscoamento;            /* numero de elementos      */

void MSD_execNameMakeCirculo(void);
void MSD_execNameMakeCubo(void);
void MSD_execNameMakePlaca(void);
void MSD_execNameMakePiramide(void);
void MSD_execNameMakeEsfera(void);
void MSD_execNameMakePerfilI(void);
void MSD_execNameMakePerfilL(void);
void MSD_execNameMakePerfilT(void);
void MSD_execNameMakePrisma(void);
void MSD_execNameMakeQuadrado(void);
void MSD_execNameMakeRetangulo(void);
void MSD_execNameMakeTriangulo(void);
void MSD_execNameDimensaoPrincipal(void);
real MSD_determinaDimensaoPrincipal(SPTYPE s, vector dir, real dim);
real MSD_determinaDistanciaParametrica(vector cc, vector dir, vector pp);
real MSD_determinaMaiorDimensao(SPTYPE s);
void MSD_execNameRotaciona(void);
void MSD_execNameDesloca(void);
void MSD_execOrdena(void);
void MSD_execProjetadaTotal(void);

#endif
