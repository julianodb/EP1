/* 
  PROJETO USPDesigner
  MODULO: GPH (Interface Grafica)
  Copyright (C) 1989 a 2008, Marcos Tsuzuki, All rights reserved
  Universidade de Sao Paulo, EPUSP-PMR

   NOME DO ARQUIVO: gphlintf.cpp
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
#ifndef __UNIX_
 #include <stdlib.h>
 #ifndef _Windows
  #include <graphics.h>
 #else
  #include <windows.h>
 #endif
#endif
#include "memvirtu.h"
#include "lowparam.h"
#include "lowsolid.h"
#include "disdispl.h"
#include "gphgraph.h"
#include "animatio.h"
#ifdef _Windows
 #include "../basicViewer/cBasicViewer.h"
#endif

#ifdef __UNIX_
extern GC           XViewGraphContext;
extern Display      *XDisplay;
extern unsigned int ViewWidth, ViewHeight;
extern Window       ViewWndw;
#else
 #ifdef _Windows
//! Pointer to viewer
cBasicViewer *viewer;

//! Brush
HGDIOBJ *Brushes;

  #define SOLID_LINE     PS_SOLID
  #define DASHED_LINE    PS_DASH
  #define ERROR          0

COLORREF BGI__Colors[16];  // The RGB values for the Borland 16 colors, misc.cpp
 #else
int GraphDriver, GraphMode, GraphError;
 #endif
int MaxColors;
#endif
int MaxX, MaxY;

void g_frame(void)
{
   int xmin, ymin, xmax, ymax;

   xmin = (int)(MaxX * (dwindbox[0]));
   ymin = (int)(MaxY * (1.0 - dwindbox[1]));
   xmax = (int)(MaxX * (dwindbox[0] + dwindbox[2]));
   ymax = (int)(MaxY * (1.0 - dwindbox[1] - dwindbox[2]));
#ifdef __UNIX_
   /* This code is used to display the frame box using the X-windows */
   if (Draw)
   {
      GGMySetLineStyle(SOLID_LINE);
      GGMySetColor(framecol);
      XDrawLine(XDisplay, ViewWndw, XViewGraphContext, xmin, ymin, xmax, ymin);
      XDrawLine(XDisplay, ViewWndw, XViewGraphContext, xmax, ymin, xmax, ymax);
      XDrawLine(XDisplay, ViewWndw, XViewGraphContext, xmax, ymax, xmin, ymax);
      XDrawLine(XDisplay, ViewWndw, XViewGraphContext, xmin, ymax, xmin, ymin);
   }
#else
   /* This code is used to display the frame box using the TurboC */
   if (Draw)
   {
 #ifdef _Windows
      POINT *polygon;

      HDC     hdc  = viewer->BeginDraw();
      HGDIOBJ hPen = SelectObject(hdc, CreatePen(SOLID_LINE, 1, BGI__Colors[0]));
      polygon      = new POINT[4];
      polygon[0].x = xmin;
      polygon[0].y = ymin;
      polygon[1].x = xmin;
      polygon[1].y = ymax;
      polygon[2].x = xmax;
      polygon[2].y = ymax;
      polygon[3].x = xmax;
      polygon[3].y = ymin;
      Polygon(hdc, polygon, 4);
      delete polygon;
      viewer->EndDraw();
      viewer->Update();
      DeleteObject(SelectObject(hdc, hPen));
 #else
      setlinestyle(SOLID_LINE, 0, NORM_WIDTH);
      setcolor(framecol);
      rectangle(xmin, ymin, xmax, ymax);
 #endif
   }
#endif
   if (dspfwptr)
   {
      framenum += 4;
      fprintf(dspfwptr, "%2d %2d %10.6f %10.6f %10.6f %10.6f\n",
              framecol, SOLID_LINE, -1.0, -xyratio, -1.0, xyratio);
      fprintf(dspfwptr, "%2d %2d %10.6f %10.6f %10.6f %10.6f\n",
              framecol, SOLID_LINE, -1.0, xyratio, 1.0, xyratio);
      fprintf(dspfwptr, "%2d %2d %10.6f %10.6f %10.6f %10.6f\n",
              framecol, SOLID_LINE, 1.0, xyratio, 1.0, -xyratio);
      fprintf(dspfwptr, "%2d %2d %10.6f %10.6f %10.6f %10.6f\n",
              framecol, SOLID_LINE, 1.0, -xyratio, -1.0, -xyratio);
   }
}

