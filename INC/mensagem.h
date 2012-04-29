/* 
  PROJETO USPDesigner
  MODULO: MEM (Virtual Memory Manipulation)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: mensagem.h
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

#ifndef EXTERN
 #ifdef _DVS_
  #define EXTERN
 #else
  #define EXTERN    extern
 #endif
#endif

#ifdef _DVS_
char *NAM_Area            = "AnaArea";
char *NAM_AreaProjetada   = "AnaAreaProjetada";
char *NAM_CentroCarena    = "AnaCentroCarena";
char *NAM_CentroGravidade = "AnaCentroGravidade";
char *NAM_DensAbs         = "AnaDensidadeAbsoluta";
char *NAM_DensRel         = "AnaDensidadeRelativa";
char *NAM_Massa           = "AnaMassa";
char *NAM_Inercia         = "AnaInercia";
char *NAM_Volume          = "AnaVolume";

char *NAM_RovCirculo   = "RovCirculo";
char *NAM_RovCone      = "RovCone";
char *NAM_RovCubo      = "RovCubo";
char *NAM_RovEsfera    = "RovEsfera";
char *NAM_RovPerfilI   = "RovPerfilI";
char *NAM_RovPerfilL   = "RovPerfilL";
char *NAM_RovPerfilT   = "RovPerfilT";
char *NAM_RovPiramide  = "RovPiramide";
char *NAM_RovPlaca     = "RovPlaca";
char *NAM_RovPrisma    = "RovPrisma";
char *NAM_RovQuadrado  = "RovQuadrado";
char *NAM_RovRetangulo = "RovRetangulo";
char *NAM_RovTriangulo = "RovTriangulo";
char *NAM_RovTranslate = "RovTranslate";
char *NAM_RovRotate    = "RovRotate";
char *NAM_DimPrinc     = "RovDimensaoPrincipal";
char *NAM_RovOrdena    = "RovOrdena";

char *NAM_AlteraCamaraCorrente        = "AltCamCor";
char *NAM_AlteraCamara                = "AltCam";
char *NAM_CopyCamara                  = "CpyCam";
char *NAM_GetCamaraCorrenteParametros = "GetCamCorPar";
char *NAM_ListCamara              = "LstCam";
char *NAM_CreateCamara            = "CrtCam";
char *NAM_RemoveCamara            = "DelCam";
char *NAM_RenameCamara            = "RenCam";
char *NAM_SetCamaraCorrente       = "SetCamCor";
char *NAM_TranslateCamaraCorrente = "TrnCamCor";

char *NAM_DisplayCoordinates = "DisCoo";
char *NAM_SetFrameColor      = "FrmCol";
char *NAM_SetDefaultColor    = "DftCol";
char *NAM_SetElementColor    = "ElmCol";
char *NAM_Display            = "Disp";
char *NAM_DisplayFile        = "DisFle";
char *NAM_DisplayEdge        = "DisEdg";
char *NAM_Monitor            = "Monit";

char *NAM_AddEulerOperator = "AddOp";
char *NAM_KEF           = "kef";
char *NAM_KEF2          = "kef2";
char *NAM_KEMR          = "kemr";
char *NAM_KEV           = "kev";
char *NAM_KEV2          = "kev2";
char *NAM_JVKE          = "jvke";
char *NAM_JEKV          = "jekv";
char *NAM_KFMRH         = "kfmrh";
char *NAM_KSFMR         = "ksfmr";
char *NAM_KVSF          = "kvsf";
char *NAM_MEF           = "mef";
char *NAM_MEF2          = "mef2";
char *NAM_MEKR          = "mekr";
char *NAM_MEKR2         = "mekr2";
char *NAM_MEV           = "mev";
char *NAM_SEMV          = "semv";
char *NAM_SVME          = "svme";
char *NAM_SVME2         = "svme2";
char *NAM_SVME3         = "svme3";
char *NAM_MFKRH         = "mfkrh";
char *NAM_MFKRH2        = "mfkrh2";
char *NAM_ModifyEulerOp = "ModEul";
char *NAM_MSFKR         = "msfkr";
char *NAM_MSFKR2        = "msfkr2";
char *NAM_MVSF          = "mvsf";
char *NAM_PrintEulerOp  = "PrtEul";
char *NAM_ScanEulerOp   = "ScnEul";
char *NAM_ApplyEulerOp  = "AppEul";
char *NAM_Color         = "Color";
char *NAM_Name          = "Nome";
char *NAM_GetHE         = "GetHe";
char *NAM_GetMate       = "GetMate";
char *NAM_GetNext       = "GetNext";
char *NAM_GetPrev       = "GetPrev";
char *NAM_DoMEV         = "DoMEV";
char *NAM_DoMEF         = "DoMEF";
char *NAM_DoNewLoc      = "DoNewVertexLocation";
char *NAM_DoPutTag      = "DoPutTag";
char *NAM_DoEvalEdge    = "DoEvaluateEdge";
char *NAM_AddLPoint     = "AddPoint";
char *NAM_RemLPoint     = "RemPoint";
char *NAM_EvalEdge      = "EvaluateEdge";
char *NAM_KillCTag      = "KillCurveTag";
char *NAM_KillPolyLine  = "KillPolyLine";
char *NAM_NewLine       = "NewLine";
char *NAM_NewPolyLine   = "NewPolyLine";
char *NAM_LengthPoly    = "LengthPoly";
char *NAM_PutCTag       = "PutCurveTag";
char *NAM_Round         = "Round";
char *NAM_RoundAll      = "RoundAll";

char *NAM_AssociaAxisElemento    = "AssAxs";
char *NAM_DesassociaAxisElemento = "DesAxs";
char *NAM_CopySolid               = "CpySol";
char *NAM_ListFaceAdjacency       = "LstFacAdj";
char *NAM_AssociaGrupoElemento    = "AssGrp";
char *NAM_DesassociaGrupoElemento = "DesGrp";
char *NAM_LoadSolido              = "LodSol";
char *NAM_RenameSolido            = "RenSol";
char *NAM_SaveSolido              = "SavSol";

char *NAM_AssociaAxisGroup        = "AssAxsGrp";
char *NAM_AssociaAxisSolid        = "AssAxsSol";
char *NAM_AssociaGroupGroup       = "AssGrpGrp";
char *NAM_AssociaGroupSolid       = "AssGrpSol";
char *NAM_CreateAxisOfTranslation = "CrtAxsTrn";
char *NAM_CreateAxisOfRotation    = "CrtAxsRot";
char *NAM_ListAxis              = "LstAxs";
char *NAM_MoveAxis              = "MovAxs";
char *NAM_RemoveAxis            = "RemAxs";
char *NAM_RenameAxis            = "RenAxs";
char *NAM_RotateAxis            = "RotAxs";
char *NAM_TransformUsingAxis    = "TrfAxs";
char *NAM_DesassociaAxisGroup   = "DesAxsGrp";
char *NAM_DesassociaAxisSolid   = "DesAxsSol";
char *NAM_DesassociaGroupGroup  = "DesGrpGrp";
char *NAM_DesassociaGroupSolid  = "DesGrpSol";
char *NAM_CreateGroup           = "CrtGrp";
char *NAM_CreateGroupSetCurrent = "CrtGrpCur";
char *NAM_CopyGroup             = "CpyGrp";
char *NAM_ListGroup             = "LstGrp";
char *NAM_MoveGroup             = "MovGrp";
char *NAM_RemoveOneGroup        = "RemOneGrp";
char *NAM_RemoveGroup           = "RemGrp";
char *NAM_RenameGroup           = "RenGrp";
char *NAM_RotateGroup           = "RotGrp";
char *NAM_SetCurrentGroup       = "SetCurGrp";
char *NAM_TransferGroup         = "TrnGrp";

char *NAM_AffineSolid     = "AffSol";
char *NAM_TransformSolid  = "TrfSol";
char *NAM_RotateSolid     = "RotSol";
char *NAM_RotateSolidAxis = "RotSolAxs";
char *NAM_ScaleSolid      = "SclSol";
char *NAM_TranslateSolid  = "TrnSol";
char *NAM_SetDifference   = "SetDif";
char *NAM_SetIntersection = "SetInt";
char *NAM_SetUnion        = "SetUni";
char *NAM_MakeCylinder    = "MakCyl";
char *NAM_MakeCone        = "MakCon";
char *NAM_MakePrism       = "MakPrm";
char *NAM_MakePyramid     = "MakPyr";
char *NAM_MakeRevolve     = "MakRev";
char *NAM_MakeSphere      = "MakSph";
char *NAM_MakeSweep       = "MakSwp";
char *NAM_MakeTorus       = "MakTor";
char *NAM_MakeCube        = "MakCub";
char *NAM_SplitSolid      = "Spl";
char *NAM_CleanSolid      = "clean";
char *NAM_Detach          = "detach";
char *NAM_Glue            = "glue";
char *NAM_MergeSolid      = "merge";
char *NAM_ModifyIds       = "modify";
char *NAM_NewLocation     = "newloc";
char *NAM_HardRemove      = "hard";
char *NAM_SoftRemove      = "soft";
char *NAM_Revert          = "revert";
char *NAM_RingMove        = "rmove";
char *NAM_AllRingMove     = "armove";

char *MEN_PerguntaNovoAntigo    = "%s: antigo novo\n";
char *MEN_PerguntaId            = "%s: id\n";
char *MEN_PerguntaNome          = "%s: nome\n";
char *MEN_CamaraParametros      = "%s: %s Ex Ey Ez Rx Ry Rz scala modo processo pontilhada paralelas\n";
char *MEN_CoordenadasParametros = "%s: -a comp cor\n-m x/y/z = const comp espacamento cor_eixo cor_malha\n";

char *MEN_NenhumaCamaraCorrente    = "%s: nenhuma camera corrente!\n";
char *MEN_CamaraNomeNaoEncontrada  = "%s: camera %s nao foi encontrada!\n";
char *MEN_CamaraIdNaoEncontrada    = "%s: camera %d nao foi encontrada!\n";
char *MEN_CamaraParametroIncorreto = "%s: parametros da camera estao incorretos!\n";
char *MEN_CamaraJaExiste           = "%s: camera %s ja existe!\n";
char *MEN_CamaraNomeCorrente       = "%s: camera %s e camera corrente!\n";
char *MEN_CamaraIdCorrente         = "%s: camera %d e camera corrente!\n";
char *MEN_NenhumaCamaraMonitora    = "%s: nenhuma camera monitora!\n";
char *MEN_CorInvalida              = "%s: cor invalida!\n";
char *MEN_ParametrosIncorretos     = "%s: parametros incorretos!\n";
char *MEN_ElementoNaoEncontrado    = "%s: elemento %s nao foi encontrado!\n";
char *MEN_NaoAbriuArquivo          = "%s: nao conseguiu abrir arquivo %s!\n";
char *MEN_NaoEncontrouAresta       = "%s: nao encontrou aresta %d em %d!\n";
char *MEN_NaoPossuiMemoria         = "%s: nao possui memoria suficiente!\n";
char *MEN_MSDAbortou               = "%s: programa abortado!\n";
char *MEN_NaoEncontrouSolido       = "%s: nao encontrou solido %d!\n";
char *MEN_NaoEncontrouFace         = "%s: nao encontrou face %d em solido %s!\n";
char *MEN_NaoEncontrouArestaV      = "%s: nao encontrou aresta %d-%d em solido %s!\n";
char *MEN_NaoEncontrouArestaF      = "%s: nao encontrou aresta %d-%d (%d-%d) em solido %d!\n";
char *MEN_NaoEncontrouVertice      = "%s: nao encontrou vertice %d em solido %s!\n";
char *MEN_NaoArestaPonte           = "%s: %d-%d nao e aresta-ponte ou aresta-strut, solido %d!\n";
char *MEN_NaoVerticeTerminal       = "%s: vertice %d nao e terminal, solido %d!\n";
char *MEN_ArestaCircular           = "%s: %d-%d e aresta circular, solido %d!\n";
char *MEN_FaceSemLoop              = "%s: face %d nao pode possuir loop, solido %d!\n";
char *MEN_FaceSemVertice           = "%s: face %d deve possuir apenas um vertice, solido %d!\n";
char *MEN_VerticesEmLoopsDistintos = "%s: vertices %d e %d devem estar em loops distintos, solido %d!\n";

char *MEN_NaoEncontrouAxisNome      = "%s: nao encontrou eixo %s!\n";
char *MEN_NaoEncontrouGroupNome     = "%s: nao encontrou grupo %s!\n";
char *MEN_SolidoJaExiste            = "%s: solido %s ja existe!\n";
char *MEN_NaoEncontrouSolidNome     = "%s: nao encontrou solido %s!\n";
char *MEN_NaoEncontrouGroupId       = "%s: nao encontrou grupo %d!\n";
char *MEN_NaoEncontrouAxisId        = "%s: nao encontrou eixo %d!\n";
char *MEN_PonteirosNulos            = "%s: ponteiros nulos!\n";
char *MEN_AxisJaExiste              = "%s: eixo %s ja existe!\n";
char *MEN_GroupJaExiste             = "%s: grupo %s ja existe!\n";
char *MEN_GrupoCorrenteEliminado    = "%s: grupo corrente pertence ao grupo eliminado!\n";
char *MEN_NaoPossuiAncestral        = "%s: grupo corrente nao possui ancestral!\n";
char *MEN_GroupAncestralGroup       = "%s: grupo %d e ancestral do group %d!\n";
char *MEN_FatorEscalaNulo           = "%s: fator de escala e nulo!\n";
char *MEN_NaoEncontrouShell         = "%s: nao encontrou shell %d em solido %d!\n";
char *MEN_MemoriaInvalida           = "%s: memoria %d nao e valida!\n";
char *MEN_NaoEncontrouTag           = "%s: nao encontrou tag %d em solido %d!\n";
char *MEN_NumeroDeSegmentosInvalido = "%s: numero de segmentos deve ser maior que 2!\n";
char *MEN_OperacaoNaoRealizada      = "%s: operacao nao foi realizada!\n";
char *MEN_NaoEncontrouEdgeNo        = "%s: nao encontrou aresta %d em solido %d!\n";
#else
EXTERN char *NAM_Area;                        /* = "AnaArea"*/
EXTERN char *NAM_AreaProjetada;               /* = "AnaAreaProjetada"*/
EXTERN char *NAM_CentroCarena;                /* = "AnaCentroCarena"*/
EXTERN char *NAM_CentroGravidade;             /* = "AnaCentroGravidade"*/
EXTERN char *NAM_DensAbs;                     /* = "AnaDensidadeAbsoluta"*/
EXTERN char *NAM_DensRel;                     /* = "AnaDensidadeRelativa"*/
EXTERN char *NAM_Massa;                       /* = "AnaMassa"*/
EXTERN char *NAM_Inercia;                     /* = "AnaInercia"*/
EXTERN char *NAM_Volume;                      /* = "AnaVolume"*/

