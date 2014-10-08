CC=gcc
CFLAGS=-c -std=c99
LFLAGS=-lncurses -lpthread
SOURCES=main.c fractal.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=fractal


all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@ $(LFLAGS)

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)
