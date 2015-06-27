#include "DLG-Edit.h"

/***
 *** Main Status Window
 ***/

/* "About" 
*/
int Main_Status_DisplayItem0( void ) {
    DisableWindows(ALLWindows);
    EasyRequest(NULL, &AboutReq, NULL, myVersionTag+7);
    EnableWindows(ALLWindows);
    return(TRUE);
}

/* "Quit" 
*/
int Main_Status_DisplayItem1( void ) {         
    return(0);
}



/*  "File Area Manager" clicked 
*/
int FileAreaSelect_buttonClicked( void ) 
{
    if (!OpenFile_Area_ManagerWindow()) {

        GT_SetGadgetAttrs(Main_Status_DisplayGadgets[GD_FileAreaSelect_button],Main_Status_DisplayWnd,NULL, GA_Disabled,TRUE, TAG_DONE);

        fileman_mask=(1 << File_Area_ManagerWnd->UserPort->mp_SigBit);
        all_mask |= fileman_mask;
        active_wnd |= FILEWnd;

        // Set listview to area names
        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileArea_listview],      
                          File_Area_ManagerWnd, NULL, GTLV_Labels, &FileAreaList_list, TAG_DONE);
        // Set cycle gadgets to access levels
        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileEnter_low],          
                          File_Area_ManagerWnd, NULL, GTCY_Labels, (ULONG)AccessLevelLabels, TAG_DONE);
        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileEnter_high],         
                          File_Area_ManagerWnd, NULL, GTCY_Labels, (ULONG)AccessLevelLabels, TAG_DONE);
        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileKill_low],           
                          File_Area_ManagerWnd, NULL, GTCY_Labels, (ULONG)AccessLevelLabels, TAG_DONE);
        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileKill_high],          
                          File_Area_ManagerWnd, NULL, GTCY_Labels, (ULONG)AccessLevelLabels, TAG_DONE);
        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileHurl_low],           
                          File_Area_ManagerWnd, NULL, GTCY_Labels, (ULONG)AccessLevelLabels, TAG_DONE);
        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileHurl_high],          
                          File_Area_ManagerWnd, NULL, GTCY_Labels, (ULONG)AccessLevelLabels, TAG_DONE);
        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileUpload_low],         
                          File_Area_ManagerWnd, NULL, GTCY_Labels, (ULONG)AccessLevelLabels, TAG_DONE);
        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileUpload_high],        
                          File_Area_ManagerWnd, NULL, GTCY_Labels, (ULONG)AccessLevelLabels, TAG_DONE);
        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileDownload_low],         
                          File_Area_ManagerWnd, NULL, GTCY_Labels, (ULONG)AccessLevelLabels, TAG_DONE);
        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileDownload_high],        
                          File_Area_ManagerWnd, NULL, GTCY_Labels, (ULONG)AccessLevelLabels, TAG_DONE);
        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileSysOp_low],          
                          File_Area_ManagerWnd, NULL, GTCY_Labels, (ULONG)AccessLevelLabels, TAG_DONE);
        GT_SetGadgetAttrs(File_Area_ManagerGadgets[GD_FileSysOp_high],         
                          File_Area_ManagerWnd, NULL, GTCY_Labels, (ULONG)AccessLevelLabels, TAG_DONE);

    }
    return(TRUE);
}

