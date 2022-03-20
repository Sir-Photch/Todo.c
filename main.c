#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ncurses.h>

typedef struct todo_item {
    char* text;
    bool done;
} todo_item_t;

typedef struct vec {
    unsigned int row, col;
} vec_t;

#define DONE_CHAR 'X'

#define TODO_SIZE   3
#define TODO_COLS   2
#define COLWIDTH    30

todo_item_t todos[TODO_COLS][TODO_SIZE] = 
{
    {
        { "imitate tsoding", false },
        { "stop using tinder", false },
        { "goto bed", false }   
    },
    {
        { "eat lunch", true },
        { "go ribbit", true },
        { "foo bar", true }
    }
};

int move_cursor(vec_t pos)
{
    return move(pos.row, pos.col);
}

void printw_at(vec_t pos, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    move_cursor(pos);    
    vw_printw(stdscr, fmt, args);

    va_end(args);
}

vec_t print_todos(todo_item_t *todo, size_t size, vec_t startPos)
{
    vec_t initialPos = startPos;
    for (size_t i = 0; i < size; i++)
    {
        printw_at(startPos, "[%c] %s", todo[i].done ? DONE_CHAR : ' ', todo[i].text);
        startPos.row++;
    }
    return (vec_t){.row = initialPos.row, .col = initialPos.col + 1};
}

int main(void)
{
    initscr();

    char userChar = '\0';
    vec_t cursor;
    vec_t selected_todo = { .row = 0, .col = 0 };

    do 
    {
        switch (userChar)
        {
            case 'w':
                if (selected_todo.row) selected_todo.row--;
                break;

            case 's':
                if (selected_todo.row < TODO_SIZE - 1) selected_todo.row++;
                break;

            case '\n': // enter
                todos[selected_todo.col][selected_todo.row].done = !todos[selected_todo.col][selected_todo.row].done;
                break;

            case 'a':
                if (selected_todo.col) selected_todo.col--;
                break;

            case 'd':
                if (selected_todo.col < TODO_COLS - 1) selected_todo.col++;
                break;
        }    

        clear();

        vec_t colPrintPos = {0};
        printw_at(colPrintPos, "TODO:");
        colPrintPos.row += 1;
        colPrintPos.col += 2;
        vec_t offset = print_todos(todos[0], TODO_SIZE, colPrintPos);
        colPrintPos.col += COLWIDTH;
        colPrintPos.row = 0;
        colPrintPos.col -= 2;

        printw_at(colPrintPos, "DONE:");
        colPrintPos.row += 1;
        colPrintPos.col += 2;
        print_todos(todos[1], TODO_SIZE, colPrintPos);    
        
        cursor.row = offset.row + selected_todo.row;
        cursor.col = offset.col + selected_todo.col * COLWIDTH;

        move_cursor(cursor);

        refresh();
    }
    while ((userChar = getch()) != 'q');

    endwin();

    return EXIT_SUCCESS;
}