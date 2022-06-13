/**
 * @file index.h
 * @author Rehoboth Okorie (www.rehoboth.link)
 * @brief file providing and index object and descriptions to constants and functions to interact with an index
 * @version 0.1
 * @date 2022-02-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "hashtable.h"
#include "counters.h"

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

