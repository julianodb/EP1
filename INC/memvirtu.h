/* 
  PROJETO USPDesigner
  MODULO: MEM (Virtual Memory Manipulation)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: memvirtu.h
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

#ifndef __Memvirtu_h
#define __Memvirtu_h

#ifndef EXTERN
 #ifdef _DVS_
  #define EXTERN
 #else
  #define EXTERN    extern
 #endif
#endif

/* Virtual Array Control Block typedef */

typedef struct
{
   FILE *file;                            /* pointer to file control block */
   long size;                             /* number of array elements in file */
   int  elsize;                           /* number of bytes in each element */
   char *buffer;                          /* pointer to array buffer */
   int  buf_elsize;                       /* size of element in buffer including index */
   int  buf_size;                         /* number of elements in buffer */
   char *blank_rec;                       /* pointer to initialization record */
   /* used for extending file */
   /* virtual array control block type name */
} VACB;

EXTERN VACB *EulerOpsArray;
EXTERN long ArrayTop[10];
EXTERN long ArrayBottom[10];
EXTERN VACB *VirtualMemoryBlock[10];
EXTERN VACB *SPL01VirtualArray;
EXTERN VACB *SPL02VirtualArray;
EXTERN VACB *SPL03VirtualArray;
EXTERN VACB *SET01VirtualArray;
EXTERN VACB *SET03VirtualArray;
EXTERN VACB *SET04VirtualArray;
EXTERN VACB *SET06VirtualArray;
EXTERN VACB *SET07VirtualArray;
EXTERN VACB *SET08VirtualArray;
EXTERN VACB *SET10VirtualArray;

/* Virtual Array Access Prototypes */
int init_v_array(char *filename, int rec_size, char filchar);
VACB *open_v_array(char *filename, int buffer_size);
void close_v_array(VACB *v_array);
char *access_v_rec(VACB *v_array, long index);

/* file memsolid.c */
void MSD_highIniciateMemVirtual(void);
void MSD_highTerminateMemVirtual(void);
void MSD_lowIniciateMemVirtual(int what);
long MSD_getMemVirtual(int what);
void MSD_putMemVirtual(long FreePointer, int what);
void MSD_lowTerminateMemVirtual(int what);


/* Definicoes para implementar memoria virtual para o modulo de          */
/*  operadores de Euler.                                                 */
#ifdef __VIRTUAL_MEM
 #define EULEROPS(pp)                 ((EulerOp *)access_v_rec(EulerOpsArray, (pp) * 2 + 2))
 #define EulerCount(p)                EULEROPS(p)->pp
 #define EulerOpNext(pp)              pp--
 #define EulerOpCode(pp)              EULEROPS(pp)->opcode
 #define EulerSolidNo(pp)             EULEROPS(pp)->solidno
 #define EulerIp1(pp)                 EULEROPS(pp)->ip1
 #define EulerIp2(pp)                 EULEROPS(pp)->ip2
 #define EulerIp3(pp)                 EULEROPS(pp)->ip3
 #define EulerIp4(pp)                 EULEROPS(pp)->ip4
 #define EulerIp5(pp)                 EULEROPS(pp)->ip5
 #define EulerIp6(pp)                 EULEROPS(pp)->ip6
 #define EulerIp7(pp)                 EULEROPS(pp)->ip7
 #define EulerIp8(pp)                 EULEROPS(pp)->ip8
 #define EulerFp1(pp)                 EULEROPS(pp)->fp1
 #define EulerFp2(pp)                 EULEROPS(pp)->fp2
 #define EulerFp3(pp)                 EULEROPS(pp)->fp3
 #define EulerFp4(pp)                 EULEROPS(pp)->fp4
 #define EulerCommandCount(pp)        EULEROPS(pp)->CommandCount

 #define EULEROPSREDO(pp)             ((EulerOp *)access_v_rec(EulerOpsArray, (pp) * 2 + 1))
 #define EulerCountRedo(p)            EULEROPSREDO(p)->pp
 #define EulerOpNextRedo(pp)          pp--
 #define EulerOpCodeRedo(pp)          EULEROPSREDO(pp)->opcode
 #define EulerSolidNoRedo(pp)         EULEROPSREDO(pp)->solidno
 #define EulerIp1Redo(pp)             EULEROPSREDO(pp)->ip1
 #define EulerIp2Redo(pp)             EULEROPSREDO(pp)->ip2
 #define EulerIp3Redo(pp)             EULEROPSREDO(pp)->ip3
 #define EulerIp4Redo(pp)             EULEROPSREDO(pp)->ip4
 #define EulerIp5Redo(pp)             EULEROPSREDO(pp)->ip5
 #define EulerIp6Redo(pp)             EULEROPSREDO(pp)->ip6
 #define EulerIp7Redo(pp)             EULEROPSREDO(pp)->ip7
 #define EulerIp8Redo(pp)             EULEROPSREDO(pp)->ip8
 #define EulerFp1Redo(pp)             EULEROPSREDO(pp)->fp1
 #define EulerFp2Redo(pp)             EULEROPSREDO(pp)->fp2
 #define EulerFp3Redo(pp)             EULEROPSREDO(pp)->fp3
 #define EulerFp4Redo(pp)             EULEROPSREDO(pp)->fp4
 #define EulerCommandCountRedo(pp)    EULEROPSREDO(pp)->CommandCount
