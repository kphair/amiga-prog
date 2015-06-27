#include "DLG-Edit.h"

/***
 *** User Editor
 ***/

struct USER_DATA userdata;

int UserList_listviewClicked( void ) {

    long SelectedUser=0;
    FILE *UserDataFile;
    char *charindex;
    static char userfile[60];
    static char username[36];
    long UserFileChoice=0;
    long UserMsgChoice=0;
    char i;

    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserList_listview], User_EditorWnd, NULL,
                      GTLV_Selected, &SelectedUser, TAG_DONE);
    
    if(SelectedUser!=~0) {

        strcpy(username,UserNamePtrs[SelectedUser]);
        for (i=0;i<36;i++)
            if (username[i]==32) username[i]='_';

        sprintf(userfile,"USER:%s/User.Data",username);

        if(UserDataFile=fopen(userfile,"rb")) {
            fread(&userdata,sizeof(userdata),1,UserDataFile);
            fclose(UserDataFile);

            GT_SetGadgetAttrs(User_EditorGadgets[GD_UserAccessLevel_cycle],     
                              User_EditorWnd,NULL,GA_Disabled,FALSE,GTCY_Active,AccessLevelIndex(userdata.User_Level), TAG_DONE);
            GT_SetGadgetAttrs(User_EditorGadgets[GD_UserUUCPPriv_cycle],        
                              User_EditorWnd,NULL,GA_Disabled,FALSE,GTCY_Active,userdata.UUCP,TAG_DONE);
            GT_SetGadgetAttrs(User_EditorGadgets[GD_UserNetmailPriv_cycle],     
                              User_EditorWnd,NULL,GA_Disabled,FALSE,GTCY_Active,userdata.NetMail,TAG_DONE);
            
            if (UserMenuSet_numbers[0]) {
                if (charindex=strchr(UserMenuSet_numbers,userdata.menuset))
                    i=(char)(charindex-UserMenuSet_numbers);
                    else i=0;
                GT_SetGadgetAttrs(User_EditorGadgets[GD_UserMenuSet_cycle], User_EditorWnd,NULL,
                                  GA_Disabled,FALSE,GTCY_Active,i,TAG_DONE);
            }
            if (UserArchiver_numbers[0]) {
                if (charindex=strchr(UserArchiver_numbers,userdata.archiver))   
                    i=(char)(charindex-UserArchiver_numbers);
                    else i=0;
                GT_SetGadgetAttrs(User_EditorGadgets[GD_UserArchiver_cycle], User_EditorWnd,NULL,
                                  GA_Disabled,FALSE,GTCY_Active,i,TAG_DONE);
            }
            if (UserCharSet_numbers[0]) {
                if (charindex=strchr(UserCharSet_numbers,userdata.charset))     
                    i=(char)(charindex-UserCharSet_numbers);
                    else i=0;
                GT_SetGadgetAttrs(User_EditorGadgets[GD_UserCharSet_cycle], User_EditorWnd,NULL,
                                  GA_Disabled,FALSE,GTCY_Active,i,TAG_DONE);
            }
            if (UserEditor_numbers[0]) {
                if (charindex=strchr(UserEditor_numbers,userdata.Editor))       
                    i=(char)(charindex-UserEditor_numbers);
                    else i=0;
                GT_SetGadgetAttrs(User_EditorGadgets[GD_UserEditor_cycle], User_EditorWnd,NULL,
                                  GA_Disabled,FALSE,GTCY_Active,i,TAG_DONE);
            }
            if (UserTransfer_letters[0]) {
                if ((userdata.send) && (charindex=strchr(UserTransfer_letters,userdata.send)))
                    i=1+(char)(charindex-UserTransfer_letters);
                    else i=0;
                GT_SetGadgetAttrs(User_EditorGadgets[GD_UserUploadProtocol_cycle], User_EditorWnd,NULL,
                                  GA_Disabled,FALSE,GTCY_Active,i,TAG_DONE);
                printf("Send: [%c] [%s](%d)\n",userdata.send,UserTransfer_letters,i);
            }
            if (UserTransfer_letters[0]) {
                if ((userdata.receive) && (charindex=strchr(UserTransfer_letters,userdata.receive)))
                    i=1+(char)(charindex-UserTransfer_letters);
                    else i=0;
                GT_SetGadgetAttrs(User_EditorGadgets[GD_UserDownloadProtocol_cycle], User_EditorWnd,NULL,
                                  GA_Disabled,FALSE,GTCY_Active,i,TAG_DONE);
                printf("Recv: [%c] [%s](%d)\n",userdata.receive,UserTransfer_letters,i);
            }

            GT_SetGadgetAttrs(User_EditorGadgets[GD_UserTerminal_cycle],        
                              User_EditorWnd,NULL,GA_Disabled,FALSE,GTCY_Active,userdata.Computer_Type-1,TAG_DONE);
            GT_SetGadgetAttrs(User_EditorGadgets[GD_UserHelp_cycle],            
                              User_EditorWnd,NULL,GA_Disabled,FALSE,GTCY_Active,userdata.Help_Level,TAG_DONE);
            GT_SetGadgetAttrs(User_EditorGadgets[GD_UserFileChooser_cycle],
                              User_EditorWnd,NULL,GA_Disabled,FALSE,TAG_DONE);
            GT_SetGadgetAttrs(User_EditorGadgets[GD_UserMsgChooser_cycle],
                              User_EditorWnd,NULL,GA_Disabled,FALSE,TAG_DONE);
        
            GT_SetGadgetAttrs(User_EditorGadgets[GD_UserMore_check],            
                              User_EditorWnd,NULL,GA_Disabled,FALSE,GTCB_Checked, (userdata.More_Flag ? TRUE : FALSE), TAG_DONE);
            GT_SetGadgetAttrs(User_EditorGadgets[GD_UserColour_check],          
                              User_EditorWnd,NULL,GA_Disabled,FALSE,GTCB_Checked, (userdata.Ansi_Flag&&ANSI_COLOR ? TRUE : FALSE), TAG_DONE);
            GT_SetGadgetAttrs(User_EditorGadgets[GD_UserPositioning_check],     
                              User_EditorWnd,NULL,GA_Disabled,FALSE,GTCB_Checked, (userdata.Ansi_Flag&&ANSI_POS ? TRUE : FALSE), TAG_DONE);
            GT_SetGadgetAttrs(User_EditorGadgets[GD_UserScreenClear_check],     
                              User_EditorWnd,NULL,GA_Disabled,FALSE,GTCB_Checked, (userdata.Ansi_Flag&&ANSI_CLR ? TRUE : FALSE), TAG_DONE);
            GT_SetGadgetAttrs(User_EditorGadgets[GD_UserPartialScroll_check],   
                              User_EditorWnd,NULL,GA_Disabled,FALSE,GTCB_Checked, (userdata.Ansi_Flag&&ANSI_SCROLL ? TRUE : FALSE), TAG_DONE);
            GT_SetGadgetAttrs(User_EditorGadgets[GD_UserHotKeys_check],         
                              User_EditorWnd,NULL,GA_Disabled,FALSE,GTCB_Checked, (userdata.Hot_Keys ? TRUE : FALSE), TAG_DONE);
            GT_SetGadgetAttrs(User_EditorGadgets[GD_UserPopScreen_check],       
                              User_EditorWnd,NULL,GA_Disabled,FALSE,GTCB_Checked, (userdata.Pop_Screen ? TRUE : FALSE), TAG_DONE);
            GT_SetGadgetAttrs(User_EditorGadgets[GD_UserLexCheck_check],        
                              User_EditorWnd,NULL,GA_Disabled,FALSE,GTCB_Checked, (userdata.Lex_Flag ? TRUE : FALSE), TAG_DONE);
            GT_SetGadgetAttrs(User_EditorGadgets[GD_UserBulletinWrite_check],   
                              User_EditorWnd,NULL,GA_Disabled,FALSE,GTCB_Checked, (userdata.Bulletin_Access ? TRUE : FALSE), TAG_DONE);
            GT_SetGadgetAttrs(User_EditorGadgets[GD_UserCapture_check],         
                              User_EditorWnd,NULL,GA_Disabled,FALSE,GTCB_Checked, (userdata.captureflags ? TRUE : FALSE), TAG_DONE);
            
            GT_SetGadgetAttrs(User_EditorGadgets[GD_UserLastLogin_string],      
                              User_EditorWnd,NULL, GA_Disabled,FALSE, GTST_String,&userdata.Last_Login, TAG_DONE);
            GT_SetGadgetAttrs(User_EditorGadgets[GD_UserLoginStack_string],     
                              User_EditorWnd,NULL, GA_Disabled,FALSE, GTST_String,&userdata.stack, TAG_DONE);
            GT_SetGadgetAttrs(User_EditorGadgets[GD_UserPassword_string],       
                              User_EditorWnd,NULL, GA_Disabled,FALSE, GTST_String,&userdata.Password, TAG_DONE);
            GT_SetGadgetAttrs(User_EditorGadgets[GD_UserName_string],           
                              User_EditorWnd,NULL, GA_Disabled,FALSE, GTST_String,UserNamePtrs[SelectedUser], TAG_DONE);
            GT_SetGadgetAttrs(User_EditorGadgets[GD_UserAlias_string],          
                              User_EditorWnd,NULL, GA_Disabled,FALSE, GTST_String,&userdata.Alias, TAG_DONE);
            GT_SetGadgetAttrs(User_EditorGadgets[GD_UserAddress_string],        
                              User_EditorWnd,NULL, GA_Disabled,FALSE, GTST_String,&userdata.Address, TAG_DONE);
            GT_SetGadgetAttrs(User_EditorGadgets[GD_UserCity_string],           
                              User_EditorWnd,NULL, GA_Disabled,FALSE, GTST_String,&userdata.City, TAG_DONE);
            GT_SetGadgetAttrs(User_EditorGadgets[GD_UserState_string],          
                              User_EditorWnd,NULL, GA_Disabled,FALSE, GTST_String,&userdata.Prov, TAG_DONE);
            GT_SetGadgetAttrs(User_EditorGadgets[GD_UserZip_string],            
                              User_EditorWnd,NULL, GA_Disabled,FALSE, GTST_String,&userdata.postal, TAG_DONE);
            GT_SetGadgetAttrs(User_EditorGadgets[GD_UserCountry_string],        
                              User_EditorWnd,NULL, GA_Disabled,FALSE, GTST_String,&userdata.Country, TAG_DONE);
            GT_SetGadgetAttrs(User_EditorGadgets[GD_UserPhone_string],          
                              User_EditorWnd,NULL, GA_Disabled,FALSE, GTST_String,&userdata.Phone, TAG_DONE);
            /*
            GT_SetGadgetAttrs(User_EditorGadgets[GD_UserJoined_string],         
                              User_EditorWnd,NULL, GA_Disabled,FALSE, GTST_String,&userdata., TAG_DONE);
            */
            
            GT_GetGadgetAttrs(User_EditorGadgets[GD_UserFileChooser_cycle],
                              User_EditorWnd,NULL, GTCY_Active, &UserFileChoice, TAG_DONE);
            switch(UserFileChoice) {

                case 0:       GT_SetGadgetAttrs(User_EditorGadgets[GD_UserFileInput_integer], User_EditorWnd,NULL, GA_Disabled,FALSE, GTIN_Number,userdata.Files_Uploaded, TAG_DONE);
                              break;
                case 1:       GT_SetGadgetAttrs(User_EditorGadgets[GD_UserFileInput_integer], User_EditorWnd,NULL, GA_Disabled,FALSE, GTIN_Number,userdata.Files_Downloaded, TAG_DONE);
                              break;
                case 2:       GT_SetGadgetAttrs(User_EditorGadgets[GD_UserFileInput_integer], User_EditorWnd,NULL, GA_Disabled,FALSE, GTIN_Number,userdata.Bytes_Uploaded, TAG_DONE);
                              break;
                case 3:       GT_SetGadgetAttrs(User_EditorGadgets[GD_UserFileInput_integer], User_EditorWnd,NULL, GA_Disabled,FALSE, GTIN_Number,userdata.Bytes_Downloaded, TAG_DONE);
                              break;
                case 4:       GT_SetGadgetAttrs(User_EditorGadgets[GD_UserFileInput_integer], User_EditorWnd,NULL, GA_Disabled,FALSE, GTIN_Number,userdata.DirLimit, TAG_DONE);
                              break;
                case 5:       GT_SetGadgetAttrs(User_EditorGadgets[GD_UserFileInput_integer], User_EditorWnd,NULL, GA_Disabled,FALSE, GTIN_Number,userdata.Ratio, TAG_DONE);
                              break;
            }

            GT_GetGadgetAttrs(User_EditorGadgets[GD_UserMsgChooser_cycle],
                              User_EditorWnd,NULL, GTCY_Active, &UserMsgChoice, TAG_DONE);
            switch(UserMsgChoice) {

                case 0:       GT_SetGadgetAttrs(User_EditorGadgets[GD_UserMsgInput_integer], User_EditorWnd,NULL, GA_Disabled,FALSE, GTIN_Number,userdata.Messages_Read, TAG_DONE);
                              break;
                case 1:       GT_SetGadgetAttrs(User_EditorGadgets[GD_UserMsgInput_integer], User_EditorWnd,NULL, GA_Disabled,FALSE, GTIN_Number,userdata.Messages_Entered, TAG_DONE);
                              break;
                case 2:       GT_SetGadgetAttrs(User_EditorGadgets[GD_UserMsgInput_integer], User_EditorWnd,NULL, GA_Disabled,FALSE, GTIN_Number,userdata.credit, TAG_DONE);
                              break;
            }

            GT_SetGadgetAttrs(User_EditorGadgets[GD_UserTermWidth_integer],     
                              User_EditorWnd,NULL, GA_Disabled,FALSE, GTIN_Number,userdata.Screen_Width, TAG_DONE);
            GT_SetGadgetAttrs(User_EditorGadgets[GD_UserSessionLimit_integer],  
                              User_EditorWnd,NULL, GA_Disabled,FALSE, GTIN_Number,userdata.Session_Limit, TAG_DONE);
            GT_SetGadgetAttrs(User_EditorGadgets[GD_UserDailyLimit_integer],    
                              User_EditorWnd,NULL, GA_Disabled,FALSE, GTIN_Number,userdata.Daily_Limit, TAG_DONE);
            GT_SetGadgetAttrs(User_EditorGadgets[GD_UserTimeUsedToday_integer], 
                              User_EditorWnd,NULL, GA_Disabled,FALSE, GTIN_Number,userdata.Daily_Used, TAG_DONE);
            GT_SetGadgetAttrs(User_EditorGadgets[GD_UserSysOpPages_integer],    
                              User_EditorWnd,NULL, GA_Disabled,FALSE, GTIN_Number,userdata.Sysop_Pages, TAG_DONE);
            GT_SetGadgetAttrs(User_EditorGadgets[GD_UserTotalCalls_integer],    
                              User_EditorWnd,NULL, GA_Disabled,FALSE, GTIN_Number,userdata.Total_Calls, TAG_DONE);
            GT_SetGadgetAttrs(User_EditorGadgets[GD_UserTotalTime_integer],     
                              User_EditorWnd,NULL, GA_Disabled,FALSE, GTIN_Number,userdata.Online_Time, TAG_DONE);
            GT_SetGadgetAttrs(User_EditorGadgets[GD_UserTermHeight_integer],    
                              User_EditorWnd,NULL, GA_Disabled,FALSE, GTIN_Number,userdata.Screen_Len, TAG_DONE);

            GT_SetGadgetAttrs(User_EditorGadgets[GD_UserSave_button],           
                              User_EditorWnd,NULL, GA_Disabled,FALSE, TAG_DONE);
            GT_SetGadgetAttrs(User_EditorGadgets[GD_UserGlobalFile_button],     
                              User_EditorWnd,NULL, GA_Disabled,TRUE, TAG_DONE);
            GT_SetGadgetAttrs(User_EditorGadgets[GD_UserGlobalMsg_button],      
                              User_EditorWnd,NULL, GA_Disabled,TRUE, TAG_DONE);
            GT_SetGadgetAttrs(User_EditorGadgets[GD_UserGlobalArc_button],      
                              User_EditorWnd,NULL, GA_Disabled,TRUE, TAG_DONE);
        }
    }
    return(TRUE);
}


