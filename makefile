build: ./source/main.c
	gcc ./source/main.c	-o ./build/minimake.exe

full_build: ./source/main.c
	gcc -std=c99 -pedantic -Werror -Wall -Wextra ./source/main.c -o ./build/minimake.exe