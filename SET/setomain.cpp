/* 
  PROJETO USPDesigner
  MODULO: SET (Operadores Booleanos)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: setomain.cpp
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
#define _BOOLEAN_
#include <stdio.h>
#include <math.h>
#include "memvirtu.h"
#include "lowparam.h"
#include "lowmacro.h"
#include "lowsolid.h"
#include "lowgeome.h"
#include "eulerops.h"
#include "shpshape.h"
#include "sethmain.h"

extern int  NumRecordsVM[10];
extern char *ArqVirtualMemory[];
SPTYPE      sola;
SPTYPE      solb;

/* outline of the set operations algorithm */
int MSD_lowSetOp(SPTYPE a, SPTYPE b, Id sn, SPTYPE *res, int op)
{
   VPTYPE v;
   DPTYPE d;
   int    i, flag, code;

#ifdef TRACE_BOOLEAN
   if ((trace = fopen("BOOLEAN.TXT", "w")) == (FILE *)NIL)
   {
      fprintf(stderr, "ERROR: nao conseguiu abrir arquivo de trace/n");
      return(ERROR);
   }
#endif

   CommandCount++;
   sola = a;
   solb = b;

#ifdef __VIRTUAL_MEM
   /*
    *  Fecha temporariamente areas de memoria de Group e Axis.
    */
   close_v_array(VirtualMemoryBlock[GROUP]);
   close_v_array(VirtualMemoryBlock[AXIS]);

   /*
    *  Abre a area de memoria relativa a analise de Shell!
    */
   if (MSD_lowSetOpInitiate0() == ERROR)
   {
      fprintf(stderr, "setop: nao conseguiu abrir arquivos de memoria virtual!\n");
      return(ERROR);
   }
#endif

   MSD_lowSetNormal(a, FALSE);
   MSD_lowSetNormal(b, FALSE);
   contshella = contshellb = 0;
   for (AllShellsSolid(a, d))
   {
      SetASheShell(contshella) = d;
      v    = SheSVerts(d);
      flag = 0;
      while (flag == 0)
      {
         if ((code = MSD_lowSolVtxContainment(b, v)) == 0)
         {
            if ((v = VerNextV(v)) == VNIL)
            {
               flag = 1;
            }
         }
         else
         {
            flag = 1;
         }
      }
      SetASheSign(contshella) = code;
      contshella++;
   }
   for (AllShellsSolid(b, d))
   {
      SetBSheShell(contshellb) = d;
      v    = SheSVerts(d);
      flag = 0;
      while (flag == 0)
      {
         if ((code = MSD_lowSolVtxContainment(a, v)) == 0)
         {
            if ((v = VerNextV(v)) == VNIL)
            {
               flag = 1;
            }
         }
         else
         {
            flag = 1;
         }
      }
      SetBSheSign(contshellb) = code;
      contshellb++;
   }

#ifdef TRACE_BOOLEAN
   fprintf(trace, "*** SOLID A - shells\n");
   for (i = 0; i < contshella; i++)
   {
      fprintf(trace, "*** shell - %2d\n", SheShellNo(SetASheShell(i)));
      fprintf(trace, "    code  - %2d\n", SetASheSign(i));
   }
   fprintf(trace, "*** SOLID B - shells\n");
   for (i = 0; i < contshellb; i++)
   {
      fprintf(trace, "*** shell - %2d\n", SheShellNo(SetBSheShell(i)));
      fprintf(trace, "    code  - %2d\n", SetBSheSign(i));
   }
#endif

#ifdef __VIRTUAL_MEM
   /*
    *  Fecha temporariamente a area de memoria relativa a analise de Shells!
    */

   close_v_array(SET01VirtualArray);
#endif

   maxf = MSD_getMaxFaceId(a);
   maxv = MSD_getMaxVertexId(a);
   maxd = MSD_getMaxShellId(a);
   //  MSD_lowModifyIds(b, maxd, maxf, maxv) ;
   maxf = MSD_getMaxFaceId(b);
   maxv = MSD_getMaxVertexId(b);
   maxd = MSD_getMaxShellId(b);

   fprintf(stderr, "1");

#ifdef __VIRTUAL_MEM
   /*
    *  Abre a area de memoria relativa a analise Vertex-Vertex
    */
   if (MSD_lowSetOpInitiate1() == ERROR)
   {
      fprintf(stderr, "setop: 1 - nao conseguiu abrir arquivos de memoria virtual!\n");
      return(ERROR);
   }

   /*
    *  Abre a area de memoria relativa a analise Face-Vertex
    */
   if (MSD_lowSetOpInitiate2() == ERROR)
   {
      fprintf(stderr, "setop: 2 - nao conseguiu abrir arquivos de memoria virtual!\n");
      return(ERROR);
   }
#endif

   MSD_lowSetOpGenerate(a, b);
   fprintf(stderr, "!");
#ifdef TRACE_BOOLEAN1
   fprintf(trace, "*** SOLID A\n");
   MSD_lowListSolid(trace, a, 101);
   fprintf(trace, "*** SOLID B\n");
   MSD_lowListSolid(trace, b, 101);
#endif

   fprintf(stderr, "2");

   MSD_lowSetOpClassify(op);
   fprintf(stderr, "!");

#ifdef TRACE_BOOLEAN2
   fprintf(trace, "*** SOLID A\n");
   MSD_lowListSolid(trace, a, 101);
   fprintf(trace, "*** SOLID B\n");
   MSD_lowListSolid(trace, b, 101);
#endif

   nfaca = nfacb = 0;
   if (nedga != 0)
   {
      fprintf(stderr, "3");
      MSD_lowSetOpConnect();
      fprintf(stderr, "!");
#ifdef TRACE_BOOLEAN3
      fprintf(trace, "*** SOLID A\n");
      MSD_lowListSolid(trace, a, 101);
      fprintf(trace, "*** SOLID B\n");
      MSD_lowListSolid(trace, b, 101);
#endif
   }

   fprintf(stderr, "4");

#ifdef __VIRTUAL_MEM
   if ((SET01VirtualArray = (VACB *)open_v_array("SETOP01.TMP", 10))
       == (VACB *)NIL)
   {
      fprintf(stderr, "setop: nao conseguiu abrir arquivos de memoria virtual!\n");
      return(ERROR);
   }
#endif

   *res = MSD_lowSetOpFinish(a, b, sn, op);

#ifdef __VIRTUAL_MEM
   MSD_lowSetOpTerminate5();
#endif

   for (flag = i = 0; i < contshella + nfaca; i++)
   {
      if (SetASheSign(i) != -2)
      {
         flag = 1;
      }
   }

   if (flag == 1)
   {
      MSD_lowSoftRemove(a);
   }
   for (flag = i = 0; i < contshellb + nfacb; i++)
   {
      if (SetBSheSign(i) != -2)
      {
         flag = 1;
      }
   }

   if (flag == 1)
   {
      MSD_lowSoftRemove(b);
   }

   if (*res != SNIL)
   {
      MSD_lowSetNormal(*res, TRUE);
      MSD_lowSetEdgeAngle(*res);
      MSD_lowSetInfo(*res);
   }

   fprintf(stderr, "!\n");
#ifdef TRACE_BOOLEAN
   fprintf(trace, "*** SOLID C\n");
   MSD_lowListSolid(trace, *res, 101);
#endif

#ifdef TRACE_BOOLEAN
   fclose(trace);
#endif

#ifdef __VIRTUAL_MEM
   MSD_lowSetOpTerminate0();
   VirtualMemoryBlock[GROUP] = (VACB *)open_v_array(ArqVirtualMemory[GROUP],
                                                    NumRecordsVM[GROUP]);
   VirtualMemoryBlock[AXIS] = (VACB *)open_v_array(ArqVirtualMemory[AXIS],
                                                   NumRecordsVM[AXIS]);
#endif

   return(SUCCESS);
}

