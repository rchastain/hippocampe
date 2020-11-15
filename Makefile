CC=gcc -g
LDFLAGS=-lpthread
CFLAGS= 

all: hippocampe

hippocampe: main.o entree.o liste.o partie.o recherche.o regles.o sortie.o temps.o
	$(CC) -o hippocampe main.o entree.o liste.o partie.o recherche.o regles.o sortie.o temps.o $(LDFLAGS)

main.o: main.c hippocampe.h entree.h sortie.h temps.h
	$(CC) -o main.o -c main.c $(CFLAGS)

entree.o: entree.c entree.h hippocampe.h sortie.h partie.h recherche.h regles.h temps.h
	$(CC) -o entree.o -c entree.c

liste.o: liste.c hippocampe.h
	$(CC) -o liste.o -c liste.c $(CFLAGS)
 
partie.o: partie.c liste.h recherche.h regles.h sortie.h temps.h
	$(CC) -o partie.o -c partie.c $(CFLAGS)

recherche.o: recherche.c hippocampe.h regles.h recherche.h sortie.h temps.h
	$(CC) -o recherche.o -c recherche.c $(CFLAGS)

regles.o: regles.c liste.h regles.h partie.h hippocampe.h
	$(CC) -o regles.o -c regles.c $(CFLAGS)

sortie.o: sortie.c regles.h
	$(CC) -o sortie.o -c sortie.c $(CFLAGS)

temps.o: temps.c hippocampe.h sortie.h temps.h entree.h
	$(CC) -o temps.o -c temps.c $(CFLAGS)

clean:
	rm -rf *.o
