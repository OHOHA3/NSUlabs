all: hello

hello: Source.o libhfdl.so
	gcc -Wl,-rpath,. Source.o -L. -lhfdl -o hello

libhfdl.so: hfdl.o
	gcc -shared -o libhfdl.so hfdl.o

hfdl.o: hfdl.c
	gcc -FPIC -c hfdl.c

Source.o: Source.c
	gcc -c Source.c

clean:
	rm *.o hello libhfdl.so