EXTERN char *NAM_RovCirculo;                  /* = "RovCirculo"*/
EXTERN char *NAM_RovCone;                     /* = "RovCone"*/
EXTERN char *NAM_RovCubo;                     /* = "RovCubo"*/
EXTERN char *NAM_RovEsfera;                   /* = "RovEsfera"*/
EXTERN char *NAM_RovPerfilI;                  /* = "RovPerfilI"*/
EXTERN char *NAM_RovPerfilL;                  /* = "RovPerfilL"*/
EXTERN char *NAM_RovPerfilT;                  /* = "RovPerfilT"*/
EXTERN char *NAM_RovPiramide;                 /* = "RovPiramide"*/
EXTERN char *NAM_RovPlaca;                    /* = "RovPlaca"*/
EXTERN char *NAM_RovPrisma;                   /* = "RovPrisma"*/
EXTERN char *NAM_RovQuadrado;                 /* = "RovQuadrado"*/
EXTERN char *NAM_RovRetangulo;                /* = "RovRetangulo"*/
EXTERN char *NAM_RovTriangulo;                /* = "RovTriangulo"*/
EXTERN char *NAM_RovTranslate;                /* = "RovTranslate"*/
EXTERN char *NAM_RovRotate;                   /* = "RovRotate"*/
EXTERN char *NAM_DimPrinc;                    /* = "RovDimensaoPrincipal"*/
EXTERN char *NAM_RovOrdena;                   /* = "RovOrdena"*/

