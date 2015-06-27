#include "DLG-Edit.h"

/*
 * Move an area from the source list into the target list
 */

int FileTemplateSrc_listviewClicked( void ) {

    long SelectedArea=~0;
    long lastareaslot=0;
    SHORT SrcAreaNum=0;
    BOOL areaplaced;
    ULONG LVDisabled;
    struct Node *areanode;
    struct Node *testnode;
    int i;

    for (i=0; FileTempTargPtrs[i]; i++);
    if(i) lastareaslot=i-1;

    GT_GetGadgetAttrs(FileAreaTemplateGadgets[GD_FileTemplateSrc_listview], 
                      FileAreaTemplateWnd, NULL,
                      GTLV_Selected, &SelectedArea, TAG_DONE);

    if(SelectedArea!=~0) {

        /* Detach lists before modifying */

        GT_SetGadgetAttrs(FileAreaTemplateGadgets[GD_FileTemplateSrc_listview],
                          FileAreaTemplateWnd, NULL, 
                          GTLV_Labels, ~0, TAG_DONE);
        GT_SetGadgetAttrs(FileAreaTemplateGadgets[GD_FileTemplateTarg_listview],
                          FileAreaTemplateWnd, NULL, 
                          GTLV_Labels, ~0, TAG_DONE);

        /*
         * Get area node pointer, remove it from source listview and pointer list,
         * then add it into target listview and pointer list
         */
        areanode=FileTempSrcPtrs[SelectedArea];

        SrcAreaNum=atoi(areanode->ln_Name);

        Remove(areanode);

        for (i=SelectedArea; FileTempSrcPtrs[i+1]; i++) FileTempSrcPtrs[i]=FileTempSrcPtrs[i+1];
        FileTempSrcPtrs[i]=NULL;
        if(SelectedArea==i) SelectedArea--;

        /* Insert new node pointer */

        if (FileTempTargPtrs[0]) {
            areaplaced=FALSE;
            for (i=lastareaslot; i>=0 && areaplaced==FALSE; i--) {
                testnode=FileTempTargPtrs[i];
                FileTempTargPtrs[i+1]=FileTempTargPtrs[i];
                if (SrcAreaNum > atoi(testnode->ln_Name)) {
                    Insert(&FileTempTargList_list,areanode,testnode);
                    FileTempTargPtrs[++i]=areanode;
                    areaplaced=TRUE;
                }
                else if (i==0) {
                    AddHead(&FileTempTargList_list,areanode);
                    FileTempTargPtrs[i]=areanode;
                }
            }
        }
        else {
            AddHead(&FileTempTargList_list,areanode);
            FileTempTargPtrs[i=0]=areanode;
        }

        /* Re-attach target list to listview */

        GT_SetGadgetAttrs(FileAreaTemplateGadgets[GD_FileTemplateTarg_listview],
                          FileAreaTemplateWnd, NULL,
                          GTLV_Labels, &FileTempTargList_list,
                          GTLV_MakeVisible, i+1,
                          GTLV_Selected, i+1,
                          GA_Disabled, FALSE,
                          TAG_DONE);

        if (FileTempSrcPtrs[0]) LVDisabled=FALSE; else LVDisabled=TRUE;

        /* Re-attach source list to listview */

        GT_SetGadgetAttrs(FileAreaTemplateGadgets[GD_FileTemplateSrc_listview],
                          FileAreaTemplateWnd, NULL,
                          GTLV_Labels, &FileTempSrcList_list,
                          GTLV_MakeVisible, SelectedArea,
                          GTLV_Selected, SelectedArea,
                          GA_Disabled, LVDisabled,
                          TAG_DONE);

        /* Update status of "Apply" button */

        GT_SetGadgetAttrs(FileAreaTemplateGadgets[GD_FileTemplateApply_button],
                          FileAreaTemplateWnd, NULL,
                          GA_Disabled, FALSE,
                          TAG_DONE);

    }
    return(TRUE);
}


