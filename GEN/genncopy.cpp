/* 
  PROJETO USPDesigner
  MODULO: GEN (Funcoes Gerais de Nivel Superior)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: genncopy.cpp
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
#include "lowmacro.h"
#include "lowsolid.h"
#include "genfunc_.h"
#include "hiegroup.h"
#include "vectorop.h"
#include "mancommd.h"

void MSD_execNameNCopySolid(void)
{
   int   ip, n;
   char  name[30];
   float th, mv, x1, y1, z1, x2, y2, z2;

   for (ip = 0; ip == 0;)
   {
      switch (optin())
      {
          case 'l':
          if (8 == sscanf(restbuf, "%s %f %f %f %f %f %f %d", name,
                          &x1, &y1, &z1, &x2, &y2, &z2, &n))
          {
             MSD_highNameNCopySolidTranslate(name, x1, y1, z1, x2, y2, z2, n);
             ip = 1;
          }
          break;

          case 's':
          if (10 == sscanf(restbuf, "%s %f %f %f %f %f %f %f %f %d",
                           name, &x1, &y1, &z1, &x2, &y2, &z2, &th, &mv, &n))
          {
             MSD_highNameNCopySolidSpiral(name, x1, y1, z1, x2, y2, z2, th, mv, n);
             ip = 1;
          }
          break;
      }
      if (ip == 0)
      {
         printf("-l Objeto Pos_ini Pos_final N_vezes\n");
         printf("-s Objeto Pto_axis Direcao Ang Pos N_vezes\n");
         if (!lineins("? "))
         {
            return;
         }
      }
   }
}

int MSD_highNameNCopySolidTranslate(char *name, real Xi, real Yi, real Zi, real Xd, \
                                    real Yd, real Zd, int n)
{
   vector omove, dmove;

   makevec(omove, Xi, Yi, Zi, 1.0);
   makevec(dmove, Xd, Yd, Zd, 1.0);
   return(MSD_lowNameNCopySolidTranslate(name, omove, dmove, n));
}

int MSD_lowNameNCopySolidTranslate(char *name, vector omove, vector dmove, int n)
{
   SPTYPE s;
   char   defname[30];
   vector disp;
   int    i;

   veccopy(disp, omove);
   for (i = 0; i < n; ++i)
   {
      MSD_lowRequestNewName(defname);
      MSD_highNameCopySolid(name, defname);
      s = MSD_lowFindSolidByName(defname);
      MSD_lowTranslate(s, disp[0], disp[1], disp[2]);
      vecplus(disp, dmove, disp);
   }
   return(TRUE);
}

int MSD_highNameNCopySolidSpiral(char *name, real Xi, real Yi, real Zi, real Xd,
                                 real Yd, real Zd, real dth, real dmv, int n)
{
   vector omove, dmove;

   makevec(omove, Xi, Yi, Zi, 1.0);
   makevec(dmove, Xd, Yd, Zd, 1.0);
   normalize(dmove);
   return(MSD_lowNameNCopySolidSpiral(name, omove, dmove, dth, dmv, n));
}

int MSD_lowNameNCopySolidSpiral(char *name, vector cpos, vector vect, real dth, real dmv, int n)
{
   SPTYPE s;
   char   defname[30];
   vector disp;
   real   th;
   int    i;

   veczer(disp);
   for (th = 0.0, i = 0; i < n; ++i, th += dth)
   {
      MSD_lowRequestNewName(defname);
      MSD_highNameCopySolid(name, defname);
      s = MSD_lowFindSolidByName(defname);
      MSD_lowRotateSolidAxis(s, cpos, vect, th);
      vecpe(vect, disp, disp, dmv);
      MSD_lowTranslate(s, disp[0], disp[1], disp[2]);
   }
   return(TRUE);
}

void MSD_lowRequestNewName(char *name)
{
   sprintf(name, "obj_%d", ++defonum);
}