EXTERN char *NAM_AlteraCamaraCorrente;        /* = "AltCamCor*/
EXTERN char *NAM_AlteraCamara;                /* = "AltCam*/
EXTERN char *NAM_CopyCamara;                  /* = "CpyCam*/
EXTERN char *NAM_GetCamaraCorrenteParametros; /* = "GetCamCorPar*/
EXTERN char *NAM_ListCamara;                  /* = "LstCam*/
EXTERN char *NAM_CreateCamara;                /* = "CrtCam*/
EXTERN char *NAM_RemoveCamara;                /* = "DelCam*/
EXTERN char *NAM_RenameCamara;                /* = "RenCam*/
EXTERN char *NAM_SetCamaraCorrente;           /* = "SetCamCor*/
EXTERN char *NAM_TranslateCamaraCorrente;     /* = "TrnCamCor*/

EXTERN char *NAM_DisplayCoordinates;          /* = "DisCoo*/
EXTERN char *NAM_SetFrameColor;               /* = "FrmCol*/
EXTERN char *NAM_SetDefaultColor;             /* = "DftCol*/
EXTERN char *NAM_SetElementColor;             /* = "ElmCol*/
EXTERN char *NAM_Display;                     /* = "Disp*/
EXTERN char *NAM_DisplayFile;                 /* = "DisFle*/
EXTERN char *NAM_DisplayEdge;                 /* = "DisEdg*/
EXTERN char *NAM_Monitor;                     /* = "Monit*/

