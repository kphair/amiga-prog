#include "DLG-Edit.h"

/***
 *** File Area Manager
 ***/

void StoreFileGadgetStates(void)
{
    ULONG templong;
    struct DLGAreaNode *areanode;

    GT_GetGadgetAttrs(File_Area_ManagerGadgets[GD_FileArea_listview], File_Area_ManagerWnd, NULL,
                      GTLV_Selected, &templong, TAG_DONE);
    
    if (templong!=~0) {

        areanode=FileAreaPtrs[templong];

        // DO CYCLES

        /* Area access     */
        GT_GetGadgetAttrs(File_Area_ManagerGadgets[GD_FileEnter_low], File_Area_ManagerWnd, NULL,GTCY_Active, &templong,TAG_DONE); 
        areanode->nn_Area.llevel=atoi((const char *)AccessLevelLabels[templong]);
        GT_GetGadgetAttrs(File_Area_ManagerGadgets[GD_FileEnter_high], File_Area_ManagerWnd, NULL,GTCY_Active, &templong,TAG_DONE); 
        areanode->nn_Area.ulevel=atoi((const char *)AccessLevelLabels[templong]);

        /* Kill access     */
        GT_GetGadgetAttrs(File_Area_ManagerGadgets[GD_FileKill_low], File_Area_ManagerWnd, NULL,GTCY_Active, &templong,TAG_DONE);
        areanode->nn_Area.lkill=atoi((const char *)AccessLevelLabels[templong]);
        GT_GetGadgetAttrs(File_Area_ManagerGadgets[GD_FileKill_high], File_Area_ManagerWnd, NULL,GTCY_Active, &templong,TAG_DONE);
        areanode->nn_Area.ukill=atoi((const char *)AccessLevelLabels[templong]);
        
        /* Transfer access     */
        GT_GetGadgetAttrs(File_Area_ManagerGadgets[GD_FileHurl_low], File_Area_ManagerWnd, NULL,GTCY_Active, &templong,TAG_DONE);
        areanode->nn_Area.lcopy=atoi((const char *)AccessLevelLabels[templong]);
        GT_GetGadgetAttrs(File_Area_ManagerGadgets[GD_FileHurl_high], File_Area_ManagerWnd, NULL,GTCY_Active, &templong,TAG_DONE);
        areanode->nn_Area.ucopy=atoi((const char *)AccessLevelLabels[templong]);
        
        /* Upload access  */
        GT_GetGadgetAttrs(File_Area_ManagerGadgets[GD_FileUpload_low], File_Area_ManagerWnd, NULL,GTCY_Active, &templong,TAG_DONE);
        areanode->nn_Area.lwrite=atoi((const char *)AccessLevelLabels[templong]);
        GT_GetGadgetAttrs(File_Area_ManagerGadgets[GD_FileUpload_high], File_Area_ManagerWnd, NULL,GTCY_Active, &templong,TAG_DONE);
        areanode->nn_Area.uwrite=atoi((const char *)AccessLevelLabels[templong]);
        
        /* Download access   */
        GT_GetGadgetAttrs(File_Area_ManagerGadgets[GD_FileDownload_low], File_Area_ManagerWnd, NULL,GTCY_Active, &templong,TAG_DONE);
        areanode->nn_Area.lforward=atoi((const char *)AccessLevelLabels[templong]);
        GT_GetGadgetAttrs(File_Area_ManagerGadgets[GD_FileDownload_high], File_Area_ManagerWnd, NULL,GTCY_Active, &templong,TAG_DONE);
        areanode->nn_Area.uforward=atoi((const char *)AccessLevelLabels[templong]);
        
        /* SysOp access    */
        GT_GetGadgetAttrs(File_Area_ManagerGadgets[GD_FileSysOp_low], File_Area_ManagerWnd, NULL,GTCY_Active, &templong,TAG_DONE);
        areanode->nn_Area.lsysop=atoi((const char *)AccessLevelLabels[templong]);
        GT_GetGadgetAttrs(File_Area_ManagerGadgets[GD_FileSysOp_high], File_Area_ManagerWnd, NULL,GTCY_Active, &templong,TAG_DONE);
        areanode->nn_Area.usysop=atoi((const char *)AccessLevelLabels[templong]);

        // DO CHECKBOXES
        /* Freqable area */
        GT_GetGadgetAttrs(File_Area_ManagerGadgets[GD_FileFreq_checkbox],File_Area_ManagerWnd,NULL, GTCB_Checked,&templong, TAG_DONE);
        (areanode->nn_Area.Flag)|=FREQ_AREA;
        if(templong!=TRUE) (areanode->nn_Area.Flag)^=FREQ_AREA;

        /* Validation area */
        GT_GetGadgetAttrs(File_Area_ManagerGadgets[GD_FileValidation_checkbox],File_Area_ManagerWnd,NULL, GTCB_Checked,&templong, TAG_DONE);
        (areanode->nn_Area.Flag)|=VALIDATION_AREA;
        if(templong!=TRUE) (areanode->nn_Area.Flag)^=VALIDATION_AREA;

        /* Signature area */
        GT_GetGadgetAttrs(File_Area_ManagerGadgets[GD_FileSignature_checkbox],File_Area_ManagerWnd,NULL, GTCB_Checked,&templong, TAG_DONE);
        (areanode->nn_Area.Flag)|=SIGNATURE;
        if(templong!=TRUE) (areanode->nn_Area.Flag)^=SIGNATURE;

        // DO STRINGS

        GT_GetGadgetAttrs(File_Area_ManagerGadgets[GD_FileAreaName_string],File_Area_ManagerWnd,NULL, GTST_String,&templong, TAG_DONE);
        strcpy(areanode->nn_Area.Name,(char const *)templong);
        GT_GetGadgetAttrs(File_Area_ManagerGadgets[GD_FileAltFilePath_string],File_Area_ManagerWnd,NULL, GTST_String,&templong, TAG_DONE);
        strcpy(areanode->nn_Area.path,(char const *)templong);

        // DO INTEGERS

        GT_GetGadgetAttrs(File_Area_ManagerGadgets[GD_FileAreaNumber],File_Area_ManagerWnd,NULL, GTIN_Number,&templong, TAG_DONE);
        areanode->nn_Area.Number=templong;
        GT_GetGadgetAttrs(File_Area_ManagerGadgets[GD_FileValidationArea_integer],File_Area_ManagerWnd,NULL, GTIN_Number,&templong, TAG_DONE);
        areanode->nn_Area.Capacity=templong;
    }
}

    
    // LISTVIEW

