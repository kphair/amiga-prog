/***************************************
*               DLG-Edit               *
*--------------------------------------*
* A graphical aid to DLG configuration *
****************************************
*       (C) 1994 Kevin J. Phair        *
***************************************/

#define MAIN
#include "DLG-Edit.h"

/// DECLARATIONS AND DEFINITIONS

/* Version information */

const char *myVersionTag=VERSTAG;
const char *myVersionString=VSTRING;

/* "About" requester structure */

struct EasyStruct AboutReq={
    sizeof(struct EasyStruct),
    0,
    "About DLG-Edit",
    "%s - Release v0.99b\n"                         \
    "© 1994 Kevin Phair - Freely distributable\n\n"    \

    "Internet:\n"                                      \
    "    Kev@Scorpio.ie\n"                             \
    "    Kevin_Phair@nemesis.ie\n\n"                   \

    "FidoNet:\n"                                       \
    "    Kevin Phair, 2:263/150.6\n\n"                 \

    "This is BETA software...\n"                       \
    "...Make sure you have a backup.",

    "Okay"
};

/* Pre-existing area warning */

struct EasyStruct AreaExistsReq={
    sizeof(struct EasyStruct),
    0,
    "Warning (Area Create)",
    "That area already exists!",
    "Okay"
};

/* Pre-existing area warning with area number */

struct EasyStruct ImportAreaExistsReq={
    sizeof(struct EasyStruct),
    0,
    "Warning (Area Import)",
    "Area %d already exists!\n" \
    "It will be skipped...",
    "Okay"
};

/* Import file open error Requester */

struct EasyStruct BadImportFileReq={
    sizeof(struct EasyStruct),
    0,
    "Warning (Area Import)",
    "Could not open file for import:\n" \
    "`%s'",
    "Okay"
};

/* Area template confirm Requester */

struct EasyStruct ConfirmTemplateReq={
    sizeof(struct EasyStruct),
    0,
    "Query (Area Template)",
    "Apply the template to this area?\n\n" \
    "%s",
    "Okay|Stop asking|No"
};

/* Cycle gadget list for user levels */
UBYTE *DefaultAccessLevels[]={
       (UBYTE *)"1",
       (UBYTE *)"10",
       (UBYTE *)"20",
       (UBYTE *)"50",
       (UBYTE *)"100",
       (UBYTE *)"150",
       (UBYTE *)"200",
       (UBYTE *)"220",
       (UBYTE *)"255",
       NULL };

///

/// Action queue management

/* Append an action command to the queue for message area management */

void MsgActionQ(int action, SHORT area1, SHORT area2) {

    FILE *qfile;

    if (qfile=fopen("T:MsgActionQueue","aa")) {

        fseek(qfile,0,SEEK_END);

        switch(action) {

            case MAKEAREA:
                fprintf(qfile,"makedir MSG:%d\n",area1);
                break;

            case KILLAREA:
                fprintf(qfile,"delete MSG:%d ALL QUIET\n",area1);
                break;

            case SWAPAREA:
                fprintf(qfile,"rename MSG:%d MSG:SwapAreaTempName\n",area1);
                fprintf(qfile,"rename MSG:%d MSG:%d\n",area2,area1);
                fprintf(qfile,"rename MSG:SwapAreaTempName MSG:%d\n",area2);
                break;

            case RENUMAREA:
                fprintf(qfile,"rename MSG:%d MSG:%d\n",area1,area2);
                break;

            case USERCOPY:
                fprintf(qfile,"copy MSG:%d/User.dat MSG:%d/User.dat\n",area1,area2);
                break;
        }
        fclose(qfile);
    }
}

/* Append an action command to the queue for file area management */

void FileActionQ(int action, SHORT area1, SHORT area2) {

    FILE *qfile;

    if (qfile=fopen("T:FileActionQueue","aa")) {

        fseek(qfile,0,SEEK_END);

        switch(action) {

            case MAKEAREA:
                fprintf(qfile,"makedir FILE:%d\n",area1);
                break;

            case KILLAREA:
                fprintf(qfile,"delete FILE:%d ALL QUIET\n",area1);
                break;

            case SWAPAREA:
                fprintf(qfile,"rename FILE:%d FILE:SwapAreaTempName\n",area1);
                fprintf(qfile,"rename FILE:%d FILE:%d\n",area2,area1);
                fprintf(qfile,"rename FILE:SwapAreaTempName FILE:%d\n",area2);
                break;

            case RENUMAREA:
                fprintf(qfile,"rename FILE:%d FILE:%d\n",area1,area2);
                break;

            case USERCOPY:
                fprintf(qfile,"copy FILE:%d/User.dat FILE:%d/User.dat\n",area1,area2);
                break;
        }
        fclose(qfile);
    }
}

