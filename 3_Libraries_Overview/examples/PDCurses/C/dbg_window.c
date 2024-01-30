//------------------------------------------------------------------------------
// Name:        dbg_window.c
// Purpose:     Using a second window instance to show debugging values. This
//              window overlay does not interfere with the layout of our
//              default windows. 
// Title:       "Simple Debug Routine"
//
// Platform:    Win64, Ubuntu64
//
// Compiler:    GCC V9.x.x, MinGw-64, libc (ISO C99)
// Depends:     PDCurses V3.9, , libtinfo (Linux), VTE (VT100) capable terminal
//
// Author:      Axle
// Created:     08/11/2022
// Updated:     28/06/2023
// Version:     0.0.1.1 beta
// Copyright:   (c) Axle 2023
// Licence:     MIT No Attribution (MIT-0)
//------------------------------------------------------------------------------
// NOTES:
// The debug print screen overlays the default screen. The default screen
// remains hidden until the debug screen closes.
//------------------------------------------------------------------------------

#include <curses.h>  // Windows, Linux

#define unused(x) (x) = (x)  // suppress agrc argv unused warning

#ifdef _WIN32
#include <stdlib.h>  // Required for Debug Print routine.
#endif
#ifdef __linux__
// Linux has no itoa() implementation, so I added this version to fill the gap.
char* itoa_x(int value, char* str, int radix);
#define itoa itoa_x
#endif


void dbg_print(char value[]);  // Debug Print

int main(int argc, char **argv)
    {
    unused(argc);  // Turns off the compiler warning for unused argc, argv
    unused(argv);  // Turns off the compiler warning for unused argc, argv

    // Start the curse screen.
    initscr();
    // Disable stdout buffer. Characters are displayed immediately without \n
    cbreak();
    // Disable echoing -- characters entered by user won't be shown in the console
    noecho();
    
    // Terminal supports colors?
    if (has_colors())
    {
        // Start color support.
        start_color();
        
        // Pair 0 should be the console's default colors
        short f, b;
        pair_content( 0, &f, &b );
        //char buffer[64]={'\0'};dbg_print( itoa(f, buffer, 10));  // DEBUG PRINT
        
        printw("pair 0 (default colors) contains: (%d,%d)\n", f, b);
        
        printw("Initializing pair 1 to red/black\n");
        init_pair(1, COLOR_RED, COLOR_BLACK);
        char buffer[64]={'\0'};dbg_print( itoa(COLOR_RED, buffer, 10));  // DEBUG PRINT
        
        printw("Initializing pair 2 to white/blue\n");
        init_pair(2, COLOR_WHITE, COLOR_BLUE);
        
        // Print some text using pair 1's colors
        attrset(COLOR_PAIR(1));
        printw("RED/BLACK\n");
        
        // Print some text using pair 2's colors
        attrset(COLOR_PAIR(2));
        printw("WHITE/BLUE\n");
        
        // Reset to default colors (pair 0)
        attrset(COLOR_PAIR(0));
        printw("Default Colors\n");
    }
    else
    {
        printw("This demo is only fun in a color terminal!");
    }
    
    // napms(DELAYSIZE)  // sleeps for at least DELAYSIZE milliseconds
    printw("Waiting for key-press...\n");
    getch();

    // Reset to the console defaults before exiting curses mode.
    nocbreak();
    echo();
    endwin();

    return 0;
    }

//------------------------------------------------------------------------------
// #include <stdlib.h>  // Required for Debug Print routine.
// A simple debug windows for curses (ncurses, pdcurses). This gives a second
// screen that overlays other screens without interference then closes when finished.
// Convert any value to string when calling the subroutine
// Use: place the following lines in your code with the variable to debug.
// Dim As Integer value = 45
// char buffer[64] = {'\0'}  // Max 64 characters
// dbg_print("Debug hello")  // dbg_print(itoa(,buffer, value, 10))
// ltoa(), ftoa(), etc. see also sprintf()
// char buffer[64]={'\0'};dbg_print( itoa(value, buffer, 10));
//------------------------------------------------------------------------------
void dbg_print(char value[])  // Convert to dbg_print(Str(value)) when calling.
{
    // stdscr is the main screen buffer.
    // We are creating a new screen above stdscr.
    WINDOW *dbg_win;  // Pointer to new screen handle dbg_win.
    dbg_win = newwin(0,0,0,0);  // New window the same size as stdscr.
    wprintw(dbg_win, "Debug Print Window\nDebug Value:\n");
    wprintw(dbg_win, "%s \n", value);  // Print our debug value/s.
    wprintw(dbg_win, "Press any key to end debug screen...\n");
    wrefresh(dbg_win);  // Refresh the dbg_win to display the screen buffer.
    wgetch(dbg_win);  // Wait for key-press to continue (blocking).
    //werase(dbg_win);  // Not required as we are closing the window anyway
    //wclear(dbg_win);  // I just added it here in case you want to create a more complex window.
    //napms(DELAYSIZE);  // sleeps for at least DELAYSIZE milliseconds.
    //move(LINES - 1, COLS - 1);  // moves the cursor to the lower right corner.
    delwin(dbg_win);  // Delete the debug window and return to last screen.
}

#ifdef __linux__
// Linux has no itoa() implementation, so I added this version to fill the gap.

/* The Itoa code is in the public domain */
char* itoa_x(int value, char* str, int radix) {
    static char dig[] =
        "0123456789"
        "abcdefghijklmnopqrstuvwxyz";
    int n = 0, neg = 0;
    unsigned int v;
    char* p, *q;
    char c;

    if (radix == 10 && value < 0) {
        value = -value;
        neg = 1;
    }
    v = value;
    do {
        str[n++] = dig[v%radix];
        v /= radix;
    } while (v);
    if (neg)
        str[n++] = '-';
    str[n] = '\0';
    for (p = str, q = p + n/2; p != q; ++p, --q)
        c = *p, *p = *q, *q = c;
    return str;
}
#endif
