cflags = -c -std=c11 -O3 -g
objects = run.o sorting.o
sorting_files = sorting.c sorting.h
.DELETE_ON_ERROR :

run : $(objects)
	cc -o run $(objects)

run.o : run.c $(sorting_files)
	cc $(cflags) run.c
sorting.o : $(sorting_files)
	cc $(cflags) sorting.c
	
clean : rm run.exe run.o sorting.o
