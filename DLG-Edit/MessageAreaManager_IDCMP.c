/***
 *** Message Area Manager
 ***/

void StoreMsgGadgetStates(void) 
{
    ULONG templong;
    struct DLGAreaNode *areanode;

    GT_GetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgArea_listview], Message_Area_ManagerWnd, NULL,
                      GTLV_Selected, &templong, TAG_DONE);

    if (templong!=~0) {
    
        areanode=MsgAreaPtrs[templong];

        // DO CYCLES

        /* Area access     */
        GT_GetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgEnter_low], Message_Area_ManagerWnd, NULL, GTCY_Active, &templong, TAG_DONE);
            areanode->nn_Area.llevel=atoi((const char *)AccessLevelLabels[templong]);
        GT_GetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgEnter_high], Message_Area_ManagerWnd, NULL, GTCY_Active, &templong, TAG_DONE);
            areanode->nn_Area.ulevel=atoi((const char *)AccessLevelLabels[templong]);
        
        GT_GetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgWrite_low], Message_Area_ManagerWnd, NULL, GTCY_Active, &templong, TAG_DONE);
            areanode->nn_Area.lwrite=atoi((const char *)AccessLevelLabels[templong]);
        GT_GetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgWrite_high], Message_Area_ManagerWnd, NULL, GTCY_Active, &templong, TAG_DONE);
            areanode->nn_Area.uwrite=atoi((const char *)AccessLevelLabels[templong]);
        
        GT_GetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgKill_low], Message_Area_ManagerWnd, NULL, GTCY_Active, &templong, TAG_DONE);
            areanode->nn_Area.lkill=atoi((const char *)AccessLevelLabels[templong]);
        GT_GetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgKill_high], Message_Area_ManagerWnd, NULL, GTCY_Active, &templong, TAG_DONE);
            areanode->nn_Area.ukill=atoi((const char *)AccessLevelLabels[templong]);
        
        GT_GetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgHurl_low], Message_Area_ManagerWnd, NULL, GTCY_Active, &templong, TAG_DONE);
            areanode->nn_Area.lcopy=atoi((const char *)AccessLevelLabels[templong]);
        GT_GetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgHurl_high], Message_Area_ManagerWnd, NULL, GTCY_Active, &templong, TAG_DONE);
            areanode->nn_Area.ucopy=atoi((const char *)AccessLevelLabels[templong]);
        
        GT_GetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgForward_low], Message_Area_ManagerWnd, NULL, GTCY_Active, &templong, TAG_DONE);
            areanode->nn_Area.lforward=atoi((const char *)AccessLevelLabels[templong]);
        GT_GetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgForward_high], Message_Area_ManagerWnd, NULL, GTCY_Active, &templong, TAG_DONE);
            areanode->nn_Area.uforward=atoi((const char *)AccessLevelLabels[templong]);
        
        GT_GetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgReEdit_low], Message_Area_ManagerWnd, NULL, GTCY_Active, &templong, TAG_DONE);
            areanode->nn_Area.ledit=atoi((const char *)AccessLevelLabels[templong]);
        GT_GetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgReEdit_high], Message_Area_ManagerWnd, NULL, GTCY_Active, &templong, TAG_DONE);
            areanode->nn_Area.uedit=atoi((const char *)AccessLevelLabels[templong]);
        
        GT_GetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgSysOp_low], Message_Area_ManagerWnd, NULL, GTCY_Active, &templong, TAG_DONE);
            areanode->nn_Area.lsysop=atoi((const char *)AccessLevelLabels[templong]);
        GT_GetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgSysOp_high], Message_Area_ManagerWnd, NULL, GTCY_Active, &templong, TAG_DONE);
            areanode->nn_Area.usysop=atoi((const char *)AccessLevelLabels[templong]);
        
        /* Character sets */
        GT_GetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgSaveCharSet_cycle],Message_Area_ManagerWnd,NULL, GTCY_Active, &templong, TAG_DONE);
            areanode->nn_Area.forceset=UserCharSet_numbers[templong];



        // DO CHECKBOXES
        /* Hide SEEN-BY */
        GT_GetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgSeenBy_check],Message_Area_ManagerWnd,NULL,
                          GTCB_Checked,&templong, TAG_DONE);
        (areanode->nn_Area.Flag)|=HIDE_SEENBY;
        if(templong==TRUE) (areanode->nn_Area.Flag)^=HIDE_SEENBY;

        /* Handles */
        GT_GetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgHandles_check],Message_Area_ManagerWnd,NULL,
                          GTCB_Checked,&templong, TAG_DONE);
        (areanode->nn_Area.Flag)|=HANDLES_AREA;
        if(templong!=TRUE) (areanode->nn_Area.Flag)^=HANDLES_AREA;
        
        /* NetMail area */
        GT_GetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgNetmail_check],Message_Area_ManagerWnd,NULL,
                          GTCB_Checked,&templong, TAG_DONE);
        (areanode->nn_Area.Flag)|=NETMAIL_AREA;
        if(templong!=TRUE) (areanode->nn_Area.Flag)^=NETMAIL_AREA;
        
        /* UseNet newsgroup area */
        GT_GetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgUseNet_check],Message_Area_ManagerWnd,NULL,
                          GTCB_Checked,&templong, TAG_DONE);
        (areanode->nn_Area.Flag)|=NEWSGROUP_AREA;
        if(templong!=TRUE) (areanode->nn_Area.Flag)^=NEWSGROUP_AREA;
        
        /* Echomail area */
        GT_GetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgEchomail_check],Message_Area_ManagerWnd,NULL,
                          GTCB_Checked,&templong, TAG_DONE);
        (areanode->nn_Area.Flag)|=ECHO_AREA;
        if(templong!=TRUE) (areanode->nn_Area.Flag)^=ECHO_AREA;
        
        /* Signatures area */
        GT_GetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgSignatures_check],Message_Area_ManagerWnd,NULL,
                          GTCB_Checked,&templong, TAG_DONE);
        (areanode->nn_Area.Flag)|=SIGNATURE_AREA;
        if(templong!=TRUE) (areanode->nn_Area.Flag)^=SIGNATURE_AREA;


        // DO STRINGS
        /* Area name */
        GT_GetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgAreaName_string],Message_Area_ManagerWnd,NULL,
                          GTST_String,&templong, TAG_DONE);
        strcpy(areanode->nn_Area.Name,(char const *)templong);

        /* Path */
        GT_GetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgTranslator_string],Message_Area_ManagerWnd,NULL,
                          GTST_String,&templong, TAG_DONE);
        strcpy(areanode->nn_Area.path,(char const *)templong);

        /* Origin line */
        GT_GetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgCustomOrigin_string],Message_Area_ManagerWnd,NULL,
                          GTST_String,&templong, TAG_DONE);
        strcpy(areanode->nn_Area.origin,(char const *)templong);


        // DO INTEGERS

        /* Area number */
        GT_GetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgAreaNumber_integer],Message_Area_ManagerWnd,NULL,
                          GTIN_Number,&templong, TAG_DONE);
        areanode->nn_Area.Number=templong;

        /* Area capacity */
        GT_GetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgCapacity_integer],Message_Area_ManagerWnd,NULL,
                          GTIN_Number,&templong, TAG_DONE);
        areanode->nn_Area.Capacity=templong;

        /* Renumber at... */
        GT_GetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgRenumber_integer],Message_Area_ManagerWnd,NULL,
                          GTIN_Number,&templong, TAG_DONE);
        areanode->nn_Area.UpperLimit=templong;
    }
}


    // LISTVIEW

           /*  gadget "" clicked */
