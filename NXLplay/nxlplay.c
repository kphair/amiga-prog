#include <exec/exec.h>
#include <proto/exec.h>
#include <dos/dos.h>
#include <dos/dosextens.h>
#include <intuition/intuition.h>
#include <intuition/intuitionbase.h>
#include <intuition/gadgetclass.h>
#include <intuition/screens.h>
#include <libraries/asl.h>
#include <proto/asl.h>
#include <graphics/gfxbase.h>
#include <graphics/text.h> 
#include <workbench/startup.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include <exec/exec.h>
#include <proto/intuition.h>
#include <proto/graphics.h>
#include <proto/wb.h>
#include <clib/alib_protos.h>
#include <devices/audio.h>

#define INTUITION_REV 39
#define GRAPHICS_REV 39

struct Screen *Screen;
struct Window *Window;
struct RastPort *demorast;
struct ViewPort *demoview;

struct NXLRecordHeader {

    UBYTE RecType;
    UBYTE u1; UWORD u2; UWORD u3;
    UWORD RecLen;
    UWORD u4;
    UWORD FrameNo;
    UWORD u5;
    UWORD TimerPos;
    UWORD FrameWidth;   UWORD FrameHeight;
    UWORD FrameX;       UWORD FrameY;
    UBYTE Type;         UBYTE SubType;
    UWORD Colours;
    UWORD u9;
    UWORD FrameLen;
    ULONG uA; ULONG uB; ULONG uC; ULONG uD;
    ULONG uE; ULONG uF; ULONG uG; ULONG uH;
};

struct NXLRecordHeader *NXLRec;

#define NXL_Clear   4

struct IOAudio *AudioIO;
struct MsgPort *AudioMP;
struct Message *AudioMSG;
ULONG  device=1;
LONG   clock=3579545;
UBYTE  channelselect[]={1,2,4,8};
ULONG  AudioActive;
BYTE   *abuffer,
       *axbuffer,
       *abuffer0,
       *abuffer1;

UBYTE *fbuffer0,
      *fbuffer1,
      *sbuffer,
      *dbuffer,
      xbyte0,
      xbyte1;
UWORD ix,
      iy,
      xword0;

ULONG PalX[256]={0}, pidx;
ULONG saved_palette[770]={0};
ULONG new_palette[770]={0};

struct FileRequester *nxlfilerequester;
struct TagItem nxlfrtags[3];
char   *nxlfile,
       *nxldrawer,
       *nxlfilename,
       *nxlfilereqtitle="Select an NXL file to play.";
struct WBStartup *wbstartup;
int    i,
       numargs;
char   **arglist;
BOOL   reqalloc=FALSE;

char pubscreenname[]="CustomScr";
char tframe[80]={0};
char windowtitle[80]={0};

