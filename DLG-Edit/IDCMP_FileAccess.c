#include "DLG-Edit.h"

int FileAccessAreaTarg_listviewClicked( void ) {

    long SelectedArea=~0;
    long lastareaslot=0;
    SHORT SrcAreaNum=0;
    BOOL areaplaced;
    ULONG LVDisabled;
    struct Node *areanode;
    struct Node *testnode;
    int i;

    for (i=0; FileAccessAreaSrcPtrs[i]; i++);
    if(i) lastareaslot=i-1;

    GT_GetGadgetAttrs(FileUserAccessGadgets[GD_FileAccessAreaTarg_listview],
                      FileUserAccessWnd, NULL,
                      GTLV_Selected, &SelectedArea, TAG_DONE);

    if(SelectedArea!=~0) {

        /* Detach lists before modifying */

        GT_SetGadgetAttrs(FileUserAccessGadgets[GD_FileAccessAreaTarg_listview],
                          FileUserAccessWnd, NULL,
                          GTLV_Labels, ~0, TAG_DONE);
        GT_SetGadgetAttrs(FileUserAccessGadgets[GD_FileAccessAreaSrc_listview],
                          FileUserAccessWnd, NULL,
                          GTLV_Labels, ~0, TAG_DONE);

        /*
         * Get area node pointer, remove it from destination listview and pointer list,
         * then add it back into source listview and pointer list
         */
        areanode=FileAccessAreaTargPtrs[SelectedArea];

        Remove(areanode);

        for (i=SelectedArea; FileAccessAreaTargPtrs[i+1]; i++) FileAccessAreaTargPtrs[i]=FileAccessAreaTargPtrs[i+1];

        FileAccessAreaTargPtrs[i]=NULL;
        if(SelectedArea==i) SelectedArea--;

        SrcAreaNum=atoi(areanode->ln_Name);

        /* Insert new node pointer */

        if (FileAccessAreaSrcPtrs[0]) {
            areaplaced=FALSE;
            for (i=lastareaslot; i>=0 && areaplaced==FALSE; i--) {
                testnode=FileAccessAreaSrcPtrs[i];
                FileAccessAreaSrcPtrs[i+1]=FileAccessAreaSrcPtrs[i];
                if (SrcAreaNum > atoi(testnode->ln_Name)) {
                    Insert(&FileAccessAreaSrcList_list,areanode,testnode);
                    FileAccessAreaSrcPtrs[++i]=areanode;
                    areaplaced=TRUE;
                }
                else if (i==0) {
                    AddHead(&FileAccessAreaSrcList_list,areanode);
                    FileAccessAreaSrcPtrs[i]=areanode;
                }
            }
        }
        else {
            AddHead(&FileAccessAreaSrcList_list,areanode);
            FileAccessAreaSrcPtrs[i=0]=areanode;
        }


        /*
         * We've just moved an area back from the target into the source list
         */

        GT_SetGadgetAttrs(FileUserAccessGadgets[GD_FileAccessAreaSrc_listview],
                          FileUserAccessWnd, NULL,
                          GTLV_Labels, &FileAccessAreaSrcList_list,
                          GTLV_MakeVisible, i+1,
                          GTLV_Selected, i+1,
                          GA_Disabled, FALSE,
                          TAG_DONE);

        if (FileAccessAreaTargPtrs[0]) LVDisabled=FALSE; else LVDisabled=TRUE;

        GT_SetGadgetAttrs(FileUserAccessGadgets[GD_FileAccessAreaTarg_listview],
                          FileUserAccessWnd, NULL,
                          GTLV_Labels, &FileAccessAreaTargList_list,
                          GTLV_MakeVisible, SelectedArea,
                          GTLV_Selected, SelectedArea,
                          GA_Disabled, LVDisabled,
                          TAG_DONE);

        GT_SetGadgetAttrs(FileUserAccessGadgets[GD_FileAccessBegin_button],
                          FileUserAccessWnd, NULL,
                          GA_Disabled, LVDisabled,
                          TAG_DONE);
    }

    return(TRUE);
}
int FileAccessAreaSrc_listviewClicked( void ) {

    long SelectedArea=~0;
    long lastareaslot=0;
    SHORT SrcAreaNum=0;
    BOOL areaplaced;
    ULONG LVDisabled;
    struct Node *areanode;
    struct Node *testnode;
    int i;

    for (i=0; FileAccessAreaTargPtrs[i]; i++);
    if(i) lastareaslot=i-1;

    GT_GetGadgetAttrs(FileUserAccessGadgets[GD_FileAccessAreaSrc_listview],
                      FileUserAccessWnd, NULL,
                      GTLV_Selected, &SelectedArea, TAG_DONE);

    if(SelectedArea!=~0) {

        /* Detach lists before modifying */

        GT_SetGadgetAttrs(FileUserAccessGadgets[GD_FileAccessAreaSrc_listview],
                          FileUserAccessWnd, NULL,
                          GTLV_Labels, ~0, TAG_DONE);
        GT_SetGadgetAttrs(FileUserAccessGadgets[GD_FileAccessAreaTarg_listview],
                          FileUserAccessWnd, NULL,
                          GTLV_Labels, ~0, TAG_DONE);

        /*
         * Get area node pointer, remove it from source listview and pointer list,
         * then add it into target listview and pointer list
         */
        areanode=FileAccessAreaSrcPtrs[SelectedArea];

        SrcAreaNum=atoi(areanode->ln_Name);

        Remove(areanode);

        for (i=SelectedArea; FileAccessAreaSrcPtrs[i+1]; i++) FileAccessAreaSrcPtrs[i]=FileAccessAreaSrcPtrs[i+1];
        FileAccessAreaSrcPtrs[i]=NULL;
        if(SelectedArea==i) SelectedArea--;

        /* Insert new node pointer */

        if (FileAccessAreaTargPtrs[0]) {
            areaplaced=FALSE;
            for (i=lastareaslot; i>=0 && areaplaced==FALSE; i--) {
                testnode=FileAccessAreaTargPtrs[i];
                FileAccessAreaTargPtrs[i+1]=FileAccessAreaTargPtrs[i];
                if (SrcAreaNum > atoi(testnode->ln_Name)) {
                    Insert(&FileAccessAreaTargList_list,areanode,testnode);
                    FileAccessAreaTargPtrs[++i]=areanode;
                    areaplaced=TRUE;
                }
                else if (i==0) {
                    AddHead(&FileAccessAreaTargList_list,areanode);
                    FileAccessAreaTargPtrs[i]=areanode;
                }
            }
        }
        else {
            AddHead(&FileAccessAreaTargList_list,areanode);
            FileAccessAreaTargPtrs[i=0]=areanode;
        }

        /* Re-attach target list to listview */

        GT_SetGadgetAttrs(FileUserAccessGadgets[GD_FileAccessAreaTarg_listview],
                          FileUserAccessWnd, NULL,
                          GTLV_Labels, &FileAccessAreaTargList_list,
                          GTLV_MakeVisible, i+1,
                          GTLV_Selected, i+1,
                          GA_Disabled, FALSE,
                          TAG_DONE);

        if (FileAccessAreaSrcPtrs[0]) LVDisabled=FALSE; else LVDisabled=TRUE;

        /* Re-attach source list to listview */

        GT_SetGadgetAttrs(FileUserAccessGadgets[GD_FileAccessAreaSrc_listview],
                          FileUserAccessWnd, NULL,
                          GTLV_Labels, &FileAccessAreaSrcList_list,
                          GTLV_MakeVisible, SelectedArea,
                          GTLV_Selected, SelectedArea,
                          GA_Disabled, LVDisabled,
                          TAG_DONE);

        /* Update status of "Apply" button */

        GT_SetGadgetAttrs(FileUserAccessGadgets[GD_FileAccessBegin_button],
                          FileUserAccessWnd, NULL,
                          GA_Disabled, FALSE,
                          TAG_DONE);

    }

    return(TRUE);
}

