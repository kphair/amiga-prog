#include "DLG-Edit_IDCMP.proto"
#include "DLG-Edit_GUI.c"
#include <DLG/dlg.h>
#include <stdio.h>
#include <stdlib.h>

#define MaxUsers    3000
#define NAMENODE_ID 100
#define MEMPOOL     MEMF_FAST|MEMF_PUBLIC|MEMF_CLEAR

#define MAKEAREA    1
#define KILLAREA    2
#define RENUMAREA   3
#define SWAPAREA    4

#define MAINWnd     0x0001

#define FILEWnd     0x0010
#define FILETemp    0x0020
#define MSGWnd      0x0100
#define MSGTemp     0x0200
#define USERWnd     0x1000

#include "DLG-Edit_rev.h"

    char myVersionTag[]=VERSTAG;
    char myVersionString[]=VSTRING;

    struct EasyStruct AboutReq={
        sizeof(struct EasyStruct),
        0,
        "About DLG-Edit",
        "%.25s - Release v0.91\n"                         \
        "© 1994 Kevin Phair - Freely distributable\n\n"   \
        "Internet:\n"                                     \
        "    Kevin_Phair@nemesis.internet-eireann.ie\n\n" \
        "FidoNet:\n"                                      \
        "    Kevin Phair, 2:263/150.6",
        "Okay"
    };

    struct EasyStruct AreaExistsReq={
        sizeof(struct EasyStruct),
        0,
        "DLG-Edit Warning",
        "That area already exists!",
        "Okay"
    };

    struct EasyStruct ImportAreaExistsReq={
        sizeof(struct EasyStruct),
        0,
        "Area Import Warning",
        "Area %d already exists!\n" \
        "It will be skipped...",
        "Okay"
    };

    struct EasyStruct BadImportFileReq={
        sizeof(struct EasyStruct),
        0,
        "Area Import Warning",
        "Could not open file for import:\n" \
        "`%s'",
        "Okay"
    };

    void *UserTerminal_pool,
         *UserArchiver_pool,
         *UserEditor_pool,
         *UserCharSet_pool,
         *UserMenuSet_pool,
         *UserTransferProtocol_pool,
         *UserUserList_pool,
         *FileAreaList_pool,
         *MsgAreaList_pool;

    UBYTE **UserTerminal_options;
    UBYTE **UserArchiver_options;
    char   *UserArchiver_numbers;
    UBYTE **UserEditor_options;
    char   *UserEditor_numbers;
    UBYTE **UserCharSet_options;
    char   *UserCharSet_numbers;
    UBYTE **UserMenuSet_options;
    char   *UserMenuSet_numbers;
    UBYTE **UserTransfer_options;

    char **UserNamePtrs;
    struct DLGAreaNode **FileAreaPtrs;
    struct DLGAreaNode **MsgAreaPtrs;

    SHORT my_sigbits;

    ULONG all_mask,
          main_mask,
          fileman_mask,filetemp_mask,
          msgman_mask,msgtemp_mask,
          usered_mask;

    SHORT active_wnd;

    UBYTE *AccessLevelLabels[]={
          (UBYTE *)"1",
          (UBYTE *)"10",
          (UBYTE *)"50",
          (UBYTE *)"100",
          (UBYTE *)"150",
          (UBYTE *)"220",
          (UBYTE *)"255",
          NULL };

    struct MinList *UserListView_list;
    struct MinList *FileAreaList_list;
    struct MinList *MsgAreaList_list;

    struct NameNode {
        struct Node nn_Node;        
        UBYTE  nn_Name[36];         
    };

    struct DLGAreaNode {
        struct Node nn_Node;        
        struct Msg_Area nn_Area;
        UBYTE  nn_ListName[42];
    };

