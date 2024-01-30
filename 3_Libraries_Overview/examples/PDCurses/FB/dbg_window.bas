''------------------------------------------------------------------------------
'' Name:        dbg_window.c
'' Purpose:     Simple Debug Routine example.
'' Title:       "Debug Window"
''
'' Platform:    Win64, Ubuntu64
''
'' Compiler:    FreeBASIC Compiler-1.09.0-win64 TDM-GCC 9.2.0 32/64-bit
'' Depends:     PDCurses V3.9, libtinfo (Linux), VTE (VT100) capable terminal
''
'' Author:      Axle
'' Created:     30/10/2022
'' Updated:     
'' Version:     0.0.1.0 beta
'' Copyright:   (c) Axle 2023
'' Licence:     MIT No Attribution (MIT-0)
''------------------------------------------------------------------------------
'' NOTES:
''
''------------------------------------------------------------------------------

''FreeBASIC Compiler-1.09.0-win64 TDM-GCC 9.2.0 32/64-bit
#include once "curses.bi"
#ifdef __FB_LINUX__
#inclib "tinfo"
#endif

Declare Function main_procedure() As Integer
Declare Sub dbg_print(value As String)

main_procedure()

Function main_procedure() As Integer  ' Main procedure
    
    initscr()
    
    cbreak()

    '' Disable echoing -- characters entered by user won't be shown in the console
    noecho()
    
    dbg_print(!"Debug hello")  '' dbg_print(Str(value))
    
    '' Terminal supports colors?
    If (has_colors()) Then
        start_color()
        
        '' Pair 0 should be the console's default colors
        Dim As Short f, b
        pair_content( 0, @f, @b )
        printw(!"pair 0 (default colors) contains: (%d,%d)\n", f, b)
        
        printw(!"Initializing pair 1 to red/black\n")
        init_pair(1, COLOR_RED, COLOR_BLACK)
        dbg_print(Str(COLOR_RED))
        
        printw(!"Initializing pair 2 to white/blue\n")
        init_pair(2, COLOR_WHITE, COLOR_BLUE)
        
        '' Print some text using pair 1's colors
        attrset(COLOR_PAIR(1))
        printw(!"RED/BLACK\n")
        
        '' Print some text using pair 2's colors
        attrset(COLOR_PAIR(2))
        printw(!"WHITE/BLUE\n")
        
        '' Reset to default colors (pair 0)
        attrset(COLOR_PAIR(0))
        printw(!"Default Colors\n")
    Else
        printw("This demo is only fun in a color terminal!")
    End If
    
    '' Sleep
    printw(!"Waiting for keypress...\n")
    getch()
    
    endwin()
    
    Return 0
End Function  ' END main_procedure <---

''------------------------------------------------------------------------------
'' A simple debug window for curses (ncurses, pdcurses). This gives a second
'' screen that overlays other screens without interference then closes when finished.
'' Convert any value to string when calling the subroutine
'' Dim As Integer value = 45
'' Use: place the following line in your code with the variable to debug.
'' dbg_print(!"Debug hello")  '' dbg_print(Str(value))
''------------------------------------------------------------------------------
Sub dbg_print(value As String)  '' Convert to dbg_print(Str(value)) when calling.
    '' stdscr is the main screen buffer.
    '' We are creating a new screen above stdscr.
    Dim As WINDOW_ ptr dbg_win  '' Pointer to new screen handle dbg_win.
    dbg_win = newwin(0,0,0,0)  '' New window the same size as stdscr.
    wprintw(dbg_win, !"%s \n", value)  '' Print our debug value/s.
    wrefresh(dbg_win)  '' Refresh the dbg_win to display the screen buffer.
    wgetch(dbg_win)  '' Wait for keypress to continue (blocking).
    'werase(dbg_win)  '' Not required as we are closing the window anyway
    'wclear(dbg_win)  '' I just added it here in case you want to create a more complex window.
    'napms(DELAYSIZE)  '' sleeps for at least DELAYSIZE milliseconds.
    'move(LINES - 1, COLS - 1)  '' moves the cursor to the lower right corner.
    delwin(dbg_win)  '' Delete the debug window and return to last screen.
End Sub
