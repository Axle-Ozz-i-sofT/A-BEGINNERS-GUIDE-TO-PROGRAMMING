#-------------------------------------------------------------------------------
# Name:        Decisions.py
# Purpose:     Example
# Title:       "A Snail's Life"
#
# Platform:    REPL*, Win64, Ubuntu64
#
# Author:      Axle
# Created:     03/02/2022
# Updated:     19/02/2022
# Copyright:   (c) Axle 2022
# Licence:     MIT No Attribution
#-------------------------------------------------------------------------------
#
# NOTE! Lift the divider on the Python Interpreter output console to see the
# full game in REPL.
# * This example is best run in a native OS console window.
#
# ' \' after a line of code is a line continuation character and allows you to
# split long lines of code over several lines. You can use a comment after a
# continuation character as long as there is no space /#comment.
#-------------------------------------------------------------------------------

import random

# Global Constants
ROWS  = 10
COLUMNS = 40

def main():

    if 1 == Con_IsREPL():  # test if we are in Python 3 REPL.
        print("This application is best viewed from the OS Command interpreter")
        Con_Pause()

    global ROWS, COLUMNS
    Row_start = ROWS - ROWS + 1  # Set the boundaries inside of the fence.
    Row_end = ROWS - 2  # -2 To account for the fence at top and bottom.
    Row_width = Row_end - Row_start  # ROWS - 2 for the fence.
    Col_start = COLUMNS - COLUMNS + 1  # Set the boundaries inside of the fence.
    Col_end = COLUMNS - 2  # -2 To account for the fence at each side.
    Col_width = Col_end - Col_start  #   COLUMNS - 2 for the fence.

    #  I am using strings instead of characters.
    #  Define the characters for the console game.
    chr_1 = " "
    chr_2 = "@"
    chr_3 = "v"
    chr_4 = "w"
    chr_5 = "+"

    # Values to random generate grass.
    # 0 - 10 (0=0% fill, 5= 50% fill, 10= 100% fill)
    GFill = 5
    # 0 - 10 Split rand fill between % 0='w' and 10='W'.
    GSplit = 4

    # defining a 2D array of array[Rows][Columns], initialised to empty.
    # Note! Columns is inside Rows.
    Table = [[None]* COLUMNS for _ in range(0, ROWS)]
    Sprite = [[None]* 2 for _ in range(0, 2)]  # Sprite[0][0|1] = current [Row|Col]
    Row_y = 0  # Counter for rows.
    Col_x = 0  # Counter for columns.

    # Use current time as seed for random generator.
    # random.randint() uses OS_Rand or time as a seed. We don't need to create a
    # seed as we do in C or FreeBASIC.
    # Fill the array with some data.
    for Row_y in range(0, ROWS ):  # Count through each row.
        for Col_x in range(0, COLUMNS ):  # Count through each column.
            if ((Col_x >= Col_start) and (Col_x <= Col_end) \#LineContinue
                and (Row_y >= Row_start) and (Row_y <= Row_end)):
                # Build a string with values for each cell.
                # random.randint(0, 9) returns 0 to 9
                if (random.randint(1, 9) > GFill):  # Total grass GFill = 5(50%) fill.
                    if (random.randint(1, 9) > GSplit):  #  Split between v and w.
                        Table[Row_y][Col_x] = chr_3  # Insert a character for grass.
                    else:
                        Table[Row_y][Col_x] = chr_4  # Insert a character for grass.
                else:
                    Table[Row_y][Col_x] = chr_1  # Insert a space character.
            else:

                Table[Row_y][Col_x] = chr_5  # Insert the fence character.

    # Variables to track the Sprite (Snail) position.
    # Sprite[Sp_now][Sp_row] <- array format.
    Sp_now = 0  # Current sprite position.
    #Const As Integer Sp_last = 1  # previous position (Unused).
    Sp_row = 0
    Sp_col = 1

    # Set the snail at a random location and record it in an array.
    # "Row_width -1" is to keep it in line with C and FB Rand generated range.
    Sprite[Sp_now][Sp_row] = random.randint(0, Row_width -1) +1  # +1 for left 0 boundary alignment..
    Sprite[Sp_now][Sp_col] = random.randint(0, Col_width -1) +1  # +1 for top 0 boundary alignment..

    # Update the Table with the sprite.
    Table[Sprite[Sp_now][Sp_row]][Sprite[Sp_now][Sp_col]] = chr_2

    # Begin main movement and life counter loop
    # Note! that Control character 7 '\a' is system dependent and may not make a sound.
    Inputs = ""  # 'N''S''e''w'
    Lnrg = 10  # Start at 10 life points. Reduces by 1 each move.
    while(1):
        UpdateScreen(Table)  # Update the screen.
        print("+ Life energy= " + str(Lnrg)[0:3])  # String[0:3] allocates 3 positions.
        if (Lnrg <= 0):
            #print("\a", end="")  # System bell (ASCII Control Chr 7) Con_bell()
            Con_bell()
            print("Oh No! You are out of life energy...")
            break
        print("N,S,E,W to move : Q to Quit")
        Inputs = input("Type your selection followed by [Enter] >>")
        print("")
        Lnrg -= 1  # Reduce 1 nrg point for each move including hitting the boundary.

        if (Inputs[0:1] == "N") or (Inputs[0:1] == "n"):
            if (Sprite[Sp_now][Sp_row] - 1 != Row_start -1):
                # The following if else block would be best subed to a function
                # to limit duplication.
                if (Table[Sprite[Sp_now][Sp_row] -1][Sprite[Sp_now][Sp_col]] == chr_3):
                    Lnrg +=1  # Increase 1 nrg point for small grass.
                elif (Table[Sprite[Sp_now][Sp_row] -1][Sprite[Sp_now][Sp_col]] == chr_4):
                    Lnrg +=2  # Increase 2 nrg points for large grass.
                else:
                    pass
                Table[Sprite[Sp_now][Sp_row]][Sprite[Sp_now][Sp_col]] = chr_1
                Sprite[Sp_now][Sp_row] -=1
                Table[Sprite[Sp_now][Sp_row]][Sprite[Sp_now][Sp_col]] = chr_2
                # Move Lnrg -= 1; here to not reduce Life nrg when hitting the boundary.
            else:
                #print("\a", end="") # System bell (ASCII Control Chr 7)
                Con_bell()
        elif (Inputs[0:1] == "S") or (Inputs[0:1] == "s"):
            if (Sprite[Sp_now][Sp_row] + 1 != Row_end +1):
                if (Table[Sprite[Sp_now][Sp_row] +1][Sprite[Sp_now][Sp_col]] == chr_3):
                    Lnrg +=1  # Increase 1 nrg point for small grass.
                elif (Table[Sprite[Sp_now][Sp_row] +1][Sprite[Sp_now][Sp_col]] == chr_4):
                    Lnrg +=2  # Increase 2 nrg points for large grass.
                else:
                    pass
                Table[Sprite[Sp_now][Sp_row]][Sprite[Sp_now][Sp_col]] = chr_1
                Sprite[Sp_now][Sp_row] +=1
                Table[Sprite[Sp_now][Sp_row]][Sprite[Sp_now][Sp_col]] = chr_2
                # Move Lnrg -= 1; here to not reduce Life nrg when hitting the boundary.
            else:
                #print("\a", end="") # System bell (ASCII Control Chr 7)
                Con_bell()
        elif (Inputs[0:1] == "E") or (Inputs[0:1] == "e"):
            if (Sprite[Sp_now][Sp_col] + 1 != Col_end +1):
                if (Table[Sprite[Sp_now][Sp_row]][Sprite[Sp_now][Sp_col] +1] == chr_3):
                    Lnrg +=1  # Increase 1 nrg point for small grass.
                elif (Table[Sprite[Sp_now][Sp_row]][Sprite[Sp_now][Sp_col] +1] == chr_4):
                    Lnrg +=2  # Increase 2 nrg points for large grass.
                else:
                    pass
                Table[Sprite[Sp_now][Sp_row]][Sprite[Sp_now][Sp_col]] = chr_1
                Sprite[Sp_now][Sp_col] +=1
                Table[Sprite[Sp_now][Sp_row]][Sprite[Sp_now][Sp_col]] = chr_2
                # Move Lnrg -= 1; here to not reduce Life nrg when hitting the boundary.
            else:
                #print("\a", end="")  # System bell (ASCII Control Chr 7)
                Con_bell()
        elif (Inputs[0:1] == "W") or (Inputs[0:1] == "w"):
            if (Sprite[Sp_now][Sp_col] - 1 != Col_start -1):
                if (Table[Sprite[Sp_now][Sp_row]][Sprite[Sp_now][Sp_col] -1] == chr_3):
                    Lnrg +=1  # Increase 1 nrg point for small grass.
                elif (Table[Sprite[Sp_now][Sp_row]][Sprite[Sp_now][Sp_col] -1] == chr_4):
                    Lnrg +=2  # Increase 2 nrg points for large grass.
                else:
                    pass
                Table[Sprite[Sp_now][Sp_row]][Sprite[Sp_now][Sp_col]] = chr_1
                Sprite[Sp_now][Sp_col] -=1
                Table[Sprite[Sp_now][Sp_row]][Sprite[Sp_now][Sp_col]] = chr_2
                # Move Lnrg -= 1; here to not reduce Life nrg when hitting the boundary.
            else:
                #print("\a", end="")  # System bell (ASCII Control Chr 7)
                Con_bell()
        elif (Inputs[0:1] == "Q") or (Inputs[0:1] == "q"):
            break
        else:
            pass

    Con_Pause()
    return None