int MsgArea_listviewClicked( void ) 
{
    long SelectedArea=~0;
    char *charindex;
    struct DLGAreaNode *areanode;

    char i;

    GT_GetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgArea_listview], Message_Area_ManagerWnd, NULL,
                      GTLV_Selected, &SelectedArea, TAG_DONE);
    
    if(SelectedArea!=~0) {

        areanode=MsgAreaPtrs[SelectedArea];

        // DO CYCLES

        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgEnter_low], Message_Area_ManagerWnd, NULL, GA_Disabled,FALSE, GTCY_Active, AccessLevelIndex(areanode->nn_Area.llevel), TAG_DONE);
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgEnter_high], Message_Area_ManagerWnd, NULL, GA_Disabled,FALSE, GTCY_Active, AccessLevelIndex(areanode->nn_Area.ulevel), TAG_DONE);
        
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgWrite_low], Message_Area_ManagerWnd, NULL, GA_Disabled,FALSE, GTCY_Active, AccessLevelIndex(areanode->nn_Area.lwrite), TAG_DONE);
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgWrite_high], Message_Area_ManagerWnd, NULL, GA_Disabled,FALSE, GTCY_Active, AccessLevelIndex(areanode->nn_Area.uwrite), TAG_DONE);
        
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgKill_low], Message_Area_ManagerWnd, NULL, GA_Disabled,FALSE, GTCY_Active, AccessLevelIndex(areanode->nn_Area.lkill), TAG_DONE);
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgKill_high], Message_Area_ManagerWnd, NULL, GA_Disabled,FALSE, GTCY_Active, AccessLevelIndex(areanode->nn_Area.ukill), TAG_DONE);
        
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgHurl_low], Message_Area_ManagerWnd, NULL, GA_Disabled,FALSE, GTCY_Active, AccessLevelIndex(areanode->nn_Area.lcopy), TAG_DONE);
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgHurl_high], Message_Area_ManagerWnd, NULL, GA_Disabled,FALSE, GTCY_Active, AccessLevelIndex(areanode->nn_Area.ucopy), TAG_DONE);
        
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgForward_low], Message_Area_ManagerWnd, NULL, GA_Disabled,FALSE, GTCY_Active, AccessLevelIndex(areanode->nn_Area.lforward), TAG_DONE);
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgForward_high], Message_Area_ManagerWnd, NULL, GA_Disabled,FALSE, GTCY_Active, AccessLevelIndex(areanode->nn_Area.uforward), TAG_DONE);
        
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgReEdit_low], Message_Area_ManagerWnd, NULL, GA_Disabled,FALSE, GTCY_Active, AccessLevelIndex(areanode->nn_Area.ledit), TAG_DONE);
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgReEdit_high], Message_Area_ManagerWnd, NULL, GA_Disabled,FALSE, GTCY_Active, AccessLevelIndex(areanode->nn_Area.uedit), TAG_DONE);
        
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgSysOp_low], Message_Area_ManagerWnd, NULL, GA_Disabled,FALSE, GTCY_Active, AccessLevelIndex(areanode->nn_Area.lsysop), TAG_DONE);
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgSysOp_high], Message_Area_ManagerWnd, NULL, GA_Disabled,FALSE, GTCY_Active, AccessLevelIndex(areanode->nn_Area.usysop), TAG_DONE);
        
        if (charindex=strchr(UserCharSet_numbers,areanode->nn_Area.savecharset)) i=(ULONG)charindex-(ULONG)UserCharSet_numbers; else i=0;
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgSaveCharSet_cycle],Message_Area_ManagerWnd,NULL, GA_Disabled,FALSE, GTCY_Active, i, TAG_DONE);


        // DO CHECKBOXES

        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgSeenBy_check],Message_Area_ManagerWnd,NULL, GA_Disabled,FALSE, GTCB_Checked, (areanode->nn_Area.Flag&HIDE_SEENBY ? FALSE : TRUE), TAG_DONE);
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgHandles_check],Message_Area_ManagerWnd,NULL, GA_Disabled,FALSE, GTCB_Checked, (areanode->nn_Area.Flag&HANDLES_AREA ? TRUE : FALSE), TAG_DONE);
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgNetmail_check],Message_Area_ManagerWnd,NULL, GA_Disabled,FALSE, GTCB_Checked, (areanode->nn_Area.Flag&NETMAIL_AREA ? TRUE : FALSE), TAG_DONE);
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgUseNet_check],Message_Area_ManagerWnd,NULL, GA_Disabled,FALSE, GTCB_Checked, (areanode->nn_Area.Flag&NEWSGROUP_AREA ? TRUE : FALSE), TAG_DONE);
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgEchomail_check],Message_Area_ManagerWnd,NULL, GA_Disabled,FALSE, GTCB_Checked, (areanode->nn_Area.Flag&ECHO_AREA ? TRUE : FALSE), TAG_DONE);
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgSignatures_check],Message_Area_ManagerWnd,NULL, GA_Disabled,FALSE, GTCB_Checked, (areanode->nn_Area.Flag&SIGNATURE_AREA ? TRUE : FALSE), TAG_DONE);

        // DO STRINGS

        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgAreaName_string],Message_Area_ManagerWnd,NULL, GA_Disabled,FALSE, GTST_String,areanode->nn_Area.Name, TAG_DONE);
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgTranslator_string],Message_Area_ManagerWnd,NULL, GA_Disabled,FALSE, GTST_String,areanode->nn_Area.path, TAG_DONE);
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgCustomOrigin_string],Message_Area_ManagerWnd,NULL, GA_Disabled,FALSE, GTST_String,areanode->nn_Area.origin, TAG_DONE);

        // DO INTEGERS

        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgAreaNumber_integer],Message_Area_ManagerWnd,NULL, GA_Disabled,FALSE, GTIN_Number,areanode->nn_Area.Number, TAG_DONE);
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgCapacity_integer],Message_Area_ManagerWnd,NULL, GA_Disabled,FALSE, GTIN_Number,areanode->nn_Area.Capacity, TAG_DONE);
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgRenumber_integer],Message_Area_ManagerWnd,NULL, GA_Disabled,FALSE, GTIN_Number,areanode->nn_Area.UpperLimit, TAG_DONE);

        // DO SLIDERS

        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgCapacity_slider],Message_Area_ManagerWnd,NULL, GA_Disabled,FALSE, GTSL_Level,areanode->nn_Area.Capacity, TAG_DONE);
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgRenumber_slider],Message_Area_ManagerWnd,NULL, GA_Disabled,FALSE, GTSL_Level,areanode->nn_Area.UpperLimit, TAG_DONE);


        // DO BUTTONS

        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgAreaUsers_button],Message_Area_ManagerWnd,NULL, GA_Disabled,TRUE, TAG_DONE);
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgAreaUp_button],Message_Area_ManagerWnd,NULL, GA_Disabled,FALSE, TAG_DONE);
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgAreaDown_button],Message_Area_ManagerWnd,NULL, GA_Disabled,FALSE, TAG_DONE);
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_RemoveMsgArea_button],Message_Area_ManagerWnd,NULL, GA_Disabled,FALSE, TAG_DONE);
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_InsertMsgArea_button],Message_Area_ManagerWnd,NULL, GA_Disabled,FALSE, TAG_DONE);
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgImportAreas_button],Message_Area_ManagerWnd,NULL, GA_Disabled,FALSE, TAG_DONE);
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgAreaSave_button],Message_Area_ManagerWnd,NULL, GA_Disabled,FALSE, TAG_DONE);
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgApplyTemplate_button],Message_Area_ManagerWnd,NULL, GA_Disabled,TRUE, TAG_DONE);

    }
    return(TRUE);
}


