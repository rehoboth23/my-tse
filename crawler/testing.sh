#! /bin/bash
# Testing script for crawler module
# Author: Rehoboth Okorie (Feb 8 2022)

echo "TESTING CRAWLER"
idx=0


echo "TESTING FUNCTIONALITY -> does it work"
mkdir ../test 2> /dev/null

echo "$1 ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../test 2"
export output=$($1 ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../test 2 2>&1)
for var in {1..3}
do
    if [[ ! -e "../test/$var" ]]
    then
        idx=1
        echo "TEST FAILED: Cannot find expected files"
        break
    fi
done
if [[ $idx == 0 ]]
then
    if [[ $1 == "" ]]
    then
        echo "TEST PASSED!"
    elif [[ $output == *"All heap blocks were freed"*"0 errors"* ]]
    then
       
        echo "TEST PASSED!"
    else
       
        echo "TEST FAILED: valgrind errors."
    fi
fi

idx=0

yes| rm - r ../test/* 2> /dev/null

echo "$1 ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/D.html ../test 3"
export output=$($1 ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/D.html ../test 3 2>&1)

for var in {1..4}
do
    if [[ ! -e "../test/$var" ]]
    then  
        
        idx=1
        echo "TEST FAILED: Cannot find expected files"
        break
    fi
done
if [[ $idx == 0 ]]
then
    if [[ $1 == "" ]]
    then
       
        echo "TEST PASSED!"
    elif [[ $output == *"All heap blocks were freed"*"0 errors"* ]]
    then
       
        echo "TEST PASSED!"
    else
       
        echo "TEST FAILED: valgrind errors."
    fi
fi

idx=0

yes| rm - r ../test/* 2> /dev/null

echo "$1 ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/E.html ../test 4"
export output=$($1 ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/E.html ../test 4 2>&1)

for var in {1..9}
do
    if [[ ! -e "../test/$var" ]]
    then
       
        idx=1
        echo "TEST FAILED: Cannot find expected files"
        break
    fi
done
if [[ $idx == 0 ]]
then
    if [[ $1 == "" ]]
    then
       
        echo "TEST PASSED!"
    elif [[ $output == *"All heap blocks were freed"*"0 errors"* ]]
    then
       
        echo "TEST PASSED!"
    else
       
        echo "TEST FAILED: valgrind errors."
    fi
fi

idx=0

yes| rm - r ../test/* 2> /dev/null

echo "$1 ./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/ ../test 1"
export output=$($1 ./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../test 1 2>&1)

for var in {1..73}
do
    if [[ ! -e "../test/$var" ]]
    then
       
        idx=1
        echo "TEST FAILED: Cannot find expected files"
        break
    fi
done
if [[ $idx == 0 ]]
then
    if [[ $1 == "" ]]
    then
       
        echo "TEST PASSED!"
    elif [[ $output == *"All heap blocks were freed"*"0 errors"* ]]
    then
       
        echo "TEST PASSED!"
    else
       
        echo "TEST FAILED: valgrind errors."
    fi
fi

idx=0

yes| rm - r ../test/* 2> /dev/null

echo "$1 ./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/ ../test 2"
export output=$($1 ./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../test 2 2>&1)

for var in {1..585}
do
    if [[ ! -e "../test/$var" ]]
    then
       
        idx=1
        echo "TEST FAILED: Cannot find expected files"
        break
    fi
done
if [[ $idx == 0 ]]
then
    if [[ $1 == "" ]]
    then
       
        echo "TEST PASSED!"
    elif [[ $output == *"All heap blocks were freed"*"0 errors"* ]]
    then
       
        echo "TEST PASSED!"
    else
       
        echo "TEST FAILED: valgrind errors."
    fi
fi

idx=0

yes| rm - r ../test/* 2> /dev/null

echo "TESTING FUNCTIONALITY -> using invalid number of params"
export output=$($1 ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../pages 2>&1)

if [[ "$output" != *"Usage: ./crawler <seedURL> <pageDirectory> <maxDepth>"* ]]
then
   
    echo "TEST FAILED: Should print \"Usage: ./crawler <seedURL> <pageDirectory> <maxDepth>\" to stderr"
else
    if [[ $1 == "" ]]
    then
       
        echo "TEST PASSED!"
    elif [[ $output == *"All heap blocks were freed"*"0 errors"* ]]
    then
       
        echo "TEST PASSED!"
    else
       
        echo "TEST FAILED: valgrind errors."
        
    fi
fi

export output=$($1 ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html 2>&1)

if [[ "$output" != *"Usage: ./crawler <seedURL> <pageDirectory> <maxDepth>"* ]]
then
   
    echo "TEST FAILED: Should print \"Usage: ./crawler <seedURL> <pageDirectory> <maxDepth>\" to stderr"
else
    if [[ $1 == "" ]]
    then
       
        echo "TEST PASSED!"
    elif [[ $output == *"All heap blocks were freed"*"0 errors"* ]]
    then
       
        echo "TEST PASSED!"
        
    else
       
        echo "TEST FAILED: valgrind errors."
        
    fi
fi

echo "TESTING FUNCTIONALITY -> using wrong order of params"
export output=$($1 ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html 2 ../pages 2>&1)
if [[ "$output" != *"Usage: ./crawler <seedURL> <pageDirectory> <maxDepth>"* ]]
then
   
    echo "TEST FAILED: Should print \"Usage: ./crawler <seedURL> <pageDirectory> <maxDepth>\" to stderr"
else
    if [[ $1 == "" ]]
    then
       
        echo "TEST PASSED!"
    elif [[ $output == *"All heap blocks were freed"*"0 errors"* ]]
    then
       
        echo "TEST PASSED!"
        
    else
       
        echo "TEST FAILED: valgrind errors."
        
    fi
fi


exit 0;