/**
 * @file index.c
 * @author Rehoboth Okorie (www.rehoboth.link)
 * @brief implements index object and descriptions to constants and functions to interact with an index
 * @version 0.1
 * @date 2022-02-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "word.h"
#include "hashtable.h"
#include "counters.h"
#include "math.h"
#include "mem.h"
#include "index.h"
#include "file.h"


/**
 * @brief print row in index using iterate
 * 
 * @param fp file pointer to print to
 * @param key key in index
 * @param value key in value
 */
static void printIndexRow(void *fp, const char *key, void *value);

/**
 * @brief function to print a counter using iterate
 * 
 * @param fp file pointer to print to
 * @param key key in counter
 * @param value value in counter
 */
static void printCounter(void *fp, const int key, const int value);

/**
 * @brief Get the next word in null terminated string line object starting from pos
 * 
 * @param line line to get word from
 * @param pos position to start seeking word from
 * @return char* new word found in line
 */
static char *getWordInLine(const char *line, int *pos);

/* */
/* see index.h for more information */
typedef struct hashtable index_t;


/* function to initialie and index */
/* see index.h for more information */
index_t *indexInit(const int slots) {
    if (slots <= 0) return NULL;
    index_t *index = (index_t *) hashtable_new(slots);
    return index;
}

/* function to add count for a word to an index */
/* see index.h for more information */
int indexAdd(index_t *index, const char *word, const int docID) {
    if (index == NULL || word  == NULL || docID < 1) {  // validate arguments
        return -1;
    }
    if (normalizeWord((char *) word) != 0) {    // normalize word
        return -1;
    }
    hashtable_t *table = (hashtable_t *) index; // cast index into hashtable
    counters_t *counters = (counters_t *) hashtable_find(table, word);  // find the counters for word in index
    bool insertAfter = false;
    if (counters == NULL) {
        counters = counters_new(); // make new caounter if not found
        if (counters == NULL) {
            return -1;
        }
        insertAfter = true;     // flag to insert counter into table
    }
    if (counters_add(counters, docID) < 1) {    // add docid instance to counters
        return -1;
    }
    if (insertAfter) {
        if (!hashtable_insert(table, word, (void *) counters)) {
            return -1;
        }
    }
    return 0;
}


/* function to find word in an index */
/* see index.h for more information */
counters_t *indexFind(index_t *index, const char *word) {
    if (index == NULL || word == NULL) {    // validate args
        return NULL;
    }
    if (normalizeWord((char *) word) != 0) {    // normalize word
        return NULL;
    }
    hashtable_t *table = (hashtable_t *) index;
    counters_t *counters = (counters_t *) hashtable_find(table, word); // find counters for word
    return counters;    // return counter
}

/* funciton to update the counter for a word in an index */
/* see index.h for more information */
int indexUpdate(index_t *index, const char *word, const int docID, const int freq) {
    if (index == NULL || word == NULL || docID < 1 || freq < 1) {   // validate args
        return -1;
    }
    bool insertAfter = false;   // track if we need to insert counter into index
    hashtable_t *table = (hashtable_t *) index;
    counters_t *counters = (counters_t *) hashtable_find(table, word);  // get counters form index
    if (counters == NULL) {
        counters = counters_new();  // make a new counter if word not in index yet
        if (counters == NULL) {
            return -1;
        }
        insertAfter = true; //  note to insert counter into index
    }
    if (!counters_set(counters, docID, freq)) { // set value of docid in counters
        return -1;
    }
    if (insertAfter) {  // insert counters into index
        if (!hashtable_insert(table, word, (void *) counters)) {
            return -1;
        }
    }
    return 0;
}


