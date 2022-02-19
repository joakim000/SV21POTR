# Run make check in Git Bash
# A Makefile to build and run the test

CC := gcc
CFLAGS := -Wall -Wextra -Wpedantic

LIB_DIR := lib
TEST_DIR := test
BUILD_DIR := build
TEST_EXE := cbuffer_test

INCLUDES := $(addprefix -I./,$(wildcard $(LIB_DIR)/*))

OBJECTS := $(notdir $(wildcard $(LIB_DIR)/*/*.c) $(wildcard $(TEST_DIR)/*.c))
OBJECTS := $(addprefix $(BUILD_DIR)/,$(OBJECTS:.c=.o))

all: .mkbuild $(BUILD_DIR)/$(TEST_EXE)
	@echo ""
	@echo "************ The Targets ************"
	@echo "**** clean: to clean"
	@echo "**** check: to run the test"
	@echo "*************************************"
	@echo ""

$(BUILD_DIR)/$(TEST_EXE): $(OBJECTS)
#	$(CC) $^ -lunity -o $@
	$(CC) $^ -lunity -o $@

$(BUILD_DIR)/%.o: $(LIB_DIR)/*/%.c
	$(CC) $(CFLAGS) -MMD -c $< -o $@

$(BUILD_DIR)/%.o : $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) -MMD -c $< $(INCLUDES) -o $@

check: .mkbuild $(BUILD_DIR)/$(TEST_EXE)
	@echo ""
	@echo "**************************************"
	@echo "********** Run The Test **************"
	@echo "**************************************"
	@echo ""
	@./$(BUILD_DIR)/$(TEST_EXE)

# Include the automatically generated dependencies
-include $(OBJECTS:.o=.d)

.PHONY: clean .mkbuild check all

clean:
	@rm -rf $(BUILD_DIR)

.mkbuild:
	@mkdir -p $(BUILD_DIR)