/************
 *      //  *
 *  \\ //   *
 *   \X/    *
 ************/

int UserLastFirst_checkClicked( void ) { 
    return(TRUE); 
}
int UserColour_checkClicked( void ) { 
    ULONG templong;
    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserColour_check],          
                      User_EditorWnd,NULL,GTCB_Checked,&templong,TAG_DONE);
    userdata.Ansi_Flag|=ANSI_COLOR; if(templong!=TRUE) userdata.Ansi_Flag^=ANSI_COLOR;
    return(TRUE); 
}
int UserPositioning_checkClicked( void ) { 
    ULONG templong;
    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserPositioning_check],     
                      User_EditorWnd,NULL,GTCB_Checked,&templong,TAG_DONE);
    userdata.Ansi_Flag|=ANSI_POS; if(templong!=TRUE) userdata.Ansi_Flag^=ANSI_POS;
    return(TRUE); 
}
int UserScreenClear_checkClicked( void ) { 
    ULONG templong;
    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserScreenClear_check],     
                      User_EditorWnd,NULL,GTCB_Checked,&templong,TAG_DONE);
    userdata.Ansi_Flag|=ANSI_CLR; if(templong!=TRUE) userdata.Ansi_Flag^=ANSI_CLR;
    return(TRUE); 
}
int UserPartialScroll_checkClicked( void ) { 
    ULONG templong;
    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserPartialScroll_check],   
                      User_EditorWnd,NULL,GTCB_Checked,&templong,TAG_DONE);
    userdata.Ansi_Flag|=ANSI_SCROLL; if(templong!=TRUE) userdata.Ansi_Flag^=ANSI_SCROLL;
    return(TRUE); 
}
int UserHotKeys_checkClicked( void ) { 
    ULONG templong;
    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserHotKeys_check],         
                      User_EditorWnd,NULL,GTCB_Checked,&templong,TAG_DONE);
    userdata.Hot_Keys=1; if(templong!=TRUE) userdata.Hot_Keys=0;
    return(TRUE); 
}
int UserPopScreen_checkClicked( void ) { 
    ULONG templong;
    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserPopScreen_check],       
                      User_EditorWnd,NULL,GTCB_Checked,&templong,TAG_DONE);
    userdata.Pop_Screen=1; if(templong!=TRUE) userdata.Pop_Screen=0;
    return(TRUE); 
}
int UserLexCheck_checkClicked( void ) { 
    ULONG templong;
    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserLexCheck_check],        
                      User_EditorWnd,NULL,GTCB_Checked,&templong,TAG_DONE);
    userdata.Lex_Flag=1; if(templong!=TRUE) userdata.Lex_Flag=0;
    return(TRUE); 
}
int UserBulletinWrite_checkClicked( void ) { 
    ULONG templong;
    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserBulletinWrite_check],   
                      User_EditorWnd,NULL,GTCB_Checked,&templong,TAG_DONE);
    userdata.Bulletin_Access=1; if(templong!=TRUE) userdata.Bulletin_Access=0;
    return(TRUE); 
}
int UserCapture_checkClicked( void ) { 
    ULONG templong;
    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserCapture_check],         
                      User_EditorWnd,NULL,GTCB_Checked,&templong,TAG_DONE);
    userdata.captureflags=1; if(templong!=TRUE) userdata.captureflags=0;
    return(TRUE); 
}
int UserMore_checkClicked( void ) {
    ULONG templong;
    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserMore_check],            
                      User_EditorWnd,NULL,GTCB_Checked,&templong,TAG_DONE);
    userdata.More_Flag=1; if(templong!=TRUE) userdata.More_Flag=0;
    return(TRUE);
}