int FileArea_listviewClicked( void ) 
{
    long SelectedArea=~0;
    struct DLGAreaNode *areanode;

    GT_GetGadgetAttrs(File_Area_ManagerGadgets[GD_FileArea_listview], File_Area_ManagerWnd, NULL,
                      GTLV_Selected, &SelectedArea, TAG_DONE);
    
    if(SelectedArea!=~0) {

        areanode=FileAreaPtrs[SelectedArea];

        // DO CYCLES

        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileEnter_low], File_Area_ManagerWnd, NULL, GA_Disabled,FALSE, GTCY_Active, AccessLevelIndex(areanode->nn_Area.llevel), TAG_DONE);
        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileEnter_high], File_Area_ManagerWnd, NULL, GA_Disabled,FALSE, GTCY_Active, AccessLevelIndex(areanode->nn_Area.ulevel), TAG_DONE);
        
        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileKill_low], File_Area_ManagerWnd, NULL, GA_Disabled,FALSE, GTCY_Active, AccessLevelIndex(areanode->nn_Area.lkill), TAG_DONE);
        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileKill_high], File_Area_ManagerWnd, NULL, GA_Disabled,FALSE, GTCY_Active, AccessLevelIndex(areanode->nn_Area.ukill), TAG_DONE);
        
        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileHurl_low], File_Area_ManagerWnd, NULL, GA_Disabled,FALSE, GTCY_Active, AccessLevelIndex(areanode->nn_Area.lcopy), TAG_DONE);
        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileHurl_high], File_Area_ManagerWnd, NULL, GA_Disabled,FALSE, GTCY_Active, AccessLevelIndex(areanode->nn_Area.ucopy), TAG_DONE);
        
        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileUpload_low], File_Area_ManagerWnd, NULL, GA_Disabled,FALSE, GTCY_Active, AccessLevelIndex(areanode->nn_Area.lwrite), TAG_DONE);
        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileUpload_high], File_Area_ManagerWnd, NULL, GA_Disabled,FALSE, GTCY_Active, AccessLevelIndex(areanode->nn_Area.uwrite), TAG_DONE);
        
        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileDownload_low], File_Area_ManagerWnd, NULL, GA_Disabled,FALSE, GTCY_Active, AccessLevelIndex(areanode->nn_Area.lforward), TAG_DONE);
        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileDownload_high], File_Area_ManagerWnd, NULL, GA_Disabled,FALSE, GTCY_Active, AccessLevelIndex(areanode->nn_Area.uforward), TAG_DONE);
        
        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileSysOp_low], File_Area_ManagerWnd, NULL, GA_Disabled,FALSE, GTCY_Active, AccessLevelIndex(areanode->nn_Area.lsysop), TAG_DONE);
        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileSysOp_high], File_Area_ManagerWnd, NULL, GA_Disabled,FALSE, GTCY_Active, AccessLevelIndex(areanode->nn_Area.usysop), TAG_DONE);
        
        // DO CHECKBOXES

        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileFreq_checkbox],File_Area_ManagerWnd,NULL, GA_Disabled,FALSE, GTCB_Checked, (areanode->nn_Area.Flag&FREQ_AREA ? TRUE : FALSE), TAG_DONE);
        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileValidation_checkbox],File_Area_ManagerWnd,NULL, GA_Disabled,FALSE, GTCB_Checked, (areanode->nn_Area.Flag&VALIDATION_AREA ? TRUE : FALSE), TAG_DONE);
        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileSignature_checkbox],File_Area_ManagerWnd,NULL, GA_Disabled,FALSE, GTCB_Checked, (areanode->nn_Area.Flag&SIGNATURE ? TRUE : FALSE), TAG_DONE);

        // DO STRINGS

        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileAreaName_string],File_Area_ManagerWnd,NULL, GA_Disabled,FALSE, GTST_String,areanode->nn_Area.Name, TAG_DONE);
        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileAltFilePath_string],File_Area_ManagerWnd,NULL, GA_Disabled,FALSE, GTST_String,areanode->nn_Area.path, TAG_DONE);

        // DO INTEGERS

        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileAreaNumber],File_Area_ManagerWnd,NULL, GA_Disabled,FALSE, GTIN_Number,areanode->nn_Area.Number, TAG_DONE);
        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileValidationArea_integer],File_Area_ManagerWnd,NULL, GA_Disabled,FALSE, GTIN_Number,areanode->nn_Area.Capacity, TAG_DONE);

        // DO BUTTONS

        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileAreaUsers_button],File_Area_ManagerWnd,NULL, GA_Disabled,FALSE, TAG_DONE);
        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileAreaUp_button],File_Area_ManagerWnd,NULL, GA_Disabled,FALSE, TAG_DONE);
        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileAreaDown_button],File_Area_ManagerWnd,NULL, GA_Disabled,FALSE, TAG_DONE);
        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_RemoveFileArea_button],File_Area_ManagerWnd,NULL, GA_Disabled,FALSE, TAG_DONE);
        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_InsertFileArea_button],File_Area_ManagerWnd,NULL, GA_Disabled,FALSE, TAG_DONE);
        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileAreaSave_button],File_Area_ManagerWnd,NULL, GA_Disabled,FALSE, TAG_DONE);
        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileImportAreas_button],File_Area_ManagerWnd,NULL, GA_Disabled,FALSE, TAG_DONE);
        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileApplyTemplate_button],File_Area_ManagerWnd,NULL, GA_Disabled,FALSE, TAG_DONE);
    }
    return(TRUE);
}