/*
 * Move a user from the source list into the target list
 */

int FileAccessUserSrc_listviewClicked( void ) {

    long SelectedUser=~0;
    long lastuserslot=0;
    SHORT SrcUserNum=0;
    BOOL areaplaced;
    ULONG LVDisabled;
    struct UserNameNode *usernode;
    struct UserNameNode *testnode;
    int i;

    for (i=0; FileAccessUserTargPtrs[i]; i++);
    if(i) lastuserslot=i-1;

    GT_GetGadgetAttrs(FileUserAccessGadgets[GD_FileAccessUserSrc_listview],
                      FileUserAccessWnd, NULL,
                      GTLV_Selected, &SelectedUser, TAG_DONE);

    if(SelectedUser!=~0) {

        /* Detach lists before modifying */

        GT_SetGadgetAttrs(FileUserAccessGadgets[GD_FileAccessUserSrc_listview],
                          FileUserAccessWnd, NULL,
                          GTLV_Labels, ~0, TAG_DONE);
        GT_SetGadgetAttrs(FileUserAccessGadgets[GD_FileAccessUserTarg_listview],
                          FileUserAccessWnd, NULL,
                          GTLV_Labels, ~0, TAG_DONE);

        /*
         * Get user node pointer, remove it from source listview and pointer list,
         * then add it into target listview and pointer list
         */
        usernode=FileAccessUserSrcPtrs[SelectedUser];

        SrcUserNum=usernode->nn_Number;

        Remove(&(usernode->nn_Node));

        for (i=SelectedUser; FileAccessUserSrcPtrs[i+1]; i++) FileAccessUserSrcPtrs[i]=FileAccessUserSrcPtrs[i+1];
        FileAccessUserSrcPtrs[i]=NULL;
        if(SelectedUser==i) SelectedUser--;

        /* Insert new node pointer */

        if (FileAccessUserTargPtrs[0]) {
            areaplaced=FALSE;
            for (i=lastuserslot; i>=0 && areaplaced==FALSE; i--) {
                testnode=FileAccessUserTargPtrs[i];
                FileAccessUserTargPtrs[i+1]=FileAccessUserTargPtrs[i];
                if (SrcUserNum > (testnode->nn_Number)) {
                    Insert(&FileAccessUserTargList_list,&(usernode->nn_Node),&(testnode->nn_Node));
                    FileAccessUserTargPtrs[++i]=usernode;
                    areaplaced=TRUE;
                }
                else if (i==0) {
                    AddHead(&FileAccessUserTargList_list,&(usernode->nn_Node));
                    FileAccessUserTargPtrs[i]=usernode;
                }
            }
        }
        else {
            AddHead(&FileAccessUserTargList_list,&(usernode->nn_Node));
            FileAccessUserTargPtrs[i=0]=usernode;
        }

        /* Re-attach target list to listview */

        GT_SetGadgetAttrs(FileUserAccessGadgets[GD_FileAccessUserTarg_listview],
                          FileUserAccessWnd, NULL,
                          GTLV_Labels, &FileAccessUserTargList_list,
                          GTLV_MakeVisible, i+1,
                          GTLV_Selected, i+1,
                          GA_Disabled, FALSE,
                          TAG_DONE);

        if (FileAccessUserSrcPtrs[0]) LVDisabled=FALSE; else LVDisabled=TRUE;

        /* Re-attach source list to listview */

        GT_SetGadgetAttrs(FileUserAccessGadgets[GD_FileAccessUserSrc_listview],
                          FileUserAccessWnd, NULL,
                          GTLV_Labels, &FileAccessUserSrcList_list,
                          GTLV_MakeVisible, SelectedUser,
                          GTLV_Selected, SelectedUser,
                          GA_Disabled, LVDisabled,
                          TAG_DONE);

        /* Update status of "Begin" button */

        GT_SetGadgetAttrs(FileUserAccessGadgets[GD_FileAccessBegin_button],
                          FileUserAccessWnd, NULL,
                          GA_Disabled, FALSE,
                          TAG_DONE);

    }
    return(TRUE);
}

