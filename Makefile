CC      = gcc
CFLAGS  = -std=c11 -Wall -Wextra -g
TARGET  = msh

all: $(TARGET)

$(TARGET): msh.c
	$(CC) $(CFLAGS) -o $(TARGET) msh.c

clean:
	rm -f $(TARGET) *.o

.PHONY: all clean
