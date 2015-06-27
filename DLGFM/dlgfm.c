#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <clib/dos_protos.h>
#include <dlg/dlg.h>

const char version[]="\0$VER: DLGFM 0.96 (18.11.94)";

/// Structures

    struct  newFile_Header {
        char    From[36];
        char    Filename[36];
        char    Date[20];
        SHORT   Times_Downloaded;
        long    Size;
        SHORT   Attribute;
        char    Body[20000];
    };
    struct newFile_Header fdstruct;

    struct QuickFile fbstruct;

    struct FileUsersStruct {
        char    User[32];
        char    Stuff1[4];
        long    Pointer;
        char    Stuff2[4];
    };
    struct FileUsersStruct fustruct;

///
/// Variables

char searchpath[60]={0};

    int  file_area=0,
         user_area=0,
         reportlevel=0,
         low_pointer=0,
         high_pointer=0,
         fd_filesize=0,
         filenumber=1;

    FILE *fd_handle,
         *fu_handle,
         *fb_handle;

///
/// Prototypes

    void short_listing(void);
    void rebuild_file(void);
    void save_fd2comment(void);
    void add_displayme(void);
    void list_users(void);
    void touch_files(void);
    void kill_orphans(void);
    void file_aliases(void);
    int  GetFilePointers(void);
    int  read_fd(int);
    int  kill_fd(int);
    int  write_fd(int);

///

/*******************
***-              -*
**- PROGRAM CODE -**
*-              -***
*******************/

void main(argc,argv) int argc; char *argv[]; {

    int c_arg=1,
        mystery_arg=0;

    if ((argc>2)&&(atoi(argv[1]))) {

        // If command line arguments are provided then do stuff

        file_area=atoi(argv[1]);

        // Scan argument list for valid options and do them sequentially

        while ((++c_arg)<argc) {

            strupr(argv[c_arg]);

            if      (!strcmp(argv[c_arg],"LIST"))     short_listing();
            else if (!strcmp(argv[c_arg],"REBUILD"))  rebuild_file();
            else if (!strcmp(argv[c_arg],"KILLFD"))   kill_orphans();
            else if (!strcmp(argv[c_arg],"TOUCH"))    touch_files();
            else if (!strcmp(argv[c_arg],"COMMENT"))  save_fd2comment();
            else if (!strcmp(argv[c_arg],"ACCESS"))   list_users();
            else if (!strcmp(argv[c_arg],"FALIAS"))   file_aliases();
            else mystery_arg=1;
        };
        if (mystery_arg) {
            printf("\n");
            printf("An unknown option was encountered!\n");
            printf("Enter %s without any arguments for help.\n",argv[0]);
            printf("\n");
        }
    } else {

        // No arguments, give help

        printf("\n");
        printf("DLG File Manager 0.96b © 1994 Kevin Phair (FIDONET 2:263/150.6)\n");
        printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        printf("Usage: %s Area [options]\n\n",argv[0]);

        printf("Options: LIST    File list of area (Number, Filename, Uploader)\n");
        printf("         REBUILD Rebuild file.dat from existing .fd files\n");
        printf("         KILLFD  Kill orphaned .fd files\n");
        printf("         TOUCH   Date-stamp files using date in .fd\n");
        printf("         COMMENT Save file description into file comment\n");
        printf("         ACCESS  List users with access to area\n");
        printf("         FALIAS  Set all uploader names in area to aliases\n");
        printf("\n");
        printf("Options take effect in the order they are entered.\n");
        printf("\n");
    }
    exit(0);
}

/// GET FILE POINTERS

    int GetFilePointers(void) {

        char *filename="FILE:9999/Pointers.file";
        FILE *pointer_file;

        sprintf(filename,"FILE:%d/Pointers.file",file_area);
        if (pointer_file=fopen(filename,"rt")) {
            fscanf(pointer_file,"%d\n%d",&low_pointer,&high_pointer);
            fclose(pointer_file);
            return(1);
        } else return(0);
    }

///
/// SHORT FILE LISTING

    void short_listing() {

        printf("SHORT LISTING OF FILES IN AREA: %4d\n",file_area);
        printf("------------------------------------\n");

        GetFilePointers();

        for (filenumber=low_pointer; filenumber<=high_pointer; filenumber++) {
            if (read_fd(filenumber)) {
                printf("File: %4d : %-30.30s %.30s\n", filenumber, fdstruct.Filename, fdstruct.From);
            }
        }
    }

///
/// REBUILD FILE DATABASE

    void rebuild_file() {

        char *FileDatName="FILE:9999/file.dat";

        printf("REBUILDING QUICK LISTING FOR AREA: %4d\n",file_area);
        printf("---------------------------------------\n");

        sprintf(FileDatName,"FILE:%d/file.dat",file_area);
        if (fb_handle=fopen(FileDatName,"wb")) {

            GetFilePointers();

            for (filenumber=low_pointer; filenumber<=high_pointer; filenumber++) {
                if (read_fd(filenumber)) {

                    printf("Rebuilding from file: %5d : %-30.30s\r",
                        filenumber,fdstruct.Filename);

                    setmem(&fbstruct,sizeof(fbstruct),0);
                    fbstruct.number=filenumber;
                    fbstruct.size=fdstruct.Size;
                    strncpy(fbstruct.filename,fdstruct.Filename,35);
                    strncpy(fbstruct.desc,fdstruct.Body, 59);

                    fwrite(&fbstruct,sizeof(fbstruct),1,fb_handle);
                }
            }
            fclose(fb_handle);

            /* Done! */
            printf("\nFile area %d rebuilt.\n\n",file_area);

        } else {
            printf("FATAL ERROR creating FILE:%d/file.dat\n\n",file_area);
            exit(30);
        }
    }

