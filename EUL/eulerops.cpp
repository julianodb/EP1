/* 
  PROJETO USPDesigner
  MODULO: EUL (Operadores de Euler)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: eulerops.cpp
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
#ifndef __UNIX_
 #include <malloc.h>
#endif
#include "mensagem.h"
#include "memvirtu.h"
#include "lowparam.h"
#include "lowsolid.h"
#include "curvgeom.h"
#include "eulerops.h"
#include "mancommd.h"

typedef struct eulercod   EuCode;

struct eulercod
{
   OpCode ecode;
   char   *eop;
   int    (*xeop)(EulerOp *op);
   void   (*peop)(FILE *f, EulerOp *op);
   int    (*reop)(FILE *f, EulerOp *op);
   int    (*meop)(EulerOp *op);
   int    num;
};

EuCode ematrix[] =
{
/* Make Vertex Solid Face ******************************************************/
   { MVSF,
     "MVSF",
     MSD_execStructMVSF,
     MSD_printStructMVSF,
     MSD_readStructMVSF,
     MSD_modifyStructMVSF,
     1 },

/* Kill Vertex Solid Face ******************************************************/
   { KVSF,
     "KVSF",
     MSD_execStructKVSF,
     MSD_printStructKVSF,
     MSD_readStructKVSF,
     MSD_modifyStructKVSF,
     1 },

/* Make Edge Vertex ************************************************************/
   { MEV,
     "SVME",
     MSD_execStructMEV,
     MSD_printStructMEV,
     MSD_readStructMEV,
     MSD_modifyStructMEV,
     1 },

/* Kill Edge Vertex ************************************************************/
   { KEV,
     "KEV",
     MSD_execStructKEV,
     MSD_printStructKEV,
     MSD_readStructKEV,
     MSD_modifyStructKEV,
     1 },

/* Make Edge Face **************************************************************/
   { MEF,
     "MEF",
     MSD_execStructMEF,
     MSD_printStructMEF,
     MSD_readStructMEF,
     MSD_modifyStructMEF,
     1 },

/* Kill Edge Face **************************************************************/
   { KEF,
     "KEF",
     MSD_execStructKEF,
     MSD_printStructKEF,
     MSD_readStructKEF,
     MSD_modifyStructKEF,
     1 },

/* Kill Edge Make Ring *********************************************************/
   { KEMR,
     "KEMR",
     MSD_execStructKEMR,
     MSD_printStructKEMR,
     MSD_readStructKEMR,
     MSD_modifyStructKEMR,
     1 },

/* Make Edge Kill Ring *********************************************************/
   { MEKR,
     "MEKR",
     MSD_execStructMEKR,
     MSD_printStructMEKR,
     MSD_readStructMEKR,
     MSD_modifyStructMEKR,
     1 },

/* Kill Face Make Ring Hole ****************************************************/
   { KFMRH,
     "KFMRH",
     MSD_execStructKFMRH,
     MSD_printStructKFMRH,
     MSD_readStructKFMRH,
     MSD_modifyStructKFMRH,
     1 },

/* Make Face Kill Ring Hole ****************************************************/
   { MFKRH,
     "MFKRH",
     MSD_execStructMFKRH,
     MSD_printStructMFKRH,
     MSD_readStructMFKRH,
     MSD_modifyStructMFKRH,
     1 },

/* Kill Shell Face Make Ring ***************************************************/
   { KSFMR,
     "KSFMR",
     MSD_execStructKSFMR,
     MSD_printStructKSFMR,
     MSD_readStructKSFMR,
     MSD_modifyStructKSFMR,
     1 },

/* Make Shell Face Kill Ring ***************************************************/
   { MSFKR,
     "MSFKR",
     MSD_execStructMSFKR,
     MSD_printStructMSFKR,
     MSD_readStructMSFKR,
     MSD_modifyStructMSFKR,
     1 },

/* Hard Remove *****************************************************************/
   { HARDREM,
     "HARDREM",
     MSD_execStructHardRemove,
     MSD_printStructHardRemove,
     MSD_readStructHardRemove,
     MSD_modifyStructHardRemove,
     1 },

