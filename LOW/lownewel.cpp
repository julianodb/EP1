/* 
  PROJETO USPDesigner
  MODULO: LOW (Operadores Basicos)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: lownewel.cpp
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
#ifndef __UNIX_
 #include <string.h>
 #include <malloc.h>
#endif
#include "memvirtu.h"
#include "lowparam.h"
#include "lowmacro.h"
#include "lowsolid.h"
#include "hiegroup.h"
#include "curvgeom.h"

unsigned nodesize[] =
{
   sizeof(STYPE), sizeof(DTYPE), sizeof(FTYPE), sizeof(LTYPE),
   sizeof(ETYPE), sizeof(HTYPE), sizeof(VTYPE), sizeof(ATYPE),
   sizeof(GTYPE),             0,
};

/* The most primitive functionality needed for Euler operators consists */
/* of the storage allocation and deallocation. We shall here build our  */
/* own allocator on the top of the ordinary UNIX utilities. In practice,*/
/* this allocator should be replaced by a more rigorous version that    */
/* itself keeps track of the available storage and avoids the costly    */
/* UNIX memory allocation procedure malloc.                             */
/* A design goal of the allocator is to simplify the implementation of  */
/* Euler Operators by pushing the handling of many of the pointers of   */
/* the half-edge data structure into the allocator and other very low-  */
/* level routines. This is accomplished by giving the allocator         */
/* information on where the new node is to be inserted.                 */
/* The allocator new returns a pointer to a new node of the data        */
/* structure; good C programming style requires that a type conversion  */
/* is specified when calling the allocator. The type of the node is     */
/* selected with argument what, which must be one of the literals SOLID,*/
/* FACE, LOOP, HALFEDGE, EDGE or VERTEX. Another argument where must    */
/* point to a parent node of the new node as follows:                   */
/*   1- what = FACE, EDGE or VERTEX: where points to the parent solid   */
/*   2- what = LOOP: where points to the parent face                    */
/* In these cases new performs the necessary manipulations to link the  */
/* new node with its parent. Otherwise where has no effect and may be   */
/* NIL. Array nodesize contains the sizes of the various nodes.         */
/* Analogously to the storage allocator new, the deallocator procedure  */
/* del(what, which, where) removes the node which from the data         */
/* structure, and disconnects it from the list of the children of the   */
/* parent node where if what = SOLID, FACE, LOOP, EDGE or VERTEX. All   */
/* our algorithms will assume that the data in a deleted node reamins   */
/* valid until the next call of new.                                    */