#else
 #define EULEROPS(pp)                 pp
 #define EulerCount(p)                EULEROPS(p)->pp
 #define EulerOpNext(pp)              pp = (pp)->opnext
 #define EulerOpCode(pp)              (pp)->opcode
 #define EulerSolidNo(pp)             (pp)->solidno
 #define EulerIp1(pp)                 (pp)->ip1
 #define EulerIp2(pp)                 (pp)->ip2
 #define EulerIp3(pp)                 (pp)->ip3
 #define EulerIp4(pp)                 (pp)->ip4
 #define EulerIp5(pp)                 (pp)->ip5
 #define EulerIp6(pp)                 (pp)->ip6
 #define EulerIp7(pp)                 (pp)->ip7
 #define EulerIp8(pp)                 (pp)->ip8
 #define EulerFp1(pp)                 (pp)->fp1
 #define EulerFp2(pp)                 (pp)->fp2
 #define EulerFp3(pp)                 (pp)->fp3
 #define EulerFp4(pp)                 (pp)->fp4
 #define EulerCommandCount(pp)        (pp)->CommandCount

 #define EULEROPSREDO(pp)             pp
 #define EulerCountRedo(p)            EULEROPSREDO(p)->pp
 #define EulerOpNextRedo(pp)          pp = (pp)->opnext
 #define EulerOpCodeRedo(pp)          (pp)->opcode
 #define EulerSolidNoRedo(pp)         (pp)->solidno
 #define EulerIp1Redo(pp)             (pp)->ip1
 #define EulerIp2Redo(pp)             (pp)->ip2
 #define EulerIp3Redo(pp)             (pp)->ip3
 #define EulerIp4Redo(pp)             (pp)->ip4
 #define EulerIp5Redo(pp)             (pp)->ip5
 #define EulerIp6Redo(pp)             (pp)->ip6
 #define EulerIp7Redo(pp)             (pp)->ip7
 #define EulerIp8Redo(pp)             (pp)->ip8
 #define EulerFp1Redo(pp)             (pp)->fp1
 #define EulerFp2Redo(pp)             (pp)->fp2
 #define EulerFp3Redo(pp)             (pp)->fp3
 #define EulerFp4Redo(pp)             (pp)->fp4
 #define EulerCommandCountRedo(pp)    (pp)->CommandCount
#endif

/* Definicoes para implementar memoria virtual para o modulo de          */
/*  estrutura de dados do solido.                                        */