/* Ring Move *******************************************************************/
   { RMOVE,
     "RMOVE",
     MSD_execStructRingMove,
     MSD_printStructRingMove,
     MSD_readStructRingMove,
     MSD_modifyStructRingMove,
     1 },

/* New Vertex Location *********************************************************/
   { NEWLOC,
     "NEWLOC",
     MSD_execStructNewVertexLocation,
     MSD_printStructNewVertexLocation,
     MSD_readStructNewVertexLocation,
     MSD_modifyStructNewVertexLocation,
     1 },

/* Revert Solid ****************************************************************/
   { REVERT,
     "REVERT",
     MSD_execStructRevert,
     MSD_printStructRevert,
     MSD_readStructRevert,
     MSD_modifyStructRevert,
     1 },

/* Modify Ids ******************************************************************/
   { MODIFYNAMES,
     "MODIFYNAMES",
     MSD_execStructModifyIds,
     MSD_printStructModifyIds,
     MSD_readStructModifyIds,
     MSD_modifyStructModifyIds,
     1 },

/* Detach Shell ****************************************************************/
   { DETACH,
     "DETACH",
     MSD_execStructDetach,
     MSD_printStructDetach,
     MSD_readStructDetach,
     MSD_modifyStructDetach,
     1 },

/* Translate Solid *************************************************************/
   { TRANS,
     "TRANS",
     MSD_execStructTranslate,
     MSD_printStructTranslate,
     MSD_readStructTranslate,
     MSD_modifyStructTranslate,
     1 },

/* Rotate Solid ****************************************************************/
   { ROTAT,
     "ROTAT",
     MSD_execStructRotate,
     MSD_printStructRotate,
     MSD_readStructRotate,
     MSD_modifyStructRotate,
     1 },

/* Scale Solid *****************************************************************/
   { SCALE,
     "SCALE",
     MSD_execStructScale,
     MSD_printStructScale,
     MSD_readStructScale,
     MSD_modifyStructScale,
     1 },

/* Transform Solid *************************************************************/
   { TRANSFORM,
     "TRANSFORM",
     MSD_execStructTransformation,
     MSD_printStructTransformation,
     MSD_readStructTransformation,
     MSD_modifyStructTransformation,
     5 },

/* Associate Two Elements ******************************************************/
   { ASSOCIATE,
     "ASSOCIATE",
     MSD_execStructAssociate,
     MSD_printStructAssociate,
     MSD_readStructAssociate,
     MSD_modifyStructAssociate,
     1 },

/* Create Axis of Translation **************************************************/
   { CRTAXSTRS,
     "CRTAXSTRS",
     MSD_execStructCrtAxsTranslation,
     MSD_printStructCrtAxsTranslation,
     MSD_readStructCrtAxsTranslation,
     MSD_modifyStructCrtAxsTranslation,
     1 },

/* Create Axis of Rotation *****************************************************/
   { CRTAXSROT,
     "CRTAXSROT",
     MSD_execStructCrtAxsRotation,
     MSD_printStructCrtAxsRotation,
     MSD_readStructCrtAxsRotation,
     MSD_modifyStructCrtAxsRotation,
     2 },

/* Create Group ****************************************************************/
   { CRTGRP,
     "CRTGRP",
     MSD_execStructCreateGroup,
     MSD_printStructCreateGroup,
     MSD_readStructCreateGroup,
     MSD_modifyStructCreateGroup,
     1 },

/* Remove Axis *****************************************************************/
   { REMAXS,
     "REMAXS",
     MSD_execStructRemoveAxis,
     MSD_printStructRemoveAxis,
     MSD_readStructRemoveAxis,
     MSD_modifyStructRemoveAxis,
     1 },

/* Remove Group ****************************************************************/
   { REMGRP,
     "REMGRP",
     MSD_execStructRemoveOneGroup,
     MSD_printStructRemoveOneGroup,
     MSD_readStructRemoveOneGroup,
     MSD_modifyStructRemoveOneGroup,
     1 },