void AllocatePointerMem(void) 
{

    UserTerminal_options=   (UBYTE **)calloc(128,sizeof(ULONG));
    UserArchiver_options=   (UBYTE **)calloc(128,sizeof(ULONG));
    UserArchiver_numbers=     (char *)calloc(128,sizeof(char));
    UserEditor_options=     (UBYTE **)calloc(128,sizeof(ULONG));
    UserEditor_numbers=       (char *)calloc(128,sizeof(char));
    UserCharSet_options=    (UBYTE **)calloc(128,sizeof(ULONG));
    UserCharSet_numbers=      (char *)calloc(128,sizeof(char));
    UserMenuSet_options=    (UBYTE **)calloc(128,sizeof(ULONG));
    UserMenuSet_numbers=      (char *)calloc(128,sizeof(char));
    UserTransfer_options=   (UBYTE **)calloc(128,sizeof(ULONG));

    UserNamePtrs=           (char **)calloc(MaxUsers,sizeof(ULONG));
    FileAreaPtrs=           (struct DLGAreaNode **)calloc(10000,sizeof(ULONG));
    MsgAreaPtrs=            (struct DLGAreaNode **)calloc(10000,sizeof(ULONG));
}

void FreePointerMem(void) 
{

    free(UserTerminal_options);
    free(UserArchiver_options);
    free(UserArchiver_numbers);
    free(UserEditor_options);
    free(UserEditor_numbers);
    free(UserCharSet_options);
    free(UserCharSet_numbers);
    free(UserMenuSet_options);
    free(UserMenuSet_numbers);
    free(UserTransfer_options);

    free(UserNamePtrs);
    free(FileAreaPtrs);
    free(MsgAreaPtrs);
}


void MsgActionQ(int action, SHORT area1, SHORT area2) {

    FILE *qfile;

    if (qfile=fopen("T:MsgActionQueue","aa")) {

        fseek(qfile,0,SEEK_END);

        switch(action) {

            case MAKEAREA:
                fprintf(qfile,"echo Creating new area: %d\n",area1);
                fprintf(qfile,"makedir MSG:%d\n",area1);
                break;

            case KILLAREA:
                fprintf(qfile,"echo Removing area %d\n",area1);
                fprintf(qfile,"delete MSG:%d ALL QUIET\n",area1);
                break;

            case SWAPAREA:
                fprintf(qfile,"echo Swapping areas %d amd %d\n",area1,area2);
                fprintf(qfile,"rename MSG:%d MSG:SwapAreaTempName\n",area1);
                fprintf(qfile,"rename MSG:%d MSG:%d",area2,area1);
                fprintf(qfile,"rename MSG:SwapAreaTempName MSG:%d\n",area2);
                break;

            case RENUMAREA:
                fprintf(qfile,"echo Renumbering area %d to %d\n",area1,area2);
                fprintf(qfile,"rename MSG:%d MSG:%d\n",area1,area2);
                break;
        }
        fclose(qfile);
    }
}

void FileActionQ(int action, SHORT area1, SHORT area2) {

    FILE *qfile;

    if (qfile=fopen("T:FileActionQueue","aa")) {

        fseek(qfile,0,SEEK_END);

        switch(action) {

            case MAKEAREA:
                fprintf(qfile,"echo Creating new area: %d\n",area1);
                fprintf(qfile,"makedir FILE:%d\n",area1);
                break;

            case KILLAREA:
                fprintf(qfile,"echo Removing area %d\n",area1);
                fprintf(qfile,"delete FILE:%d ALL QUIET\n",area1);
                break;

            case SWAPAREA:
                fprintf(qfile,"echo Swapping areas %d amd %d\n",area1,area2);
                fprintf(qfile,"rename FILE:%d FILE:SwapAreaTempName\n",area1);
                fprintf(qfile,"rename FILE:%d FILE:%d",area2,area1);
                fprintf(qfile,"rename FILE:SwapAreaTempName FILE:%d\n",area2);
                break;

            case RENUMAREA:
                fprintf(qfile,"echo Renumbering area %d to %d\n",area1,area2);
                fprintf(qfile,"rename FILE:%d FILE:%d",area1,area2);
                break;
        }
        fclose(qfile);
    }
}