#ifdef __VIRTUAL_MEM
 #define SSTYPE(pp)         ((STYPE *)access_v_rec(VirtualMemoryBlock[SOLID], pp))
 #define SolSolidNo(p)      SSTYPE(p)->solidno
 #define SolName(p)         SSTYPE(p)->name
 #define SolNxt(p)          SSTYPE(p)->nxt
 #define SolPrevS(p)        SSTYPE(p)->prevs
 #define SolSShells(p)      SSTYPE(p)->sshells
 #define SolGroup(p)        SSTYPE(p)->group
 #define SolOAxs(p)         SSTYPE(p)->oaxs
 #define SolBox(p)          SSTYPE(p)->box
 #define SolSBits(p)        SSTYPE(p)->sbits
 #define SolEuCoef(p)       SSTYPE(p)->eucoef
 #define SolDsp(p)          SSTYPE(p)->dsp
 #define SolColor(p)        SSTYPE(p)->color
 #define SolDensRel(p)      SSTYPE(p)->densrel
 #define SolDensAbs(p)      SSTYPE(p)->densabs
 #define SolAreaProj(p)     SSTYPE(p)->areaproj
 #define SolType(p)         SSTYPE(p)->type
 #define SolGeometria(p)    SSTYPE(p)->geometria
 #define SolFronteira(p)    SSTYPE(p)->fronteira
 #define SolBAreaProj(p)    SSTYPE(p)->bareaproj
 #define SolParam1(p)       SSTYPE(p)->param1
 #define SolParam2(p)       SSTYPE(p)->param2
 #define SolParam3(p)       SSTYPE(p)->param3
 #define SolParam4(p)       SSTYPE(p)->param4
 #define SolPrincDir(p)     SSTYPE(p)->PrincDir
 #define SolCenter(p)       SSTYPE(p)->center
 #define SolAux(p)          SSTYPE(p)->aux
 #define SolPos(p)          SSTYPE(p)->pos
 #define SolPerfil(p)       SSTYPE(p)->corte
 #define SolSurf(p)         SSTYPE(p)->surf
 #define SolCurv(p)         SSTYPE(p)->curv
#else
 #define SolSolidNo(p)      ((p)->solidno)
 #define SolName(p)         ((p)->name)
 #define SolNxt(p)          ((p)->nxt)
 #define SolPrevS(p)        ((p)->prevs)
 #define SolSShells(p)      ((p)->sshells)
 #define SolGroup(p)        ((p)->group)
 #define SolOAxs(p)         ((p)->oaxs)
 #define SolBox(p)          ((p)->box)
 #define SolSBits(p)        ((p)->sbits)
 #define SolEuCoef(p)       ((p)->eucoef)
 #define SolDsp(p)          ((p)->dsp)
 #define SolColor(p)        ((p)->color)
 #define SolDensRel(p)      ((p)->densrel)
 #define SolDensAbs(p)      ((p)->densabs)
 #define SolAreaProj(p)     ((p)->areaproj)
 #define SolType(p)         ((p)->type)
 #define SolGeometria(p)    ((p)->geometria)
 #define SolFronteira(p)    ((p)->fronteira)
 #define SolBAreaProj(p)    ((p)->bareaproj)
 #define SolParam1(p)       ((p)->param1)
 #define SolParam2(p)       ((p)->param2)
 #define SolParam3(p)       ((p)->param3)
 #define SolParam4(p)       ((p)->param4)
 #define SolPrincDir(p)     ((p)->PrincDir)
 #define SolCenter(p)       ((p)->center)
 #define SolAux(p)          ((p)->aux)
 #define SolPos(p)          ((p)->pos)
 #define SolPerfil(p)       ((p)->corte)
 #define SolSurf(p)         ((p)->surf)
 #define SolCurv(p)         ((p)->scurvs)
#endif

#ifdef __VIRTUAL_MEM
 #define DDTYPE(pp)       ((DTYPE *)access_v_rec(VirtualMemoryBlock[SHELL], pp))
 #define SheShellNo(p)    DDTYPE(p)->shellno
 #define SheNextD(p)      DDTYPE(p)->nextd
 #define ShePrevD(p)      DDTYPE(p)->prevd
 #define SheSSolids(p)    DDTYPE(p)->ssolids
 #define SheSFaces(p)     DDTYPE(p)->sfaces
 #define SheSEdges(p)     DDTYPE(p)->sedges
 #define SheSVerts(p)     DDTYPE(p)->sverts
 #define SheBox(p)        DDTYPE(p)->box
 #define SheDBits(p)      DDTYPE(p)->dbits
 #define SheColor(p)      DDTYPE(p)->color
#else
 #define SheShellNo(p)    ((p)->shellno)
 #define SheNextD(p)      ((p)->nextd)
 #define ShePrevD(p)      ((p)->prevd)
 #define SheSSolids(p)    ((p)->ssolids)
 #define SheSFaces(p)     ((p)->sfaces)
 #define SheSEdges(p)     ((p)->sedges)
 #define SheSVerts(p)     ((p)->sverts)
 #define SheBox(p)        ((p)->box)
 #define SheDBits(p)      ((p)->dbits)
 #define SheColor(p)      ((p)->color)
 #define SheSurf(p)       ((p)->surf)
 #define SheCurv(p)       ((p)->curv)
