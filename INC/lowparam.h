/* 
  PROJETO USPDesigner
  MODULO: LOW (Operadores Basicos)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: lowparam.h
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

#ifndef __Params_h
#define __Params_h

#ifndef EXTERN
 #ifdef _DVS_
  #define EXTERN
 #else
  #define EXTERN    extern
 #endif
#endif

/* static memory definition */
#define MAXCTAG              50000  /* size of curve tag vector - curvgeom.h */
#define MAXSTAG              50000  /* size of surf. tag vector - curvgeom.h */

/* return codes and misc constants */
#define NIL                  0
#ifdef _Windows
 #define ERROR                0
#else
 #define ERROR                -1
#endif
#define SUCCESS              -2
#define PI                   3.141592653589793
#define PIPER2               1.570796326494896
#define RTOD                 57.295779513082321
#define DTOR                 0.017453292519943

/* solid status bits */
#define CURVED               1

/* node type parameters for memory allocation routines */
#define SOLID                0
#define SHELL                1
#define FACE                 2
#define LOOP                 3
#define EDGE                 4
#define HALFEDGE             5
#define VERTEX               6
#define AXIS                 7
#define GROUP                8

#define TRUE                 1
#define FALSE                0

#define PLUS                 0
#define MINUS                1

#define _VISIBLE             2
#define INVISIBLE            1
#define DONTSHOW             0

#define AxisOfTranslation    0
#define AxisOfRotation       1
#define NONE                 2
#define AxisSolid            3
#define AxisGroup            4
#define AxisSG               5
#define GroupGroup           6
#define GroupSolid           7

/* coordinate plane names */
#define X                    0
#define Y                    1
#define Z                    2

#define FaceEqNOK(f)         (!(FacFBits((f)) & 1))
#define SetFaceEqOK(f)       (FacFBits((f)) |= 1)
#define SetFaceEqNOK(f)      (FacFBits((f)) &= ~1)

#define FaceBoxNOK(f)        (!(FacFBits(f) & 2))
#define SetFaceBoxOK(f)      (FacFBits(f) |= 2)
#define SetFaceBoxNOK(f)     (FacFBIts(f) &= ~2)

#define FaceBitNOK1(f)       (!((FacFBits(f)) & 4))
#define SetFaceBitOK1(f)     ((FacFBits(f)) = (FacFBits(f)) | 4)
#define SetFaceBitNOK1(f)    ((FacFBits(f)) = (FacFBits(f)) & ~4)

#define FaceBitNOK2(f)       (!((FacFBits(f)) & 8))
#define SetFaceBitOK2(f)     ((FacFBits(f)) = (FacFBits(f)) | 8)
#define SetFaceBitNOK2(f)    ((FacFBits(f)) = (FacFBits(f)) & ~8)

#define ShellBoxNOK(d)       (!(SheDBits(d) & 1))
#define SetShellBoxOK(d)     (SheDBits(d) |= 1)
#define SetShellBoxNOK(d)    (SheDBits(d) &= ~1)

#define SolidBoxNOK(s)       (!(SolSBits(s) & 1))
#define SetSolidBoxOK(s)     (SolSBits(s) |= 1)
#define SetSolidBoxNOK(s)    (SolSBits(s) &= ~1)

/* Box Operations          */
#define INF    1e20;

#define MSD_lowNaoUtilizado(x)    if (x) x = x

#define assert(exp)                                                          \
   if (!((exp))) {                                                           \
      fprintf(stderr, "\"%s\":Assertion failed (file %s, line %d)\n", # exp, \
              __FILE__, __LINE__); fflush(stdout); return (ERROR); }

#define assert1(exp)                                                         \
   if (!((exp))) {                                                           \
      fprintf(stderr, "\"%s\":Assertion failed (file %s, line %d)\n", # exp, \
              __FILE__, __LINE__); fflush(stdout); exit(1); }

#define EPS          0.0001
#define CONTBVEPS    0.001

#define SetError(a)    fprintf(stderr, "ERRO!\n")

#endif