ULONG AccessLevelIndex(int access_level) 
{
    ULONG levelindex=0;

    while (AccessLevelLabels[levelindex]) {
        if(access_level<=atoi((const char *)AccessLevelLabels[levelindex])) break;
        levelindex++;
    }
    return(levelindex);
}

void SaveFileAreaList(void) 
{
    FILE *DefsFile;
    int i=0;
    struct DLGAreaNode *namenode;
    struct Msg_Area AreaName;

    unlink("FILE:Area.bbs.old");
    rename("FILE:Area.bbs","FILE:Area.bbs.old");
    
    if (DefsFile=fopen("FILE:Area.bbs","wb")) {

        while(FileAreaPtrs[i]) {
            namenode=FileAreaPtrs[i++];
            memcpy((void *)&AreaName, &(namenode->nn_Area), sizeof(AreaName));
            fwrite(&AreaName,sizeof(AreaName),1,DefsFile);
        }
        fclose(DefsFile);
    }
}

void SaveMsgAreaList(void) 
{
    FILE *DefsFile;
    int i=0;
    struct DLGAreaNode *namenode;
    struct Msg_Area AreaName;

    unlink("MSG:Area.bbs.old");
    rename("MSG:Area.bbs","MSG:Area.bbs.old");
    
    if (DefsFile=fopen("MSG:Area.bbs","wb")) {

        while(MsgAreaPtrs[i]) {
            namenode=MsgAreaPtrs[i++];
            memcpy((void *)&AreaName, &(namenode->nn_Area), sizeof(AreaName));
            fwrite(&AreaName,sizeof(AreaName),1,DefsFile);
        }
        fclose(DefsFile);
    }
}




#include "DLG-Edit_IDCMP.c"


void GetDLGTerminalTypes(void) 
{
    FILE *DefsFile;
    char *poolAllocPtr;
    int i=0;
    struct Computer_Type ComputerTypes;

    UserTerminal_pool=CreatePool(MEMPOOL,2048,2048);
    
    if (DefsFile=fopen("DLGConfig:Port/ComputerTypes.bbs","rb")) {

        while(fread(&ComputerTypes,sizeof(ComputerTypes),1,DefsFile)) {
         /*
         *  Allocate memory for string using pool manager,
         *  Copy the string from structure into the allocation,
         *  And place the pointer to it into the cycle gadget list...
         */
            poolAllocPtr=AllocPooled(UserTerminal_pool,(ULONG)strlen(ComputerTypes.Name)+1);
            strcpy(poolAllocPtr,ComputerTypes.Name);
            UserTerminal_options[i++]=poolAllocPtr;
        }
        fclose(DefsFile);
    }
}


void GetDLGArchiverTypes(void) 
{
    FILE *DefsFile;
    char *poolAllocPtr;
    int i=0;
    struct Archiver ArchiverTypes;

    UserArchiver_pool=CreatePool(MEMPOOL,2048,2048);
    
    if (DefsFile=fopen("DLGConfig:Misc/Archivers.bbs","rb")) {

        while(fread(&ArchiverTypes,sizeof(ArchiverTypes),1,DefsFile)) {
            poolAllocPtr=AllocPooled(UserArchiver_pool,(ULONG)strlen(ArchiverTypes.name)+1);
            strcpy(poolAllocPtr,ArchiverTypes.name);
            UserArchiver_numbers[i]=ArchiverTypes.number;
            UserArchiver_options[i++]=poolAllocPtr;
        }
        fclose(DefsFile);
    }
}


