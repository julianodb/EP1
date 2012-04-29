/* 
  PROJETO USPDesigner
  MODULO: MEM (Administrador de Memoria)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: memsolid.cpp
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
#include "eulerops.h"

#ifdef __VIRTUAL_MEM
extern unsigned nodesize[];

int  NumRecordsVM[10] = { 10, 10, 50, 50, 200, 200, 100, 10, 10, 10 };
char *ArqVirtualMemory[] =
{
   "VIRSOLID.TMP", "VIRSHELL.TMP", "VIRFACE_.TMP", "VIRLOOP.TMP",
   "VIREDGE_.TMP", "VIRHALFE.TMP", "VIRVERTX.TMP", "VIRAXIS.TMP",
   "VIRGROUP.TMP",              0,
};

void MSD_highIniciateMemVirtual(void)
{
   FILE *fp;

   if ((fp = fopen("uspdesin.mem", "r")) != (FILE *)NULL)
   {
      fscanf(fp, "%d", &NumRecordsVM[0]);
      fscanf(fp, "%d", &NumRecordsVM[1]);
      fscanf(fp, "%d", &NumRecordsVM[2]);
      fscanf(fp, "%d", &NumRecordsVM[3]);
      fscanf(fp, "%d", &NumRecordsVM[4]);
      fscanf(fp, "%d", &NumRecordsVM[5]);
      fscanf(fp, "%d", &NumRecordsVM[6]);
      fscanf(fp, "%d", &NumRecordsVM[7]);
      fscanf(fp, "%d", &NumRecordsVM[8]);
      fclose(fp);
   }

   MSD_lowIniciateMemVirtual(SOLID);
   MSD_lowIniciateMemVirtual(SHELL);
   MSD_lowIniciateMemVirtual(FACE);
   MSD_lowIniciateMemVirtual(LOOP);
   MSD_lowIniciateMemVirtual(EDGE);
   MSD_lowIniciateMemVirtual(HALFEDGE);
   MSD_lowIniciateMemVirtual(VERTEX);
   MSD_lowIniciateMemVirtual(GROUP);
   MSD_lowIniciateMemVirtual(AXIS);

   init_v_array("EULEROPS.TMP", sizeof(EulerOp), ' ');
   EulerOpsArray = (VACB *)open_v_array("EULEROPS.TMP", 500);
}

void MSD_highTerminateMemVirtual(void)
{
   MSD_lowTerminateMemVirtual(SOLID);
   MSD_lowTerminateMemVirtual(SHELL);
   MSD_lowTerminateMemVirtual(FACE);
   MSD_lowTerminateMemVirtual(LOOP);
   MSD_lowTerminateMemVirtual(EDGE);
   MSD_lowTerminateMemVirtual(HALFEDGE);
   MSD_lowTerminateMemVirtual(VERTEX);
   MSD_lowTerminateMemVirtual(GROUP);
   MSD_lowTerminateMemVirtual(AXIS);

   close_v_array(EulerOpsArray);
   remove("EULEROPS.TMP");
}

void MSD_lowIniciateMemVirtual(int what)
{
   init_v_array(ArqVirtualMemory[what], nodesize[what], ' ');
   VirtualMemoryBlock[what] = (VACB *)open_v_array(ArqVirtualMemory[what],
                                                   NumRecordsVM[what]);
   ArrayTop[what] = ArrayBottom[what] = 0;
}

long MSD_getMemVirtual(int what)
{
   long FreeMemVirtuCell;

   FreeMemVirtuCell = ArrayTop[what];
   if (ArrayTop[what] == ArrayBottom[what])
   {
      ArrayTop[what]++;
      ArrayBottom[what]++;
   }
   else
   {
      switch (what)
      {
          case SOLID:
          ArrayTop[what] = SolNxt(ArrayTop[what]);
          break;

          case SHELL:
          ArrayTop[what] = SheNextD(ArrayTop[what]);
          break;

          case FACE:
          ArrayTop[what] = FacNextF(ArrayTop[what]);
          break;

          case LOOP:

          ArrayTop[what] = LooNextL(ArrayTop[what]);
          break;

          case EDGE:
          ArrayTop[what] = EdgNextE(ArrayTop[what]);
          break;

          case HALFEDGE:
          ArrayTop[what] = HalNextH(ArrayTop[what]);
          break;

          case VERTEX:
          ArrayTop[what] = VerNextV(ArrayTop[what]);
          break;

          case GROUP:
          ArrayTop[what] = GrpNextG(ArrayTop[what]);
          break;

          case AXIS:
          ArrayTop[what] = AxsNextA(ArrayTop[what]);
          break;
      }
   }
   return(FreeMemVirtuCell);
}

void MSD_putMemVirtual(long FreePointer, int what)
{
   switch (what)
   {
       case SOLID:
       SolNxt(FreePointer) = ArrayTop[what];
       break;

       case SHELL:
       SheNextD(FreePointer) = ArrayTop[what];
       break;

       case FACE:
       FacNextF(FreePointer) = ArrayTop[what];
       break;

       case LOOP:
       LooNextL(FreePointer) = ArrayTop[what];
       break;

       case EDGE:
       EdgNextE(FreePointer) = ArrayTop[what];
       break;

       case HALFEDGE:
       HalNextH(FreePointer) = ArrayTop[what];
       break;

       case VERTEX:
       VerNextV(FreePointer) = ArrayTop[what];
       break;

       case GROUP:
       GrpNextG(FreePointer) = ArrayTop[what];
       break;

       case AXIS:
       AxsNextA(FreePointer) = ArrayTop[what];
       break;
   }
   ArrayTop[what] = FreePointer;
}

void MSD_lowTerminateMemVirtual(int what)
{
   close_v_array(VirtualMemoryBlock[what]);
   remove(ArqVirtualMemory[what]);
}

#endif
