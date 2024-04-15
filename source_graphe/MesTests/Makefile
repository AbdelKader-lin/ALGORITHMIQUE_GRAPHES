all: TestDijkstra


TestDijkstra: TestDijkstra.o ../graphe.o ../pile.o ../file.o ../abr.o
	gcc -o TestDijkstra TestDijkstra.o ../graphe.o ../pile.o ../file.o ../abr.o

TestDijkstra.o : TestDijkstra.c ../graphe.c ../graphe.h
	gcc -Wall -c TestDijkstra.c





clean:
	rm -f *.o TestDijkstra