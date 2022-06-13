/**
 * @file indextest.c
 * @author Rehoboth okorie (www.rehoboth.link)
 * @brief file to test index.c
 * @version 0.1
 * @date 2022-02-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "index.h"
#include "file.h"
#include "mem.h"
#include "hashtable.h"

/**
 * @brief iterate function to be given to hastables (index)
 * 
 * @param arg other index
 * @param key key/word in index
 * @param item counter of key/word
 */
static void countersIterate(void *arg, const int key, const int  item);

/**
 * @brief iterate function to be given to counters
 * 
 * @param arg other counter
 * @param key key in counter (docid)
 * @param item freq of word in doc
 */
static void compareHashTable(void *arg, const char *key, void *item);

/**
 * @brief stuct to containg args for iteration functions
 * 
 */
typedef struct IndexTestArg
{
    void *data;
    int res;
}arg_t;



int main(int argc, char const *argv[]) {
    if (!(argc == 3)) { // ensure usage is valid
        fprintf(stderr, "Usage: ./indextest <oldindexfile> <newindexfile>\n");
        return -1;
    } 
    index_t *index1 = indexLoad(argv[1]);   // load index from argv[1]
    if (index1 == NULL) {
        return -1;
    }
    indexSave(index1, argv[2]); // save index to argv[2]
    index_t *index2 = indexLoad(argv[2]);   // load index from argv[2]
    if (index2 == NULL) {
        return -1;
    }
    hashtable_t *table1 = (hashtable_t *) index1;
    arg_t arg;
    arg.data = (void *) index2;
    arg.res = 0;
    hashtable_iterate(table1, (void *) &arg, compareHashTable); // check if both indexes are the same in values
    indexDelete(index1);
    indexDelete(index2);
    printf("%s\n", arg.res == 0 ? "TEST PASSED!" : "TEST FAILED");  // log test result
    return arg.res; // will be zero if all went well
}

/**
 * @brief function to be used in hashtable iterate to check correctness
 * 
 * @param argv argument containing second hashtable and return value
 * @param key word to validate counts for
 * @param item counter for word in first hastbale
 */
static void compareHashTable(void *argv, const char *key, void *item) {
    arg_t *arg = (arg_t *) argv;
    hashtable_t *table2 = (hashtable_t *) arg->data;
    counters_t *item1 = (counters_t *) item;
    counters_t *item2 = (counters_t *) hashtable_find(table2, key);
    if (item2 == NULL) {    // if word is not in second counter set test faile to false
        arg->res = -1;  
        return;
    }
    arg_t argn; // arg for second counter
    argn.data = (void *) item2;
    argn.res = arg->res;
    counters_iterate(item1, (void *) &argn, countersIterate);   // iterate in counters
    arg->res = argn.res;    // set arg result as result of iterate arg
}

/**
 * @brief function to be used in counter iterate to check correctness
 * 
 * @param argv argument containing second counter and return value
 * @param key docid to compare count for
 * @param item argument count in first counter
 */
static void countersIterate(void *argv, const int key, const int  item) {
    arg_t *arg = (arg_t *) argv;    // get argument and cast to type arg_t
    counters_t *c2 = (counters_t *) arg->data;  // get second counter from arg
    int val = counters_get(c2, key);
    if (val != item) {  // if values are not the same in both counters set arg->res to test fail value
        arg->res = -1;
    }
}