/************************
 * .--. *               *
 * |  v * CYCLE GADGETS *
 * `-   *               *
 ************************/

int UserAccessLevel_cycleClicked( void ) {
    ULONG templong;
    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserAccessLevel_cycle],     
                      User_EditorWnd,NULL,GTCY_Active,&templong,TAG_DONE);
    userdata.User_Level=atoi((const char *)AccessLevelLabels[templong]);
    return(TRUE);
}
int UserEditor_cycleClicked( void ) { 
    ULONG templong;
    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserEditor_cycle],          
                      User_EditorWnd,NULL,GTCY_Active,&templong,TAG_DONE);
    userdata.Editor=UserEditor_numbers[templong];
    return(TRUE); 
}
int UserCharSet_cycleClicked( void ) { 
    ULONG templong;
    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserCharSet_cycle],         
                      User_EditorWnd,NULL,GTCY_Active,&templong,TAG_DONE);
    userdata.charset=UserCharSet_numbers[templong];
    return(TRUE); 
}
int UserArchiver_cycleClicked( void ) { 
    ULONG templong;
    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserArchiver_cycle],        
                      User_EditorWnd,NULL,GTCY_Active,&templong,TAG_DONE);
    userdata.archiver=UserArchiver_numbers[templong];
    return(TRUE); 
}
int UserMenuSet_cycleClicked( void ) {
    ULONG templong;
    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserMenuSet_cycle],         
                      User_EditorWnd,NULL,GTCY_Active,&templong,TAG_DONE);
    userdata.menuset=UserMenuSet_numbers[templong];
    return(TRUE);
}
int UserUUCPPriv_cycleClicked( void ) { 
    ULONG templong;
    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserUUCPPriv_cycle],        
                      User_EditorWnd,NULL,GTCY_Active,&templong,TAG_DONE);
    userdata.UUCP=templong;
    return(TRUE); 
}
int UserNetmailPriv_cycleClicked( void ) { 
    ULONG templong;
    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserNetmailPriv_cycle],     
                      User_EditorWnd,NULL,GTCY_Active,&templong,TAG_DONE);
    userdata.NetMail=templong;
    return(TRUE); 
}
int UserUploadProtocol_cycleClicked( void ) { 
    ULONG templong;
    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserUploadProtocol_cycle],  
                      User_EditorWnd,NULL,GTCY_Active,&templong,TAG_DONE);
    if (templong) userdata.send=UserTransfer_letters[templong-1];
        else userdata.send=0;
    return(TRUE); 
}
int UserDownloadProtocol_cycleClicked( void ) { 
    ULONG templong;
    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserDownloadProtocol_cycle],
                      User_EditorWnd,NULL,GTCY_Active,&templong,TAG_DONE);
    if (templong) userdata.receive=UserTransfer_letters[templong-1];
        else userdata.receive=0;
    return(TRUE);
}
int UserHelp_cycleClicked( void ) { 
    ULONG templong;
    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserHelp_cycle],            
                      User_EditorWnd,NULL,GTCY_Active,&templong,TAG_DONE);
    userdata.Help_Level=templong;
    return(TRUE); 
}
int UserTerminal_cycleClicked( void ) { 
    ULONG templong;
    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserTerminal_cycle],        
                      User_EditorWnd,NULL,GTCY_Active,&templong,TAG_DONE);
    userdata.Computer_Type=templong+1;
    return(TRUE); 
}

