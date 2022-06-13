#! /bin/bash


# Testing indexer with letters-1
export output=$($1 ./indexer letters-1 index.txt 2>&1)
if [[ $1 == "" ]]
then
    echo "TEST PASSED! ./indexer letters-1 index.txt"
elif [[ $output == *"All heap blocks were freed"*"0 errors"* ]]
then
    echo "TEST PASSED! ./indexer letters-1 index.txt"
else
    echo "TEST FAILED: Valgrind errors. ./indexer letters-1 index.txt "
fi
export output=$($1 ./indextest index.txt nindex.txt 2>&1)
if [[ $1 == "" && $output == *"TEST PASSED"* ]]
then
    echo "TEST PASSED! ./indextest index.txt nindex.txt"
elif [[ $1 == "" ]]
then
    echo "TEST FAILED! ./indextest index.txt nindex.txt"
elif [[ $output == *"TEST PASSED"*"All heap blocks were freed"*"0 errors"* ]]
then
    echo "TEST PASSED! ./indextest index.txt nindex.txt"
else
    echo "TEST FAILED: Valgrind errors. ./indextest index.txt nindex.txt"
fi


# Testing indexer with letters-10
export output=$($1 ./indexer ../../shared/tse/output/letters-10 index.txt 2>&1)
if [[ $1 == "" ]]
then
    echo "TEST PASSED! ./indexer letters-10 index.txt"
elif [[ $output == *"All heap blocks were freed"*"0 errors"* ]]
then
    echo "TEST PASSED! ./indexer letters-10 index.txt"
else
    echo "TEST FAILED: Valgrind errors. ./indexer letters-10 index.txt"
fi
export output=$($1 ./indextest index.txt nindex.txt 2>&1)
if [[ $1 == "" && $output == *"TEST PASSED"* ]]
then
    echo "TEST PASSED! ./indextest index.txt nindex.txtt"
elif [[ $1 == "" ]]
then
    echo "TEST FAILED!"
elif [[ $output == *"TEST PASSED"*"All heap blocks were freed"*"0 errors"* ]]
then
    echo "TEST PASSED! ./indextest index.txt nindex.txt"
else
    echo "TEST FAILED: Valgrind errors. ./indextest index.txt nindex.txt"
fi


# Testing indexer with toscrape-1
export output=$($1 ./indexer ../../shared/tse/output/toscrape-1 index.txt 2>&1)
if [[ $1 == "" ]]
then
    echo "TEST PASSED! ./indexer toscrape-1 index.txt"
elif [[ $output == *"All heap blocks were freed"*"0 errors"* ]]
then
    echo "TEST PASSED! ./indexer toscrape-1 index.txt"
else
    echo "TEST FAILED: Valgrind errors. ./indexer toscrape-1 index.txt"
fi
export output=$($1 ./indextest index.txt nindex.txt 2>&1)
if [[ $1 == "" && $output == *"TEST PASSED"* ]]
then
    echo "TEST PASSED! ./indextest index.txt nindex.txt"
elif [[ $1 == "" ]]
then
    echo "TEST FAILED! ./indextest index.txt nindex.txt"
elif [[ $output == *"TEST PASSED"*"All heap blocks were freed"*"0 errors"* ]]
then
    echo "TEST PASSED! ./indextest index.txt nindex.txt"
else
    echo "TEST FAILED: Valgrind errors. ./indextest index.txt nindex.txt"
fi

# Testing indexer with wikipedia-3
export output=$($1 ./indexer ../../shared/tse/output/wikipedia-3 index.txt 2>&1)
if [[ $1 == "" ]]
then
    echo "TEST PASSED! ./indexer wikipedia-3 index.txt"
elif [[ $output == *"All heap blocks were freed"*"0 errors"* ]]
then
    echo "TEST PASSED! ./indexer wikipedia-3 index.txt"
# else
    echo "TEST FAILED: Valgrind errors. ./indexer wikipedia-3 index.txt"
fi
export output=$($1 ./indextest index.txt nindex.txt 2>&1)
if [[ $1 == "" && $output == *"TEST PASSED"* ]]
then
    echo "TEST PASSED! ./indextest index.txt nindex.txt"
elif [[ $1 == "" ]]
then
    echo "TEST FAILED! ./indextest index.txt nindex.txt"
elif [[ $output == *"TEST PASSED"*"All heap blocks were freed"*"0 errors"* ]]
then
    echo "TEST PASSED! ./indextest index.txt nindex.txt"
else
    echo "TEST FAILED: Valgrind errors. ./indextest index.txt nindex.txt"
fi


# testing with wrong number of inputs
export output=$($1 ./indexer ../../shared/tse/output/letters-1 2>&1)
if [[ $1 == "" && $output == *"Usage: indexer <pageDir> <indexFile>"* ]]
then
    echo "TEST PASSED! ./indexer letters-1"
elif [[ $output == *"Usage: indexer <pageDir> <indexFile>"*"All heap blocks were freed"*"0 errors"* ]]
then
    echo "TEST PASSED! ./indexer letters-1"
else
    echo "TEST FAILED: Valgrind errors. ./indexer letters-1"
fi
export output=$($1 ./indextest index.txt 2>&1)
if [[ $1 == "" && $output == *"Usage: ./indextest <oldindexfile> <newindexfile>"* ]]
then
    echo "TEST PASSED! ./indextest index.txt"
elif [[ $1 == "" ]]
then
    echo "TEST FAILED! ./indextest index.txt"
elif [[ $output == *"Usage: ./indextest <oldindexfile> <newindexfile>"*"All heap blocks were freed"*"0 errors"* ]]
then
    echo "TEST PASSED! ./indextest index.txt"
else
    echo "TEST FAILED: Valgrind errors. ./indextest index.txt"
fi


# testing with non-crawler directory
export output=$($1 ./indexer ../../shared/ index.txt 2>&1)
if [[ $1 == "" && $output == *"Bag Arguments"* ]]
then
    echo "TEST PASSED! ./indexer ../../shared/ index.txt"
elif [[ $output == *"Bag Arguments"*"All heap blocks were freed"*"0 errors"* ]]
then
    echo "TEST PASSED! ./indexer ../../shared/ index.txt"
else
    echo "TEST FAILED: Valgrind errors../indexer ../../shared/ index.txt"
fi


exit 0
