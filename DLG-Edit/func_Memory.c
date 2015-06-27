/*
 * Memory allocation and deallocation module
 */

#include "DLG-Edit.h"

/// General memory handling
/*
 * Allocate the memory for cycle gadget pointers and area nodes (name member)
 */
void AllocatePointerMem(void) {

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
    UserTransfer_letters=     (char *)calloc(128,sizeof(char));
    AccessLevelLabels=      (UBYTE **)calloc(256,sizeof(ULONG));

    UserNamePtrs=           (char **)calloc(MaxUsers,sizeof(ULONG));
    UserNodePtrs=           (struct UserNameNode **)calloc(MaxUsers,sizeof(ULONG));
    FileAreaPtrs=           (struct DLGAreaNode **)calloc(10000,sizeof(ULONG));
    MsgAreaPtrs=            (struct DLGAreaNode **)calloc(10000,sizeof(ULONG));
}

/*
 * Free memory allocated in the above function
 */
void FreePointerMem(void) {

    free(MsgAreaPtrs);
    free(FileAreaPtrs);
    free(UserNodePtrs);
    free(UserNamePtrs);

    free(AccessLevelLabels);
    free(UserTransfer_letters);
    free(UserTransfer_options);
    free(UserMenuSet_numbers);
    free(UserMenuSet_options);
    free(UserCharSet_numbers);
    free(UserCharSet_options);
    free(UserEditor_numbers);
    free(UserEditor_options);
    free(UserArchiver_numbers);
    free(UserArchiver_options);
    free(UserTerminal_options);
}
///

/// List copying for access functions
/*
 * Copy the file area list to the two lists used by the file area template
 * functions.
 */
void CopyAccessLists(void)
{
    int i=0;
    struct DLGAreaNode *sourcenode;
    struct Node *destnode;
    struct UserNameNode *usernamenode;
    struct UserNameNode *userdestnode;

    /*
     * Initialise lists
     */
    NewList(&FileAccessAreaSrcList_list);  FileAccessAreaSrcList_pool=CreatePool(MEMPOOL,sizeof(struct Node)*10,sizeof(struct Node));
    NewList(&FileAccessAreaTargList_list); FileAccessAreaTargList_pool=CreatePool(MEMPOOL,sizeof(struct Node)*10,sizeof(struct Node));
    NewList(&FileAccessUserSrcList_list);  FileAccessUserSrcList_pool=CreatePool(MEMPOOL,sizeof(struct Node)*10,sizeof(struct Node));
    NewList(&FileAccessUserTargList_list); FileAccessUserTargList_pool=CreatePool(MEMPOOL,sizeof(struct Node)*10,sizeof(struct Node));

    /*
     * Go through the file area list structure, making Nodes & copying the
     * area descriptions into the Node->ln_Name, and linking it into our
     * new list structure. Also fill array of pointers to the Node
     * structures
     */
    for (i=0; sourcenode=FileAreaPtrs[i]; i++) {

        destnode=AllocPooled(FileAccessAreaSrcList_pool,sizeof(struct Node));
        destnode->ln_Name=sourcenode->nn_ListName;
        destnode->ln_Type=NAMENODE_ID;
        destnode->ln_Pri=0;
        AddTail(&FileAccessAreaSrcList_list,destnode);
        FileAccessAreaSrcPtrs[i]=destnode;
        FileAccessAreaSrcPtrs[i+1]=NULL;
    }

    /*
     * Go through the user list structure, making duplicates and
     * linking the embedded node into the new list structure. Also
     * fill the array of pointers to the UserNameNode structures.
     */
    for (i=0; usernamenode=UserNodePtrs[i]; i++) {

        userdestnode=AllocPooled(FileAccessUserSrcList_pool,sizeof(struct UserNameNode));
        memcpy(userdestnode,usernamenode,sizeof(struct UserNameNode));
        AddTail(&FileAccessUserSrcList_list,&(userdestnode->nn_Node));
        FileAccessUserSrcPtrs[i]=userdestnode;
        FileAccessUserSrcPtrs[i+1]=NULL;
    }
}

///

/// Area list copying for template functions
/*
 * Copy the file area list to the two lists used by the file area template
 * functions.
 */
void CopyFileAreaList(void)
{
    int i=0;
    struct DLGAreaNode *sourcenode;
    struct Node *destnode;

    /*
     * Initialise lists
     */
    NewList(&FileTempSrcList_list);
    FileTempSrcList_pool=CreatePool(MEMPOOL,sizeof(struct Node)*10,sizeof(struct Node));
    NewList(&FileTempTargList_list);
    FileTempTargList_pool=CreatePool(MEMPOOL,sizeof(struct Node)*10,sizeof(struct Node));

    /*
     * Go through the file area list structure, copying the area descriptions
     * into our new list structure. Also fill pointer array.
     */
    for (i=0; sourcenode=FileAreaPtrs[i]; i++) {

        destnode=AllocPooled(FileTempSrcList_pool,sizeof(struct Node));
        destnode->ln_Name=sourcenode->nn_ListName;
        destnode->ln_Type=NAMENODE_ID;
        destnode->ln_Pri=0;
        AddTail(&FileTempSrcList_list,destnode);
        FileTempSrcPtrs[i]=destnode; 
        FileTempSrcPtrs[i+1]=NULL;
    }
}

/*
 * Copy the message area list to the two lists used by the message area template
 * functions.
 */
void CopyMsgAreaList(void)
{
    int i=0;
    struct DLGAreaNode *sourcenode;
    struct Node *destnode;

    /*
     * Initialise lists
     */

    NewList(&MsgTempSrcList_list);
    MsgTempSrcList_pool=CreatePool(MEMPOOL,sizeof(struct Node)*10,sizeof(struct Node));
    NewList(&MsgTempTargList_list);
    MsgTempTargList_pool=CreatePool(MEMPOOL,sizeof(struct Node)*10,sizeof(struct Node));

    /*
     * Go through the message area list structure, copying the area descriptions
     * into our new list structure. Also fill pointer array.
     */
    for (i=0; sourcenode=MsgAreaPtrs[i]; i++) {

        destnode=AllocPooled(MsgTempSrcList_pool,sizeof(struct Node));
        destnode->ln_Name=sourcenode->nn_ListName;
        destnode->ln_Type=NAMENODE_ID;
        destnode->ln_Pri=0;
        AddTail(&MsgTempSrcList_list,destnode);
        MsgTempSrcPtrs[i]=destnode;
        MsgTempSrcPtrs[i+1]=NULL;
    }
}
///

