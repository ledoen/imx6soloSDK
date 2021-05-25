CC 				= /opt/gcc-arm-none-eabi-5_4-2016q3/bin/arm-none-eabi-
TARGET 			= solo_app

INCLUDEDIRS 	= nxpheader \
				bsp/sysinit \
				bsp/gpio \
				bsp/epit \
				bsp/uart
				
			
SRCDIRS 		= project \
				bsp/sysinit \
				bsp/gpio \
				bsp/epit \
				bsp/uart

		
INCLUDE 		= $(patsubst %, -I %, $(INCLUDEDIRS))

SFILES 			= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.S))
CFILES			= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))

SFILENDIR       = $(notdir $(SFILES))
CFILENDIR       = $(notdir $(CFILES))

SOBJS              := $(patsubst %, obj/%, $(SFILENDIR:.S=.o))
COBJS              := $(patsubst %, obj/%, $(CFILENDIR:.c=.o))
OBJS               := $(SOBJS) $(COBJS)

VPATH              := $(SRCDIRS)

.PHONY: clean

$(TARGET).bin:$(OBJS)
	$(CC)ld -Tmysdk_app.lds -o $(TARGET).elf $^ -lgcc -L/opt/gcc-arm-none-eabi-5_4-2016q3/lib/gcc/arm-none-eabi/5.4.1
	$(CC)objcopy -O binary -S $(TARGET).elf $@
	
$(SOBJS): obj/%.o : %.S
	$(CC)gcc -Wall -nostdlib -c -O2 $(INCLUDE) -o $@ $<
    
$(COBJS): obj/%.o : %.c
	$(CC)gcc -Wall -nostdlib -c -O2 $(INCLUDE) -o $@ $<
	
clean:
	rm -rf $(TARGET).elf $(TARGET).bin $(SOBJS) $(COBJS)
	
test:
	echo $(COBJS)
	echo $(OBJS)
