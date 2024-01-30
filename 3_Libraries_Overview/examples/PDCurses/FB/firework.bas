
''------------------------------------------------------------------------------
'' Name:        \PDCurses-3.9\demos\firework.c
'' Purpose:     Modified by Axle. 
'' Title:       "PDCurses fireworks example"
''
'' Platform:    Win64, Ubuntu64
''
'' Compiler:    FreeBASIC Compiler-1.09.0-win64 TDM-GCC 9.2.0 32/64-bit
'' Depends:     PDCurses V3.9, libtinfo (Linux), VTE (VT100) capable terminal
''
'' Author:      Axle
'' Created:     30/10/2022
'' Updated:     30/06/2023
'' Version:     0.0.1.2 beta
'' Copyright:   (c) Axle 2022
'' Licence:     MIT No Attribution (MIT-0)
''------------------------------------------------------------------------------
'' NOTES:
'' Press any key to quit.
''
'' Functions #define as MACROS in ncurses.bi containing
'' IIf ( condition, expr_if_true, expr_if_false ) must receive a return value
'' as an expression.
'' ie. The MACRO
'' #define mvwaddstr(win, y, x, str) iif(wmove(win, y, x) = ERR_, ERR_, waddnstr(win, str, -1))
'' requires
'' Dim As Long result = mvwaddstr(stdscr, row, col, @"-")
'' SEE: check_error()
''------------------------------------------------------------------------------
'' The following symbols have been renamed:
''     constant TRUE => CTRUE
''     constant ERR => ERR_
''     typedef WINDOW => WINDOW_
''     struct SCREEN => SCREEN_
''     procedure beep => beep_
''     procedure clear => clear_
''     procedure erase => erase_
''     procedure instr => instr_
''     procedure getmouse => getmouse_

''#define __FB_WIN32__
''#define PDC_DLL_BUILD

#include once "curses.bi"
#ifdef __FB_LINUX__
#inclib "tinfo"
#endif

#define DELAYSIZE 200
'' LINES defined in n/curses number of lines on terminal screen
'' COLS defined in n/curses number of columns on terminal screen
'' Windows 10 CMD.EXE default
'' LINES = 30 DEBUG (NOTE = 0 to 29)
'' COLS = 120 DEBUG (NOTE = 0 to 119)
'' XTerm default
'' curses.LINES = 24 DEBUG (NOTE = 0 to 23)
'' curses.COLS = 80 DEBUG (NOTE = 0 to 79)

Declare Function main_procedure() As Integer

Declare Sub myrefresh()
Declare Sub get_color()
Declare Sub explode(row As Long, col As Long)
Declare Function check_error(Byval status As Long) As Long  '' Error check routine.

dim Shared As Short color_table(8) = { _
    COLOR_RED, COLOR_BLUE, COLOR_GREEN, COLOR_CYAN, _
    COLOR_RED, COLOR_MAGENTA, COLOR_YELLOW, COLOR_WHITE}

main_procedure()

Function main_procedure() As Integer  ' Main procedure

    Dim as Long start, end1, row, diff, flag, direction
    Dim As Short i

    '' stdscr is the main screen buffer. Smaller screens (Panels) can be placed 
    '' above the main screen.
    initscr()  '' Start curses mode 
    keypad(stdscr, CTRUE)  '' Get function keys without testing escape sequence
    nodelay(stdscr, CTRUE)  '' Forces non blocking of getch().
                            '' False make getch() wait (blocking) until a key press.
    noecho()  '' Disable printing user input to the screen.

    If (has_colors()) Then  '' Check if the terminal can support colours.
        start_color()  '' initializes 8x8 colors, 8 foreground, 8 background.
    End If

    for i = 0 To 8 -1 Step 1
        '' The second value (background) is always set to black in this example.
        init_pair(i, color_table(i), COLOR_BLACK)  '' COLOR_PAIR (color_table[i], Black)
    Next i

    Randomize , 1  '' generate random numbers table from Time.
    flag = 0

    '' You could change this to test for a specific key.
    while (getch() = ERR_)  '' loop until any key is hit.
            
        do  '' Loop through rand screen positions until within boundaries.
            '' Note: rnd*100 will generate rnd numbers between 0 and 99.
            '' rnd*100 +1 or (rnd*100) +1 , 1 to 100.
            start = Rnd * (COLS - 3)
            end1 = Rnd * (COLS - 3)
            start = IIf(start < 2, 2, start)  '' Ternary conditional operator,
            end1 = IIf(end1 < 2, 2, end1)  '' ternary if, inline if (IIF).
            direction = IIf(start > end1, -1, 1)
            diff = Abs(start - end1)  '' Change -integer to +integer.
        Loop While (diff < 2 Or diff >= LINES - 2)  '' Conditional end do while.
        
        attrset(A_NORMAL)  '' set character attributes to normal.

        for row = 0 To diff -1 Step 1  '' Draw launch lines.
            check_error(mvaddstr(LINES -1 - row, row * direction + start, _
            IIf(direction < 0, @!"\\", @!"/")))

            flag += 1
            if (flag > 1) Then  '' >1 keeps 2 x \ or / displayed.
                myrefresh()
                erase_()  '' Clears all y,x in the screen. clear() clears the
                flag = 0  '' entire screen buffer.
            End If
        Next row

        flag += 1
        if (flag > 1) Then  '' >1 Displays the last \ or / before explosion.
            myrefresh()
            flag = 0
        End If

        '' Draw fireworks explode from rnd values
        explode(LINES -1 - row, diff * direction + start)
        erase_()  '' Clears the fireworks for next launch.
        myrefresh()
    WEnd  '' end while

    endwin()  '' End curses mode

    Return 0
