#! /bin/bash

# test correctness of the program
$1 ./querier ../../shared/tse/output/letters-2 ../../shared/tse/output/letters-2.index << END
   b
          B
     Home
homE
b or home
B HOME
Breadth first
search
Breadth first AND search
   BrEADth first     SEArch
END
echo
echo
$1 ./querier ../../shared/tse/output/letters-10 ../../shared/tse/output/letters-10.index<< END
Breadth first
search
Breadth first AND search
   BrEADth first     SEArch
Depth first
Computational and      biology
CompuTATional     BioLogy
search
Depth first AND search
   DEPth first  or   SEArch
END
echo
echo
$1 ./querier ../../shared/tse/output/toscrape-2 ../../shared/tse/output/toscrape-2.index << END
   football
harvard    
cOMPuter        Science
computer          and Science
computer or     science
computer and        science or       sCIENCE
END
echo
echo
$1 ./querier ../../shared/tse/output/wikipedia-1 ../../shared/tse/output/wikipedia-1.index << END
contributors and fame
esalen eberhart      or universal
and eberhart hop AND uses     members OR encompasses
spain OR brady AND compatible     cross OR today friends miss 
predictive division
lat 
takes OR translation AND authors
END
echo
echo

# ensure the program check that the arguments passed are valid
$1 ./querier ../../shared/tse/output/letters-2 ../../shared/tse/output/lett.index
echo
echo
$1 ./querier ../../shared/tse/output/lett ../../shared/tse/output/letters-2.index
echo
echo
# test that the program ensure the correct number of arguments
$1 ./querier ../../shared/tse/output/letters-2
echo
echo
$1 ./querier ../../shared/tse/output/letters-2 ../../shared/tse/output/letters-2.index bag_param
echo
echo
exit 0