# $(info $(SHELL))
# $(info $(PATH))

.DELETE_ON_ERROR :

SRC = src/
TEST = test/
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

objects = $(BUILD)test_cbuffer.o $(BUILD)cbuffer.o $(BUILD)unity.o


test_cbuffer : $(objects)
	cc $(LFLAGS) -o bin/test_cbuffer $(objects)

# Cbuffer test 
$(BUILD)test_cbuffer.o : $(TEST)test_cbuffer.c $(LIB)cbuffer.h  
	cc -c $(CFLAGS) $(TEST)test_cbuffer.c -o $(@D)/test_cbuffer.o -I$(LIB) -I$(UNITY)	

# Cbuffer lib
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