int FileTemplateTarg_listviewClicked( void ) {

    long SelectedArea=~0;
    long lastareaslot=0;
    SHORT SrcAreaNum=0;
    BOOL areaplaced;
    ULONG LVDisabled;
    struct Node *areanode;
    struct Node *testnode;
    int i;

    for (i=0; FileTempSrcPtrs[i]; i++);
    if(i) lastareaslot=i-1;

    GT_GetGadgetAttrs(FileAreaTemplateGadgets[GD_FileTemplateTarg_listview],
                      FileAreaTemplateWnd, NULL,
                      GTLV_Selected, &SelectedArea, TAG_DONE);

    if(SelectedArea!=~0) {

        /* Detach lists before modifying */

        GT_SetGadgetAttrs(FileAreaTemplateGadgets[GD_FileTemplateTarg_listview],
                          FileAreaTemplateWnd, NULL, 
                          GTLV_Labels, ~0, TAG_DONE);
        GT_SetGadgetAttrs(FileAreaTemplateGadgets[GD_FileTemplateSrc_listview],
                          FileAreaTemplateWnd, NULL,
                          GTLV_Labels, ~0, TAG_DONE);

        /*
         * Get area node pointer, remove it from source listview and pointer list,
         * then add it into target listview and pointer list
         */
        areanode=FileTempTargPtrs[SelectedArea];

        Remove(areanode);

        for (i=SelectedArea; FileTempTargPtrs[i+1]; i++) FileTempTargPtrs[i]=FileTempTargPtrs[i+1];

        FileTempTargPtrs[i]=NULL;
        if(SelectedArea==i) SelectedArea--;

        SrcAreaNum=atoi(areanode->ln_Name);

        /* Insert new node pointer */

        if (FileTempSrcPtrs[0]) {
            areaplaced=FALSE;
            for (i=lastareaslot; i>=0 && areaplaced==FALSE; i--) {
                testnode=FileTempSrcPtrs[i];
                FileTempSrcPtrs[i+1]=FileTempSrcPtrs[i];
                if (SrcAreaNum > atoi(testnode->ln_Name)) {
                    Insert(&FileTempSrcList_list,areanode,testnode);
                    FileTempSrcPtrs[++i]=areanode;
                    areaplaced=TRUE;
                }
                else if (i==0) {
                    AddHead(&FileTempSrcList_list,areanode);
                    FileTempSrcPtrs[i]=areanode;
                }
            }
        }
        else {
            AddHead(&FileTempSrcList_list,areanode);
            FileTempSrcPtrs[i=0]=areanode;
        }


        /*
         * We've just moved an area back from the target into the source list
         */

        GT_SetGadgetAttrs(FileAreaTemplateGadgets[GD_FileTemplateSrc_listview],
                          FileAreaTemplateWnd, NULL,
                          GTLV_Labels, &FileTempSrcList_list,
                          GTLV_MakeVisible, i+1,
                          GTLV_Selected, i+1,
                          GA_Disabled, FALSE,
                          TAG_DONE);

        if (FileTempTargPtrs[0]) LVDisabled=FALSE; else LVDisabled=TRUE;

        GT_SetGadgetAttrs(FileAreaTemplateGadgets[GD_FileTemplateTarg_listview],
                          FileAreaTemplateWnd, NULL,
                          GTLV_Labels, &FileTempTargList_list,
                          GTLV_MakeVisible, SelectedArea,
                          GTLV_Selected, SelectedArea,
                          GA_Disabled, LVDisabled,
                          TAG_DONE);

        GT_SetGadgetAttrs(FileAreaTemplateGadgets[GD_FileTemplateApply_button],
                          FileAreaTemplateWnd, NULL,
                          GA_Disabled, LVDisabled,
                          TAG_DONE);
    }
    return(TRUE);
}
/* "APPLY" */
int FileTemplateApply_buttonClicked( void ) {

    ULONG SelectedArea=~0;
    struct DLGAreaNode *templatenode;
    struct DLGAreaNode *targetnode;
    struct Node *tempnode;
    int i,j;
    SHORT templatenumber, targetnumber;
    ULONG T_Confirm,
          T_Access,
          T_Upload,
          T_Download,
          T_Transfer,
          T_Kill,
          T_SysOp,
          T_Validation,
          T_Freq,
          T_Path,
          T_Users,
          T_ValArea;
    BOOL ConfirmTemp;

    /*
     * Get selected entry in the original area listview
     */
    GT_GetGadgetAttrs(File_Area_ManagerGadgets[GD_FileArea_listview], File_Area_ManagerWnd, NULL, GTLV_Selected, &SelectedArea, TAG_DONE);
    templatenode=FileAreaPtrs[SelectedArea];
    templatenumber=templatenode->nn_Area.Number;

    GT_GetGadgetAttrs(FileAreaTemplateGadgets[GD_FileTempAccess_check], FileAreaTemplateWnd, NULL,      GTCB_Checked, &T_Access, TAG_DONE);
    GT_GetGadgetAttrs(FileAreaTemplateGadgets[GD_FileTempUpload_check], FileAreaTemplateWnd, NULL,      GTCB_Checked, &T_Upload, TAG_DONE);
    GT_GetGadgetAttrs(FileAreaTemplateGadgets[GD_FileTempDownload_check], FileAreaTemplateWnd, NULL,    GTCB_Checked, &T_Download, TAG_DONE);
    GT_GetGadgetAttrs(FileAreaTemplateGadgets[GD_FileTempTransfer_check], FileAreaTemplateWnd, NULL,    GTCB_Checked, &T_Transfer, TAG_DONE);
    GT_GetGadgetAttrs(FileAreaTemplateGadgets[GD_FileTempKill_check], FileAreaTemplateWnd, NULL,        GTCB_Checked, &T_Kill, TAG_DONE);
    GT_GetGadgetAttrs(FileAreaTemplateGadgets[GD_FileTempSysop_check], FileAreaTemplateWnd, NULL,       GTCB_Checked, &T_SysOp, TAG_DONE);
    GT_GetGadgetAttrs(FileAreaTemplateGadgets[GD_FileTempValidation_check], FileAreaTemplateWnd, NULL,  GTCB_Checked, &T_Validation, TAG_DONE);
    GT_GetGadgetAttrs(FileAreaTemplateGadgets[GD_FileTempFreq_check], FileAreaTemplateWnd, NULL,        GTCB_Checked, &T_Freq, TAG_DONE);
    GT_GetGadgetAttrs(FileAreaTemplateGadgets[GD_FileTempPath_check], FileAreaTemplateWnd, NULL,        GTCB_Checked, &T_Path, TAG_DONE);
    GT_GetGadgetAttrs(FileAreaTemplateGadgets[GD_FileTempUsers_check], FileAreaTemplateWnd, NULL,       GTCB_Checked, &T_Users, TAG_DONE);
    GT_GetGadgetAttrs(FileAreaTemplateGadgets[GD_FileTempValArea_check], FileAreaTemplateWnd, NULL,     GTCB_Checked, &T_ValArea, TAG_DONE);
    GT_GetGadgetAttrs(FileAreaTemplateGadgets[GD_FileTemplateConfirm_check], FileAreaTemplateWnd, NULL, GTCB_Checked, &T_Confirm, TAG_DONE);

    GT_SetGadgetAttrs(FileAreaTemplateGadgets[GD_FileTemplateTarg_listview],
                      FileAreaTemplateWnd, NULL,
                      GTLV_MakeVisible, 0,
                      GTLV_Selected, 0,
                      TAG_DONE);

    for (i=j=0; tempnode=FileTempTargPtrs[i]; i++) {

        /*
         * Find number of current target, then scan list of full area nodes
         * until a match is found. Then we can work on that.
         */
        targetnumber=atoi(tempnode->ln_Name);
        for(targetnode=FileAreaPtrs[j++]; (targetnode->nn_Area.Number)!=targetnumber; targetnode=FileAreaPtrs[j++])

        ConfirmTemp=TRUE;
        if (T_Confirm) {
            DisableWindows(FILETemp);
            switch(EasyRequest(NULL, &ConfirmTemplateReq, NULL, (tempnode->ln_Name))) {

                case 0: ConfirmTemp=FALSE;
                        break;
                case 2: T_Confirm=FALSE;
                        break;
                default:
                        break;
            }
            EnableWindows(FILETemp);
        }
        if (ConfirmTemp) {

            if(T_Access)     { targetnode->nn_Area.ulevel=templatenode->nn_Area.ulevel; targetnode->nn_Area.llevel=templatenode->nn_Area.llevel; }
            if(T_Upload)     { targetnode->nn_Area.uwrite=templatenode->nn_Area.uwrite; targetnode->nn_Area.lwrite=templatenode->nn_Area.lwrite; }
            if(T_Download)   { targetnode->nn_Area.uforward=templatenode->nn_Area.uforward; targetnode->nn_Area.lforward=templatenode->nn_Area.lforward; }
            if(T_Transfer)   { targetnode->nn_Area.ucopy=templatenode->nn_Area.ucopy; targetnode->nn_Area.lcopy=templatenode->nn_Area.lcopy; }
            if(T_Kill)       { targetnode->nn_Area.ukill=templatenode->nn_Area.ukill; targetnode->nn_Area.lkill=templatenode->nn_Area.lkill; }
            if(T_SysOp)      { targetnode->nn_Area.usysop=templatenode->nn_Area.usysop; targetnode->nn_Area.lsysop=templatenode->nn_Area.lsysop; }
            if(T_Validation) { targetnode->nn_Area.Flag |= VALIDATION_AREA; if (!(templatenode->nn_Area.Flag & VALIDATION_AREA)) targetnode->nn_Area.Flag ^= VALIDATION_AREA; }
            if(T_Freq)       { targetnode->nn_Area.Flag |= FREQ_AREA; if (!(templatenode->nn_Area.Flag & FREQ_AREA)) targetnode->nn_Area.Flag ^= FREQ_AREA; }
            if(T_Path)         strncpy(targetnode->nn_Area.path,templatenode->nn_Area.path,8);
            if(T_Users)        FileActionQ(USERCOPY,templatenode->nn_Area.Number,targetnode->nn_Area.Number);
            if(T_ValArea)      targetnode->nn_Area.Capacity=templatenode->nn_Area.Capacity;

        }
        GT_SetGadgetAttrs(FileAreaTemplateGadgets[GD_FileTemplateTarg_listview],
                          FileAreaTemplateWnd, NULL,
                          GTLV_Labels, ~0, TAG_DONE);

        Remove(FileTempTargPtrs[i]);

        GT_SetGadgetAttrs(FileAreaTemplateGadgets[GD_FileTemplateTarg_listview],
                          FileAreaTemplateWnd, NULL,
                          GTLV_Labels, &FileTempTargList_list,
                          GTLV_MakeVisible, 0,
                          GTLV_Selected, 0,
                          TAG_DONE);
    }

    return(FALSE);
}

int FileTemplateConfirm_checkClicked (void) { return(TRUE); }
int FileTempUpload_checkClicked (void)      { return(TRUE); }
int FileTempSysop_checkClicked (void)       { return(TRUE); }
int FileTempUsers_checkClicked (void)       { return(TRUE); }
int FileTempDownload_checkClicked (void)    { return(TRUE); }
int FileTempTransfer_checkClicked (void)    { return(TRUE); }
int FileTempPath_checkClicked (void)        { return(TRUE); }
int FileTempKill_checkClicked (void)        { return(TRUE); }
int FileTempAccess_checkClicked (void)      { return(TRUE); }
int FileTempFreq_checkClicked (void)        { return(TRUE); }
int FileTempValidation_checkClicked (void)  { return(TRUE); }
int FileTempValArea_checkClicked (void)     { return(TRUE); }

int FileTemplateCancel_buttonClicked (void) {      /* "Cancel" */
    return(FALSE);
}
int FileAreaTemplateCloseWindow (void) {       /* routine for "IDCMP_CLOSEWINDOW". */
    return(FALSE);
}

