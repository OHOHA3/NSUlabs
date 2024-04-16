all: hello

hello: Source.o libhfdl.so
	gcc Source.o -ldl -o hello

libhfdl.so: hfdl.o
	gcc -shared -o libhfdl.so hfdl.o

hfdl.o: hfdl.c
	gcc -c -FPIC hfdl.c

Source.o: Source.c
	gcc -c Source.c

clean:
	rm *.o hello libhfdl.so
