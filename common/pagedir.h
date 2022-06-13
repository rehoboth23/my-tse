/**
 * @file pagedir.h
 * @author Rehoboth Okorie (www.rehoboth.link)
 * @brief pagedir provides functions to create the page directory and save webpages to files.
 * @version 0.1
 * @date 2022-02-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __PAGE_DIR_H_
#define __PAGE_DIR_H_
#include <webpage.h>

/**
 * @brief function to initialize the pageDirectory and create a .crawler file
 * 
 * @param pageDirectory path to page directory
 * @return true if no error occurs
 * @return false if there is an error
 */
bool pageDirInit(const char *pageDirectory);

/**
 * @brief function to save the url, depth from seedurl and html of a webpage to a file in pagedirectory
 * 
 * @param page webpage_t struct
 * @param pageDirectory path to page directory
 * @param fn file id
 * do
 *  - nothing is page or pageDirectory is NULL or fn < 0
 *  - create an open file (in writing mode) 
 *  - write url, depth and html f webpage to file;
 */
void pageDirSave(webpage_t *page, const char* pageDirectory, const int fn);

/**
 * @brief function to load a file that is in a crawler directory into a webpage object
 * 
 * @param page pointer to webpage object pointer to load page into
 * @param pageDirectory  name of crawler directory where doc file is
 * @param docID name of docfile to load
 * @return int
 * -1 if page file is invalid
 *  0  for other failures
 * 1 for success
 */
int pageDirLoad(webpage_t **page, const char* pageDirectory, int docID);

/**
 * @brief function to validate that a directory exists and is a crawler directory
 * 
 * @param pageDirectory name of crawler directory
 * @return true if directory exists and is a crawler directory
 * @return false if directory does not exist or is not a crawler directory of something fails in function
 */
bool pageDirValidate(const char* pageDirectory);

/**
 * @brief Get the Page Url for doc1d
 * 
 * @param pageDirectory page directory of webpage files
 * @param docID document id
 * @return char* page url
 */
char *getPageUrl(const char *pageDirectory, const int docID);

#endif