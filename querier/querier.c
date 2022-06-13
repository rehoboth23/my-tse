/**
 * @file querier.c
 * @author Rehoboth Okorie (www.rehoboth.link)
 * @brief program load and indexer from a file and read queries to run against the loaded indexer and provide ranked results
 * @version 0.1
 * @date 2022-02-20
 * Usage: ./querier <pageDirectory> <indexFilename> 
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>
#include "pagedir.h"
#include "index.h"
#include "bag.h"
#include "counters.h"
#include "set.h"
#include "pagedir.h"
#include "file.h"
#include "word.h"

/**
 * @brief node type used to sort results
 * 
 */
typedef struct lnode{
    int values[2];
    struct lnode *next;
} lnode_t;

/**
 * @brief arguments struct passed to iteration functions
 * 
 */
typedef struct IterArg {
    counters_t *res;    // argument to save result
    counters_t *other;  // argument used to pass a seperate counters
    lnode_t *argNode;   // node used for sorting
    int count;  // count of nodes created
    } iter_arg_t;

int fileno(FILE *stream);

/**
 * @brief functinn to print pessages only when in DEV or TEST modes
 * 
 * @param argc arguments to parse and log
 * @param ... 
 */
static void logMessage(const int argc, ...);

/**
 * @brief helper function to parse arguments for the querier
 * 
 * @param args command line arguments to parse
 * @param pageDir pointer to char pointer to store the page directory
 * @param indexFile pointer to char pointer tp stpre the index file name
 * @return int
 * - 0 for failure
 * - -1 for success
 */
static int parseArgs(char *args[], char **pageDir, char **indexFile);

/**
 * @brief helper function that accepts and indexer and queries the indexer
 * 
 * @param index indexer to query
 * @param pageDir pointer to char pointer to store the page directory
 *  * @param queryList list of words in query
 * @return int return status code
 * - 0 for failure
 * - -1 for success
 */
static int query(index_t *index, char *pageDir, char **queryList);

/**
 * @brief helper function to reads from stdin, validates input and parses into a normalized query
 * 
 * @param index indexer to query
 * @param pageDir pointer to char pointer to store the page directory
 */
static void readParse(index_t *index, char *pageDir);

/**
 * @brief helper function to prompt and read line
 * 
 * @return char* line read
 */
static char *prompt();

/**
 * @brief function to tokenize a string by space and insert into a list
 * 
 */
static int tokenize(char **list, char *line);

/**
 * @brief helper function to check if a word is an op (and , or)
 * 
 * @param word word to check
 * @return true if word is "and" or "or"
 * @return false any other word
 */
static bool isOP(char *word);

/**
 * @brief check if a query is valid
 * 
 * @param query query (string array to validate)
 * @param querySize size of query list
 * @return true if query is valid (see design doc)
 * @return false if query is invalid
 */
static bool validateQuery(char **query, int querySize);

/**
 * @brief helper function to iterate through 2 counters and perfom an intersect them
 * for two counters c1, c2 ::: c3[key] = min(c1[key], c2[key])
 * @param arg argument struct passed during iteration
 * @param key key to intersect
 * @param val1 value of key in iterating counters
 */
static void countersIntersect(void *arg, const int key, const int val1);

/**
 * @brief helper function to make a uninon of counters 
 * i.e with two counters c1, c2 ::: c3 [key] = c1[key] + c2[key]
 * 
 * @param arg argument sturct passed during iteration
 * @param key key to unionize
 * @param val1 value of key in iterating counters
 */
static void countersUnion(void *arg, const int key, const int val1);

/**
 * @brief helper function to sort and print the values in a counter
 * 
 * @param scores counters to sort
 * @param pageDir pointer to char pointer to store the page directory
 * @return int 0 if on success and -1 if there is a failure
 */
static int sortPrint(counters_t *scores, char *pageDir);

/**
 * @brief helper function to be user to iterate when sorting counter
 * 
 * @param arg argument sturct passed during iteration
 * @param key key in counters to sort
 * @param val value to sort by
 */
static void sortIterate(void *arg, const int key, const int val);

/**
 * @brief helper function to copy a counter into another
 * 
 * @param arg agument passed to counters for iteration
 * @param key key in iterating counter
 * @param val value of key in iterating counter
 */