int UserMsgChooser_cycleClicked( void ) {
    ULONG UserMsgChoice;

    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserMsgChooser_cycle],
                      User_EditorWnd,NULL, GTCY_Active, &UserMsgChoice, TAG_DONE);
    switch(UserMsgChoice) {

        case 0:       GT_SetGadgetAttrs(User_EditorGadgets[GD_UserMsgInput_integer], User_EditorWnd,NULL, GA_Disabled,FALSE, GTIN_Number,userdata.Messages_Read, TAG_DONE);
                      break;
        case 1:       GT_SetGadgetAttrs(User_EditorGadgets[GD_UserMsgInput_integer], User_EditorWnd,NULL, GA_Disabled,FALSE, GTIN_Number,userdata.Messages_Entered, TAG_DONE);
                      break;
        case 2:       GT_SetGadgetAttrs(User_EditorGadgets[GD_UserMsgInput_integer], User_EditorWnd,NULL, GA_Disabled,FALSE, GTIN_Number,userdata.credit, TAG_DONE);
                      break;
    }
    return(TRUE);
}

int UserFileChooser_cycleClicked( void ) {
    ULONG UserFileChoice;

    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserFileChooser_cycle],
                      User_EditorWnd,NULL, GTCY_Active, &UserFileChoice, TAG_DONE);
    switch(UserFileChoice) {

        case 0:       GT_SetGadgetAttrs(User_EditorGadgets[GD_UserFileInput_integer], User_EditorWnd,NULL, GA_Disabled,FALSE, GTIN_Number,userdata.Files_Uploaded, TAG_DONE);
                      break;
        case 1:       GT_SetGadgetAttrs(User_EditorGadgets[GD_UserFileInput_integer], User_EditorWnd,NULL, GA_Disabled,FALSE, GTIN_Number,userdata.Files_Downloaded, TAG_DONE);
                      break;
        case 2:       GT_SetGadgetAttrs(User_EditorGadgets[GD_UserFileInput_integer], User_EditorWnd,NULL, GA_Disabled,FALSE, GTIN_Number,userdata.Bytes_Uploaded, TAG_DONE);
                      break;
        case 3:       GT_SetGadgetAttrs(User_EditorGadgets[GD_UserFileInput_integer], User_EditorWnd,NULL, GA_Disabled,FALSE, GTIN_Number,userdata.Bytes_Downloaded, TAG_DONE);
                      break;
        case 4:       GT_SetGadgetAttrs(User_EditorGadgets[GD_UserFileInput_integer], User_EditorWnd,NULL, GA_Disabled,FALSE, GTIN_Number,userdata.DirLimit, TAG_DONE);
                      break;
        case 5:       GT_SetGadgetAttrs(User_EditorGadgets[GD_UserFileInput_integer], User_EditorWnd,NULL, GA_Disabled,FALSE, GTIN_Number,userdata.Ratio, TAG_DONE);
                      break;
    }
    return(TRUE);
}



