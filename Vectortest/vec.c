/*
 * 3D stuff to test implementation of tranformations and triangle rendering
 * (c) 1994-1995 Kevin Phair
 */

#include <exec/exec.h>
#include <proto/exec.h>
#include <dos/dos.h>
#include <dos/dosextens.h>
#include <intuition/intuition.h>
#include <intuition/intuitionbase.h>
#include <intuition/gadgetclass.h>
#include <intuition/screens.h>
#include <graphics/gfxbase.h>
#include <graphics/text.h> 
#include <workbench/startup.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <dos.h>
#include <exec/exec.h>
#include <proto/intuition.h>
#include <proto/graphics.h>
#include <proto/wb.h>
#include <clib/alib_protos.h>

#define INTUITION_REV 39
#define GRAPHICS_REV 39

#define pal_range 32L

struct Screen *Screen;
struct Window *Window;
struct RastPort *demorast;
struct ViewPort *demoview;

UBYTE *fbuffer0=0;

char pubscreenname[]="Workbench";

char windowtitle[]="3D Polygon Window - fps:        ";


/* struct NewWindow vectorwindow={
    10,20,320,240,
    2,3,
    CLOSEWINDOW,
    WINDOWCLOSE | WINDOWDRAG | WINDOWDEPTH | GIMMEZEROZERO,
    NULL,
    NULL,
    windowtitle,
    NULL,
    NULL,
    320,240,
    360,240,
    WBENCHSCREEN,
};
*/

void rasterise(int,int,int,int);
void kev_triangle(int,int,int,int,int,int,int);

int x_scan1[240]={999}, x_scan2[240]={999};
int *scan1=&x_scan1[0], *scan2=&x_scan2[0];

