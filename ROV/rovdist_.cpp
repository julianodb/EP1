/* 
  PROJETO USPDesigner
  MODULO: ROV (Criacao de Formas Especificas)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: rovdist_.cpp
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
#include <math.h>
#include "memvirtu.h"
#include "lowmacro.h"
#include "lowparam.h"
#include "lowsolid.h"
#include "vectorop.h"
#include "analise_.h"
#include "basrov__.h"

/****************************************************************************
*
* Determina a distancia parametrica a partir de uma ponto ate um plano.
*
*   cc:  ponto que desejamos determinar a distancia
*   dir: vetor normal do plano
*   pp:  ponto do plano
*
****************************************************************************/
real MSD_determinaDistanciaParametrica(vector cc, vector dir, vector pp)
{
   real d;

   d = -dot(dir, pp);
   return(-(d + dot(dir, cc)) / dot(dir, dir));
}

real MSD_determinaDimensaoPrincipalBarra(SPTYPE s, vector dir)
{
   DPTYPE d;
   VPTYPE v;
   VPTYPE vmax;
   VPTYPE vmin;
   real   pmax = -9999, pmin = 9999, dis;
   vector pc;

   d = SolSShells(s);
   makevec(pc, 0.0, 0.0, 0.0, 1.0);

   for (AllVerticesShell(d, v))
   {
      dis = MSD_determinaDistanciaParametrica(pc, dir, VerVCoord(v));
      if (pmax < dis)
      {
         pmax = dis;
         vmax = v;
      }
      if (pmin > dis)
      {
         pmin = dis;
         vmin = v;
      }
   }

   veccopy(pc, VerVCoord(vmax));
   dis = MSD_determinaDistanciaParametrica(VerVCoord(vmin), dir, pc);

   vecesc(pc, dir, dis);
   return(sqrt(dot(pc, pc)));
}

real MSD_determinaMaiorDimensao(SPTYPE s)
{
   switch (SolType(s))
   {
       case SOLIDO_TIPO_BARRA:
       switch (SolGeometria(s))
       {
           case BARRA_GEOM_CIRCULO:
           case BARRA_GEOM_QUADRADO:
           return(SolParam2(s));

           case BARRA_GEOM_RETANGULO:
           case BARRA_GEOM_ELIPSE:
           case BARRA_GEOM_PERFIL_L:
           return(SolParam3(s));

           case BARRA_GEOM_TRIANGULO:
           case BARRA_GEOM_PERFIL_I:
           case BARRA_GEOM_PERFIL_T:
           return(SolParam4(s));

           default:
           return(0.0);
       }

       case SOLIDO_TIPO_SOLID:
       return(0.0);

       case SOLIDO_TIPO_PLACA:
       return(0.0);

       default:
       return(0.0);
   }
}

real MSD_determinaDimensaoPrincipal(SPTYPE s, vector dir, real dim)
{
   SPTYPE sp;
   vector DirCalc;

   switch (SolType(s))
   {
       case SOLIDO_TIPO_BARRA:
       if ((sp = MSD_getSolid(SolPerfil(s))) == SNIL)
       {
          return(0.0);
       }
       cross(DirCalc, dir, FacFeq(MSD_getFace(sp, 1)));
       if (!vecnull(DirCalc, 0.001))
       {
          switch (SolGeometria(s))
          {
              case BARRA_GEOM_CIRCULO:
              case BARRA_GEOM_QUADRADO:
              case BARRA_GEOM_TRIANGULO:
              case BARRA_GEOM_RETANGULO:
              case BARRA_GEOM_ELIPSE:
              case BARRA_GEOM_PERFIL_L:
              case BARRA_GEOM_PERFIL_T:
              case BARRA_GEOM_PERFIL_I:
              return(MSD_determinaDimensaoPrincipalBarra(sp, DirCalc));
          }
       }
       else
       {
          switch (SolGeometria(s))
          {
              case BARRA_GEOM_CIRCULO:
              case BARRA_GEOM_QUADRADO:
              case BARRA_GEOM_TRIANGULO:
              case BARRA_GEOM_RETANGULO:
              case BARRA_GEOM_ELIPSE:
              case BARRA_GEOM_PERFIL_L:
              case BARRA_GEOM_PERFIL_T:
              case BARRA_GEOM_PERFIL_I:
              veccopy(DirCalc, FacFeq(MSD_getFace(sp, 1)));
              return(MSD_lowSolidDimensaoPrincipal(sp, DirCalc));
          }
       }
       break;

       case SOLIDO_TIPO_SOLID:
       return(dim);

       case SOLIDO_TIPO_PLACA:
       if (comp(dot(SolPrincDir(s), dir), 0.0, 0.001) == 0)
       {
          if ((sp = MSD_getSolid(SolPerfil(s))) == SNIL)
          {
             return(0.0);
          }
          return(MSD_determinaDimensaoPrincipalBarra(sp, dir));
       }
       else
       {
          if (SolParam1(s) > SolParam2(s))
          {
             return(SolParam2(s));
          }
          return(SolParam1(s));
       }
   }
   return(0.0);
}