///
/// SAVE FILE DESCRIPTION INTO FILE COMMENT

    void save_fd2comment() {

        char newcomment[116];
        char *Archivename="FILE:9999/123456789012345678901234567890";
        char *commentsrc;
        char *commentptr;
        int i;

        printf("SAVING DESCRIPTIONS INTO FILE COMMENT FOR AREA: %4d\n",file_area);
        printf("----------------------------------------------------\n");

        GetFilePointers();

        for (filenumber=low_pointer; filenumber<=high_pointer; filenumber++) {
            if (read_fd(filenumber)) {

                /* Move file description into file's comment */
                commentptr=newcomment;
                commentsrc=fdstruct.Body;

                /* Do it byte by byte to weed out strange characters */
                for (i=0;i<79;i++) {
                    *commentptr=*commentsrc;
                    *(commentptr+1)=0;
                    if (*commentptr==0) break;
                    if (*commentptr<32) *commentptr=32;
                    ++commentptr;commentsrc++;
                }

                sprintf(Archivename,"FILE:%d/%s",file_area,fdstruct.Filename);
                SetComment(Archivename, newcomment);

                printf("Commented file %d (%.30s) as %.30s...\n", filenumber, fdstruct.Filename, newcomment);
            }
        }
    }

///
/// LIST USERS WITH ACCESS TO FILE AREA

    void list_users() {
        char *userfile="FILE:9999/User.file";

        printf("LISTING OF USERS WITH ACCESS TO FILE AREA: %4d\n",file_area);
        printf("-----------------------------------------------\n");

        /* Open user access data file */
        sprintf(userfile,"FILE:%d/User.file",file_area);
        if (fu_handle=fopen(userfile,"rb")) {
            while (fread(&fustruct,sizeof(fustruct),1,fu_handle)) printf("%.30s\n",fustruct.User);
            fclose(fu_handle);
        }
    }

///
/// TOUCH FILE DATE AND TIME USING .FD

    void touch_files() {
        printf("RESETTING FILE DATES AND TIMES IN FILE AREA: %4d\n",file_area);
        printf("-------------------------------------------------\n");
    }

///
/// KILL ORPHANED .FD FILES

    void kill_orphans() {
        FILE *tempfile;
        char *Archivename="FILE:9999/123456789012345678901234567890";

        printf("KILLING ORPHANED .FD FILES IN FILE AREA: %4d\n",file_area);
        printf("---------------------------------------------\n");


        GetFilePointers();

        for (filenumber=low_pointer; filenumber<=high_pointer; filenumber++) {
            if (read_fd(filenumber)) {
                sprintf(Archivename,"FILE:%d/%s",file_area,fdstruct.Filename);
                if (tempfile=fopen(Archivename,"rb")) {
                    fclose(tempfile);
                } else {
                    printf("Found orphan: %d.fd, killing it... ",filenumber);
                    if (kill_fd(filenumber))
                        printf("ERROR!\n");
                    else
                        printf("Done\n");
                }
            }
        }
        printf("\n");
    }
///
/// CHANGE USER NAMES TO ALIASES

    void file_aliases() {

        char *UserFile="USER:123456789012345678901234567890/user.data";
        struct USER_DATA UserData;
        int i;
        FILE *userfile;

        printf("CHANGING USER NAMES TO ALIASES IN AREA: %4d\n",file_area);
        printf("--------------------------------------------\n");

        GetFilePointers();

        for (filenumber=low_pointer; filenumber<=high_pointer; filenumber++) {
            if (read_fd(filenumber)) {
                sprintf(UserFile,"USER:%s/user.data",fdstruct.From);
                for(i=0;i<30;i++) if (UserFile[i]==32) UserFile[i]='_';
                if (userfile=fopen(UserFile,"rb")) {
                    setmem(&UserData,sizeof(struct USER_DATA),0);
                    if (fread(&UserData,sizeof(struct USER_DATA),1,userfile)) {
                        printf("Changing %s to %s for file %d: %s\n",fdstruct.From,UserData.Alias,filenumber,fdstruct.Filename);
                        strcpy(fdstruct.From,UserData.Alias);
                        write_fd(filenumber);
                    }
                    fclose(userfile);
                }
            }
        }
    }

///
/// Write a new description file from buffer

    int write_fd(file) {
        char *filename="FILE:9999/9999.fd";
        FILE *fd_file;

        sprintf(filename,"FILE:%d/%d.fd",file_area,file);
        if (fd_file=fopen(filename,"wb")) {
            fwrite(&fdstruct,fd_filesize,1,fd_file);
            fclose(fd_file);
            return(1);
        } else return(0);
    }
///
/// Open a description file and read it into the structure

    int read_fd(file) {
        char *filename="FILE:9999/9999.fd";
        FILE *fd_file;

        sprintf(filename,"FILE:%d/%d.fd",file_area,file);
        fd_filesize=0;
        if (fd_file=fopen(filename,"rb")) {
            setmem(&fdstruct,sizeof(fdstruct),0);
            fread(&fdstruct,sizeof(fdstruct),1,fd_file);
            fd_filesize=ftell(fd_file);
            fclose(fd_file);
            return(1);
        } else return(0);
    }
///
/// Kill a description file

    int kill_fd(file) {
        char *filename="FILE:9999/9999.fd";

        sprintf(filename,"FILE:%d/%d.fd",file_area,file);
        return (unlink(filename));
    }
///