/* function to load an index from a file */
/* see index.h for more information */
index_t *indexLoad(const char* fn) {
    if (fn == NULL) {   // validate arguments
        return NULL;
    }
    index_t *index = indexInit(IndexCoeff); // get and index
    if (index == NULL) {
        return NULL;
    }
    FILE *fp = fopen(fn, "r");  // open file in read mode
    if (fp == NULL) {
    }
    char *line;
    for (line = file_readLine(fp); line != NULL; line = file_readLine(fp)) {    // read line
        int pos = 0;    // will be neeed by helper function to track position in line
        char *word = getWordInLine(line, &pos);   // get a word from line (word is any contiguous charaters not includeing space and new lines and null)
        if (word == NULL) {    // ensure word was got successfuly
            mem_free(line);
            return NULL;
        }
        for (char *token = getWordInLine(line, &pos); token != NULL; token = getWordInLine(line, &pos)) {
            if (token == NULL) {    // ensure token was got
                break;
            }
            int docID = atoi(token);    // convert to integer
            if (docID < 1) {    // ensure conversion was successful
                break;
            }
            mem_free(token);    // free token
            token = getWordInLine(line, &pos);  // get next word (freq)
            if (token == NULL) { // ensure token was got
                break;
            }
            int freq = atoi(token);     // convert to integer
            if (freq < 1) {  // ensure conversion was successful
                break;
            }
            indexUpdate(index, word, docID, freq); // update index for word -> docID with freq
            mem_free(token);
        }
        // free line and word
        mem_free(line);
        mem_free(word);
    }
    if (fp != stdin) {  // close file
        fclose(fp);
    }
    return index;
}


/* functio to delete an index and free memory */
/* see index.h for more information */
void indexDelete(index_t *index) {
    if (index == NULL) {    // validate arguments
        return;
    }
    hashtable_t *table = (hashtable_t *) index; // cast to hashtable
    hashtable_delete(table, (void (*)(void *)) counters_delete);    // delete hashtable
}

/* function to save and index to a file */
/* see index.h for more information */
void indexSave(index_t *index, const char *fn) {
    if (index == NULL || fn == NULL) {  // validate arguments
        return;
    }
    FILE *fp;
    if (strncmp(fn, "-", 1) == 0) { // saved file name for stdout
        fp = stdout;
    } else {
        fp = fopen(fn, "w");    // open file in write mode
    }
    if (fp == NULL) {
        return;
    }
    hashtable_t *table = (hashtable_t *) index; // cast to hashtable
    hashtable_iterate(table, fp, printIndexRow);    // iterate through index to get values and write to file
    fclose(fp); // close file
}

/**
 * @brief print row in index using iterate
 * 
 * @param fp file pointer to print to
 * @param key key in index
 * @param value key in value
 */
static void printIndexRow(void *fp, const char *key, void *value) {
    if (fp == NULL || key == NULL || value == NULL) {   // validate arguments
        return;
    }
    counters_t *counters = (counters_t *) value;    // cast to couters
    fprintf(fp, "%s ", key);
    counters_iterate(counters, fp, printCounter);   // iterate counter t oget values and write to file
    fprintf(fp, "\n");
}

/**
 * @brief function to print a counter using iterate
 * 
 * @param fp file pointer to print to
 * @param key key in counter
 * @param value value in counter
 */
static void printCounter(void *fp, const int key, const int value) {
    if (fp == NULL || key < 0 || value < 0) {   // validate  arguments
        return;
    }
    fprintf(fp, " %d %d", key, value);  // write values to file
}

/**
 * @brief Get the next word in null terminated string line object starting from pos
 * 
 * @param line line to get word from
 * @param pos position to start seeking word from
 * @return char* new word found in line
 */
static char *getWordInLine(const char *line, int *pos) {
    if (line == NULL || pos == NULL) {  // validate arguments
        return NULL;
    }
    if (*pos >= strlen(line)) { // check if start point is in range of string
        return NULL;
    }
    int end = *pos;
    bool startedWord = false;
    for (; end < strlen(line); end++) { // loop till end of line
        if (!startedWord) { // if start of a new word has not been found in line
            if (line[end] == ' ' || line[end] == '\n') {    // if still not a new word
                (*pos)++;   // increments starting position
            } else {
                startedWord = true; // note that a new word has been found
            }
        } else {
            if (line[end] == ' ' || line[end] == '\n') {    // if word has been found and is over break
                break;
            }
        }
    }
    if (*pos >= strlen(line)) return NULL;  // is pos is out of range of the string return NULL
    
    char *token = mem_calloc((end - (*pos)) + 1, sizeof(char)); // malloc space for token/word
    if (token == NULL) {    // ensure malloc was successful 
        return NULL;
    }
    strncpy(token, line + *pos, end - *pos);    // copy word found into token
    *pos = end; // update value of pos to current word end
    return token;   // return token
}
