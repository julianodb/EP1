/* 
  PROJETO USPDesigner
  MODULO: DIS (Hidden Line Elimination)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: discdisp.cpp
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
#define __discdisp_c
#include <stdio.h>
#include <string.h>
#ifndef __UNIX_
 #ifndef _Windows
  #include <graphics.h>
 #else
  #include <windows.h>
 #endif
#endif
#include "mensagem.h"
#include "memvirtu.h"
#include "lowparam.h"
#include "lowmacro.h"
#include "lowsolid.h"
#include "hiegroup.h"
#include "disdispl.h"
#include "gphgraph.h"
#include "genfunc_.h"
#include "shpshape.h"
#include "vectorop.h"
#include "mancommd.h"

void MSD_execDisplay(void)
{
   char *olist, *oname;
   char name1[15];
   int  pn;

   oname = olist = (char *)workbuf;
   for (pn = 0; pn == 0;)
   {
      if (sscanf(restbuf, "%s", name1) == 1)
      {
         skipnext(&restbuf);
         for ( ; sscanf(restbuf, "%s", oname) == 1; ++pn)
         {
            skipnext(&restbuf);
            oname += strlen(oname) + 1;
         }
         *oname = NIL;
      }
      if (pn == 0)
      {
         printf("camera objeto/grupo ...\n");
         if (!lineins("? "))
         {
            return;
         }
      }
   }
   MSD_highNameDisplay(name1, olist);
}

int MSD_highNameDisplay(char *cname, char *olist)
{
   CTYPE *cptr;

   if ((cptr = MSD_lowFindCameraByName(cname)) == (CTYPE *)NIL)
   {
      fprintf(stderr, MEN_CamaraNomeNaoEncontrada,
              NAM_Display, cname);
      return(FALSE);
   }
   camptr = cptr;
   if (MSD_highNameSetDisplay(olist))
   {
#if !defined (__UNIX) && !defined (_Windows)
      MSD_lowSetBreakInterruption();
#endif

      switch (cptr->hid)
      {
          case 0:
          MSD_highDisplayAllEdges();
          break;

          case 1:
          MSD_highDisplayLocalHidden();
          break;

          case 2:
          MSD_lowCameraDisplayHidden();
          break;

          case 3:
          MSD_lowCameraDisplayIntersection();
      }

#if !defined (__UNIX) && !defined (_Windows)
      MSD_lowSetBreakResident();
#endif

      if (dspfwptr)
      {
         fclose(dspfwptr);
         dspfwptr = (FILE *)NIL;
      }
   }
#ifdef __UNIX_
   GGGraphicFlush();
#endif
   return(TRUE);
}

int MSD_highNameSetDisplay(char *oname)
{
   SPTYPE optr;
   GPTYPE gptr;

   if (!strcmp(oname, "**"))
   {
      for (AllSolids(optr))
      {
         SolDsp(optr) = TRUE;
      }
      return(TRUE);
   }

   for (AllSolids(optr))
   {
      SolDsp(optr) = FALSE;
   }
   for ( ; *oname; oname += strlen(oname) + 1)
   {
      if (!strcmp(oname, "*"))
      {
         for (AllSolids(optr))
         {
            if (SolGroup(optr) == def_grpptr)
            {
               SolDsp(optr) = TRUE;
            }
         }
      }
      else
      {
         switch (MSD_getTypeByName(oname))
         {
             case 'o':
             optr         = MSD_lowFindSolidByName(oname);
             SolDsp(optr) = TRUE;
             break;

             case 'g':
             gptr = MSD_lowFindGroupByName(oname);
             for (AllSolids(optr))
             {
                if (MSD_lowIsAncestorGroup(gptr, SolGroup(optr)))
                {
                   SolDsp(optr) = TRUE;
                }
             }
             break;

             default:
             fprintf(stderr, MEN_ElementoNaoEncontrado,
                     NAM_Display, oname);
             return(FALSE);
         }
      }
   }
   return(TRUE);
}

void MSD_highDisplayAllEdges(void)
{
   CTYPE *cptr;

   if (camptr->mode == 3)
   {
      cptr   = camptr;
      camptr = &camera3;
      MSD_lowSetCamera3View(1, cptr);
      MSD_lowDisplayAllEdges();
      MSD_lowSetCamera3View(2, cptr);
      MSD_lowDisplayAllEdges();
      MSD_lowSetCamera3View(3, cptr);
      MSD_lowDisplayAllEdges();
      MSD_lowSetCamera3View(4, cptr);
      MSD_lowDisplayAllEdges();
      MSD_lowSetCamera3View(0, cptr);
   }
   else
   {
      MSD_lowDisplayAllEdges();
   }
}

void MSD_lowDisplayAllEdges(void)
{
   SPTYPE optr;
   DPTYPE dptr;
   EPTYPE eptr;
   VPTYPE vptr;
   vector v1, v2;

   g_frame();
   for (AllSolids(optr))
   {
      if (SolDsp(optr) != TRUE)
      {
         continue;
      }
      g_pcolor(MSD_lowGetSolidColor(optr));
      for (AllShellsSolid(optr, dptr))
      {
         for (AllEdgesShell(dptr, eptr))
         {
            if (breakflg)
            {
               return;
            }
            veccopy(v1, VerVCoord(HalVtx(EdgHe1(eptr))));
            veccopy(v2, VerVCoord(HalVtx(EdgHe2(eptr))));
            MSD_lowDisplayLine(SOLID_LINE, v1, v2);
         }
         for (AllVerticesShell(dptr, vptr))
         {
            if (breakflg)
            {
               return;
            }
            veccopy(v1, VerVCoord(vptr));
            MSD_lowDisplayVertex(SOLID_LINE, v1);
         }
      }
   }
}

void MSD_highDisplayLocalHidden(void)
{
   CTYPE *cptr;

   if (camptr->mode == 3)
   {
      cptr   = camptr;
      camptr = &camera3;
      MSD_lowSetCamera3View(1, cptr);
      MSD_lowDisplayLocalHidden(camptr);
      MSD_lowSetCamera3View(2, cptr);
      MSD_lowDisplayLocalHidden(camptr);
      MSD_lowSetCamera3View(3, cptr);
      MSD_lowDisplayLocalHidden(camptr);
      MSD_lowSetCamera3View(4, cptr);
      MSD_lowDisplayLocalHidden(camptr);
      MSD_lowSetCamera3View(0, cptr);
   }
   else
   {
      MSD_lowDisplayLocalHidden(camptr);
   }
}

void MSD_lowDisplayLocalHidden(CTYPE *cptr)
{
   SPTYPE optr;
   DPTYPE dptr;
   FPTYPE fptr;
   EPTYPE eptr;
   vector v1, v2;

   g_frame();
   for (AllSolids(optr))
   {
      if (SolDsp(optr) == TRUE)
      {
         g_pcolor(MSD_lowGetSolidColor(optr));
         for (AllShellsSolid(optr, dptr))
         {
            for (AllFacesShell(dptr, fptr))
            {
               FacFVisi(fptr) = MSD_lowIsFaceVisible(cptr, fptr);
            }
         }

         for (AllShellsSolid(optr, dptr))
         {
            for (AllEdgesShell(dptr, eptr))
            {
               if (breakflg)
               {
                  return;
               }
               switch (MSD_lowIsEdgeVisible(cptr, eptr))
               {
                   case INVISIBLE:
                   if (cptr->dash == TRUE)
                   {
                      veccopy(v1, VerVCoord(HalVtx(EdgHe1(eptr))));
                      veccopy(v2, VerVCoord(HalVtx(EdgHe2(eptr))));
                      MSD_lowDisplayLine(DASHED_LINE, v1, v2);
                   }
                   break;

                   case _VISIBLE:
                   veccopy(v1, VerVCoord(HalVtx(EdgHe1(eptr))));
                   veccopy(v2, VerVCoord(HalVtx(EdgHe2(eptr))));
                   MSD_lowDisplayLine(SOLID_LINE, v1, v2);
               }
            }
         }
      }
   }
}

void MSD_lowDisplayLine(int l, vector sv, vector ev)
{
   vect2 sv2, ev2;

   if (MSD_lowExecDisplayTransformation(sv, ev, sv2, ev2))
   {
      g_tline(l, sv2, ev2);
   }
}

void MSD_lowDisplayVertex(int l, vector v)
{
   vect2 sv2, ev2;

   if (MSD_lowExecDisplayTransformation(v, v, sv2, ev2))
   {
      g_tpoint(l, sv2);
   }
}
