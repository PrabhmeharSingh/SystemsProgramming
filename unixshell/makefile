shell379: ass1.o ass1Functionality.o processList.o
	gcc -std=gnu99 -o shell379 ass1.o ass1Functionality.o processList.o
ass1.o: ass1.c 
	gcc -std=gnu99 -o ass1.o ass1.c -c
ass1Functionality.o: ass1Functionality.c
	gcc -std=gnu99 -o ass1Functionality.o ass1Functionality.c -c
processList.o: processList.c
	gcc -std=gnu99 -o processList.o processList.c -c
clear:
	rm *.o shell379