/* 
  PROJETO USPDesigner
  MODULO: MAN (Modulo Principal)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: manmainp.cpp
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
#define _DVS_
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#ifndef __UNIX_
 #include <malloc.h>
 #include <stdlib.h>
 #ifndef _Windows
  #include <graphics.h>
 #else
  #include <windows.h>
 #endif
#endif
#include "mensagem.h"
#include "memvirtu.h"
#include "lowparam.h"
#include "curvgeom.h"
#include "lowsolid.h"
#include "eulerops.h"
#include "shpshape.h"
#include "sethmain.h"
#include "splsplit.h"
#include "hiegroup.h"
#include "animatio.h"
#include "disdispl.h"
#include "genfunc_.h"
#include "gphgraph.h"
#include "analise_.h"
#include "basrov__.h"
#include "rndround.h"
#include "prop.h"
#include "mancommd.h"

int main(int argc, char **argv)
{
   char    com[20];
   COMTYPE *p;
   FILE    *fp;

   objary       = SNIL;
   OpHead       = EuNIL;
   SetTrace     = FALSE;
   error        = FALSE;
   Gdoinversion = TRUE;
   instream     = stdin;

   g_open();
   maninit();

   printf("******************* USP Designer *******************\n");
   printf("*** Mecatronica/EPUSP - (C)Copyright 1990 - 2008 ***\n");
   printf("*** para Pentium IV ----------------- versao 1.3 ***\n");

#if !defined (_Windows)
   MSD_lowSetBreakInterruption();
#endif

   if ((fp = fopen("autodemo.cad", "r")) != (FILE *)NIL)
   {
      instream = fp;
   }

   for ( ; ;)
   {
#ifdef __UNIX_
      GGGraphicFlush();
#endif
      if (lineins("USPDesigner:"))
      {
         sscanf(restbuf, "%s", com);
         skipnext(&restbuf);
         strupper((unsigned char *)com);
         for (p = comtable; *p->comname; ++p)
         {
            if (!strncmp(p->comname, com, strlen(com)))
            {
               break;
            }
         }
         (*p->comfunc)();
      }
   }
}

void maninit(void)
{
#ifdef __VIRTUAL_MEM
   if (objary != SNIL)
   {
      MSD_highTerminateMemVirtual();
   }
#endif

   maxs         = maxf = maxv = 1;
   maxd         = 0;
   objary       = SNIL;
   OpHead       = EuNIL;
   OpHeadRedo   = EuNIL;
   SetTrace     = FALSE;
   error        = FALSE;
   Gdoinversion = TRUE;
   Gdodirect    = TRUE;
   instream     = stdin;

   g_clear();
   lncolor          = defltcol = framecol = 0;
   Draw             = TRUE;
   lntype           = SOLID_LINE;
   abseps           = 0.005;
   releps           = 0.0001;
   OutputPlanoCorte = FALSE;
#ifndef __UNIX_
   xyratio = 0.7;
#else
   xyratio = 1.0;
#endif
   dwindbox[0]        = windbox[0] = 0.0;
   dwindbox[1]        = windbox[1] = 0.0;
   dwindbox[2]        = windbox[2] = 1.0;
   angsmooth          = 0.4;
   camused            = grpused = axsused = objused = 0;
   vtxused            = edgused = facused = 0;
   camptr             = camary = (CTYPE *)NIL;
   grpary             = GNIL;
   axsary             = ANIL;
   objary             = SNIL;
   def_grpptr         = GNIL;
   defonum            = 0;
   monisw             = FALSE;
   monicptr           = (CTYPE *)NIL;
   instream           = stdin;
   dspfrptr           = dspfwptr = (FILE *)NIL;
   breakflg           = FALSE;
   ListaEscoamento    = (LOPType *)NIL;
   NumListaEscoamento = 0;
#ifdef __VIRTUAL_MEM
   MSD_highIniciateMemVirtual();
#endif
}

void MSD_readFileExecute(void)
{
   char name[30];

   while (1 != sscanf(restbuf, "%s", name))
   {
      printf("Arquivo ");
      if (!lineins("? "))
      {
         instream = stdin;
         return;
      }
   }
   MSD_executeFileCommand(name);
}

void MSD_executeFileCommand(char *name)
{
   FILE *fp;
   char namecad[30];

   if (instream != stdin)
   {
      fclose(instream);
      instream = stdin;
   }
   if ((fp = fopen(name, "r")) == (FILE *)NIL)
   {
      strcpy(namecad, name);
      strcat(namecad, ".cad");
      if ((fp = fopen(namecad, "r")) == (FILE *)NIL)
      {
         printf("Nao conseguiu abrir o arquivo %s !\n", name);
         instream = stdin;
         return;
      }
   }
   instream = fp;
}

void endfunc(void)
{
   if (instream == stdin)
   {
      g_close();
      printf("Sistema encerrado!\n");
#ifdef __VIRTUAL_MEM
      MSD_highTerminateMemVirtual();
#endif
      exit(0);
   }
   else
   {
      fclose(instream);
      instream = stdin;
   }
}

void comerror(void)
{
   printf("Comando Incorreto!\n");
}
