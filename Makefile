# Build Type
BUILD :=

# Project Name
PROJECT_NAME := fsctl

# Compiler
CC := gcc

# Compiler Flags
CFLAGS += -Wall -Wextra -Werror
ifeq ($(BUILD), debug)
	CFLAGS += -g -O0
	CFLAGS += -fsanitize=address,undefined,leak
endif
# Release flags: -O2 or -O3 (optimization) -DNDEBUG (disable assertions)

# Source Directory
SRC_DIR := src

# Include Directory
INC_DIR := inc

# Build Directory
BUILD_DIR := build

# Find all .c files recursively
SRCS := $(shell find $(SRC_DIR) -name "*.c" -print)

# Object Files (replace .c with .o in source files, preserving directory structure)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# Create directory structure for object files
DIRS := $(dir $(OBJS))

# Executable Name
EXECUTABLE := $(BUILD_DIR)/$(PROJECT_NAME)

# Find all include directories recursively
INCLUDE_DIRS := $(shell find $(INC_DIR) -type d)

# Include path
INC := $(addprefix -I,$(INCLUDE_DIRS))

# Define a variable for running test
TEST_COMMAND := $(BUILD_DIR)/test

# Linker Flags
LDFLAGS :=  # Example: -lm (math library)

# All Target
all: $(EXECUTABLE)
	size $(EXECUTABLE)

# Executable Target
$(EXECUTABLE): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INC) -o $@ $^ $(LDFLAGS)
	@echo "  [LD] $@ created"

# Create directories for object files
$(BUILD_DIR)/%:
	@mkdir -p $@

# Object File Compilation Rule (preserving directory structure)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@
	@echo "  [CC] $< compiled to $@"

# Clean Target (remove object files and executable)
clean:
	@echo "Cleaning project..."
	@rm -rf $(BUILD_DIR)
	@echo "Clean complete."

# Phony Targets (targets that don't represent files)
.PHONY: all clean test release

# Test Target
test: $(EXECUTABLE)
	@echo "Running tests..."
	$(TEST_COMMAND)

# Release Target
release:
	@echo "Build in release mode"
	$(MAKE) CFLAGS="-Wall -Wextra -Werror -O2 -DNDEBUG" all