# Rehoboth Okorie
## CS50 Spring 2021, Lab 5

### Indexer
The `indexer` program creates an index using the files produced by the crawler saves the index to a file.
`Usage: indexer <pageDir> <indexFile>`
- pageDir: is the pathname to a crawler directory
- indexFile: is the pathname the file to save the index

### indextest
The `indextest` program reads an index file and load an index object using the file. It saves this to a new file.
`Usage: Usage: ./indextest <oldindexfile> <newindexfile>`
- oldindexfile: is the pathname to the file to read and load index from
- newindexfile: is the pathname the file to save the index


### Implementation
```c
int main(int argc, char const *argv[]);

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

```

### Assumptions
No assumptions beyond those that are clear from the spec.

### Compilation
```bash
make -> to make all targets (indexer and indestext)
make test -> to run testiung script
make valgrind -> to run testing script with valgrind memory check
```

### Testing
The `testing.sh` script tests indexer.c
To enable error logging set `TEST=-DTEST` in make file or run `export TEST=-DTEST` in command line
