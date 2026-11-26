CC = gcc
CFLAGS = -Wall -Wextra -std=gnu99

TARGETS = main kierownik kasjer kierowca

all: $(TARGETS)

main: main.c common.h
	$(CC) $(CFLAGS) -o main main.c

kierownik: kierownik.c common.h
	$(CC) $(CFLAGS) -o kierownik kierownik.c

kasjer: kasjer.c common.h
	$(CC) $(CFLAGS) -o kasjer kasjer.c

kierowca: kierowca.c common.h
	$(CC) $(CFLAGS) -o kierowca kierowca.c

clean:
	rm -f $(TARGETS)
