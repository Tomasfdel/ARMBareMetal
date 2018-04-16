all: gameoflife	

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img


gameoflife: gameoflife.c
	arm-none-eabi-gcc gameoflife.c -o gameoflife