void main() {

    SHORT i,fcolr,t;
    long xf=0, yf=0, zf=150;
    long winmidx=160,winmidy=120;
    SHORT xr=0, yr=0, zr=0;
    long x0,y0,z0,x1,y1,z1,x2,y2,z2;
    long sinx,siny,sinz;
    long cosx,cosy,cosz;
    long xt,yt,eax,ebx;

    unsigned int clock[2], fpscount;
    ULONG etime,etime0,etime1;
    ULONG rval,gval,bval,pidx;

    long sine[720]={
        #include "long_sin.dat"
    };
    long cosine[720]={
        #include "long_cos.dat"
    };

    SHORT polys[240][8]={0};
    SHORT lst_end, polycount=0;
    long depth, width, height;
    SHORT distance;

    long plist[]={
        #include "cube2.dat"
    999};

    for (i=0;i<240;i++) { scan1[i]=999; scan2[i]=999; }

    if (!(fbuffer0=calloc(19200,4)))
        goto failbufalloc;

    if(!(IntuitionBase=(struct IntuitionBase *)OpenLibrary("intuition.library",INTUITION_REV)))
        goto failintuilib;

    if (!(GfxBase=(struct GfxBase *)OpenLibrary("graphics.library",GRAPHICS_REV)))
        goto failgfxlib;

    if (!(Screen=LockPubScreen(pubscreenname))) goto failscreen;

    //vectorwindow.Screen=Screen;
    if (!(Window=OpenWindowTags(NULL,
            WA_Left,30, WA_Top,20,
            WA_InnerWidth,320, WA_InnerHeight,240,
            WA_CloseGadget,    TRUE,
            WA_DragBar,        TRUE,
            WA_DepthGadget,    TRUE,
            WA_Title,          "3D Polygon Window",
            WA_PubScreen,      Screen,
            WA_GimmeZeroZero,  TRUE,
            WA_IDCMP,          IDCMP_CLOSEWINDOW,
            TAG_END)))
        goto failwindow;

    demorast=Window->RPort;
    demoview=&(Window->WScreen->ViewPort);

    for(pidx=0;pidx<pal_range;pidx++) {

        rval=((60<<16L)+((255-60)*pidx<<16L)/(pal_range-1))<<8;
        gval=((50<<16L)+((255-50)*pidx<<16L)/(pal_range-1))<<8;
        bval=((0<<16L)+((160)*pidx<<16L)/(pal_range-1))<<8;

        SetRGB32(demoview,pidx+32,rval,gval,bval);
    }

    timer(clock); etime0=clock[0]*1000+clock[1];

    i=0;
    while (1) {

        t=plist[i++];
        if (t!=999) {
            x0=plist[i++]; y0=plist[i++]; z0=plist[i++];
            x1=plist[i++]; y1=plist[i++]; z1=plist[i++];
            x2=plist[i++]; y2=plist[i++]; z2=plist[i++];

         // * Get sine and cosine values from precalc table
            cosx = cosine[xr % 360 + 360]; sinx = sine[xr % 360 + 360];
            cosy = cosine[yr % 360 + 360]; siny = sine[yr % 360 + 360];
            cosz = cosine[zr % 360 + 360]; sinz = sine[zr % 360 + 360];

         // * Rotate first point and add offsets
            yt = ((y0 * cosx - z0 * sinx)>>16);
            z0 = ((z0 * cosx + y0 * sinx)>>16); y0 = yt;
            xt = ((x0 * cosy - z0 * siny)>>16);
            z0 = ((z0 * cosy + x0 * siny)>>16); x0 = xt;
            xt = ((x0 * cosz - y0 * sinz)>>16);
            y0 = ((y0 * cosz + x0 * sinz)>>16); x0 = xt;
            x0+=xf; y0+=yf; z0+=zf;

         // * Rotate second point and add offsets
            yt = ((y1 * cosx - z1 * sinx)>>16);
            z1 = ((z1 * cosx + y1 * sinx)>>16); y1 = yt;
            xt = ((x1 * cosy - z1 * siny)>>16);
            z1 = ((z1 * cosy + x1 * siny)>>16); x1 = xt;
            xt = ((x1 * cosz - y1 * sinz)>>16);
            y1 = ((y1 * cosz + x1 * sinz)>>16); x1 = xt;
            x1+=xf; y1+=yf; z1+=zf;

         // * Rotate third point and add offsets
            yt = ((y2 * cosx - z2 * sinx)>>16);
            z2 = ((z2 * cosx + y2 * sinx)>>16); y2 = yt;
            xt = ((x2 * cosy - z2 * siny)>>16);
            z2 = ((z2 * cosy + x2 * siny)>>16); x2 = xt;
            xt = ((x2 * cosz - y2 * sinz)>>16);
            y2 = ((y2 * cosz + x2 * sinz)>>16); x2 = xt;
            x2+=xf; y2+=yf; z2+=zf;

            if (z0>-512 && z1>-512 && z2>-512) {

             // * Get depth of triangle
                eax=z0;
                if (z1<eax) eax=z1;
                if (z2<eax) eax=z2;
                ebx=z0;
                if (z1>ebx) ebx=z1;
                if (z2>ebx) ebx=z2;
                depth=(ebx-eax);
                distance=eax;

             // * Get width of triangle
                eax=x0;
                if (x1<eax) eax=x1;
                if (x2<eax) eax=x2;
                ebx=x0;
                if (x1>ebx) ebx=x1;
                if (x2>ebx) ebx=x2;
                width=(ebx-eax);

             // * Get height of triangle
                eax=y0;
                if (y1<eax) eax=y1;
                if (y2<eax) eax=y2;
                ebx=y0;
                if (y1>ebx) ebx=y1;
                if (y2>ebx) ebx=y2;
                height=(ebx-eax);

             // * Find slope of inclincation of the triangle.
             // * An inverse TAN lookup table would give the actual angle
                width=(height*height)+(width*width);
                depth=(depth*depth);
                if (depth>width)
                    fcolr=32+(((pal_range/2)*width)/depth);    // 45-90° to viewer
                else
                    fcolr=32+pal_range-1-(((pal_range/2)*depth)/width);    // 0-45° to viewer


             // * Project points onto picture plane
                z0 = (0x1000000L / (z0 + 512));
                z1 = (0x1000000L / (z1 + 512));
                z2 = (0x1000000L / (z2 + 512));
                x0 = (winmidx + ((x0 * z0)>>16));
                y0 = (winmidy - ((y0 * z0)>>16));
                x1 = (winmidx + ((x1 * z1)>>16));
                y1 = (winmidy - ((y1 * z1)>>16));
                x2 = (winmidx + ((x2 * z2)>>16));
                y2 = (winmidy - ((y2 * z2)>>16));

             // * Insert sort it into the polygon array


                lst_end=polycount;
                while((lst_end>0) && (distance<polys[lst_end-1][0])) lst_end--;

                memmove(&polys[lst_end+1][0],&polys[lst_end][0],16*(polycount-lst_end));
                polys[lst_end][0]=distance;
                polys[lst_end][1]=fcolr;
                polys[lst_end][2]=x0;
                polys[lst_end][3]=y0;
                polys[lst_end][4]=x1;
                polys[lst_end][5]=y1;
                polys[lst_end][6]=x2;
                polys[lst_end][7]=y2;

                polycount++;
            }
        }

     // * Go backwards through list of sorted triangles
     // * drawing them with raster fill
        else {
            while (--polycount>=0) {
                kev_triangle(polys[polycount][2], polys[polycount][3],
                             polys[polycount][4], polys[polycount][5],
                             polys[polycount][6], polys[polycount][7],
                             polys[polycount][1]);
                polys[polycount][0]=32000;
            }
            polycount=0;

            WriteChunkyPixels(demorast,0,0,320,240,fbuffer0,320);
            memset(fbuffer0,1,76800);

            timer(clock); etime1=clock[0]*1000000+clock[1];
            etime=etime1-etime0; etime0=etime1;

            fpscount=(unsigned int) 1000000L/etime;
            sprintf(windowtitle,"3D Polygon Window - fps:%d",fpscount);
            SetWindowTitles(Window,windowtitle,(UBYTE *)~0);

            i=0;
            yr+=3; xr+=2; zr+=1;
            zf=150+sine[xr % 360 + 360]>>9;
            if (GetMsg(Window->UserPort)) {
                CloseWindow(Window);
failwindow:     UnlockPubScreen(pubscreenname,Screen);
failscreen:     CloseLibrary((struct Library *)GfxBase);
failintuilib:   CloseLibrary((struct Library *)IntuitionBase);
failgfxlib:     free(fbuffer0);
failbufalloc:   exit(TRUE);
            }
        }
    }
}

