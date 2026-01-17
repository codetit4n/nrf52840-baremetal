PROJECT = spi

CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

# Folders
SRC_DIR = src
BUILD_DIR = build

# CPU options: nRF52840 = Cortex-M4, Thumb instruction set
CPUFLAGS = -mcpu=cortex-m4 -mthumb

# General C Compiler flags
CFLAGS = $(CPUFLAGS) -Wall -Wextra -O0 -g3 -ffreestanding

# Linker flags
LDFLAGS = $(CPUFLAGS) -nostartfiles -nostdlib -Wl,-T$(SRC_DIR)/linker.ld

# Source files
SRCS = $(SRC_DIR)/startup.s $(SRC_DIR)/main.c

# Object files go inside build/
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
OBJS := $(OBJS:$(SRC_DIR)/%.s=$(BUILD_DIR)/%.o)

# Final outputs
ELF = $(BUILD_DIR)/$(PROJECT).elf
HEX = $(BUILD_DIR)/$(PROJECT).hex

# Default target
all: $(ELF) $(HEX)

# Create build directory if missing
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Link the ELF
$(ELF): $(OBJS) | $(BUILD_DIR)
	$(CC) $(LDFLAGS) -o $@ $^

# Compile C files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Assemble .s files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.s | $(BUILD_DIR)
	$(CC) $(CPUFLAGS) -c $< -o $@

# Create HEX file
$(HEX): $(ELF)
	$(OBJCOPY) -O ihex $< $@

# Clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