/*-------------------+
 |******************+*
 |* INTEGER GADGETS |*
 |+-----------------+*
 +********************/

int FileAreaNumberClicked( void ) { 

    long SelectedArea=~0;
    struct DLGAreaNode *areanode;
    struct DLGAreaNode *nextareanode;
    struct DLGAreaNode *currentareanode;
    int i; 
    BOOL sortswapped;
    SHORT oldareanum, newareanum, lastareanum;
    ULONG templong;

    GT_GetGadgetAttrs(File_Area_ManagerGadgets[GD_FileArea_listview], File_Area_ManagerWnd, NULL,
                      GTLV_Selected, &SelectedArea, TAG_DONE);


    GT_GetGadgetAttrs(File_Area_ManagerGadgets[GD_FileAreaNumber],File_Area_ManagerWnd,NULL, GTIN_Number,&templong, TAG_DONE);
    newareanum=templong;

    // Get last area node pointer and check for duplicate area number
    for (i=0; FileAreaPtrs[i]; i++) {
        areanode=FileAreaPtrs[i];
        if ((areanode->nn_Area.Number)==newareanum) newareanum=10000;
    }
    lastareanum=i-1;

    areanode=FileAreaPtrs[SelectedArea];
    currentareanode=areanode;
    oldareanum=areanode->nn_Area.Number;

    if((SelectedArea!=~0) && (newareanum!=10000)) {
        
        // Detach list before modifying it
        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileArea_listview],File_Area_ManagerWnd, NULL, GTLV_Labels, ~0, TAG_DONE);

        // Change area number
        areanode->nn_Area.Number=newareanum;
        sprintf((UBYTE *)&(areanode->nn_ListName), "%4d: %s", areanode->nn_Area.Number, areanode->nn_Area.Name);
        
        // Sort list of nodes
        do {
            sortswapped=FALSE;
            for (i=0; i<lastareanum-1; i++) {
                areanode=FileAreaPtrs[i];
                nextareanode=FileAreaPtrs[i+1];
                if ((areanode->nn_Area.Number) > (nextareanode->nn_Area.Number)) {
                    sortswapped=TRUE;
                    Remove((struct Node *)areanode);
                    Insert(&FileAreaList_list,(struct Node *)areanode,(struct Node *)nextareanode);
                    FileAreaPtrs[i]=nextareanode;
                    FileAreaPtrs[i+1]=areanode;
                }
            }
        } while(sortswapped==TRUE);
    
        for (i=0; FileAreaPtrs[i]!=currentareanode; i++);

        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileArea_listview],File_Area_ManagerWnd, NULL,
                          GTLV_Labels, &FileAreaList_list,
                          GTLV_MakeVisible, i,
                          GTLV_Selected, i,
                          TAG_DONE);
                          
        FileArea_listviewClicked();

        FileActionQ(RENUMAREA,oldareanum,newareanum);

    } else {
        DisableWindows(FILEWnd);
        DisplayBeep(NULL);
        EasyRequest(NULL, &AreaExistsReq, NULL, NULL);
        EnableWindows(FILEWnd);
    }

    return(TRUE); 
}