/******************
 *                *
 * STRING GADGETS *
 *                *
 ******************/

int UserName_stringClicked( void ) { 
    /*
    ULONG templong;
    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserName_string],       
                      User_EditorWnd,NULL,GTST_String,&templong, TAG_DONE);
    strcpy(UserNamePtrs[SelectedUser], ...);
    */
    return(TRUE);
}
int UserAddress_stringClicked( void ) { 
    ULONG templong;
    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserAddress_string],    
                      User_EditorWnd,NULL,GTST_String,&templong, TAG_DONE);
    strcpy(userdata.Address,(char const *)templong);
    return(TRUE); 
}
int UserCity_stringClicked( void ) { 
    ULONG templong;
    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserCity_string],       
                      User_EditorWnd,NULL,GTST_String,&templong, TAG_DONE);
    strcpy(userdata.City,(char const *)templong);
    return(TRUE); 
}
int UserState_stringClicked( void ) { 
    ULONG templong;
    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserState_string],      
                      User_EditorWnd,NULL,GTST_String,&templong, TAG_DONE);
    strcpy(userdata.Prov,(char const *)templong);
    return(TRUE); 
}
int UserCountry_stringClicked( void ) { 
    ULONG templong;
    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserCountry_string],    
                      User_EditorWnd,NULL,GTST_String,&templong, TAG_DONE);
    strcpy(userdata.Country,(char const *)templong);
    return(TRUE); 
}
int UserZip_stringClicked( void ) { 
    ULONG templong;
    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserZip_string],        
                      User_EditorWnd,NULL,GTST_String,&templong, TAG_DONE);
    strcpy(userdata.postal,(char const *)templong);
    return(TRUE); 
}
int UserAlias_stringClicked( void ) { 
    ULONG templong;
    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserAlias_string],      
                      User_EditorWnd,NULL,GTST_String,&templong, TAG_DONE);
    strcpy(userdata.Alias,(char const *)templong);
    return(TRUE); 
}
int UserPassword_stringClicked( void ) { 
    ULONG templong;
    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserPassword_string],   
                      User_EditorWnd,NULL,GTST_String,&templong, TAG_DONE);
    strcpy(userdata.Password,(char const *)templong);
    return(TRUE); 
}
int UserJoined_stringClicked( void ) { 
    /* 
    ULONG templong;
    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserJoined_string],     
                      User_EditorWnd,NULL,GTST_String,&templong, TAG_DONE);
    */
    return(TRUE);
}
int UserLastLogin_stringClicked( void ) { 
    ULONG templong;
    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserLastLogin_string],  
                      User_EditorWnd,NULL,GTST_String,&templong, TAG_DONE);
    strcpy(userdata.Last_Login,(char const *)templong);
    return(TRUE); 
}
int UserPhone_stringClicked( void ) { 
    ULONG templong;
    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserPhone_string],      
                      User_EditorWnd,NULL,GTST_String,&templong, TAG_DONE);
    strcpy(userdata.Phone,(char const *)templong);
    return(TRUE); 
}
int UserLoginStack_stringClicked( void ) { 
    ULONG templong;
    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserLoginStack_string], 
                      User_EditorWnd,NULL,GTST_String,&templong, TAG_DONE);
    strcpy(userdata.stack,(char const *)templong);
    return(TRUE); 
}

