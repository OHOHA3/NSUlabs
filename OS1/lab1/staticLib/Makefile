all: hello

hello: Source.o libhfsl.a
	gcc Source.o -L. -lhfsl -o hello

Source.o: Source.c
	gcc -c Source.c

libhfsl.a: hfsl.o
	ar -rc libhfsl.a hfsl.o

hfsl.o: hfsl.c 
	gcc -c hfsl.c

clean:
	rm *.o hello libhfsl.a
