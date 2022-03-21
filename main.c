#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ncurses.h>
#include <assert.h>
#include <errno.h>
#include <string.h>

#define DG_DYNARR_IMPLEMENTATION
#include "DG_dynarr.h"

#define DONE_CHAR 'X'

#define TODO_SIZE   3
#define TODO_COLS   2
#define COLWIDTH    30

#define TODO_MAXLENGTH 261 // 256 + 5 for (TODO|)

typedef struct {
    char* text;
    bool done;
} todo_item_t;

#define todo_item_empty ((todo_item_t){.done = false, .text = calloc(1,1) })

DA_TYPEDEF(todo_item_t, todo_list);

typedef struct {
    unsigned int row, col;
} vec_t;

#define vec_origin ((vec_t){0,0})

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

void print_todos(todo_list *list, vec_t startPos)
{
    size_t size = da_count(*list);
    for (size_t i = 0; i < size; i++)
    {
        printw_at(startPos, "[%c] %s", list->p[i].done ? DONE_CHAR : ' ', list->p[i].text);
        startPos.row++;
    }
}

todo_list *create_demo_list()
{
    todo_list *list = malloc(sizeof(todo_list));
    da_init(*list);
    todo_item_t demo_item = { .done = false, .text = malloc(5) };

    // yikes
    demo_item.text[0] = 'd';
    demo_item.text[1] = 'e';
    demo_item.text[2] = 'm';
    demo_item.text[3] = 'o';
    demo_item.text[4] = '\0'; 

    da_push(*list, demo_item);

    return list;
}

todo_list *read_list_from_file(const char *filename)
{
    errno = 0;
    FILE *f = fopen(filename, "r");

    if (f == NULL)
    {
        fprintf(stderr, "Could not open file: %s\n", strerror(errno));
        return NULL;
    }

    todo_list *list = malloc(sizeof(todo_list));
    da_init(*list);

    char buffer[TODO_MAXLENGTH];

    while (fgets(buffer, TODO_MAXLENGTH, f))
    {
        int delimiterIndex = 0;
        while(buffer[delimiterIndex++] != '|');

        char state[delimiterIndex];
        memcpy(state, buffer, delimiterIndex - 1);
        state[delimiterIndex - 1] = '\0';

        size_t textLen = strlen(&buffer[delimiterIndex]);
        char *message = malloc(textLen - 1); // exclude '\n'
        memcpy(message, &buffer[delimiterIndex], textLen - 1);

        todo_item_t item = { .text = message };
        
        if (0 == strncmp("TODO", state, TODO_MAXLENGTH))
        {
            item.done = false;
        }
        else // default to done
        {
            item.done = true;
        }

        da_push(*list, item);
    }

    fclose(f);

    return list;
}

void save_list_to_file(const char *filename, todo_list *todos)
{
    errno = 0;
    FILE *f = fopen(filename, "w");

    if (f == NULL)
    {
        fprintf(stderr, "Could not save to file: %s\n", strerror(errno));
        return;
    }

    size_t nTodos = da_count(*todos);

    for (size_t i = 0; i < nTodos; i++)
        fprintf(f, "%s|%s\n", todos->p[i].done ? "DONE" : "TODO", todos->p[i].text);

    fclose(f);
}

void free_todolist(todo_list *list)
{
    size_t nElements = da_count(*list);

    for (size_t i = 0; i < nElements; i++)
        free(list->p[i].text);

    da_free(*list);
    free(list);
}

void cursor_back()
{
    int y = getcury(stdscr),
        x = getcurx(stdscr);
    mvaddch(y, x - 1, ' ');
    move(y, x - 2);
}

int main(int argc, char **argv)
{
    char* path = argc == 1 ? "./todofile" : argv[1];
    todo_list *list = argc == 1 ? create_demo_list() : read_list_from_file(path);

    if (list == NULL)
        return EXIT_FAILURE;

    initscr();

    char user_char = '\0';
    vec_t cursor;
    size_t selected_todo = 0;
    bool insert_mode = false;

    do 
    {
        switch (user_char)
        {
            case 'k':
                if (selected_todo) selected_todo--;
                break;

            case 'j':
                if (selected_todo < da_count(*list) - 1) selected_todo++;
                break;

            case '\n': // enter
                list->p[selected_todo].done = !list->p[selected_todo].done;
                break;

            case 'i':
                insert_mode = true;
                break;

            case 'd':
                if (da_count(*list))
                {
                    da_delete(*list, selected_todo);
                    if (!selected_todo)
                        da_push(*list, todo_item_empty);

                    if (selected_todo >= 1)
                        selected_todo--;
                }
                break;

            case 'a':
                todo_item_t newItem = todo_item_empty;
                da_push(*list, newItem);
                selected_todo = da_count(*list) - 1;
                break;
                
        }    

        clear();

        printw_at(vec_origin, "\t-- TODO --");

        print_todos(list, (vec_t){ .row = 1, .col = 0 });

        cursor.row = selected_todo + 1;
        cursor.col = 1;

        if (insert_mode)
        {
            move_cursor((vec_t){ .row = selected_todo + 1, .col = 4 });
            echo();
            char buffer[TODO_MAXLENGTH];
            size_t i = 0;
            while (i < TODO_MAXLENGTH - 1)
            {
                char c = getch();

                // THIS DOES NOTHING!!!!!!!!!!!
                switch (c)
                {
                    case '\x1B':
                        cursor_back();
                        goto _end_read;
                    case '\x08':
                        move(getcury(stdscr), getcurx(stdscr) - 1);
                        cursor_back();
                        continue;
                }
                
                buffer[i++] = c;
            }
        _end_read:
            buffer[i] = '\0'; // replace escape-char with zero

            free(list->p[selected_todo].text);
            list->p[selected_todo].text = malloc(i);
            memcpy(list->p[selected_todo].text, buffer, i);
            noecho();
            insert_mode = false;
        }

        move_cursor(cursor);

        refresh();
    }
    while ((user_char = getch()) != 'q');

    endwin();

    save_list_to_file(path, list);

    free_todolist(list);

    return EXIT_SUCCESS;
}