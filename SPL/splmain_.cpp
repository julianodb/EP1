/* 
  PROJETO USPDesigner
  MODULO: SPL (Corte de Solido)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: splmain_.cpp
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
#include <math.h>
#include "mensagem.h"
#include "memvirtu.h"
#include "lowparam.h"
#include "lowmacro.h"
#include "lowsolid.h"
#include "mancommd.h"
#include "shpshape.h"
#include "vectorop.h"
#include "genfunc_.h"
#include "disdispl.h"
#include "splsplit.h"
#include "eulerops.h"

extern int  NumRecordsVM[10];
extern char *ArqVirtualMemory[];

void MSD_execNameSplit(void)
{
   char  name1[15], name2[15], name3[15];
   float vx, vy, vz, vd;

   while (7 != sscanf(restbuf, "%s %s %s %f %f %f %f", name3, name1, name2, &vx, &vy, &vz, &vd))
   {
      printf("split: Name New1 New2 pa pb pc pd\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highNameSplit(name3, name1, name2, vx, vy, vz, vd);
}

void MSD_highNameSplit(char *nameo, char *name1, char *name2, real a, real b, real c, real d)
{
   Id sn, s1;

   if ((sn = MSD_getSolidIdFromName(nameo)) == -1)
   {
      fprintf(stderr, MEN_NaoEncontrouSolidNome, NAM_SplitSolid, nameo);
      return;
   }
   if (MSD_getSolidIdFromName(name1) != -1)
   {
      fprintf(stderr, MEN_SolidoJaExiste, NAM_SplitSolid, name1);
      return;
   }
   if (MSD_getSolidIdFromName(name2) != -1)
   {
      fprintf(stderr, MEN_SolidoJaExiste, NAM_SplitSolid, name2);
      return;
   }
   if ((s1 = MSD_highSplit(sn, a, b, c, d)) != -1)
   {
      if (MSD_getSolid(s1) != SNIL)
      {
         MSD_highName(s1, name1, SOLID);
      }
      if (MSD_getSolid(s1 + 1) != SNIL)
      {
         MSD_highName(s1 + 1, name1, SOLID);
      }
   }
}

#ifdef __Extended_Version
void MSD_execSplit(void)
{
   int   sn;
   float vx, vy, vz, vd;

   while (5 != sscanf(restbuf, "%d %f %f %f %f", &sn, &vx, &vy, &vz, &vd))
   {
      printf("split: Sid pa pb pc pd\n");
      if (!lineins("? "))
      {
         return;
      }
   }
   sn = MSD_highSplit(sn, vx, vy, vz, vd);
   printf("Id = %d & Id = %d\n", sn, sn + 1);
}

#endif

Id MSD_highSplit(Id sn, real a, real b, real c, real d)
{
   SPTYPE optr;
   SPTYPE Above;
   SPTYPE Below;
   vector vec;
   Id     snm;


   if ((optr = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, MEN_NaoEncontrouSolido, NAM_SplitSolid, sn);
      return(-1);
   }
   snm = MSD_getMaxSolidId();
   makevec(vec, a, b, c, d);
   if (MSD_lowSplit(optr, vec, snm, snm + 1, &Above, &Below) == SUCCESS)
   {
      if (Above != SNIL)
      {
         MSD_lowDisplayMonitor('o', Above, GNIL);
      }
      if (Below != SNIL)
      {
         MSD_lowDisplayMonitor('o', Below, GNIL);
      }
      return(snm);
   }
   return(-1);
}

/* outline the splitting algorithm */
int MSD_lowSplit(SPTYPE s, vector SP, Id sn1, Id sn2, SPTYPE *Above, SPTYPE *Below)
{
   DPTYPE d;
   FPTYPE f;

#ifdef TRACE_SPLIT
   if ((trace = fopen("SPLIT.TXT", "w")) == (FILE *)NIL)
   {
      fprintf(stderr, MEN_NaoAbriuArquivo, NAM_SplitSolid, "SPLIT.TXT");
      return(0);
   }
#endif

#ifdef __VIRTUAL_MEM
   MSD_lowSplitInitiate();
#endif

   CommandCount++;
   SP[3] = SP[3] / sqrt(dot(SP, SP));
   normalize(SP);
   *Above        = *Below = SNIL;
   maxv          = MSD_getMaxVertexId(s);
   maxf          = MSD_getMaxFaceId(s);
   spl_contshell = 0;
   for (AllShellsSolid(s, d))
   {
      for (AllFacesShell(d, f))
      {
         MSD_lowFaceEq(FacFLOut(f), FacFeq(f));
      }
      SplShellShell(spl_contshell) = d;
      SplShellSign(spl_contshell)  =
         comp(MSD_lowDistancePlanePoint(SP, SheSVerts(d)), 0.0, EPS);
      spl_contshell++;
   }

   MSD_lowSplitGenerate(s, SP);
#ifdef TRACE_SPLIT1
   fprintf(trace, "*** SOLID A\n");
   MSD_lowListSolid(trace, s, 101);
#endif

   MSD_lowSplitClassify(SP);
#ifdef TRACE_SPLIT2
   fprintf(trace, "*** SOLID A\n");
   MSD_lowListSolid(trace, s, 101);
#endif

   if (spl_nedge != 0)
   {
      MSD_lowSplitConnect();
#ifdef TRACE_SPLIT3
      fprintf(trace, "*** SOLID A\n");
      MSD_lowListSolid(trace, s, 101);
#endif
   }

   MSD_lowSplitFinish(s, sn1, sn2, Above, Below);
#ifdef TRACE_SPLIT
   fprintf(trace, "*** SOLID A\n");
   MSD_lowListSolid(trace, *Above, 101);
   MSD_lowListSolid(trace, *Below, 101);
   fclose(trace);
#endif

   if (*Above != SNIL)
   {
      MSD_lowSetNormal(*Above, TRUE);
      MSD_lowSetEdgeAngle(*Above);
      MSD_lowSetInfo(*Above);
   }
   if (*Below != SNIL)
   {
      MSD_lowSetNormal(*Below, TRUE);
      MSD_lowSetEdgeAngle(*Below);
      MSD_lowSetInfo(*Below);
   }

#ifdef __VIRTUAL_MEM
   MSD_lowSplitTerminate();
#endif
   return(SUCCESS);
}

