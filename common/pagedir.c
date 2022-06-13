/**
 * @file pagedir.c
 * @author Rehoboth Okorie (www.rehoboth.link)
 * @brief pagedir provides functions to create the page directory and save webpages to files.
 * @version 0.1
 * @date 2022-02-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <math.h>
#include "pagedir.h"
#include "webpage.h"
#include "mem.h"
#include "file.h"

/* function to initialize the pageDirectory and create a .crawler file */
/* see pagedir.h for more information */
bool pageDirInit(const char *pageDirectory) {
    if (pageDirectory == NULL) {    // ensure args are valid
        return false;
    }
    DIR* dir = opendir(pageDirectory);
    if (!dir || dir == NULL) {  // ensure directory exists
        return false;
    }
    closedir(dir);
    char crawlerMark[strlen(pageDirectory) + 10];   // .crawler file name +1 for null pointer at end, +1 for log+1 and +1 for '/'
    sprintf(crawlerMark, "%s/.crawler", pageDirectory);
    FILE *fp = fopen(crawlerMark, "w");
    if (fp == NULL) {
        return false;
    }
    fclose(fp);
    return true;
}

/* function to save the url, depth from seedurl and html of a webpage to a file in pagedirectory */
/* see pagedir.h for more information */
void pageDirSave(webpage_t *page, const char* pageDirectory, int fn) {
    if (page == NULL || pageDirectory == NULL || fn < 0) {   // ensure args are valid
        return;
    }
    DIR* dir = opendir(pageDirectory);
    if (!dir || dir == NULL) {  // ensure directory exists
        return;
    }
    closedir(dir);
    char fileName[strlen(pageDirectory) + (int) log10(fn) + 3]; // file name +1 for null pointer at end, +1 for log+1 and +1 for '/'
    sprintf(fileName, "%s/%d", pageDirectory, fn);
    FILE *fp = fopen(fileName, "w");
    if (fp == NULL) {
        return;
    }
    int depth = webpage_getDepth(page);
    char *html = webpage_getHTML(page);
    char *url = webpage_getURL(page);
    fprintf(fp, "%s\n", url);
    fprintf(fp, "%d\n", depth);
    fprintf(fp, "%s\n", html);
    fclose(fp);
}

/* function to load a file that is in a crawler directory into a webpage object */
/* see pagedir.h for more information */
int pageDirLoad(webpage_t **page, const char* pageDirectory, int docID) {
    if (page == NULL || pageDirectory == NULL || docID < 1) {   // validate arguments
        return 0;
    }
    char docFile[strlen(pageDirectory) + (int) log10(docID) + 1];
    sprintf(docFile, "%s/%d", pageDirectory, docID);
    FILE *fp = fopen(docFile, "r");
    if (fp == NULL) {
        return -1;
    }
    char *url = file_readLine(fp);  // url for webpage
    char *depthS = file_readLine(fp);   // depth string
    char *html = file_readFile(fp);
    int depth = atoi(depthS);   // webpage depth
    mem_free(depthS);   // free depth string
    *page = webpage_new(url, depth, html);    // create webpage
    if (*page == NULL) { // ensure calloc was successful
        fclose(fp);
        mem_free(html);
        mem_free(url);
        return 0;
    }
    fclose(fp);
    return 1;
}

/* function to validate that a directory is a crawler directory */
/* see pagedir.h for more information */
bool pageDirValidate(const char* pageDirectory) {
    if (pageDirectory == NULL) {    // ensure args are valid
        return NULL;
    }
    DIR* dir = opendir(pageDirectory);
    if (!dir || dir == NULL) {  // check if directory exists
        return false;
    }
    closedir(dir);
    char crawlerMark[strlen(pageDirectory) + 10];   // .crawler file name +1 for null pointer at end, +1 for log+1 and +1 for '/'
    sprintf(crawlerMark, "%s/.crawler", pageDirectory);
    FILE *fp = fopen(crawlerMark, "r"); /// open .crawler file in read mode to check if it exists
    if (fp == NULL) {   // if fp is null file does not exists so this is not a crawler direstory
        return false;
    }
    fclose(fp);
    return true;
}

/* function to get the url of the webpage from a doc file */
/* see pagedir.h for more information */
char *getPageUrl(const char *pageDirectory, const int docID) {
    if (pageDirectory == NULL || docID < 0) return NULL;
    if (!pageDirValidate(pageDirectory)) return NULL;
    char docFile[strlen(pageDirectory) + (int) log10(docID) + 1];
    sprintf(docFile, "%s/%d", pageDirectory, docID);
    FILE *fp = fopen(docFile, "r");
    if (fp == NULL) {
        return NULL;
    }
    char *url = file_readLine(fp);
    fclose(fp);
    return url;
}