int FileValidationArea_integerClicked(void) { 
    StoreFileGadgetStates();
    return(TRUE); 
}




/*----------+
 |      //  *
 |  \\ //   *
 |   \X/    *
 +***********/

int FileFreq_checkboxClicked( void ) { 
    StoreFileGadgetStates();
    return(TRUE); 
}
int FileValidation_checkboxClicked( void ) { 
    StoreFileGadgetStates();
    return(TRUE); 
}
int FileSignature_checkboxClicked( void ) { 
    StoreFileGadgetStates();
    return(TRUE); 
}


/*------++---------------+
 | .--. *|               *
 | |  v *| CYCLE GADGETS *
 | `-   *|               *
 +*******+****************/

int FileEnter_lowClicked(void) { 
    StoreFileGadgetStates();
    return(TRUE); 
}
int FileEnter_highClicked(void) { 
    StoreFileGadgetStates();
    return(TRUE); 
}
int FileKill_lowClicked( void ) { 
    StoreFileGadgetStates();
    return(TRUE); 
}
int FileKill_highClicked( void ) { 
    StoreFileGadgetStates();
    return(TRUE); 
}
int FileHurl_lowClicked( void ) { 
    StoreFileGadgetStates();
    return(TRUE); 
}
int FileHurl_highClicked( void ) { 
    StoreFileGadgetStates();
    return(TRUE); 
}
int FileUpload_lowClicked( void ) { 
    StoreFileGadgetStates();
    return(TRUE); 
}
int FileUpload_highClicked( void ) { 
    StoreFileGadgetStates();
    return(TRUE); 
}
int FileDownload_lowClicked( void ) { 
    StoreFileGadgetStates();
    return(TRUE); 
}
int FileDownload_highClicked( void ) { 
    StoreFileGadgetStates();
    return(TRUE); 
}
int FileSysOp_lowClicked( void ) { 
    StoreFileGadgetStates();
    return(TRUE); 
}
int FileSysOp_highClicked( void ) { 
    StoreFileGadgetStates();
    return(TRUE); 
}


/*------------------+
 |*****************+*
 |* STRING GADGETS |*
 |+----------------+*
 +*******************/

int FileAreaName_stringClicked( void ) 
{
    long SelectedArea=~0;
    ULONG namestring;
    struct DLGAreaNode *areanode;

    GT_GetGadgetAttrs(File_Area_ManagerGadgets[GD_FileArea_listview], File_Area_ManagerWnd, NULL,
                      GTLV_Selected, &SelectedArea, TAG_DONE);

    if(SelectedArea!=~0) {

        // Detach list before modifying it
        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileArea_listview],File_Area_ManagerWnd, NULL, 
                          GTLV_Labels, ~0, TAG_DONE);

        areanode=FileAreaPtrs[SelectedArea];

        GT_GetGadgetAttrs(File_Area_ManagerGadgets[GD_FileAreaName_string],File_Area_ManagerWnd, NULL, 
                          GTST_String,&namestring, TAG_DONE);
        strcpy(areanode->nn_Area.Name,(char const *)namestring);
        sprintf((UBYTE *)&(areanode->nn_ListName), "%4d: %s", areanode->nn_Area.Number, areanode->nn_Area.Name);

        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileArea_listview],File_Area_ManagerWnd, NULL,
                          GTLV_Labels,&FileAreaList_list,
                          GTLV_MakeVisible, SelectedArea,
                          GTLV_Selected, SelectedArea,
                          TAG_DONE);
    }
    return(TRUE);
}

int FileAltFilePath_stringClicked( void ) { 
    StoreFileGadgetStates();
    return(TRUE); 
}


/*------------+
 |            *
 |    SAVE    *
 |            *
 +*************/

int FileAreaSave_buttonClicked(void) {
    StoreFileGadgetStates();
    SaveFileAreaList();
    return(TRUE);
}

/*------------+
 |            *
 |   REMOVE   *
 |            *
 +*************/

