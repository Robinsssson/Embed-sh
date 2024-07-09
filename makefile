# Name for output binary files
PROJECT ?= sh_demo
CC = arm-none-eabi-gcc
LD = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
CC_PATH = /opt/gcc-arm-none-eabi/arm-none-eabi
STD_PERIPH_LIBS ?= ./STM32F10x_StdPeriph_Lib_V3.6.0/

SOURCES_DIR = src
BUILD_DIR   = build
INCLUDE_DIR = include
STARTUP_DIR = startup
LIB_DIR = lib
3RD_LIB_DIR = 3rdparty

# List of source files
SOURCES  = $(SOURCES_DIR)/main.c
SOURCES += $(SOURCES_DIR)/sh/sh_map/sh_map.c
SOURCES += $(SOURCES_DIR)/sh/sh_func/echo.c
SOURCES += $(SOURCES_DIR)/sh/sh_func/export.c
SOURCES += $(SOURCES_DIR)/sh/sh.c
SOURCES += $(SOURCES_DIR)/string/sh_string.c
SOURCES += $(SOURCES_DIR)/sh/sh_utli/usart1.c
SOURCES += $(SOURCES_DIR)/stm32f10x_it.c
SOURCES += $(SOURCES_DIR)/system_stm32f10x.c
SOURCES += $(STD_PERIPH_LIBS)/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_rcc.c
SOURCES += $(STD_PERIPH_LIBS)/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_gpio.c
SOURCES += $(STD_PERIPH_LIBS)/Libraries/STM32F10x_StdPeriph_Driver/src/misc.c
SOURCES += $(STD_PERIPH_LIBS)/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_usart.c
SOURCES += $(STD_PERIPH_LIBS)/Libraries/CMSIS/CM3/CoreSupport/core_cm3.c

INCLUDES  = -I$(INCLUDE_DIR)/
INCLUDES += -I$(3RD_LIB_DIR)/
INCLUDES += -I$(STD_PERIPH_LIBS)/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/
INCLUDES += -I$(STD_PERIPH_LIBS)/Libraries/CMSIS/CM3/CoreSupport/
INCLUDES += -I$(STD_PERIPH_LIBS)/Libraries/STM32F10x_StdPeriph_Driver/inc/
INCLUDES += -I/opt/local/picolibc/arm-none-eabi/include/

LIBS =  -Llib/sds -lsds
# LIBS += -L/opt/local/picolibc/arm-none-eabi/lib/thumb/v7-m/nofp -lc

CFLAGS  = -g -Wall --specs=nosys.specs -march=armv7-m -O0
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m3 # -mthumb-interwork
CFLAGS += -mfloat-abi=soft # -mfpu=fpv4-sp-d16 
CFLAGS += -DSTM32F10X_HD -DUSE_STDPERIPH_DRIVER
CFLAGS += -Wl,--gc-sections $(INCLUDES) 
CFLAGS += $(LIBS)

LDFLAGS = -T$(STARTUP_DIR)/stm32_flash.ld

# Compiler, objcopy (should be in PATH)
START_FILE = startup_stm32f10x_hd
# Path to st-flash (or should be specified in PATH)
ST_FLASH ?= st-flash

# Object files
OBJECTS = $(patsubst $(SOURCES_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))
OBJECTS += $(BUILD_DIR)/startup/$(START_FILE).o

all: $(BUILD_DIR)/$(PROJECT).elf

# Compile target
$(BUILD_DIR)/$(PROJECT).elf: $(OBJECTS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) $^ -o $@
	$(OBJCOPY) -O ihex $@ $(BUILD_DIR)/$(PROJECT).hex
	$(OBJCOPY) -O binary $@ $(BUILD_DIR)/$(PROJECT).bin

# Pattern rule for object files
$(BUILD_DIR)/%.o: $(SOURCES_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

# Rule for compiling the startup file
$(BUILD_DIR)/startup/$(START_FILE).o: $(STARTUP_DIR)/$(START_FILE).s
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

# Remove binary files
.PHONY: clean flash compile_commands

init_project:
	@mkdir $(SOURCES_DIR); mkdir $(STARTUP_DIR); mkdir $(INCLUDE_DIR); mkdir $(LIB_DIR); mkdir $(3RD_LIB_DIR)
	@cp $(STD_PERIPH_LIBS)/Project/STM32F10x_StdPeriph_Template/*.c $(SOURCES_DIR)
	@cp $(STD_PERIPH_LIBS)/Project/STM32F10x_StdPeriph_Template/*.h $(INCLUDE_DIR)
	@cp $(STD_PERIPH_LIBS)/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/gcc_ride7/*.s $(STARTUP_DIR)
	echo "you should cp the stm32_flash by yourself from $(STD_PERIPH_LIBS)/Project/STM32F10x_StdPeriph_Template/"

clean:
	rm -rf $(BUILD_DIR)

# Flash target
flash:
	/opt/openocd/bin/openocd -f /opt/openocd/openocd/scripts/interface/stlink.cfg \
	-f /opt/openocd/openocd/scripts/target/stm32f1x.cfg \
	-c "program $(BUILD_DIR)/$(PROJECT).bin verify reset exit 0x08000000"

port:
	/opt/openocd/bin/openocd -f /opt/openocd/openocd/scripts/interface/stlink.cfg \
	-f /opt/openocd/openocd/scripts/target/stm32f1x.cfg 
# Generate compile_commands.json for clangd
compile_commands:
	bear -- make all -j2
	mv -f compile_commands.json $(BUILD_DIR)/compile_commands.json