void g_tpoint(int lt, vect2 v)
{
   int gdcsv[2];

   if (lncolor != -1)
   {
      gdcpos(v, gdcsv);
      if (Draw)
      {
//            setlinestyle(lt, 0, NORM_WIDTH) ;
//            setcolor(lncolor) ;
//            circle(gdcsv[0], gdcsv[1], 2) ;
      }
   }
}

void g_iline(int lt, int *gdcsv, int *gdcev)
{
   if (lncolor != -1)
   {
#ifdef __UNIX_
      /* Display a line using the X-windows */
      if (Draw)
      {
         GGMySetLineStyle(lt);
         GGMySetColor(lncolor);
         XDrawLine(XDisplay, ViewWndw, XViewGraphContext, gdcsv[0], gdcsv[1],
                   gdcev[0], gdcev[1]);
      }
#else
      /* display a line using the TurboC */
      if (Draw)
      {
 #ifdef _Windows
         POINT   *polygon;
         HGDIOBJ hPen;

         HDC hdc = viewer->BeginDraw();
         switch (lt)
         {
             case DASHED_LINE:
             hPen = SelectObject(hdc, CreatePen(PS_DOT, 1, BGI__Colors[lncolor]));
             break;

             case SOLID_LINE:
             hPen = SelectObject(hdc, CreatePen(PS_SOLID, 1, BGI__Colors[lncolor]));
             break;
         }
         polygon      = new POINT[2];
         polygon[0].x = gdcsv[0];
         polygon[0].y = gdcsv[1];
         polygon[1].x = gdcev[0];
         polygon[1].y = gdcev[1];
         Polygon(hdc, polygon, 2);
         delete polygon;
         viewer->EndDraw();
         viewer->Update();
         DeleteObject(SelectObject(hdc, hPen));
 #else
         setlinestyle(lt, 0, NORM_WIDTH);
         setcolor(lncolor);
         line(gdcsv[0], gdcsv[1], gdcev[0], gdcev[1]);
 #endif
      }
#endif
   }
}

void g_tline(int lt, vect2 sv, vect2 ev)
{
   int gdcsv[2], gdcev[2];

   if (lncolor != -1)
   {
      gdcpos(sv, gdcsv);
      gdcpos(ev, gdcev);
#ifdef __UNIX_
      /* Display a line using the X-windows */
      if (Draw)
      {
         GGMySetLineStyle(lt);
         GGMySetColor(lncolor);
         XDrawLine(XDisplay, ViewWndw, XViewGraphContext, gdcsv[0], gdcsv[1],
                   gdcev[0], gdcev[1]);
      }
#else
      /* display a line using the TurboC */
      if (Draw)
      {
 #ifdef _Windows
         POINT   *polygon;
         HGDIOBJ hPen;

         HDC hdc = viewer->BeginDraw();
         switch (lt)
         {
             case DASHED_LINE:
             hPen = SelectObject(hdc, CreatePen(PS_DOT, 1, BGI__Colors[lncolor]));
             break;

             case SOLID_LINE:
             hPen = SelectObject(hdc, CreatePen(PS_SOLID, 1, BGI__Colors[lncolor]));
             break;
         }
         polygon      = new POINT[2];
         polygon[0].x = gdcsv[0];
         polygon[0].y = gdcsv[1];
         polygon[1].x = gdcev[0];
         polygon[1].y = gdcev[1];
         Polygon(hdc, polygon, 2);
         delete polygon;
         viewer->EndDraw();
         viewer->Update();
         DeleteObject(SelectObject(hdc, hPen));
 #else
         setlinestyle(lt, 0, NORM_WIDTH);
         setcolor(lncolor);
         line(gdcsv[0], gdcsv[1], gdcev[0], gdcev[1]);
 #endif
      }
#endif
      if (dspfwptr)
      {
         framenum++;
         fprintf(dspfwptr, "%2d %2d %10.6f %10.6f %10.6f %10.6f\n",
                 lncolor, lntype, sv[0], sv[1], ev[0], ev[1]);
      }
   }
}

void h_line(void)
{
   int   col, typ;
   float x1, y1, x2, y2;
   vect2 sv, ev;

   if (fscanf(dspfrptr, "%d %d %f %f %f %f", &col, &typ, &x1, &y1, &x2, &y2) == 6)
   {
      sv[0]   = x1;
      sv[1]   = y1;
      ev[0]   = x2;
      ev[1]   = y2;
      lncolor = col;
      g_tline(typ, sv, ev);
   }
}

