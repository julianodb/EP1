/* 
  PROJETO USPDesigner
  MODULO: LOW (Operadores Basicos)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: lowaddel.cpp
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
 #include <stdlib.h>
#endif
#include "memvirtu.h"
#include "lowparam.h"
#include "lowmacro.h"
#include "lowsolid.h"

/* The actual insertion of faces, loops, edges and vertices into the */
/* proper lists takes place with the procedure addlist. It adds the  */
/* node pointede to by which into the linked list of nodes owned by  */
/* where. As a special case, solids are simply linked to the list    */
/* of all solids; in this case, where has no effect. The type of     */
/* which is again given in terms of what and must be one of the      */
/* literals SOLID, FACE, LOOP, EDGE or VERTEX.                       */
/* To unlink children from their parents, the corresponding inverse  */
/* procedure dellist(what, which, where) is used. Specifically, the  */
/* node which of type what (SOLID, FACE, LOOP, EDGE, VERTEX) will be */
/* removed from the respective linked list of the parent node which. */
/* Again, solids are an exception. Obviously, dellist would be used  */
/* in the implementation of del in analogous way to addlist and new. */

void MSD_lowAddList(int what, NPTYPE which, NPTYPE where)
{
   switch (what)
   {
       case SOLID:
#ifndef __VIRTUAL_MEM
       which->s.nxt   = objary;
       which->s.prevs = SNIL;
#else
       SolNxt(which)   = objary;
       SolPrevS(which) = SNIL;
#endif
       if (objary != SNIL)
       {
          SolPrevS(objary) = (SPTYPE)which;
       }
       objary = (SPTYPE)which;
       break;

       case SHELL:
#ifndef __VIRTUAL_MEM
       which->d.nextd = where->s.sshells;
       which->d.prevd = DNIL;
       if (where->s.sshells != DNIL)
       {
          where->s.sshells->prevd = (DPTYPE)which;
       }
       where->s.sshells = (DPTYPE)which;
       which->d.ssolids = (SPTYPE)where;
#else
       SheNextD(which) = SolSShells(where);
       ShePrevD(which) = DNIL;
       if (SolSShells(where) != DNIL)
       {
          ShePrevD(SolSShells(where)) = (DPTYPE)which;
       }
       SolSShells(where) = (DPTYPE)which;
       SheSSolids(which) = (SPTYPE)where;
#endif
       break;

       case FACE:
#ifndef __VIRTUAL_MEM
       which->f.nextf = where->d.sfaces;
       which->f.prevf = FNIL;
       if (where->d.sfaces != FNIL)
       {
          where->d.sfaces->prevf = (FPTYPE)which;
       }
       where->d.sfaces = (FPTYPE)which;
       which->f.fshell = (DPTYPE)where;
#else
       FacNextF(which) = SheSFaces(where);
       FacPrevF(which) = FNIL;
       if (SheSFaces(where) != FNIL)
       {
          FacPrevF(SheSFaces(where)) = (FPTYPE)which;
       }
       SheSFaces(where) = (FPTYPE)which;
       FacFShell(which) = (DPTYPE)where;
#endif
       break;

       case LOOP:
#ifndef __VIRTUAL_MEM
       which->l.nextl = where->f.floops;
       which->l.prevl = LNIL;
       if (where->f.floops != LNIL)
       {
          where->f.floops->prevl = (LPTYPE)which;
       }
       where->f.floops = (LPTYPE)which;
       which->l.lface  = (FPTYPE)where;
#else
       LooNextL(which) = FacFLoops(where);
       LooPrevL(which) = LNIL;
       if (FacFLoops(where) != LNIL)
       {
          LooPrevL(FacFLoops(where)) = (LPTYPE)which;
       }
       FacFLoops(where) = (LPTYPE)which;
       LooLFace(which)  = (FPTYPE)where;
#endif
       break;

       case EDGE:
#ifndef __VIRTUAL_MEM
       which->e.nexte = where->d.sedges;
       which->e.preve = ENIL;
       if (where->d.sedges != ENIL)
       {
          where->d.sedges->preve = (EPTYPE)which;
       }
       where->d.sedges = (EPTYPE)which;
#else
       EdgNextE(which) = SheSEdges(where);
       EdgPrevE(which) = ENIL;
       if (SheSEdges(where) != ENIL)
       {
          EdgPrevE(SheSEdges(where)) = (EPTYPE)which;
       }
       SheSEdges(where) = (EPTYPE)which;
#endif
       break;

       case VERTEX:
#ifndef __VIRTUAL_MEM
       which->v.nextv = where->d.sverts;
       which->v.prevv = VNIL;
       if (where->d.sverts != VNIL)
       {
          where->d.sverts->prevv = (VPTYPE)which;
       }
       where->d.sverts = (VPTYPE)which;
#else
       VerNextV(which) = SheSVerts(where);
       VerPrevV(which) = VNIL;
       if (SheSVerts(where) != VNIL)
       {
          VerPrevV(SheSVerts(where)) = (VPTYPE)which;
       }
       SheSVerts(where) = (VPTYPE)which;
#endif
       break;

       case GROUP:
#ifndef __VIRTUAL_MEM
       which->g.nextg = grpary;
       which->g.prevg = GNIL;
#else
       GrpNextG(which) = grpary;
       GrpPrevG(which) = GNIL;
#endif
       if (grpary != GNIL)
       {
          GrpPrevG(grpary) = (GPTYPE)which;
       }
       grpary = (GPTYPE)which;
       break;

       case AXIS:
#ifndef __VIRTUAL_MEM
       which->a.nexta = axsary;
       which->a.preva = ANIL;
#else
       AxsNextA(which) = axsary;
       AxsPrevA(which) = ANIL;
#endif
       if (axsary != ANIL)
       {
          AxsPrevA(axsary) = (APTYPE)which;
       }
       axsary = (APTYPE)which;
       break;

       default:
       fprintf(stderr, "addlist: codigo incorreto!\n");
       break;
   }
}

