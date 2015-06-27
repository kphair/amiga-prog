#include "DLG-Edit.h"

int MsgTemplateSrc_listviewClicked( void ) {

    long SelectedArea=~0;
    long lastareaslot=0;
    SHORT SrcAreaNum=0;
    BOOL areaplaced;
    ULONG LVDisabled;
    struct Node *areanode;
    struct Node *testnode;
    int i;

    for (i=0; MsgTempTargPtrs[i]; i++);
    if (i) lastareaslot=i-1;

    GT_GetGadgetAttrs(MsgAreaTemplateGadgets[GD_MsgTemplateSrc_listview],
                      MsgAreaTemplateWnd, NULL,
                      GTLV_Selected, &SelectedArea, TAG_DONE);

    if(SelectedArea!=~0) {

        /*
         * Detach lists before modifying
         */
        GT_SetGadgetAttrs(MsgAreaTemplateGadgets[GD_MsgTemplateSrc_listview],
                          MsgAreaTemplateWnd, NULL,
                          GTLV_Labels, ~0, TAG_DONE);
        GT_SetGadgetAttrs(MsgAreaTemplateGadgets[GD_MsgTemplateTarg_listview],
                          MsgAreaTemplateWnd, NULL,
                          GTLV_Labels, ~0, TAG_DONE);

        /*
         * Get area node pointer, remove it from source listview and pointer list,
         * then add it into target listview and pointer list
         */
        areanode=MsgTempSrcPtrs[SelectedArea];

        SrcAreaNum=atoi(areanode->ln_Name);

        Remove(areanode);

        for (i=SelectedArea; MsgTempSrcPtrs[i+1]!=NULL; i++) MsgTempSrcPtrs[i]=MsgTempSrcPtrs[i+1];
        MsgTempSrcPtrs[i]=NULL;
        if(SelectedArea==i) SelectedArea--;

        /*
         * Insert new node pointer
         */
        if (MsgTempTargPtrs[0]) {
            areaplaced=FALSE;
            for (i=lastareaslot; i>=0 && areaplaced==FALSE; i--) {
                testnode=MsgTempTargPtrs[i];
                MsgTempTargPtrs[i+1]=MsgTempTargPtrs[i];
                if (SrcAreaNum > atoi(testnode->ln_Name)) {
                    Insert(&MsgTempTargList_list,areanode,testnode);
                    MsgTempTargPtrs[++i]=areanode;
                    areaplaced=TRUE;
                }
                else if (i==0) {
                    AddHead(&MsgTempTargList_list,areanode);
                    MsgTempTargPtrs[i]=areanode;
                }
            } 
        }
        else {
            AddHead(&MsgTempTargList_list,areanode);
            MsgTempTargPtrs[i=0]=areanode;
        }


        /*
         * We've just moved an area from the source list to the target list
         */

        GT_SetGadgetAttrs(MsgAreaTemplateGadgets[GD_MsgTemplateTarg_listview],
                          MsgAreaTemplateWnd, NULL,
                          GTLV_Labels, &MsgTempTargList_list,
                          GTLV_MakeVisible, i+1,
                          GTLV_Selected, i+1,
                          GA_Disabled, FALSE,
                          TAG_DONE);

        if (MsgTempSrcPtrs[0]) LVDisabled=FALSE; else LVDisabled=TRUE;

        GT_SetGadgetAttrs(MsgAreaTemplateGadgets[GD_MsgTemplateSrc_listview],
                          MsgAreaTemplateWnd, NULL,
                          GTLV_Labels, &MsgTempSrcList_list,
                          GTLV_MakeVisible, SelectedArea,
                          GTLV_Selected, SelectedArea,
                          GA_Disabled, LVDisabled,
                          TAG_DONE);

        GT_SetGadgetAttrs(MsgAreaTemplateGadgets[GD_MsgTemplateApply_button],
                          MsgAreaTemplateWnd, NULL,
                          GA_Disabled, FALSE,
                          TAG_DONE);
    }
    return(TRUE);
}
int MsgTemplateTarg_listviewClicked( void ) {

    long SelectedArea=~0;
    long lastareaslot=0;
    SHORT SrcAreaNum=0;
    BOOL areaplaced;
    ULONG LVDisabled;
    struct Node *areanode;
    struct Node *testnode;
    int i;

    for (i=0; MsgTempSrcPtrs[i]; i++);
    if (i) lastareaslot=i-1;

    GT_GetGadgetAttrs(MsgAreaTemplateGadgets[GD_MsgTemplateTarg_listview],
                      MsgAreaTemplateWnd, NULL,
                      GTLV_Selected, &SelectedArea, TAG_DONE);

    if(SelectedArea!=~0) {

        /*
         * Detach lists before modifying
         */
        GT_SetGadgetAttrs(MsgAreaTemplateGadgets[GD_MsgTemplateTarg_listview],
                          MsgAreaTemplateWnd, NULL,
                          GTLV_Labels, ~0, TAG_DONE);
        GT_SetGadgetAttrs(MsgAreaTemplateGadgets[GD_MsgTemplateSrc_listview],
                          MsgAreaTemplateWnd, NULL,
                          GTLV_Labels, ~0, TAG_DONE);

        /*
         * Get area node pointer, remove it from source listview and pointer list,
         * then add it into target listview and pointer list
         */
        areanode=MsgTempTargPtrs[SelectedArea];

        SrcAreaNum=atoi(areanode->ln_Name);

        Remove(areanode);

        for (i=SelectedArea; MsgTempTargPtrs[i+1]!=NULL; i++) MsgTempTargPtrs[i]=MsgTempTargPtrs[i+1];
        MsgTempTargPtrs[i]=NULL;
        if(SelectedArea==i) SelectedArea--;

        /*
         * Insert new node pointer
         */
        if (MsgTempSrcPtrs[0]) {
            areaplaced=FALSE;
            for (i=lastareaslot; i>=0 && areaplaced==FALSE; i--) {
                testnode=MsgTempSrcPtrs[i];
                MsgTempSrcPtrs[i+1]=MsgTempSrcPtrs[i];
                if (SrcAreaNum > atoi(testnode->ln_Name)) {
                    Insert(&MsgTempSrcList_list,areanode,testnode);
                    MsgTempSrcPtrs[++i]=areanode;
                    areaplaced=TRUE;
                }
                else if (i==0) {
                    AddHead(&MsgTempSrcList_list,areanode);
                    MsgTempSrcPtrs[i]=areanode;
                }
            } 
        }
        else {
            AddHead(&MsgTempSrcList_list,areanode);
            MsgTempSrcPtrs[i=0]=areanode;
        }


        /*
         * We've just moved an area back from the target into the source list
         */

        GT_SetGadgetAttrs(MsgAreaTemplateGadgets[GD_MsgTemplateSrc_listview],
                          MsgAreaTemplateWnd, NULL,
                          GTLV_Labels, &MsgTempSrcList_list,
                          GTLV_MakeVisible, i+1,
                          GTLV_Selected, i+1,
                          GA_Disabled, FALSE,
                          TAG_DONE);

        if (MsgTempTargPtrs[0]) LVDisabled=FALSE; else LVDisabled=TRUE;

        GT_SetGadgetAttrs(MsgAreaTemplateGadgets[GD_MsgTemplateTarg_listview],
                          MsgAreaTemplateWnd, NULL,
                          GTLV_Labels, &MsgTempTargList_list,
                          GTLV_MakeVisible, SelectedArea,
                          GTLV_Selected, SelectedArea,
                          GA_Disabled, LVDisabled,
                          TAG_DONE);

        GT_SetGadgetAttrs(MsgAreaTemplateGadgets[GD_MsgTemplateApply_button],
                          MsgAreaTemplateWnd, NULL,
                          GA_Disabled, LVDisabled,
                          TAG_DONE);
    }
    return(TRUE);
}
int MsgTemplateApply_buttonClicked( void ) {    /* "APPLY" */

    long SelectedArea=~0;
    struct DLGAreaNode *templatenode;
    struct DLGAreaNode *targetnode;
    struct Node *tempnode;
    struct Node *areanode;
    int i,j;
    SHORT templatenumber, targetnumber;
    ULONG T_Confirm,
          T_Access,
          T_Write,
          T_Forward,
          T_ReEdit,
          T_Copy,
          T_Kill,
          T_SysOp,
          T_Translator,
          T_Users,
          T_MailType,
          T_Charset,
          T_Handles,
          T_Capacity;
    BOOL ConfirmTemp;

    /*
     * Get selected entry in the original area listview
     */
    GT_GetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgArea_listview], Message_Area_ManagerWnd, NULL, GTLV_Selected, &SelectedArea, TAG_DONE);
    templatenode=MsgAreaPtrs[SelectedArea];
    templatenumber=templatenode->nn_Area.Number;

    GT_GetGadgetAttrs(MsgAreaTemplateGadgets[GD_MsgTempAccess_check], MsgAreaTemplateWnd, NULL,      GTCB_Checked, &T_Access, TAG_DONE);
    GT_GetGadgetAttrs(MsgAreaTemplateGadgets[GD_MsgTempWrite_check], MsgAreaTemplateWnd, NULL,       GTCB_Checked, &T_Write, TAG_DONE);
    GT_GetGadgetAttrs(MsgAreaTemplateGadgets[GD_MsgTempForward_check], MsgAreaTemplateWnd, NULL,     GTCB_Checked, &T_Forward, TAG_DONE);
    GT_GetGadgetAttrs(MsgAreaTemplateGadgets[GD_MsgTempReEdit_check], MsgAreaTemplateWnd, NULL,      GTCB_Checked, &T_ReEdit, TAG_DONE);
    GT_GetGadgetAttrs(MsgAreaTemplateGadgets[GD_MsgTempCopy_check], MsgAreaTemplateWnd, NULL,        GTCB_Checked, &T_Copy, TAG_DONE);
    GT_GetGadgetAttrs(MsgAreaTemplateGadgets[GD_MsgTempKill_check], MsgAreaTemplateWnd, NULL,        GTCB_Checked, &T_Kill, TAG_DONE);
    GT_GetGadgetAttrs(MsgAreaTemplateGadgets[GD_MsgTempSysop_check], MsgAreaTemplateWnd, NULL,       GTCB_Checked, &T_SysOp, TAG_DONE);
    GT_GetGadgetAttrs(MsgAreaTemplateGadgets[GD_MsgTempTranslator_check], MsgAreaTemplateWnd, NULL,  GTCB_Checked, &T_Translator, TAG_DONE);
    GT_GetGadgetAttrs(MsgAreaTemplateGadgets[GD_MsgTempUsers_check], MsgAreaTemplateWnd, NULL,       GTCB_Checked, &T_Users, TAG_DONE);
    GT_GetGadgetAttrs(MsgAreaTemplateGadgets[GD_MsgTempMailtype_check], MsgAreaTemplateWnd, NULL,    GTCB_Checked, &T_MailType, TAG_DONE);
    GT_GetGadgetAttrs(MsgAreaTemplateGadgets[GD_MsgTempCharset_check], MsgAreaTemplateWnd, NULL,     GTCB_Checked, &T_Charset, TAG_DONE);
    GT_GetGadgetAttrs(MsgAreaTemplateGadgets[GD_MsgTempHandles_check], MsgAreaTemplateWnd, NULL,     GTCB_Checked, &T_Handles, TAG_DONE);
    GT_GetGadgetAttrs(MsgAreaTemplateGadgets[GD_MsgTempCapacity_check], MsgAreaTemplateWnd, NULL,    GTCB_Checked, &T_Capacity, TAG_DONE);
    GT_GetGadgetAttrs(MsgAreaTemplateGadgets[GD_MsgTemplateConfirm_check], MsgAreaTemplateWnd, NULL, GTCB_Checked, &T_Confirm, TAG_DONE);

    for (i=j=0; tempnode=MsgTempTargPtrs[i]; i++) {

        /*
         * Find number of current target, then scan list of full area nodes
         * until a match is found. Then we can work on that.
         */
        targetnumber=atoi(tempnode->ln_Name);
        for(targetnode=MsgAreaPtrs[j++]; (targetnode->nn_Area.Number)!=targetnumber; targetnode=MsgAreaPtrs[j++])

        ConfirmTemp=TRUE;
        if (T_Confirm) {
            DisableWindows(ALLWindows);
            switch(EasyRequest(NULL, &ConfirmTemplateReq, NULL, (tempnode->ln_Name))) {

                case 0: ConfirmTemp=FALSE;
                        break;
                case 2: T_Confirm=FALSE;
                        break;
                default:
                        break;
            }
            EnableWindows(ALLWindows);
        }
        if (ConfirmTemp) {

            if(T_Access)  { targetnode->nn_Area.ulevel=templatenode->nn_Area.ulevel; targetnode->nn_Area.llevel=templatenode->nn_Area.llevel; }
            if(T_Write)   { targetnode->nn_Area.uwrite=templatenode->nn_Area.uwrite; targetnode->nn_Area.lwrite=templatenode->nn_Area.lwrite; }
            if(T_Forward) { targetnode->nn_Area.uforward=templatenode->nn_Area.uforward; targetnode->nn_Area.lforward=templatenode->nn_Area.lforward; }
            if(T_ReEdit)  { targetnode->nn_Area.uedit=templatenode->nn_Area.uedit; targetnode->nn_Area.ledit=templatenode->nn_Area.ledit; }
            if(T_Copy)    { targetnode->nn_Area.ucopy=templatenode->nn_Area.ucopy; targetnode->nn_Area.lcopy=templatenode->nn_Area.lcopy; }
            if(T_Kill)    { targetnode->nn_Area.ukill=templatenode->nn_Area.ukill; targetnode->nn_Area.lkill=templatenode->nn_Area.lkill; }
            if(T_SysOp)   { targetnode->nn_Area.usysop=templatenode->nn_Area.usysop; targetnode->nn_Area.lsysop=templatenode->nn_Area.lsysop; }
            if(T_Capacity)  targetnode->nn_Area.Capacity=templatenode->nn_Area.Capacity;
            if(T_Charset)   targetnode->nn_Area.savecharset=templatenode->nn_Area.savecharset;
            if(T_MailType)  targetnode->nn_Area.Flag=templatenode->nn_Area.Flag;
            if(T_Handles) { targetnode->nn_Area.Flag |= HANDLES_AREA; if (!(templatenode->nn_Area.Flag & HANDLES_AREA)) targetnode->nn_Area.Flag ^= HANDLES_AREA; }

            if(T_Users)     MsgActionQ(USERCOPY,templatenode->nn_Area.Number,targetnode->nn_Area.Number);

            if(T_Translator) strncpy(targetnode->nn_Area.processor,templatenode->nn_Area.processor,12);

        }
        GT_SetGadgetAttrs(MsgAreaTemplateGadgets[GD_MsgTemplateTarg_listview],
                          MsgAreaTemplateWnd, NULL,
                          GTLV_Labels, ~0, TAG_DONE);

        areanode=MsgTempTargPtrs[i];

        Remove(areanode);

        GT_SetGadgetAttrs(MsgAreaTemplateGadgets[GD_MsgTemplateTarg_listview],
                          MsgAreaTemplateWnd, NULL,
                          GTLV_Labels, &MsgTempTargList_list,
                          GTLV_MakeVisible, 0,
                          GTLV_Selected, 0,
                          TAG_DONE);
    }

    return(FALSE);

}

int MsgTemplateConfirm_checkClicked (void)  {return(TRUE);}
int MsgTempReEdit_checkClicked (void)   {return(TRUE);}
int MsgTempKill_checkClicked (void)     {return(TRUE);}
int MsgTempSysop_checkClicked (void)    {return(TRUE);}
int MsgTempCopy_checkClicked (void)     {return(TRUE);}
int MsgTempUsers_checkClicked (void)    {return(TRUE);}
int MsgTempWrite_checkClicked (void)    {return(TRUE);}
int MsgTempAccess_checkClicked (void)   {return(TRUE);}
int MsgTempForward_checkClicked (void)  {return(TRUE);}
int MsgTempCapacity_checkClicked (void) {return(TRUE);}
int MsgTempTranslator_checkClicked (void) {return(TRUE);}
int MsgTempCharset_checkClicked (void)  {return(TRUE);}
int MsgTempHandles_checkClicked (void)  {return(TRUE);}
int MsgTempMailtype_checkClicked (void) {return(TRUE);}

int MsgTemplateCancel_buttonClicked( void ) {   /* "Cancel" */
    return(FALSE);
}
int MsgAreaTemplateCloseWindow( void ) {        /* "IDCMP_CLOSEWINDOW" */
    return(FALSE);
}