#ifdef __VIRTUAL_MEM
void MSD_lowSplitInitiate(void)
{
   /*
    *  Fecha temporariamente areas de memoria de Group e Axis.
    */
   close_v_array(VirtualMemoryBlock[GROUP]);
   close_v_array(VirtualMemoryBlock[AXIS]);

   init_v_array("SPLIT01.TMP", sizeof(SF), ' ');
   init_v_array("SPLIT02.TMP", sizeof(VPTYPE), ' ');
   init_v_array("SPLIT03.TMP", sizeof(spl_NBR), ' ');
   if ((SPL01VirtualArray = (VACB *)open_v_array("SPLIT01.TMP", 10)) == (VACB *)NIL)
   {
      fprintf(stderr, "split: nao conseguiu abrir arquivos de memoria virtual!\n");
      return;
   }
   if ((SPL02VirtualArray = (VACB *)open_v_array("SPLIT02.TMP", 10)) == (VACB *)NIL)
   {
      fprintf(stderr, "split: nao conseguiu abrir arquivos de memoria virtual!\n");
      return;
   }
   if ((SPL03VirtualArray = (VACB *)open_v_array("SPLIT03.TMP", 10)) == (VACB *)NIL)
   {
      fprintf(stderr, "split: nao conseguiu abrir arquivos de memoria virtual!\n");
      return;
   }
}

void MSD_lowSplitTerminate(void)
{
   close_v_array(SPL01VirtualArray);
   close_v_array(SPL02VirtualArray);
   close_v_array(SPL03VirtualArray);
   remove("SPLIT01.TMP");
   remove("SPLIT02.TMP");
   remove("SPLIT03.TMP");

   VirtualMemoryBlock[GROUP] = (VACB *)open_v_array(ArqVirtualMemory[GROUP],
                                                    NumRecordsVM[GROUP]);
   VirtualMemoryBlock[AXIS] = (VACB *)open_v_array(ArqVirtualMemory[AXIS],
                                                   NumRecordsVM[AXIS]);
}

#endif