void g_open(void)
{
#ifdef __UNIX_
   GGInitGraph(argc, argv, FALSE, FALSE);
   MaxX = ViewWidth;
   MaxY = ViewHeight;
#else
 #ifndef _Windows
   GraphMode   = 1;
   GraphDriver = DETECT;
   initgraph(&GraphDriver, &GraphMode, "");
   if ((GraphError = graphresult()) != grOk)
   {
      printf("initgraph error: %s\n", grapherrormsg(GraphError));
      exit(1);
   }
   MaxX      = getmaxx();
   MaxY      = getmaxy();
   MaxColors = getmaxcolor() + 1;
 #else
   viewer = new cBasicViewer(700, 550, "USPDesigner ver. 1.3");

   HDC hdc = viewer->BeginDraw();
   SelectObject(hdc, GetStockObject(BLACK_PEN));
   SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));
   viewer->EndDraw();
   g_clear();
   viewer->Show();

   MaxX = viewer->GetWidth();
   MaxY = viewer->GetHeight();

   MaxColors = 50;

   BGI__Colors[0]  = RGB(0, 0, 0);           // Black
   BGI__Colors[1]  = RGB(0, 0, 128);         // Blue
   BGI__Colors[2]  = RGB(0, 128, 0);         // Green
   BGI__Colors[3]  = RGB(0, 128, 128);       // Cyan
   BGI__Colors[4]  = RGB(128, 0, 0);         // Red
   BGI__Colors[5]  = RGB(128, 0, 128);       // Magenta
   BGI__Colors[6]  = RGB(128, 128, 0);       // Brown
   BGI__Colors[7]  = RGB(192, 192, 192);     // Light Gray
   BGI__Colors[8]  = RGB(128, 128, 128);     // Dark Gray
   BGI__Colors[9]  = RGB(128, 128, 255);     // Light Blue
   BGI__Colors[10] = RGB(128, 255, 128);     // Light Green
   BGI__Colors[11] = RGB(128, 255, 255);     // Light Cyan
   BGI__Colors[12] = RGB(255, 128, 128);     // Light Red
   BGI__Colors[13] = RGB(255, 128, 255);     // Light Magenta
   BGI__Colors[14] = RGB(255, 255, 0);       // Yellow
   BGI__Colors[15] = RGB(255, 255, 255);     // White
 #endif
#endif
}

void g_filbox(int color, vect2 v0, vect2 v1)
{
   int d0[2], d1[2];

//    Rect rect;

#ifdef __UNIX_
#else
   gdcpos(v0, d0);
   gdcpos(v1, d1);
 #ifdef _Windows
   // --> Clear the view
   RECT r;
   r.top    = d0[0];
   r.left   = d0[1];
   r.bottom = d1[0];
   r.right  = d1[1];

   HDC hdc = viewer->BeginDraw();
   FillRect(hdc, &r, (HBRUSH)GetStockObject(WHITE_BRUSH));
   viewer->EndDraw();
   viewer->Update();
 #else
   setfillpattern(EMPTY_FILL, color);
   bar(d0[0], d0[1], d1[0], d1[1]);
 #endif
#endif
}

void g_clear(void)
{
#ifdef __UNIX_
   GGClearViewArea();
#else
 #ifdef _Windows
   // --> Clear the view
   RECT r;
   r.top    = 0;
   r.left   = 0;
   r.bottom = viewer->GetHeight();
   r.right  = viewer->GetWidth();

   HDC hdc = viewer->BeginDraw();
   FillRect(hdc, &r, (HBRUSH)GetStockObject(WHITE_BRUSH));
   viewer->EndDraw();
   viewer->Update();
 #else
   cleardevice();
 #endif
#endif
}

void g_pcolor(int color)
{
   lncolor = color;
}

void g_close(void)
{
#ifdef __UNIX_
   GGCloseGraph();
#else
 #ifdef _Windows
   delete viewer;
 #else
   closegraph();
 #endif
#endif
}

void gdcpos(vect2 npos, int *dpos)
{
   dpos[0] = (int)(MaxX * (dwindbox[0] + (npos[0] + 1.0) / 2.0 * dwindbox[2]));
   dpos[1] = (int)(MaxY * (1.0 - dwindbox[1] - (npos[1] + xyratio) / xyratio
                           / 2.0 * dwindbox[2]));
}
