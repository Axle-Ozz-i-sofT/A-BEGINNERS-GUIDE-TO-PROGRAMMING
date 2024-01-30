#------------------------------------------------------------------------------
# Name:        \PDCurses-3.9\demos\firework.c
# Purpose:     Modified by Axle. 
# Title:       "PDCurses fireworks example"
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
# Press any key to quit.
#------------------------------------------------------------------------------


import curses
import random

DELAYSIZE = 200  # global ?
# LINES defined in n/curses number of lines on terminal screen
# COLS defined in n/curses number of columns on terminal screen
# Windows 10 CMD.EXE default
# LINES = 30 DEBUG (NOTE = 0 to 29)
# COLS = 120 DEBUG (NOTE = 0 to 119)
# XTerm default
# curses.LINES = 24 DEBUG (NOTE = 0 to 23)
# curses.COLS = 80 DEBUG (NOTE = 0 to 79)

color_table = [curses.COLOR_RED, \
	curses.COLOR_BLUE, \
	curses.COLOR_GREEN, \
	curses.COLOR_CYAN, \
    curses.COLOR_RED, \
    curses.COLOR_MAGENTA, \
    curses.COLOR_YELLOW, \
    curses.COLOR_WHITE]

def main():

    if 1 == Con_IsREPL():
        print("This application is best viewed running from the OS Command interpreter")
        Con_Pause()

    #global color_table
    start = end1 = row = diff = flag = direction = 0
    i = 0  # short

    # stdscr is the main screen buffer. Smaller screens (Panels) can be placed 
    # above the main screen.
    global stdscr
    stdscr = curses.initscr()  # Start curses mode 
    stdscr.keypad(True)  # Get function keys without testing escape sequence
    stdscr.nodelay(True)  # Forces non blocking of getch().
                            # False make getch() wait (blocking) until a key press.
    curses.noecho()  # Disable printing user input to the screen.

    if curses.has_colors():  # Check if the terminal can support colours.
        curses.start_color()  # initialises 8x8 colors, 8 foreground, 8 background.

    for i in range(0, 8):  # Range = 0 to 8 -1
        # init_pair( 0. is protected and cannot be used. We start at position 1 (i+1).
        # The second value (background) is always set to black in this example.
        curses.init_pair(i+1, color_table[i], curses.COLOR_BLACK)  # ()COLOR_PAIR (color_table[i], Black)

    random.seed()  # generate random numbers table from Time.
    flag = 0

    # You could change this to test for a specific key.
    while (stdscr.getch() == curses.ERR):  # loop until any key is hit.
            
        while True: # Python does not have a Do While loop.  
			# Loop through rand screen positions until within boundaries.
            # Note: rnd*100 will generate rnd numbers between 0 and 99.
            # rnd*100 +1 or (rnd*100) +1 , 1 to 100.
            start = random.randint(0, curses.COLS - 3)
            end1 = random.randint(0, curses.COLS - 3)
            start = 2 if start < 2 else start  # Ternary conditional operator,
            end1 = 2 if end1 < 2 else end1  # ternary if, inline if (IIF).
            direction = -1 if start > end1 else 1
            diff = abs(start - end1)  # Change -integer to +integer.
            ## Double check the boundaries of the following test.
            if (diff > 2) and (diff <= curses.LINES-2 - 1):  # Conditional end do while.
                break

        stdscr.attrset(curses.A_NORMAL)  # set character attributes to normal.

        for row in range(diff):  # Draw launch lines.
            stdscr.addstr(curses.LINES-1 - row, row * direction + start, ("\\" if direction < 0 else "/")) #("\\" if direction < 0 else "/")|str("*")

            flag += 1
            if (flag > 1):  # >1 keeps 2 x \ or / displayed.
                myrefresh()
                stdscr.erase()  # Clears all y,x in the screen. clear() clears the
                flag = 0  # entire screen buffer.

        flag += 1
        if (flag > 1):  # >1 Displays the last \ or / before explosion.
            myrefresh()
            flag = 0

        # Draw fireworks explode from rnd values
        explode(curses.LINES-1 - row, diff * direction + start)
        stdscr.erase()  # Clears the fireworks for next launch.
        myrefresh()
    # end while

    curses.endwin()  # End curses mode

    return None

# Explode. Draw each ASCII frame.
def explode(row , col):  # Maybe consider Long
    stdscr.erase()
    stdscr.addstr(row, col, "-")
    myrefresh()

    col = col - 1  # Adjust left x boundary for string width centre

    get_color()
    stdscr.addstr(row - 1, col, " - ")
    stdscr.addstr(row,     col, "-+-")
    stdscr.addstr(row + 1, col, " - ")
    myrefresh()

    col = col - 1

    get_color()
    stdscr.addstr(row - 2, col, " --- ")  # row - 2 expand to give explosion effect
    stdscr.addstr(row - 1, col, "-+++-")
    stdscr.addstr(row,     col, "-+#+-")
    stdscr.addstr(row + 1, col, "-+++-")
    stdscr.addstr(row + 2, col, " --- ")
    myrefresh()

    get_color()
    stdscr.addstr(row - 2, col, " +++ ")
    stdscr.addstr(row - 1, col, "++#++")
    stdscr.addstr(row,     col, "+# #+")
    stdscr.addstr(row + 1, col, "++#++")
    stdscr.addstr(row + 2, col, " +++ ")
    myrefresh()

    get_color()
    stdscr.addstr(row - 2, col, "  #  ")
    stdscr.addstr(row - 1, col, "## ##")
    stdscr.addstr(row,     col, "#   #")
    stdscr.addstr(row + 1, col, "## ##")
    stdscr.addstr(row + 2, col, "  #  ")
    myrefresh()

    get_color()
    stdscr.addstr(row - 2, col, " # # ")
    stdscr.addstr(row - 1, col, "#   #")
    stdscr.addstr(row,     col, "     ")
    stdscr.addstr(row + 1, col, "#   #")
    stdscr.addstr(row + 2, col, " # # ")
    myrefresh()
    return None

def myrefresh():
    curses.napms(DELAYSIZE)  # sleeps for at least DELAYSIZE milliseconds
    stdscr.move(curses.LINES-1 , curses.COLS-1 )  # moves the cursor to the lower right corner
    stdscr.refresh()
    return None

# Generate our random color set for each frame
def get_color():
    bold = curses.A_BOLD if random.randint(0, 1) else curses.A_NORMAL  # random.randint(0|1) bold/normal
    rnd1 = random.randint(1, 8)
    stdscr.attrset(curses.color_pair( rnd1 ) | bold)  # sets text color+[A_BOLD|A_NORMAL].| bold
    return None
#A_NORMAL        Normal display (no highlight)
#A_STANDOUT      Best highlighting mode of the terminal
#A_UNDERLINE     Underlining
#A_REVERSE       Reverse video
#A_BLINK         Blinking
#A_DIM           Half bright
#A_BOLD          Extra bright or bold
#A_PROTECT       Protected mode
#A_INVIS         Invisible or blank mode
#A_ALTCHARSET    Alternate character set
#A_CHARTEXT      Bit-mask to extract a character
#COLOR_PAIR(n)   Color-pair number n


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
    
