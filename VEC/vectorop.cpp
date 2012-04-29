/* 
  PROJETO USPDesigner
  MODULO: VEC (Operacao com vetores)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: vectorop.cpp
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
#include <stdio.h>
#include <math.h>
#include "memvirtu.h"
#include "lowparam.h"
#include "lowsolid.h"
#include "vectorop.h"

/* Invert an nxn (n < 5) square matrix using the adjoint matrix method.      */
/*  Dimension limited to at most 4 because of data structure matrix.         */
static matrix I =
{
   { 1.0, 0.0, 0.0, 0.0 },
   { 0.0, 1.0, 0.0, 0.0 },
   { 0.0, 0.0, 1.0, 0.0 },
   { 0.0, 0.0, 0.0, 1.0 },
};

void invmat(matrix mat, matrix inv, int n)
{
   real detA;
   int  i, j;

   detA = determ(mat, n);
   for (i = 0; i < n; i++)
   {
      for (j = 0; j < n; j++)
      {
         inv[j][i] = cofactor(mat, n, i, j) / detA;
      }
   }
}

/* Calculate cofactor of element ij in matrix mat. */
real cofactor(matrix mat, int dim, int i, int j)
{
   real   tmp;
   matrix minor;
   int    m, n, k, l;

   for (m = k = 0; k < dim - 1; k++)
   {
      if (m == i)
      {
         m++;
      }
      for (n = l = 0; l < dim - 1; l++)
      {
         if (n == j)
         {
            n++;
         }
         minor[k][l] = mat[m][n];
         n++;
      }
      m++;
   }

   tmp = determ(minor, dim - 1);
   return((i + j) % 2 == 0 ? tmp : -tmp);
}

/* Calculate determinant of matrix. */
real determ(matrix mat, int dim)
{
   real sum;
   int  i;

   if (dim == 1)
   {
      return(mat[0][0]);
   }
   for (sum = 0.0, i = 0; i < dim; i++)
   {
      sum += mat[0][i] * cofactor(mat, dim, 0, i);
   }
   return(sum);
}

/*** Calculate the matrix product m = m1 x m2. The procedure     ***/
/***   should be implemented so that the argument matrix for the ***/
/***   result m can be same as m1.                               ***/
void matmult(matrix m, matrix m1, matrix m2)
{
   vecmult(m[0], m1[0], m2);
   vecmult(m[1], m1[1], m2);
   vecmult(m[2], m1[2], m2);
   vecmult(m[3], m1[3], m2);
}

/*** Calculate the matrix difference m = m1 - m2. The procedure  ***/
/***   should be implemented so that the argument matrix for the ***/
/***   result m can be same as m1 or m2.                         ***/
void matminus(matrix m, matrix m1, matrix m2)
{
   int i, j;

   for (i = 0; i < 4; i++)
   {
      for (j = 0; j < 4; j++)
      {
         m[i][j] = m1[i][j] - m2[i][j];
      }
   }
}

/*** Calculate the matrix sum m = m1 + m2. The procedure         ***/
/***   should be implemented so that the argument matrix for the ***/
/***   result m can be same as m1 or m2.                         ***/
void matplus(matrix m, matrix m1, matrix m2)
{
   int i, j;

   for (i = 0; i < 4; i++)
   {
      for (j = 0; j < 4; j++)
      {
         m[i][j] = m1[i][j] + m2[i][j];
      }
   }
}

/* Refine an inversion matrix according to the method described in Faddejew  */
/*  -Faddejewa: Numerische methoden fuer Lineare Algebra. Iterate until      */
/*  required accuracy is achieved.                                           */
void invrefine(matrix mat, matrix inv)
{
   matrix R, M;

   matmult(M, mat, inv);
   matminus(R, I, M);
   matmult(M, inv, R);
   matplus(inv, M, inv);
}

/*** Make m an identity matrix                                   ***/
void matident(matrix m)
{
   m[0][0] = 1.0;
   m[1][0] = m[2][0] = m[3][0] = 0.0;
   m[1][1] = 1.0;
   m[0][1] = m[2][1] = m[3][1] = 0.0;
   m[2][2] = 1.0;
   m[0][2] = m[1][2] = m[3][2] = 0.0;
   m[3][3] = 1.0;
   m[0][3] = m[1][3] = m[2][3] = 0.0;
}

/*** Make m an identity matrix                                   ***/
void matzer(matrix m)
{
   m[0][0] = m[1][0] = m[2][0] = m[3][0] = 0.0;
   m[1][1] = m[0][1] = m[2][1] = m[3][1] = 0.0;
   m[2][2] = m[0][2] = m[1][2] = m[3][2] = 0.0;
   m[3][3] = m[0][3] = m[1][3] = m[2][3] = 0.0;
}

