$(info $(SHELL))
# $(info $(PATH))

.DELETE_ON_ERROR :

SRC = src/
BUILD = build/
LIB = lib/
UNITY = lib/unity/

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

objects = $(BUILD)test.o $(BUILD)cbuffer.o $(BUILD)unity.o


cbuffer : $(objects)
	cc $(LFLAGS) -o test $(objects)

# Test exe
$(BUILD)test.o : $(SRC)test.c $(LIB)cbuffer.h  
	cc -c $(CFLAGS) $(SRC)test.c -o $(@D)/test.o -I$(LIB) -I$(UNITY)	

# Lib
$(BUILD)cbuffer.o : $(LIB)cbuffer.c $(LIB)cbuffer.h
	cc -c $(CFLAGS) $(LIB)cbuffer.c -o $(@D)/cbuffer.o -I$(LIB)

# Unity
$(BUILD)unity.o : $(UNITY)unity.c $(UNITY)unity.h
	cc -c $(CFLAGS) $(UNITY)unity.c -o $(@D)/unity.o -I$(UNITY)



.PHONY : debug
debug :
	$(MAKE) DEBUG=1

.PHONY : prof
prof :
	$(MAKE) PROF=1

.PHONY : clean
clean : 
	rm -f $(BUILD)*.o
	
.PHONY : winclean
winclean :
	powershell rm $(BUILD)*.o

.PHONY : superclean
superclean:
	rm -f $(BUILD)*.o
	rm -f cbuffer
	rm -f cbuffer.exe

#.PHONY : cleanportable
#cleanportable : 
#	$(info $(OS))
#ifeq ("$(OS)","Windows_NT")
#cleanportable ::
#	powershell rm $(BUILD)*.o
#else
#cleanportable ::
#	rm -f $(BUILD)*.o
#endif


# Notes