void main(int argc, char **argv) {

    FILE *xlfile;

    if(argc==0) {
        wbstartup=(struct WBStartup *)argv;
        numargs=wbstartup->sm_NumArgs;
        arglist=(char **)wbstartup->sm_ArgList;
    } else {
        numargs=argc;
        arglist=argv;
    }

    /*
     * Use ASL file requester if no argument was provided
     */
    if (numargs==1) {
        nxlfrtags[0].ti_Tag =ASLFR_TitleText;
        nxlfrtags[0].ti_Data=(ULONG)nxlfilereqtitle;
        nxlfrtags[1].ti_Tag =ASLFR_RejectIcons;
        nxlfrtags[1].ti_Data=TRUE;
        nxlfrtags[2].ti_Tag =TAG_DONE;

        if (nxlfilerequester=AllocAslRequest(ASL_FileRequest,nxlfrtags)) {
            if (AslRequest(nxlfilerequester,NULL)) {

                /* Concatenate filename and path into full file name
                 */
                nxlfilename=calloc(strlen(nxlfile=nxlfilerequester->fr_File)+strlen(nxldrawer=nxlfilerequester->fr_Drawer)+1,1);
                reqalloc=TRUE;

                if (strlen(nxldrawer) && nxldrawer[strlen(nxldrawer)]!=':')
                    sprintf(nxlfilename,"%s/%s",nxldrawer,nxlfile);
                else
                    sprintf(nxlfilename,"%s",nxlfile);

            } else {
                FreeAslRequest(nxlfilerequester);
                goto miscfail;
            }

            FreeAslRequest(nxlfilerequester);

        } else goto miscfail;
    } else if (numargs>1)
        nxlfilename=arglist[1];

    /* Attempt to open the NXL file
     */
    if (!(xlfile=fopen(nxlfilename,"rb"))) goto miscfail;

    /* Get DMA clock rate
     */
    if (GfxBase->DisplayFlags & PAL) clock=3546895;   /* PAL clock  */
        else clock = 3579545;                         /* NTSC clock */

    /* Create an audio I/O block
     */
    if (!(AudioIO=(struct IOAudio *)AllocMem(sizeof(struct IOAudio),MEMF_PUBLIC | MEMF_CLEAR))) goto miscfail;

    /* Create the reply port
     */
    if (!(AudioMP=CreatePort(0,0))) goto miscfail;

    /* Set up the audio I/O block for channel allocation:
     */
    AudioIO->ioa_Request.io_Message.mn_ReplyPort   = AudioMP;
    AudioIO->ioa_Request.io_Message.mn_Node.ln_Pri = 0;
    AudioIO->ioa_Request.io_Command                = ADCMD_ALLOCATE;
    AudioIO->ioa_Request.io_Flags                  = ADIOF_NOWAIT;
    AudioIO->ioa_AllocKey                          = 0;
    AudioIO->ioa_Data                              = channelselect;
    AudioIO->ioa_Length                            = sizeof(channelselect);

    /* Open the audio device and allocate a channel
     */
    if (device=OpenDevice("audio.device",0L, (struct IORequest *) AudioIO ,0L)) goto miscfail;

    /* Allocate frame buffers
     * fbuffer0 is where the frame is read into, from the file,
     * fbuffer1 is where it is then decoded into
     */
    if (!(fbuffer0=calloc(65536,1))) goto miscfail;
    if (!(fbuffer1=calloc(65000,1))) goto miscfail;

    /* Allocate record header buffer
     */
    if (!(NXLRec=calloc(sizeof(struct NXLRecordHeader),1))) goto miscfail;

    /* Allocate audio buffers
     */
    if (!(abuffer0=(BYTE *)AllocMem(65000,MEMF_CHIP|MEMF_PUBLIC))) goto miscfail;
    if (!(abuffer1=(BYTE *)AllocMem(65000,MEMF_CHIP|MEMF_PUBLIC))) goto miscfail;
    abuffer=0;

    /* Check to see that it is actually an NXL file
     */
    fread(NXLRec,sizeof(struct NXLRecordHeader),1,xlfile);
    if (strncmp((char *)NXLRec+16,"NXL2",4)) goto miscfail;

    /* Get intuition library base pointer
     */
    if(!(IntuitionBase=(struct IntuitionBase *)OpenLibrary("intuition.library",INTUITION_REV))) goto miscfail;

    /* Get graphics library base pointer
     */
    if (!(GfxBase=(struct GfxBase *)OpenLibrary("graphics.library",GRAPHICS_REV))) goto miscfail;

    /* Get a lock on the workbench screen
     */
    if (!(Screen=LockPubScreen(pubscreenname))) goto miscfail;

    /* Open the window on the screen
     */
    if (!(Window=OpenWindowTags(NULL,
            WA_Left,            30,
            WA_Top,             20,
            WA_InnerWidth,      320,
            WA_InnerHeight,     200,
            WA_CloseGadget,     TRUE,
            WA_DragBar,         TRUE,
            WA_DepthGadget,     TRUE,
            WA_SizeGadget,      FALSE,
            WA_Title,           "",
            WA_PubScreen,       Screen,
            WA_GimmeZeroZero,   FALSE,
            WA_Activate,        TRUE,
            WA_IDCMP,           IDCMP_CLOSEWINDOW,
            TAG_END)))
        goto miscfail;

    /* Get RastPort and ViewPort
     */
    demorast=Window->RPort;
    demoview=&(Window->WScreen->ViewPort);

    /* Save the palette and make sure the structure is complete
     */
    GetRGB32(demoview->ColorMap,0,256l,&saved_palette[1]);
    saved_palette[0]=256l<<16+0;
    saved_palette[769]=0;

    /* Create CLUT for 6-bit VGA to 32-bit RGB
     */
    for(pidx=0;pidx<256;pidx++) PalX[pidx]=pidx<<26L;

    /* Now we do the tricky stuff!
     */
    while (1) {

        /* Read in an NXL record header correct for total size
         */
        if (fread(NXLRec,sizeof(struct NXLRecordHeader),1,xlfile)) {

            NXLRec->RecLen-=sizeof(struct NXLRecordHeader);

            switch (NXLRec->RecType) {

                case 4:     /* Type 4 - Audio block */

                    /* Swap audio buffers so that we are about to use the
                     * one that's not in use right now.
                     */
                    if (abuffer==abuffer0) abuffer=abuffer1;
                        else abuffer=abuffer0;

                    /* Read the audio block into the buffer
                     */
                    fread(abuffer,NXLRec->RecLen,1,xlfile);

                    /* Set up I/O block for transfer
                     */
                    AudioIO->ioa_Request.io_Message.mn_ReplyPort =AudioMP;
                    AudioIO->ioa_Request.io_Command              =CMD_WRITE;
                    AudioIO->ioa_Request.io_Flags                =ADIOF_PERVOL;
                    AudioIO->ioa_Data                            =(BYTE *)abuffer;
                    AudioIO->ioa_Length                          =NXLRec->RecLen;
                    AudioIO->ioa_Period                          =clock/16000;
                    AudioIO->ioa_Volume                          =64;
                    AudioIO->ioa_Cycles                          =1;

                    /* Translate audio buffer from unsigned values to signed
                     */
                    axbuffer=abuffer;
                    for(i=NXLRec->RecLen-1;i>=0;i--) *(axbuffer++)-=128;

                    /* Wait for current audio block to end before starting
                     * to play the next one.
                     */
                    if (AudioActive) {
                        WaitPort(AudioMP);
                        AudioMSG=GetMsg(AudioMP);
                        AudioActive=0;
                    }
                    BeginIO((struct IORequest *) AudioIO );
                    AudioActive=1;

                    break;

                case 3:     /* Type 3 - Graphic data block */

                    /* Sub-type 4 - Clear frame buffer before proceeding
                     */
//                    if (NXLRec->SubType==NXL_Clear) memset(fbuffer1,0,64000);

                    /* Read in the frame buffer and set window title stats
                     */
                    fread(fbuffer0,NXLRec->RecLen,1,xlfile);
                    sprintf(windowtitle,"Frame: %d %d-%d %d,%d %d×%d",
                            NXLRec->FrameNo,
                            NXLRec->Type, NXLRec->SubType,
                            NXLRec->FrameX, NXLRec->FrameY,
                            NXLRec->FrameWidth, NXLRec->FrameHeight);
                    SetWindowTitles(Window,windowtitle,(UBYTE *)~0);

                    /* Better code for width 288
                     */
                    if (NXLRec->FrameWidth==1288) {
                        dbuffer=fbuffer1+NXLRec->FrameX+NXLRec->FrameY*320;
                        sbuffer=fbuffer0+768;
                        for (iy=NXLRec->FrameHeight; iy>0; iy--) {
                            for (ix=0; ix<72; ix++) {

                                *(dbuffer++)=*(sbuffer+ix);
                                *(dbuffer++)=*(sbuffer+ix+72);
                                *(dbuffer++)=*(sbuffer+ix+144);
                                *(dbuffer++)=*(sbuffer+ix+216);
                            }
                            dbuffer+=32;
                            /* Double height pixels?
                             */
                            if (NXLRec->Type==2) {
                                memcpy(dbuffer,dbuffer-320,288);
                                dbuffer+=320;
                            }
                            sbuffer+=288;
                        }
                    }
                    /* Otherwise use more general conversion code
                     */
                    else if (NXLRec->Type==1000) {
                        dbuffer=fbuffer1+NXLRec->FrameX+NXLRec->FrameY*320;
                        sbuffer=fbuffer0+768;
                        xword0=NXLRec->FrameWidth>>2;
                        for (iy=NXLRec->FrameHeight; iy>0; iy--) {
                            for (ix=0; ix<xword0; ix++) {

                                *(dbuffer+(ix*4)  )=*(sbuffer+ix);
                                *(dbuffer+(ix*4)+1)=*(sbuffer+ix+xword0);
                                *(dbuffer+(ix*4)+2)=*(sbuffer+ix+(xword0*2));
                                *(dbuffer+(ix*4)+3)=*(sbuffer+ix+(xword0*3));
                            }
                            dbuffer+=320;
                            sbuffer+=NXLRec->FrameWidth;
                        }
                    }

                    /* Better code for width 144
                     */
                    else if (NXLRec->FrameWidth==1144) {
                        dbuffer=fbuffer1+NXLRec->FrameX+NXLRec->FrameY*320;
                        sbuffer=fbuffer0+768;
                        for (iy=NXLRec->FrameHeight; iy>0; iy--) {
                            for (ix=0; ix<72; ix++) {

                                xbyte0=*(sbuffer+ix);
                                xbyte1=*(sbuffer+ix+72);
                                *(dbuffer++)=xbyte0;
                                *(dbuffer++)=xbyte0;
                                *(dbuffer++)=xbyte1;
                                *(dbuffer++)=xbyte1;
                            }
                            dbuffer+=32;
                            /* Double height pixels?
                             */
                            if (NXLRec->Type==2) {
                                memcpy(dbuffer,dbuffer-320,288);
                                dbuffer+=320;
                            }
                            sbuffer+=144;
                        }
                    }
                    /* Otherwise use more general conversion code
                     */
                    else if (NXLRec->Type==1001) {
                        dbuffer=fbuffer1+NXLRec->FrameX+NXLRec->FrameY*320;
                        sbuffer=fbuffer0+768;
                        xword0=NXLRec->FrameWidth>>1;
                        for (iy=NXLRec->FrameHeight; iy>0; iy--) {
                            for (ix=0; ix<xword0; ix++) {

                                xbyte0=*(sbuffer+ix);
                                xbyte1=*(sbuffer+ix+xword0);
                                *(dbuffer+(ix*4)  )=xbyte0;
                                *(dbuffer+(ix*4)+1)=xbyte0;
                                *(dbuffer+(ix*4)+2)=xbyte1;
                                *(dbuffer+(ix*4)+3)=xbyte1;
                            }
                            dbuffer+=320;
                            sbuffer+=NXLRec->FrameWidth;
                        }
                    }


                    for(pidx=0;pidx<768; pidx++) new_palette[pidx+1]=PalX[fbuffer0[pidx]];
                    new_palette[0]=0x1000000L;
                    new_palette[769]=0;
                    if (NXLRec->SubType>3) {
                        WriteChunkyPixels(demorast,
                          Window->BorderLeft,Window->BorderTop,
                          Window->BorderLeft+319,Window->BorderTop+199,
                          fbuffer1,320);
                        LoadRGB32(demoview,new_palette);
                    }
                    break;

                default:     /* If it's an unknown type, read past it. */
                    fread(fbuffer0,NXLRec->RecLen,1,xlfile);
                    break;
            }
        } else goto exit_player;    /* No more records; End of file */

        /* Do we have a message from our window close gadget?
         */
        if (GetMsg(Window->UserPort)) {

            exit_player:
                /* Wait for audio I/O to finish
                 */
                if (AudioActive) {
                    WaitPort(AudioMP);
                    AudioMSG=GetMsg(AudioMP);
                }
                /* Restore WB palette
                 */
                LoadRGB32(demoview,saved_palette);

            miscfail:
                if (Window)         CloseWindow(Window);
                if (Screen)         UnlockPubScreen(pubscreenname,Screen);
                if (GfxBase)        CloseLibrary((struct Library *)GfxBase);
                if (IntuitionBase)  CloseLibrary((struct Library *)IntuitionBase);
                if (xlfile)         fclose(xlfile);
                if (fbuffer0)       free(fbuffer0);
                if (fbuffer1)       free(fbuffer1);
                if (NXLRec)         free(NXLRec);
                if (abuffer0)       FreeMem(abuffer0,65000);
                if (abuffer1)       FreeMem(abuffer1,65000);
                if (device==0)      CloseDevice((struct IORequest *)AudioIO);
                if (AudioMP)        DeletePort(AudioMP);
                if (AudioIO)        FreeMem(AudioIO,sizeof(struct IOAudio) );
                if (reqalloc)       free(nxlfilename);
                exit(TRUE);
        }
    }
}