/*----------+
 |      //  *
 |  \\ //   *
 |   \X/    *
 +***********/

int MsgSeenBy_checkClicked( void ) { 
    StoreMsgGadgetStates();
    return(TRUE); 
}
int MsgHandles_checkClicked( void ) { 
    StoreMsgGadgetStates();
    return(TRUE); 
}
int MsgSignatures_checkClicked( void ) { 
    StoreMsgGadgetStates();
    return(TRUE); 
}
int MsgEchomail_checkClicked( void ) { 
    StoreMsgGadgetStates();
    return(TRUE); 
}
int MsgNetmail_checkClicked( void ) { 
    StoreMsgGadgetStates();
    return(TRUE); 
}
int MsgUseNet_checkClicked( void ) { 
    StoreMsgGadgetStates();
    return(TRUE); 
}

    // SLIDERS

         /*  gadget "" clicked */
int MsgCapacity_sliderClicked( void ) 
{
    long mygadlevel=0;

    GT_GetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgCapacity_slider],   Message_Area_ManagerWnd, NULL,
                      GTSL_Level, &mygadlevel,
                      TAG_DONE);
    GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgCapacity_integer], Message_Area_ManagerWnd, NULL,
                      GTIN_Number, mygadlevel,
                      TAG_DONE);
    StoreMsgGadgetStates();
    return(TRUE);
}

         /*  gadget "" clicked */