/*
 * Move a user from the target listview into the source listview
 */

int FileAccessUserTarg_listviewClicked( void ) {

    long SelectedUser=~0;
    long lastuserslot=0;
    SHORT SrcUserNum=0;
    BOOL areaplaced;
    ULONG LVDisabled;
    struct UserNameNode *usernode;
    struct UserNameNode *testnode;
    int i;

    for (i=0; FileAccessUserSrcPtrs[i]; i++);
    if(i) lastuserslot=i-1;

    GT_GetGadgetAttrs(FileUserAccessGadgets[GD_FileAccessUserTarg_listview],
                      FileUserAccessWnd, NULL,
                      GTLV_Selected, &SelectedUser, TAG_DONE);

    if(SelectedUser!=~0) {

        /* Detach lists before modifying */

        GT_SetGadgetAttrs(FileUserAccessGadgets[GD_FileAccessUserTarg_listview],
                          FileUserAccessWnd, NULL,
                          GTLV_Labels, ~0, TAG_DONE);
        GT_SetGadgetAttrs(FileUserAccessGadgets[GD_FileAccessUserSrc_listview],
                          FileUserAccessWnd, NULL,
                          GTLV_Labels, ~0, TAG_DONE);

        /*
         * Get area node pointer, remove it from source listview and pointer list,
         * then add it into target listview and pointer list
         */
        usernode=FileAccessUserTargPtrs[SelectedUser];

        Remove(&(usernode->nn_Node));

        for (i=SelectedUser; FileAccessUserTargPtrs[i+1]; i++) FileAccessUserTargPtrs[i]=FileAccessUserTargPtrs[i+1];

        FileAccessUserTargPtrs[i]=NULL;
        if(SelectedUser==i) SelectedUser--;

        /* Update text gadget to new selection status */
        GT_SetGadgetAttrs(FileUserAccessGadgets[GD_FileAccessUsers_text]
                          FileUserAccessWnd, NULL,
                          GTST_Text, NULL, TAG_DONE);

        SrcUserNum=usernode->nn_Number;

        /* Insert new node pointer */

        if (FileAccessUserSrcPtrs[0]) {
            areaplaced=FALSE;
            for (i=lastuserslot; i>=0 && areaplaced==FALSE; i--) {
                testnode=FileAccessUserSrcPtrs[i];
                FileAccessUserSrcPtrs[i+1]=FileAccessUserSrcPtrs[i];
                if (SrcUserNum > (testnode->nn_Number)) {
                    Insert(&FileAccessUserSrcList_list,&(usernode->nn_Node),&(testnode->nn_Node));
                    FileAccessUserSrcPtrs[++i]=usernode;
                    areaplaced=TRUE;
                }
                else if (i==0) {
                    AddHead(&FileAccessUserSrcList_list,&(usernode->nn_Node));
                    FileAccessUserSrcPtrs[i]=usernode;
                }
            }
        }
        else {
            AddHead(&FileAccessUserSrcList_list,&(usernode->nn_Node));
            FileAccessUserSrcPtrs[i=0]=usernode;
        }


        /*
         * We've just moved a user back from the target into the source list
         */

        GT_SetGadgetAttrs(FileUserAccessGadgets[GD_FileAccessUserSrc_listview],
                          FileUserAccessWnd, NULL,
                          GTLV_Labels, &FileAccessUserSrcList_list,
                          GTLV_MakeVisible, i+1,
                          GTLV_Selected, i+1,
                          GA_Disabled, FALSE,
                          TAG_DONE);

        if (FileAccessUserTargPtrs[0]) LVDisabled=FALSE; else LVDisabled=TRUE;

        GT_SetGadgetAttrs(FileUserAccessGadgets[GD_FileAccessUserTarg_listview],
                          FileUserAccessWnd, NULL,
                          GTLV_Labels, &FileAccessUserTargList_list,
                          GTLV_MakeVisible, SelectedUser,
                          GTLV_Selected, SelectedUser,
                          GA_Disabled, LVDisabled,
                          TAG_DONE);

        GT_SetGadgetAttrs(FileUserAccessGadgets[GD_FileAccessBegin_button],
                          FileUserAccessWnd, NULL,
                          GA_Disabled, LVDisabled,
                          TAG_DONE);
    }
    return(TRUE);
}

