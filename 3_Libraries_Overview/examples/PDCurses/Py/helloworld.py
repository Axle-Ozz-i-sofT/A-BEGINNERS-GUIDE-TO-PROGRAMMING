#------------------------------------------------------------------------------
# Name:        helloworld.py
# Purpose:     Basic "Hello world" example.
# Title:       "Hello world"
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
# Additional functions show use of window ID as well as removing the cursor
# from the screen. Additional *print*() functions to choose window ID and place
# text string at x,y.
#------------------------------------------------------------------------------
# TODO:
# No printw(), use addstr()
# stdscr.getch() Fails!
# No print, use addstr()
# window.keypad(flag)??
#------------------------------------------------------------------------------

import curses

def main():

    if 1 == Con_IsREPL():
        print("This application is best viewed running from the OS Command interpreter")
        Con_Pause()

    stdscr = curses.initscr()                 # Start curses mode
    curses.noecho()
    #curses.cbreak()
    #stdscr.nodelay(False)
    stdscr.keypad(True)
    stdscr.clear()            # Clear the screen
    curses.flushinp()

    stdscr.addstr("Hello World !!!") # Print Hello World
    #curses.curs_set(0)               # Set the curser to invisible (0, 1, 2)
    stdscr.refresh()          # Print it on to the real screen
    #curses.flushinp()
    stdscr.getkey()                  # Wait for user input (keypress)
    #curses.napms(2000)
    #curses.curs_set(1)               # Set the curser to visible (2 = High vis mode)

    #curses.nocbreak()
    stdscr.keypad(False)
    curses.echo()
    curses.endwin()                  # End curses mode

    x = input()
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