static void copyIter(void *arg, const int key, const int val);




int main(int argc, char const *argv[]) {

    if (argc != 3) {    // ensure arguments are the require number
        printf("Usage: ./querier <pageDirectory> <indexFilename>\n");
        exit(-1);
    }
    int exit_code = 0;
    char *pageDir = NULL;
    char *indexFile = NULL;
    index_t *index = NULL;

    if (parseArgs((char **) argv, &pageDir, &indexFile) == -1) {    // parse arguments into varaibles and validate them
        logMessage(5, "%s", "main: invalid arguments (", "%s", argv[1], "%s" , ", ", "%s", argv[2], "%s", ")\n");
        exit_code = -1;
        goto prep_exit;
    }

    index = indexLoad((char *) indexFile);  // load an index using filename provided
    if (index == NULL) goto prep_exit;  // ensure index was created

    readParse(index, pageDir);

    prep_exit:  // exit prep that can be moved to from anypoint in the function to cover all bases
    if (pageDir != NULL) free(pageDir);
    if(indexFile != NULL) free(indexFile);
    if (index != NULL) indexDelete(index);
    return exit_code;
}

/* helper function to parse arguments for the querier */
static int parseArgs(char *args[], char **pageDir, char **indexFile) {
    if (args == NULL || pageDir == NULL || indexFile == NULL) {  //   validate arguments
        return -1;
    }

    *pageDir = calloc(strlen(args[1]) + 1, sizeof(char)); 
    if (pageDir == NULL) {  // ensure calloc worked
        logMessage(3, "%s", "parseArgs: Failed to malloc space for pageDIR (", "%s", args[1], "%s", ")\n");
        return -1;
    }
    strncpy(*pageDir, args[1], strlen(args[1]));    // copy value into allocated space
    if ((*pageDir)[strlen( (*pageDir) ) - 1] == '/') (*pageDir)[strlen( (*pageDir) ) - 1] = '\0';   // normalize page dir argument
    if (!pageDirValidate(*pageDir)) {   // ensure it is a crawler page directory
        return -1;
    }

    *indexFile = calloc(strlen(args[2]) + 1, sizeof(char)); // allocate space for index file
    if (*indexFile == NULL) {   // in calse callof for indexFile fails
        logMessage(3, "%s", "parseArgs: Failed to allocate indexFile (", "%s", indexFile, "%s", ")\n");
        return -1;
    }
    strncpy(*indexFile, args[2], strlen(args[2])); // copy value into allocated space
    FILE *fp = fopen(*indexFile, "r");
    if (fp == NULL) {
        logMessage(3, "%s", "parseArgs: Failed to open indexFile (", "%s", *indexFile, "%s", ")\n");
        return -1;
    }
    fclose(fp);
    return 0;
}

