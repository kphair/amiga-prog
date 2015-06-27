/*
 * Program to update .next files in Usenet newsgroup directory hierarchy.
 * Optionally strips out unneccessary information from the message header.
 * Can also kill articles by poster, with posters being listed in a killfile.
 *
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <exec/types.h>

#define BUFFERSIZE 16384

const char version[]="\0$VER: NewsUpdate 2.2 (29.10.95)";

char *ansi_1="\x00[31m";
char *ansi_2="\x00[32m";
char *ansi_3="\x00[33m";

/* Matches particular header fields and returns TRUE if found
 */
int matchXlines(char *tempbuff) {
    if (!strncmp(tempbuff,"From:",5)
     || !strncmp(tempbuff,"Subject:",8)
     || !strncmp(tempbuff,"Date:",5)
     || !strncmp(tempbuff,"Organization:",13)
     || !strncmp(tempbuff,"Organisation:",13)
     || !strncmp(tempbuff,"Message-ID:",11)
     || !strncmp(tempbuff,"References:",11))
        return(TRUE);
    else
        return(FALSE);
}

void main (int argc, char **argv) {

    FILE *infile,
         *nfile,
         *afile,
         *kfile,
         *tfile;


    char tempbuff[BUFFERSIZE],
         *tempptr,
         *killptr[3000],
         nextfile[512],
         articlefile[512],
         newsgroup[512],
         groupname[512];

    long article,i,
         highpointer,lowpointer,
         c_arg=1,
         buflen;

    char groupflag;

    int verb=1;

    BOOL processbin=TRUE;
    BOOL mysteryarg=FALSE;
    BOOL cleanarticles=FALSE;
    BOOL killfile=FALSE;
    BOOL killit=FALSE;

    if (argc<2) {
        printf("\nPlease specify the active newsgroup list!\n\n");
        printf("Usage: %s active-file [QUIET] [NOISY] [KILL] [CLEAN] [NOBIN] [COLOUR]\n\n",argv[0]);
        goto userhelp;
    }

    while ((++c_arg)<argc) {

        strupr(argv[c_arg]);

        if      (!strcmp(argv[c_arg],"QUIET")) verb=0;
        else if (!strcmp(argv[c_arg],"NOISY")) verb=2;
        else if (!strcmp(argv[c_arg],"KILL")) killfile=TRUE;
        else if (!strcmp(argv[c_arg],"NOBIN")) processbin=FALSE;
        else if (!strcmp(argv[c_arg],"CLEAN")) cleanarticles=TRUE;
        else if (!strcmp(argv[c_arg],"COLOUR")) { *ansi_1='\x1b'; *ansi_2='\x1b'; *ansi_3='\x1b'; }
        else if (!strcmp(argv[c_arg],"COLOR")) { *ansi_1='\x1b'; *ansi_2='\x1b'; *ansi_3='\x1b'; }
        else {
            printf("Unrecognised option: %s\n",argv[c_arg]);
            mysteryarg=TRUE;
        }
    };
    if(mysteryarg==TRUE) {

        printf("\n%sUnrecognised parameters were encountered!%s\n\n",ansi_2,ansi_1);

    userhelp:
        printf("Valid options are:\n");
        printf("%s------------------\n",ansi_3);
        printf("%s    QUIET   %s No program output (except errors)\n",ansi_2,ansi_1);
        printf("%s    NOISY   %s List articles as they are cleaned/killed\n",ansi_2,ansi_1);
        printf("%s    COLOUR  %s User ANSI colour codes in output (also COLOR)\n",ansi_2,ansi_1);
        printf("%s    KILL    %s Use UULib:KillFile.From to kill articles\n",ansi_2,ansi_1);
        printf("%s    CLEAN   %s Clean article headers\n",ansi_2,ansi_1);
        printf("%s    NOBIN   %s Do not clean in 'binaries' newsgroups\n\n",ansi_2,ansi_1);

        printf("Normal program activity is to list each newsgroup and number\n");
        printf("of new articles, and then update the .next pointer.\n\n");
        printf("%sNewsUpdate%s - © 1994-1995 Kevin J.Phair (Kev@Scorpio.ie)\n\n",ansi_3,ansi_1);
        exit(10);
    }

    if (!(infile=fopen(argv[1],"rb"))) {
        printf("\n%sError opening active list!%s\n\n",ansi_2,ansi_1);
        exit(20);
    }

    if (killfile) {
        if (kfile=fopen("UULib:KillFile.From","rb")) {

            i=0;
            if (verb) printf("\n%sScanning UULib:KillFile.From to get unwanted posters list.%s\n",ansi_2,ansi_1);

            while (!feof(kfile)) {
                *(tempbuff)=0;
                fgets(tempbuff,BUFFERSIZE,kfile);
                if (strchr(tempbuff,'\n')) *(strchr(tempbuff,'\n'))=0;
                if (strlen(tempbuff)) {
                    killptr[i]=calloc(strlen(tempbuff)+1,sizeof(char));
                    strcpy(killptr[i++],tempbuff);
                    killptr[i]=NULL;
                    if (verb>1) printf("    %sGot %ld: %s%s%s\n",ansi_1,i,ansi_2,tempbuff,ansi_1);
                }
                else break;
            }
            fclose(kfile);

        } else {
            if (verb) printf("\n%sCouldn't open kill file! (%sUULib:KillFile.From%s)%s\n\n",ansi_2,ansi_1,ansi_2,ansi_1);
            exit(10);
        }
    }

    printf("Success getting killfile\n");

    while (!feof(infile)) {

        /* Parse in the group name and new high message pointer from the
         * active list file used by NNTPxfer
         */
        if (fscanf(infile,"%s %ld %ld %c\n",groupname,&highpointer,&lowpointer,&groupflag)==4) {

            sprintf(newsgroup,"UUNews:%s/",groupname);

            // Convert the group name to a path
            for(tempptr=newsgroup; *tempptr; tempptr++) if (*tempptr=='.') *tempptr='/';

            // Create file spec for the high message pointer
            sprintf(nextfile,"%s.next",newsgroup);

            // Read in the message pointer (which at this stage is still
            // pointing to the last highest message)
            if (nfile=fopen(nextfile,"rb")) {
                fscanf(nfile,"%ld\n",&lowpointer);
                fclose(nfile);
            }

            if (verb) {
                if (lowpointer<highpointer) printf("%sArticles %s%ld%s to %s%ld%s [%s %s %s]%s\n",ansi_2,ansi_1,lowpointer,ansi_2,ansi_1,highpointer,ansi_2,ansi_1,groupname,ansi_2,ansi_1);
                else if (lowpointer==highpointer) printf("%sArticle %s%ld%s [%s %s %s]%s\n",ansi_2,ansi_1,lowpointer,ansi_2,ansi_1,groupname,ansi_2,ansi_1);
                else printf("%sNo new articles [%s %s %s]%s\n",ansi_3,ansi_1,groupname,ansi_3,ansi_1);
            }

            if (lowpointer<=highpointer) {


                /* This section handles scanning of messages for cleaning and
                 * optionally killing articles
                 */

                if((cleanarticles==TRUE) && (processbin==TRUE || strstr(groupname,"binaries")==NULL)) {

                    // Now we start processing messages!
                    for (article=lowpointer; article<=highpointer; article++) {
                        sprintf(articlefile,"%s%ld",newsgroup,article);
                        if (afile=fopen(articlefile,"rt")) {
                            if (tfile=fopen("t:NNTPUpdate.temp","wb")) {
                                if (verb>1) printf("\r    Cleaning article %s%ld%s",ansi_2,article,ansi_1);

                                /* Parse file filtering out the lines we don't want
                                 * until we hit the blank line which marks the end
                                 * of the header
                                 */
                                while (strlen(fgets(tempbuff,BUFFERSIZE,afile))>1 && !killit) {
                                    if (matchXlines(tempbuff)) {
                                        fprintf(tfile,"%s",tempbuff);
                                        if (killfile) {
                                            if (!strncmp(tempbuff,"From:",5)) {
                                                for(i=0;killptr[i] && !killit;i++)
                                                    if (strstr(tempbuff,killptr[i])) killit=TRUE;
                                            }
                                        }
                                    }
                                }

                                if (!killit) {

                                    // Keep the blank line
                                    fprintf(tfile,"%s",tempbuff);

                                    /* Now dump the rest of the message through the
                                     * total size of the buffer into the temporary file
                                     */
                                    do {
                                        buflen=fread(tempbuff,1,BUFFERSIZE,afile);
                                        fwrite(tempbuff,buflen,1,tfile);
                                    } while (buflen);
                                }
                                fclose(tfile);
                            }
                            fclose(afile);

                                // Is it tagged for deletion? If so, kill it.
                            if (killit) {
                                if (verb) printf("\r    Killing article %s%ld%s (from %s%s%s)\n",ansi_2,article,ansi_1,ansi_2,killptr[i-1],ansi_1);
                                unlink(articlefile);
                                killit=FALSE;
                            }

                                // Otherwise move the cleaned article back from T:
                            else {
                                if (afile=fopen(articlefile,"wb")) {
                                    if (tfile=fopen("t:NNTPUpdate.temp","rb")) {
                                        do {
                                            buflen=fread(tempbuff,1,BUFFERSIZE,tfile);
                                            fwrite(tempbuff,buflen,1,afile);
                                        } while (buflen);
                                        fclose(tfile);
                                    }
                                    fclose(afile);
                                }
                                unlink("t:NNTPUpdate.temp");
                            }
                        }
                    }
                    if (verb) {
                        if (killfile) printf("\r    Clean/Kill scan completed. ");
                        else printf("\r    Clean scan completed. ");
                    }
                }

                /* This part deals with just scanning and killing messages */

                else if(killfile) {

                    // Now we start scanning messages!
                    for (article=lowpointer; article<=highpointer; article++) {
                        sprintf(articlefile,"%s%ld",newsgroup,article);
                        if (afile=fopen(articlefile,"rt")) {
                            if (verb>1) printf("\r    Scanning article %s%ld%s",ansi_2,article,ansi_1);

                            while (strlen(fgets(tempbuff,BUFFERSIZE,afile))>1 && !killit) {
                                if (matchXlines(tempbuff)) {
                                    if (killfile) {
                                        if (!strncmp(tempbuff,"From:",5)) {
                                            for(i=0;killptr[i] && !killit;i++)
                                                if (strstr(tempbuff,killptr[i])) killit=TRUE;
                                        }
                                    }
                                }
                            }
                            fclose(afile);
                        }
                        if (killit) {
                            if (verb) printf("\r    Killing article %s%ld%s (from %s%s%s)\n",ansi_2,article,ansi_1,ansi_2,killptr[i-1],ansi_1);
                            unlink(articlefile);
                            killit=FALSE;
                        }
                    }
                    if (verb) printf("\r    Kill scan completed. ");
                }

                /*
                 * Update the .next pointer
                 */
                if (highpointer>0) {
                    if (nfile=fopen(nextfile,"wb")) {
                        highpointer++;
                        if (verb) printf("Updating .next pointer to %s%ld%s\n",ansi_2,highpointer,ansi_1);
                        fprintf(nfile,"%ld\n",highpointer);
                        fclose(nfile);
                    }
                }
            }
        }
    }
    fclose(infile);
    for (i=0;killptr[i];i++) free(killptr[i]);
    printf("NEWS SCAN/UPDATE COMPLETE\n");
    exit(0);
}
