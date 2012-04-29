/* 
  PROJETO USPDesigner
  MODULO: VEC (Operacao com vetores)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: vectorop.h
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

#ifndef __vectorop_h
#define __vectorop_h

#ifndef __double_real
 #define __double_real
typedef double   real;                    /* floating point number */
#endif
#ifndef __double_vector
 #define __double_vector
typedef real     vector[4];             /* vector of xyz-space   */
#endif
#ifndef __double_matrix
 #define __double_matrix
typedef real     matrix[4][4];          /* transformation matrix */
#endif

void invmat(matrix mat, matrix inv, int n);
real cofactor(matrix mat, int dim, int i, int j);
real determ(matrix mat, int dim);
void matmult(matrix m, matrix m1, matrix m2);
void matminus(matrix m, matrix m1, matrix m2);
void matplus(matrix m, matrix m1, matrix m2);
void invrefine(matrix mat, matrix inv);
void matident(matrix m);
int testident(matrix m, real eps);
void mattrans(matrix m, real tx, real ty, real tz);
void matrotat(matrix mat, real rx, real ry, real rz);
void matscale(matrix m, real sx, real sy, real sz);
void matcopy(matrix m1, matrix m2);
void vecmult(vector v1, vector v2, matrix m);
void matprint(matrix m);
void mattranspose(matrix m, matrix mtransp);
int comp(real f1, real f2, real tol);
real *makevec(real *v, real x, real y, real z, real w);
real dot(real *v1, real *v2);
real dot2(real *v1, real *v2, int drop);
real vecd(real *v1, real *v2);
int vecnull(real *vec, real eps);
int vecnull2(real *vec, real eps, int drop);
int vecequal(real *v1, real *v2);
int veccolin(real *v1, real *v2, real eps);
real *cross(real *v1, real *v2, real *v3);
char normalize(real *v);
real *veccopy(real *v1, real *v2);
real *vecminus(real *res, real *v1, real *v2);
real *vecplus(real *res, real *v1, real *v2);
void vecesc(real *v, real *ve, real k);
int getdrop(real *vec);
void vecprint(vector v);
real *veczer(real *v);
real vecdet(real *v1, real *v2, real *v3);
real *vecpe(real *vs, real *ve, real *v, real k);
real *calc_p(real *v1, real *v2, real t, real *v);
real *vec2cp(real *v, real *o);
real vecd2(real *v1, real *v2);
void rotmat(vector axis, real th, matrix rot);
void matzer(matrix mat);

#endif
