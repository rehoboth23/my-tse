# Rehoboth Okorie
## CS50 Spring 2021, Lab 6

```
****************
* 2 EXTENSIONS USED
****************
```

### Querier
The `querier` program load and indexer from a file and read queries to run against the loaded indexer and provide ranked results
Usage: ./querier <pageDirectory> <indexFilename> 
- pageDir: is the pathname to a crawler directory
- indexFile: is the pathname of the file to load the index from



### Implementation
```c
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
```

### Assumptions
No assumptions beyond those that are clear from the spec.

### Compilation
```bash
make -> to make all targets (querier)
make test -> to run testing script
make valgrind -> to run testing script with valgrind memory check
```

### Testing
The `testing.sh` script tests querier.c
To enable error logging set `TEST=-DTEST` in make file or run `export TEST=-DTEST` in command line before compiling