int MsgRenumber_sliderClicked( void ) 
{
    long mygadlevel=0;
    
    GT_GetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgRenumber_slider],   Message_Area_ManagerWnd, NULL,
                      GTSL_Level, &mygadlevel,
                      TAG_DONE);
    GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgRenumber_integer], Message_Area_ManagerWnd, NULL,
                      GTIN_Number, mygadlevel,
                      TAG_DONE);
    StoreMsgGadgetStates();
    return(TRUE);
}


/*-------------------+
 |******************+*
 |* INTEGER GADGETS |*
 |+-----------------+*
 +********************/

int MsgAreaNumber_integerClicked( void ) { 

    long SelectedArea=~0;
    struct DLGAreaNode *areanode;
    struct DLGAreaNode *nextareanode;
    struct DLGAreaNode *currentareanode;
    int i; 
    BOOL sortswapped;
    SHORT newareanum, lastareanum, tempareanum;
    ULONG templong;

    GT_GetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgArea_listview], Message_Area_ManagerWnd, NULL,
                      GTLV_Selected, &SelectedArea, TAG_DONE);


    GT_GetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgAreaNumber_integer],Message_Area_ManagerWnd,NULL, GTIN_Number,&templong, TAG_DONE);
    newareanum=templong;

    // Get last area node pointer and check for duplicate area number
    for (i=0; MsgAreaPtrs[i]; i++) {
        areanode=MsgAreaPtrs[SelectedArea];
        if ((areanode->nn_Area.Number)==newareanum) newareanum=10000;
    }
    lastareanum=i-1;

    areanode=MsgAreaPtrs[SelectedArea];
    currentareanode=areanode;

    if((SelectedArea!=~0) && (newareanum!=10000)) {
        
        // Detach list before modifying it
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgArea_listview],Message_Area_ManagerWnd, NULL, GTLV_Labels, ~0, TAG_DONE);

        // Change area number
        areanode->nn_Area.Number=newareanum;
        sprintf((UBYTE *)&(areanode->nn_ListName), "%4d: %s", areanode->nn_Area.Number, areanode->nn_Area.Name);
        
        // Sort list of nodes
        do {
            sortswapped=FALSE;
            for (i=0; i<lastareanum-1; i++) {
                areanode=MsgAreaPtrs[i];
                nextareanode=MsgAreaPtrs[i+1];
                if ((areanode->nn_Area.Number) > (nextareanode->nn_Area.Number)) {
                    sortswapped=TRUE;
                    Remove((struct Node *)areanode);
                    Insert((struct List *)MsgAreaList_list,(struct Node *)areanode,(struct Node *)nextareanode);
                    MsgAreaPtrs[i]=nextareanode;
                    MsgAreaPtrs[i+1]=areanode;
                }
            }
        } while(sortswapped==TRUE);
    
        for (i=0; MsgAreaPtrs[i]!=currentareanode; i++);

        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgArea_listview],Message_Area_ManagerWnd, NULL,
                          GTLV_Labels, (ULONG)MsgAreaList_list,
                          GTLV_MakeVisible, i,
                          GTLV_Selected, i,
                          TAG_DONE);
                          
        MsgArea_listviewClicked();
    } else {
        DisplayBeep(NULL);
        EasyRequest(NULL, &AreaExistsReq, NULL, NULL);
    }

    return(TRUE); 
}

        /*  gadget "Capacity:" clicked */
int MsgCapacity_integerClicked( void ) 
{
    long mygadlevel=0;

    GT_GetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgCapacity_integer], Message_Area_ManagerWnd, NULL,
                      GTIN_Number, &mygadlevel,
                      TAG_DONE);
    GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgCapacity_slider],   Message_Area_ManagerWnd, NULL,
                      GTSL_Level, mygadlevel,
                      TAG_DONE);
    StoreMsgGadgetStates();
    return(TRUE);
}

        /*  gadget "Renumber:" clicked */