EXTERN char *NAM_AddEulerOperator;            /* = "AddOp*/
EXTERN char *NAM_KEF;                         /* = "kef*/
EXTERN char *NAM_KEF2;                        /* = "kef2*/
EXTERN char *NAM_KEMR;                        /* = "kemr*/
EXTERN char *NAM_KEV;                         /* = "kev*/
EXTERN char *NAM_KEV2;                        /* = "kev2*/
EXTERN char *NAM_JVKE;                        /* = "jvke*/
EXTERN char *NAM_JEKV;                        /* = "jekv*/
EXTERN char *NAM_KFMRH;                       /* = "kfmrh*/
EXTERN char *NAM_KSFMR;                       /* = "ksfmr*/
EXTERN char *NAM_KVSF;                        /* = "kvsf*/
EXTERN char *NAM_MEF;                         /* = "mef*/
EXTERN char *NAM_MEF2;                        /* = "mef2*/
EXTERN char *NAM_MEKR;                        /* = "mekr*/
EXTERN char *NAM_MEKR2;                       /* = "mekr2*/
EXTERN char *NAM_MEV;                         /* = "mev*/
EXTERN char *NAM_SEMV;                        /* = "semv*/
EXTERN char *NAM_SVME;                        /* = "svme*/
EXTERN char *NAM_SVME2;                       /* = "svme2*/
EXTERN char *NAM_SVME3;                       /* = "svme3*/
EXTERN char *NAM_MFKRH;                       /* = "mfkrh*/
EXTERN char *NAM_MFKRH2;                      /* = "mfkrh2*/
EXTERN char *NAM_ModifyEulerOp;               /* = "ModEul*/
EXTERN char *NAM_MSFKR;                       /* = "msfkr*/
EXTERN char *NAM_MSFKR2;                      /* = "msfkr2*/
EXTERN char *NAM_MVSF;                        /* = "mvsf*/
EXTERN char *NAM_PrintEulerOp;                /* = "PrtEul*/
EXTERN char *NAM_ScanEulerOp;                 /* = "ScnEul*/
EXTERN char *NAM_ApplyEulerOp;                /* = "AppEul*/
EXTERN char *NAM_Color;                       /* = "Color*/
EXTERN char *NAM_Name;                        /* = "Nome"*/
EXTERN char *NAM_GetHE;                       /* = "GetHe"*/
EXTERN char *NAM_GetMate;                     /* = "GetMate"*/
EXTERN char *NAM_GetNext;                     /* = "GetNext"*/
EXTERN char *NAM_GetPrev;                     /* = "GetPrev"*/
EXTERN char *NAM_DoMEV;                       /* = "DoMEV"*/
EXTERN char *NAM_DoMEF;                       /* = "DoMEF"*/
EXTERN char *NAM_DoNewLoc;                    /* = "DoNewVertexLocation"*/
EXTERN char *NAM_DoPutTag;                    /* = "DoPutTag"*/
EXTERN char *NAM_DoEvalEdge;                  /* = "DoEvaluateEdge"*/
EXTERN char *NAM_AddLPoint;                   /* = "AddPoint"*/
EXTERN char *NAM_RemLPoint;                   /* = "RemPoint"*/
EXTERN char *NAM_EvalEdge;                    /* = "EvaluateEdge"*/
EXTERN char *NAM_KillCTag;                    /* = "KillCurveTag"*/
EXTERN char *NAM_KillPolyLine;                /* = "KillPolyLine"*/
EXTERN char *NAM_NewLine;                     /* = "NewLine"*/
EXTERN char *NAM_NewPolyLine;                 /* = "NewPolyLine"*/
EXTERN char *NAM_LengthPoly;                  /* = "LengthPoly"*/
EXTERN char *NAM_PutCTag;                     /* = "PutCurveTag"*/
EXTERN char *NAM_Round;                       /* = "Round"*/
EXTERN char *NAM_RoundAll;                    /* = "RoundAll"*/

