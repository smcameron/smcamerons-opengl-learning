
GLLIBS=-lglfw

main:	main.c Makefile
	gcc -Bstatic -g -W -Wall -o main main.c ${GLLIBS}