void GetDLGEditorTypes(void) 
{
    FILE *DefsFile;
    char *poolAllocPtr;
    int i=0;
    struct Editor EditorTypes;

    UserEditor_pool=CreatePool(MEMPOOL,2048,2048);
    
    if (DefsFile=fopen("DLGConfig:Misc/Editors.bbs","rb")) {

        while(fread(&EditorTypes,sizeof(EditorTypes),1,DefsFile)) {
            poolAllocPtr=AllocPooled(UserEditor_pool,(ULONG)strlen(EditorTypes.name)+1);
            strcpy(poolAllocPtr,EditorTypes.name);
            UserEditor_numbers[i]=EditorTypes.number;
            UserEditor_options[i++]=poolAllocPtr;
        }
        fclose(DefsFile);
    }
}


void GetDLGCharacterSetTypes(void) 
{
    FILE *DefsFile;
    char *poolAllocPtr;
    int i=0;
    struct CharSet CharSetTypes;

    UserCharSet_pool=CreatePool(MEMPOOL,2048,2048);
    
    if (DefsFile=fopen("DLGConfig:Charsets/CharSets.bbs","rb")) {

        while(fread(&CharSetTypes,sizeof(CharSetTypes),1,DefsFile)) {
            poolAllocPtr=AllocPooled(UserCharSet_pool,(ULONG)strlen(CharSetTypes.name)+1);
            strcpy(poolAllocPtr,CharSetTypes.name);
            UserCharSet_numbers[i]=CharSetTypes.number;
            UserCharSet_options[i++]=poolAllocPtr;
        }
        fclose(DefsFile);
    }
}


void GetDLGMenuSetTypes(void) 
{
    FILE *DefsFile;
    char *poolAllocPtr;
    int i=0;
    struct MenuSet MenuSetTypes;

    UserMenuSet_pool=CreatePool(MEMPOOL,2048,2048);
    
    if (DefsFile=fopen("DLGConfig:Port/MenuSets.bbs","rb")) {

        while(fread(&MenuSetTypes,sizeof(MenuSetTypes),1,DefsFile)) {
            poolAllocPtr=AllocPooled(UserMenuSet_pool,(ULONG)strlen(MenuSetTypes.name)+1);
            strcpy(poolAllocPtr,MenuSetTypes.name);
            UserMenuSet_numbers[i]=MenuSetTypes.number;
            UserMenuSet_options[i++]=poolAllocPtr;
        }
        fclose(DefsFile);
    }
}


void GetDLGTransferProtocols(void) 
{
    FILE *DefsFile;
    char *poolAllocPtr;
    int i=0;
    char askname[]="Ask before transfer";
    struct Protocol TransferTypes;

    UserTransferProtocol_pool=CreatePool(MEMPOOL,2048,2048);

    poolAllocPtr=AllocPooled(UserTransferProtocol_pool,(ULONG)strlen(askname)+1);
    strcpy(poolAllocPtr,askname);
    UserTransfer_options[i++]=poolAllocPtr;

    if (DefsFile=fopen("DLGConfig:Misc/Protocols.bbs","rb")) {

        while(fread(&TransferTypes,sizeof(TransferTypes),1,DefsFile)) {
            poolAllocPtr=AllocPooled(UserTransferProtocol_pool,(ULONG)strlen(TransferTypes.name)+1);
            strcpy(poolAllocPtr,TransferTypes.name);
            UserTransfer_options[i++]=poolAllocPtr;
        }
        fclose(DefsFile);
    }
}


