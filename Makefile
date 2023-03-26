build:
	nasm switch.asm -f elf32 -o switch.o
	gcc -g -m32 -c co.c
	gcc -g -m32 -o co.elf switch.o co.o

clean:
	rm -f *.o *.elf