#include "io.hpp"
#include <cstdarg>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <ncurses.h>

namespace io {
    // This is a C-style variadic function
    // For specific window
    void centerText(WINDOW *win, bool shouldMove, const char *format_string, int count, ...) {
        int y, max_x, center_x, overall_size;
        va_list args, args_copy;
        const char *p = format_string; // This variable will use for iterating each character of the string

        overall_size = 0;
        va_start(args, count);
        va_copy(args_copy, args);
        for (int i=0; i < count; i++) {
            overall_size += strlen(va_arg(args_copy, char*));
        }
        va_end(args_copy);

        getmaxyx(win, y, max_x);
        center_x = (max_x - overall_size) / 2; // calculate the cursor position that makes the text center
        getyx(win, y, max_x); // Get the current position of the cursor
        wmove(win, y, center_x); // Move the cursor

        while (*p) { // While p is not pointing to NULL terminator
            if (*p == '%' && *(p+1) == 's') {
                wprintw(win, "%s", va_arg(args, char*));
                wrefresh(win);
                p += 2;
            } else if (*p == '%' && *(p+1) == 'd') {
                wprintw(win, "%d", va_arg(args, int));
                wrefresh(win);
                p += 2;
            } else {
                addch(*p);
                p++;
            }
        }

        if (shouldMove) {
            wmove(win, y+1, 1);
        }

        va_end(args);
    }

    // This is a C-style variadic function
    void centerText(const char *format_string, int count, ...) {
        int y, max_x, center_x, overall_size;
        va_list args, args_copy;
        const char *p = format_string; // This variable will use for iterating each character of the string

        overall_size = 0;
        va_start(args, count);
        va_copy(args_copy, args);
        for (int i=0; i < count; i++) {
            overall_size += strlen(va_arg(args_copy, char*));
        }
        va_end(args_copy);

        getmaxyx(stdscr, y, max_x);
        center_x = (max_x - overall_size) / 2; // calculate the cursor position that makes the text center
        getyx(stdscr, y, max_x); // Get the current position of the cursor
        move(y, center_x); // Move the cursor

        while (*p) { // While p is not pointing to NULL terminator
            if (*p == '%' && *(p+1) == 's') {
                printw("%s", va_arg(args, char*));
                refresh();
                p += 2;
            } else if (*p == '%' && *(p+1) == 'd') {
                printw("%d", va_arg(args, int));
                refresh();
                p += 2;
            } else {
                addch(*p);
                p++;
            }
        }

        va_end(args);
    }

    void centerText(const char *str) {
        int y, max_x, center_x;

        getmaxyx(stdscr, y, max_x);
        center_x = (max_x - strlen(str)) / 2; // calculate the cursor position that makes the text center
        getyx(stdscr, y, max_x); // Get the current position of the cursor
        move(y, center_x); // Move the cursor

        printw("%s", str);
        refresh();
    }

    void printAndMoveDown(WINDOW *win, const char *format_string, int count, ...) {
        int y, x;
        getyx(win, y, x);

        va_list args;
        va_start(args, count);
        const char *p = format_string; // This variable will use for iterating each character of the string

        std::string full_text;

        while (*p) { // While p is not pointing to NULL terminator
            if (*p == '%' && *(p+1) == 's') {
                full_text += va_arg(args, char*);
                // wprintw(win, "%s", va_arg(args, char*));
                // refresh();
                p += 2;
            } else if (*p == '%' && *(p+1) == 'd') {
                full_text += std::to_string(va_arg(args, int));
                // wprintw(win, "%d", va_arg(args, int));
                // refresh();
                p += 2;
            } else {
                // full_text += *p;
                p++;
            }
        }

        wprintw(win, "%s", full_text.c_str());
        va_end(args);
        wmove(win, y+1, x);
        wrefresh(win);
    }

    void printAndMoveDown(WINDOW *win, std::string txt) {
        int y, x;
        getyx(win, y, x);
        wprintw(win, "%s", txt.c_str());
        wmove(win, y+1, x);
        wrefresh(win);
    }

    void typePrintAndMoveDown(WINDOW *win, const char *format_string, int count, ...) {
        int y, x;
        getyx(win, y, x);

        va_list args;
        va_start(args, count);
        const char *p = format_string; // This variable will use for iterating each character of the string

        std::string full_text;

        while (*p) { // While p is not pointing to NULL terminator
            if (*p == '%' && *(p+1) == 's') {
                full_text += va_arg(args, char*);
                // wprintw(win, "%s", va_arg(args, char*));
                // refresh();
                p += 2;
            } else if (*p == '%' && *(p+1) == 'd') {
                full_text += std::to_string(va_arg(args, int));
                // wprintw(win, "%d", va_arg(args, int));
                // refresh();
                p += 2;
            } else {
                // full_text += *p;
                p++;
            }
        }

        va_end(args);

        for (char c : full_text) {
            waddch(win, c);
            wrefresh(win);
            usleep(10000);
        }


        wmove(win, y+1, x);
        wrefresh(win);
    }
}