#ifdef __VIRTUAL_MEM
int MSD_lowSetOpInitiate0(void)
{
   init_v_array("SETOP01.TMP", sizeof(SFF), ' ');
   if ((SET01VirtualArray = (VACB *)open_v_array("SETOP01.TMP", 20))
       == (VACB *)NIL)
   {
      fprintf(stderr, "*** Boolean --> sem memoria!\n");
      return(ERROR);
   }
   return(SUCCESS);
}

void MSD_lowSetOpTerminate0(void)
{
   close_v_array(SET01VirtualArray);
   remove("SETOP01.TMP");
}

int MSD_lowSetOpInitiate1(void)
{
   init_v_array("SETOP03.TMP", sizeof(SONVV), ' ');
   if ((SET03VirtualArray = (VACB *)open_v_array("SETOP03.TMP", 20))
       == (VACB *)NIL)
   {
      fprintf(stderr, "*** Boolean --> sem memoria!\n");
      return(ERROR);
   }
   return(SUCCESS);
}

void MSD_lowSetOpTerminate1(void)
{
   close_v_array(SET03VirtualArray);
   remove("SETOP03.TMP");
}

int MSD_lowSetOpInitiate2(void)
{
   init_v_array("SETOP04.TMP", sizeof(SONVF), ' ');
   if ((SET04VirtualArray = (VACB *)open_v_array("SETOP04.TMP", 20))
       == (VACB *)NIL)
   {
      fprintf(stderr, "*** Boolean --> sem memoria!\n");
      return(ERROR);
   }
   return(SUCCESS);
}

