CC = gcc
CFLAGS = -Wall -Wextra -std=gnu99

TARGETS = main kasjer

all: $(TARGETS)

main: main.c common.h
	$(CC) $(CFLAGS) -o main main.c


kasjer: kasjer.c common.h
	$(CC) $(CFLAGS) -o kasjer kasjer.c

clean:
	rm -f $(TARGETS)
