CC = g++ -std=c++0x -O3
CFLAGS = -Wall
LINKER = g++

SRCS = main.cpp myparser.cpp porter2_stemmer.cpp
PROG = main
LIBS = `pkg-config --cflags --libs libxml++-2.6`

all: $(PROG)

$(PROG):	$(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)
