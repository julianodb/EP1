/* 
  PROJETO USPDesigner
  MODULO: DIS (Hidden Line Elimination)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: discaxis.cpp
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
#include "vectorop.h"
#include "disdispl.h"
#include "gphgraph.h"
#include "mancommd.h"

extern int MaxColors;

void MSD_execDisplayCoordinates(void)
{
   int   acolor, mcolor = 0, dir = 0;
   char  opt;
   float l;
   real  dis = 0.0, dl = 0.0;

   for ( ; ;)
   {
      switch (opt = optin())
      {
          case 'a':
          if (2 == sscanf(restbuf, "%f %d", &l, &acolor))
          {
             MSD_highDisplayCoordinates(opt, dir, dis, l, dl, acolor, mcolor);
             return;
          }
          break;

          case 'm':
          skipsp(&restbuf);
          switch (*restbuf++)
          {
              case 'x':
              case 'X':
              dir = 0;
              break;

              case 'y':
              case 'Y':
              dir = 1;
              break;

              case 'z':
              case 'Z':
              dir = 2;
              break;

              default:
              dir = -1;
          }
          skipsp(&restbuf);
          if ((dir == -1) || (*restbuf++ != '='))
          {
             break;
          }
          if (5 == sscanf(restbuf, "%f %f %f %d %d", &dis, &l, &dl,
                          &acolor, &mcolor))
          {
             MSD_highDisplayCoordinates(opt, dir, dis, l, dl, acolor, mcolor);
             return;
          }
      }
      printf(MEN_CoordenadasParametros, NAM_DisplayCoordinates);
      if (!lineins("? "))
      {
         return;
      }
   }
}

int MSD_highDisplayCoordinates(int opt, int dir, real dis, real l, real dl,
                               int acolor, int mcolor)
{
   if (!monicptr)
   {
      fprintf(stderr, MEN_NenhumaCamaraMonitora, NAM_DisplayCoordinates);
      return(FALSE);
   }
   if ((0 > acolor) || (acolor > MaxColors - 1) && (0 > mcolor) && (mcolor > MaxColors - 1))
   {
      fprintf(stderr, MEN_CorInvalida, NAM_DisplayCoordinates);
      return(TRUE);
   }

   switch (opt)
   {
       case 'a':
       return(MSD_lowDisplayAxis(l, acolor));

       case 'm':
       return(MSD_lowDisplayMesh(dir, dis, l, dl, acolor, mcolor));
   }
   fprintf(stderr, MEN_ParametrosIncorretos, NAM_DisplayCoordinates);
   return(FALSE);
}

int MSD_lowDisplayAxis(real len, int acolor)
{
   static vector zero = { 0.0, 0.0, 0.0, 0.0 };
   vector        v3;

   camptr = monicptr;
   g_pcolor(acolor);

   v3[0] = len;
   v3[1] = v3[2] = 0.0;
   MSD_lowDisplayLineCoordinates(SOLID_LINE, v3, zero);
   v3[0] = -len;
   MSD_lowDisplayLineCoordinates(DASHED_LINE, v3, zero);

   v3[1] = len;
   v3[0] = v3[2] = 0.0;
   MSD_lowDisplayLineCoordinates(SOLID_LINE, v3, zero);
   v3[1] = -len;
   MSD_lowDisplayLineCoordinates(DASHED_LINE, v3, zero);

   v3[2] = len;
   v3[0] = v3[1] = 0.0;
   MSD_lowDisplayLineCoordinates(SOLID_LINE, v3, zero);
   v3[2] = -len;
   MSD_lowDisplayLineCoordinates(DASHED_LINE, v3, zero);
   return(TRUE);
}

int MSD_lowDisplayMesh(int dir, real dis, real len, real dl, int acolor, int mcolor)
{
   vector v0, v3;
   real   d;
   int    idx1, idx2, dn, i, j;

   camptr = monicptr;
   dn     = (int)((len + abseps) / dl);
   switch (dir)
   {
       case 0:
       idx1 = 1;
       idx2 = 2;
       break;

       case 1:
       idx1 = 2;
       idx2 = 0;
       break;

       case 2:
       idx1 = 0;
       idx2 = 1;
       break;
   }
   v0[dir] = v3[dir] = dis;
   for (i = 0; i < 2; ++i)
   {
      for (d = dl * (-dn), j = -dn; j <= dn; d += dl, ++j)
      {
         g_pcolor(j ? mcolor : acolor);
         v0[idx2] = v3[idx2] = d;
         v0[idx1] = 0.0;
         v3[idx1] = len;
         MSD_lowDisplayLineCoordinates(SOLID_LINE, v0, v3);
         v3[idx1] = -len;
         MSD_lowDisplayLineCoordinates(DASHED_LINE, v0, v3);
      }
      j    = idx1;
      idx1 = idx2;
      idx2 = j;
   }
   return(TRUE);
}

void MSD_lowDisplayLineCoordinates(int lt, vector sv, vector ev)
{
   vect2 sv2, ev2;

   if (MSD_lowExecDisplayTransformation(sv, ev, sv2, ev2))
   {
      g_tline(lt, sv2, ev2);
   }
}
