ARMGNU ?= arm-none-eabi

#arm-none-eabi

AOPS = --warn --fatal-warnings 
COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding -mcpu=arm1176jzf-s -std=c99

gcc : gameoflife.hex gameoflife.bin kernel.img

all : gcc

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

gameoflife.o : gameoflife.c
	$(ARMGNU)-gcc $(COPS) -c gameoflife.c -o gameoflife.o -std=c99

gameoflife.elf : memmap vectors.o gameoflife.o 
	$(ARMGNU)-ld vectors.o gameoflife.o -T memmap -o gameoflife.elf

gameoflife.bin : gameoflife.elf
	$(ARMGNU)-objcopy gameoflife.elf -O binary gameoflife.bin
	
kernel.img : gameoflife.elf
	$(ARMGNU)-objcopy gameoflife.elf -O binary kernel.img
