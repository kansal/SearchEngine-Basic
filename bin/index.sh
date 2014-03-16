#!/bin/bash
./main $1 $2
sort $2/parse.txt > $2/sorted.txt
g++ ../src/merge.cpp
./a.out .
python ../src/sorting.py $2 open.txt
python ../src/secondaryDoc.py $2 docWord.txt
python ../src/secondaryVocab.py $2 vocabulary.txt
sort $2/docWord.txt >  sortedDocTitle.txt

