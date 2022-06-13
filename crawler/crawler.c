/* 
 * crawler.c - crawler is a standalone program that crawls the web and 
                retrieves webpages starting from a "seed" URL.
 *  
 * Rehoboth Okorie, Feb 3 2022
 * Usage: ./crawler <seedURL> <pageDirectory> <maxDepth>
 */

#ifndef CrawlerCoeff
#define CrawlerCoeff 500 // alter this in compilation (using D flag) to improve table efficiency of hashtable depending on expected url density per page
#endif


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "webpage.h"
#include "bag.h"
#include "hashtable.h"
#include "mem.h"
#include "pagedir.h"

/**
 * @brief helper function to parse args from main into variables
 * 
 * @param args unparsed arguments from main
 * @param seedUrl  seed/first url to star crawling from
 * @param pageDirectory name of directory to save webpage files
 * @param maxDepth maximum depth to reach in crawling
 * @return int return 0 if not error -1 if errors
 * do
 *  - nothing id any of args, seedUrl, pageDirectory is NULL or maxDepth < 0
 *  - parse the arguments int args into the variables
 */
static int parseArgs(char const *args[], char **seedUrl, char **pageDirectory, int *maxDepth);

/**
 * @brief helper function to initialize required data structures
 * 
 * @param page webpage_t struct
 * @param pagesToCrawl bag_t struct containing pages to crawl
 * @param pagesSeen hashtable_t struct containing seen urls
 * @param seedUrl seed/first url to star crawling from
 * @param maxDepth maximum depth to reach in crawling
 * @return int return 0 if not error -1 if errors 
 * do 
 *  - nothing if any of page, pagesToCrawl, pagesSeen, seedUrlis NULL or maxDepth < 0
 *  - initialize the pointers to required structs
 */
static int initStructures(webpage_t **page, bag_t **pagesToCrawl,  hashtable_t **pagesSeen, 
                                                                const char *seedUrl, const int maxDepth);

/**
 * @brief function to crawl the web starting from a seed url and continuing down into links within the pages html unitl some max depth
 * 
 * @param seedUrl initial url to begin crawling from
 * @param pageDirectory name of directory to save webpage files
 * @param maxDepth 
 * @return int return 0 if not error -1 if errors 
 * do
 *  - nothing if any of seedUrl or pageDirectory is NULL or maxDepth < 0
 *  - normalize seedUrl and initialize structures
 *  - pull a webpage from the bag
 *  - fetch the webpage html using pagefetcher
 *  - save the webpage to file using pageSave
 *  - if not at maxdepth scan the page for new urls using pageScan
 */
static int crawl(const char *seedUrl, const char *pageDirectory, const int maxDepth);

/**
 * @brief function to fetch the html of a webpage into a webpage_t struct
 * 
 * @param page webpage_t struct
 * @param pageDirectory 
 * @return int return 0 if not error -1 if errors 
 * do 
 *  - nothing is any arg is NULL
 *  - fetch the webpage html
 */
static int pageFetch(webpage_t *page, const char *pageDirectory);

/**
 * @brief function to scan/parse a webpage using the webpage_t struct and extract all urls and links in the page
 * 
 * @param page webpage_t struct
 * @param pagesSeen hashtable of seen urls
 * @param pagesToCrawl bag containing pages to crawl
 * @return int return 0 if not error -1 if errors
 * do 
 *  - nothing if any arg is NULL
 *  - scna the page for new/unseen urls and add the to the bad and hashtable
 */
static int pageScan(webpage_t *page, hashtable_t *pagesSeen, bag_t *pagesToCrawl);

/**
 * @brief function to save a webpages url, depth from seed and html into a file identified by pageId
 * 
 * @param page webpage_t struct
* @param pageDirectory name of directory to save webpage files
 * @param pageId pageId which will be used as file name
 * @return int return 0 if not error -1 if errors
 * do 
 *  - nothing is page is NULL or pageId < 0
 * - save the page url, depth and html to a file
 */
static int pageSave(webpage_t *page, const char *pageDirectory, const int pageId);

/**
 * @brief functino to print error pessages only when in DEV or TEST modes
 * 
 * @param message message to print
 * do 
 *  - nothing if message is NULL or mode not in DEV or TEST
 *  - print message so stderr
 */
static void printErrorMessage(const char *message);