/*  "Message Area Manager" clicked 
*/
int MessageAreaSelect_buttonClicked( void ) 
{
    if (!OpenMessage_Area_ManagerWindow()) {

        GT_SetGadgetAttrs(Main_Status_DisplayGadgets[GD_MessageAreaSelect_button],Main_Status_DisplayWnd,NULL, GA_Disabled,TRUE, TAG_DONE);

        msgman_mask=(1 << Message_Area_ManagerWnd->UserPort->mp_SigBit);
        all_mask |= msgman_mask;
        active_wnd |= MSGWnd;

        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgArea_listview], Message_Area_ManagerWnd, NULL,
                          GTLV_Labels, &MsgAreaList_list, TAG_DONE);
        
        // Set cycle gadgets to access levels
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgEnter_low],    
                          Message_Area_ManagerWnd, NULL, GTCY_Labels, (ULONG)AccessLevelLabels, TAG_DONE);
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgEnter_high],   
                          Message_Area_ManagerWnd, NULL, GTCY_Labels, (ULONG)AccessLevelLabels, TAG_DONE);
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgWrite_low],    
                          Message_Area_ManagerWnd, NULL, GTCY_Labels, (ULONG)AccessLevelLabels, TAG_DONE);
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgWrite_high],   
                          Message_Area_ManagerWnd, NULL, GTCY_Labels, (ULONG)AccessLevelLabels, TAG_DONE);
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgKill_low],     
                          Message_Area_ManagerWnd, NULL, GTCY_Labels, (ULONG)AccessLevelLabels, TAG_DONE);
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgKill_high],    
                          Message_Area_ManagerWnd, NULL, GTCY_Labels, (ULONG)AccessLevelLabels, TAG_DONE);
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgHurl_low],    
                          Message_Area_ManagerWnd, NULL, GTCY_Labels, (ULONG)AccessLevelLabels, TAG_DONE);
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgHurl_high],    
                          Message_Area_ManagerWnd, NULL, GTCY_Labels, (ULONG)AccessLevelLabels, TAG_DONE);
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgForward_low],  
                          Message_Area_ManagerWnd, NULL, GTCY_Labels, (ULONG)AccessLevelLabels, TAG_DONE);
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgForward_high], 
                          Message_Area_ManagerWnd, NULL, GTCY_Labels, (ULONG)AccessLevelLabels, TAG_DONE);
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgReEdit_low],   
                          Message_Area_ManagerWnd, NULL, GTCY_Labels, (ULONG)AccessLevelLabels, TAG_DONE);
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgReEdit_high],  
                          Message_Area_ManagerWnd, NULL, GTCY_Labels, (ULONG)AccessLevelLabels, TAG_DONE);
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgSysOp_low],    
                          Message_Area_ManagerWnd, NULL, GTCY_Labels, (ULONG)AccessLevelLabels, TAG_DONE);
        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgSysOp_high],   
                          Message_Area_ManagerWnd, NULL, GTCY_Labels, (ULONG)AccessLevelLabels, TAG_DONE);

        GT_SetGadgetAttrs(Message_Area_ManagerGadgets[GD_MsgSaveCharSet_cycle],  
                          Message_Area_ManagerWnd, NULL, GTCY_Labels, (ULONG)UserCharSet_options, TAG_DONE);
    }
    return(TRUE);
}

/*  "User Editor" clicked 
*/
int UserEditorSelect_buttonClicked( void ) 
{
    if (!OpenUser_EditorWindow()) {

        GT_SetGadgetAttrs(Main_Status_DisplayGadgets[GD_UserEditorSelect_button],Main_Status_DisplayWnd,NULL, GA_Disabled,TRUE, TAG_DONE);

        usered_mask=(1 << User_EditorWnd->UserPort->mp_SigBit);
        all_mask |= usered_mask;
        active_wnd |= USERWnd;

        GT_SetGadgetAttrs(User_EditorGadgets[GD_UserTerminal_cycle], User_EditorWnd,NULL, 
                          GTCY_Labels, (ULONG)UserTerminal_options, TAG_DONE);
        GT_SetGadgetAttrs(User_EditorGadgets[GD_UserAccessLevel_cycle], User_EditorWnd,NULL, 
                          GTCY_Labels, (ULONG)AccessLevelLabels, TAG_DONE);
        GT_SetGadgetAttrs(User_EditorGadgets[GD_UserArchiver_cycle], User_EditorWnd,NULL, 
                          GTCY_Labels, (ULONG)UserArchiver_options, TAG_DONE);
        GT_SetGadgetAttrs(User_EditorGadgets[GD_UserEditor_cycle], User_EditorWnd,NULL, 
                          GTCY_Labels, (ULONG)UserEditor_options, TAG_DONE);
        GT_SetGadgetAttrs(User_EditorGadgets[GD_UserCharSet_cycle], User_EditorWnd,NULL, 
                          GTCY_Labels, (ULONG)UserCharSet_options, TAG_DONE);
        GT_SetGadgetAttrs(User_EditorGadgets[GD_UserMenuSet_cycle], User_EditorWnd,NULL, 
                          GTCY_Labels, (ULONG)UserMenuSet_options, TAG_DONE);
        GT_SetGadgetAttrs(User_EditorGadgets[GD_UserUploadProtocol_cycle], User_EditorWnd,NULL, 
                          GTCY_Labels, (ULONG)UserTransfer_options, TAG_DONE);
        GT_SetGadgetAttrs(User_EditorGadgets[GD_UserDownloadProtocol_cycle], User_EditorWnd,NULL, 
                          GTCY_Labels, (ULONG)UserTransfer_options, TAG_DONE);

        GT_SetGadgetAttrs(User_EditorGadgets[GD_UserList_listview],         
                          User_EditorWnd,NULL, GTLV_Labels, &UserListView_list, TAG_DONE);
    }
    return(TRUE);
}


int Main_Status_DisplayCloseWindow(void) { return(0); }

