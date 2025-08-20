# Project Name
PROJECT = SERVER

# Global defines
GLOBAL_DEFINES = -DMG_ARCH=MG_ARCH_CUSTOM

# Toolchain definitions - using MSYS style paths
TOOLCHAIN_PATH = /c/PROGRA~2/GNUARM~1/102021~1.10
CC = "$(TOOLCHAIN_PATH)/bin/arm-none-eabi-gcc.exe"
AS = "$(TOOLCHAIN_PATH)/bin/arm-none-eabi-as.exe"
CP = "$(TOOLCHAIN_PATH)/bin/arm-none-eabi-objcopy.exe"
SZ = "$(TOOLCHAIN_PATH)/bin/arm-none-eabi-size.exe"

# Include paths
ARM_INC = "$(TOOLCHAIN_PATH)/arm-none-eabi/include"
GCC_INC = "$(TOOLCHAIN_PATH)/lib/gcc/arm-none-eabi/10.3.1/include"

# Directories
SRC_DIR = src
INC_DIR = inc
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj

# Output files
ELF = $(BUILD_DIR)/$(PROJECT).elf
HEX = $(BUILD_DIR)/$(PROJECT).hex
BIN = $(BUILD_DIR)/$(PROJECT).bin

# Source files
C_SOURCES = $(wildcard $(SRC_DIR)/*.c)
ASM_SOURCES = $(wildcard $(SRC_DIR)/*.s)

# Object files
OBJS = $(C_SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
OBJS += $(ASM_SOURCES:$(SRC_DIR)/%.s=$(OBJ_DIR)/%.o)

# MCU flags
CPU = -mcpu=arm7tdmi-s
MCU = $(CPU) 

# AS flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall 
# C flags
CFLAGS = $(MCU)
CFLAGS += $(GLOBAL_DEFINES)
CFLAGS += -Wall -Wextra -fno-common -ffunction-sections -fdata-sections
CFLAGS += -I$(INC_DIR)
CFLAGS += -ffreestanding
CFLAGS += -D__CLOCK_T_DEFINED
CFLAGS += -D_SYS_STAT_H_
CFLAGS += -I$(ARM_INC)
CFLAGS += -I$(ARM_INC)/machine
CFLAGS += -I$(ARM_INC)/sys
CFLAGS += -I$(GCC_INC)
CFLAGS += -I$(GCC_INC)/include-fixed
CFLAGS += -I$(INC_DIR) -I$(CUSTOM_INC)

# Define MG_ARCH for Mongoose
CFLAGS += -DMG_ENABLE_CUSTOM_NETWORKING=1
CFLAGS += -DMG_ENABLE_CUSTOM_MILLIS=1
CFLAGS += -DMG_ENABLE_CUSTOM_RANDOM=1
CFLAGS += -DMG_ENABLE_LWIP=0
CFLAGS += -include time.h 

# Linker flags
LDFLAGS = $(MCU) -specs=nosys.specs -nostartfiles -T lpc2148_flash.ld \
          -Wl,-Map=$(BUILD_DIR)/$(PROJECT).map,--cref -Wl,--gc-sections

# Default target
all: $(ELF) $(HEX) $(BIN)

# Create build directories
$(BUILD_DIR):
	mkdir -p "$@"

$(OBJ_DIR): | $(BUILD_DIR)
	mkdir -p "$@"

# Print debug information
print-debug:
	@echo "Current directory: $$(pwd)"
	@echo "Toolchain path: $(TOOLCHAIN_PATH)"
	@echo "Compiler: $(CC)"
	@echo "Assembler: $(AS)"
	@echo "Testing compiler existence:"
	@if [ -f "$(CC)" ]; then echo "Compiler found!"; else echo "Compiler not found!"; fi
	@echo "Testing assembler existence:"
	@if [ -f "$(AS)" ]; then echo "Assembler found!"; else echo "Assembler not found!"; fi

#Print C Flags
print-cflags:
    @echo "CFLAGS: $(CFLAGS)"

# Compile C files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "Compiling: $<"
	$(CC) -c $(GLOBAL_DEFINES) $(CFLAGS) "$<" -o "$@"

# Compile assembly files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.s | $(OBJ_DIR)
	@echo "Assembling: $<"
	$(AS) $(ASFLAGS) "$<" -o "$@"

# Create ELF file
$(ELF): $(OBJS) | $(BUILD_DIR)
	@echo "Linking: $@"
	$(CC) $(OBJS) $(LDFLAGS) -o "$@"
	$(SZ) "$@"

# Create HEX file
$(HEX): $(ELF)
	@echo "Creating hex: $@"
	$(CP) -O ihex "$<" "$@"

# Create binary file
$(BIN): $(ELF)
	@echo "Creating binary: $@"
	$(CP) -O binary -S "$<" "$@"

# Clean
clean:
	rm -rf "$(BUILD_DIR)"

# Ensure all necessary targets are phony
.PHONY: all clean print-debug print-cflags