#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libraries/dos.h>
#include <dos.h>
#include <clib/dos_protos.h>

/// Structures

    struct File_Header {
        char    From[36];           /* The user who uploaded the file */
        char    Filename[36];       /* Filename of the file this is describing */
        char    Date[20];           /* Text string of the date the file was uploaded */
        SHORT   Times_Downloaded;   /* Number of downloads the file has had */
        long    Size;               /* Size of the file in bytes */
        SHORT   Attribute;          /* See attributes below */
        char    Desc[10000];
    };
    struct File_Header fdstruct;

    // Flags for the attribute field of the file header
    #define NORATIO   1         /* No file ratio will be imposed on this file */
    #define UNVALIDATED 2       /* This is an unvalidated file */

    /* Quick file index

     The following structure must be written 'alphabetically on the first field'
     into an index file called 'file.dat' in each public file area when a file is
     added.  Also must be deleted when the file is removed.  Applicable fields
     must also be modified in the index file when the filename, size, date, or
     description changes.

     All utilities that place a file into a public file area must be modified to
     update this file. (Private files need not worry about this index file).
    */
    struct QuickFile {
        char    Filename[36];
        long    Date;         /* number of seconds since Jan 1, 1978 */
        SHORT   Number;       /* File number (.fd file number)*/
        long    Size;         /* size of file in bytes */
        char    Desc[60];     /* 60 character of the description */
    };
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
         list_area=0,
         reportlevel=0,
         filenumber=1;

    char current_dir[255];

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
    void cd_area(int);
    int  read_fd(int);
    int  kill_fd(int);
    void fatal_error(char *);

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
        if (getcd(0,current_dir)) {
            printf("FATAL ERROR retrieving current directory");
            exit(1);
        }
        cd_area(file_area);

        // Scan argument list for valid options and do them sequentially

        while ((++c_arg)<argc) {

            strupr(argv[c_arg]);

            if (!strcmp(argv[c_arg],"LIST")) {
                short_listing();
                break;
            }
            if (!strcmp(argv[c_arg],"REBUILD")) {
                rebuild_file();
                break;
            }
            if (!strcmp(argv[c_arg],"KILLFD")) {
                kill_orphans();
                break;
            }
            if (!strcmp(argv[c_arg],"TOUCH")) {
                touch_files();
                break;
            }
            if (!strcmp(argv[c_arg],"COMMENT")) {
                save_fd2comment();
                break;
            }
            if (!strcmp(argv[c_arg],"ACCESS")) {
                list_users();
                break;
            }
            mystery_arg=1;
        };
        if (mystery_arg) {
            printf("\n");
            printf("An unknown option was encountered!\n");
            printf("Enter %s without any arguments for help.\n",argv[0]);
            printf("\n");
        }
        chdir(current_dir);
    } else {

        // No arguments, give help

        printf("\n");
        printf("DLG File Manager 0.91b © 1994 Kevin Phair (FIDONET 2:263/150.6)\n");
        printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        printf("Usage: %s Area [options]\n\n",argv[0]);

        printf("Options: LIST    File list of area (Number, Filename, Uploader)\n");
        printf("         REBUILD Rebuild file.dat from existing .fd files\n");
        printf("         KILLFD  Kill orphaned .fd files\n");
        printf("         TOUCH   Date-stamp files using date in .fd\n");
        printf("         COMMENT Save file description into file comment\n");
        printf("         ACCESS  List users with access to area\n");
        printf("\n");
        printf("Options take effect in the order they are entered.\n");
        printf("\n");
    }
}


/// SHORT FILE LISTING

    void short_listing() {

        printf("SHORT LISTING OF FILES IN AREA: %d\n",file_area);
        printf("-------------------------------------\n");

        filenumber=1;

        while (read_fd(filenumber))
            printf("File: %5d : %-30.30s %.30s\n", filenumber++, fdstruct.Filename, fdstruct.From);
    }