void InitActionFiles(void) {
    FILE *qfile;

    /* Create new action queue file for message areas... */
    if (qfile=fopen("T:MsgActionQueue","wt")) {
        fprintf(qfile,"failat 999\n");
        fclose(qfile);
    }
    /* ...and file areas. */
    if (qfile=fopen("T:FileActionQueue","wt")) {
        fprintf(qfile,"failat 999\n");
        fclose(qfile);
    }
}
///

/// Convert number to referenced cycle gadget option

/* Convert a number into the gadget option number for the cycle gadget */

ULONG AccessLevelIndex(int access_level)
{
    ULONG levelindex=0;

    while (AccessLevelLabels[levelindex]) {
        if(access_level<=atoi((const char *)AccessLevelLabels[levelindex])) break;
        levelindex++;
    }
    return(levelindex);
}
///

/// Window blocking/unblocking

/* Disable gadget processing in windows specified by active mask        *
 * Goes through the supplied request mask, comparing the bits with the  *
 * defined masks to see which windows to disable.                       *
 * EnableWindows works in a similar fashion.                            */

void DisableWindows(SHORT active_mask) {

    if ((active_wnd & MAINWnd) && (active_mask & MAINWnd)) {
        InitRequester(&MainStatusDisplayBlock_Req);
        Request(&MainStatusDisplayBlock_Req, Main_Status_DisplayWnd);
        SetWindowPointer(Main_Status_DisplayWnd, WA_BusyPointer, TRUE, TAG_DONE);
    }
    
    /* File area manager and subwindows */

    if ((active_wnd & FILEWnd) && (active_mask & FILEWnd)) {
        InitRequester(&FileAreaManagerBlock_Req);
        Request(&FileAreaManagerBlock_Req, File_Area_ManagerWnd);
        SetWindowPointer(File_Area_ManagerWnd, WA_BusyPointer, TRUE, TAG_DONE);
    }
    if ((active_wnd & FILETemp) && (active_mask & FILETemp)) {
        InitRequester(&FileAreaTemplateBlock_Req);
        Request(&FileAreaTemplateBlock_Req, FileAreaTemplateWnd);
        SetWindowPointer(FileAreaTemplateWnd, WA_BusyPointer, TRUE, TAG_DONE);
    }
    if ((active_wnd & FILEAccess) && (active_mask & FILEAccess)) {
        InitRequester(&FileUserAccessBlock_Req);
        Request(&FileUserAccessBlock_Req, FileUserAccessWnd);
        SetWindowPointer(FileUserAccessWnd, WA_BusyPointer, TRUE, TAG_DONE);
    }

    /* Check for the message area manager special case */

    if ((active_wnd & MSGTemp) && (active_mask & MSGTemp)) {
        InitRequester(&MsgAreaTemplateBlock_Req);
        Request(&MsgAreaTemplateBlock_Req, MsgAreaTemplateWnd);
        SetWindowPointer(MsgAreaTemplateWnd, WA_BusyPointer, TRUE, TAG_DONE);
    }
    if ((active_wnd & MSGWnd) && (active_mask & MSGWnd)) {
        InitRequester(&MsgAreaManagerBlock_Req);
        Request(&MsgAreaManagerBlock_Req, Message_Area_ManagerWnd);
        SetWindowPointer(Message_Area_ManagerWnd, WA_BusyPointer, TRUE, TAG_DONE);
    }
    
    if ((active_wnd & USERWnd) && (active_mask & USERWnd)) {
        InitRequester(&UserEditorBlock_Req);
        Request(&UserEditorBlock_Req, User_EditorWnd);
        SetWindowPointer(User_EditorWnd, WA_BusyPointer, TRUE, TAG_DONE);
    }
}

/* Enable gadget processing in windows specified by active mask */

