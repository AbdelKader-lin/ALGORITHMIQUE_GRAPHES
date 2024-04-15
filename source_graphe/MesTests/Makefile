CC=gcc
CFLAGS=-Wall -g

all: TestDijkstra

# Lien final
TestDijkstra: TestDijkstra.o ../graphe.o ../pile.o ../file.o ../abr.o
	$(CC) $(CFLAGS) -o $@ $^

# Compilation des sources
TestDijkstra.o: TestDijkstra.c ../graphe.h
	$(CC) $(CFLAGS) -c $< -o $@

../graphe.o: ../graphe.c ../graphe.h
	$(CC) $(CFLAGS) -c $< -o $@

../pile.o: ../pile.c ../pile.h
	$(CC) $(CFLAGS) -c $< -o $@

../file.o: ../file.c ../file.h
	$(CC) $(CFLAGS) -c $< -o $@

../abr.o: ../abr.c ../abr.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o ../graphe.o ../pile.o ../file.o ../abr.o TestDijkstra
