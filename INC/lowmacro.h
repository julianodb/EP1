/* 
  PROJETO USPDesigner
  MODULO: LOW (Operadores Basicos)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: lowmacro.h
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

#ifndef __Macros_h
#define __Macros_h

#define MSD_lowNaoUtilizado(x)    if (x) x = x

#define AllSolids(s)              (s) = objary; (s) != SNIL; (s) = SolNxt(s)
#define AllShellsSolid(s, d)      (d) = SolSShells(s); (d) != DNIL; (d) = SheNextD(d)
#define AllFacesShell(d, f)       (f) = SheSFaces(d); (f) != FNIL; (f) = FacNextF(f)
#define AllLoopsFace(f, l)        (l) = FacFLoops(f); (l) != LNIL; (l) = LooNextL(l)
#define AllEdgesShell(d, e)       (e) = SheSEdges(d); (e) != ENIL; (e) = EdgNextE(e)
#define AllVerticesShell(d, v)    (v) = SheSVerts(d); (v) != VNIL; (v) = VerNextV(v)
#define AllCamera(c)              (c) = camary; (c); (c) = (c)->nxt
#define AllAxis(a)                (a) = axsary; (a) != ANIL; (a) = AxsNextA(a)
#define AllGroup(g)               (g) = grpary; (g) != GNIL; (g) = GrpNextG(g)

#ifdef __curved_env__
 #define AllCurvesSolid(s, c)      (c) = SolCurv(s); (c); (c) = (c)->nextc
#endif

#define InnerLoop(f)                              \
   (FacFLoops(f) != FacFLOut(f)) ? FacFLoops(f) : \
   ((FacFLoops(f) != LNIL) ? (LooNextL(FacFLoops(f))) : LNIL)

#define mate(x)        (((x) == EdgHe1(HalEdg(x))) ? EdgHe2(HalEdg(x)) : EdgHe1(HalEdg(x)))

#define trace_v(v)                                                                       \
   fprintf(trace, "*** vertice %3d                       %10.4f %10.4f %10.4f %10.4f\n", \
           VerVertexNo(v), VerVCoord(v)[0], VerVCoord(v)[1], VerVCoord(v)[2],            \
           VerVCoord(v)[3])

#define trace_f(ff)                                                                      \
   fprintf(trace, "*** face    %3d                       %10.4f %10.4f %10.4f %10.4f\n", \
           FacFaceNo(ff), FacFeq(ff)[0], FacFeq(ff)[1], FacFeq(ff)[2],                   \
           FacFeq(ff)[3])

#define trace_sec(x)                                                                                 \
   fprintf(trace, "*** sector[%2i] s1a-%4s  s2a-%4s  s1b-%4s  s2b-%4s  seca%2d secb%2d int%2d\n", x, \
           ff[SetSecs1a(x) + 1], ff[SetSecs2a(x) + 1], ff[SetSecs1b(x) + 1],                         \
           ff[SetSecs2b(x) + 1], SetSecA(x), SetSecB(x),                                             \
           SetSecInter(x))

#endif
