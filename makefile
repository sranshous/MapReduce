CC=g++
CFLAGS=-g -Wall -std=c++0x -pg -O3
LDFLAGS=-pg
EXEC=main
DEPS=mapreduce.h writer.h
SOURCES=main.cpp mapreduce.cpp
OBJECTS=$(SOURCES:.cpp=.o)
OUTPUTS=*.intermediate *.final

all: $(SOURCES) $(EXEC)

%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

clean:
	rm -f $(OBJECTS) $(OUTPUTS)