void GetBBSUsers(void) 
{
    FILE *DefsFile;
    void *poolAllocPtr;
    int i=0;
    struct NameNode *namenode;
    char BBSUserName[36];

    UserUserList_pool=CreatePool(MEMPOOL,2048,2048);

    if (DefsFile=fopen("DLGConfig:Misc/Users.bbs","rb")) {

        UserListView_list=AllocPooled(UserUserList_pool,sizeof(struct List));
        NewList((struct List *)UserListView_list);

        while(fread(&BBSUserName,sizeof(BBSUserName),1,DefsFile)) {
            namenode=AllocPooled(UserUserList_pool,sizeof(struct NameNode));
            strcpy(namenode->nn_Name,BBSUserName);
            namenode->nn_Node.ln_Name=namenode->nn_Name;
            namenode->nn_Node.ln_Type=NAMENODE_ID;
            namenode->nn_Node.ln_Pri=0;
            AddTail((struct List *)UserListView_list,(struct Node *)namenode);
            UserNamePtrs[i++]=namenode->nn_Name;
        }
        fclose(DefsFile);
    }
}


void GetFileAreaList(void) 
{
    FILE *DefsFile;
    void *poolAllocPtr;
    int i=0;
    struct DLGAreaNode *namenode;
    struct Msg_Area AreaName;

    FileAreaList_pool=CreatePool(MEMPOOL,2048,2048);

    if (DefsFile=fopen("FILE:Area.bbs","rb")) {

        FileAreaList_list=AllocPooled(FileAreaList_pool,sizeof(struct List));
        NewList((struct List *)FileAreaList_list);

        while(fread(&AreaName,sizeof(AreaName),1,DefsFile)) {
            namenode=AllocPooled(FileAreaList_pool,sizeof(struct DLGAreaNode));
            memcpy(&(namenode->nn_Area), (void *)&AreaName, sizeof(AreaName));
            sprintf((UBYTE *)&(namenode->nn_ListName), "%4d: %s", AreaName.Number, AreaName.Name);
            namenode->nn_Node.ln_Name=namenode->nn_ListName;
            namenode->nn_Node.ln_Type=NAMENODE_ID;
            namenode->nn_Node.ln_Pri=0;
            AddTail((struct List *)FileAreaList_list,(struct Node *)namenode);
            FileAreaPtrs[i++]=namenode;
        }
        fclose(DefsFile);
    }
}


void GetMsgAreaList(void) 
{
    FILE *DefsFile;
    void *poolAllocPtr;
    int i=0;
    struct DLGAreaNode *namenode;
    struct Msg_Area AreaName;

    MsgAreaList_pool=CreatePool(MEMPOOL,2048,2048);

    if (DefsFile=fopen("MSG:Area.bbs","rb")) {

        MsgAreaList_list=AllocPooled(MsgAreaList_pool,sizeof(struct List));
        NewList((struct List *)MsgAreaList_list);

        while(fread(&AreaName,sizeof(AreaName),1,DefsFile)) {
            namenode=AllocPooled(MsgAreaList_pool,sizeof(struct DLGAreaNode));
            memcpy(&(namenode->nn_Area), (void *)&AreaName, sizeof(AreaName));
            sprintf((UBYTE *)&(namenode->nn_ListName), "%4d: %s", AreaName.Number, AreaName.Name);
            namenode->nn_Node.ln_Name=namenode->nn_ListName;
            namenode->nn_Node.ln_Type=NAMENODE_ID;
            namenode->nn_Node.ln_Pri=0;
            AddTail((struct List *)MsgAreaList_list,(struct Node *)namenode);
            MsgAreaPtrs[i++]=namenode;
        }
        fclose(DefsFile);
    }
}




/**  T~~~~~:----(~)----.  ***
***  | T T | O| |~| |  |  ***
***  |_|_|_|__|_|_|_|__|  **/



