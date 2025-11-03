#ifndef IO_H
#define IO_H

#include <cstdarg>
#include <iostream>
#include <cstring>
#include <ncurses.h>

namespace io {
    void centerText(const char *str);
    void centerText(const char *format_string, int count, ...);
    void centerText(WINDOW *win, bool shouldMove, const char *format_string, int count, ...);
    void printAndMoveDown(WINDOW *win, const char *format_string, int count, ...);
    void printAndMoveDown(WINDOW *win, std::string txt);
    void typePrintAndMoveDown(WINDOW *win, const char *format_string, int count, ...);
}

#endif