/*******************
 *                 *
 * INTEGER GADGETS *
 *                 *
 *******************/

int UserTermHeight_integerClicked( void ) { 
    ULONG templong;
    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserTermHeight_integer],    
                      User_EditorWnd,NULL,GTIN_Number,&templong, TAG_DONE);
    userdata.Screen_Len=templong;
    return(TRUE); 
}
int UserTermWidth_integerClicked( void ) { 
    ULONG templong;
    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserTermWidth_integer],     
                      User_EditorWnd,NULL,GTIN_Number,&templong, TAG_DONE);
    userdata.Screen_Width=templong;
    return(TRUE); 
}
int UserDailyLimit_integerClicked( void ) { 
    ULONG templong;
    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserDailyLimit_integer],    
                      User_EditorWnd,NULL,GTIN_Number,&templong, TAG_DONE);
    userdata.Daily_Limit=templong;
    return(TRUE); 
}
int UserSessionLimit_integerClicked( void ) { 
    ULONG templong;
    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserSessionLimit_integer],  
                      User_EditorWnd,NULL,GTIN_Number,&templong, TAG_DONE);
    userdata.Session_Limit=templong;
    return(TRUE); 
}
int UserSysOpPages_integerClicked( void ) { 
    ULONG templong;
    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserSysOpPages_integer],    
                      User_EditorWnd,NULL,GTIN_Number,&templong, TAG_DONE);
    userdata.Sysop_Pages=templong;
    return(TRUE); 
}
int UserTimeUsedToday_integerClicked( void ) { 
    ULONG templong;
    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserTimeUsedToday_integer], 
                      User_EditorWnd,NULL,GTIN_Number,&templong, TAG_DONE);
    userdata.Daily_Used=templong;
    return(TRUE); 
}
int UserTotalCalls_integerClicked( void ) { 
    ULONG templong;
    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserTotalCalls_integer],    
                      User_EditorWnd,NULL,GTIN_Number,&templong, TAG_DONE);
    userdata.Total_Calls=templong;
    return(TRUE); 
}
int UserTotalTime_integerClicked( void ) { 
    ULONG templong;
    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserTotalTime_integer],     
                      User_EditorWnd,NULL,GTIN_Number,&templong, TAG_DONE);
    userdata.Online_Time=templong;
    return(TRUE); 
}
int UserLastMsgArea_integerClicked( void ) {
    ULONG templong;
    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserLastMsgArea_integer],   
                      User_EditorWnd,NULL,GTIN_Number,&templong, TAG_DONE);
    userdata.Last_Area=templong;
    return(TRUE); 
}
int UserLastFileArea_integerClicked( void ) { 
    ULONG templong;
    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserLastFileArea_integer],  
                      User_EditorWnd,NULL,GTIN_Number,&templong, TAG_DONE);
    userdata.Last_File_Area=templong;
    return(TRUE); 
}
int UserFileInput_integerClicked( void ) {
    ULONG templong,UserFileChoice;
    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserFileInput_integer],
                      User_EditorWnd,NULL,GTIN_Number,&templong, TAG_DONE);
    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserFileChooser_cycle],
                      User_EditorWnd,NULL, GTCY_Active, &UserFileChoice, TAG_DONE);
    switch(UserFileChoice) {

        case 0:       userdata.Files_Uploaded=templong;
                      break;
        case 1:       userdata.Files_Downloaded=templong;
                      break;
        case 2:       userdata.Bytes_Uploaded=templong;
                      break;
        case 3:       userdata.Bytes_Downloaded=templong;
                      break;
        case 4:       userdata.DirLimit=templong;
                      break;
        case 5:       userdata.Ratio=templong;
                      break;
    }
    return(TRUE);
}

