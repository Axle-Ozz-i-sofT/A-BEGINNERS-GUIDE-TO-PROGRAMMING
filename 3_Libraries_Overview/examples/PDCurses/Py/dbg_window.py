#------------------------------------------------------------------------------
# Name:        dbg_window.py
# Purpose:     Simple Debug Routine example. 
# Title:       "Debug Window"
#
# Platform:    Win64, Ubuntu64
#
# Compiler:    GCC V9.x.x, MinGw-64, libc (ISO C99)
# Depends:     PDCurses V3.9, libtinfo (Linux), VTE (VT100) capable terminal
#
# Author:      Axle
# Created:     07/11/2022
# Updated:     
# Version:     0.0.1.0 beta
# Copyright:   (c) Axle 2022
# Licence:     MIT No Attribution (MIT-0)
#------------------------------------------------------------------------------
# NOTES:
#
#------------------------------------------------------------------------------


import curses


def main():

    if 1 == Con_IsREPL():
        print("This application is best viewed running from the OS Command interpreter")
        Con_Pause()

    global stdscr  # Global declaration needed to use in other functions.
    stdscr = curses.initscr()
    curses.cbreak()
    # Disable echoing -- characters entered by user won't be shown in the console
    curses.noecho()
    
    # Terminal supports colors?
    if curses.has_colors():

        curses.start_color()
        
        # Pair 0 should be the console's default colors
        # Python curses functions behave differently to the C API.
        curses.pair_content( 0 )
        stdscr.addstr("pair 0 (default colors) contains: " + str(curses.pair_content( 0 )) + "\n")
        dbg_print( curses.pair_content( 0 ) )
        
        stdscr.addstr("Initializing pair 1 to red/black\n")
        curses.init_pair(1, curses.COLOR_RED, curses.COLOR_BLACK)
        #dbg_print( str(curses.COLOR_RED) )
        
        stdscr.addstr("Initializing pair 2 to white/blue\n")
        curses.init_pair(2, curses.COLOR_WHITE, curses.COLOR_BLUE)
        
        # Print some text using pair 1's colors
        stdscr.attrset(curses.color_pair(1))
        stdscr.addstr("RED/BLACK\n")
        
        # Print some text using pair 2's colors
        stdscr.attrset(curses.color_pair(2))
        stdscr.addstr("WHITE/BLUE\n")
        
        # Reset to default colors (pair 0)
        stdscr.attrset(curses.color_pair(0))
        stdscr.addstr("Default Colors\n")

    else:
        stdscr.addstr("This demo is only fun in a color terminal!")

    
    # napms(DELAYSIZE)  # sleeps for at least DELAYSIZE milliseconds
    stdscr.addstr("Waiting for keypress...\n")
    stdscr.getch()

    # Reset to the console defaults before exiting curses mode.
    curses.nocbreak()
    curses.echo()
    curses.endwin()

    return None


#------------------------------------------------------------------------------
# A simple debug window for curses (ncurses, pdcurses). This gives a second
# screen that overlays other screens without interference then closes when finished.
# Convert any value to string when calling the subroutine
# value = 45
# dbg_print("Debug hello")  # dbg_print( value )
# Any object can be sent to dbg_print() as the str() function will convert it
# to it string representation.
# take care with creating newwin with the same size dimensions as stdscr as the
# python binder can error. If you have issues reduce the newwin border dimensions
# by 1  curses.newwin(nlines-2, ncols-2, begin_y+1, begin_x+1)
# Use: place the following line in your code with the variable to debug.
# dbg_print( "0" )  ## DEBUG PRINT ##
#------------------------------------------------------------------------------
def dbg_print(value):  # Convert to dbg_print(Str(value)) when calling.

    if 1 == Con_IsREPL():
        print("This application is best viewed running from the OS Command interpreter")
        Con_Pause()

    # stdscr is the main screen buffer.
    # We are creating a new screen above stdscr.
    #WINDOW *dbg_win  # Pointer to new screen hanlde dbg_win.
    dbg_win = curses.newwin(0,0,0,0)  # New window the same size as stdscr
    dbg_win = curses.newwin(curses.LINES,curses.COLS,0,0)
    dbg_win.addstr( str(value) + "\n")  # Print our debug value/s.
    dbg_win.refresh()  # Refresh the dbg_win to display the screen buffer.
    dbg_win.getch()  # Wait for keypress to continue (blocking).
    #dbg_win.erase()  # Not required as we are closing the window anyway
    #dbg_win.clear()  # I just added it here in case you want to create a more complex window.
    #curses.napms(DELAYSIZE)  # sleeps for at least DELAYSIZE milliseconds.
    #dbg_win.move(LINES - 1, COLS - 1)  # moves the cursor to the lower right corner.
    #dbg_win.delwin()  # Delete the debug window and return to last screen.
    # python curses(Linux) has no delwin. It is assumed the the local object
    # will be deleted when the function returns. Just in case I am del(eting)
    # the object, but it may not be required.
    # Experimental! Should delete the WINDOW object an clear memory
    del dbg_win
    return None

# Test if we are inside of the REPL interactive interpreter.
# This function is in alpha and may not work as expected.
def Con_IsREPL():
    import os
    if os.sys.stdin and os.sys.stdin.isatty():
        if os.isatty(os.sys.stdout.fileno()):
            return 0  # OS Command Line
        else:
            return 1  # REPL - Interactive Linux?
    else:
        return 1  # REPL - Interactive Windows?
    return None

if __name__ == '__main__':
    main()
