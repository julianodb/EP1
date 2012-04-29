/* 
  PROJETO USPDesigner
  MODULO: HIE (Estrutura Hierarquica)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: hieaxstr.cpp
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
#include "mensagem.h"
#include "memvirtu.h"
#include "lowparam.h"
#include "lowmacro.h"
#include "lowsolid.h"
#include "vectorop.h"
#include "eulerops.h"
#include "mancommd.h"
#include "disdispl.h"
#include "hiegroup.h"

#ifdef __Extended_Version
void MSD_execNameTransformUsingAxis(void)
{
   char  aname[30];
   float desl;

   while (2 != sscanf(restbuf, "%s %f", aname, &desl))
   {
      printf("TransformUsingAxis: axis desl\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highNameTransformUsingAxis(aname, desl);
}

#endif

int MSD_highNameTransformUsingAxis(char *name, real desl)
{
   int an;

   if ((an = MSD_getAxisIdFromName(name)) == -1)
   {
      fprintf(stderr, MEN_NaoEncontrouAxisNome,
              NAM_TransformUsingAxis, name);
      return(ERROR);
   }
   return(MSD_highTransformUsingAxis(an, desl));
}

#ifdef __Extended_Version
void MSD_execTransformUsingAxis(void)
{
   float desl;
   int   an;

   while (2 != sscanf(restbuf, "%d %f", &an, &desl))
   {
      printf("TransformUsingAxis: an desl\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highTransformUsingAxis(an, desl);
}

#endif

int MSD_highTransformUsingAxis(int an, real desl)
{
   APTYPE aptr;

   if ((aptr = MSD_getAxis(an)) == ANIL)
   {
      fprintf(stderr, MEN_NaoEncontrouAxisId,
              NAM_TransformUsingAxis, an);
      return(ERROR);
   }
   return(MSD_lowTransformUsingAxis(aptr, desl));
}

int MSD_lowTransformUsingAxis(APTYPE aptr, real desl)
{
   vector disp;

   switch (AxsMode(aptr))
   {
       case AxisOfRotation:
       MSD_lowRotateUsingAxis(aptr, AxsCenter(aptr), AxsVector(aptr), desl);
       break;

       case AxisOfTranslation:
       vecesc(disp, AxsVector(aptr), desl);
       MSD_lowTranslateUsingAxis(aptr, disp);
       break;
   }
   return(SUCCESS);
}

void MSD_lowTranslateUsingAxis(APTYPE aptr, vector disp)
{
   SPTYPE optr;
   GPTYPE gptr;
   APTYPE aptt;

   if (AxsType(aptr) == SOLID)
   {
      for (AllSolids(optr))
      {
         if (SolOAxs(optr) == aptr)
         {
            MSD_lowTranslate(optr, disp[0], disp[1], disp[2]);
         }
      }
   }

   if (AxsType(aptr) == GROUP)
   {
      for (AllGroup(gptr))
      {
         if (GrpGAxs(gptr) == aptr)
         {
            for (AllSolids(optr))
            {
               if (MSD_lowIsAncestorGroup(gptr, SolGroup(optr)))
               {
                  MSD_lowTranslate(optr, disp[0], disp[1], disp[2]);
               }
            }
         }
      }
   }

   for (AllAxis(aptt))
   {
      if (MSD_lowIsAncestorAxis(aptr, aptt))
      {
         MSD_lowMoveAxis(aptt, disp);
      }
   }
}

void MSD_lowRotateUsingAxis(APTYPE aptr, vector center, vector vec, real th)
{
   SPTYPE optr;
   GPTYPE gptr;
   APTYPE aptt;
   matrix rot;

   th *= PI / 180.0;
   if (AxsType(aptr) == SOLID)
   {
      for (AllSolids(optr))
      {
         if (SolOAxs(optr) == aptr)
         {
            MSD_lowTranslate(optr, -center[0], -center[1], -center[2]);
            matident(rot);
            rotmat(vec, th, rot);
            MSD_lowTransformation(optr, rot);
            MSD_lowTranslate(optr, center[0], center[1], center[2]);
         }
      }
   }

   if (AxsType(aptr) == GROUP)
   {
      for (AllGroup(gptr))
      {
         if (GrpGAxs(gptr) == aptr)
         {
            for (AllSolids(optr))
            {
               if (MSD_lowIsAncestorGroup(gptr, SolGroup(optr)))
               {
                  MSD_lowTranslate(optr, -center[0], -center[1], -center[2]);
                  matident(rot);
                  rotmat(vec, th, rot);
                  MSD_lowTransformation(optr, rot);
                  MSD_lowTranslate(optr, center[0], center[1], center[2]);
               }
            }
         }
      }
   }

   for (AllAxis(aptt))
   {
      if (MSD_lowIsAncestorAxis(aptr, aptt))
      {
         MSD_lowRotateAxis(aptt, center, vec, th);
      }
   }
}