int main(int argc, char const *argv[]) {
    if (argc < 4) {
        printErrorMessage("Usage: ./crawler <seedURL> <pageDirectory> <maxDepth>");
        exit(-1);
    }
    char *seedUrl;
    char *pageDirectory;
    int maxDept;
    // parse args and exit non-zero if there are any issues
    if (parseArgs(argv, &seedUrl, &pageDirectory, &maxDept) != 0) {
        printErrorMessage("Usage: ./crawler <seedURL> <pageDirectory> <maxDepth>");
        exit(-1);
    }
    // initi dir and exit non-zero if there are any issues 
    if (!pageDirInit(pageDirectory)) {
        printErrorMessage("main: pageDirectory does not exist.");
        exit(-1);
    }
    // crawl from seed url and exit non-zero if there are any issues 
    if (crawl(seedUrl, pageDirectory, maxDept) != 0) {
        printErrorMessage("main: crawler failed to run successfuly.");
        exit(-1);
    }
    mem_free(pageDirectory);
    return 0;
}

/* helper function to parse args from main into variables */
// use double pointer to refer to original pointer values even in function
static int parseArgs(char const *args[], char **seedUrl, char **pageDirectory, int *maxDepth) {
    if (args == NULL || seedUrl == NULL || pageDirectory == NULL || maxDepth == NULL) { // ensure args are valid
        printErrorMessage("parseArgs: Invalid Args.");
        return -1;
    }
    *seedUrl = mem_calloc(strlen(args[1]) + 1, sizeof(char));
    if (*seedUrl == NULL) { // ensure calloc was succesful
        printErrorMessage("parseArgs: Calloc seedUrl failed.");
        return -1;
    }
    strncpy(*seedUrl, args[1], strlen(args[1]));    // copy value into seedurl
    *pageDirectory = mem_calloc(strlen(args[2]) + 1, sizeof(char));
    if (*pageDirectory == NULL) { // ensure calloc was succesful
        printErrorMessage("parseArgs: Calloc pageDirectory failed.");
        mem_free(*seedUrl);
        return -1;
    }
    strncpy(*pageDirectory, args[2], strlen(args[2])); // copy value into pageDirectory
    char *str = *pageDirectory;
    if (str[strlen(str) - 1] == '/') {  // normalize the pageDirectory path
        str[strlen(str) - 1] = '\0';
    }

    for (int i = 0; i < strlen(args[3]); i++) {
        if (!isdigit(args[3][i])) {
            mem_free(*seedUrl);
            mem_free(*pageDirectory);
            return -1;
        }
    }
    *maxDepth = strtol(args[3], NULL, 10);
    return 0;
}

/* helper function to initialize required data structures */
// use double pointer to refer to original pointer values even in function
static int initStructures(webpage_t **page, bag_t **pagesToCrawl,  hashtable_t **pagesSeen,    
                                                                const char *seedUrl, const int maxDepth) {
    if (page == NULL || pagesToCrawl == NULL || pagesSeen == NULL || seedUrl == NULL || maxDepth < 0) { // ensure args are valid
        printErrorMessage("initStructures: Invalid args.");
        return -1;
    }
    char *html = NULL;
    *page = webpage_new((char *) seedUrl, 0, html);
    if (*page == NULL) { // ensure webpage_new was successful
        printErrorMessage("initStructures: webpage new failed.");
        return -1;
    }
    *pagesToCrawl = bag_new();
    if (*pagesToCrawl == NULL) { // ensure bag_new was successful
        printErrorMessage("initStructures: bag new failed.");
        return -1;
    }
    bag_insert(*pagesToCrawl, *page); // inser webpage into page
    
    *pagesSeen = hashtable_new(CrawlerCoeff);
    if (*pagesSeen == NULL) {  // ensure hashtable_new was successful
        printErrorMessage("initStructures: hashtable new failed.");
        return -1;
    }
    return 0;
}

