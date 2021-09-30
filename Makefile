# Makefile for myfind.c

all: myfind

myfind: myfind.c ; gcc -Wall -o myfind myfind.c

clean: ; rm -f myfind