#endif

#ifdef __VIRTUAL_MEM
 #define FFTYPE(pp)       ((FTYPE *)access_v_rec(VirtualMemoryBlock[FACE], pp))
 #define FacFaceNo(p)     FFTYPE(p)->faceno
 #define FacFShell(p)     FFTYPE(p)->fshell
 #define FacNextF(p)      FFTYPE(p)->nextf
 #define FacPrevF(p)      FFTYPE(p)->prevf
 #define FacFLOut(p)      FFTYPE(p)->flout
 #define FacFLoops(p)     FFTYPE(p)->floops
 #define FacFeq(p)        FFTYPE(p)->feq
 #define FacBox(p)        FFTYPE(p)->box
 #define FacFBits(p)      FFTYPE(p)->fbits
 #define FacFVisi(p)      FFTYPE(p)->fvisi
 #define FacColor(p)      FFTYPE(p)->color
 #define FacFlag(p)       FFTYPE(p)->flag
 #define FacSurf(p)       FFTYPE(p)->surf
#else
 #define FacFaceNo(p)     ((p)->faceno)
 #define FacFShell(p)     ((p)->fshell)
 #define FacNextF(p)      ((p)->nextf)
 #define FacPrevF(p)      ((p)->prevf)
 #define FacFLOut(p)      ((p)->flout)
 #define FacFLoops(p)     ((p)->floops)
 #define FacFeq(p)        ((p)->feq)
 #define FacBox(p)        ((p)->box)
 #define FacFBits(p)      ((p)->fbits)
 #define FacFVisi(p)      ((p)->fvisi)
 #define FacColor(p)      ((p)->color)
 #define FacFlag(p)       ((p)->flag)
 #define FacSurf(p)       ((p)->surf)
#endif

#ifdef __VIRTUAL_MEM
 #define LLTYPE(pp)       ((LTYPE *)access_v_rec(VirtualMemoryBlock[LOOP], pp))
 #define LooLoopNo(p)     LLTYPE(p)->loopno
 #define LooLFace(p)      LLTYPE(p)->lface
 #define LooNextL(p)      LLTYPE(p)->nextl
 #define LooPrevL(p)      LLTYPE(p)->prevl
 #define LooLEdg(p)       LLTYPE(p)->ledg
 #define LooLength(p)     LLTYPE(p)->length
#else
 #define LooLoopNo(p)     ((p)->loopno)
 #define LooLFace(p)      ((p)->lface)
 #define LooNextL(p)      ((p)->nextl)
 #define LooPrevL(p)      ((p)->prevl)
 #define LooLEdg(p)       ((p)->ledg)
 #define LooLength(p)     ((p)->length)
#endif

#ifdef __VIRTUAL_MEM
 #define EETYPE(pp)       ((ETYPE *)access_v_rec(VirtualMemoryBlock[EDGE], pp))
 #define EdgEdgeNo(p)     EETYPE(p)->edgeno
 #define EdgNextE(p)      EETYPE(p)->nexte
 #define EdgPrevE(p)      EETYPE(p)->preve
 #define EdgHe1(p)        EETYPE(p)->he1
 #define EdgHe2(p)        EETYPE(p)->he2
 #define EdgAngle(p)      EETYPE(p)->angle
 #define EdgEVis(p)       EETYPE(p)->evis
 #define EdgEClp(p)       EETYPE(p)->eclp
 #define EdgSVClp3(p)     EETYPE(p)->svclp3
 #define EdgEVClp3(p)     EETYPE(p)->evclp3
 #define EdgSVClp2(p)     EETYPE(p)->svclp2
 #define EdgEVClp2(p)     EETYPE(p)->evclp2
 #define EdgColor(p)      EETYPE(p)->color
 #define EdgFgRound(p)    EETYPE(p)->fg_round
 #define EdgFgMake(p)     EETYPE(p)->fg_make
 #define EdgT1(p)         EETYPE(p)->t1
 #define EdgT2(p)         EETYPE(p)->t2
 #define EdgT3(p)         EETYPE(p)->t3
 #define EdgT4(p)         EETYPE(p)->t4
 #define EdgD(p)          EETYPE(p)->d
