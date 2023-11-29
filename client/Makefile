
CC = gcc
CFLAGS = -Wall -Wextra

SRCDIR = .
SOURCES := $(shell find $(SRCDIR) -name '*.c')
OBJECTS := $(SOURCES:.c=.o)
EXECUTABLE = client

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