/* Test identity of matrix */
int testident(matrix m, real eps)
{
   int i, j;

   for (i = 0; i < 4; i++)
   {
      for (j = 0; j < 4; j++)
      {
         if (comp(m[i][j], (i == j) ? 1.0 : 0.0, eps) != 0)
         {
            return(FALSE);
         }
      }
   }
   return(SUCCESS);
}

/*** Combine a translation (tx, ty, tz) with m by constructing a ***/
/***   translation matrix t from tx, ty and tz and multiplying m ***/
/***   with t on the right.                                      ***/
void mattrans(matrix m, real tx, real ty, real tz)
{
   real b;

   b        = m[0][3];
   m[0][0] += b * tx;
   m[0][1] += b * ty;
   m[0][2] += b * tz;
   b        = m[1][3];
   m[1][0] += b * tx;
   m[1][1] += b * ty;
   m[1][2] += b * tz;
   b        = m[2][3];
   m[2][0] += b * tx;
   m[2][1] += b * ty;
   m[2][2] += b * tz;
   b        = m[3][3];
   m[3][0] += b * tx;
   m[3][1] += b * ty;
   m[3][2] += b * tz;
}

/*** Combine a rotation (rx, ry, rz) with m by  constructing  a  ***/
/***   rotation matrix r from tx, ty and tz and multiplying m    ***/
/***   with r on the right. If several rotations are specified,  ***/
/***   they are applied in the order rx, ry, rz.                 ***/
void matrotat(matrix mat, real rx, real ry, real rz)
{
   real c, s, m, n;

   rx = rx * PI / 180.0;
   ry = ry * PI / 180.0;
   rz = rz * PI / 180.0;
   if (comp(rx, 0.0, EPS) != 0)
   {
      c         = cos(rx);
      s         = sin(rx);
      m         = mat[0][1];
      n         = mat[0][2];
      mat[0][1] = m * c - n * s;
      mat[0][2] = n * c + m * s;
      m         = mat[1][1];
      n         = mat[1][2];
      mat[1][1] = m * c - n * s;
      mat[1][2] = n * c + m * s;
      m         = mat[2][1];
      n         = mat[2][2];
      mat[2][1] = m * c - n * s;
      mat[2][2] = n * c + m * s;
      m         = mat[3][1];
      n         = mat[3][2];
      mat[3][1] = m * c - n * s;
      mat[3][2] = n * c + m * s;
   }
   if (comp(ry, 0.0, EPS) != 0)
   {
      c         = cos(ry);
      s         = sin(ry);
      m         = mat[0][0];
      n         = mat[0][2];
      mat[0][0] = m * c + n * s;
      mat[0][2] = n * c - m * s;
      m         = mat[1][0];
      n         = mat[1][2];
      mat[1][0] = m * c + n * s;
      mat[1][2] = n * c - m * s;
      m         = mat[2][0];
      n         = mat[2][2];
      mat[2][0] = m * c + n * s;
      mat[2][2] = n * c - m * s;
      m         = mat[3][0];
      n         = mat[3][2];
      mat[3][0] = m * c + n * s;
      mat[3][2] = n * c - m * s;
   }
   if (comp(rz, 0.0, EPS) != 0)
   {
      c         = cos(rz);
      s         = sin(rz);
      m         = mat[0][0];
      n         = mat[0][1];
      mat[0][0] = m * c - n * s;
      mat[0][1] = n * c + m * s;
      m         = mat[1][0];
      n         = mat[1][1];
      mat[1][0] = m * c - n * s;
      mat[1][1] = n * c + m * s;
      m         = mat[2][0];
      n         = mat[2][1];
      mat[2][0] = m * c - n * s;
      mat[2][1] = n * c + m * s;
      m         = mat[3][0];
      n         = mat[3][1];
      mat[3][0] = m * c - n * s;
      mat[3][1] = n * c + m * s;
   }
}

/*** Combine a scaling (sx, sy, sz) with m by constructing a     ***/
/***   scaling matrix t from sx, sy and sz and multiplying m     ***/
/***   with t on the right.                                      ***/
/* Apply a scaling to matrix m */
void matscale(matrix m, real sx, real sy, real sz)
{
   m[0][0] *= sx;
   m[0][1] *= sy;
   m[0][2] *= sz;
   m[1][0] *= sx;
   m[1][1] *= sy;
   m[1][2] *= sz;
   m[2][0] *= sx;
   m[2][1] *= sy;
   m[2][2] *= sz;
   m[3][0] *= sx;
   m[3][1] *= sy;
   m[3][2] *= sz;
}