EXTERN char *NAM_AssociaAxisElemento;         /* = "AssAxs*/
EXTERN char *NAM_DesassociaAxisElemento;      /* = "DesAxs*/
EXTERN char *NAM_CopySolid;                   /* = "CpySol*/
EXTERN char *NAM_ListFaceAdjacency;           /* = "LstFacAdj*/
EXTERN char *NAM_AssociaGrupoElemento;        /* = "AssGrp*/
EXTERN char *NAM_DesassociaGrupoElemento;     /* = "DesGrp*/
EXTERN char *NAM_LoadSolido;                  /* = "LodSol*/
EXTERN char *NAM_RenameSolido;                /* = "RenSol*/
EXTERN char *NAM_SaveSolido;                  /* = "SavSol*/

EXTERN char *NAM_AssociaAxisGroup;            /* = "AssAxsGrp*/
EXTERN char *NAM_AssociaAxisSolid;            /* = "AssAxsSol*/
EXTERN char *NAM_AssociaGroupGroup;           /* = "AssGrpGrp*/
EXTERN char *NAM_AssociaGroupSolid;           /* = "AssGrpSol*/
EXTERN char *NAM_CreateAxisOfTranslation;     /* = "CrtAxsTrn*/
EXTERN char *NAM_CreateAxisOfRotation;        /* = "CrtAxsRot*/
EXTERN char *NAM_ListAxis;                    /* = "LstAxs*/
EXTERN char *NAM_MoveAxis;                    /* = "MovAxs*/
EXTERN char *NAM_RemoveAxis;                  /* = "RemAxs*/
EXTERN char *NAM_RenameAxis;                  /* = "RenAxs*/
EXTERN char *NAM_RotateAxis;                  /* = "RotAxs*/
EXTERN char *NAM_TransformUsingAxis;          /* = "TrfAxs*/
EXTERN char *NAM_DesassociaAxisGroup;         /* = "DesAxsGrp*/
EXTERN char *NAM_DesassociaAxisSolid;         /* = "DesAxsSol*/
EXTERN char *NAM_DesassociaGroupGroup;        /* = "DesGrpGrp*/
EXTERN char *NAM_DesassociaGroupSolid;        /* = "DesGrpSol*/
EXTERN char *NAM_CreateGroup;                 /* = "CrtGrp*/
EXTERN char *NAM_CreateGroupSetCurrent;       /* = "CrtGrpCur*/
EXTERN char *NAM_CopyGroup;                   /* = "CpyGrp*/
EXTERN char *NAM_ListGroup;                   /* = "LstGrp*/
EXTERN char *NAM_MoveGroup;                   /* = "MovGrp*/
EXTERN char *NAM_RemoveOneGroup;              /* = "RemOneGrp*/
EXTERN char *NAM_RemoveGroup;                 /* = "RemGrp*/
EXTERN char *NAM_RenameGroup;                 /* = "RenGrp*/
EXTERN char *NAM_RotateGroup;                 /* = "RotGrp*/
EXTERN char *NAM_SetCurrentGroup;             /* = "SetCurGrp*/
EXTERN char *NAM_TransferGroup;               /* = "TrnGrp*/

