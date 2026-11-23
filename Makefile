CC = gcc
CFLAGS = -Wall -Wextra -std=gnu99

TARGET = main

all: $(TARGET)

$(TARGET): main.c common.h
	$(CC) $(CFLAGS) -o $(TARGET) main.c

clean:
	rm -f $(TARGET) *.o
