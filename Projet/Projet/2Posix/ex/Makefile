CC = gcc -Wall
FLAGR = -D_REENTRANT
LDFLAG = -lpthread

all : aio ex1 ex2 ex3 mess thr1 thr2 thr3 thr4 thr5 sig1 sig2 tim1 tim2

# faire make sansmess si erreur sur mess
#
sansmess : aio ex1 ex2 ex3 thr1 thr2 thr3 thr4 thr5 sig1 sig2 tim1 tim2

aio : aio.c
	${CC} -o aio aio.c -lrt

ex1 : ex1.c
	${CC} -o ex1 ex1.c

ex2 : ex2.c
	${CC} -o ex2 ex2.c

ex3 : ex3.c
	${CC} -o ex3 ex3.c

sig1 : sig1.c
	${CC} -o sig1 sig1.c

sig2 : sig2.c
	${CC} -o sig2 sig2.c

tim1 : tim1.c
	${CC} -o tim1 tim1.c -lrt

tim2 : tim2.c
	${CC} -o tim2 tim2.c -lrt

mess : mess.c
	${CC} ${FLAGR} -o mess mess.c ${LDFLAG} -lrt

thr1 : thr1.c
	${CC} ${FLAGR} -o thr1 thr1.c ${LDFLAG}

thr2 : thr2.c
	${CC} ${FLAGR} -o thr2 thr2.c ${LDFLAG}

thr3 : thr3.c
	${CC} ${FLAGR} -o thr3 thr3.c ${LDFLAG}

thr4 : thr4.c
	${CC} ${FLAGR} -o thr4 thr4.c ${LDFLAG}

thr5 : thr5.c
	${CC} ${FLAGR} -o thr5 thr5.c ${LDFLAG}

clean :
	rm -f aio ex1 ex2 ex3 mess thr1 thr2 thr3 thr4 thr5 sig1 sig2 tim1 tim2 *~ fic.txt
