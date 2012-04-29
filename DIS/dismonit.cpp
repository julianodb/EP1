/* 
  PROJETO USPDesigner
  MODULO: DIS (Hidden Line Elimination)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: dismonit.cpp
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
#include <string.h>
#include "mensagem.h"
#include "memvirtu.h"
#include "lowparam.h"
#include "lowmacro.h"
#include "lowsolid.h"
#include "hiegroup.h"
#include "disdispl.h"
#include "mancommd.h"
#include "vectorop.h"

void MSD_execMonitor(void)
{
   char name1[15];

   while (1 != sscanf(restbuf, "%s", name1))
   {
      printf("%s: Nome da camera/ON/OFF\n", NAM_Monitor);
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highMonitor(name1);
}

int MSD_highMonitor(char *name)
{
   CTYPE *cptr;

   strlower((unsigned char *)name);
   if (!strcmp(name, "on"))
   {
      return(MSD_lowMonitor((CTYPE *)NIL, -1));
   }
   if (!strcmp(name, "off"))
   {
      return(MSD_lowMonitor((CTYPE *)NIL, -2));
   }
   if ((cptr = MSD_lowFindCameraByName(name)) != (CTYPE *)NIL)
   {
      return(MSD_lowMonitor(cptr, -3));
   }
   fprintf(stderr, MEN_ParametrosIncorretos, NAM_Monitor);
   return(FALSE);
}

int MSD_lowMonitor(CTYPE *cptr, int flag)
{
   switch (flag)
   {
       case - 1:
       if (monicptr == (CTYPE *)NIL)
       {
          fprintf(stderr, MEN_NenhumaCamaraMonitora,
                  NAM_Monitor);
          return(FALSE);
       }
       monisw = TRUE;
       break;

       case - 2:
       if (monicptr == (CTYPE *)NIL)
       {
          fprintf(stderr, MEN_NenhumaCamaraMonitora,
                  NAM_Monitor);
          return(FALSE);
       }
       monisw = FALSE;
       break;

       case - 3:
       monicptr = camptr = cptr;
       monisw   = TRUE;
       break;
   }
   return(TRUE);
}

void MSD_lowDisplayMonitor(int t, SPTYPE optr, GPTYPE gptr)
{
   if (monisw)
   {
      camptr = monicptr;
      switch (t)
      {
          case 'a':
          MSD_lowDisplayMonitorAxis();
          break;

          case 'o':
          MSD_lowDisplayMonitorSolid(optr);
          break;

          case 'g':
          MSD_lowDisplayMonitorGroup(gptr);
          break;
      }
   }
}

void MSD_lowDisplayMonitorSolid(SPTYPE op)
{
   SPTYPE optr;

   for (AllSolids(optr))
   {
      SolDsp(optr) = (op == optr);
   }

#if !defined (__UNIX) && !defined (_Windows)
   MSD_lowSetBreakInterruption();
#endif

   switch (monicptr->hid)
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
       break;
   }

#if !defined (__UNIX) && !defined (_Windows)
   MSD_lowSetBreakResident();
#endif
}

void MSD_lowDisplayMonitorGroup(GPTYPE gptr)
{
   SPTYPE optr;

   for (AllSolids(optr))
   {
      SolDsp(optr) = MSD_lowIsAncestorGroup(gptr, SolGroup(optr));
   }

#if !defined (__UNIX) && !defined (_Windows)
   MSD_lowSetBreakInterruption();
#endif

   switch (monicptr->hid)
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
       break;
   }

#if !defined (__UNIX) && !defined (_Windows)
   MSD_lowSetBreakResident();
#endif
}

void MSD_lowDisplayMonitorAxis(void)
{
#if !defined (__UNIX) && !defined (_Windows)
   MSD_lowSetBreakInterruption();
#endif

   switch (monicptr->hid)
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
       break;
   }

#if !defined (__UNIX) && !defined (_Windows)
   MSD_lowSetBreakResident();
#endif
}
