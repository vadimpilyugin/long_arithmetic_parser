task1: task2.c
	gcc -Wall -O0 -I ./ -g -o task2 task2.c

.PHONY: test
test: task2
	cat task2.c | ./task2
