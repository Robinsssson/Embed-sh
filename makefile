# Name for output binary files
PROJECT ?= sh_demo
CC = arm-none-eabi-gcc
LD = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
CC_PATH = /opt/gcc-arm-none-eabi/arm-none-eabi
# Path to STM32F103 standard peripheral library
STD_PERIPH_LIBS ?= ./STM32F10x_StdPeriph_Lib_V3.6.0/

SOURCES_DIR = src
BUILD_DIR   = build
INCLUDE_DIR = include
STARTUP_DIR = startup
LIB_DIR = lib

# List of source files
SOURCES  = $(SOURCES_DIR)/main.c
SOURCES += $(SOURCES_DIR)/sh/sh_map/sh_map.c
SOURCES += $(SOURCES_DIR)/sh/sh.c
SOURCES += $(SOURCES_DIR)/sh/sh_utli/*.c
SOURCES += $(SOURCES_DIR)/stm32f10x_it.c
SOURCES += $(SOURCES_DIR)/system_stm32f10x.c
SOURCES += $(STD_PERIPH_LIBS)/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_rcc.c
SOURCES += $(STD_PERIPH_LIBS)/Libraries/STM32F10x_StdPeriph_Driver/src/*.c
SOURCES += $(STD_PERIPH_LIBS)/Libraries/CMSIS/CM3/CoreSupport/core_cm3.c

INCLUDES  = -I$(INCLUDE_DIR)/
INCLUDES += -I3rdparty
INCLUDES += -I$(STD_PERIPH_LIBS)/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/
INCLUDES += -I$(STD_PERIPH_LIBS)/Libraries/CMSIS/CM3/CoreSupport/
INCLUDES += -I$(STD_PERIPH_LIBS)/Libraries/STM32F10x_StdPeriph_Driver/inc/
INCLUDES += -I$(CC_PATH)/include

LIBS = -L$(LIB_DIR)/sds -lsds
# LIBS = -L$(LIB_DIR)/sds -lsds
CFLAGS  = -g -Wall --specs=nosys.specs -march=armv7-m -O0
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m3 # -mthumb-interwork
CFLAGS += -mfloat-abi=soft # -mfpu=fpv4-sp-d16 
CFLAGS += -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER
CFLAGS += -Wl,--gc-sections $(INCLUDES) $(LIBS)

LDFLAGS = -T$(STARTUP_DIR)/stm32_flash.ld

# Compiler, objcopy (should be in PATH)

# Path to st-flash (or should be specified in PATH)
ST_FLASH ?= st-flash

# Object files
OBJECTS = $(patsubst $(SOURCES_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))
OBJECTS += $(BUILD_DIR)/startup/startup_stm32f10x_md.o

all: $(BUILD_DIR)/$(PROJECT).elf

# Compile target
$(BUILD_DIR)/$(PROJECT).elf: $(OBJECTS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@
	$(OBJCOPY) -O ihex $@ $(BUILD_DIR)/$(PROJECT).hex
	$(OBJCOPY) -O binary $@ $(BUILD_DIR)/$(PROJECT).bin

# Pattern rule for object files
$(BUILD_DIR)/%.o: $(SOURCES_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

# Rule for compiling the startup file
$(BUILD_DIR)/startup/startup_stm32f10x_md.o: $(STARTUP_DIR)/startup_stm32f10x_md.s
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

# Remove binary files
.PHONY: clean flash compile_commands

clean:
	rm -rf $(BUILD_DIR)

# Flash target
flash:
	/opt/openocd/bin/openocd -f /opt/openocd/openocd/scripts/interface/stlink.cfg \
	-f /opt/openocd/openocd/scripts/target/stm32f1x.cfg \
	-c "program $(BUILD_DIR)/$(PROJECT).bin verify reset exit 0x08000000"

# Generate compile_commands.json for clangd
compile_commands:
	bear -- make all -j2
	mv -f compile_commands.json $(BUILD_DIR)/compile_commands.json