void kev_triangle(int x0,int y0,int x1,int y1,int x2,int y2,int colr) {

    int i,x,dx;
    UBYTE *fras;
    int *scanx1=scan1, *scanx2=scan2;

    rasterise(x0,y0,x1,y1);
    rasterise(x1,y1,x2,y2);
    rasterise(x2,y2,x0,y0);

    fras=fbuffer0;

    for (i=240;i>0;i--) {
        if (*scanx2!=999) {

            dx=(*scanx1)-(*scanx2);
            if (dx<0) {
                dx=dx*-1;
                x=(*scanx1);
            } else {
                x=(*scanx2);
            }
            dx++;
            memset(fras+x,colr,dx);
        }
        *(scanx1++)=999; *(scanx2++)=999;
        fras+=320;
    }
}

void rasterise(int x0, int y0, int x1, int y1) {

    int xt,yt,x,height,vertc;
    int *scanx1, *scanx2;
    long dfx;

    if (y1<y0) { xt=x0; yt=y0; x0=x1; y0=y1; x1=xt; y1=yt; }    // Swap axes maybe

    scanx1=(scan1+y0);scanx2=(scan2+y0);

    if (y0<240) {
        if (y0>=0) {
            x=x0;
            if (x<0) x=0; else if (x>319) x=319;
            if (*scanx1==999) *scanx1=x;
            else if (*scanx2==999 || *scanx1!=x) *scanx2=x;
        }
        if (y1>y0 && y1>0) {
            height=y1-y0;

            dfx=((long)(x1-x0)<<8)/height;

            if (y0>=0) {
                vertc=1;
                y0++;
            }
            else {
                vertc=1-y0;
                scanx1=scan1;scanx2=scan2;
                y0=0;
            }

            while (vertc<=height && (y0<240)) {
                scanx1++;scanx2++;
                if (y0>=0) {
                    x=x0+((128+dfx*vertc)>>8);
                    if (x<0) x=0; else if (x>319) x=319;
                    if (*scanx1==999) *scanx1=x;
                    else if (*scanx2==999 || *scanx1!=x) *scanx2=x;
                }
                vertc++;y0++;
            }
        }
    }
}

