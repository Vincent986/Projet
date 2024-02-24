# Les parametres de compilation et edition des liens
# pour compiler un programme sous GNU/Linux+Xenomai

CFLAGS=-Wall $(shell xeno-config --skin=native --cflags)
LDFLAGS= $(shell xeno-config --skin=native --ldflags)

exeno1: exeno1.o

clean:
	rm -f *.o exeno1


