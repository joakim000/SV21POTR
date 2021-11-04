$(info $(SHELL))
# $(info $(PATH))

.DELETE_ON_ERROR :

LFLAGS = 
ifdef PROF
CFLAGS = -std=c11 -O3 -pg -g
LFLAGS = -pg
else
ifdef DEBUG
CFLAGS = -std=c11 -O0 -g
else
CFLAGS = -std=c11 -O3 
endif
endif

crc15objects = crc15.o crc.o binutils.o cmdargs.o 
objects = main.o crc.o engine.o jlibc/binutils.o jlibc/cmdargs.o jlibc/datagenerator.o jlibc/files.o 


extra : $(objects)
	cc $(LFLAGS) -o crc $(objects)

crc15 : $(crc15objects)
	cc $(LFLAGS) -o crc15 $(crc15objects)

# Program
main.o : main.c crc_zoo.c crc.h  jlibc/common.h jlibc/binutils.h jlibc/cmdargs.h jlibc/datagenerator.h jlibc/files.h
	cc -c $(CFLAGS) main.c
crc.o : crc.c crc.h engine.h jlibc/common.h jlibc/binutils.h 
	cc -c $(CFLAGS) crc.c
engine.o : engine.c engine.h  
	cc -c $(CFLAGS) engine.c

# jlibc
binutils.o : jlibc/binutils.c jlibc/binutils.h jlibc/common.h  
	cc -c $(CFLAGS) jlibc/binutils.c
cmdargs.o : jlibc/cmdargs.c jlibc/cmdargs.h jlibc/common.h
	cc -c $(CFLAGS) jlibc/cmdargs.c
#da.o :  jlibc/da.c jlibc/da.h jlibc/common.h
#	cc -c $(CFLAGS) jlibc/da.c
files.o : jlibc/files.c jlibc/files.h  jlibc/common.h 
	cc -c $(CFLAGS) jlibc/files.c
datagenerator.o : jlibc/datagenerator.c jlibc/datagenerator.h  jlibc/common.h 
	cc -c $(CFLAGS) jlibc/datagenerator.c

#old
crc15.o : crc15.c crc.h crc_zoo.c jlibc/binutils.h jlibc/cmdargs.h 
	cc -c $(CFLAGS) crc15.c


debug :
	$(MAKE) DEBUG=1

prof :
	$(MAKE) PROF=1

.PHONY : clean
clean : 
	$(info $(OS))
#	ifeq ($(OS),Windows_NT)
#	clean ::
		powershell rm *.exe 
		powershell rm *.o
		powershell rm jlibc/*.o
#	else
#	clean ::
#		rm -f bench *.o
#	endif
	
.PHONY : debclean
debclean : 
	rm -f crc *.o jlibc/*.o