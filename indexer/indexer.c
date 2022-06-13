/**
 * @file indexer.c
 * @author Rehoboth Okorie (www.rehoboth.link)
 * @brief module to index words in crawled webpage documents
 * @version 0.1
 * @date 2022-02-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "mem.h"
#include "webpage.h"
#include "hashtable.h"
#include "pagedir.h"
#include "index.h"

/**
 * @brief functino to print error pessages only when in DEV or TEST modes
 * 
 * @param argc number of arguments
 * @param ... messages to print
 * do 
 *  - nothing if message is NULL or mode not in DEV or TEST
 *  - print message so stderr
 */
static void printErrorMessage(const int argc, ...);

/**
 * @brief helper function to parse args from main into variables
 * 
 * @param args string array containing arguments to indexer
 * @return int 
 * - 0 if success
 * - 1 if failure
 */
static int parseArgs(const char *args[], char **pageDir, char **indexFile);

/**
 * @brief  creates a new 'index' object
           loops over document ID numbers
           loads a webpage from the document
           passes the webpage and docID to indexPage
 * @param pageDir
 * @param indexFile
 * @return int 
 * - 0 if successful
 * - 1 if something went wrong
 */
static int indexBuild (const char *pageDir, const char *indexFile);

/**
 * @brief steps through each word of the webpage
 *        looks up the word in the index
 *        increments the count of occurrences of this word
 * @param page : webpage to get words from
 * @param index : index to update
 * @param docID : document id
 * 
 */
static void indexPage(webpage_t *page, index_t *index, const int docID);

int main(int argc, char const *argv[])
{
    if (argc != 3) {    // ensure arguments are valid
        printf("Usage: indexer <pageDir> <indexFile>\n");
        exit(-1);
    }
    char *pageDir, *indexFile;  // pointers to parsed args
    if (parseArgs(argv, &pageDir, &indexFile) == -1) {  // parse args and ensure correctnes
        printErrorMessage(1, "Bag Arguments\n");
        exit(-1);
    }

    if (indexBuild(pageDir, indexFile) != 0) {  // if indexBuild was successful
        printErrorMessage(1, "main: something went wrong with indexBuild\n");
    }
    mem_free(pageDir);
    mem_free(indexFile);
    return 0;
}

/**
 * @brief helper function to parse args from main into variables
 * 
 * @param args string array containing arguments to indexer
 * @return int 
 * - 0 if success
 * - 1 if failure
 */
static int parseArgs(const char *args[], char **pageDir, char **indexFile) {
    if (args == NULL || pageDir == NULL || indexFile == NULL) { // validate arguments
        printErrorMessage(1, "parseArgs: Invlaid arguments\n");
        return -1;
    }
    
    *pageDir = mem_calloc(strlen(args[1]) + 1, sizeof(char));   // alloate space for pageDir
    if (*pageDir == NULL) { // in case calloc for page dir fails
        printErrorMessage(1, "parseArgs: Failed to allocate pageDir\n");
        return -1;
    }
    strncpy(*pageDir, args[1], strlen(args[1]));    // copy value into allocated space
    if ((*pageDir)[strlen( (*pageDir) ) - 1] == '/') (*pageDir)[strlen( (*pageDir) ) - 1] = '\0';   // normalize page dir argument
    if (!pageDirValidate(*pageDir)) {
        mem_free(*pageDir);
        return -1;
    }
    *indexFile = mem_calloc(strlen(args[2]) + 1, sizeof(char)); // allocate space for index file
    if (*indexFile == NULL) {   // in calse callof for indexFile fails
        printErrorMessage(1, "parseArgs: Failed to allocate indexFile\n");
        mem_free(*pageDir); // free pageDir
        return -1;
    }
    strncpy(*indexFile, args[2], strlen(args[2])); // copy value into allocated space
    return 0;
}

/**
 * @brief  creates a new 'index' object
           loops over document ID numbers
           loads a webpage from the document
           passes the webpage and docID to indexPage
 * @param pageDir
 * @param indexFile
 * @return int 
 * - 0 if successful
 * - 1 if something went wrong
 */
static int indexBuild (const char *pageDir, const char *indexFile) {
    if (pageDir == NULL || indexFile == NULL) { // validate arguments
        printErrorMessage(2, "indexBuild: Invalid Args\n");
        return -1;
    }
    int docID = 1;  // starting doc id
    index_t *index = indexInit(IndexCoeff);
    int loaded = 0; // used to teminate loop
    for (; loaded != -1; docID++) { // loop web page fils to load; -1 is used to indicate the file does not exists so terminate
        webpage_t *page = NULL;
        loaded = pageDirLoad(&page, pageDir, docID);    // locad a webpage
        if (loaded == -1 || loaded == 0) {
            continue;
        }
        indexPage(page, index, docID);  // index the webpage
        webpage_delete(page);
    }
    indexSave(index, indexFile);    // save index to file
    indexDelete(index);
    return 0;
}

/**
 * @brief steps through each word of the webpage
 *        looks up the word in the index
 *        increments the count of occurrences of this word
 * @param page : webpage to get words from
 * @param index : index to update
 * @param docID : document id
 * 
 */
static void indexPage(webpage_t *page, index_t *index, const int docID) {
    if (page == NULL || index == NULL || docID < 1) {   // validate arguments
        return;
    }
    int pos = 0;    // used for reading words from webpage
    char *word = webpage_getNextWord(page, &pos);
    for (; word != NULL; word = webpage_getNextWord(page, &pos)) {  // loop for all words in webpage
        if (strlen(word) >= 3) indexAdd(index, word, docID);    // add count for word to index
        mem_free(word);
    }
}

/* functino to print error pessages only when in DEV or TEST modes */
static void printErrorMessage(const int argc, ...) {
    #ifdef TEST
        if (argc <= 0) {
            fprintf(stderr, "\n");
            return;
        }
        va_list argv;
        va_start(argv, argc);
        int argi;
        
        for (argi = 1; argi <= argc; argi++) {
            char *arg = va_arg(argv, char *);
            fprintf(stderr, "%s", arg);
        }
    #endif
}