void main() 
{
    long Sigs, RetVal;

    if (!SetupScreen()) {

        if (!OpenMain_Status_DisplayWindow()) {

            active_wnd=MAINWnd;
            main_mask=(1 << Main_Status_DisplayWnd->UserPort->mp_SigBit);
            all_mask=main_mask;

            AllocatePointerMem();
            GetDLGTerminalTypes();
            GetDLGArchiverTypes();
            GetDLGEditorTypes();
            GetDLGCharacterSetTypes();
            GetDLGMenuSetTypes();
            GetDLGTransferProtocols();
            GetBBSUsers();
            GetFileAreaList();
            GetMsgAreaList();

            do {
                Sigs=Wait(all_mask);

                if ((active_wnd & MAINWnd)  && (Sigs & main_mask))
                    RetVal=HandleMain_Status_DisplayIDCMP();
                
                if ((active_wnd & FILEWnd)  && (Sigs & fileman_mask)) {
                    if (!HandleFile_Area_ManagerIDCMP()) {
                        all_mask |= fileman_mask; all_mask ^= fileman_mask;
                        active_wnd |= FILEWnd;    active_wnd ^= FILEWnd;
                        CloseFile_Area_ManagerWindow();
                        GT_SetGadgetAttrs(Main_Status_DisplayGadgets[GD_FileAreaSelect_button],Main_Status_DisplayWnd,NULL, GA_Disabled,FALSE, TAG_DONE);
                    }
                }
                if ((active_wnd & FILETemp) && (Sigs & filetemp_mask)) {
                    if (!HandleFileAreaTemplateIDCMP()) {
                        all_mask |= filetemp_mask; all_mask ^= filetemp_mask;
                        active_wnd |= FILETemp;    active_wnd ^= FILETemp;
                        CloseFileAreaTemplateWindow();
                    }
                }

                if ((active_wnd & MSGWnd)   && (Sigs & msgman_mask)) {
                    if (!HandleMessage_Area_ManagerIDCMP()) {
                        all_mask |= msgman_mask; all_mask ^= msgman_mask;
                        active_wnd |= MSGWnd;   active_wnd ^= MSGWnd;
                        CloseMessage_Area_ManagerWindow();
                        GT_SetGadgetAttrs(Main_Status_DisplayGadgets[GD_MessageAreaSelect_button],Main_Status_DisplayWnd,NULL, GA_Disabled,FALSE, TAG_DONE);
                    }
                }
                if ((active_wnd & MSGTemp)  && (Sigs & msgtemp_mask)) {
                    if (!HandleMsgAreaTemplateIDCMP()) {
                        all_mask |= msgtemp_mask; all_mask ^= msgtemp_mask;
                        active_wnd |= MSGTemp;   active_wnd ^= MSGTemp;
                        CloseMsgAreaTemplateWindow();
                    }
                }

                if ((active_wnd & USERWnd)  && (Sigs & usered_mask)) {
                    if (!HandleUser_EditorIDCMP()) {
                        all_mask |= usered_mask; all_mask ^= usered_mask;
                        active_wnd |= USERWnd;   active_wnd ^= USERWnd;
                        CloseUser_EditorWindow();
                        GT_SetGadgetAttrs(Main_Status_DisplayGadgets[GD_UserEditorSelect_button],Main_Status_DisplayWnd,NULL, GA_Disabled,FALSE, TAG_DONE);
                    }
                }
            } while(RetVal);

            DeletePool(UserTerminal_pool);
            DeletePool(UserArchiver_pool);
            DeletePool(UserEditor_pool);
            DeletePool(UserCharSet_pool);
            DeletePool(UserMenuSet_pool);
            DeletePool(UserTransferProtocol_pool);
            DeletePool(UserUserList_pool);
            DeletePool(FileAreaList_pool);
            DeletePool(MsgAreaList_pool);
            FreePointerMem();
        }
        if (active_wnd & FILETemp)  CloseFileAreaTemplateWindow();
        if (active_wnd & MSGTemp)   CloseMsgAreaTemplateWindow();
        if (active_wnd & FILEWnd)   CloseFile_Area_ManagerWindow();
        if (active_wnd & MSGWnd)    CloseMessage_Area_ManagerWindow();
        if (active_wnd & USERWnd)   CloseUser_EditorWindow();
        if (active_wnd & MAINWnd)   CloseMain_Status_DisplayWindow();
    }
    CloseDownScreen();
}