/* Copy matrix m2 to matrix m1 */
void matcopy(matrix m1, matrix m2)
{
   int i, j;

   for (i = 0; i < 4; i++)
   {
      for (j = 0; j < 4; j++)
      {
         m1[i][j] = m2[i][j];
      }
   }
}

/* Matrix multiplication res = m1 x m2 */
/*** Calculate the vector-matrix product v1 = v2 x m. The        ***/
/***   procedure should be implemented so that v1 can equal v2.  ***/
void vecmult(vector v1, vector v2, matrix m)
{
   vector v;

   v[0] = v2[0] * m[0][0] + v2[1] * m[1][0] + v2[2] * m[2][0] + v2[3] * m[3][0];
   v[1] = v2[0] * m[0][1] + v2[1] * m[1][1] + v2[2] * m[2][1] + v2[3] * m[3][1];
   v[2] = v2[0] * m[0][2] + v2[1] * m[1][2] + v2[2] * m[2][2] + v2[3] * m[3][2];
   v[3] = v2[0] * m[0][3] + v2[1] * m[1][3] + v2[2] * m[2][3] + v2[3] * m[3][3];
   veccopy(v1, v);
}

/* Print out a matrix */
void matprint(matrix m)
{
   int i, j;

   for (i = 0; i < 4; i++, printf("\n"))
   {
      for (j = 0; j < 4; j++)
      {
         printf("%f ", m[i][j]);
      }
   }
}

/* Transpose a matrix */
void mattranspose(matrix m, matrix mtransp)
{
   int    i, j;
   matrix tmp;

   for (i = 0; i < 4; ++i)
   {
      for (j = 0; j < 4; ++j)
      {
         tmp[i][j] = m[j][i];
      }
   }
   matcopy(mtransp, tmp);
}

/* Compare floating-point numbers with tolerance */
int comp(real f1, real f2, real tol)
{
   if (fabs(f1 - f2) < tol)
   {
      return(0);
   }
   if (f1 > f2)
   {
      return(1);
   }
   return(-1);
}

/* Make 4-vector out of components */
real *makevec(vector v, real x, real y, real z, real w)
{
   v[0] = x;
   v[1] = y;
   v[2] = z;
   v[3] = w;
   return(v);
}

/*** Calculate the dot (scalar) product of v1 and v2.            ***/
real dot(vector v1, vector v2)
{
   return(v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2]);
}

/* Two-dimensional dot product -- "drop" gives the axis to be ignored */
real dot2(vector v1, vector v2, int drop)
{
   switch (drop)
   {
       case 0:
       return(v1[1] * v2[1] + v1[2] * v2[2]);

       case 1:
       return(v1[0] * v2[0] + v1[2] * v2[2]);

       case 2:
       return(v1[0] * v2[0] + v1[1] * v2[1]);
   }
   return(0.0);
}

/* Calculate Euclidean distance of two points */
real vecd(vector v1, vector v2)
{
   real dx, dy, dz;

   dx = v1[0] - v2[0];
   dy = v1[1] - v2[1];
   dz = v1[2] - v2[2];
   return(sqrt(dx * dx + dy * dy + dz * dz));
}

/* Is vec a null vector ? */
int vecnull(vector v, real tol)
{
   return(comp(dot(v, v), 0.0, tol) == 0);
}

/* Is the projection of vec a null vector ? */
int vecnull2(vector vec, real eps, int drop)
{
   int nulx, nuly, nulz;

   nulx = comp(vec[0], 0.0, eps);
   nuly = comp(vec[1], 0.0, eps);
   nulz = comp(vec[2], 0.0, eps);
   if ((drop == 0) && (nuly == 0) && (nulz == 0))
   {
      return(TRUE);
   }
   if ((drop == 1) && (nulx == 0) && (nulz == 0))
   {
      return(TRUE);
   }
   if ((drop == 2) && (nulx == 0) && (nuly == 0))
   {
      return(TRUE);
   }
   return(FALSE);
}

/*** Check whether vectors v1 and v2 are the same.               ***/
int vecequal(vector v1, vector v2)
{
   real dx, dy, dz, diff;

   dx   = v1[0] - v2[0];
   dy   = v1[1] - v2[1];
   dz   = v1[2] - v2[2];
   diff = dx * dx + dy * dy + dz * dz;
   return(comp(diff, 0.0, EPS * EPS) == 0);
}

/* Are vectors approximatively colinear ? */
int veccolin(vector v1, vector v2, real eps)
{
   vector tmp;

   return(vecnull(cross(tmp, v1, v2), eps));
}

