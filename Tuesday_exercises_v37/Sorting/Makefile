$(info $(SHELL))
# $(info $(PATH))

.DELETE_ON_ERROR :

LFLAGS = 
ifdef PROF
CFLAGS = -pthread -std=c11 -O3 -pg -g
LFLAGS = -pg
else
ifdef OPT
CFLAGS = -pthread -std=c11 -O3
else
CFLAGS = -pthread -std=c11 -O0 -g
endif
endif

objects = bench.o sorting.o cmdargs.o

default : $(objects)
	cc $(LFLAGS) -pthread -o bench $(objects)

bench.o : bench.c bench.h options.h sort_struct.h sort_init.h cmdargs/cmdargs.h
	cc -c $(CFLAGS) bench.c
sorting.o : sorting.c sorting.h sort_struct.h
	cc -c $(CFLAGS) sorting.c
cmdargs.o : cmdargs/cmdargs.c cmdargs/cmdargs.h
	cc -c $(CFLAGS) cmdargs/cmdargs.c

opt :
	$(MAKE) OPT=1

prof :
	$(MAKE) PROF=1

.PHONY : clean
clean : 
	$(info $(OS))
#	ifeq ($(OS),Windows_NT)
#	clean ::
		powershell rm *.exe 
		powershell rm *.o
#	else
#	clean ::
#		rm -f bench *.o
#	endif
	
.PHONY : debclean
debclean : 
	rm -f bench *.o