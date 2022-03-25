CC=gcc
SRC=main.c
OUT=todo
FLAGS=-Wall -Wextra -Werror -std=gnu17

debug:
	$(CC) $(SRC) $(FLAGS) -lncurses -o $(OUT)

release:
	$(CC) $(SRC) $(FLAGS) -Ofast -lncurses -o $(OUT)

windows:
	$(CC) $(SRC) libpdcurses.a $(FLAGS) -Ofast -o $(OUT)

