# the compiler: gcc for C program, define as g++ for C++
CC = gcc
# compiler flags:
# -g adds debugging information to the executable file
# -Wall turns on most, but not all, compiler warnings
CFLAGS = -g -Wall -Wextra -Werror -pedantic -std=c99
# the build target executable:
# executable file name
default : 
		$(CC) $(CFLAGS) main.c parser.c converter.c -o minipng

clean :
		rm -f minipng *.pnm