int MsgRenumber_integerClicked( void ) 
{
    long mygadlevel=0;

    GT_GetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgRenumber_integer], Message_Area_ManagerWnd, NULL,
                      GTIN_Number, &mygadlevel,
                      TAG_DONE);
    GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgRenumber_slider],   Message_Area_ManagerWnd, NULL,
                      GTSL_Level, mygadlevel,
                      TAG_DONE);
    StoreMsgGadgetStates();
    return(TRUE);
}


/*------++---------------+
 | .--. *|               *
 | |  v *| CYCLE GADGETS *
 | `-   *|               *
 +*******+****************/

int MsgSaveCharSet_cycleClicked( void ) { 
    StoreMsgGadgetStates();
    return(TRUE); 
}
int MsgEnter_lowClicked( void ) { 
    StoreMsgGadgetStates();
    return(TRUE); 
}
int MsgEnter_highClicked( void ) { 
    StoreMsgGadgetStates();
    return(TRUE); 
}
int MsgWrite_lowClicked( void ) { 
    StoreMsgGadgetStates();
    return(TRUE); 
}
int MsgWrite_highClicked( void ) { 
    StoreMsgGadgetStates();
    return(TRUE); 
}
int MsgKill_lowClicked( void ) { 
    StoreMsgGadgetStates();
    return(TRUE); 
}
int MsgKill_highClicked( void ) { 
    StoreMsgGadgetStates();
    return(TRUE); 
}
int MsgForward_lowClicked( void ) { 
    StoreMsgGadgetStates();
    return(TRUE); 
}
int MsgForward_highClicked( void ) { 
    StoreMsgGadgetStates();
    return(TRUE); 
}
int MsgHurl_lowClicked( void ) { 
    StoreMsgGadgetStates();
    return(TRUE); 
}
int MsgHurl_highClicked( void ) { 
    StoreMsgGadgetStates();
    return(TRUE); 
}
int MsgReEdit_lowClicked( void ) { 
    StoreMsgGadgetStates();
    return(TRUE); 
}
int MsgReEdit_highClicked( void ) { 
    StoreMsgGadgetStates();
    return(TRUE); 
}
int MsgSysOp_lowClicked( void ) { 
    StoreMsgGadgetStates();
    return(TRUE); 
}
int MsgSysOp_highClicked( void ) { 
    StoreMsgGadgetStates();
    return(TRUE); 
}


/*------------------+
 |*****************+*
 |* STRING GADGETS |*
 |+----------------+*
 +*******************/

int MsgAreaName_stringClicked( void ) {

    long SelectedArea=~0;
    ULONG namestring;
    struct DLGAreaNode *areanode;

    GT_GetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgArea_listview], Message_Area_ManagerWnd, NULL,
                      GTLV_Selected, &SelectedArea, TAG_DONE);

    if(SelectedArea!=~0) {

        // Detach list before modifying it
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgArea_listview],Message_Area_ManagerWnd, NULL,
                          GTLV_Labels, ~0, TAG_DONE);

        areanode=MsgAreaPtrs[SelectedArea];

        GT_GetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgAreaName_string],Message_Area_ManagerWnd, NULL,
                          GTST_String,&namestring, TAG_DONE);
        strcpy(areanode->nn_Area.Name,(char const *)namestring);
        sprintf((UBYTE *)&(areanode->nn_ListName), "%4d: %s", areanode->nn_Area.Number, areanode->nn_Area.Name);

        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgArea_listview],Message_Area_ManagerWnd, NULL,
                          GTLV_Labels,(ULONG)MsgAreaList_list,
                          GTLV_MakeVisible, SelectedArea,
                          GTLV_Selected, SelectedArea,
                          TAG_DONE);
    }
    return(TRUE); 
}

int MsgTranslator_stringClicked( void ) { 
    StoreMsgGadgetStates();
    return(TRUE); 
}
int MsgCustomOrigin_stringClicked( void ) { 
    StoreMsgGadgetStates();
    return(TRUE); 
}


/*------------+
 |            *
 |    SAVE    *
 |            *
 +*************/

int MsgAreaSave_buttonClicked( void ) {         /* "SAVE" is clicked. */
    StoreMsgGadgetStates();
    SaveMsgAreaList();
    return(TRUE);                                  
}

/*------------+
 |            *
 |    USERS   *
 |            *
 +*************/

int MsgAreaUsers_buttonClicked( void ) {
    return(TRUE);
}

/*------------+
 |            *
 |   REMOVE   *
 |            *
 +*************/