int RemoveFileArea_buttonClicked( void ) 
{
    long SelectedArea=~0;
    struct DLGAreaNode *areanode;
    int i;

    GT_GetGadgetAttrs(File_Area_ManagerGadgets[GD_FileArea_listview], File_Area_ManagerWnd, NULL,
                      GTLV_Selected, &SelectedArea, TAG_DONE);

    if(SelectedArea!=~0) {

        // Detach list before modifying it
        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileArea_listview],File_Area_ManagerWnd, NULL, GTLV_Labels, ~0, TAG_DONE);

        areanode=FileAreaPtrs[SelectedArea];

        Remove((struct Node *)areanode);

        for (i=SelectedArea; FileAreaPtrs[i+1]!=NULL; i++) FileAreaPtrs[i]=FileAreaPtrs[i+1];
        FileAreaPtrs[i]=NULL;
        if(SelectedArea==i) SelectedArea--;

        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileArea_listview],File_Area_ManagerWnd, NULL,
                          GTLV_Labels, &FileAreaList_list,
                          GTLV_MakeVisible, SelectedArea,
                          GTLV_Selected, SelectedArea,
                          TAG_DONE);
                          
        FileArea_listviewClicked();

        FileActionQ(KILLAREA,areanode->nn_Area.Number,0);

    }
    return(TRUE);
}

/*------------+
 |            *
 |   INSERT   *
 |            *
 +*************/

int InsertFileArea_buttonClicked( void ) {      /* "Insert" clicked */
    
    long SelectedArea=~0;
    struct DLGAreaNode *areanode;
    struct DLGAreaNode *nextareanode;
    struct DLGAreaNode *newareanode;
    int i;
    SHORT lastareanum;

    GT_GetGadgetAttrs(File_Area_ManagerGadgets[GD_FileArea_listview], File_Area_ManagerWnd, NULL,
                      GTLV_Selected, &SelectedArea, TAG_DONE);

    if(SelectedArea!=~0) {

        for (i=0; FileAreaPtrs[i]; i++);
        lastareanum=i-1;

        areanode=FileAreaPtrs[SelectedArea];
        nextareanode=FileAreaPtrs[SelectedArea+1];

        if ( ((nextareanode==NULL) && (areanode->nn_Area.Number!=9999)) || ((nextareanode->nn_Area.Number)!=(areanode->nn_Area.Number+1)) ) {
        
            // Detach list before modifying it
            GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileArea_listview],File_Area_ManagerWnd, NULL, GTLV_Labels, ~0, TAG_DONE);

            // Allocate new node...
            newareanode=AllocPooled(FileAreaList_pool,sizeof(struct DLGAreaNode));
            
            // Fill the area definition fields...
            memcpy(&(newareanode->nn_Area), (void *)&(areanode->nn_Area), sizeof(struct Msg_Area));
            newareanode->nn_Area.Number=newareanode->nn_Area.Number+1;
            sprintf((UBYTE *)&(newareanode->nn_Area.Name),"** CHANGE ME! **");
            sprintf((UBYTE *)&(newareanode->nn_Area.path),"\0");
            sprintf((UBYTE *)&(newareanode->nn_ListName), "%4d: %s", newareanode->nn_Area.Number, newareanode->nn_Area.Name);

            // Link it into the listview list...
            newareanode->nn_Node.ln_Name=newareanode->nn_ListName;
            newareanode->nn_Node.ln_Type=NAMENODE_ID;
            newareanode->nn_Node.ln_Pri=0;
            Insert(&FileAreaList_list,(struct Node *)newareanode,(struct Node *)areanode);

            // Insert new node pointer
            for (i=lastareanum+1; i>SelectedArea; i--) FileAreaPtrs[i+1]=FileAreaPtrs[i];
            FileAreaPtrs[i+1]=newareanode;

            GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileArea_listview],File_Area_ManagerWnd, NULL,
                              GTLV_Labels, &FileAreaList_list,
                              GTLV_MakeVisible, i+1,
                              GTLV_Selected, i+1,
                              TAG_DONE);
            FileArea_listviewClicked();

            FileActionQ(MAKEAREA,newareanode->nn_Area.Number,0);
        }
        else DisplayBeep(NULL);   /* Flash all screens */
    }
    return(TRUE);
}


/*------------+
 |            *
 |     UP     *
 |            *
 +*************/

