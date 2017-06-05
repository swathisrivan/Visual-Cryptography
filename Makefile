visual: visual.o visual_util.o
	gcc -o visual -g visual.o visual_util.o -lcrypto -lm

visual.o: visual.c 
	gcc -g -c -Wall visual.c 

visual_util.o: visual_util.c
	gcc -g -c -Wall visual_util.c

clean:
	rm -f *.o visual