/* "BEGIN" */
int FileAccessBegin_buttonClicked( void ) {

    ULONG SelectedUser=~0;
    struct UserNameNode *srcnode;
    struct UserNameNode *targetnode;
    struct Node *tempnode;
    int i,j;
    SHORT srcnumber, targetnumber;

    /*
     * Get selected entry in the original area listview
     */
    GT_GetGadgetAttrs(File_Area_ManagerGadgets[GD_FileArea_listview], File_Area_ManagerWnd, NULL, GTLV_Selected, &SelectedUser, TAG_DONE);
    srcnode=UserNodePtrs[SelectedUser];
    srcnumber=srcnode->nn_Number;

    GT_SetGadgetAttrs(FileUserAccessGadgets[GD_FileAccessUserTarg_listview],
                      FileUserAccessWnd, NULL,
                      GTLV_MakeVisible, 0,
                      GTLV_Selected, 0,
                      TAG_DONE);

/************************
    for (i=j=0; tempnode=FileAccessUserTargPtrs[i]; i++) {

         * Find number of current target, then scan full list of user nodes
         * until a match is found. Then we can work on that.
        targetnumber=atoi(tempnode->ln_Name);
        for(targetnode=FileAreaPtrs[j++]; (targetnode->nn_Area.Number)!=targetnumber; targetnode=FileAreaPtrs[j++])

        GT_SetGadgetAttrs(FileUserAccessGadgets[GD_FileAccessUserTarg_listview],
                          FileUserAccessWnd, NULL,
                          GTLV_Labels, ~0, TAG_DONE);

        Remove(FileAccessUserTargPtrs[i]);

        GT_SetGadgetAttrs(FileUserAccessGadgets[GD_FileAccessUserTarg_listview],
                          FileUserAccessWnd, NULL,
                          GTLV_Labels, &FileAccessUserTargList_list,
                          GTLV_MakeVisible, 0,
                          GTLV_Selected, 0,
                          TAG_DONE);
    }
************************/

    return(FALSE);
}

int FileUserAccessMXClicked (void) {            /* Add/Remove */
    return(TRUE);
}

int FileAccessCancel_buttonClicked (void) {     /* "Cancel" */
    return(FALSE);
}
int FileUserAccessCloseWindow (void) {          /* "IDCMP_CLOSEWINDOW" */
    return(FALSE);
}