int RemoveMsgArea_buttonClicked( void ) 
{
    long SelectedArea=~0;
    struct DLGAreaNode *areanode;
    int i;

    GT_GetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgArea_listview], Message_Area_ManagerWnd, NULL,
                      GTLV_Selected, &SelectedArea, TAG_DONE);

    if(SelectedArea!=~0) {

        // Detach list before modifying it
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgArea_listview],Message_Area_ManagerWnd, NULL, GTLV_Labels, ~0, TAG_DONE);

        areanode=MsgAreaPtrs[SelectedArea];

        Remove((struct Node *)areanode);

        for (i=SelectedArea; MsgAreaPtrs[i+1]!=NULL; i++) MsgAreaPtrs[i]=MsgAreaPtrs[i+1];
        MsgAreaPtrs[i]=NULL;
        if(SelectedArea==i) SelectedArea--;

        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgArea_listview],Message_Area_ManagerWnd, NULL,
                          GTLV_Labels, (ULONG)MsgAreaList_list,
                          GTLV_MakeVisible, SelectedArea,
                          GTLV_Selected, SelectedArea,
                          TAG_DONE);
                          
        MsgArea_listviewClicked();

        MsgActionQ(KILLAREA,areanode->nn_Area.Number,0);

    }
    return(TRUE);
}

/*------------+
 |            *
 |   INSERT   *
 |            *
 +*************/

int InsertMsgArea_buttonClicked( void ) {      /* "Insert" clicked */
    
    long SelectedArea=~0;
    struct DLGAreaNode *areanode;
    struct DLGAreaNode *nextareanode;
    struct DLGAreaNode *newareanode;
    int i;
    SHORT lastareanum;

    GT_GetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgArea_listview], Message_Area_ManagerWnd, NULL,
                      GTLV_Selected, &SelectedArea, TAG_DONE);

    if(SelectedArea!=~0) {

        for (i=0; MsgAreaPtrs[i]; i++);
        lastareanum=i-1;

        areanode=MsgAreaPtrs[SelectedArea];
        nextareanode=MsgAreaPtrs[SelectedArea+1];

        if ((nextareanode->nn_Area.Number)!=(areanode->nn_Area.Number+1) || (nextareanode==NULL) && (areanode->nn_Area.Number!=9999)) {
        
            // Detach list before modifying it
            GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgArea_listview],Message_Area_ManagerWnd, NULL, GTLV_Labels, ~0, TAG_DONE);

            // Allocate new node...
            newareanode=AllocPooled(MsgAreaList_pool,sizeof(struct DLGAreaNode));
            
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
            Insert((struct List *)MsgAreaList_list,(struct Node *)newareanode,(struct Node *)areanode);

            // Insert new node pointer
            for (i=lastareanum+1; i>SelectedArea; i--) MsgAreaPtrs[i+1]=MsgAreaPtrs[i];
            MsgAreaPtrs[i+1]=newareanode;

            GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgArea_listview],Message_Area_ManagerWnd, NULL,
                              GTLV_Labels, (ULONG)MsgAreaList_list,
                              GTLV_MakeVisible, i+1,
                              GTLV_Selected, i+1,
                              TAG_DONE);
            MsgArea_listviewClicked();

            MsgActionQ(MAKEAREA,newareanode->nn_Area.Number,0);
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

int MsgAreaUp_buttonClicked( void ) 
{
    long SelectedArea=~0;
    struct DLGAreaNode *areanode;
    struct DLGAreaNode *prevareanode;
    SHORT tempareanum;
    void *sel_temp;

    GT_GetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgArea_listview], Message_Area_ManagerWnd, NULL,
                      GTLV_Selected, &SelectedArea, TAG_DONE);

    if(SelectedArea!=~0 && SelectedArea!=0) {

        // Detach list before modifying it
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgArea_listview],Message_Area_ManagerWnd, NULL, GTLV_Labels, ~0, TAG_DONE);

        areanode=MsgAreaPtrs[SelectedArea];
        prevareanode=MsgAreaPtrs[SelectedArea-1];

        MsgActionQ(SWAPAREA,areanode->nn_Area.Number,prevareanode->nn_Area.Number);

        Remove((struct Node *)prevareanode);
        Insert((struct List *)MsgAreaList_list,(struct Node *)prevareanode,(struct Node *)areanode);

        tempareanum=areanode->nn_Area.Number;
        areanode->nn_Area.Number=prevareanode->nn_Area.Number;
        prevareanode->nn_Area.Number=tempareanum;
        sprintf((UBYTE *)&(areanode->nn_ListName), "%4d: %s", areanode->nn_Area.Number, areanode->nn_Area.Name);
        sprintf((UBYTE *)&(prevareanode->nn_ListName), "%4d: %s", prevareanode->nn_Area.Number, prevareanode->nn_Area.Name);

        sel_temp=MsgAreaPtrs[SelectedArea];
        MsgAreaPtrs[SelectedArea]=MsgAreaPtrs[SelectedArea-1];
        MsgAreaPtrs[SelectedArea-1]=sel_temp;

        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgArea_listview],Message_Area_ManagerWnd, NULL,
                          GTLV_Labels, (ULONG)MsgAreaList_list,
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

