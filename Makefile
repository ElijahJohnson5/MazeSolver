all: maze debug profile

maze: maze.c
	gcc -Wall -pedantic -ansi maze.c -o maze

debug: maze.c
	gcc -Wall -g -pedantic -ansi maze.c -o debug

profile: maze.c
	gcc -Wall -pg -pedantic -ansi maze.c -o profile

clean:
	rm maze
	rm debug