/* helper function that accepts and indexer and reads queries parses them and queries the indexer */
static int query(index_t *index, char *pageDir, char **queryList) {
    if (index == NULL || pageDir == NULL || queryList == NULL) {
        logMessage(1, "query: Invalid arguments\n");
        return -1;
    }
    int return_code = 0;
    int querySize = 0;
    counters_t *prev = NULL;
    counters_t *queryResult = NULL;
    bag_t *bag = NULL;

    for(;queryList[querySize] != NULL; querySize++);
    if (querySize == 0)  {
        return_code = 0;
        goto prep_return;
    }
    if (validateQuery(queryList, querySize)) {  // validate query
        logMessage(1, "%s", "\nQuery is Valid!\n");
    } else {
        logMessage(1, "%s", "\nQuery is invalid!\n");
        return_code = -1;
        goto prep_return;
    }
    
    
    bag = bag_new();    // will keep discrete blocks ( x1 or .... or xn) of the query
    // discreet block are unionized and not intersected
    if (bag == NULL) {  // ensure bag was created
        return_code = -1;
        goto prep_return;
    }
    for (int i = 0; i < querySize;) {
        char *word1 = NULL;
        char*op = NULL;
        char *word2 = NULL;
        counters_t *counters1 = NULL;
        counters_t *counters2 = NULL;
        if (i == 0) {   // if this is the first word in the query
            word1 = queryList[i];
            if (i < querySize - 1 && isOP(queryList[i+1])) {    // if there is at least two more words in the query and next word is an operation
                op = queryList[i+1];
                word2 = queryList[i+2];
                i += 3;
            } else if(i < querySize - 1) {   // if there is at least two more words in the query and next word is not an operation
                op = "and";
                word2 = queryList[i+1];
                i += 2;
            } else {    // if there is one word in the query
                op = "or";
                i++;
            }
        } else {    // if not the first word
            if (isOP(queryList[i])) {   // is the word is an operation
                op = queryList[i];
                word1 = queryList[i + 1];
                i += 2;
            } else {    // if word is not an operation
                op = "and";
                word1 = queryList[i];
                i += 1;
            }
        }
        if (prev != NULL) { // prev is the other value to operate with
            iter_arg_t arg;
            counters1 = prev;
            counters2 = counters_new();
            arg.res = counters2;
            counters_iterate(indexFind(index, word1), (void *) &arg, copyIter); // copy index counter of word 1 to the counters1
        } else if (word2 == NULL) { // only one word in query
            iter_arg_t arg;
            counters1 = counters_new();
            arg.res = counters1;
            counters_iterate(indexFind(index, word1), (void *) &arg, copyIter); // copy index counter of word 1 to the counters1
        } else {    // first two words in query
            iter_arg_t arg;
            counters1 = counters_new();
            counters2 = counters_new();
            arg.res = counters1;
            counters_iterate(indexFind(index, word1), (void *) &arg, copyIter); // copy index counter of word 1 to the counters1
            arg.res = counters2;
            counters_iterate(indexFind(index, word2), (void *) &arg, copyIter); // copy index counter of word 2 to the counters2
        }
        if (strncmp(op, "or", 2) == 0) {    // when or use prev as discreet bock and add it to bag; prev now becomes counters2
            bag_insert(bag, (void *) counters1);
            prev = counters2;
        } else if (strncmp(op, "and", 2) == 0) {    // when and intersect counters 1 and 2
            iter_arg_t arg;
            arg.res = counters1;
            arg.other = counters2;
            counters_iterate(counters1, (void *) &arg, countersIntersect);
            if (prev == NULL) prev = counters1;
            counters_delete(counters2); // delete counters 2 (it is a copy so this does not affect the indexer )
        }
    }

    if (prev != NULL) bag_insert(bag, (void *) prev);   // put last item into the bag

    queryResult = counters_new();   // result counters
    counters_t *next;
    while((next = (counters_t *) bag_extract(bag)) != NULL) {   // extract all items from bag and unionize into result
        iter_arg_t arg;
        arg.res = queryResult;
        counters_iterate(next, (void *) &arg, countersUnion);
        counters_delete(next);
    }

    sortPrint(queryResult, pageDir);    // sort and print result

    prep_return:    // return prep location that can be jumped to from anywhere in the fucntion
        if (queryList != NULL) {
            for (int i = 0; i < querySize; i++) {
                free(queryList[i]);
            }
            free(queryList);
        }
        if (queryResult != NULL) counters_delete(queryResult);
        if(bag != NULL) bag_delete(bag, (void (*)(void *)) counters_delete);
    return return_code;
}

