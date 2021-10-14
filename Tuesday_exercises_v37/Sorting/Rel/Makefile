$(info $(SHELL))
# $(info $(PATH))

.DELETE_ON_ERROR :

WIN = if windows
NOWIN = if not windows

ifdef OPT
CFLAGS = -c -std=c11 -O3
else
CFLAGS = -c -std=c11 -O0 -g
endif

objects = run.o sorting.o

default : $(objects)
	cc -o run $(objects)

run.o : run.c options.h run.h sorts.h 
	cc $(CFLAGS) run.c
sorting.o : sorting.c sorting.h
	cc $(CFLAGS) sorting.c

opt :
	$(MAKE) OPT=1

.PHONY : clean
clean : 
	$(info $(OS))
#	ifeq ($(OS),Windows_NT)
#	clean ::
#		$(info "if windows")
		powershell rm *.exe 
		powershell rm *.o
#	else
#	clean ::
#		$(info "if not windows")
#		rm run *.o
#	endif
	