/* Translate Axis **************************************************************/
   { MOVAXIS,
     "MOVAXS",
     MSD_execStructMoveAxis,
     MSD_printStructMoveAxis,
     MSD_readStructMoveAxis,
     MSD_modifyStructMoveAxis,
     1 },

/* Rotate Axis *****************************************************************/
   { ROTAXIS,
     "ROTAXS",
     MSD_execStructRotateAxis,
     MSD_printStructRotateAxis,
     MSD_readStructRotateAxis,
     MSD_modifyStructRotateAxis,
     2 },

/* Color of Solid **************************************************************/
   { COLOR,
     "COLOR",
     MSD_execStructColor,
     MSD_printStructColor,
     MSD_readStructColor,
     MSD_modifyStructColor,
     1 },

/* Name of Solid ***************************************************************/
   { NAME,
     "NAME",
     MSD_execStructName,
     MSD_printStructName,
     MSD_readStructName,
     MSD_modifyStructName,
     2 },

#ifdef __curved_env__
/* Color of Solid **************************************************************/
   { KILLLINE,
     "KILLLINE",
     MSD_execStructKILLLINE,
     MSD_printStructKILLLINE,
     MSD_readStructKILLLINE,
     MSD_modifyStructKILLLINE,
     1 },

/* Color of Solid **************************************************************/
   { NEWLINE,
     "NEWLINE",
     MSD_execStructNEWLINE,
     MSD_printStructNEWLINE,
     MSD_readStructNEWLINE,
     MSD_modifyStructNEWLINE,
     1 },

/* Color of Solid **************************************************************/
   { KILLPOLY,
     "KILLPOLY",
     MSD_execStructKILLPOLY,
     MSD_printStructKILLPOLY,
     MSD_readStructKILLPOLY,
     MSD_modifyStructKILLPOLY,
     1 },

/* Color of Solid **************************************************************/
   { NEWPOLY,
     "NEWPOLY",
     MSD_execStructNEWPOLY,
     MSD_printStructNEWPOLY,
     MSD_readStructNEWPOLY,
     MSD_modifyStructNEWPOLY,
     1 },

/* Color of Solid **************************************************************/
   { SETCTAG,
     "SETCTAG",
     MSD_execStructSETCTAG,
     MSD_printStructSETCTAG,
     MSD_readStructSETCTAG,
     MSD_modifyStructSETCTAG,
     1 },

/* Add point to polyline *******************************************************/
   { ADDLPOINT,
     "ADDLPOINT",
     MSD_execStructADDLPOINT,
     MSD_printStructADDLPOINT,
     MSD_readStructADDLPOINT,
     MSD_modifyStructADDLPOINT,
     1 },

/* Remove point from polyline **************************************************/
   { REMLPOINT,
     "REMLPOINT",
     MSD_execStructREMLPOINT,
     MSD_printStructREMLPOINT,
     MSD_readStructREMLPOINT,
     MSD_modifyStructREMLPOINT,
     1 },
#endif

/* Error Code ******************************************************************/
   { MAXCODE,
     "\0",
     MSD_execStructError,
     MSD_printStructError,
     MSD_readStructError,
     MSD_modifyStructError,
     1 }
};