void MSD_lowDelList(int what, NPTYPE which, NPTYPE where)
{
   SPTYPE s;

#ifdef __VIRTUAL_MEM
   SPTYPE Sol;
   DPTYPE She;
   FPTYPE Fac;
   LPTYPE Loo;
   EPTYPE Edg;
   VPTYPE Ver;
#endif
   int ip;

   switch (what)
   {
       case SOLID:
       if (SetTrace)
       {
          ip = 0;
          for (AllSolids(s))
          {
             if (s == (SPTYPE)which)
             {
                ip = 1;
             }
          }
          if (ip == 0)
          {
             fprintf(stderr, "*** del(SOLID): aborted ***\n");
             error = TRUE;
             return;
          }
       }
#ifndef __VIRTUAL_MEM
       if (which->s.nxt != SNIL)
       {
          which->s.nxt->prevs = which->s.prevs;
       }
       if (which->s.prevs != SNIL)
       {
          which->s.prevs->nxt = which->s.nxt;
       }
       if (objary == (SPTYPE)which)
       {
          objary = which->s.nxt;
       }
#else
       Sol = SolPrevS(which);
       if (SolNxt(which) != SNIL)
       {
          SolPrevS(SolNxt(which)) = Sol;
       }
       Sol = SolNxt(which);
       if (SolPrevS(which) != SNIL)
       {
          SolNxt(SolPrevS(which)) = Sol;
       }
       if (objary == (SPTYPE)which)
       {
          objary = SolNxt(which);
       }
#endif
       break;

       case SHELL:
       if (SetTrace)
       {
          if (findd((SPTYPE)where, (DPTYPE)which) == 0)
          {
             fprintf(stderr, "*** del(SHELL): aborted ***\n");
             error = TRUE;
             return;
          }
       }
#ifndef __VIRTUAL_MEM
       if (which->d.prevd != DNIL)
       {
          which->d.prevd->nextd = which->d.nextd;
       }
       if (which->d.nextd != DNIL)
       {
          which->d.nextd->prevd = which->d.prevd;
       }
       if ((DPTYPE)which == where->s.sshells)
       {
          where->s.sshells = which->d.nextd;
       }
#else
       She = SheNextD(which);
       if (ShePrevD(which) != DNIL)
       {
          SheNextD(ShePrevD(which)) = She;
       }
       She = ShePrevD(which);
       if (SheNextD(which) != DNIL)
       {
          ShePrevD(SheNextD(which)) = She;
       }
       She = SheNextD(which);
       if ((DPTYPE)which == SolSShells(where))
       {
          SolSShells(where) = She;
       }
#endif
       break;

       case FACE:
       if (SetTrace)
       {
          if (findf((DPTYPE)where, (FPTYPE)which) == 0)
          {
             fprintf(stderr, "*** del(FACE): aborted ***\n");
             error = TRUE;
             return;
          }
       }
#ifndef __VIRTUAL_MEM
       if (which->f.prevf != FNIL)
       {
          which->f.prevf->nextf = which->f.nextf;
       }
       if (which->f.nextf != FNIL)
       {
          which->f.nextf->prevf = which->f.prevf;
       }
       if ((FPTYPE)which == where->d.sfaces)
       {
          where->d.sfaces = which->f.nextf;
       }
#else
       Fac = FacNextF(which);
       if (FacPrevF(which) != FNIL)
       {
          FacNextF(FacPrevF(which)) = Fac;
       }
       Fac = FacPrevF(which);
       if (FacNextF(which) != FNIL)
       {
          FacPrevF(FacNextF(which)) = Fac;
       }
       Fac = FacNextF(which);
       if ((FPTYPE)which == SheSFaces(where))
       {
          SheSFaces(where) = Fac;
       }
#endif
       break;

       case LOOP:
       if (SetTrace)
       {
          if (findl((LPTYPE)which, (FPTYPE)where) == 0)
          {
             fprintf(stderr, "*** del(LOOP): aborted ***\n");
             error = TRUE;
             return;
          }
       }
#ifndef __VIRTUAL_MEM
       if (which->l.prevl != LNIL)
       {
          which->l.prevl->nextl = which->l.nextl;
       }
       if (which->l.nextl != LNIL)
       {
          which->l.nextl->prevl = which->l.prevl;
       }
       if ((LPTYPE)which == where->f.floops)
       {
          where->f.floops = which->l.nextl;
       }
#else
       Loo = LooNextL(which);
       if (LooPrevL(which) != LNIL)
       {
          LooNextL(LooPrevL(which)) = Loo;
       }
       Loo = LooPrevL(which);
       if (LooNextL(which) != LNIL)
       {
          LooPrevL(LooNextL(which)) = Loo;
       }
       Loo = LooNextL(which);
       if ((LPTYPE)which == FacFLoops(where))
       {
          FacFLoops(where) = Loo;
       }
#endif
       break;

       case EDGE:
       if (SetTrace)
       {
          if (findes((DPTYPE)where, (EPTYPE)which) == 0)
          {
             fprintf(stderr, "*** del(EDGE): aborted ***\n");
             error = TRUE;
             return;
          }
       }
#ifndef __VIRTUAL_MEM
       if (which->e.preve != ENIL)
       {
          which->e.preve->nexte = which->e.nexte;
       }
       if (which->e.nexte != ENIL)
       {
          which->e.nexte->preve = which->e.preve;
       }
       if ((EPTYPE)which == where->d.sedges)
       {
          where->d.sedges = which->e.nexte;
       }
#else
       Edg = EdgNextE(which);
       if (EdgPrevE(which) != ENIL)
       {
          EdgNextE(EdgPrevE(which)) = Edg;
       }
       Edg = EdgPrevE(which);
       if (EdgNextE(which) != ENIL)
       {
          EdgPrevE(EdgNextE(which)) = Edg;
       }
       Edg = EdgNextE(which);
       if ((EPTYPE)which == SheSEdges(where))
       {
          SheSEdges(where) = Edg;
       }
#endif
       break;

       case VERTEX:
       if (SetTrace)
       {
          if (findv((DPTYPE)where, (VPTYPE)which) == 0)
          {
             fprintf(stderr, "*** del(VERTEX): aborted ***\n");
             error = TRUE;
             return;
          }
       }
#ifndef __VIRTUAL_MEM
       if (which->v.prevv != VNIL)
       {
          which->v.prevv->nextv = which->v.nextv;
       }
       if (which->v.nextv != VNIL)
       {
          which->v.nextv->prevv = which->v.prevv;
       }
       if ((VPTYPE)which == where->d.sverts)
       {
          where->d.sverts = which->v.nextv;
       }
#else
       Ver = VerNextV(which);
       if (VerPrevV(which) != VNIL)
       {
          VerNextV(VerPrevV(which)) = Ver;
       }
       Ver = VerPrevV(which);
       if (VerNextV(which) != VNIL)
       {
          VerPrevV(VerNextV(which)) = Ver;
       }
       Ver = VerNextV(which);
       if ((VPTYPE)which == SheSVerts(where))
       {
          SheSVerts(where) = Ver;
       }
#endif
       break;

       case GROUP:
#ifndef __VIRTUAL_MEM
       if (which->g.nextg != GNIL)
       {
          which->g.nextg->prevg = which->g.prevg;
       }
       if (which->g.prevg != GNIL)
       {
          which->g.prevg->nextg = which->g.nextg;
       }
       if (grpary == (GPTYPE)which)
       {
          grpary = which->g.nextg;
       }
#else
       if (GrpNextG(which) != GNIL)
       {
          GrpPrevG(GrpNextG(which)) = GrpPrevG(which);
       }
       if (GrpPrevG(which) != GNIL)
       {
          GrpNextG(GrpPrevG(which)) = GrpNextG(which);
       }
       if (grpary == (GPTYPE)which)
       {
          grpary = GrpNextG(which);
       }
#endif
       break;

       case AXIS:
#ifndef __VIRTUAL_MEM
       if (which->a.nexta != ANIL)
       {
          which->a.nexta->preva = which->a.preva;
       }
       if (which->a.preva != ANIL)
       {
          which->a.preva->nexta = which->a.nexta;
       }
       if (axsary == (APTYPE)which)
       {
          axsary = which->a.nexta;
       }
#else
       if (AxsNextA(which) != ANIL)
       {
          AxsPrevA(AxsNextA(which)) = AxsPrevA(which);
       }
       if (AxsPrevA(which) != ANIL)
       {
          AxsNextA(AxsPrevA(which)) = AxsNextA(which);
       }
       if (axsary == (APTYPE)which)
       {
          axsary = AxsNextA(which);
       }
#endif
       break;

       default:
       fprintf(stderr, "dellist: codigo incorreto!\n");
       break;
   }
}
