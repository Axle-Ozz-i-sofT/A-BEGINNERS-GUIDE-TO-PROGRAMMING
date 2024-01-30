//------------------------------------------------------------------------------
// Name:        \PDCurses-3.9\demos\firework.c
// Purpose:     Modified by Axle. 
// Title:       "PDCurses fireworks example"
//
// Platform:    Win64, Ubuntu64
//
// Compiler:    GCC V9.x.x, MinGw-64, libc (ISO C99)
// Depends:     PDCurses V3.9, libtinfo (Linux), VTE (VT100) capable terminal
//
// Author:      Axle
// Created:     30/10/2022
// Updated:     
// Version:     0.0.1.0 beta
// Copyright:   (c) Axle 2022
// Licence:     MIT No Attribution (MIT-0)
//------------------------------------------------------------------------------
// NOTES:
// Press any key to quit.
//------------------------------------------------------------------------------
#include <curses.h>  // Windows, linux
#include <stdlib.h>
#include <time.h>

#define unused(x) (x) = (x)  // suppress agrc argv unused warning

#define DELAYSIZE 200
// LINES defined in n/curses number of lines on terminal screen
// COLS defined in n/curses number of columns on terminal screen
// Windows 10 CMD.EXE default
// LINES = 30 DEBUG (NOTE = 0 to 29)
// COLS = 120 DEBUG (NOTE = 0 to 119)
// XTerm default
// curses.LINES = 24 DEBUG (NOTE = 0 to 23)
// curses.COLS = 80 DEBUG (NOTE = 0 to 79)

void myrefresh(void);
void get_color(void);
void explode(int, int);

short color_table[] =
    {
    COLOR_RED, COLOR_BLUE, COLOR_GREEN, COLOR_CYAN,
    COLOR_RED, COLOR_MAGENTA, COLOR_YELLOW, COLOR_WHITE
    };

int main(int argc, char **argv)
    {
    unused(argc);  // Turns off the compiler warning for unused argc, argv
    unused(argv);  // Turns off the compiler warning for unused argc, argv
    time_t seed;  // used to "seed" rnd numb generator
    int start, end, row, diff, flag, direction;
    short i;

    // stdscr is the main screen buffer. Smaller screens (Panels) can be placed 
    // above the main screen.
    initscr();  // Start curses mode 
    keypad(stdscr, TRUE);  // Get function keys without testing escape sequence
    nodelay(stdscr, TRUE);  // Forces non blocking of getch().
                            // False make getch() wait (blocking) until a key press.
    // cbreak()  // Similar to nodelay()
    noecho();  // Disable printing user input to the screen

    if (has_colors())  // Check if the terminal can support colours
    {
        start_color();  // initialises 8x8 colors, 8 foreground, 8 background
    }

    for (i = 0; i < 8; i++)
    {   // The second value (background) is always set to black in this example
        init_pair(i+1, color_table[i], COLOR_BLACK);  // COLOR_PAIR (color_table[i], Black)
    }

    seed = time((time_t *)0);  // use current time as seed
    srand(seed);  // generate random numbers table
    flag = 0;

    // You could change this to test for a specific key
    while (getch() == ERR)  // loop until any key is hit
        {
        do  // Loop through rand screen positions until within boundaries
            {
            start = rand() % (COLS - 3);
            end = rand() % (COLS - 3);
            start = (start < 2) ? 2 : start;  // Ternary conditional operator,
            end = (end < 2) ? 2 : end;  // ternary if, inline if (IIF).
            direction = (start > end) ? -1 : 1;
            diff = abs(start - end);  // abs() change signed int to unsigned int.
            }
        while (diff < 2 || diff >= LINES - 2);  // Conditional end do while.
        
        attrset(A_NORMAL);  // set character attributes to normal

        for (row = 0; row < diff; row++)  // Draw launch lines
            {
            mvaddstr(LINES - row, row * direction + start,
                     (direction < 0) ? "\\" : "/");

            if (flag++)  // >1 keeps 2 x \ or / displayed.
                {
                myrefresh();
                erase();  // Clears all y,x in the screen. clear() clears the entire screen buffer.
                flag = 0;
                }
            }

        if (flag++)  // >1 Displays the last \ or / before explosion.
            {
            myrefresh();
            flag = 0;
            }

        // Draw fireworks explode from rnd values
        explode(LINES -1 - row, diff * direction + start);
        erase();
        myrefresh();
        }  // End while

    // Turn off console controls before exiting to restore the terminal to
    // its default state.
    keypad(stdscr, FALSE);
    nodelay(stdscr, FALSE);
    nocbreak();
    echo();

    endwin();  // End curses mode

    return 0;
    }

// Explode. Draw each ASCII frame.
void explode(int row, int col)
    {
    erase();
    mvaddstr(row, col, "-");
    myrefresh();

    --col;  // Adjust left x boundary for string width centre

    get_color();
    mvaddstr(row - 1, col, " - ");
    mvaddstr(row,     col, "-+-");
    mvaddstr(row + 1, col, " - ");
    myrefresh();

    --col;

    get_color();
    mvaddstr(row - 2, col, " --- ");// row - 2 expand to give explosion effect
    mvaddstr(row - 1, col, "-+++-");
    mvaddstr(row,     col, "-+#+-");
    mvaddstr(row + 1, col, "-+++-");
    mvaddstr(row + 2, col, " --- ");
    myrefresh();

    get_color();
    mvaddstr(row - 2, col, " +++ ");
    mvaddstr(row - 1, col, "++#++");
    mvaddstr(row,     col, "+# #+");
    mvaddstr(row + 1, col, "++#++");
    mvaddstr(row + 2, col, " +++ ");
    myrefresh();

    get_color();
    mvaddstr(row - 2, col, "  #  ");
    mvaddstr(row - 1, col, "## ##");
    mvaddstr(row,     col, "#   #");
    mvaddstr(row + 1, col, "## ##");
    mvaddstr(row + 2, col, "  #  ");
    myrefresh();

    get_color();
    mvaddstr(row - 2, col, " # # ");
    mvaddstr(row - 1, col, "#   #");
    mvaddstr(row,     col, "     ");
    mvaddstr(row + 1, col, "#   #");
    mvaddstr(row + 2, col, " # # ");
    myrefresh();
    }

void myrefresh(void)
    {
    napms(DELAYSIZE);  // sleeps for at least DELAYSIZE milliseconds
    move(LINES - 1, COLS - 1);  // moves the cursor to the lower right corner
    refresh();
    }

// Generate our random color set for each frame
void get_color(void)
    {
    chtype bold = (rand() % 2) ? A_BOLD : A_NORMAL;  // Rnd bold/normal
    attrset(COLOR_PAIR((rand() % 8) +1) | bold);  // sets color|[bold/normal] text.
    }
/*
A_NORMAL        Normal display (no highlight)
A_STANDOUT      Best highlighting mode of the terminal
A_UNDERLINE     Underlining
A_REVERSE       Reverse video
A_BLINK         Blinking
A_DIM           Half bright
A_BOLD          Extra bright or bold
A_PROTECT       Protected mode
A_INVIS         Invisible or blank mode
A_ALTCHARSET    Alternate character set
A_CHARTEXT      Bit-mask to extract a character
COLOR_PAIR(n)   Color-pair number n
*/