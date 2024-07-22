CC = gcc
SRC = pbuffer-agenda.c
TARGET = pbuffer-agenda
CFLAGS = -Wall -Wextra -g

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean

