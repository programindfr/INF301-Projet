CC=clang
CFLAGS=-Wall -Wextra -Werror -gdwarf-4
LDFLAGS=-lm

EXEC= client-Labyrinthe

all: $(EXEC)

client-Labyrinthe: client-Labyrinthe.o client.o

client.o: client.c client.h

clean:
	rm -f *.o $(EXEC)
