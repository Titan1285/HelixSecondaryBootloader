AS		:= aarch64-linux-gnu-as
GCC		:= aarch64-linux-gnu-gcc
LD		:= aarch64-linux-gnu-ld
OBJCOPY	:= aarch64-linux-gnu-objcopy


TARGET	:= sbl

ASFLAGS	?= -g

CFLAGS	?=	\
	-g \
	-Wall	\
	-Wextra \
	-nostdlib \
	-mcpu=cortex-a57 \
	-fno-stack-protector \
	-ffreestanding \
	-fno-pic \
	-Iinclude \
	-mstrict-align


ARCH_LIBS	:= \
	arch/start.o \
	arch/vectors.o \
	arch/arch.o \
	arch/irq_handler.o \
	arch/exc_handler.o

HW_OBJS		:= \
	devices/timer.o \
	devices/sdhci.o \
	devices/gicv3.o \
	devices/uart.o

LIB_OBJS	:= \
	lib/puts.o \
	lib/putc.o \
	lib/printf.o \
	lib/itoa.o \
	lib/memcmp.o \
	lib/memcpy.o \
	lib/memmove.o \
	lib/memset.o \
	lib/strcpy.o \
	lib/utoa.o \
	lib/tolower.o \
	lib/toupper.o \
	lib/strrev.o \
	lib/vprintf.o \
	lib/strlen.o \
	lib/utoa64.o

MM_LIBS		:= \
	mm/pmm.o \
	mm/vmm.o \
	mm/slab.o \
	mm/alloc.o

SRC_OBJS	:= \
	init/main.o \
	$(ARCH_LIBS) \
	$(HW_OBJS) \
	$(LIB_OBJS) \
	$(MM_LIBS)








all: $(TARGET).bin

$(TARGET).bin: $(TARGET).elf
	@echo "OBJCOPY	$(TARGET).bin"
	@$(OBJCOPY) -Obinary $(TARGET).elf $(TARGET).bin

$(TARGET).elf: $(SRC_OBJS)
	@echo "LD	$(TARGET).elf"
	@$(LD) -T helix.ld $(SRC_OBJS) -o $(TARGET).elf -nostdlib

%.o: %.c
	@echo "CC	$<"
	@$(GCC) -c $(CFLAGS) $< -o $@

%.o: %.s
	@echo "AS	$<"
	@$(GCC) -c $(ASFLAGS) $< -o $@

clean:
	@rm -f $(SRC_OBJS) *.elf *.bin *.img