/* helper function to reads from stdin, validates input and parses into a normalized query */
static void readParse(index_t *index, char *pageDir) {
    if (index == NULL || pageDir == NULL) {
        logMessage(1, "readParse: invalid arguments\n");
        return;
    }
    char **list = NULL;
    char *line = NULL;
    
    while( ( line = prompt() ) != NULL ) {  // prompt and check line
        for (int i = 0; i < strlen(line); i++) {    // validate the characters in  the line read
            if ( !isspace(line[i]) && !isalpha(line[i]) ) {
                printf("Invalid query\n");
                logMessage(3, "%s", "\nreadParse: query (", "%s", line, "%s", ") is an invalid query!\n");
                if (line != NULL) free(line);
                line = NULL;
                break;
            }
        }
        if (line == NULL) continue;
        
        int wordAppr = (int) (strlen(line) / 2) + 1; // number of words in line is at most #chars/2 + 1
        list = calloc( wordAppr, sizeof(char *) );
        if (list == NULL) { // ensure list is not null
            logMessage(3, "%s", "\nreadParse: failed to allocate ", "%d", wordAppr * sizeof(char *), "%s", "bytes for list\n");
            if (line != NULL) free(line);
            line = NULL;
            continue;
        }
        for (int i = 0; i < wordAppr; i++) {    // preset positions in list to null
            list[i] = NULL;
        }
        if (tokenize(list, line) == -1) {   // tokenize line and save words into list
            logMessage(2, "%s", "\nreadParse: failed to tokenize line :\n", "%s", line);
            if (line != NULL) free(line);
            line = NULL;
            if (list != NULL) free(list);
            line = NULL;
            continue;
        }
        printf("Normalized Query: ");
        for (int i = 0; i < wordAppr; i++) {
            if (list[i] != NULL) printf("%s ", list[i]);
        }
        printf("\n");
        query(index, pageDir, list);    // run words in list as query
        if (line != NULL) free(line);
        line = NULL;
    }
    printf("\n");

    if (line != NULL) free(line);
    line = NULL;
    if (list == NULL) free(list);
    list = NULL;
}

/* helper function to both prompt and read line */
static char *prompt() {
    if (isatty(fileno(stdin))) { 
        printf("Query? ");
    }
    return file_readLine(stdin);
}

/* function to tokenize a string by space and insert into a list */
static int tokenize(char **list, char *line) {
    if (list == NULL || line == NULL) {
        logMessage(1, "tokenize: Invalid arguments\n");
        return -1;
    }
    int wordStart;  // start of a word being read from line
    int wordEnd;    // current furthest point read in line
    int count;  // numnber of words inserted into the list
    int readingWord = false;    // value to check if a word is being read
    int lineLen = strlen(line); // lenght of line to tokenize
    wordStart = wordEnd = count = 0;

    for (; wordEnd < lineLen; wordEnd++) {  // loop throug line
        // end word being read if space is found or at last char in line
        if ( (isspace(line[wordEnd]) || wordEnd == lineLen - 1) && readingWord ) {
            if (wordEnd == lineLen - 1) wordEnd++;
            char *word = calloc((wordEnd - wordStart) + 1, sizeof(char));
            if (word == NULL) {
                return -1;
            }
            strncpy(word, &(line[wordStart]), wordEnd - wordStart);
            word[wordEnd - wordStart] = '\0';
            normalizeWord(word);    // normalize the word
            list[count] = word;
            count++;
            readingWord = false;
        } else if ( !isspace(line[wordEnd]) && !readingWord ) { // consume spaces if not reading a word
            wordStart = wordEnd;
            readingWord = true;
        }
    }
    return 0;
}

/* check if a query is valid */
static bool validateQuery(char **query, int querySize) {
    if (querySize < 0 || query == NULL) {   // validate arguments
        logMessage(1, "validateQuery: Invalid arguments\n");
    }
    if (isOP(query[0])) {   // ensure first word is not an op
        printf("'%s' cannot be first\n", query[0]);
        return false;
    } else if (isOP(query[querySize - 1])) {    // ensure last word is not an op
        printf("'%s' cannot be last\n", query[0]);
        return false;
    }

    char *prev = NULL;
    for (int i = 0; i < querySize; i++) {   // ensure rest of query is valid
        if (isOP(query[i]) && isOP(prev)) {
                printf("'%s' and '%s' cannot be adjacent\n", prev, query[i]);
                return false;
        }
        prev = query[i];
    }
    logMessage(1, "\nvalidateQuery: query is valid\n");
    return true;
}

/* helper function to check if a word is an op (and , or) */
static bool isOP(char *word) {
    if (word == NULL) { // validate arguments
        logMessage(1, "Invalid arguments\n");
        return false;
    }
    return strncmp("and", word, 3) == 0 || strncmp("or", word, 2) == 0;
}

/* helper function to iterate through 2 counters and perfom an intersect them */
static void countersIntersect(void *arg, const int key, const int val1) {
    if (arg == 0 || key < 0 || val1 < 0) return;    // validate arguments
    iter_arg_t *args = (iter_arg_t *) arg;  // cast to argument struct
    int min_val;
    int val2 = counters_get(args->other, key);   // get value (count) of key in other counters
    min_val = val2 < val1 ? val2 : val1;    // find the min of both values
    // as key is from first counters keys outside the intersect will be valued at zero
    counters_set(args->res, key, min_val);  // set value to min in both counters
}