#else
 #define EdgEdgeNo(p)     ((p)->edgeno)
 #define EdgNextE(p)      ((p)->nexte)
 #define EdgPrevE(p)      ((p)->preve)
 #define EdgHe1(p)        ((p)->he1)
 #define EdgHe2(p)        ((p)->he2)
 #define EdgAngle(p)      ((p)->angle)
 #define EdgEVis(p)       ((p)->evis)
 #define EdgEClp(p)       ((p)->eclp)
 #define EdgSVClp3(p)     ((p)->svclp3)
 #define EdgEVClp3(p)     ((p)->evclp3)
 #define EdgSVClp2(p)     ((p)->svclp2)
 #define EdgEVClp2(p)     ((p)->evclp2)
 #define EdgColor(p)      ((p)->color)
 #define EdgFgRound(p)    ((p)->fg_round)
 #define EdgFgMake(p)     ((p)->fg_make)
 #define EdgT1(p)         ((p)->t1)
 #define EdgT2(p)         ((p)->t2)
 #define EdgT3(p)         ((p)->t3)
 #define EdgT4(p)         ((p)->t4)
 #define EdgD(p)          ((p)->d)
#endif

#ifdef __VIRTUAL_MEM
 #define HHTYPE(pp)       ((HTYPE *)access_v_rec(VirtualMemoryBlock[HALFEDGE], pp))
 #define HalWLoop(p)      HHTYPE(p)->wloop
 #define HalEdg(p)        HHTYPE(p)->edg
 #define HalNxt(p)        HHTYPE(p)->nxt
 #define HalPrv(p)        HHTYPE(p)->prv
 #define HalVtx(p)        HHTYPE(p)->vtx
 #define HalNextH(p)      HHTYPE(p)->nexth
 #define HalCurv(p)       HHTYPE(p)->curv
 #define HalR(p)          HHTYPE(p)->r
#else
 #define HalWLoop(p)      ((p)->wloop)
 #define HalEdg(p)        ((p)->edg)
 #define HalNxt(p)        ((p)->nxt)
 #define HalPrv(p)        ((p)->prv)
 #define HalVtx(p)        ((p)->vtx)
 #define HalCurv(p)       ((p)->curv)
 #define HalR(p)          ((p)->r)
#endif

#ifdef __VIRTUAL_MEM
 #define VVTYPE(pp)        ((VTYPE *)access_v_rec(VirtualMemoryBlock[VERTEX], pp))
 #define VerVertexNo(p)    VVTYPE(p)->vertexno
 #define VerVEdge(p)       VVTYPE(p)->vedge
 #define VerNextV(p)       VVTYPE(p)->nextv
 #define VerPrevV(p)       VVTYPE(p)->prevv
 #define VerVCoord(p)      VVTYPE(p)->vcoord
 #define VerFgRound(p)     VVTYPE(p)->fg_round
 #define VerFgMake(p)      VVTYPE(p)->fg_make
#else
 #define VerVertexNo(p)    ((p)->vertexno)
 #define VerVEdge(p)       ((p)->vedge)
 #define VerNextV(p)       ((p)->nextv)
 #define VerPrevV(p)       ((p)->prevv)
 #define VerVCoord(p)      ((p)->vcoord)
 #define VerFgRound(p)     ((p)->fg_round)
 #define VerFgMake(p)      ((p)->fg_make)
#endif

#ifdef __VIRTUAL_MEM
 #define GGTYPE(pp)       ((GTYPE *)access_v_rec(VirtualMemoryBlock[GROUP], pp))
 #define GrpGroupNo(p)    GGTYPE(p)->groupno
 #define GrpName(p)       GGTYPE(p)->gname
 #define GrpParent(p)     GGTYPE(p)->parent
 #define GrpGAxs(p)       GGTYPE(p)->gaxs
 #define GrpNextG(p)      GGTYPE(p)->nextg
 #define GrpPrevG(p)      GGTYPE(p)->prevg
 #define GrpGColor(p)     GGTYPE(p)->gcolor
