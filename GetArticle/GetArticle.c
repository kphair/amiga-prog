/*
 * Tool to retrieve a news article fron an NNTP server
 * (c) 1995 Kevin Phair
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <exec/types.h>
#define DEBUG

const char version[]="\0$VER: GetArticle 1.0 (9.07.95)";

void main (int argc, char **argv) {

    FILE *news;
    FILE *article;

    unsigned int clock[2];

    char ArticleBuffer[4096]={0};
    char NewsServer[256]={0};
    char outname[40]={0};

    long lineno=1;

    // Check arguments
    if (argc<3) {
        printf ("\nPlease specify the news server, newsgroup, and article!\n\n");
        printf ("Usage: %s news-server newsgroup article\n\n", argv[0]);
        exit(10);
    }

    // Open news server connection
    sprintf(NewsServer,"TCP:%s/nntp",argv[1]);
    if(!(news=fopen(NewsServer,"rt+"))) {
        printf("\nError opening TCP connection!\n\n");
        exit(30);
    }

    // Get welcome response from server
    #ifdef DEBUG
        printf("Getting response from news server\n");
    #endif
    fgets(ArticleBuffer, sizeof(ArticleBuffer,news));
    printf("%s",ArticleBuffer);
    if (ArticleBuffer[0] != '2') {
        printf ("\nNews server rejected connection!\n%s\n\n", ArticleBuffer);
        exit(20);
    }

    // Enter newsgroup
    #ifdef DEBUG
        printf("Requesting entry to %s\n",argv[2]);
    #endif
    fprintf(news,"group %s\r\n",argv[2]);
    #ifdef DEBUG
        printf("Waiting...\n",argv[2]);
    #endif
    fgets(ArticleBuffer, sizeof(ArticleBuffer), news);
    printf("%s",ArticleBuffer);
    if (ArticleBuffer[0] != '2') {
        printf ("\nError entering newsgroup!\n%s\n\n", ArticleBuffer);
        exit (20);
    }

    // Open an output file to put the body text in
    timer(clock);
    sprintf(outname,"%ld.%ld",clock[0],clock[1]);
    #ifdef DEBUG
        printf("Opening output file %s\n",outname);
    #endif
    if (!(article=fopen(outname,"wb"))) {
        printf("\nError opening output file!\n\n");
        fclose(news);
        exit (30);
    }

    // Get article
    #ifdef DEBUG
        printf("Requestiing message body\n",argv[2]);
    #endif
    fprintf(news,"body %s\r",argv[3]);
    #ifdef DEBUG
        printf("Waiting...\n",argv[2]);
    #endif
    fgets(ArticleBuffer, sizeof(ArticleBuffer), news);
    printf("%s",ArticleBuffer);
    if (ArticleBuffer[0] != '2') {
        printf("\nError getting article!\n%s\n\n",ArticleBuffer);
        exit (20);
    }

    while (1) {
        if (fgets(ArticleBuffer, sizeof(ArticleBuffer), news)) {
            fputs(ArticleBuffer, article);
            if (!(lineno % 10)) printf("%ld\r",lineno++);
        } else {
            ArticleBuffer[0] = '.'; ArticleBuffer[1] = 13;
            printf("\nfgets returned NULL pointer!\n");
        }
        if ((ArticleBuffer[0] == '.') && (ArticleBuffer[1] == 13)) break;
    }

    fclose(article);
    printf("\nArticle retrieved\n\n");
    exit(0);
}