/* helper function to make a uninon of counters */
static void countersUnion(void *arg, const int key, const int val1) {
    if (arg == 0 || key < 0 || val1 < 0) return;    // validate arguments
    iter_arg_t *args = (iter_arg_t *) arg;  // cat to argument struct
    int val2 = counters_get(args->res, key);    // get value in other counters
    int res = val1 + val2;  // union is the sum of value in both counters
    if (res != 0) { // only set non-zero values to reduce redundancy
        counters_set(args->res, key, res);
    }
}

/* helper function to sort and print the values in a counter */
static int sortPrint(counters_t *scores, char *pageDir) {
    if (scores == NULL || pageDir == NULL) { // validate arguments
    logMessage(1, "sortPrint: Invalid arguments\n");
        return -1;
    }
    iter_arg_t args;
    args.argNode = NULL;
    args.count = 0;
    counters_iterate(scores, (void *) &args, sortIterate);  // iterate through scores counters and sort them
    printf("Matches %d documents (ranked):\n", args.count);
    if (args.argNode == NULL) { // ensure sort word or there is at least one item
        return -1;
    }
    for (lnode_t *node = (lnode_t *) args.argNode; node != NULL; ) {    // loop and print score
        char *url = getPageUrl(pageDir, node->values[0]);
        if (url != NULL) {
            printf("score\t%d doc %d: %s\n", node->values[1], node->values[0], url);
        }
        lnode_t *tmp = node;
        node = node->next;
        if (tmp != NULL) free(tmp);
        if (url != NULL) free(url);
    }
    return 0;
}

/* helper function to be user to iterate when sorting counter */
static void sortIterate(void *arg, const int key, const int val) {
    if (arg == NULL || key < 0 || val < 0) return;
    iter_arg_t *args = (iter_arg_t *) arg;  // cast to argument struct
    args->count++;  // increment count
    lnode_t *node;
    lnode_t *next = (lnode_t *) calloc(1, sizeof(lnode_t)); // make space for new node
    next->next = NULL;
    next->values[0] = key;
    next->values[1] = val;
    if (args->argNode == NULL) {    // if this is the first node
        args->argNode = next;
        return;
    }
    node = args->argNode;

    if (node->values[1] <= next->values[1]) {   // if new node should be the first node
        args->argNode = next;
        next->next = node;
    } else {
        do {    // loop thoguh nodes and insert node as appropriate
            if (node->next == NULL || node->next->values[1] <= next->values[1]) {
                next->next = node->next;
                node->next = next;
                break;  // loop should only ever escape at this point
            }
            node = node->next;
        } while( node != NULL ); // should never resolve this condition
    }
}

/* helper function to cpy a counter through iteration */
static void copyIter(void *arg, const int key, const int val) {
    if (arg == NULL || key < 0 || val < 0) return;
    iter_arg_t *args = (iter_arg_t *) arg;  // cast to argument structy
    counters_t *counters = args->res;   // get other counters object
    counters_set(counters, key, val);   // set value of key as same in counters object
}

/* functinn to print pessages only when in DEV or TEST modes */
static void logMessage(const int argc, ...) {
    #ifdef TEST
        if (argc <= 0) {
            fprintf(stderr, "\n");
            return;
        }
        va_list argv;
        va_start(argv, argc);
        int argi;
        
        for (argi = 1; argi <= argc; argi ++) {
            char *argt = va_arg(argv, char *);
            if (strncmp(argt, "%lu", strlen(argt)) == 0) {
                fprintf(stderr, "%lu", va_arg(argv, unsigned long));
            } else if(strncmp(argt, "%s", strlen(argt)) == 0) {
                fprintf(stderr, "%s",  va_arg(argv, char *));
            } else if(strncmp(argt, "%d", strlen(argt)) == 0) {
                fprintf(stderr, "%d", va_arg(argv, int));
            } else if(strncmp(argt, "%c", strlen(argt)) == 0) {
                fprintf(stderr, "%c", (char) va_arg(argv, int));
            }
        }
    #endif
}
