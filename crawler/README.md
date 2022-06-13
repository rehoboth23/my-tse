# Rehoboth Okorie
## CS50 Spring 2021, Lab 4

# Crawler
## Files
- crawler.c: crawler is a standalone program that crawls the web and retrieves webpages starting from a "seed" URL.
    ```c
    int main(const int argc, char* argv[]);
    static int parseArgs(const int argc, char* argv[],
                        char** seedURL, char** pageDirectory, int* maxDepth);
    static int crawl(char* seedURL, char* pageDirectory, const int maxDepth);
    static int pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);
    ```


## Notes
- **Usage**: `./crawler <seedURL> <pageDirectory> <maxDepth>`
- **CrawlerCoeff**: This is the number of slots that the hashtable has. This can be changed by setting `FLAGS=... -DCrawlerCoeff=<Value>` in the make file.
- **Test Logs**: The program is designed to only log error and output when in compiled for testing. This can be done by setting `FLAGS=... -DTEST` in the make file.

## Error Handling
Funtions in the module return an integer value. 0 for success and -1 for failure. When necessary, this value is used to appropriately handle an error or in extreme cases, fail gracefully.

## Testing
Testing can be done with `make test` for normal tests and `make valgrind` for testing with valgrind. The testing script `testing.sh` parses the valgrind output and looks for certain signatures in the output (valgrind logs) to determine success. Normal tests check only for correctness e.g `All block freed, 0 errors, etc.`. Valgrind tests check for both correctness and proper dynamic memory usage.