cflags = -c -std=c11 -O3 -g
objects = sorting.o sorting_lib.o
sorting_lib = sorting_lib.c sorting_lib.h
.DELETE_ON_ERROR :

run : $(objects)
	cc -o run $(objects)

sorting.o : sorting.c $(sorting_lib)
	cc $(cflags) sorting.c
sorting_lib.o : $(sorting_lib)
	cc $(cflags) sorting_lib.c

clean : rm run.exe 
		rm sorting_lib.o
		rm sorting.o