EXTERN char *NAM_AffineSolid;                 /* = "AffSol*/
EXTERN char *NAM_TransformSolid;              /* = "TrfSol*/
EXTERN char *NAM_RotateSolid;                 /* = "RotSol*/
EXTERN char *NAM_RotateSolidAxis;             /* = "RotSolAxs*/
EXTERN char *NAM_ScaleSolid;                  /* = "SclSol*/
EXTERN char *NAM_TranslateSolid;              /* = "TrnSol*/

EXTERN char *NAM_SetDifference;               /* = "SetDif*/
EXTERN char *NAM_SetIntersection;             /* = "SetInt*/
EXTERN char *NAM_SetUnion;                    /* = "SetUni*/
EXTERN char *NAM_MakeCylinder;                /* = "MakCyl*/
EXTERN char *NAM_MakeCone;                    /* = "MakCon*/
EXTERN char *NAM_MakePrism;                   /* = "MakPrm*/
EXTERN char *NAM_MakePyramid;                 /* = "MakPyr*/
EXTERN char *NAM_MakeRevolve;                 /* = "MakRev*/
EXTERN char *NAM_MakeSphere;                  /* = "MakSph*/
EXTERN char *NAM_MakeTorus;                   /* = "MakTor*/
EXTERN char *NAM_MakeCube;                    /* = "MakCub*/
EXTERN char *NAM_MakeSweep;                   /* = "MakSwp*/
EXTERN char *NAM_SplitSolid;                  /* = "Spl*/
EXTERN char *NAM_CleanSolid;                  /* = "clean*/
EXTERN char *NAM_Detach;                      /* = "detach*/
EXTERN char *NAM_Glue;                        /* = "glue*/
EXTERN char *NAM_MergeSolid;                  /* = "merge*/
EXTERN char *NAM_ModifyIds;                   /* = "modify*/
EXTERN char *NAM_NewLocation;                 /* = "newloc*/
EXTERN char *NAM_HardRemove;                  /* = "hard*/
EXTERN char *NAM_SoftRemove;                  /* = "soft*/
EXTERN char *NAM_Revert;                      /* = "revert*/
EXTERN char *NAM_RingMove;                    /* = "rmove*/
EXTERN char *NAM_AllRingMove;                 /* = "armove*/

