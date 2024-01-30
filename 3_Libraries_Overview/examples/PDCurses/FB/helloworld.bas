''------------------------------------------------------------------------------
'' Name:        helloworld.c
'' Purpose:     Basic "Hello world" example.
'' Title:       "Hello world"
''
'' Platform:    Win64, Ubuntu64
''
'' Compiler:    FreeBASIC Compiler-1.09.0-win64 TDM-GCC 9.2.0 32/64-bit
'' Depends:     PDCurses V3.9, libtinfo (Linux), VTE (VT100) capable terminal
''
'' Author:      Axle
'' Created:     30/10/2022
'' Updated:     28/06/2023
'' Version:     0.0.1.1 beta
'' Copyright:   (c) Axle 2023
'' Licence:     MIT No Attribution (MIT-0)
''------------------------------------------------------------------------------
'' NOTES:
'' Additional functions show use of window ID as well as removing the cursor
'' from the screen. Additional *print*() functions to choose window ID and place
'' text string at x,y.
''------------------------------------------------------------------------------

'' #define PDC_DLL_BUILD  // When using shared (DLL)?
#include once "curses.bi"
#ifdef __FB_LINUX__
#inclib "tinfo"
#endif

Declare Function main_procedure() As Integer
main_procedure()

Function main_procedure() As Integer  ' Main procedure
    initscr()                       '' Start curses mode as stdscr.
    'WINDOW *win = initscr()         '' Start curses mode, and get handle to stdscr.
    'clear()                         '' Clear the screen (Not available in FB).
    wclear(stdscr)                  '' Clear the screen (WINDOW *win)
    'wclear(win)                     '' Clear the screen (WINDOW *win, aka stdscr)
    printw("Hello World !!!")       '' Print Hello World
    'int printw(char *fmt, ...);
    ''int mvprintw(int y, int x, char *fmt, ...);
    ''int wprintw(WINDOW *win, char *fmt, ...);
    ''int mvwprintw(WINDOW *win, int y, int x, char *fmt, ...);
    'curs_set(0)                     '' Set the curser to invisible (0, 1, 2)
    'refresh()                       '' Print it on to the real screen (Not available in FB).
    wrefresh(stdscr)                '' Print it on to the real screen  (WINDOW *win)
    getch()                         '' Wait for user input. Also does w/refresh().
    'curs_set(1)                     '' Set the cursor to visible (2 = High vis mode)
    endwin()                        '' End curses mode'' End curses mode

    Return 0
End Function  ' END main_procedure <---