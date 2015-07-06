# put your *.o targets here, make should handle the rest!
SRCS = main.c system_stm32f0xx.c

# all the files will be generated with this name (main.elf, main.bin, main.hex, etc)
PROJECT=firmware
EXECUTABLE= $(PROJECT).elf
BIN_IMAGE=  $(PROJECT).bin
HEX_IMAGE = $(PROJECT).hex

# Location of the Libraries folder from the STM32F0xx Standard Peripheral Library
STD_PERIPH_LIB= ../Libraries

# Location of the linker scripts
LDSCRIPT_INC= 

# location of OpenOCD Board .cfg files (only used with 'make program')
OPENOCD_BOARD_DIR=/home/lab071/openocd/tcl/target

# Configuration (cfg) file containing programming directives for OpenOCD
OPENOCD_PROC_FILE=/home/lab071/openocd/tcl/target/stm32f0x.cfg

#CMSIS=../Libraries/CMSIS
ST= ../Libraries/STM32F0xx_StdPeriph_Driver

# that's it, no need to change anything below this line!

###################################################

CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
OBJDUMP=arm-none-eabi-objdump
SIZE=arm-none-eabi-size
LD=arm-none-eabi-gcc


ARCH_FLAGS=-mlittle-endian -mcpu=cortex-m0  -march=armv6-m -mthumb -ffunction-sections -fdata-sections
LDFLAGS += -Wl,--gc-sections -Wl,-Map=$(PROJECT).map
LDFLAGS +=-Tstm32f0.ld 

CFLAGS_INCLUDE=-I. \
	-I ./inc \
	-I $(STD_PERIPH_LIB) \
	-I $(STD_PERIPH_LIB)/CMSIS/Device/ST/STM32F0xx/Include \
	-I $(STD_PERIPH_LIB)/CMSIS/Include \
	-I $(STD_PERIPH_LIB)/STM32F0xx_StdPeriph_Driver/inc \
	-include $(STD_PERIPH_LIB)/stm32f0xx_conf.h 

CFLAGS_WARNING= \
	-Wall \
	-Wextra \
	-Wdouble-promotion \
	-Wshadow \
	-Werror=array-bounds \
	-Wfatal-errors \
	-Wmissing-prototypes \
	-Wbad-function-cast  \
	-Wstrict-prototypes \
	-Wmissing-parameter-type

CFLAGS  = -Wall -g -std=c99 -Os $(ARCH_FLAGS)\
	${CFLAGS_INCLUDE} \
	${CFLAGS_WARNING}
#CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m0 -march=armv6s-m

##CFLAGS += -mlittle-endian -mcpu=cortex-m0  -march=armv6-m -mthumb
##CFLAGS += -ffunction-sections -fdata-sections
##CFLAGS += -Wl,--gc-sections -Wl,-Map=$(PROJECT).map

###################################################

vpath %.c src
vpath %.a $(STD_PERIPH_LIB)

ROOT=$(shell pwd)


#CFLAGS += -I inc -I $(STD_PERIPH_LIB) -I $(STD_PERIPH_LIB)/CMSIS/Device/ST/STM32F0xx/Include
#CFLAGS += -I $(STD_PERIPH_LIB)/CMSIS/Include -I $(STD_PERIPH_LIB)/STM32F0xx_StdPeriph_Driver/inc
#CFLAGS += -include $(STD_PERIPH_LIB)/stm32f0xx_conf.h

SRCS += Device/startup_stm32f072.s # add startup file to build

# need if you want to build with -DUSE_CMSIS 
#SRCS += stm32f0_discovery.c
#SRCS += stm32f0_discovery.c stm32f0xx_it.c

OBJS = Device/startup_stm32f072.o \
       $(ST)/src/stm32f0xx_rcc.o \
       $(ST)/src/stm32f0xx_gpio.o \
       $(ST)/src/stm32f0xx_usart.o \
       $(ST)/src/stm32f0xx_misc.o \
       $(ST)/src/stm32f0xx_dma.o \
	   src/main.o \
	   src/usart.o \
	   src/system_stm32f0xx.o

###################################################

#.PHONY: lib proj

all:$(BIN_IMAGE)

$(BIN_IMAGE):$(EXECUTABLE)
	@$(OBJCOPY) -O binary $^ $@
	@echo 'OBJCOPY $(BIN_IMAGE)'


$(EXECUTABLE): $(OBJS)
	@$(LD) $(LDFLAGS) $(ARCH_FLAGS) $(OBJS) -o $@ 
	@echo 'LD $(EXECUTABLE)'

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo 'CC $<'

%.o: %.s
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo 'CC $<'

#lib:
#	@$(MAKE) -C $(STD_PERIPH_LIB) 
#
#proj: 	$(PROJECT).elf
#
#$(PROJECT).elf: 
#	@$(CC) $(CFLAGS) $^ -o $@ -L$(STD_PERIPH_LIB) -lstm32f0 -L$(LDSCRIPT_INC) -Tstm32f0.ld
	#$(OBJCOPY) -O ihex $(PROJECT).elf $(PROJECT).hex
	#$(OBJCOPY) -O binary $(PROJECT).elf $(PROJECT).bin
	#$(OBJDUMP) -St $(PROJECT).elf >$(PROJECT).lst
	#$(SIZE) $(PROJECT).elf
	
checksize:$(PROJECT).elf
	$(SIZE) $(PROJECT).elf

flash_openocd: $(PROJECT).bin
	openocd -f interface/stlink-v2.cfg \
	-c "transport select hla_swd"  \
	-f $(OPENOCD_PROC_FILE) \
	-c "init" \
	-c "reset init" \
	-c "halt" \
	-c "flash write_image erase $(PROJECT).elf" \
	-c "verify_image $(PROJECT).elf" \
	-c "reset run" -c shutdown

clean:
	find ./ -name '*~' | xargs rm -f	
	rm -f *.o
	rm -f $(PROJECT).elf
	rm -f $(PROJECT).hex
	rm -f $(PROJECT).bin
	rm -f $(PROJECT).map
	rm -f $(PROJECT).lst
	rm -f $(OBJS)

reallyclean: clean
	$(MAKE) -C $(STD_PERIPH_LIB) clean
