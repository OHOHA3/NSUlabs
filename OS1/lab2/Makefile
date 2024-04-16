all: Hello-asm

Hello-asm: Hello-asm.o
	ld Hello-asm.o -o Hello-asm

Hello-asm.o: Hello-asm.S
	gcc -c Hello-asm.S

clean:
	rm Hello-asm.o Hello-asm
