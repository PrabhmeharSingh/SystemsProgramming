prodcon: main.o tands.o 
	gcc -std=gnu99 -o prodcon main.o tands.o -pthread
main.o: main.c 
	gcc -std=gnu99 -o main.o main.c -c
tands.o: tands.c 
	gcc -std=gnu99 -o tands.o tands.c -c
clear:
	rm *.o prodcon *.log