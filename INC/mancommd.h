/* 
  PROJETO USPDesigner
  MODULO: MAN (Modulo Principal)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: mancommd.h
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

#ifndef __Mancommd_h
#define __Mancommd_h

#include <stdio.h>

#define BUFLENG    256
typedef struct command   COMTYPE;
struct command
{
   char *comname;
   void (*comfunc)(void);
};

/* file manhelp_.c */
void help(void);
void helpman(COMTYPE *p);

/* file manlinin.c */
char optin(void);
char lineins(char *s);
void control(void);

/* file manmainp.c */
#if defined (_Windows)
void msdinit(void);

#else
int main(int argc, char **argv);

#endif
void maninit(void);
void MSD_readFileExecute(void);
void MSD_executeFileCommand(char *name);
void endfunc(void);
void comerror(void);

/* file manstrmp */
void strupper(unsigned char *s);
void strlower(unsigned char *s);
void skipsp(char **p);
void skipnext(char **p);

/* file mantrace.c */
void MSD_execSetTrace(void);
void MSD_execResetTrace(void);

#ifdef _DVS_
FILE *instream;
char linebuf[BUFLENG];
char *restbuf;

COMTYPE comtable[] =
{
   { "AFFINE",       MSD_execNameAffine                },
   { "AREA",         MSD_execNameArea                  },
   { "ASHOW",        MSD_execAnaliseShow               },
   { "AXIS",         MSD_execManipulateAxis            },
   { "CAMERA",       MSD_execCamera                    },
   { "CARENA",       MSD_execNameCentroCarena          },
   { "CGRAVIDADE",   MSD_execNameCentroDeGravidade     },
   { "CLEAR",        MSD_execClear                     },
   { "COLOR",        MSD_execColor                     },
   { "CONE",         MSD_execNameMakeCone              },
   { "COPY",         MSD_execNameCopyElement           },
   { "CUBE",         MSD_execNameMakeCube              },
   { "CYLINDER",     MSD_execNameMakeCylinder          },
   { "DELETE",       MSD_execNameDeleteElement         },
   { "DENSABS",      MSD_execNameDensidadeAbsoluta     },
   { "DENSFLUIDO",   MSD_execDensidadeFluido           },
   { "DENSREL",      MSD_execNameDensidadeRelativa     },
 #if !defined (__UNIX_) && !defined (_Windows)
   { "DIR",          MSD_execDir                       },
 #endif
   { "DISPLAY",      MSD_execDisplay                   },
   { "DISPCOOR",     MSD_execDisplayCoordinates        },
   { "DISPFILE",     MSD_execDisplayFile               },
   { "END",          endfunc                           },
   { "FILE",         MSD_readFileExecute               },
   { "GROUP",        MSD_execManipulateGroup           },
   { "HELP",         help                              },
   { "INERCIA",      MSD_execNameMatrizInercia         },
   { "INTERSECT",    MSD_execNameIntersection          },
   { "LIST",         MSD_execListElement               },
   { "LOAD",         MSD_execNameLoadSolid             },
   { "MASSA",        MSD_execNameMassa                 },
   { "MINUS",        MSD_execNameDifference            },
   { "MONITOR",      MSD_execMonitor                   },
   { "MOVE",         MSD_execNameTranslateElement      },
   { "NCOPY",        MSD_execNameNCopySolid            },
   { "NEW",          maninit                           },
   { "PRINT",        MSD_execPrint                     },
   { "PRISM",        MSD_execNameMakePrism             },
   { "PROJETADA",    MSD_execNameAreaProjetada         },
   { "PROJTOT",      MSD_execProjetadaTotal            },
   { "PROPRIEDADE",  MSD_execManipulatePropriedade     },
   { "PYRAMID",      MSD_execNameMakePyramid           },
 #ifdef __ROV__
   { "RCIRCULO",     MSD_execNameMakeCirculo           },
   { "RCUBO",        MSD_execNameMakeCubo              },
   { "RPERFILI",     MSD_execNameMakePerfilI           },
   { "RPERFILL",     MSD_execNameMakePerfilL           },
   { "RPERFILT",     MSD_execNameMakePerfilT           },
   { "RPIRAMIDE",    MSD_execNameMakePiramide          },
   { "RPLACA",       MSD_execNameMakePlaca             },
   { "RPRISMA",      MSD_execNameMakePrisma            },
   { "RESFERA",      MSD_execNameMakeEsfera            },
   { "RQUADRADO",    MSD_execNameMakeQuadrado          },
   { "RRETANGULO",   MSD_execNameMakeRetangulo         },
   { "RTRIANGULO",   MSD_execNameMakeTriangulo         },
   { "RDIM",         MSD_execNameDimensaoPrincipal     },
   { "RDESLOCA",     MSD_execNameDesloca               },
   { "RROTACIONA",   MSD_execNameRotaciona             },
   { "RORDENA",      MSD_execOrdena                    },
 #endif
   { "RESETTRACE",   MSD_execResetTrace                },
   { "REVOLVE",      MSD_execNameMakeRevolve           },
   { "RENAME",       MSD_execNameRenameElement         },
   { "ROTATE",       MSD_execNameRotateElement         },
 #ifdef __curved_env__
   { "ROUND",        MSD_execRound,                    },
   { "ROUNDALL",     MSD_execRndAllEdges,              },
 #endif
   { "SAVE",         MSD_execNameSaveSolid             },
   { "SCALE",        MSD_execNameScale                 },
   { "SETTRACE",     MSD_execSetTrace                  },
   { "SHOW",         MSD_execShow                      },
   { "SPLIT",        MSD_execNameSplit                 },
   { "SPHERE",       MSD_execNameMakeSphere            },
   { "SWEEP",        MSD_execNameMakeSweep             },
   { "TCGRAVIDADE",  MSD_execCentroMassaTotal          },
   { "TCARENA",      MSD_execCentroCarenaTotal         },
   { "TEMPUXO",      MSD_execEmpuxoTotal               },
   { "TIME",         MSD_execTime                      },
   { "TINERCIA",     MSD_execMatrizInerciaTotal        },
   { "TMASSA",       MSD_execMassaTotal                },
   { "TORUS",        MSD_execNameMakeTorus             },
   { "TPESO",        MSD_execPesoTotal                 },
   { "TVOLFLU",      MSD_execVolumeFlutuabilidadeTotal },
   { "TVOLUME",      MSD_execVolumeTotal               },
   { "UNION",        MSD_execNameUnion                 },
   { "VOLUME",       MSD_execNameVolume                },
   { "WAIT",         MSD_execWait                      },
   { "WINDOW",       MSD_execWindow                    },
 #ifdef __Extended_Version
   { "LLISTA",       MSD_execListSolid                 },
   { "MVSF",         MSD_execMVSF                      },
   { "MSFKR",        MSD_execMSFKR2                    },
   { "MSFKR2",       MSD_execMSFKR2                    },
   { "MFKRH",        MSD_execMFKRH2                    },
   { "MFKRH2",       MSD_execMFKRH2                    },
   { "MEV",          MSD_execMEV                       },
   { "SEMV",         MSD_execSEMV                      },
   { "SVME",         MSD_execSVME3                     },
   { "SVME2",        MSD_execSVME2                     },
   { "SVME3",        MSD_execSVME3                     },
   { "MEKR",         MSD_execMEKR2                     },
   { "MEKR2",        MSD_execMEKR2                     },
   { "MEF",          MSD_execMEF2                      },
   { "MEF2",         MSD_execMEF2                      },
   { "KVSF",         MSD_execKVSF                      },
   { "KSFMR",        MSD_execKSFMR                     },
   { "KFMRH",        MSD_execKFMRH                     },
   { "KEV",          MSD_execKEV2                      },
   { "JVKE",         MSD_execJVKE                      },
   { "JEKV",         MSD_execJEKV                      },
   { "KEV2",         MSD_execKEV2                      },
   { "KEMR",         MSD_execKEMR                      },
   { "KEF",          MSD_execKEF2                      },
   { "KEF2",         MSD_execKEF2                      },
   { "DETACH",       MSD_execDetach                    },
   { "GLUE",         MSD_execGlue                      },
   { "MERGE",        MSD_execMerge                     },
   { "DOINV",        MSD_execSetDoInverse              },
   { "UNDO",         MSD_execUndoOp                    },
   { "LOPS",         MSD_highPrintAllOps               },
   { "HARDREM",      MSD_execHardRemove                },
   { "NAME",         MSD_execNome                      },
   { "REMSOFT",      MSD_execNameSoftRemove            },
   { "LIMPA",        MSD_execCleanSolid                },
   { "MODIFYNAMES",  MSD_execModifyIds                 },
   { "NEWLOC",       MSD_execNewVertexLocation         },
   { "REVERT",       MSD_execRevert                    },
   { "RMOVE",        MSD_execRingMove                  },
   { "ALLRINGMOVE",  MSD_execAllRingMove               },
   { "TRANS",        MSD_execTranslate                 },
   { "TRANSFORMA",   MSD_execTransformation            },
   { "IDROTATE",     MSD_execRotate                    },
   { "LADJAFACE",    MSD_execNameListFaceAdjacency     },
   { "IDUNION",      MSD_execUnion                     },
   { "IDMINUS",      MSD_execDifference                },
   { "IDINTERSECT",  MSD_execIntersection              },
   { "IDCUBE",       MSD_execMakeCube                  },
   { "IDCYLINDER",   MSD_execMakeCylinder              },

  #ifdef __curved_env__
   { "GETHE",        hget_he                           },
   { "GETMATE",      hget_mate                         },
   { "GETNEXT",      hget_next                         },
   { "GETPREV",      hget_prev                         },
   { "CNEWLOC",      cnewloc                           },
   { "CMEV",         hcmev                             },
   { "CMEF",         hcmef                             },
   { "MEMO_SHOW",    hshow_memo                        },
   { "PUTCTAG",      MSD_execSetCurveAtributeToEdge    },
   { "CPUTCTAG",     cputctag                          },
   { "NEWPOLY",      MSD_execNewCurvePolyline          },
   { "ADDPOINT",     MSD_execAddPointToCurve           },
   { "EVALPOLYLINE", MSD_execEvaluateCurvePolyline     },
   { "CEVALPOLYLINE",ceval_polyline                    },
   { "EVALEDGES",    MSD_execEvalEdge                  },
  #endif
 #endif
   { "UNDO",         MSD_execUndoOp                    },
   { "LOPS",         MSD_highPrintAllOps               },
   { "ROPS",         MSD_highPrintAllRedoOps           },
   { "TRACE",        MSD_execTrace                     },
 #ifdef _Windows
   { "ANIME",        animatio                          },
   { "CINEMA",       cinema                            },
 #endif
   { "\0",           comerror                          }
};

#else
extern COMTYPE comtable[];
extern FILE    *instream;
extern char    linebuf[BUFLENG];
extern char    *restbuf;
#endif

#endif
