/* 
  PROJETO USPDesigner
  MODULO: GPH (Interface Grafica)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: gphgraph.h
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

#ifndef __GphGraph_h
#define __GphGraph_h

#ifndef EXTERN
 #ifdef _DVS_
  #define EXTERN
 #else
  #define EXTERN    extern
 #endif
#endif

#include <stdio.h>

#ifdef __UNIX_
 #include <X11/Xlib.h>
 #include <X11/Xutil.h>
 #include <X11/cursorfont.h>
 #include <X11/Xresource.h>
#endif

#ifndef __double_real
 #define __double_real
typedef double   real;
#endif
#ifndef __double_vector
 #define __double_vector
typedef real     vector[4];
#endif

#ifdef __UNIX_
 #define DASHED_LINE    0
 #define SOLID_LINE     1
#endif

EXTERN vector windbox;
EXTERN vector dwindbox;
EXTERN double releps;

#ifdef  BLACK
 #undef  BLACK
#endif
#define BLACK    0 /* Must be inorder of XViewColorDefs - xgrphgen.c. */
#ifdef  RED
 #undef  RED
#endif
#define RED    4
#ifdef  GREEN
 #undef  GREEN
#endif
#define GREEN    2
#ifdef  BLUE
 #undef  BLUE
#endif
#define BLUE    1
#ifdef  YELLOW
 #undef  YELLOW
#endif
#define YELLOW    14
#ifdef  CYAN
 #undef  CYAN
#endif
#define CYAN    3
#ifdef  MAGENTA
 #undef  MAGENTA
#endif
#define MAGENTA    5
#ifdef  WHITE
 #undef  WHITE
#endif
#define WHITE        15
#define MAX_COLOR    15

#ifndef __SUIT_
/* Name of generic matric transformation saved by this program: */
 #define GENERIC_MAT_FILE             "generic#.mat" /* Generic mat file name to save. */
 #define GENERIC_GIF_FILE             "generic#.gif" /* Generic gif file name to save. */
 #define GENERIC_PS_FILE              "generic#.ps"  /* Generic PS file name to save. */

/* The current NormalLength is divided by scalar to form real normal length: */
 #define NORMAL_DEFAULT_LENGTH        10
 #define NORMAL_SCALER_LENGTH         100

 #define DEFAULT_COLOR                1    /* For objects with no color defined. */

 #define VIEW_PERSPECTIVE             1    /* View modes. */
 #define VIEW_ORTHOGRAPHIC            2
 #define DEFAULT_PERSPECTIVE_Z        -5.0 /* Default Z focal point. */

 #define TRANS_SCREEN                 1    /* Screen, Object coords. transformation mode. */
 #define TRANS_OBJECT                 2

 #define DEFAULT_NUM_OF_ISOLINES      10
 #define DEFAULT_SAMPLES_PER_CURVE    6
 #define LINE_LEN_LONG                256 /* Lines read from stdin/files... */

typedef enum
{
   EVENT_NONE,
   EVENT_QUIT,
   EVENT_SCR_OBJ_TGL,
   EVENT_PERS_ORTHO_TGL,
   EVENT_PERS_ORTHO_Z,
   EVENT_ROTATE_X,
   EVENT_ROTATE_Y,
   EVENT_ROTATE_Z,
   EVENT_TRANSLATE_X,
   EVENT_TRANSLATE_Y,
   EVENT_TRANSLATE_Z,
   EVENT_SCALE,
   EVENT_DEPTH_CUE,
   EVENT_DRAW_SOLID,
   EVENT_SAVE_MATRIX,
   EVENT_SAVE_GIF,
   EVENT_SAVE_PS,
   EVENT_RESET_MATRIX
} GraphicEventType;

EXTERN int
GlblDoGraphics,                /* If FALSE, disable graphics. */
GlblTransformMode,             /* Screen, Object coords. trans. mode. */
GlblViewMode,                  /* Perspective, Orthographic etc. */
GlblDepthCue;                  /* Activate depth cueing. */

/* file ghpclear.c */
void MSD_execClear(void);
void MSD_lowClearWindow(void);

/* file gphprint.c */
void MSD_execPrint(void);
void MSD_highPrint(FILE *fptr, char *mens);
void MSD_lowPrint(char *to, char *from);

/* file gphwindw.c */
void MSD_execWindow(void);

int MSD_highWindow(real, real, real);
char MSD_lowCheckWindow(real x, real y, real r);

/* file gphx11__.c */
 #ifdef __UNIX_
int GGMapX(real x);
int GGMapY(real y);
void GGMyMove(real x, real y);
void GGMyDraw(real x, real y);
void GGMySetLineStyle(int Style);
void GGMySetColor(int color);
void GGInitGraph(int argc, char **argv, int FullTransMode, int TransAlways);
void GGCloseGraph(void);
void GGPutMsgXY(char *s, real x, real y);
void GGClearViewArea(void);
void GGTone(int Frequency, int Duration);
char *ReadOneXDefault(char *Entry);
void ReadXDefaults(void);
void SetTransformWindow(int argc, char **argv);
void RedrawTransformWindow(void);
Window SetTransformSubWindow(int SubTransPosX, int SubTransPosY,
                             unsigned int SubTransWidth, unsigned int SubTransHeight);
void RedrawTransformSubWindow(Window Win, int SubTransPosX, int SubTransPosY,
                              unsigned int SubTransWidth, unsigned int SubTransHeight,
                              int DrawMiddleVertLine, char *DrawString);
void SetViewWindow(int argc, char **argv);
long GGGetViewWindowID(long *XDisplayAddr, long *XColorMapID);
int GGIsAbortKeyPressed(void);
void GGGraphicFlush(void);
GraphicEventType GGGetGraphicEvent(real *ChangeFactor);
void DrawText(Window Win, char *Str, int PosX, int PosY, unsigned long Color);

 #endif

 #ifdef _Windows
  #define SOLID_LINE     PS_SOLID
  #define DASHED_LINE    PS_DASH
 #endif

#endif
#endif