int FileAreaUp_buttonClicked( void ) 
{
    long SelectedArea=~0;
    struct DLGAreaNode *areanode;
    struct DLGAreaNode *prevareanode;
    SHORT tempareanum;
    void *sel_temp;

    GT_GetGadgetAttrs(File_Area_ManagerGadgets[GD_FileArea_listview], File_Area_ManagerWnd, NULL,
                      GTLV_Selected, &SelectedArea, TAG_DONE);

    if(SelectedArea!=~0 && SelectedArea!=0) {

        // Detach list before modifying it
        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileArea_listview],File_Area_ManagerWnd, NULL, GTLV_Labels, ~0, TAG_DONE);

        areanode=FileAreaPtrs[SelectedArea];
        prevareanode=FileAreaPtrs[SelectedArea-1];

        FileActionQ(SWAPAREA,areanode->nn_Area.Number,prevareanode->nn_Area.Number);

        Remove((struct Node *)prevareanode);
        Insert(&FileAreaList_list,(struct Node *)prevareanode,(struct Node *)areanode);

        tempareanum=areanode->nn_Area.Number;
        areanode->nn_Area.Number=prevareanode->nn_Area.Number;
        prevareanode->nn_Area.Number=tempareanum;
        sprintf((UBYTE *)&(areanode->nn_ListName), "%4d: %s", areanode->nn_Area.Number, areanode->nn_Area.Name);
        sprintf((UBYTE *)&(prevareanode->nn_ListName), "%4d: %s", prevareanode->nn_Area.Number, prevareanode->nn_Area.Name);

        sel_temp=FileAreaPtrs[SelectedArea];
        FileAreaPtrs[SelectedArea]=FileAreaPtrs[SelectedArea-1];
        FileAreaPtrs[SelectedArea-1]=sel_temp;

        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileArea_listview],File_Area_ManagerWnd, NULL,
                          GTLV_Labels, &FileAreaList_list,
                          GTLV_MakeVisible, SelectedArea-1,
                          GTLV_Selected, SelectedArea-1,
                          TAG_DONE);
    }
    return(TRUE);
}

/*------------+
 |            *
 |    DOWN    *
 |            *
 +*************/

int FileAreaDown_buttonClicked( void ) 
{
    long SelectedArea=~0;
    struct DLGAreaNode *areanode;
    struct DLGAreaNode *nextareanode;
    SHORT tempareanum;
    void *sel_temp;

    GT_GetGadgetAttrs(File_Area_ManagerGadgets[GD_FileArea_listview], File_Area_ManagerWnd, NULL,
                      GTLV_Selected, &SelectedArea, TAG_DONE);

    if(SelectedArea!=~0 && FileAreaPtrs[SelectedArea+1]!=0) {

        // Detach list before modifying it
        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileArea_listview],File_Area_ManagerWnd, NULL, GTLV_Labels, ~0, TAG_DONE);

        areanode=FileAreaPtrs[SelectedArea];
        nextareanode=FileAreaPtrs[SelectedArea+1];

        FileActionQ(SWAPAREA,areanode->nn_Area.Number,nextareanode->nn_Area.Number);

        Remove((struct Node *)areanode);
        Insert(&FileAreaList_list,(struct Node *)areanode,(struct Node *)nextareanode);

        tempareanum=areanode->nn_Area.Number;
        areanode->nn_Area.Number=nextareanode->nn_Area.Number;
        nextareanode->nn_Area.Number=tempareanum;
        sprintf((UBYTE *)&(areanode->nn_ListName), "%4d: %s", areanode->nn_Area.Number, areanode->nn_Area.Name);
        sprintf((UBYTE *)&(nextareanode->nn_ListName), "%4d: %s", nextareanode->nn_Area.Number, nextareanode->nn_Area.Name);

        sel_temp=FileAreaPtrs[SelectedArea];
        FileAreaPtrs[SelectedArea]=FileAreaPtrs[SelectedArea+1];
        FileAreaPtrs[SelectedArea+1]=sel_temp;


        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileArea_listview],File_Area_ManagerWnd, NULL,
                          GTLV_Labels, &FileAreaList_list,
                          GTLV_MakeVisible, SelectedArea+1,
                          GTLV_Selected, SelectedArea+1,
                          TAG_DONE);
    }
    return(TRUE);
}


/*------------+
 |            *
 |   IMPORT   *
 |            *
 +*************/