#else
 #define GrpGroupNo(p)    ((p)->groupno)
 #define GrpName(p)       ((p)->gname)
 #define GrpParent(p)     ((p)->parent)
 #define GrpGAxs(p)       ((p)->gaxs)
 #define GrpNextG(p)      ((p)->nextg)
 #define GrpPrevG(p)      ((p)->prevg)
 #define GrpGColor(p)     ((p)->gcolor)
#endif

#ifdef __VIRTUAL_MEM
 #define AATYPE(pp)       ((ATYPE *)access_v_rec(VirtualMemoryBlock[AXIS], pp))
 #define AxsType(p)       AATYPE(p)->type
 #define AxsName(p)       AATYPE(p)->aname
 #define AxsAxisNo(p)     AATYPE(p)->axisno
 #define AxsMode(p)       AATYPE(p)->amode
 #define AxsVector(p)     AATYPE(p)->avector
 #define AxsCenter(p)     AATYPE(p)->acenter
 #define AxsNextA(p)      AATYPE(p)->nexta
 #define AxsPrevA(p)      AATYPE(p)->preva
 #define AxsAGroup(p)     AATYPE(p)->hierarq
 #define AxsASolid(p)     AATYPE(p)->hierarq
#else
 #define AxsType(p)       ((p)->type)
 #define AxsName(p)       ((p)->aname)
 #define AxsAxisNo(p)     ((p)->axisno)
 #define AxsMode(p)       ((p)->amode)
 #define AxsVector(p)     ((p)->avector)
 #define AxsCenter(p)     ((p)->acenter)
 #define AxsNextA(p)      ((p)->nexta)
 #define AxsPrevA(p)      ((p)->preva)
 #define AxsAGroup(p)     ((p)->hierarq.agroup)
 #define AxsASolid(p)     ((p)->hierarq.asolid)
#endif

#ifdef __VIRTUAL_MEM
 #define SPLSHELL(pp)        ((SF *)access_v_rec(SPL01VirtualArray, pp))
 #define SPLSOOV(pp)         ((VPTYPE *)access_v_rec(SPL02VirtualArray, (pp) * 4))
 #define SPLNBR(pp)          ((spl_NBR *)access_v_rec(SPL03VirtualArray, pp))
 #define SPLSONE(pp)         ((HPTYPE *)access_v_rec(SPL02VirtualArray, (pp) * 4 + 1))
 #define SPLENDS(pp)         ((HPTYPE *)access_v_rec(SPL02VirtualArray, (pp) * 4 + 2))
 #define SPLSONF(pp)         ((FPTYPE *)access_v_rec(SPL02VirtualArray, (pp) * 4 + 3))
 #define SplShellShell(p)    SPLSHELL(p)->shell
 #define SplShellSign(p)     SPLSHELL(p)->sign
 #define SplSoov(p)          * (SPLSOOV(p))
 #define SplNBRC1(p)         SPLNBR(p)->c1
 #define SplNBRSector(p)     SPLNBR(p)->sector
 #define SplSone(p)          * (SPLSONE(p))
 #define SplEnds(p)          * (SPLENDS(p))
 #define SplSonf(p)          * (SPLSONF(p))
#else
 #define SplShellShell(p)    spl_shellpos[p].shell
 #define SplShellSign(p)     spl_shellpos[p].sign
 #define SplSoov(p)          spl_soov[p]
 #define SplNBRC1(p)         spl_nbr[p].c1
 #define SplNBRSector(p)     spl_nbr[p].sector
 #define SplSone(p)          spl_sone[p]
 #define SplEnds(p)          spl_ends[p]
 #define SplSonf(p)          spl_sonf[p]
#endif