/* Add an Euler operator to the list of operators of a solid for undoing */
void MSD_lowAddEulerOp(SPTYPE s, OpCode code, Id ip1, Id ip2, Id ip3, Id ip4, \
                       Id ip5, Id ip6, Id ip7, Id ip8,                        \
                       real fp1, real fp2,                                    \
                       real fp3, real fp4)
{
#ifndef __VIRTUAL_MEM
   EulerOp *newop;

   if ((newop = (EulerOp *)malloc(sizeof(EulerOp))) == (EulerOp *)NIL)
   {
      fprintf(stderr, MEN_NaoPossuiMemoria, NAM_AddEulerOperator);
      return;
   }
   newop->opnext = OpHead;
   OpHead        = newop;
   OpCount++;
#else
   OpHead++;
   OpCount++;
   EulerCount(OpHead) = OpHead;
#endif

   EulerOpCode(OpHead)       = code;
   EulerSolidNo(OpHead)      = s != SNIL ? SolSolidNo(s) : -1;
   EulerIp1(OpHead)          = ip1;
   EulerIp2(OpHead)          = ip2;
   EulerIp3(OpHead)          = ip3;
   EulerIp4(OpHead)          = ip4;
   EulerIp5(OpHead)          = ip5;
   EulerIp6(OpHead)          = ip6;
   EulerIp7(OpHead)          = ip7;
   EulerIp8(OpHead)          = ip8;
   EulerFp1(OpHead)          = fp1;
   EulerFp2(OpHead)          = fp2;
   EulerFp3(OpHead)          = fp3;
   EulerFp4(OpHead)          = fp4;
   EulerCommandCount(OpHead) = CommandCount;
}

/* Add an Euler operator to the list of operators of a solid for redoing */
void MSD_lowAddEulerOpRedo(SPTYPE s, OpCode code, Id ip1, Id ip2, Id ip3, Id ip4, \
                           Id ip5, Id ip6, Id ip7, Id ip8,                        \
                           real fp1, real fp2,                                    \
                           real fp3, real fp4)
{
#ifndef __VIRTUAL_MEM
   EulerOp *newop;

   if ((newop = (EulerOp *)malloc(sizeof(EulerOp))) == (EulerOp *)NIL)
   {
      fprintf(stderr, MEN_NaoPossuiMemoria, NAM_AddEulerOperator);
      return;
   }
   newop->opnext = OpHeadRedo;
   OpHeadRedo    = newop;
   OpCountRedo++;
#else
   OpHeadRedo++;
   OpCountRedo++;
   EulerCountRedo(OpHeadRedo) = OpHeadRedo;
#endif

   EulerOpCodeRedo(OpHeadRedo)       = code;
   EulerSolidNoRedo(OpHeadRedo)      = s != SNIL ? SolSolidNo(s) : -1;
   EulerIp1Redo(OpHeadRedo)          = ip1;
   EulerIp2Redo(OpHeadRedo)          = ip2;
   EulerIp3Redo(OpHeadRedo)          = ip3;
   EulerIp4Redo(OpHeadRedo)          = ip4;
   EulerIp5Redo(OpHeadRedo)          = ip5;
   EulerIp6Redo(OpHeadRedo)          = ip6;
   EulerIp7Redo(OpHeadRedo)          = ip7;
   EulerIp8Redo(OpHeadRedo)          = ip8;
   EulerFp1Redo(OpHeadRedo)          = fp1;
   EulerFp2Redo(OpHeadRedo)          = fp2;
   EulerFp3Redo(OpHeadRedo)          = fp3;
   EulerFp4Redo(OpHeadRedo)          = fp4;
   EulerCommandCountRedo(OpHeadRedo) = CommandCount;
}

/* Add a matrix into the log */
void MSD_lowAddMatrix(SPTYPE s, OpCode opcode, Id ip1, Id ip2, Id ip3, \
                      Id ip4, Id ip5, Id ip6,                          \
                      matrix m)
{
   int i;

   for (i = 3; i > -1; i--)
   {
      MSD_lowAddEulerOp(s, opcode, ip1, ip2, ip3, ip4, ip5, ip6,
                        0, 0, m[i][0], m[i][1], m[i][2], m[i][3]);
   }
}

/* Add a matrix into the log */
void MSD_lowAddMatrixRedo(SPTYPE s, OpCode opcode, Id ip1, Id ip2, Id ip3, \
                          Id ip4, Id ip5, Id ip6,                          \
                          matrix m)
{
   int i;

   for (i = 3; i > -1; i--)
   {
      MSD_lowAddEulerOpRedo(s, opcode, ip1, ip2, ip3, ip4, ip5, ip6,
                            0, 0, m[i][0], m[i][1], m[i][2], m[i][3]);
   }
}