int FileImportAreas_buttonClicked( void ) {

    struct FileRequester *myfilerequester;
    char *ourfile, *ourdrawer;
    static char fullpath[250];
    FILE *importfile;

    long SelectedArea=~0;
    struct DLGAreaNode *areanode;
    struct DLGAreaNode *tempareanode;
    struct DLGAreaNode *newareanode;
    int i;
    char *sptr;
    SHORT lastareanum,newareanum,lastfilearea;
    char *tempbuff=fullpath;
    BOOL areaplaced;

    char importreqtitle[]="Select the import list file...";

    struct TagItem myfrtags[3];
    
    myfrtags[0].ti_Tag =ASLFR_TitleText;
    myfrtags[0].ti_Data=(ULONG)importreqtitle;
    myfrtags[1].ti_Tag =ASLFR_RejectIcons;
    myfrtags[1].ti_Data=TRUE;
    myfrtags[2].ti_Tag =TAG_DONE;

    myfilerequester=AllocAslRequest(ASL_FileRequest,myfrtags);

    GT_GetGadgetAttrs(File_Area_ManagerGadgets[GD_FileArea_listview], File_Area_ManagerWnd, NULL,
                      GTLV_Selected, &SelectedArea, TAG_DONE);

    for (i=0; FileAreaPtrs[i]; i++); lastareanum=i-1;

    tempareanode=FileAreaPtrs[SelectedArea];
    lastfilearea=(tempareanode->nn_Area.Number)+1;

    DisableWindows(FILEWnd);

    if (AslRequest(myfilerequester,NULL)) {

        ourfile=myfilerequester->fr_File;
        ourdrawer=myfilerequester->fr_Drawer;
        if (strlen(ourdrawer) && ourdrawer[strlen(ourdrawer)]!=':') {
            sprintf(fullpath,"%s/%s",ourdrawer,ourfile);
        } else {
            sprintf(fullpath,"%s",ourfile);
        }

        if(importfile=fopen(fullpath,"rt")) {
            do {

                // Detach list before modifying it
                GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileArea_listview],File_Area_ManagerWnd, NULL, GTLV_Labels, ~0, TAG_DONE);
                
                // Allocate new area node...
                newareanode=AllocPooled(FileAreaList_pool,sizeof(struct DLGAreaNode));

                // Define the new number, name and path fields...
                memcpy(&(newareanode->nn_Area), (void *)&(tempareanode->nn_Area), sizeof(struct Msg_Area));
                
                // Area number
                memset(tempbuff,0,sizeof(fullpath));
                fgets(tempbuff, 250, importfile);
                newareanum=atoi(tempbuff);

                if (newareanum>0 && newareanum<10000) {
                    newareanode->nn_Area.Number=newareanum;
                    lastfilearea=newareanum+1;
                } else {
                    newareanode->nn_Area.Number=lastfilearea;
                    newareanum=lastfilearea++;
                }

                for (i=0; i<=lastareanum; i++) {
                    areanode=FileAreaPtrs[i];
                    if (newareanum==areanode->nn_Area.Number) {
                        DisplayBeep(NULL);
                        EasyRequest(NULL, &ImportAreaExistsReq, NULL, newareanum);
                        FreePooled(FileAreaList_pool,newareanode,sizeof(struct DLGAreaNode));
                        break;
                    }
                }
                
                if (newareanum!=areanode->nn_Area.Number) {

                    // Area name
                    memset(tempbuff,0,sizeof(fullpath));
                    fgets(tempbuff, 250, importfile);
                    for (sptr=tempbuff; sptr<(tempbuff+sizeof(newareanode->nn_Area.Name)); sptr++) if (*sptr==10) *sptr=0;
                    *sptr=0;
                    strcpy(newareanode->nn_Area.Name,tempbuff);

                    // Alternate path
                    memset(tempbuff,0,sizeof(fullpath));
                    fgets(tempbuff, 250, importfile);
                    for (sptr=tempbuff; sptr<(tempbuff+sizeof(newareanode->nn_Area.path)); sptr++) if (*sptr==10) *sptr=0;
                    *sptr=0;
                    strcpy(newareanode->nn_Area.path,tempbuff);

                    // Wait for the blank line
                    while(strlen(tempbuff)>1) {
                        memset(tempbuff,0,sizeof(fullpath));
                        fgets(tempbuff, 250, importfile);
                    }

                    // Validate node entry
                    sprintf((UBYTE *)&(newareanode->nn_ListName), "%4d: %s", newareanode->nn_Area.Number, newareanode->nn_Area.Name);
                    newareanode->nn_Node.ln_Name=newareanode->nn_ListName;
                    newareanode->nn_Node.ln_Type=NAMENODE_ID;
                    newareanode->nn_Node.ln_Pri=0;
                
                    // Do an insert sort with the newly created area node...
                    areaplaced=FALSE;
                    for (i=lastareanum; i>=0; i--) {
                        areanode=FileAreaPtrs[i];
                        FileAreaPtrs[i+1]=FileAreaPtrs[i];
                        if (newareanode->nn_Area.Number > areanode->nn_Area.Number) {
                            Insert(&FileAreaList_list,(struct Node *)newareanode,(struct Node *)areanode);
                            FileAreaPtrs[++i]=newareanode;
                            areaplaced=TRUE;
                        }
                        if (areaplaced==TRUE) break;
                        if (i==0 && (newareanode->nn_Area.Number != areanode->nn_Area.Number)) {
                            AddHead(&FileAreaList_list,(struct Node *)newareanode);
                            FileAreaPtrs[i]=newareanode;
                        }
                    }
                    FileActionQ(MAKEAREA,newareanode->nn_Area.Number,0);
                    lastareanum++;

                    GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileArea_listview],File_Area_ManagerWnd, NULL,
                                      GTLV_Labels, &FileAreaList_list,
                                      GTLV_MakeVisible, i,
                                      GTLV_Selected, i,
                                      TAG_DONE);
                }
            } while(!feof(importfile));
            fclose(importfile);
            FileArea_listviewClicked();
        }
        else {
            DisplayBeep(NULL);   /* Flash all screens */
            EasyRequest(NULL, &BadImportFileReq, NULL, fullpath);
        }
    }
    EnableWindows(FILEWnd);
    return(TRUE);
}

