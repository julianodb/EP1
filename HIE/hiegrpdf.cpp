/* 
  PROJETO USPDesigner
  MODULO: HIE (Estrutura Hierarquica)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: hiegrpdf.cpp
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
#include "vectorop.h"
#include "mancommd.h"
#include "genfunc_.h"
#include "hiegroup.h"

void MSD_execManipulateGroup(void)
{
   int  ip;
   char gname[30], oname[30];

   for (ip = 0; ip == 0;)
   {
      switch (optin())
      {
          case 'd':
          if (1 == sscanf(restbuf, "%s", oname))
          {
             MSD_highNameDesassociateGroupElement(oname);
             ip = 1;
          }
          break;

          case 'm':
          if (2 == sscanf(restbuf, "%s %s", gname, oname))
          {
             MSD_highNameAssociateGroupElement(gname, oname);
             ip = 1;
          }
          break;

          case 'p':
          if (2 == sscanf(restbuf, "%s %s", gname, oname))
          {
             MSD_highNameTransferGroup(gname, oname);
             ip = 1;
          }
          break;

          case 'c':
          if (1 == sscanf(restbuf, "%s", gname))
          {
             MSD_highNameSetGroupCurrent(gname);
             ip = 1;
          }

          break;

          case 'e':
          if (1 == sscanf(restbuf, "%s", gname))
          {
             MSD_highNameRemoveGroup(gname);
             ip = 1;
          }
          break;

          case 'g':
          if (1 == sscanf(restbuf, "%s", gname))
          {
             MSD_highNameCreateSetGroup(gname, "FALSE");
             ip = 1;
          }
          break;

          case 'G':
          if (1 == sscanf(restbuf, "%s", gname))
          {
             MSD_highNameCreateSetGroup(gname, "TRUE");
             ip = 1;
          }
      }
      if (ip == 0)
      {
         printf("-cdegGmp Grupo Objeto/Grupo filho\n");
         if (!lineins("? "))
         {
            return;
         }
      }
   }
}
