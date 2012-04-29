/* 
  PROJETO USPDesigner
  MODULO: SET (Operadores Booleanos)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: setopfin.cpp
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
#include <stdlib.h>
#include <math.h>
#include "memvirtu.h"
#include "lowparam.h"
#include "lowmacro.h"
#include "lowsolid.h"
#include "eulerops.h"
#include "sethmain.h"
#include "splsplit.h"

int MSD_lowCheckSOFBorderFace(FPTYPE f, int flag)
{
   int i;

   for (i = 0; i < (flag == 0 ? nfaca : nfacb); i++)
   {
      if (flag == 0)
      {
         if (f == SetSonfa(i))
         {
            return(TRUE);
         }
      }
      else
      if (f == SetSonfb(i))
      {
         return(TRUE);
      }
   }
   return(FALSE);
}

int MSD_lowCheckSOFShell(FPTYPE f, FPTYPE ff, int flag)
{
   FPTYPE f2;
   LPTYPE l;
   HPTYPE he;

   if (f == ff)
   {
      return(0);
   }
   if (MSD_lowCheckSOFBorderFace(f, flag))
   {
      return(1);
   }
   if (!FaceBitNOK1(f))
   {
      return(0);
   }
   SetFaceBitOK1(f);

   for (AllLoopsFace(f, l))
   {
      he = LooLEdg(l);
      do
      {
         f2 = LooLFace(HalWLoop(mate(he)));
         if (FaceBitNOK1(f2))
         {
            if (MSD_lowCheckSOFShell(f2, ff, flag) == 1)
            {
               return(1);
            }
         }
      } while ((he = HalNxt(he)) != LooLEdg(l));
   }
   return(0);
}

int MSD_lowCheckSetOpFinish(LPTYPE l, int flag)
{
   SPTYPE s;
   DPTYPE d;
   FPTYPE f;
   FPTYPE ff;
   HPTYPE he;

   ff = LooLFace(l);
   d  = FacFShell(ff);
   s  = SheSSolids(d);
   for (AllShellsSolid(s, d))
   {
      for (AllFacesShell(d, f))
      {
         SetFaceBitNOK1(f);
      }
   }

   d  = FacFShell(ff);
   he = LooLEdg(l);
   do
   {
      f = LooLFace(HalWLoop(mate(he)));
      if (FaceBitNOK1(f))
      {
         if (MSD_lowCheckSOFShell(f, ff, flag) == 1)
         {
            return(1);
         }
      }
   } while ((he = HalNxt(he)) != LooLEdg(l));
   return(0);
}

/* Generation of the result */
SPTYPE MSD_lowSetOpFinish(SPTYPE a, SPTYPE b, Id sn, int op)
{
   SPTYPE res;
   DPTYPE d;
   LPTYPE l;
   int    i, j, inda, indb, flag, index;


#ifdef TRACE_BOOLEAN
   fprintf(trace, "\n\n\n");
   fprintf(trace, "*************************************************\n");
   fprintf(trace, "*                                               *\n");
   fprintf(trace, "*                  SETOPFINISH                  *\n");
   fprintf(trace, "*                                               *\n");
   fprintf(trace, "*************************************************\n");
   fprintf(trace, "\n");
   if (op == USP_UNION)
   {
      fprintf(trace, "*** Operacao UNIAO\n");
   }
   if (op == USP_INTERSECT)
   {
      fprintf(trace, "*** Operacao INTERSECCAO\n");
   }
   if (op == USP_DIFFERENCE)
   {
      fprintf(trace, "*** Operacao DIFERENCA\n");
   }
#endif

   if (op == USP_UNION)
   {
      inda = 0;
      indb = 0;
   }
   if (op == USP_INTERSECT)
   {
      inda = nfaca;
      indb = nfacb;
   }
   if (op == USP_DIFFERENCE)
   {
      inda = 0;
      indb = nfacb;
   }
   res = SNIL;

   if (op == USP_DIFFERENCE)
   {
      for (AllShellsSolid(b, d))
      {
         MSD_lowRevert(d);
      }
   }

   for (i = 0; i < nfaca; ++i)
   {
      l = InnerLoop(SetSonfa(i));
      if ((MSD_lowCheckSetOpFinish(l, 0) == 1) &&
          (MSD_lowCheckSetOpFinish(FacFLOut(SetSonfa(i)), 0) == 1))
      {
         SetSonfa(nfaca + i) = MSD_lowMFKRH(SetSonfa(i), l, maxf++);
      }
      else
      {
         SetSonfa(nfaca + i) = MSD_lowMSFKR(l, maxf++, maxd++);
      }

      l = InnerLoop(SetSonfb(i));
      if ((MSD_lowCheckSetOpFinish(l, 1) == 1) &&
          (MSD_lowCheckSetOpFinish(FacFLOut(SetSonfb(i)), 1) == 1))
      {
         SetSonfb(nfacb + i) = MSD_lowMFKRH(SetSonfb(i), l, maxf++);
      }
      else
      {
         SetSonfb(nfacb + i) = MSD_lowMSFKR(l, maxf++, maxd++);
      }

      SetASheShell(contshella + i) = FacFShell(SetSonfa(nfaca + i));
      SetBSheShell(contshellb + i) = FacFShell(SetSonfb(nfacb + i));
   }

   for (i = 0; i < nfaca; ++i)
   {
      for (j = 0; j < contshella + nfaca; j++)
      {
         if (SetASheShell(j) == FacFShell(SetSonfa(i + inda)))
         {
            SetASheSign(j) = -2;
         }
      }
      for (j = 0; j < contshellb + nfaca; j++)
      {
         if (SetBSheShell(j) == FacFShell(SetSonfb(i + indb)))
         {
            SetBSheSign(j) = -2;
         }
      }

      for (j = 0; j < contshella + nfaca; j++)
      {
         index = i + (inda == 0 ? nfaca : 0);
         if (SetASheShell(j) == FacFShell(SetSonfa(index)))
         {
            SetASheSign(j) = 0;
         }
      }
      for (j = 0; j < contshellb + nfaca; j++)
      {
         index = i + (indb == 0 ? nfacb : 0);
         if (SetBSheShell(j) == FacFShell(SetSonfb(index)))
         {
            SetBSheSign(j) = 0;
         }
      }
   }

#ifdef TRACE_BOOLEAN
   fprintf(trace, "\n");
   fprintf(trace, "*** setopfinish *** - inda = %2i\n", inda);
   fprintf(trace, "*** setopfinish *** - indb = %2i\n", indb);
   fprintf(trace, "*** setopfinish *** - nfaca = %2i\n", 2 * nfaca);
   fprintf(trace, "*** setopfinish *** - nfacb = %2i\n", 2 * nfacb);
   fprintf(trace, "*** estrutura sonfa ***\n");
   for (i = 0; i < 2 * nfaca; i++)
   {
      MSD_lowFaceEq(FacFLOut(SetSonfa(i)), FacFeq(SetSonfa(i)));
      trace_f(SetSonfa(i));
      fprintf(trace, "*** shell - %2d\n", SheShellNo(FacFShell(SetSonfa(i))));
   }
   fprintf(trace, "*** estrutura sonfb ***\n");
   for (i = 0; i < 2 * nfacb; i++)
   {
      MSD_lowFaceEq(FacFLOut(SetSonfb(i)), FacFeq(SetSonfb(i)));
      trace_f(SetSonfb(i));
      fprintf(trace, "*** shell - %2d\n", SheShellNo(FacFShell(SetSonfb(i))));
   }

   fprintf(trace, "*** SOLID A - shells\n");
   for (i = 0; i < contshella + nfaca; i++)
   {
      fprintf(trace, "*** shell - %2d\n", SheShellNo(SetASheShell(i)));
      fprintf(trace, "    code  - %2d\n", SetASheSign(i));
   }
   fprintf(trace, "*** SOLID B - shells\n");
   for (i = 0; i < contshellb + nfacb; i++)
   {
      fprintf(trace, "*** shell - %2d\n", SheShellNo(SetBSheShell(i)));
      fprintf(trace, "    code  - %2d\n", SetBSheSign(i));
   }
#endif

   for (i = 0; i < nfaca; ++i)
   {
      for (j = flag = 0; j < i; j++)
      {
         if (FacFShell(SetSonfa(i + inda)) == FacFShell(SetSonfa(j + inda)))
         {
            flag = 1;
         }
      }
      if (flag == 0)
      {
         res = MSD_lowDetach(a, sn, FacFShell(SetSonfa(i + inda)), -1, 0, 0);
      }
   }

   for (i = 0; i < nfacb; ++i)
   {
      for (j = flag = 0; j < i; j++)
      {
         if (FacFShell(SetSonfb(i + indb)) == FacFShell(SetSonfb(j + indb)))
         {
            flag = 1;
         }
      }
      if (flag == 0)
      {
         res = MSD_lowDetach(b, sn, FacFShell(SetSonfb(i + indb)), -1, 0, 0);
      }
   }

   for (i = 0; i < nfaca; ++i)
   {
      if (FacFShell(SetSonfa(i + inda)) == FacFShell(SetSonfb(i + indb)))
      {
         MSD_lowKFMRH(SetSonfa(i + inda), SetSonfb(i + indb));
      }
      else
      {
         MSD_lowKSFMR(SetSonfa(i + inda), SetSonfb(i + indb));
      }
      MSD_lowLoopGlue(SetSonfa(i + inda));
   }


   for (i = 0; i < contshella; i++)
   {
      if (SetASheSign(i) != -2)
      {
         if ((op == USP_UNION) || (op == USP_DIFFERENCE))
         {
            if (SetASheSign(i) == -1)
            {
               SetASheSign(i) = -2;
               res            = MSD_lowDetach(a, sn, SetASheShell(i), -1, 0, 0);
            }
         }
         else
         {
            if (op == USP_INTERSECT)
            {
               if (SetASheSign(i) == 1)
               {
                  SetASheSign(i) = -2;
                  res            = MSD_lowDetach(a, sn, SetASheShell(i), -1, 0, 0);
               }
            }
         }
      }
   }

   for (i = 0; i < contshellb; i++)
   {
      if (SetBSheSign(i) != -2)
      {
         if (op == USP_UNION)
         {
            if (SetBSheSign(i) == -1)
            {
               SetBSheSign(i) = -2;
               res            = MSD_lowDetach(b, sn, SetBSheShell(i), -1, 0, 0);
            }
         }
         else
         {
            if ((op == USP_INTERSECT) || (op == USP_DIFFERENCE))
            {
               if (SetASheSign(i) == 1)
               {
                  SetBSheSign(i) = -2;
                  res            = MSD_lowDetach(b, sn, SetBSheShell(i), -1, 0, 0);
               }
            }
         }
      }
   }

   return(res);
}