int MsgAreaDown_buttonClicked( void ) 
{
    long SelectedArea=~0;
    struct DLGAreaNode *areanode;
    struct DLGAreaNode *nextareanode;
    SHORT tempareanum;
    void *sel_temp;

    GT_GetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgArea_listview], Message_Area_ManagerWnd, NULL,
                      GTLV_Selected, &SelectedArea, TAG_DONE);

    if(SelectedArea!=~0 && MsgAreaPtrs[SelectedArea+1]!=0) {

        // Detach list before modifying it
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgArea_listview],Message_Area_ManagerWnd, NULL, GTLV_Labels, ~0, TAG_DONE);

        areanode=MsgAreaPtrs[SelectedArea];
        nextareanode=MsgAreaPtrs[SelectedArea+1];

        MsgActionQ(SWAPAREA,areanode->nn_Area.Number,nextareanode->nn_Area.Number);

        Remove((struct Node *)areanode);
        Insert((struct List *)MsgAreaList_list,(struct Node *)areanode,(struct Node *)nextareanode);

        tempareanum=areanode->nn_Area.Number;
        areanode->nn_Area.Number=nextareanode->nn_Area.Number;
        nextareanode->nn_Area.Number=tempareanum;
        sprintf((UBYTE *)&(areanode->nn_ListName), "%4d: %s", areanode->nn_Area.Number, areanode->nn_Area.Name);
        sprintf((UBYTE *)&(nextareanode->nn_ListName), "%4d: %s", nextareanode->nn_Area.Number, nextareanode->nn_Area.Name);

        sel_temp=MsgAreaPtrs[SelectedArea];
        MsgAreaPtrs[SelectedArea]=MsgAreaPtrs[SelectedArea+1];
        MsgAreaPtrs[SelectedArea+1]=sel_temp;


        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgArea_listview],Message_Area_ManagerWnd, NULL,
                          GTLV_Labels, (ULONG)MsgAreaList_list,
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

int MsgImportAreas_buttonClicked( void ) {

    struct FileRequester *myfilerequester;
    char *ourfile, *ourdrawer;
    char fullpath[250];
    char *sptr;
    FILE *importfile;
    ULONG templong;

    long SelectedArea=~0;
    struct DLGAreaNode *areanode;
    struct DLGAreaNode *tempareanode;
    struct DLGAreaNode *newareanode;
    int i;
    SHORT lastareanum,newareanum,lastmsgarea;
    char *tempbuff=fullpath;
    BOOL areaplaced;

    char importreqtitle[]="Select Message Areas Import File";

    struct TagItem myfrtags[3];
    
    myfrtags[0].ti_Tag =ASLFR_TitleText;
    myfrtags[0].ti_Data=(ULONG)importreqtitle;
    myfrtags[1].ti_Tag =ASLFR_RejectIcons;
    myfrtags[1].ti_Data=TRUE;
    myfrtags[2].ti_Tag =TAG_DONE;

    myfilerequester=AllocAslRequest(ASL_FileRequest,myfrtags);

    GT_GetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgArea_listview], Message_Area_ManagerWnd, NULL,
                      GTLV_Selected, &SelectedArea, TAG_DONE);

    for (i=0; MsgAreaPtrs[i]; i++); lastareanum=i-1;

    tempareanode=MsgAreaPtrs[SelectedArea];
    lastmsgarea=tempareanode->nn_Area.Number;

    if (AslRequest(myfilerequester,NULL)) {

        ourfile=myfilerequester->fr_File;
        ourdrawer=myfilerequester->fr_Drawer;
        sprintf(fullpath,"%s%s",ourdrawer,ourfile);
        if(importfile=fopen(fullpath,"rt")) {
            do {

                // Detach list before modifying it
                GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgArea_listview],Message_Area_ManagerWnd, NULL, GTLV_Labels, ~0, TAG_DONE);
                
                // Allocate new area node...
                newareanode=AllocPooled(MsgAreaList_pool,sizeof(struct DLGAreaNode));

                // Define the new number, name and path fields...
                memcpy(&(newareanode->nn_Area), (void *)&(tempareanode->nn_Area), sizeof(struct Msg_Area));
                
                // Area number
                memset(tempbuff,0,sizeof(fullpath));
                fgets(tempbuff, 250, importfile);
                newareanum=atoi(tempbuff);

                if (newareanum>0 && newareanum<10000) {
                    newareanode->nn_Area.Number=newareanum;
                    lastmsgarea=newareanum+1;
                } else {
                    newareanode->nn_Area.Number=lastmsgarea;
                    newareanum=lastmsgarea++;
                }

                for (i=0; i<=lastareanum; i++) {
                    areanode=MsgAreaPtrs[i];
                    if (newareanum==areanode->nn_Area.Number) {
                        DisplayBeep(NULL);
                        EasyRequest(NULL, &ImportAreaExistsReq, NULL, newareanum);
                        FreePooled(MsgAreaList_pool,newareanode,sizeof(struct DLGAreaNode));
                        break;
                    }
                }
                
                if (newareanum!=areanode->nn_Area.Number) {

                    // Area name
                    memset(tempbuff,0,sizeof(fullpath));
                    fgets(tempbuff,250,importfile);
                    for (sptr=tempbuff; sptr<(tempbuff+sizeof(newareanode->nn_Area.Name)); sptr++) if (*sptr==10) *sptr=0;
                    *sptr=0;
                    strcpy(newareanode->nn_Area.Name,tempbuff);

                    // Newsgroup if a UseNet area
                    GT_GetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgUseNet_check],Message_Area_ManagerWnd,NULL,
                                      GTCB_Checked,&templong, TAG_DONE);
                    if (areanode->nn_Area.Flag & NEWSGROUP_AREA) {
                        memset(tempbuff,0,sizeof(fullpath));
                        fgets(tempbuff, 250, importfile);
                        for (sptr=tempbuff; sptr<(tempbuff+sizeof(newareanode->nn_Area.origin)); sptr++) if (*sptr==10) *sptr=0;
                        *sptr=0;
                        strcpy(newareanode->nn_Area.origin,tempbuff);
                    }

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
                        areanode=MsgAreaPtrs[i];
                        MsgAreaPtrs[i+1]=MsgAreaPtrs[i];
                        if (newareanode->nn_Area.Number > areanode->nn_Area.Number) {
                            Insert((struct List *)MsgAreaList_list,(struct Node *)newareanode,(struct Node *)areanode);
                            MsgAreaPtrs[i+1]=newareanode;
                            i+=1;
                            areaplaced=TRUE;
                        }
                        if (areaplaced==TRUE) break;
                        if (i==0 && (newareanode->nn_Area.Number != areanode->nn_Area.Number)) {
                            AddHead((struct List *)MsgAreaList_list,(struct Node *)newareanode);
                            MsgAreaPtrs[i]=newareanode;
                        }
                    }
                    MsgActionQ(MAKEAREA,newareanode->nn_Area.Number,0);

                    GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgArea_listview],Message_Area_ManagerWnd, NULL,
                                      GTLV_Labels, (ULONG)MsgAreaList_list,
                                      GTLV_MakeVisible, i,
                                      GTLV_Selected, i,
                                      TAG_DONE);
                }
            } while(!feof(importfile));
            fclose(importfile);
            MsgArea_listviewClicked();
        }
        else {
            DisplayBeep(NULL);   /* Flash all screens */
            EasyRequest(NULL, &BadImportFileReq, NULL, fullpath);
        }
    }
    return(TRUE);
}

