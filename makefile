all: a.out
FLAGS=-std=c++2a -Wall -Wextra -fsanitize=address -fsanitize=undefined

a.out: main.cpp matrix.h makefile
	g++-11 $(FLAGS) main.cpp

run: a.out
	time ./a.out 