# Rehoboth Okorie
## CS50 Spring 2021, Lab 6

# Querier

# Design Spec
According to the Querier Requirements Spec, the TSE querier is a standalone program that reads the index file produced by the TSE Indexer, and page files produced by the TSE Querier, and answers search queries submitted via stdin.

User interface
The queriers has two interfaces with the user on the command-line
- The arguments. It must always have two arguments.
- It may also read queries from the command line if no other input source is specified

**Usage**: ./querier \<pageDirectory> \<indexFilename>

`$ ./querier ../data/pageDir ../data/file.index`
**Input**: 
- The querier reads an index file from the `file.index`. It will also extract the url from the documnets in `pageDir` when printing ranked output

- The querier reads queries from a specifiec source (usually stdin). The query is composed of only alphabetical characters. Each query is terminated by a new line feed. An end of file feed determines end of all input.

**Output**: 
- Ranked outputs of files scored on the degree to which the match the query

Functional decomposition into modules
We anticipate the following modules or functions:

main: which parses arguments and initializes other modules;
parseArgs: this would parse and validate the command line inputs  
readParse: this would prompt and read the queries from the input source. it would also invoke the module to clean and process the query
query: this would process a query; rank and print the result
tokenize: this would break up a line into its component words
isOP: this check if a word is a reserved operator (or, and)
validateQuery: this validates that a provided query adheres to the query syntax. see requirements spec for more information on query syntax

### Pseduo Logic/Control flow

*Main and parseArgs*
```
parse the command line, validate parameters, initialize other modules
call readParse, with pageDirectory and loaded index.
```

*readParse*
```
this will loop, prompinting for queries ,until end of file feed
It will call tokenizer on the read line to seperate it into a words and stores the word into a list
query is the called with the index, pagedir and word list
```


*query*
```
validates the words in the word list make up a valid query
processes the query to build discreet 'and' logic blocks which are stored in a bag
the logic block are unionized into one block (the bag represents a series of 'or' operations)
the values in the bag are sorted by rank and printed
```

**Testing plan**
Unit testing. testing.sh runs the querier with cuutom input dericed from files in the `/shared/tse/outputs/*` directory.
Run valgrind (using make valgrind) to run the unit tests with valgrind memory check