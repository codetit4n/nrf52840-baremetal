PROJECT = blinky

CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

# CPU options: nRF52840 = Cortex-M4, Thumb instruction set
CPUFLAGS = -mcpu=cortex-m4 -mthumb

# General C Compiler flags
CFLAGS = $(CPUFLAGS) -Wall -Wextra -O0 -g3 -ffreestanding

# Linker flags
LDFLAGS = $(CPUFLAGS) -nostartfiles -nostdlib -Wl,-Tlinker.ld

# Source files
SRCS = src/startup.s src/main.c

# Object files
OBJS = src/startup.o src/main.o

# Default target: build ELF + HEX
all: $(PROJECT).elf $(PROJECT).hex

# Link object files into a final ELF image
$(PROJECT).elf: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^


# Assemble .s files
src/%.o: src/%.s
	$(CC) $(CPUFLAGS) -c $< -o $@

# Compile .c files
src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Convert ELF -> Intel HEX (useful for flashing)
$(PROJECT).hex: $(PROJECT).elf
	$(OBJCOPY) -O ihex $< $@

# Clean build artifacts
clean:
	rm -f $(PROJECT).elf $(PROJECT).hex $(OBJS)
