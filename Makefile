.PHONY: clean

all: main.o
	gcc main.o -o tinnyTim
	./tinnyTim

main.o: main.c tinny_tim.h
	gcc -c main.c

clean:
	-@rm -f tinnyTim
	-@rm -f *.o

