#Makefile by Carter Reynolds and Alazar Genene 1/18/17

all: mc0 mc1 mc2

mc0 : mc0.c 
	gcc mc0.c -o mc0
mc1 : mc1.c 
	gcc mc1.c -o mc1
mc2 : mc2.c
	gcc mc2.c -o mc2
clean :
	rm mc0 mc1 mc2
