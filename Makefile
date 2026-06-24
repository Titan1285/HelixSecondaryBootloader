AS		:= aarch64-linux-gnu-as
GCC		:= aarch64-linux-gnu-gcc
LD		:= aarch64-linux-gnu-ld
OBJCOPY	:= aarch64-linux-gnu-objcopy


TARGET	:= sbl

ASFLAGS	?= -g

CFLAGS	?=	\
	-g \
	-O0 \
	-Wall	\
	-Wextra \
	-nostdlib \
	-mcpu=cortex-a57 \
	-fno-stack-protector \
	-ffreestanding \
	-fno-pic \
	-Iinclude \
	-mgeneral-regs-only \
	-mstrict-align \
	-Wno-unused-function \
	-Wno-unused-variable

LDFLAGS	?= \
	-T helix.ld \
	-z max-page-size=0x1000 \
	-nostdlib \
	-static


ARCH_LIBS	:= \
	arch/start.o \
	arch/vectors.o \
	arch/arch.o \
	arch/irq_handler.o \
	arch/exc_handler.o \
	arch/cache.o

HW_OBJS		:= \
	devices/timer.o \
	devices/sdhci.o \
	devices/gicv3.o \
	devices/uart.o \
	devices/fw_cfg.o

GFX_LIBS	:= \
	gfx/put_pixel.o

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
	lib/utoa64.o \
	lib/strcmp.o \
	lib/mbr.o \
	lib/gpt.o \
	lib/utf16_to_utf8.o

MM_LIBS		:= \
	mm/pmm.o \
	mm/slab.o \
	mm/alloc.o \
	mm/vmm.o

SRC_OBJS	:= \
	init/main.o \
	$(ARCH_LIBS) \
	$(HW_OBJS) \
	$(LIB_OBJS) \
	$(MM_LIBS) \
	$(GFX_LIBS)








all: $(TARGET).bin

$(TARGET).bin: $(TARGET).elf
	@echo "OBJCOPY	$(TARGET).bin"
	@$(OBJCOPY) -Obinary $(TARGET).elf $(TARGET).bin

$(TARGET).elf: $(SRC_OBJS)
	@echo "LD	$(TARGET).elf"
	@$(LD) $(LDFLAGS) $(SRC_OBJS) -o $(TARGET).elf

%.o: %.c
	@echo "CC	$<"
	@$(GCC) -c $(CFLAGS) $< -o $@

%.o: %.s
	@echo "AS	$<"
	@$(GCC) -c $(ASFLAGS) $< -o $@

clean:
	@rm -f $(SRC_OBJS) *.elf *.bin *.img