EXTERN char *MEN_PerguntaNovoAntigo;          /* = "%s: antigo novo\n*/
EXTERN char *MEN_PerguntaId;                  /* = "%s: id\n*/
EXTERN char *MEN_PerguntaNome;                /* = "%s: nome\n*/
EXTERN char *MEN_CamaraParametros;            /* = "%s: %s Ex Ey Ez Rx Ry Rz scala modo processo pontilhada paralelas\n*/
EXTERN char *MEN_CoordenadasParametros;       /* = "%s: -a comp cor\n-m x/y/z ; /* = const comp espacamento cor_eixo cor_malha\n*/

EXTERN char *MEN_NenhumaCamaraCorrente;       /* = "%s: nenhuma camera corrente!\n*/
EXTERN char *MEN_CamaraNomeNaoEncontrada;     /* = "%s: camera %s nao foi encontrada!\n*/
EXTERN char *MEN_CamaraIdNaoEncontrada;       /* = "%s: camera %d nao foi encontrada!\n*/
EXTERN char *MEN_CamaraParametroIncorreto;    /* = "%s: parametros da camera estao incorretos!\n*/
EXTERN char *MEN_CamaraJaExiste;              /* = "%s: camera %s ja existe!\n*/
EXTERN char *MEN_CamaraNomeCorrente;          /* = "%s: camera %s e camera corrente!\n*/
EXTERN char *MEN_CamaraIdCorrente;            /* = "%s: camera %d e camera corrente!\n*/
EXTERN char *MEN_NenhumaCamaraMonitora;       /* = "%s: nenhuma camera monitora!\n*/
EXTERN char *MEN_CorInvalida;                 /* = "%s: cor invalida!\n*/
EXTERN char *MEN_ParametrosIncorretos;        /* = "%s: parametros incorretos!\n*/
EXTERN char *MEN_ElementoNaoEncontrado;       /* = "%s: elemento %s nao foi encontrado!\n*/
EXTERN char *MEN_NaoAbriuArquivo;             /* = "%s: nao conseguiu abrir arquivo %s!\n*/
EXTERN char *MEN_NaoEncontrouAresta;          /* = "%s: nao encontrou aresta %d em %d!\n*/
EXTERN char *MEN_NaoPossuiMemoria;            /* = "%s: nao possui memoria!\n*/
EXTERN char *MEN_MSDAbortou;                  /* = "%s: programa abortado!\n*/
EXTERN char *MEN_NaoEncontrouSolido;          /* = "%s: nao encontrou solido %d!\n*/
EXTERN char *MEN_NaoEncontrouFace;            /* = "%s: nao encontrou face %d em solido %d!\n*/
EXTERN char *MEN_NaoEncontrouArestaV;         /* = "%s: nao encontrou aresta %d-%d em solido %s!\n*/
EXTERN char *MEN_NaoEncontrouArestaF;         /* = "%s: nao encontrou aresta %d-%d (%d-%d) em solido %d!\n*/
EXTERN char *MEN_NaoEncontrouVertice;         /* = "%s: nao encontrou vertice %d em solido %s!\n*/
EXTERN char *MEN_NaoArestaPonte;              /* = "%s: %d-%d nao e aresta-ponte ou aresta-strut, solido %d!\n*/
EXTERN char *MEN_NaoVerticeTerminal;          /* = "%s: vertice %d nao e terminal, solido %d!\n*/
EXTERN char *MEN_ArestaCircular;              /* = "%s: %d-%d e aresta circular, solido %d!\n*/
EXTERN char *MEN_FaceSemLoop;                 /* = "%s: face %d nao pode possuir loop, solido %d!\n*/
EXTERN char *MEN_FaceSemVertice;              /* = "%s: face %d deve possuir apenas um vertice, solido %d!\n*/
EXTERN char *MEN_VerticesEmLoopsDistintos;    /* = "%s: vertices %d e %d devem estar em loops distintos, solido %d!\n*/