void EnableWindows(SHORT active_mask) {

    if ((active_wnd & MAINWnd) && (active_mask & MAINWnd)) {
        EndRequest(&MainStatusDisplayBlock_Req, Main_Status_DisplayWnd);
        SetWindowPointer(Main_Status_DisplayWnd, TAG_DONE);
    }
    
    if ((active_wnd & FILETemp) && (active_mask & FILETemp)) {
        EndRequest(&FileAreaTemplateBlock_Req, FileAreaTemplateWnd);
        SetWindowPointer(FileAreaTemplateWnd, TAG_DONE);
    } else if ((active_wnd & FILEWnd) && (active_mask & FILEWnd)) {
        EndRequest(&FileAreaManagerBlock_Req, File_Area_ManagerWnd);
        SetWindowPointer(File_Area_ManagerWnd, TAG_DONE);
    }
    
    if ((active_wnd & MSGTemp) && (active_mask & MSGTemp)) {
        EndRequest(&MsgAreaTemplateBlock_Req, MsgAreaTemplateWnd);
        SetWindowPointer(MsgAreaTemplateWnd, TAG_DONE);
    } else if ((active_wnd & MSGWnd) && (active_mask & MSGWnd)) {
        EndRequest(&MsgAreaManagerBlock_Req, Message_Area_ManagerWnd);
        SetWindowPointer(Message_Area_ManagerWnd, TAG_DONE);
    }
    
    if ((active_wnd & USERWnd) && (active_mask & USERWnd)) {
        EndRequest(&UserEditorBlock_Req, User_EditorWnd);
        SetWindowPointer(User_EditorWnd, TAG_DONE);
    }
}
///

/// Plug-in replacement for ComputeFont()
/*
void ComputeFont(UWORD width, UWORD height) {

extern UBYTE *AppFontName;
extern UWORD AppFontXSize;
extern UWORD AppFontYSize;

    Font = &Attr;
    Font->ta_Name = AppFontName;
    Font->ta_YSize = FontY = AppFontYSize;
    FontX = AppFontXSize;

    OffX = Scr->WBorLeft;
    OffY = Scr->RastPort.TxHeight + Scr->WBorTop + 1;

    if ( width && height ) {
            if (( ComputeX( width ) + OffX + Scr->WBorRight ) > Scr->Width )
                    goto UseTopaz;
            if (( ComputeY( height ) + OffY + Scr->WBorBottom ) > Scr->Height )
                    goto UseTopaz;
    }
    return;

UseTopaz:
    Font->ta_Name = (STRPTR)"topaz.font";
    FontX = FontY = Font->ta_YSize = 8;
}
*/

///

/**                       ***
/**  T~~~~~:----(~)----.  ***
***  | T T | O| |~| |  |  ***
***  |_|_|_|__|_|_|_|__|  ***
***                       **/