void MSD_lowSetOpTerminate2(void)
{
   close_v_array(SET04VirtualArray);
   remove("SETOP04.TMP");
}

int MSD_lowSetOpInitiate3(void)
{
   init_v_array("SETOP06.TMP", sizeof(NBR), ' ');
   if ((SET06VirtualArray = (VACB *)open_v_array("SETOP06.TMP", 10))
       == (VACB *)NIL)
   {
      fprintf(stderr, "*** Boolean --> sem memoria!\n");
      return(ERROR);
   }
   return(SUCCESS);
}

void MSD_lowSetOpTerminate3(void)
{
   close_v_array(SET06VirtualArray);
   remove("SETOP06.TMP");
}

int MSD_lowSetOpInitiate4(void)
{
   init_v_array("SETOP07.TMP", sizeof(SECTORS), ' ');
   init_v_array("SETOP08.TMP", sizeof(NB), ' ');
   if ((SET07VirtualArray = (VACB *)open_v_array("SETOP07.TMP", 10))
       == (VACB *)NIL)
   {
      fprintf(stderr, "*** Boolean --> sem memoria!\n");
      return(ERROR);
   }
   if ((SET08VirtualArray = (VACB *)open_v_array("SETOP08.TMP", 10))
       == (VACB *)NIL)
   {
      fprintf(stderr, "*** Boolean --> sem memoria!\n");
      return(ERROR);
   }
   return(SUCCESS);
}

void MSD_lowSetOpTerminate4(void)
{
   close_v_array(SET07VirtualArray);
   close_v_array(SET08VirtualArray);
   remove("SETOP07.TMP");
   remove("SETOP08.TMP");
}

int MSD_lowSetOpInitiate5(void)
{
   init_v_array("SETOP10.TMP", sizeof(EPTYPE), ' ');
   if ((SET10VirtualArray = (VACB *)open_v_array("SETOP10.TMP", 10))
       == (VACB *)NIL)
   {
      fprintf(stderr, "*** Boolean --> sem memoria!\n");
      return(ERROR);
   }
   return(SUCCESS);
}

void MSD_lowSetOpTerminate5(void)
{
   close_v_array(SET10VirtualArray);
   remove("SETOP10.TMP");
}

#endif
