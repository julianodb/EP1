/* 
  PROJETO USPDesigner
  MODULO: LOW (Operadores Basicos)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: lowaddhe.cpp
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
#include "memvirtu.h"
#include "lowparam.h"
#include "lowsolid.h"

/* In the implementation of the allocator, the actual initialization     */
/* of halfedges and edges is performed by procedure addhe. The procedure */
/* allocates a new halfedge, links it into loop l at the front of an     */
/* existing halfedge where, and sets the outer pointers appropriately.   */
/* Note that addhe is written so as to handle the special cases of an    */
/* empty original loop (where->edg == NIL) or no loop at all (where ==   */
/* NIL).                                                                 */
/* The inverse procedure delhe performs the opposite actions. Note in    */
/* particular how it creates an "empty" loop in the case that he->edg != */
/* NIL and he->nxt == he.                                                */
/* Algorithms of ESM (Educational Solid Modeler) will do the bulk of     */
/* their work with Euler Operators. Occasionally, however, they will also*/
/* use some of the functions described above. For instance, the          */
/* sectioning algorithm will use new, addlist and dellist to actually    */
/* divide the sectioned solid.                                           */

HPTYPE MSD_lowAddHE(EPTYPE e, VPTYPE v, HPTYPE where, int orient)
{
   HPTYPE he;

   if (HalEdg(where) == ENIL)
   {
      he = where;
   }
   else
   {
      if ((he = (HPTYPE)MSD_lowNewElement(HALFEDGE, NNIL)) == HNIL)
      {
         return(HNIL);
      }
      HalNxt(HalPrv(where)) = he;
      HalPrv(he)            = HalPrv(where);
      HalPrv(where)         = he;
      HalNxt(he)            = where;
   }
   HalEdg(he)   = e;
   HalVtx(he)   = v;
   HalWLoop(he) = HalWLoop(where);

   if (orient == PLUS)
   {
      EdgHe1(e) = he;
   }
   else
   {
      EdgHe2(e) = he;
   }
   return(he);
}

HPTYPE MSD_lowDelHE(HPTYPE he)
{
   HPTYPE hhe;

   if (HalEdg(he) == ENIL)
   {
      MSD_lowDelElement(HALFEDGE, (NPTYPE)he, NNIL);
      return(HNIL);
   }
   if (HalNxt(he) == he)
   {
      HalEdg(he) = ENIL;
      return(he);
   }
   hhe = HalPrv(he);
   HalNxt(HalPrv(he)) = HalNxt(he);
   HalPrv(HalNxt(he)) = HalPrv(he);   /****************** 01/03/95 */
   MSD_lowDelElement(HALFEDGE, (NPTYPE)he, NNIL);
   return(hhe);
}
