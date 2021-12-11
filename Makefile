TARGET_EXEC ?= a.out

# Where to build our resources
BUILD_DIR ?= ./build

# Where our code lives
SRC_DIRS ?= ./src

# Makefile magic to build object files and deps for our sources
SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

# Compiler and other tool flags
CC := gcc
CFLAGS := -ggdb -g -Iinclude
FLEX := flex
# FLEX_FLAGS :=
BISON := bison
BISON_FLAGS := -d
INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# Build our executable
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS) parser
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Build the source objects
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Generate our parser code
%.yy.c: %.l
	flex -o $@ $< 
%.tab.c: %.y
	bison -d -o $@ $< 
parser: src/lex.yy.c src/parser.tab.c

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p
