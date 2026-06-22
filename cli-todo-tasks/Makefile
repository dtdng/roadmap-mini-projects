CC=g++
CLAGS=-c -Wall
LFLAGS=-o

SOURCES=./src/*
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=todo-cli

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LFLAGS) $@ $^

.cpp.o:
	$(CC) $(CFLAGS) $<

clean:
	rm -f *.o $(EXECUTABLE)

run: $(EXECUTABLE)
	./$(EXECUTABLE)