# END main <---

# --> START helper functions

# Prints updated array to screen.
def UpdateScreen(table):
    Con_Clear()
    Row_y = 0  # Counter for rows.
    Col_x = 0  # Counter for columns.

    # Print the array to the console.
    for Row_y in range(0, ROWS):  # Count through each row.
        for Col_x in range(0, COLUMNS):  # Count through each column.
            print(table[Row_y][Col_x], end="") #  No line breaks.
        print("")  # Print a line break to start next row.
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

# Console bell. May not work on all systems.
def Con_bell():
    import os
    if os.sys.stdin and os.sys.stdin.isatty():
        if os.isatty(os.sys.stdout.fileno()):
            # OS Command Line
            # Note! System bell may not work on all OSs.
            print("\a", end='')  # System ding (If enabled)
            return None
        else:
            pass  # REPL - Interactive Linux?
    else:
        pass  # REPL - Interactive Windows?
    return None

# Cross platform console clear.
# This function is in alpha and may not work as expected.
def Con_Clear():
    # The system() call allows the programmer to run OS command line batch commands.
    # It is discouraged as there are more appropriate C functions for most tasks.
    # I am only using it in this instance to avoid invoking additional OS API headers and code.
    import os
    if os.isatty(os.sys.stdout.fileno()):  # Clear function doesn't work in Py REPL
        # for windows
        if os.name == 'nt':
            os.system('cls')
        # for mac and linux
        elif os.name == 'posix':
            os.system('clear')
        else:
            return -1  # Unknown OS
    else:
        return None

# Console Pause wrapper.
def Con_Pause():
    dummy = ""
    dummy = input("Press [Enter] key to continue...")

    return None

if __name__ == '__main__':
    main()
