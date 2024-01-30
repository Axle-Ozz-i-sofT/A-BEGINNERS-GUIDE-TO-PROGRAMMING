//------------------------------------------------------------------------------
// Name:        hello_world.c
// Purpose:     Basic "Hello world" example.
// Title:       "Hello world"
//
// Platform:    Win64, Ubuntu64
//
// Compiler:    GCC V9.x.x, MinGw-64, libc (ISO C99)
// Depends:     PDCurses V3.9, libtinfo (Linux), VTE (VT100) capable terminal
//
// Author:      Axle
// Created:     30/10/2022
// Updated:     28/06/2023
// Version:     0.0.1.1 beta
// Copyright:   (c) Axle 2023
// Licence:     MIT No Attribution (MIT-0)
//------------------------------------------------------------------------------
// NOTES:
// Additional functions show use of window ID as well as removing the cursor
// from the screen. Additional *print*() functions to choose window ID and place
// text string at x,y.
//------------------------------------------------------------------------------

//#include <stdio.h>  // stdio.h is included by curses.h
//#include <stdlib.h>

// #define PDC_DLL_BUILD  // When using shared (DLL)
#include <curses.h>  // Windows, Linux

// Linker -lpdcurses(default) - libpdcurses.a, -lpdcursesdll - libpdcursesdll.a
// Note: pdcurses library is named pdcurses.a by default, not libpdcurses.a

int main()  // ncurses
    {
    initscr();			           // Start curses mode as stdscr.
    //WINDOW *win = initscr();  // Start curses mode, and get handle to stdscr.
    clear();
    //wclear(stdscr);            // Clear the screen (WINDOW *win)
    //wclear(win);            // Clear the screen (WINDOW *win, aka stdscr)
    printw("Hello World !!!\n");	   // Print Hello World
    //int printw(char *fmt, ...);
    //int mvprintw(int y, int x, char *fmt, ...);
    //int wprintw(WINDOW *win, char *fmt, ...);
    //int mvwprintw(WINDOW *win, int y, int x, char *fmt, ...);
    //curs_set(0);               // Set the curser to invisible (0, 1, 2)
    refresh();			           // Print it on to the real screen
    wrefresh(stdscr);          // Print it on to the real screen  (WINDOW *win)
    getch();			           // Wait for user input. Also does w/refresh().
    //curs_set(1);               // Set the curser to visible (2 = High vis mode)
    endwin();			           // End curses mode

    return 0;
    }


