CC=gcc
SRC=main.c
OUT=todo
FLAGS=-Wall -Wextra -Werror -std=c17

debug:
	$(CC) $(SRC) $(FLAGS) -lncurses -o $(OUT) && ./$(OUT)

release:
	$(CC) $(SRC) $(FLAGS) -Ofast -lncurses -o $(OUT)

