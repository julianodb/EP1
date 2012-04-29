/* 
  PROJETO USPDesigner
  MODULO: LOW (Operadores Basicos)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: lowlists.cpp
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
#include "mancommd.h"

/* funcao para execucao de listsolid por comando */
#ifdef __Extended_Version
void MSD_execListSolid(void)
{
   int sn, sw;

   while (sscanf(restbuf, "%d %d", &sn, &sw) != 2)
   {
      printf("list: Sid opcao");
      if (!lineins("? "))
      {
         return;
      }
   }
   MSD_highListSolid(sn, sw);
}

#endif

int MSD_highListSolid(Id sn, int sw)
{
   SPTYPE s;

   if ((s = MSD_getSolid(sn)) == SNIL)
   {
      fprintf(stderr, "list: solid %d not found\n", sn);
      return(ERROR);
   }
   MSD_lowListSolid(stdout, s, sw);
   return(SUCCESS);
}

void MSD_lowListSolid(FILE *file, SPTYPE s, int sw)
{
   DPTYPE d;
   FPTYPE f;
   VPTYPE v;
   EPTYPE e;
   HPTYPE l3;

   if (s == SNIL)
   {
      fprintf(file, "<empty>\n");
      return;
   }

   fprintf(file, "solid %d:\n", SolSolidNo(s));

   fprintf(file, "faces:\n");
   for (AllShellsSolid(s, d))
   {
      fprintf(file, "shell %d:\n", SheShellNo(d));
      for (AllFacesShell(d, f))
      {
         MSD_lowListFace(file, f, sw);
      }
   }
   if (sw > 1)
   {
      fprintf(file, "vertices:\n");
      for (AllShellsSolid(s, d))
      {
         fprintf(file, "shell: %d:\n", SheShellNo(d));
         for (AllVerticesShell(d, v))
         {
            fprintf(file, "%d <%5.1f, %5.1f, %5.1f>", VerVertexNo(v),
                    VerVCoord(v)[0],
                    VerVCoord(v)[1],
                    VerVCoord(v)[2]);
            if (sw > 3)
            {
               if ((l3 = VerVEdge(v)) != HNIL)
               {
                  do
                  {
                     e = HalEdg(l3);
                     fprintf(file, l3 == EdgHe1(e) ? " +" : " -");
                     fprintf(file, "<%d,%d>", VerVertexNo(HalVtx(EdgHe1(e))),
                             VerVertexNo(HalVtx(EdgHe2(e))));
                  } while ((l3 = HalNxt(mate(l3))) != VerVEdge(v));
               }
            }
            fprintf(file, "\n");
         }
      }
   }
   if (sw > 100)
   {
      fprintf(file, "edges:\n");
      for (AllShellsSolid(s, d))
      {
         fprintf(file, "shell %d:\n", SheShellNo(d));
         for (AllEdgesShell(d, e))
         {
            fprintf(file, "v1, v2, f1, f2 = <%d, %d, %d, %d>\n",
                    VerVertexNo(HalVtx(EdgHe1(e))),
                    VerVertexNo(HalVtx(EdgHe2(e))),
                    FacFaceNo(LooLFace(HalWLoop(EdgHe1(e)))),
                    FacFaceNo(LooLFace(HalWLoop(EdgHe2(e)))));
         }
      }
   }
}

void MSD_lowListFace(FILE *file, FPTYPE f, int sw)
{
   LPTYPE l;
   VPTYPE v;
   EPTYPE e;
   HPTYPE l3;
   int    i, breakline;
   vector eq;

   breakline = sw > 2 ? 3 : 8;
   fprintf(file, "face no. %d: ", FacFaceNo(f));

   for (AllLoopsFace(f, l))
   {
      i  = 0;
      l3 = LooLEdg(l);
      do
      {
         v = HalVtx(l3);
         if ((sw > 2) && ((e = HalEdg(l3)) != ENIL))
         {
            if (l3 == EdgHe1(e))
            {
               fprintf(file, "%d -(+(%d,%d))-> ", VerVertexNo(v),
                       VerVertexNo(HalVtx(EdgHe1(e))),
                       VerVertexNo(HalVtx(EdgHe2(e))));
            }
            else
            {
               fprintf(file, "%d -(-(%d,%d))-> ", VerVertexNo(v),
                       VerVertexNo(HalVtx(EdgHe1(e))),
                       VerVertexNo(HalVtx(EdgHe2(e))));
            }
         }
         else
         {
            fprintf(file, "%d --> ", VerVertexNo(v));
         }
         if ((++i % breakline) == 0)
         {
            fprintf(file, "\n               ");
         }
      } while ((l3 = HalNxt(l3)) != LooLEdg(l));

      if (l == FacFLOut(f))
      {
         fprintf(file, "<outer>\n");
      }
      else
      {
         fprintf(file, "<inner>\n");
      }

      MSD_lowFaceEq(l, eq);
      fprintf(file, " <%f, %f, %f, %f>\n", eq[0], eq[1], eq[2], eq[3]);
      fprintf(file, "                ");
   }
   fprintf(file, "\n");
}