/* function to crawl the web starting from a seed url and continuing down into links within the pages html unitl some max depth */
static int crawl(const char *seedUrl, const char *pageDirectory, const int maxDepth) {
    if (seedUrl == NULL || pageDirectory == NULL || maxDepth < 0) { // ensure args are valid
        printErrorMessage("crawl: Invalid args.");
        return -1;
    }
    webpage_t *page;
    bag_t *pagesToCrawl;
    hashtable_t *pagesSeen;
    char *url = normalizeURL(seedUrl);  // normalize url
    if (url == NULL) { // ensure normalizeURL was successful
        printErrorMessage("crawl: normalize url failed.");
        mem_free((char *) seedUrl);
        mem_free((char *) pageDirectory);
        return -1;
    }
    if (!isInternalURL(url)) { // ensure url is an internal url
        printErrorMessage("crawl: external url.");
        mem_free((char *) seedUrl);
        mem_free((char *) pageDirectory);
        mem_free(url);
        return -1;
    }
    if (initStructures(&page, &pagesToCrawl, &pagesSeen, url, maxDepth) != 0) {
        mem_free((char *) seedUrl);
        mem_free((char *) pageDirectory);
        mem_free(url);
        return -1;    // enure required structures are initializzed
    }
    hashtable_insert(pagesSeen, seedUrl, ""); // insert seedUrl into hashtable
    hashtable_insert(pagesSeen, url, ""); // insert seedUrl into hashtable
    int pageId = 1;
    while ((page = bag_extract(pagesToCrawl)) != NULL) {    // stop when bag is empty
        if (pageFetch(page, pageDirectory) != 0) {    // ensure webpage html is properly fetched
            webpage_delete(page); // delete webpage
            continue;
        }
        if (pageSave(page, pageDirectory, pageId) != 0) {  // ensure webpage is saved to file
            webpage_delete(page); // delete webpage
            continue;
        }
        // save the webpage to pageDirectory
        if (webpage_getDepth(page) < maxDepth) {    // skip if at depth limit
            if (pageScan(page, pagesSeen, pagesToCrawl) != 0) {  // ensure pagescan was successful
                webpage_delete(page); // delete webpage
                continue;
            }
        }
        pageId++;
        webpage_delete(page); // delete webpage
    }
    mem_free((char *) seedUrl);
    bag_delete(pagesToCrawl, NULL); // delete bag struct
    hashtable_delete(pagesSeen, NULL); // delete hastable struct
    return 0;
}

/* function to fetch the html of a webpage into a webpage_t struct */
static int pageFetch(webpage_t *page, const char *pageDirectory) {
    if (page == NULL || pageDirectory == NULL) {    // ensure args are valid
        printErrorMessage("pageFetch: invalid args.");
        return -1;
    }
    if (!webpage_fetch(page)) { // ensure webpage_fetch was succesful
        printErrorMessage(webpage_getURL(page));
        printErrorMessage("pageFetch: webpage fetch failed.");
        return -1;
    }
    return 0;
}

/* function to scan/parse a webpage using the webpage_t struct and extract all urls and links in the page */
static int pageScan(webpage_t *page, hashtable_t *pagesSeen, bag_t *pagesToCrawl) {
    if (page == NULL || pagesSeen == NULL || pagesToCrawl == NULL) { // ensure args are valie
        printErrorMessage("pageScan: invalid args.");
        return -1;
    }
    int pos = 0;
    // ensure webpage_getNextURL was succesful; nexturl is fred when hastable is deleted
    char *nextUrl = webpage_getNextURL(page, &pos);
    for (; nextUrl != NULL; nextUrl = webpage_getNextURL(page, &pos)) { 
        char *url = normalizeURL(nextUrl); // normalize url, will be freed when webpage is deleted
        if (url == NULL) { // ensure normalie url was successful
            printErrorMessage("pageScan: normalize url failed.");
            mem_free(nextUrl);
            continue;
        }
        if (!isInternalURL(url)) {  // ensure url is an internal url
            printErrorMessage("pageScan: external url.");
            mem_free(url);
            mem_free(nextUrl);
            continue;
        }
        if (hashtable_find(pagesSeen, url) != NULL || hashtable_find(pagesSeen, nextUrl) != NULL) { // ensure url has not been seen before
            printErrorMessage("pageScan: duplicate url.");
            mem_free(url);
            mem_free(nextUrl);
            continue;
        }
        hashtable_insert(pagesSeen, nextUrl, "");
        hashtable_insert(pagesSeen, url, "");
        char *html = NULL;
        webpage_t *tempPage = webpage_new(url, webpage_getDepth(page) + 1, html);
        if (tempPage == NULL) { // ensure webpage_new was succesful
            printErrorMessage("pageScan: webpage new failed.");
            mem_free(url);
            mem_free(nextUrl);
            continue;
        }
        bag_insert(pagesToCrawl, tempPage); // add webpage to bag
        mem_free(nextUrl);
    }
    return 0;
}

/* function to save a webpages url, depth from seed and html into a file identified by pageId */
static int pageSave(webpage_t *page, const char *pageDirectory, const int pageId) {
    if (page == NULL || pageId < 0) { // ensureargs are valid
        printErrorMessage("pageSave: invalid args.");
        return -1;
    }
    pageDirSave(page, pageDirectory, pageId);
    return 0;
}

/* functino to print error pessages only when in DEV or TEST modes */
static void printErrorMessage(const char *message) {
    #ifdef TEST
        if (message != NULL) {
            fprintf(stderr, "%s", message);
            fprintf(stderr, "\n");
        }
    #endif
}
