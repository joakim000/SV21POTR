run : sorting.o sorting_lib.o
	cc -std=c11 -g -o run sorting.o sorting_lib.o

sorting.o : sorting.c sorting_lib.c sorting_lib.h
	cc -c -std=c11 -O3 -g sorting.c
sorting_lib.o : sorting_lib.c sorting_lib.h
	cc -c -std=c11 -O3 -g sorting_lib.c
clean : rm run.exe sorting_lib.o sorting.o
