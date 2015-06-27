#include "DLG-Edit_GUI.h"
#include "DLG-Edit.proto"
#include "DLG-Edit_rev.h"
#include <DLG/dlg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#undef GLOBAL

#ifdef MAIN
#   define GLOBAL
#else
#   define GLOBAL extern
#endif

/*
 * Max number of users to read in (affects how much memory to allocate for
 * storing user name pointers.
 */
#define MaxUsers    3000


#define ProtocolMAXLEN 20

/*
 * Used to signify a name pointer in node structures
 */
#define NAMENODE_ID 100

/*
 * Important, we want to make sure that the listview nodes go into fast mem
 * and also that they are public so they don't get swapped out (being system
 * system structures and all)
 */
#define MEMPOOL     MEMF_FAST|MEMF_PUBLIC|MEMF_CLEAR

/*
 * Define constants used in file/message area management activity queueing
 */
#define MAKEAREA    1
#define KILLAREA    2
#define RENUMAREA   3
#define SWAPAREA    4
#define USERCOPY    5

/*
 * Define constants used to describe the windows for status checking
 */
#define MAINWnd     0x0001

#define FILEWnd     0x0010
#define FILETemp    0x0020
#define FILEAccess  0x0040
#define ALLFile     FILEWnd | FILETemp | FILEAccess

#define MSGWnd      0x0100
#define MSGTemp     0x0200
#define MSGAccess   0x0400
#define ALLMsg      MSGWnd | MSGTemp | MSGAccess

#define USERWnd     0x1000
#define USERMore    0x2000
#define ALLUser     USERWnd | USERMore

#define ALLWindows  MAINWnd | ALLFile | ALLMsg | ALLUser

/*
 * Version information
 */
GLOBAL const char *myVersionTag;
GLOBAL const char *myVersionString;

GLOBAL struct EasyStruct AboutReq;
GLOBAL struct EasyStruct AreaExistsReq;
GLOBAL struct EasyStruct ImportAreaExistsReq;
GLOBAL struct EasyStruct BadImportFileReq;
GLOBAL struct EasyStruct ConfirmTemplateReq;
/*
 * The requesters are used to block gadget processing when the template
 * windows are opened for the appropriate sections
 */
GLOBAL struct Requester MainStatusDisplayBlock_Req;
GLOBAL struct Requester FileAreaManagerBlock_Req;
GLOBAL struct Requester FileAreaTemplateBlock_Req;
GLOBAL struct Requester FileUserAccessBlock_Req;
GLOBAL struct Requester MsgAreaManagerBlock_Req;
GLOBAL struct Requester MsgAreaTemplateBlock_Req;
GLOBAL struct Requester MsgUserAccessBlock_Req;
GLOBAL struct Requester UserEditorBlock_Req;
/*
 * Memory pool pointers
 */
GLOBAL void *UserTerminal_pool;
GLOBAL void *UserArchiver_pool;
GLOBAL void *UserEditor_pool;
GLOBAL void *UserCharSet_pool;
GLOBAL void *UserMenuSet_pool;
GLOBAL void *UserTransferProtocol_pool;
GLOBAL void *UserUserList_pool;
GLOBAL void *FileAreaList_pool;
GLOBAL void *FileTempSrcList_pool;
GLOBAL void *FileTempTargList_pool;
GLOBAL void *FileAccessAreaSrcList_pool;
GLOBAL void *FileAccessAreaTargList_pool;
GLOBAL void *FileAccessUserSrcList_pool;
GLOBAL void *FileAccessUserTargList_pool;
GLOBAL void *MsgAreaList_pool;
GLOBAL void *MsgTempSrcList_pool;
GLOBAL void *MsgTempTargList_pool;
GLOBAL void *MsgAccessUserSrcList_pool;
GLOBAL void *MsgAccessUserTargList_pool;
GLOBAL void *MsgAccessAreaSrcList_pool;
GLOBAL void *MsgAccessAreaTargList_pool;
/*
 * Cycle gadget option list pointers
 */
GLOBAL UBYTE **UserTerminal_options;
GLOBAL UBYTE **UserArchiver_options;
GLOBAL char   *UserArchiver_numbers;
GLOBAL UBYTE **UserEditor_options;
GLOBAL char   *UserEditor_numbers;
GLOBAL UBYTE **UserCharSet_options;
GLOBAL char   *UserCharSet_numbers;
GLOBAL UBYTE **UserMenuSet_options;
GLOBAL char   *UserMenuSet_numbers;
GLOBAL UBYTE **UserTransfer_options;
GLOBAL char   *UserTransfer_letters;
/*
 * Cycle gadget list for user levels
 */
GLOBAL UBYTE **AccessLevelLabels;
/*
 * String pointers for use in listviews
 */
GLOBAL char **UserNamePtrs;
GLOBAL struct UserNameNode **UserNodePtrs;
GLOBAL struct DLGAreaNode **FileAreaPtrs;
GLOBAL struct Node **FileTempSrcPtrs;
GLOBAL struct Node **FileTempTargPtrs;
GLOBAL struct Node **FileAccessAreaSrcPtrs;
GLOBAL struct Node **FileAccessAreaTargPtrs;
GLOBAL struct UserNameNode **FileAccessUserSrcPtrs;
GLOBAL struct UserNameNode **FileAccessUserTargPtrs;
GLOBAL struct DLGAreaNode **MsgAreaPtrs;
GLOBAL struct Node **MsgTempSrcPtrs;
GLOBAL struct Node **MsgTempTargPtrs;
GLOBAL struct Node **MsgAccessAreaSrcPtrs;
GLOBAL struct Node **MsgAccessAreaTargPtrs;
GLOBAL struct UserNameNode **MsgAccessUserSrcPtrs;
GLOBAL struct UserNameNode **MsgAccessUserTargPtrs;
/*
 * Listview list pointers
 */
GLOBAL struct List UserListView_list;
GLOBAL struct List FileAreaList_list;
GLOBAL struct List FileTempSrcList_list;
GLOBAL struct List FileTempTargList_list;
GLOBAL struct List FileAccessUserSrcList_list;
GLOBAL struct List FileAccessUserTargList_list;
GLOBAL struct List FileAccessAreaSrcList_list;
GLOBAL struct List FileAccessAreaTargList_list;
GLOBAL struct List MsgAreaList_list;
GLOBAL struct List MsgTempSrcList_list;
GLOBAL struct List MsgTempTargList_list;
GLOBAL struct List MsgAccessUserSrcList_list;
GLOBAL struct List MsgAccessUserTargList_list;
GLOBAL struct List MsgAccessAreaSrcList_list;
GLOBAL struct List MsgAccessAreaTargList_list;
/*
 * Signal masks for window messages
 */
GLOBAL SHORT my_sigbits;
GLOBAL ULONG all_mask,
             main_mask,
             fileman_mask,filetemp_mask,fileaccess_mask,
             msgman_mask,msgtemp_mask,msgaccess_mask,
             usered_mask;
/*
 * For keeping track of which windows are open
 */
GLOBAL SHORT active_wnd;

GLOBAL UBYTE *AppFontName;
GLOBAL UWORD AppFontXSize;
GLOBAL UWORD AppFontYSize;

typedef struct NameNode {
               struct Node nn_Node;
               UBYTE  nn_Name[36];
        };

typedef struct UserNameNode {
               struct Node nn_Node;
               UBYTE  nn_Name[36];
               ULONG  nn_Number;
        };

typedef struct DLGAreaNode {
               struct Node nn_Node;
               struct Msg_Area nn_Area;
               UBYTE  nn_ListName[42];
        };
