run : run.o sorting.o
	cc -std=c11 -O3 -g -o run run.o sorting.o

run.o : run.c sorting.c sorting.h
	cc -c -std=c11 -O3 -g run.c
sorting.o : sorting.c sorting.h
	cc -c -std=c11 -O3 -g sorting.c
clean : rm run.exe run.o sorting.o