/*------------+
 |            *
 |  TEMPLATE  *
 |            *
 +*************/

int MsgApplyTemplate_buttonClicked( void )
{
    long SelectedArea=0;

    if (!OpenMsgAreaTemplateWindow()) {

        SetWindowPointer(Message_Area_ManagerWnd,
                         WA_BusyPointer, TRUE,
                         TAG_DONE);

        msgtemp_mask=(1 << MsgAreaTemplateWnd->UserPort->mp_SigBit);
        all_mask |= msgtemp_mask;
        active_wnd |= MSGTemp;
    }
    return(TRUE);
}



    // RAW KEY PROCESSOR

int Message_Area_ManagerRawKey(void) 
{
    long SelectedArea=~0;

    switch(Message_Area_ManagerMsg.Code) {

        case 76:        // Cursor up

            GT_GetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgArea_listview], Message_Area_ManagerWnd, NULL,
                              GTLV_Selected, &SelectedArea, TAG_DONE);
            if(SelectedArea!=~0 && SelectedArea!=0) 
                GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgArea_listview], Message_Area_ManagerWnd, NULL,
                                  GTLV_MakeVisible, SelectedArea-1, 
                                  GTLV_Selected, SelectedArea-1, TAG_DONE);
            MsgArea_listviewClicked();
            break;

        case 77:        // Cursor Down
            
            GT_GetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgArea_listview], Message_Area_ManagerWnd, NULL,
                              GTLV_Selected, &SelectedArea, TAG_DONE);
            if(SelectedArea!=~0 && MsgAreaPtrs[SelectedArea+1]!=0) 
                GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgArea_listview], Message_Area_ManagerWnd, NULL,
                                  GTLV_MakeVisible, SelectedArea+1, 
                                  GTLV_Selected, SelectedArea+1, TAG_DONE);
            MsgArea_listviewClicked();
            break;

    }
    return(TRUE);
}
    

/*--------+
 |  ****  *
 |  *  |  *
 |  *--+  *
 +*********/

int Message_Area_ManagerCloseWindow( void ) {   /* "IDCMP_CLOSEWINDOW". */
    StoreMsgGadgetStates();
    return(0);
}

int MsgTemplateSrc_listviewClicked( void ) {   /* routine when gadget "" is clicked. */
    return(TRUE);
}
int MsgTemplateDest_listviewClicked( void ) {  /* routine when gadget "" is clicked. */
    return(TRUE);
}
int MsgTemplateApply_buttonClicked( void ) {   /* routine when gadget "APPLY" is clicked. */
    return(TRUE);
}
int MsgTemplateCancel_buttonClicked( void ) {  /* routine when gadget "Cancel" is clicked. */
    return(TRUE);
}
int MsgTemplateConfirmApply_checkClicked( void ) { /* routine when gadget "Confirm each application of template area" is clicked. */
    return(TRUE);
}
int MsgAreaTemplateCloseWindow( void ) {       /* routine for "IDCMP_CLOSEWINDOW". */
    SetWindowPointer(Message_Area_ManagerWnd,
                     TAG_DONE);
    return(0);
}