NPTYPE MSD_lowNewElement(int what, NPTYPE where)
{
   NPTYPE node;

#ifndef __VIRTUAL_MEM
   if ((node = (NPTYPE)malloc(nodesize[what])) == NNIL)
   {
#else
   if ((node = (NPTYPE)MSD_getMemVirtual(what)) == NNIL)
   {
#endif
      fprintf(stderr, "new: no memory left!\n");
      return(NNIL);
   }

   switch (what)
   {
       case SOLID:
       MSD_lowAddList(SOLID, node, NNIL);
#ifndef __VIRTUAL_MEM
 #ifdef __curved_env__
       node->s.scurvs = CNIL;
 #endif
       node->s.sshells = DNIL;
       node->s.dsp     = TRUE;
       node->s.sbits   = 0;
       node->s.color   = -1;
       node->s.corte   = -1;
       node->s.group   = def_grpptr;
       node->s.oaxs    = ANIL;
       strcpy(node->s.name, "");
#else
 #ifdef __curved_env__
       SolCurv(node) = (CURVE *)NIL;
 #endif
       SolSShells(node) = DNIL;
       SolDsp(node)     = TRUE;
       SolSBits(node)   = 0;
       SolColor(node)   = -1;
       SolPerfil(node)  = -1;
       SolGroup(node)   = def_grpptr;
       SolOAxs(node)    = ANIL;
       strcpy(SolName(node), "");
#endif
       break;

       case SHELL:
       MSD_lowAddList(SHELL, node, where);
#ifndef __VIRTUAL_MEM
       node->d.sfaces = FNIL;
       node->d.sedges = ENIL;
       node->d.sverts = VNIL;
       node->d.dbits  = 0;
       node->d.color  = -1;
#else
       SheSFaces(node) = FNIL;
       SheSEdges(node) = ENIL;
       SheSVerts(node) = VNIL;
       SheDBits(node)  = 0;
       SheColor(node)  = -1;
#endif
       break;

       case FACE:
       MSD_lowAddList(FACE, node, where);
#ifndef __VIRTUAL_MEM
       node->f.floops = LNIL;
       node->f.flout  = LNIL;
       node->f.fbits  = 0;
       node->f.color  = -1;
#else
       FacFLoops(node) = LNIL;
       FacFLOut(node)  = LNIL;
       FacFBits(node)  = 0;
       FacColor(node)  = -1;
       FacFlag(node)   = 0;
#endif
       break;

       case LOOP:
       MSD_lowAddList(LOOP, node, where);
       break;

       case HALFEDGE:
#ifndef __VIRTUAL_MEM
 #ifdef __curved_env__
       node->h.curv = CNIL;
       node->h.r    = 0;
 #endif
#else
 #ifdef __curved_env__
       HalCurv(node) = (CURVE *)NIL;
       HalR(node)    = 0;
 #endif
#endif
       break;

       case EDGE:
       MSD_lowAddList(EDGE, node, where);
#ifndef __VIRTUAL_MEM
       node->e.color = -1;
 #ifdef __curved_env__
       node->e.fg_round = 1;
       node->e.fg_make  = 0;
       node->e.t1       = 0;
       node->e.t2       = 0;
       node->e.t3       = 0;
       node->e.t4       = 0;
       node->e.d        = 0;
 #endif
#else
       EdgColor(node) = -1;
 #ifdef __curved_env__
       EdgFgRound(node) = 1;
       EdgFgMake(node)  = 0;
       EdgT1(node)      = 0;
       EdgT2(node)      = 0;
       EdgT3(node)      = 0;
       EdgT4(node)      = 0;
       EdgD(node)       = 0;
 #endif
#endif
       break;

       case VERTEX:
       MSD_lowAddList(VERTEX, node, where);
#ifndef __VIRTUAL_MEM
       node->v.vedge = HNIL;
 #ifdef __curved_env__
       node->v.fg_round = 0;
       node->v.fg_make  = 0;
 #endif
#else
       VerVEdge(node) = HNIL;
 #ifdef __curved_env__
       VerFgRound(node) = 0;
       VerFgMake(node)  = 0;
 #endif
#endif
       break;

       case AXIS:
       MSD_lowAddList(AXIS, node, where);
#ifndef __VIRTUAL_MEM
       node->a.type           = NONE;
       node->a.hierarq.agroup = GNIL;
#else
       AxsType(node)   = NONE;
       AxsAGroup(node) = GNIL;
#endif
       break;

       case GROUP:
       MSD_lowAddList(GROUP, node, where);
#ifndef __VIRTUAL_MEM
       node->g.gcolor = -1;
       node->g.gaxs   = ANIL;
       node->g.parent = def_grpptr;
#else
       GrpGColor(node) = -1;
       GrpGAxs(node)   = ANIL;
       GrpParent(node) = def_grpptr;
#endif
       break;

       default:
       fprintf(stderr, "new: codigo incorreto!\n");
       break;
   }
   return(node);
}

void MSD_lowDelElement(int what, NPTYPE node, NPTYPE where)
{
   switch (what)
   {
       case SOLID:
       MSD_lowDelList(SOLID, node, where);
       break;

       case SHELL:
       MSD_lowDelList(SHELL, node, where);
       break;

       case FACE:
       MSD_lowDelList(FACE, node, where);
       break;

       case LOOP:
       MSD_lowDelList(LOOP, node, where);
       break;

       case HALFEDGE:
       break;

       case EDGE:
       MSD_lowDelList(EDGE, node, where);
       break;

       case VERTEX:
       MSD_lowDelList(VERTEX, node, where);
       break;

       case GROUP:
       MSD_lowDelList(GROUP, node, where);
       break;

       case AXIS:
       MSD_lowDelList(AXIS, node, where);
       break;

       default:
       fprintf(stderr, "del: codigo incorreto!\n");
       break;
   }

#ifdef __VIRTUAL_MEM
   MSD_putMemVirtual(node, what);
#else
   free((char *)node);
#endif
}

#ifndef __VIRTUAL_MEM
/* how many bytes does s use ? */
int bytesused(STYPE *s)
{
   int   used;
   DTYPE *d;
   FTYPE *f;
   LTYPE *l;
   ETYPE *e;
   VTYPE *v;

   used = nodesize[SOLID];
   for (AllShellsSolid(s, d))
   {
      for (AllFacesShell(d, f))
      {
         for (AllLoopsFace(f, l))
         {
            used += l->length ? l->length * nodesize[HALFEDGE] :
                    nodesize[HALFEDGE];
            used += nodesize[LOOP];
         }
         used += nodesize[FACE];
      }
      used += nodesize[SHELL];
   }
   for (AllShellsSolid(s, d))
   {
      for (AllEdgesShell(d, e))
      {
         used += nodesize[EDGE];
      }
   }
   for (AllShellsSolid(s, d))
   {
      for (AllVerticesShell(d, v))
      {
         used += nodesize[VERTEX];
      }
   }
   return(used);
}

#endif