///
/// REBUILD FILE DATABASE

    void rebuild_file() {

        if (fb_handle=fopen("file.dat","wb")) {

            filenumber=1;
            while (read_fd(filenumber)) {

                printf("Rebuilding from file: %5d : %-30.30s\r",
                    filenumber,fdstruct.Filename);

                setmem(&fbstruct,sizeof(fbstruct),0);
                fbstruct.Number=filenumber;
                fbstruct.Size=fdstruct.Size;
                memcpy(fbstruct.Filename,fdstruct.Filename,36);
                memcpy(fbstruct.Desc,fdstruct.Desc,    60);

                fwrite(&fbstruct,sizeof(fbstruct),1,fb_handle);

                filenumber++;
            }
            fclose(fb_handle);

            /* Create new pointer settings */
            fb_handle=fopen("Pointers.file","wt");
            fprintf(fb_handle,"1\x0a%d\x0a",filenumber-1);
            fclose(fb_handle);

            /* Done! */
            printf("\nFile area %d rebuilt.\n\n",list_area);

        } else fatal_error("FATAL ERROR creating file.dat\n\n");
    }

///
/// SAVE FILE DESCRIPTION INTO FILE COMMENT

    void save_fd2comment() {

        char newcomment[116];
        char *commentsrc;
        char *commentptr;
        int i;

        filenumber=1;
        while (read_fd(filenumber)) {

            /* Move file description into file's comment */
            commentptr=newcomment;
            commentsrc=fdstruct.Desc;

            /* Do it byte by byte to weed out strange characters */
            for (i=0;i<79;i++) {
                *commentptr=*commentsrc;
                *(commentptr+1)=0;
                if (*commentptr==0) break;
                if (*commentptr<32) *commentptr=32;
                ++commentptr;commentsrc++;
            }
          
            SetComment(fdstruct.Filename, newcomment);

            printf("Commented file %d (%.30s) as %.30s...\n", filenumber++, fdstruct.Filename, newcomment);
        }
    }

///
/// LIST USERS WITH ACCESS TO FILE AREA

    void list_users() {

        printf("LISTING OF USERS WITH ACCESS TO FILE AREA: %d\n",file_area);
        printf("------------------------------------------------\n");

        /* Open user access data file */
        fu_handle=fopen("user.file","rb");

        while (fread(&fustruct,sizeof(fustruct),1,fu_handle)) {
            printf("%.30s\n",fustruct.User);
        }

        fclose(fu_handle);
    }

///
/// TOUCH FILE DATE AND TIME USING .FD

    void touch_files() {
        printf("RESETTING FILE DATES AND TIMES IN FILE AREA: %d\n",file_area);
        printf("- Not implemented yet ----------------------------\n");
        printf("--------------------------------------------------\n");
    }

///
/// KILL ORPHANED .FD FILES

    void kill_orphans() {
        FILE *tempfile;

        printf("KILLING ORPHANED .FD FILES IN FILE AREA %d\n",file_area);

        filenumber=1;
        while(read_fd(filenumber)) {

            if (tempfile=fopen(fdstruct.Filename,"rb"))
                printf("%d.fd has matching file\r",filenumber);
            else {
                fclose(tempfile);
                printf("Found orphan: %d.fd, killing it... ",filenumber);
                if (kill_fd(filenumber)) 
                    printf("ERROR!\n"); 
                else 
                    printf("OK\n");
            }
            filenumber++;
        }
        printf("\n");
    }
///

/// Change directory to a file area

    void cd_area(int area) {
        char area_path[16];

        setmem(area_path,sizeof(area_path),0);
        strcpy(area_path,"File:");

        stci_d((area_path+5),area);

        if(chdir(area_path))
            fatal_error("FATAL ERROR changing directory to file area.\n");

    }

///
/// Open a description file and read it into the structure

    int read_fd(file) {
        char filename[16];
        FILE *fd_file;

        setmem(filename,sizeof(filename),0);
        strcpy((filename+stci_d(filename,file)),".fd");
        if (fd_file=fopen(filename,"rb")) {
            setmem(&fdstruct,sizeof(fdstruct),0);
            fread(&fdstruct,sizeof(fdstruct),1,fd_file);
            fclose(fd_file);
            return(1);
        } else return(0);
    }
///
/// Kill a description file

    int kill_fd(file) {
        char filename[16];

        setmem(filename,sizeof(filename),0);
        strcpy((filename+stci_d(filename,file)),".fd");
        return (unlink(filename));
    }
///
/// Exit to DOS with a fatal error string

void fatal_error(char *errstring) {

    printf(errstring);
    chdir(current_dir);
    exit(2);
}
///