int main(int argc, char **argv)
{
    struct Library *DiskfontBase=NULL;
    struct FontRequester *fr;
    long Sigs, RetVal;
    struct DiskObject *myDiskObject=NULL;
    struct WBStartup *wbstartup;
    UBYTE **tooltypearray;
    static char programname[512];
    UBYTE *publicscreenname;
    UBYTE *appfontname;
    UBYTE *appfontxsize=NULL;
    UBYTE *appfontysize=NULL;
    UBYTE *userlevelsTT=NULL;
    int i;

    static struct TextAttr myAppFont;
    struct TextFont *myTextFont=NULL;

    UBYTE *DefAppFontName="topaz.font";
    UWORD DefAppFontXSize=8;
    UWORD DefAppFontYSize=8;

    /// Program initialisation, get tooltypes etc.

    AllocatePointerMem();

    /* Get tooltypes and act on them */

    /* Started from Workbench */

    if(argc==0) {
        wbstartup=(struct WBStartup *)argv;
        CurrentDir(wbstartup->sm_ArgList->wa_Lock);
        strcpy(programname,wbstartup->sm_ArgList->wa_Name);
    } else

    /* Started from CLI */

    GetProgramName(programname,512);

    AppFontName=DefAppFontName;
    AppFontXSize=DefAppFontXSize;
    AppFontYSize=DefAppFontYSize;

    if (strlen(programname)) {
        if (myDiskObject=GetDiskObject(programname)) {
            tooltypearray=myDiskObject->do_ToolTypes;
            if (publicscreenname=FindToolType(tooltypearray, "PUBSCREEN")) {
                PubScreenName=malloc(strlen(publicscreenname)+1);
                strcpy(PubScreenName,publicscreenname);
            }
            if (appfontname=FindToolType(tooltypearray, "FONTNAME")) {
                AppFontName=malloc(strlen(appfontname)+1);
                strcpy(AppFontName,appfontname);
                if (appfontysize=FindToolType(tooltypearray, "FONTYSIZE")) {
                    AppFontYSize=atoi(appfontysize);
                }
                if (DiskfontBase=OpenLibrary("diskfont.library", 37L)) {
                    myAppFont.ta_Name=AppFontName;
                    myAppFont.ta_YSize=AppFontYSize;
                    if (myTextFont=OpenDiskFont(&myAppFont)) {
                        AppFontXSize=myTextFont->tf_XSize;
                        if(myTextFont->tf_Flags & FPF_PROPORTIONAL) AppFontXSize=(1+AppFontXSize*2)/3;
                    } else {
                        /* Open an ASL font requester */
                        if (fr = (struct FontRequester *)AllocAslRequestTags(ASL_FontRequest, ASL_FontName, AppFontName, ASL_FontHeight, AppFontYSize, TAG_DONE)) {
                            /* Pop up the requester */
                            if (AslRequest(fr, 0L)) {
                                myAppFont.ta_Name  = fr->fo_Attr.ta_Name;    /* extract the font and */
                                myAppFont.ta_YSize = fr->fo_Attr.ta_YSize;   /* display attributes   */
                                myAppFont.ta_Style = fr->fo_Attr.ta_Style;   /* from the FontRequest */
                                myAppFont.ta_Flags = fr->fo_Attr.ta_Flags;   /* structure.           */

                                if (myTextFont=OpenDiskFont(&myAppFont)) {
                                    free(AppFontName);
                                    AppFontName=malloc(strlen(myAppFont.ta_Name)+1);
                                    strcpy(AppFontName,myAppFont.ta_Name);
                                    AppFontXSize=myTextFont->tf_XSize;
                                    AppFontYSize=myAppFont.ta_YSize;
                                } else {
                                    AppFontName=DefAppFontName;
                                    AppFontXSize=DefAppFontXSize; AppFontYSize=DefAppFontYSize;
                                }
                            } else {
                                /* Requester was cancelled */
                                AppFontName=DefAppFontName;
                                AppFontXSize=DefAppFontXSize; AppFontYSize=DefAppFontYSize;
                                FreeAslRequest(fr);
                            }
                        } else {
                            /* ASL request failed */
                            AppFontName=DefAppFontName;
                            AppFontXSize=DefAppFontXSize; AppFontYSize=DefAppFontYSize;
                        }
                    }
                    if (appfontxsize=FindToolType(tooltypearray, "FONTXSIZE")) {
                        AppFontXSize=atoi(appfontxsize);
                    }
                } else {
                    AppFontName=DefAppFontName;
                    AppFontXSize=DefAppFontXSize; AppFontYSize=DefAppFontYSize;
                }
            }

            i=0;
            if (userlevelsTT=FindToolType(tooltypearray, "USERLEVELS")) {
                while(*userlevelsTT) {
                    AccessLevelLabels[i++]=userlevelsTT;
                    AccessLevelLabels[i]=NULL;
                    while(*userlevelsTT !=',' && *userlevelsTT !=0) userlevelsTT++;
                    if(*userlevelsTT==',') *(userlevelsTT++)=0;
                }
            } else AccessLevelLabels=DefaultAccessLevels;
        }
    }

    ///

/// Open screen, allocate memory etc.

    /* Open on the public screen */
    if (!SetupScreen()) {

        /* Now open the main window */
        if (!OpenMain_Status_DisplayWindow()) {

            /* Set flags to signify that the window is open */
            active_wnd=MAINWnd;
            main_mask=(1 << Main_Status_DisplayWnd->UserPort->mp_SigBit);
            all_mask=main_mask;

            /* Allocate all the memory pools and blocks and fill them in with   */
            /* the appropriate configuration information                        */
            GetDLGTerminalTypes();
            GetDLGArchiverTypes();
            GetDLGEditorTypes();
            GetDLGCharacterSetTypes();
            GetDLGMenuSetTypes();
            GetDLGTransferProtocols();
            GetBBSUsers();
            GetFileAreaList();
            GetMsgAreaList();

            /* Initialise the action queue scripts */
            InitActionFiles();

///

            /*--------------------------------------------------------------*
             * The main loop.                                               *
             *--------------------------------------------------------------*
             * What we do is wait for a message at one of our signal ports. *
             * When a message arrives we then go through this, checking to  *
             * see which window the message is for, and then process it.    *
             * When they've been all been dealt with we go back and wait.   *
             *==============================================================*
             * all_mask is a dynamic value. When a window opens, we put its *
             * message bit into this, removing it when the window closes.   *
             *--------------------------------------------------------------*/

            do {
                Sigs=Wait(all_mask);

                /* Is the main window (control panel) open?
                 */
                if ((active_wnd & MAINWnd) && (Sigs & main_mask))
                    RetVal=HandleMain_Status_DisplayIDCMP();

                /* Is the file area manager window open? Any subwindows?
                 * Follow through if IDCMP handler returns an exit result
                 */
                if ((active_wnd & FILEWnd) && (Sigs & fileman_mask)) {
                    if (!HandleFile_Area_ManagerIDCMP()) {

                        /* Close the window */
                        CloseFile_Area_ManagerWindow();

                        /* Remove window from signal checking */
                        all_mask ^= fileman_mask; active_wnd ^= FILEWnd;

                        /* Re-enable selection button on control panel */
                        GT_SetGadgetAttrs(Main_Status_DisplayGadgets[GD_FileAreaSelect_button],Main_Status_DisplayWnd,NULL, GA_Disabled,FALSE, TAG_DONE);
                    }
                }

                /* Is the file template window open?
                 * Follow through if IDCMP handler returns an exit result
                 */
                if ((active_wnd & FILETemp) && (Sigs & filetemp_mask)) {
                    if (!HandleFileAreaTemplateIDCMP()) {

                        /* Free memory for area nodes and node pointers */
                        DeletePool(FileTempSrcList_pool);
                        free(FileTempSrcPtrs);
                        DeletePool(FileTempTargList_pool);
                        free(FileTempTargPtrs);

                        /* Close the window and enable parent window */
                        CloseFileAreaTemplateWindow();

                        /* Remove window from signal checking */
                        all_mask ^= filetemp_mask; active_wnd ^= FILETemp;
                        EnableWindows(FILEWnd);
                    }
                }

                /* Is the file access manager window open?
                 * Follow through if IDCMP handler returns an exit result
                 */
                if ((active_wnd & FILEAccess) && (Sigs & fileaccess_mask)) {
                    if (!HandleFileUserAccessIDCMP()) {

                        /* Free memory for area nodes and node pointers */
                        DeletePool(FileAccessAreaSrcList_pool);
                        free(FileAccessAreaSrcPtrs);
                        DeletePool(FileAccessAreaTargList_pool);
                        free(FileAccessAreaTargPtrs);

                        /* Free memory for user nodes and node pointers */
                        DeletePool(FileAccessUserSrcList_pool);
                        free(FileAccessUserSrcPtrs);
                        DeletePool(FileAccessUserTargList_pool);
                        free(FileAccessUserTargPtrs);

                        /* Close the window and enable parent window */
                        CloseFileUserAccessWindow();

                        /* Remove window from signal checking */
                        all_mask ^= fileaccess_mask; active_wnd ^= FILEAccess;
                        EnableWindows(FILEWnd);
                    }
                }

                /* Is the message area manager open? Any subwindows?
                 * Follow through if IDCMP handler returns an exit result
                 */
                if ((active_wnd & MSGWnd)   && (Sigs & msgman_mask)) {
                    if (!HandleMessage_Area_ManagerIDCMP()) {
                        /* Close the window */
                        CloseMessage_Area_ManagerWindow();
                        /* Remove window from signal checking */
                        all_mask ^= msgman_mask; active_wnd ^= MSGWnd;
                        /* Re-enable selection button on control panel */
                        GT_SetGadgetAttrs(Main_Status_DisplayGadgets[GD_MessageAreaSelect_button],Main_Status_DisplayWnd,NULL, GA_Disabled,FALSE, TAG_DONE);
                    }
                }

                /* Is the message template window open?
                 * Follow through if IDCMP handler returns an exit result
                 */
                if ((active_wnd & MSGTemp)  && (Sigs & msgtemp_mask)) {
                    if (!HandleMsgAreaTemplateIDCMP()) {
                        /* Free memory for area nodes and node pointers */
                        DeletePool(MsgTempSrcList_pool);
                        free(MsgTempSrcPtrs);
                        DeletePool(MsgTempTargList_pool);
                        free(MsgTempTargPtrs);
                        /* Close the window and enable parent window */
                        CloseMsgAreaTemplateWindow();
                        /* Remove window from signal checking */
                        all_mask ^= msgtemp_mask; active_wnd ^= MSGTemp;
                        EnableWindows(MSGWnd);
                    }
                }

                /* Is the user editor window open?
                 * Follow through if IDCMP handler returns an exit result
                 */
                if ((active_wnd & USERWnd)  && (Sigs & usered_mask)) {
                    if (!HandleUser_EditorIDCMP()) {

                        /* Close the window */
                        CloseUser_EditorWindow();

                        /* Remove window from signal checking */
                        all_mask ^= usered_mask; active_wnd ^= USERWnd;

                        /* Re-enable selection button on control panel */
                        GT_SetGadgetAttrs(Main_Status_DisplayGadgets[GD_UserEditorSelect_button],Main_Status_DisplayWnd,NULL, GA_Disabled,FALSE, TAG_DONE);
                    }
                }
            } while(RetVal);

/// Clean up and exit

            /* Delete action queue files */
            unlink("t:MsgActionQueue");     
            unlink("t:FileActionQueue");

            /* Close any open windows */
            if (active_wnd & FILETemp) {
                DeletePool(FileTempSrcList_pool);
                free(FileTempSrcPtrs);
                DeletePool(FileTempTargList_pool);
                free(FileTempTargPtrs);
                CloseFileAreaTemplateWindow();
            }
            if (active_wnd & FILEAccess) {

                /* Free memory for area nodes and node pointers */
                DeletePool(FileAccessAreaSrcList_pool);
                free(FileAccessAreaSrcPtrs);
                DeletePool(FileAccessAreaTargList_pool);
                free(FileAccessAreaTargPtrs);

                /* Free memory for user nodes and node pointers */
                DeletePool(FileAccessUserSrcList_pool);
                free(FileAccessUserSrcPtrs);
                DeletePool(FileAccessUserTargList_pool);
                free(FileAccessUserTargPtrs);

                /* Close the window and enable parent window */
                CloseFileUserAccessWindow();
            }
            if (active_wnd & FILEWnd)   CloseFile_Area_ManagerWindow();

            if (active_wnd & MSGTemp) {
                DeletePool(MsgTempSrcList_pool);
                free(MsgTempSrcPtrs);
                DeletePool(MsgTempTargList_pool);
                free(MsgTempTargPtrs);
                CloseMsgAreaTemplateWindow();
            }
            if (active_wnd & MSGWnd)    CloseMessage_Area_ManagerWindow();

            if (active_wnd & USERWnd)   CloseUser_EditorWindow();

            if (active_wnd & MAINWnd)   CloseMain_Status_DisplayWindow();

            /* Deallocate memory pools and blocks */
            DeletePool(UserTerminal_pool);  
            DeletePool(UserArchiver_pool);
            DeletePool(UserEditor_pool);
            DeletePool(UserCharSet_pool);
            DeletePool(UserMenuSet_pool);
            DeletePool(UserTransferProtocol_pool);

            DeletePool(UserUserList_pool);

            DeletePool(FileAreaList_pool);

            DeletePool(MsgAreaList_pool);

        }
    }
    /* And close the screen */
    CloseDownScreen();

    if (AppFontName) free(AppFontName);
    if (PubScreenName) free(PubScreenName);
    if (myDiskObject) FreeDiskObject(myDiskObject);

    if (myTextFont) {
        CloseFont(myTextFont);
        CloseLibrary(DiskfontBase);
    }

    FreePointerMem();

    return(0);
}

///
