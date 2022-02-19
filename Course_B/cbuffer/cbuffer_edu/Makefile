# $(info $(SHELL))
# $(info $(PATH))

.DELETE_ON_ERROR :

SRC = src/
TEST = test/
BUILD = build/
LIB = lib/
BIN = bin/
UNITY = lib/unity/

RANGE_LOW = 8
RANGE_HIGH = 10
# BUFSIZERANGE = $(shell seq ${RANGE_LOW} ${RANGE_HIGH})
BUFSIZERANGE = 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32

LFLAGS =
CFLAGS := -std=c11 
ifdef PROF
CFLAGS := $(CFLAGS) -O3 -pg -g
LFLAGS = -pg
else
ifdef DEBUG
CFLAGS := $(CFLAGS) -DDEBUG
else
ifdef RANGE
CFLAGS := $(CFLAGS) -DCBUFFER_SIZE=$(BUFSIZE)
else
CFLAGS := $(CFLAGS) -O3 
endif
endif
endif

ifdef DEMO
CFLAGS := $(CFLAGS) -DVERBOSE=true
endif

ifdef SIZE
CFLAGS := $(CFLAGS) -DCBUFFER_SIZE=$(SIZE)
endif
ifdef CW
CFLAGS := $(CFLAGS) -DCLOCKWISE=$(CW)
endif
ifdef INIT
CFLAGS := $(CFLAGS) -DINDEXINIT=$(INIT)
endif

TEST_EXE = test_cbuffer
objects = $(BUILD)test_cbuffer.o $(BUILD)cbuffer.o $(BUILD)unity.o
test_cbuffer : $(objects)
	@cc $(LFLAGS) -o bin/test_cbuffer $(objects)
	@rm $(BUILD)*.o
	@./$(BIN)$(TEST_EXE)

TEST_EXE_FAROCH = test_cbuffer_faroch
objects_faroch = $(BUILD)test_cbuffer_faroch.o $(BUILD)cbuffer.o $(BUILD)unity.o
faroch : $(objects_faroch)
	@cc $(LFLAGS) -o bin/test_cbuffer_faroch $(objects_faroch)
	@rm $(BUILD)*.o
	@./$(BIN)$(TEST_EXE_FAROCH)


# Cbuffer test 
$(BUILD)test_cbuffer.o : $(TEST)test_cbuffer.c $(LIB)cbuffer.h 
	@cc -c $(CFLAGS) $(TEST)test_cbuffer.c -o $(@D)/test_cbuffer.o -I$(LIB) -I$(UNITY)	

# Cbuffer test faroch
$(BUILD)test_cbuffer_faroch.o : $(TEST)test_cbuffer_faroch.c $(LIB)cbuffer.h  
	@cc -c $(CFLAGS) $(TEST)test_cbuffer_faroch.c -o $(@D)/test_cbuffer_faroch.o -I$(LIB) -I$(UNITY)	

# Cbuffer lib
$(BUILD)cbuffer.o : $(LIB)cbuffer.c $(LIB)cbuffer.h
	@cc -c $(CFLAGS) $(LIB)cbuffer.c -o $(@D)/cbuffer.o -I$(LIB)

# Unity
$(BUILD)unity.o : $(UNITY)unity.c $(UNITY)unity.h
	@cc -c $(CFLAGS) $(UNITY)unity.c -o $(@D)/unity.o -I$(UNITY)


.PHONY : range
range :
# $(foreach var,$(BUFSIZERANGE),rm -f $(BUILD)*.o;$(MAKE) --no-print-directory faroch RANGE=1 BUFSIZE=$(var) &&) true
	@$(foreach var, $(BUFSIZERANGE), $(MAKE) faroch RANGE=1 BUFSIZE=$(var) --no-print-directory; ) 
# @rm $(BUILD)*.o

.PHONY : debug
debug :
	$(MAKE) DEBUG=1

.PHONY : demo
demo :
	$(MAKE) DEMO=1

.PHONY : prof
prof :
	$(MAKE) PROF=1

.PHONY : clean
clean : 
	rm -f $(BUILD)*.o
	
.PHONY : winclean
winclean :
	powershell rm -Force $(BUILD)*.o

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