/*------------+
 |            *
 |  TEMPLATE  *
 |            *
 +*************/

int FileApplyTemplate_buttonClicked( void )
{
    if (!OpenFileAreaTemplateWindow()) {

        filetemp_mask=(1 << FileAreaTemplateWnd->UserPort->mp_SigBit);
        all_mask |= filetemp_mask;
        active_wnd |= FILETemp;

        DisableWindows(FILEWnd);

        FileTempSrcPtrs=  (struct Node **)calloc(10000,sizeof(ULONG));
        FileTempTargPtrs= (struct Node **)calloc(10000,sizeof(ULONG));

        CopyFileAreaList();

        /*
         * Set listviews
         */
        GT_SetGadgetAttrs(FileAreaTemplateGadgets[GD_FileTemplateSrc_listview],
                          FileAreaTemplateWnd, NULL,
                          GTLV_Labels, &FileTempSrcList_list,
                          TAG_DONE);
        GT_SetGadgetAttrs(FileAreaTemplateGadgets[GD_FileTemplateTarg_listview],
                          FileAreaTemplateWnd, NULL,
                          GTLV_Labels, &FileTempTargList_list,
                          GA_Disabled, TRUE,
                          TAG_DONE);
    }
    return(TRUE);
}

/*---------------+
 |               *
 |  USER ACCESS  *
 |               *
 +****************/

int FileAreaUsers_buttonClicked( void )
{
    if (!OpenFileUserAccessWindow()) {

        fileaccess_mask=(1 << FileUserAccessWnd->UserPort->mp_SigBit);
        all_mask |= fileaccess_mask;
        active_wnd |= FILEAccess;

        DisableWindows(FILEWnd);

        FileAccessUserSrcPtrs=  (struct Node **)calloc(10000,sizeof(ULONG));
        FileAccessUserTargPtrs= (struct Node **)calloc(10000,sizeof(ULONG));
        FileAccessAreaSrcPtrs=  (struct Node **)calloc(10000,sizeof(ULONG));
        FileAccessAreaTargPtrs= (struct Node **)calloc(10000,sizeof(ULONG));

        CopyAccessLists();

        /*
         * Set listviews
         */
        GT_SetGadgetAttrs(FileUserAccessGadgets[GD_FileAccessUserSrc_listview],
                          FileUserAccessWnd, NULL,
                          GTLV_Labels, &FileAccessUserSrcList_list,
                          TAG_DONE);
        GT_SetGadgetAttrs(FileUserAccessGadgets[GD_FileAccessUserTarg_listview],
                          FileUserAccessWnd, NULL,
                          GTLV_Labels, &FileAccessUserTargList_list,
                          GA_Disabled, TRUE,
                          TAG_DONE);
        GT_SetGadgetAttrs(FileUserAccessGadgets[GD_FileAccessAreaSrc_listview],
                          FileUserAccessWnd, NULL,
                          GTLV_Labels, &FileAccessAreaSrcList_list,
                          TAG_DONE);
        GT_SetGadgetAttrs(FileUserAccessGadgets[GD_FileAccessAreaTarg_listview],
                          FileUserAccessWnd, NULL,
                          GTLV_Labels, &FileAccessAreaTargList_list,
                          GA_Disabled, TRUE,
                          TAG_DONE);
    }
    return(TRUE);
}


    // RAW KEY PROCESSOR

int File_Area_ManagerRawKey(void)
{
    long SelectedArea=~0;

    switch(File_Area_ManagerMsg.Code) {

        case 76:        // Cursor up

            GT_GetGadgetAttrs(File_Area_ManagerGadgets[GD_FileArea_listview], File_Area_ManagerWnd, NULL, GTLV_Selected, &SelectedArea, TAG_DONE);
            if(SelectedArea!=~0 && SelectedArea!=0) GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileArea_listview],File_Area_ManagerWnd, NULL, GTLV_MakeVisible, SelectedArea-1, GTLV_Selected, SelectedArea-1, TAG_DONE);
            FileArea_listviewClicked();
            break;

        case 77:        // Cursor Down

            GT_GetGadgetAttrs(File_Area_ManagerGadgets[GD_FileArea_listview], File_Area_ManagerWnd, NULL, GTLV_Selected, &SelectedArea, TAG_DONE);
            if(SelectedArea!=~0 && FileAreaPtrs[SelectedArea+1]!=0) GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileArea_listview],File_Area_ManagerWnd, NULL, GTLV_MakeVisible, SelectedArea+1, GTLV_Selected, SelectedArea+1, TAG_DONE);
            FileArea_listviewClicked();
            break;

    }
    return(TRUE);
}



/*--------+
 |  ****  *
 |  *  |  *
 |  *--+  *
 +*********/

int File_Area_ManagerCloseWindow( void ) {          /* "IDCMP_CLOSEWINDOW". */
    StoreFileGadgetStates();
    return(FALSE);
}

