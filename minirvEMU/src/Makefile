CFLAGS=-g -O2 -Wall -rdynamic -DNDEBUG 

# The Sources
SOURCES=$(wildcard ./**/*.c ./*.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))

MAIN_SOURCE = $(wildcard ./*.c)
TARGET = $(patsubst %.c,%,$(MAIN_SOURCE))

# The Target Build
all: $(OBJECTS) compiler

compiler: $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $(TARGET) 

# The Cleaner
clean:
	rm -f $(OBJECTS) $(TARGET)

