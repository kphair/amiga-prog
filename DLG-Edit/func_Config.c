/*
 * Configuration management module
 */

#include "DLG-Edit.h"

/* Configuration writing */
/// Save file area
/*
 * Create a new area.bbs for the newly rearranged file areas
 */
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

        system("execute t:FileActionQueue");
        unlink("t:FileActionQueue");
        if (DefsFile=fopen("T:FileActionQueue","wt")) {
            fprintf(DefsFile,"failat 999\n");
            fclose(DefsFile);
        }
    }
}
///
/// Save message areas
/*
 * Create a new area.bbs for the newly rearranged message areas
 */
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

        system("execute t:MsgActionQueue");
        unlink("t:MsgActionQueue");
        if (DefsFile=fopen("T:MsgActionQueue","wt")) {
            fprintf(DefsFile,"failat 999\n");
            fclose(DefsFile);
        }
    }
}
///

/* Configuration reading */
/// Get terminal/computer types
/*
 * Read in the computer types from the config file and allocate them within
 * the memory pool for use by the cycle gadget
 */
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
///
/// Get archivers
/*
 * Read in the archiver types from the config file and allocate them within
 * the memory pool for use by the cycle gadget
 */
void GetDLGArchiverTypes(void) 
{
    FILE *DefsFile;
    char *poolAllocPtr;
    int i=0;
    struct Archiver ArchiverTypes;

    UserArchiver_pool=CreatePool(MEMPOOL,2048,2048);
    
    if (DefsFile=fopen("DLGConfig:Misc/Archivers.bbs","rb")) {

        while(fread(&ArchiverTypes,sizeof(ArchiverTypes),1,DefsFile)) {
            poolAllocPtr=AllocPooled(UserArchiver_pool,(ULONG)strlen(ArchiverTypes.extension)+1);
            strcpy(poolAllocPtr,ArchiverTypes.name);
            UserArchiver_numbers[i]=ArchiverTypes.number;
            UserArchiver_options[i++]=poolAllocPtr;
        }
        fclose(DefsFile);
    }
}
///
/// Get editors
/*
 * Read in the editor types from the config file and allocate them within
 * the memory pool for use by the cycle gadget
 */
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
///
/// Get character sets
/*
 * Read in the character set names from the config file and allocate them within
 * the memory pool for use by the cycle gadget
 */
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
///
/// Get menu sets
/*
 * Read in the menu set names from the config file and allocate them within
 * the memory pool for use by the cycle gadget
 */
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
///
/// Get transfer protocols
/*
 * Read in the transfer protocol names from the config file and allocate them within
 * the memory pool for use by the cycle gadget
 */
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
            poolAllocPtr[ProtocolMAXLEN]=0;
            UserTransfer_letters[i-1]=TransferTypes.letter;
            UserTransfer_options[i++]=poolAllocPtr;
        }
        fclose(DefsFile);
    }
}
///
/// Get users
/*
 * Read in the BBS user names from the config file and allocate them within
 * the memory pool for use by the listview
 */
void GetBBSUsers(void) 
{
    FILE *DefsFile;
    int i=0;
    struct UserNameNode *usernamenode;
    static char BBSUserName[36];

    UserUserList_pool=CreatePool(MEMPOOL,2048,2048);

    if (DefsFile=fopen("DLGConfig:Misc/Users.bbs","rb")) {

        NewList(&UserListView_list);

        /*
         * Read a user's name from the config file, allocate a
         * UserNameNode structure, and fill it, linking the
         * embedded Node structure into the user list structure.
         * Also places serial number into UserNameNode structure
         * for easier sorting.
         */

        while(fread(&BBSUserName,sizeof(BBSUserName),1,DefsFile)) {
            usernamenode=AllocPooled(UserUserList_pool,sizeof(struct UserNameNode));
            strcpy(usernamenode->nn_Name,BBSUserName);
            usernamenode->nn_Number=i;
            usernamenode->nn_Node.ln_Name=usernamenode->nn_Name;
            usernamenode->nn_Node.ln_Type=NAMENODE_ID;
            usernamenode->nn_Node.ln_Pri=0;
            AddTail(&UserListView_list,&(usernamenode->nn_Node));
            UserNodePtrs[i]=usernamenode;
            UserNamePtrs[i++]=usernamenode->nn_Name;
        }
        fclose(DefsFile);
    }
}
///
/// Get file areas
/*
 * Read in the file areas from the config file and allocate them within
 * the memory pool for use by the listview and other functions
 */
void GetFileAreaList(void) 
{
    FILE *DefsFile;
    int i=0;
    struct DLGAreaNode *namenode;
    struct Msg_Area AreaName;

    FileAreaList_pool=CreatePool(MEMPOOL,2048,2048);

    if (DefsFile=fopen("FILE:Area.bbs","rb")) {

        NewList(&FileAreaList_list);

        while(fread(&AreaName,sizeof(AreaName),1,DefsFile)) {
            namenode=AllocPooled(FileAreaList_pool,sizeof(struct DLGAreaNode));
            memcpy(&(namenode->nn_Area), (void *)&AreaName, sizeof(AreaName));
            sprintf((UBYTE *)&(namenode->nn_ListName), "%4d: %s", AreaName.Number, AreaName.Name);
            namenode->nn_Node.ln_Name=namenode->nn_ListName;
            namenode->nn_Node.ln_Type=NAMENODE_ID;
            namenode->nn_Node.ln_Pri=0;
            AddTail(&FileAreaList_list,(struct Node *)namenode);
            FileAreaPtrs[i++]=namenode;
        }
        fclose(DefsFile);
    }
}
///
/// Get message areas
/*
 * Read in the message areas from the config file and allocate them within
 * the memory pool for use by the listview and other functions
 */
void GetMsgAreaList(void) 
{
    FILE *DefsFile;
    int i=0;
    struct DLGAreaNode *namenode;
    struct Msg_Area AreaName;

    MsgAreaList_pool=CreatePool(MEMPOOL,2048,2048);

    if (DefsFile=fopen("MSG:Area.bbs","rb")) {

        NewList(&MsgAreaList_list);

        while(fread(&AreaName,sizeof(AreaName),1,DefsFile)) {
            namenode=AllocPooled(MsgAreaList_pool,sizeof(struct DLGAreaNode));
            memcpy(&(namenode->nn_Area), (void *)&AreaName, sizeof(AreaName));
            sprintf((UBYTE *)&(namenode->nn_ListName), "%4d: %s", AreaName.Number, AreaName.Name);
            namenode->nn_Node.ln_Name=namenode->nn_ListName;
            namenode->nn_Node.ln_Type=NAMENODE_ID;
            namenode->nn_Node.ln_Pri=0;
            AddTail(&MsgAreaList_list,(struct Node *)namenode);
            MsgAreaPtrs[i++]=namenode;
        }
        fclose(DefsFile);
    }
}
///

