SRC=main.c

all: cli lib

cli:
	cc -DCLI main.c -o procrule

lib:
	cc -DLIB -fPIC --shared  main.c -o libprocrule.so
clean:
	-rm procrule
	-rm libprocrule.so