End Function  ' END main_procedure <---

'' Explode. Draw each ASCII frame.
Sub explode(row As Long, col As Long)  '' Maybe consider Long
    erase_()
    check_error(mvaddstr(row, col, @"-"))
    myrefresh()

    col = col - 1  '' Adjust left x boundary for string width centre

    get_color()
    check_error(mvaddstr(row - 1, col, @" - "))
    check_error(mvaddstr(row,     col, @"-+-"))
    check_error(mvaddstr(row + 1, col, @" - "))
    myrefresh()

    col = col - 1

    get_color()
    check_error(mvaddstr(row - 2, col, @" --- "))  '' row - 2 expand to give explosion effect
    check_error(mvaddstr(row - 1, col, @"-+++-"))
    check_error(mvaddstr(row,     col, @"-+#+-"))
    check_error(mvaddstr(row + 1, col, @"-+++-"))
    check_error(mvaddstr(row + 2, col, @" --- "))
    myrefresh()

    get_color()
    check_error(mvaddstr(row - 2, col, @" +++ "))
    check_error(mvaddstr(row - 1, col, @"++#++"))
    check_error(mvaddstr(row,     col, @"+# #+"))
    check_error(mvaddstr(row + 1, col, @"++#++"))
    check_error(mvaddstr(row + 2, col, @" +++ "))
    myrefresh()

    get_color()
    check_error(mvaddstr(row - 2, col, @"  #  "))
    check_error(mvaddstr(row - 1, col, @"## ##"))
    check_error(mvaddstr(row,     col, @"#   #"))
    check_error(mvaddstr(row + 1, col, @"## ##"))
    check_error(mvaddstr(row + 2, col, @"  #  "))
    myrefresh()

    get_color()
    check_error(mvaddstr(row - 2, col, @" # # "))
    check_error(mvaddstr(row - 1, col, @"#   #"))
    check_error(mvaddstr(row,     col, @"     "))
    check_error(mvaddstr(row + 1, col, @"#   #"))
    check_error(mvaddstr(row + 2, col, @" # # "))
    myrefresh()
End Sub

Sub myrefresh()
    napms(DELAYSIZE)  '' sleeps for at least DELAYSIZE milliseconds
    move(LINES - 1, COLS - 1)  '' moves the cursor to the lower right corner
    refresh()
End Sub

'' Generate our random color set for each frame
Sub get_color()
    Dim As chtype bold = IIf(Rnd * 2, A_BOLD, A_NORMAL)  '' Rnd(0|1) bold/normal
    attrset(COLOR_PAIR((Rnd * 8) Or bold))  '' sets text color+[A_BOLD|A_NORMAL].
    
	'' A_NORMAL        Normal display (no highlight)
	'' A_STANDOUT      Best highlighting mode of the terminal
	'' A_UNDERLINE     Underlining
	'' A_REVERSE       Reverse video
	'' A_BLINK         Blinking
	'' A_DIM           Half bright
	'' A_BOLD          Extra bright or bold
	'' A_PROTECT       Protected mode
	'' A_INVIS         Invisible or blank mode
	'' A_ALTCHARSET    Alternate character set
	'' A_CHARTEXT      Bit-mask to extract a character
	'' COLOR_PAIR(n)   Color-pair number n
End Sub

'' Error check routine. 
Function check_error(Byval status As Long) As Long
	'' ncurses(.bi) only has 2 error return values.
	'' ERR_ = -1
	'' OK = 0
	If status = -1 Then  '' If status = ERR_ Then
		Print "curses API error!"  '' DEBUG
        '' Handle error appropriately...
        'endwin()  '' End curses mode
        'End 1  '' Terminate the application
        ' or
		'Return 1  '' return an error value to main_procedure()
    End If
    return 0
End Function
