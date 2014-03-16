#!/bin/bash
apt-get install libxml++2.6-dev libxml++2.6-doc #libxml++ API
apt-get install libboost1.48-all-dev            #boost Library
make                                            #make file for compiling the indexing part
g++ -std=c++0x -Wall ../src/search.cpp ../src/porter2_stemmer.cpp `pkg-config --cflags --libs libxml++-2.6`-lrt #for compiling the query part

