all: q1 q2
q1:
	gcc q1.c -std=c99 -Wall -pedantic -o q1
q2:
	gcc q2.c -std=c99 -Wall -pedantic -o q2