#ifdef __VIRTUAL_MEM
 #define SETASHELL(pp)      ((SFF *)access_v_rec(SET01VirtualArray, (pp) * 2))
 #define SETBSHELL(pp)      ((SFF *)access_v_rec(SET01VirtualArray, (pp) * 2 + 1))
 #define SETSONVV(pp)       ((SONVV *)access_v_rec(SET03VirtualArray, pp))
 #define SETASONVF(pp)      ((SONVF *)access_v_rec(SET04VirtualArray, (pp) * 2))
 #define SETBSONVF(pp)      ((SONVF *)access_v_rec(SET04VirtualArray, (pp) * 2 + 1))
 #define SETNBR(pp)         ((NBR *)access_v_rec(SET06VirtualArray, pp))
 #define SETSECTORS(pp)     ((SECTORS *)access_v_rec(SET07VirtualArray, pp))
 #define SETANBR(pp)        ((NB *)access_v_rec(SET08VirtualArray, (pp) * 2))
 #define SETBNBR(pp)        ((NB *)access_v_rec(SET08VirtualArray, (pp) * 2 + 1))
 #define SETSONEA(pp)       ((EPTYPE *)access_v_rec(SET10VirtualArray, (pp) * 6))
 #define SETSONEB(pp)       ((EPTYPE *)access_v_rec(SET10VirtualArray, (pp) * 6 + 1))
 #define SETSONFA(pp)       ((FPTYPE *)access_v_rec(SET10VirtualArray, (pp) * 6 + 2))
 #define SETSONFB(pp)       ((FPTYPE *)access_v_rec(SET10VirtualArray, (pp) * 6 + 3))
 #define SETENDSA(pp)       ((HPTYPE *)access_v_rec(SET10VirtualArray, (pp) * 6 + 4))
 #define SETENDSB(pp)       ((HPTYPE *)access_v_rec(SET10VirtualArray, (pp) * 6 + 5))
 #define SetASheShell(p)    SETASHELL(p)->shell
 #define SetASheSign(p)     SETASHELL(p)->sign
 #define SetBSheShell(p)    SETBSHELL(p)->shell
 #define SetBSheSign(p)     SETBSHELL(p)->sign
 #define SetVVva(p)         SETSONVV(p)->va
 #define SetVVvb(p)         SETSONVV(p)->vb
 #define SetAVFv(p)         SETASONVF(p)->v
 #define SetAVFf(p)         SETASONVF(p)->f
 #define SetBVFv(p)         SETBSONVF(p)->v
 #define SetBVFf(p)         SETBSONVF(p)->f
 #define SetNBRC1(p)        SETNBR(p)->c1
 #define SetNBRSector(p)    SETNBR(p)->sector
 #define SetSecA(p)         SETSECTORS(p)->secta
 #define SetSecB(p)         SETSECTORS(p)->sectb
 #define SetSecs1a(p)       SETSECTORS(p)->s1a
 #define SetSecs2a(p)       SETSECTORS(p)->s2a
 #define SetSecs1b(p)       SETSECTORS(p)->s1b
 #define SetSecs2b(p)       SETSECTORS(p)->s2b
 #define SetSecInter(p)     SETSECTORS(p)->intersect
 #define SetSecOnOn(p)      SETSECTORS(p)->onon
 #define SetANBHe(p)        SETANBR(p)->he
 #define SetANB1(p)         SETANBR(p)->ref1
 #define SetANB2(p)         SETANBR(p)->ref2
 #define SetANB12(p)        SETANBR(p)->ref12
 #define SetBNBHe(p)        SETBNBR(p)->he
 #define SetBNB1(p)         SETBNBR(p)->ref1
 #define SetBNB2(p)         SETBNBR(p)->ref2
 #define SetBNB12(p)        SETBNBR(p)->ref12
 #define SetSonea(p)        * (SETSONEA(p))
 #define SetSoneb(p)        * (SETSONEB(p))
 #define SetSonfa(p)        * (SETSONFA(p))
 #define SetSonfb(p)        * (SETSONFB(p))
 #define SetEndsa(p)        * (SETENDSA(p))
 #define SetEndsb(p)        * (SETENDSB(p))