/*** Calculate the cross (vector) product v1 = v2 x v3. The      ***/
/***   procedure should be implemented so that the argument      ***/
/***   vector for the result v1 can be the same as v2 or v3      ***/
real *cross(vector v1, vector v2, vector v3)
{
   vector v;

   v[0] = v2[1] * v3[2] - v2[2] * v3[1];
   v[1] = v2[2] * v3[0] - v2[0] * v3[2];
   v[2] = v2[0] * v3[1] - v2[1] * v3[0];
   v[3] = 1.0;
   return(veccopy(v1, v));
}

/*** Make v a unit vector, return its previous length            ***/
char normalize(vector v)
{
   real vl;

   vl = sqrt(dot(v, v));
   if (comp(vl, 0.0, EPS) != 0)
   {
      v[0] /= vl;
      v[1] /= vl;
      v[2] /= vl;
   }
   return(comp(vl, 0.0, EPS) != 0);
}

/*** Make v1 equal v2.                                           ***/
real *veccopy(vector v1, vector v2)
{
   v1[0] = v2[0];
   v1[1] = v2[1];
   v1[2] = v2[2];
   v1[3] = v2[3];
   return(v1);
}

/*** Let res = v1 - v2.                                          ***/
real *vecminus(vector res, vector v1, vector v2)
{
   int i;

   for (i = 0; i < 3; i++)
   {
      res[i] = v1[i] - v2[i];
   }
   res[3] = 1.0;
   return(res);
}

/*** Sum vectors v1 and v2 into res.                             ***/
real *vecplus(vector res, vector v1, vector v2)
{
   int i;

   for (i = 0; i < 3; i++)
   {
      res[i] = v1[i] + v2[i];
   }
   res[3] = 1.0;
   return(res);
}

/* Multiply vector by a scalar */
void vecesc(vector v, vector ve, real k)
{
   int i;

   for (i = 0; i < 3; i++)
   {
      v[i] = k * ve[i];
   }
   v[3] = 1.0;
}

/* Select largest absolute value of vec (for dropping) */
int getdrop(vector vec)
{
   real max, tmp;
   int  res;

   max = fabs(vec[0]);
   res = 0;
   if ((tmp = fabs(vec[1])) > max)
   {
      max = tmp;
      res = 1;
   }
   if (fabs(vec[2]) > max)
   {
      res = 2;
   }
   return(res);
}

extern FILE *trace;
/* Print out vector */
void vecprint(vector v)
{
   printf("%f %f %f %f\n", v[0], v[1], v[2], v[3]);
}

real *veczer(vector v)
{
   v[0] = v[1] = v[2] = 0.0;
   v[3] = 1.0;
   return(v);
}

real *vecpe(vector vs, vector ve, vector v, real k)
{
   int i;

   for (i = 0; i < 3; i++)
   {
      v[i] = ve[i] + k * vs[i];
   }
   v[3] = 1.0;
   return(v);
}

real *calc_p(vector v1, vector v2, real t, vector v)
{
   int i;

   for (i = 0; i < 3; i++)
   {
      v[i] = (v1[i] - v2[i]) * t + v2[i];
   }
   v[3] = 1.0;
   return(v);
}

real *vec2cp(vector v, vector o)
{
   v[0] = o[0];
   v[1] = o[1];
   v[2] = o[2];
   return(v);
}

real vecd2(vector v1, vector v2)
{
   real dx, dy;

   dx = v1[0] - v2[0];
   dy = v1[1] - v2[1];
   return(sqrt(dx * dx + dy * dy));
}

void rotmat(vector axis, real th, matrix rot)
{
   real dth;
   real cx, sx, dx, r1, r2, r3, r12, r13, r23;

   dth = th;
   cx  = cos(dth);
   sx  = sin(dth);
   dx  = 1.0 - cx;
   r1  = axis[0];
   r2  = axis[1];
   r3  = axis[2];
   r12 = r1 * r2;
   r13 = r1 * r3;
   r23 = r2 * r3;

   matident(rot);
   rot[0][0] = cx + dx * r1 * r1;
   rot[0][1] = sx * r3 + dx * r12;
   rot[0][2] = -sx * r2 + dx * r13;

   rot[1][0] = -sx * r3 + dx * r12;
   rot[1][1] = cx + dx * r2 * r2;
   rot[1][2] = sx * r1 + dx * r23;

   rot[2][0] = sx * r2 + dx * r13;
   rot[2][1] = -sx * r1 + dx * r23;
   rot[2][2] = cx + dx * r3 * r3;
}