int UserMsgInput_integerClicked( void ) {
    ULONG templong,UserMsgChoice;
    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserMsgInput_integer],
                      User_EditorWnd,NULL,GTIN_Number,&templong, TAG_DONE);
    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserMsgChooser_cycle],
                      User_EditorWnd,NULL, GTCY_Active, &UserMsgChoice, TAG_DONE);

    switch(UserMsgChoice) {

        case 0:       userdata.Messages_Read=templong;
                      break;
        case 1:       userdata.Messages_Entered=templong;
                      break;
        case 2:       userdata.credit=templong;
                      break;
    }
    return(TRUE);
}

/**********
 *        *
 *  SAVE  *
 *        *
 **********/

int UserSave_buttonClicked( void ) {

    long SelectedUser=0;
    FILE *UserDataFile;
    static char newuserfile[60];
    static char olduserfile[60];
    static char username[36];
    char i;

    GT_GetGadgetAttrs(User_EditorGadgets[GD_UserList_listview], User_EditorWnd, NULL,
                      GTLV_Selected, &SelectedUser, TAG_DONE);

    if(SelectedUser!=~0) {

        strcpy(username,UserNamePtrs[SelectedUser]);
        for (i=0;i<36;i++) if (username[i]==32) username[i]='_';

        sprintf(newuserfile,"USER:%s/User.Data",username);
        sprintf(olduserfile,"USER:%s/User.Data.old",username);

        unlink(olduserfile);
        rename(newuserfile,olduserfile);
    
        if(UserDataFile=fopen(newuserfile,"wb")) {
            fwrite(&userdata,sizeof(userdata),1,UserDataFile);
            fclose(UserDataFile);
        }
    }
    return(TRUE);
}

int UserGlobalFile_buttonClicked( void ) { 
    return(TRUE); 
}
int UserGlobalMsg_buttonClicked( void ) { 
    return(TRUE); 
}
int UserGlobalArc_buttonClicked( void ) { 
    return(TRUE); 
}

/********
 * ____ *
 * #  # *
 * ~~~~ *
 ********/

int User_EditorCloseWindow( void ) {           /* "IDCMP_CLOSEWINDOW". */
    return(FALSE);
}