EXTERN char *MEN_NaoEncontrouAxisNome;        /* = "%s: nao encontrou axis %s!\n*/
EXTERN char *MEN_NaoEncontrouGroupNome;       /* = "%s: nao encontrou grupo %s!\n*/
EXTERN char *MEN_SolidoJaExiste;              /* = "%s: solido %s ja existe!\n*/
EXTERN char *MEN_NaoEncontrouSolidNome;       /* = "%s: nao encontrou solido %s!\n*/
EXTERN char *MEN_NaoEncontrouGroupId;         /* = "%s: nao encontrou grupo %d!\n*/
EXTERN char *MEN_NaoEncontrouAxisId;          /* = "%s: nao encontrou axis %d!\n*/
EXTERN char *MEN_PonteirosNulos;              /* = "%s: ponteiros nulos!\n*/
EXTERN char *MEN_AxisJaExiste;                /* = "%s: axis %s ja existe!\n*/
EXTERN char *MEN_GroupJaExiste;               /* = "%s: grupo %s ja existe!\n*/
EXTERN char *MEN_GrupoCorrenteEliminado;      /* = "%s: grupo corrente pertence ao grupo eliminado!\n*/
EXTERN char *MEN_NaoPossuiAncestral;          /* = "%s: grupo corrente nao possui ancestral!\n*/
EXTERN char *MEN_GroupAncestralGroup;         /* = "%s: grupo %d e ancestral do group %d!\n*/
EXTERN char *MEN_FatorEscalaNulo;             /* = "%s: fator de escala e nulo!\n*/
EXTERN char *MEN_NaoEncontrouShell;           /* = "%s: nao encontrou shell %d em solido %d!\n*/
EXTERN char *MEN_MemoriaInvalida;             /* = "%s: memoria %d nao e valida!\n"*/
EXTERN char *MEN_NaoEncontrouTag;             /* = "%s: nao encontrou tag %d em solido %d!\n"*/
EXTERN char *MEN_NumeroDeSegmentosInvalido;   /* = "%s: numero de segmentos deve ser maior que 2!\n"*/
EXTERN char *MEN_OperacaoNaoRealizada;        /* = "%s: operacao nao foi realizada!\n"*/
EXTERN char *MEN_NaoEncontrouEdgeNo;          /* = "%s: nao encontrou aresta %d em solido %d!\n"*/

#endif