#else
 #define SetASheShell(p)    shellposa[p].shell
 #define SetASheSign(p)     shellposa[p].sign
 #define SetBSheShell(p)    shellposb[p].shell
 #define SetBSheSign(p)     shellposb[p].sign
 #define SetVVva(p)         sonvv[p].va
 #define SetVVvb(p)         sonvv[p].vb
 #define SetAVFv(p)         sonva[p].v
 #define SetAVFf(p)         sonva[p].f
 #define SetBVFv(p)         sonvb[p].v
 #define SetBVFf(p)         sonvb[p].f
 #define SetNBRC1(p)        nbr[p].c1
 #define SetNBRSector(p)    nbr[p].sector
 #define SetSecA(p)         sectors[p].secta
 #define SetSecB(p)         sectors[p].sectb
 #define SetSecs1a(p)       sectors[p].s1a
 #define SetSecs2a(p)       sectors[p].s2a
 #define SetSecs1b(p)       sectors[p].s1b
 #define SetSecs2b(p)       sectors[p].s2b
 #define SetSecInter(p)     sectors[p].intersect
 #define SetSecOnOn(p)      sectors[p].onon
 #define SetANBHe(p)        nba[p].he
 #define SetANBLig(p)       nba[p].lig_he
 #define SetANB1(p)         nba[p].ref1
 #define SetANB2(p)         nba[p].ref2
 #define SetANB12(p)        nba[p].ref12
 #define SetBNBHe(p)        nbb[p].he
 #define SetBNBLig(p)       nbb[p].lig_he
 #define SetBNB1(p)         nbb[p].ref1
 #define SetBNB2(p)         nbb[p].ref2
 #define SetBNB12(p)        nbb[p].ref12
 #define SetSonea(p)        sonea[p]
 #define SetSoneb(p)        soneb[p]
 #define SetSonfa(p)        sonfa[p]
 #define SetSonfb(p)        sonfb[p]
 #define SetEndsa(p)        endsa[p]
 #define SetEndsb(p)        endsb[p]
#endif


/* Definicoes para implementar memoria virtual para os ponteiros nulos.  */
#ifdef __VIRTUAL_MEM
 #define SNIL     ((long)-1)
 #define DNIL     ((long)-1)
 #define FNIL     ((long)-1)
 #define LNIL     ((long)-1)
 #define ENIL     ((long)-1)
 #define HNIL     ((long)-1)
 #define VNIL     ((long)-1)
 #define NNIL     ((long)-1)
 #define ANIL     ((long)-1)
 #define GNIL     ((long)-1)
 #define EuNIL    ((long)-1)
#else
 #define SNIL     ((STYPE *)NIL)
 #define DNIL     ((DTYPE *)NIL)
 #define FNIL     ((FTYPE *)NIL)
 #define LNIL     ((LTYPE *)NIL)
 #define ENIL     ((ETYPE *)NIL)
 #define HNIL     ((HTYPE *)NIL)
 #define VNIL     ((VTYPE *)NIL)
 #define NNIL     ((NTYPE *)NIL)
 #define ANIL     ((ATYPE *)NIL)
 #define GNIL     ((GTYPE *)NIL)
 #define EuNIL    ((EulerOp *)NIL)
 #define CNIL     ((CURVE *)NIL)
#endif

#ifndef __VIRTUAL_MEM
 #define SPTYPE      STYPE *
 #define DPTYPE      DTYPE *
 #define FPTYPE      FTYPE *
 #define LPTYPE      LTYPE *
 #define EPTYPE      ETYPE *
 #define HPTYPE      HTYPE *
 #define VPTYPE      VTYPE *
 #define GPTYPE      GTYPE *
 #define APTYPE      ATYPE *
 #define NPTYPE      NTYPE *
 #define PEulerOp    EulerOp *
#else
 #define PEulerOp    long
 #define SPTYPE      long
 #define DPTYPE      long
 #define FPTYPE      long
 #define LPTYPE      long
 #define EPTYPE      long
 #define HPTYPE      long
 #define VPTYPE      long
 #define GPTYPE      long
 #define APTYPE      long
 #define NPTYPE      long
#endif

#ifdef __VIRTUAL_MEM
 #define MSD_checkVertex(x)      assert((x) < ArrayBottom[VERTEX] && (x) > -1)
 #define MSD_checkHalfedge(x)    assert((x) < ArrayBottom[HALFEDGE] && (x) > -1)
 #define MSD_checkEdge(x)        assert((x) < ArrayBottom[EDGE] && (x) > -1)
 #define MSD_checkLoop(x)        assert((x) < ArrayBottom[LOOP] && (x) > -1)
 #define MSD_checkFace(x)        assert((x) < ArrayBottom[FACE] && (x) > -1)
 #define MSD_checkShell(x)       assert((x) < ArrayBottom[SHELL] && (x) > -1)
 #define MSD_checkSolid(x)       assert((x) < ArrayBottom[SOLID] && (x) > -1)
#endif
#endif
