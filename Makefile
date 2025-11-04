# --- Project Configuration ---
TARGET_EXEC ?= coverfloat_reference
BUILD_DIR   ?= ./build
# TODO: yuck fix paths
SRC_DIRS    ?= ./src ./../riscv-isa-sim/softfloat/

# Compiler and Flags
CC          ?= gcc
CFLAGS      ?= -Wall -Wextra -O2
CPPFLAGS    ?= -MMD -MP $(INC_FLAGS)
LDFLAGS     ?= 

# --- Automatic File Finding ---
# Find all C source files in source directories
SRCS        := $(shell find $(SRC_DIRS) -name *.c)

# Generate list of object file paths in the build directory
OBJS        := $(patsubst $(SRC_DIRS)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# Generate list of dependency file paths in the build directory
DEPS        := $(OBJS:.o=.d)

# Find all include directories and format them with -I flag
INC_DIRS    := $(shell find $(SRC_DIRS) -type d) ./include
INC_FLAGS   := $(addprefix -I,$(INC_DIRS))

# Command for creating directories
MKDIR_P     ?= mkdir -p
RM          ?= rm -rf

# --- Targets ---

.PHONY: all clean 

all: $(BUILD_DIR)/$(TARGET_EXEC)

# Rule to create the final executable
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	@echo "Linking: $@"
	$(MKDIR_P) $(dir $@)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Rule to compile C source files into object files
# $< is the prerequisite (source file), $@ is the target (object file)
$(BUILD_DIR)/%.o: $(SRC_DIRS)/%.c
	@echo "Compiling C file: $<"
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Clean target to remove build artifacts
clean:
	@echo "Cleaning build directory..."
	$(RM) $(BUILD_DIR)

# --- Include Dependency Files ---
# Include auto-generated dependency files if they exist
# -include $(DEPS)
