# Rehoboth Okorie
## CS50 Spring 2021, Lab 1
## common

# Common Lib
## Files
- pagedir.h: provides function descriptors for methods to
```c
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
```
- pagedir.c: implements the functions described in pagedir.h
- index.h: file providing and index object and descriptions to constants and functionsto interact with an index
```c
#ifndef IndexCoeff
#define IndexCoeff 825 // alter this in compilation (using D flag) to improve table efficiency of hashtable
#endif

/**
 * @brief extends to hashtable struct type into an index_t type
 * 
 */
typedef struct hashtable index_t;

/**
 * @brief function to make a new index
 * 
 * @param slots : number of slots needed in index hashtable
 * 
 * @return index_t* : new index object
 */
index_t *indexInit(const int slots);


/**
 * @brief function to insert/update a word in the index
 * 
 * @param index : index to update
 * @param word  : word to add to the index
 * @param docID : docID of document where word was found
 * 
 * @return int : 0 if success -1 if failure
 */
int indexAdd(index_t *index, const char *word, const int docID);

/**
 * @brief 
 * 
 * @param index : index to look through
 * @param word  : word to find in index
 * @return counters_t * : counter of word in index
 */
counters_t *indexFind(index_t *index, const char *word);

/**
 * @brief function to update the counters of a word in the index
 * 
 * @param index index to update
 * @param word word to update its counters
 * @param docID doc id to update count for word
 * @param freq new frequency/count to be used
 * @return int 0 if success ; -1 if failure
 */
int indexUpdate(index_t *index, const char *word, const int docID, const int freq);

/**
 * @brief load an index from a file
 * 
 * @param fn  : name of file to load index from
 * 
 * @return index_t * : loaded index 
 */
index_t *indexLoad(const char* fn);

/**
 * @brief functiom to delete an index and free used heap memory
 * 
 * @param index : index to delete
 * 
 */
void indexDelete(index_t *index);

/**
 * @brief function to save and index to a given file
 * 
 * @param index : index to save to file
 * @param fn    : name of file to save index to
 * 
 */
void indexSave(index_t *index, const char *fn);
```
- index.c: implements index object and descriptions to constants and functions to interact with an index
- word.h: module providing the method normalizeWord which converts a word to lowercase
```c
/**
 * @brief functio to normalize a word (convert the word to lower case)
 * 
 * @param word word to normalize
 * @return int 0 on success; -1 if failure
 */
int normalizeWord(char *word);
```
- word.c: implements items defined in word.h (module providing the method normalizeWord which converts a word to lowercase)

## Usage
Used as a support Library for crawler

## COMPILATION NOTES
- INDEXCOEFF: edit this value in index.h to alter the number of slots in index hashtable. This can affect index efficiency. Alternatively add FLAGS= ... -DINDEXCOEFF=<value> to make file.