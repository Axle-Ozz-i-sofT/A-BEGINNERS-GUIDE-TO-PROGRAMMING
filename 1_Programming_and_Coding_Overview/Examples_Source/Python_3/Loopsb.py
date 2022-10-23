#-------------------------------------------------------------------------------
# Name:        Loopsb.py
# Purpose:     Loops Animations
#
# Platform:    REPL*, Win64, Ubuntu64
#
# Author:      Axle
# Created:     16/02/2022
# Updated:     19/02/2022
# Copyright:   (c) Axle 2022
# Licence:     MIT No Attribution
#-------------------------------------------------------------------------------
# * This example is best run in a native OS console window.
#-------------------------------------------------------------------------------

def main():

    if 1 == Con_IsREPL():
        print("This application is best viewed running from the OS Command interpreter")
        Con_Pause()

    # Loading screen animation one.
    BarRotate = "|\-/"
    Notice = "Please wait..."
    RotSpeed = 0.5  # This is the sleep timer in seconds. Lower setting = faster.
    counts = 15  # The number of scenes.
    x = 0  # loop counters
    y = 0  # loop counters
    z = 0  # loop counters

    print(Notice)
    # Keep looping through the 4 characters in BarRotate.
    for x in range(0, counts):
        if(y > 3): # At the forth (0,1,2,3) character '/' restart from 0
            y = 0  # reset to 0 and restart character enumeration.
        # Python will hold the print statements until a new line is encountered.
        # Use flush to push the print character to the console stdout.
        print(BarRotate[y], end='', flush=True)
        Con_sleep(RotSpeed)  # Sleep(pause) for a moment to slow the animation.
        y+= 1
        # I was using backspace '\b', but it doesn't work on all consoles or REPL.
        print("\r", end="", flush=True)  # '\r' = Carriage return to overwrite the last character.

    Con_Clear()  # Clear the console stdout screen.
    Con_sleep(0.5)  # Just a pause to reduce screen/buffer flicker.

    # Loading screen animation two.
    # Create the list containing the 4 strings.
    # Special characters must be preceded by an escape inside of strings '\'
    BigRotate2 = [
    "\\  \n"
    " \\ \n"
    "  \\",
    " | \n"
    " | \n"
    " | ",
    "  /\n"
    " / \n"
    "/  ",
    "   \n"
    "---\n"
    "   "]

    while(z < counts):  # Repeat counts times.
        for x in range(0, 4):  # loop through all 4 list elements.
            print(Notice)
            print(BigRotate2[x], end="", flush=True)
            Con_sleep(RotSpeed/2)  # Slow the animation down.
            Con_Clear()
        z+= 4
    print("")  # Reset caret (Cursor) CRLF for next print statements.

    Con_Clear()  # Clear the screen for the next animation.
    Con_sleep(0.5)  # Just a pause to reduce screen/buffer flicker.

    # Loading screen animation 3.
    Tracer = ""  # Initiate all to 0.
    for x in range(0, 21):  # Populate with space, except for string terminator[22]
        Tracer+= " "

    t = 0  # loop counters
    z = 0  # Reset loop counters
    while(z < (abs(counts/4))):  # abs(n/5) to reduce the repeats.
        while(t < 20):  # Loop forward...
            t += 1  # Start at 0 +1 to allow room for the tail.
            # !!! It may be more efficient to run this from a list rather
            # than rebuild the string "Tracer" 3 times in each loop.
            Tracer = Tracer[:t] + "#" + Tracer[t+1:]  # Rebuild the string with
            Tracer = Tracer[:t-1] + ":" + Tracer[t:]  # the new characters in place.
            print(Notice)
            print(Tracer, end="", flush=True)  # Print full array/string to the screen.
            Tracer = Tracer[:t-1] + " " + Tracer[t:]  # clear a character from the string.
            Con_sleep(RotSpeed/10)  # slow the animation down a bit
            if 1 == Con_IsREPL():  # REPL vs OS Console.
                print("")
            Con_Clear()  # Clear the screen for the next print.
        while(t > 0):  # Loop backwards...
            t -= 1  # Start at -1 to allow room for the tail at t.
            Tracer = Tracer[:t] + "#" + Tracer[t+1:]
            Tracer = Tracer[:t+1] + ":" + Tracer[t+2:]  # t+1 is the right side of '#'.
            print(Notice)
            print(Tracer, end="", flush=True)
            Tracer = Tracer[:t+1] + " " + Tracer[t+2:]
            Con_sleep(RotSpeed/10)
            if 1 == Con_IsREPL():
                print("")
            Con_Clear()
        z+= 1
    print("")  # Reset caret (Cursor) CRLF for next print statements.

    Con_Clear()  # Clear the screen for the next animation.
    Con_sleep(0.5)  # Just a pause to reduce screen/buffer flicker.

    # Stick-man animation
    # Define our data arrays...
    # This is the original ASCII text file from the net.
    # The arrays were created by hand in Notepad++ on this occasion.
     # o   \ o /  _ o         __|    \ /     |__        o _  \ o /   o
    # /|\    |     /\   ___\o   \o    |    o/    o/__   /\     |    /|\
    # / \   / \   | \  /)  |    ( \  /o\  / )    |  (\  / |   / \   / \

    Len_Cart = 11  # Keep a record of the list length.
    # Note: We must use an escape character for special characters \\ = '\'.
    Cart2 = [
    "   o           \n"
    "  /|\\          \n"
    "  / \\          ",
    " \\ o /         \n"
    "   |           \n"
    "  / \\          ",
    "  _ o          \n"
    "   /\\          \n"
    "  | \\          ",
    "               \n"
    "   ___\\o       \n"
    "  |)  |        ",
    "    __|        \n"
    "      \\o       \n"
    "      ( \\      ",
    "      \\ /      \n"
    "       |       \n"
    "      /o\\      ",
    "         |__   \n"
    "       o/      \n"
    "      / )      ",
    "               \n"
    "        o/__   \n"
    "        |  (\\  ",
    "           o _ \n"
    "           /\\  \n"
    "           / | ",
    "          \\ o /\n"
    "            |  \n"
    "           / \\ ",
    "            o  \n"
    "           /|\\ \n"
    "           / \\ "]

    a = 0  # loop counters
    b = 0  # loop counters
    repeat = 3  # Repeat the animation for loops 3 times.
    Speed = 0.3  # Delay in seconds to slow the animation down.

    while(a < repeat):  # Repeat the animation n times.
        for b in range(0, Len_Cart):  # Loop animation forward.
            Con_Clear()  # Clear the console ready for the next print.
            print(Cart2[b])  # Print list element [n]
            Con_sleep(Speed)  # Replace with cross platform wrapper.
        Con_sleep(Speed)  # slow the animation down a bit.
        for b in range(Len_Cart-1, -1, -1):  # Loop animation backwards.
            Con_Clear()  # Clear the console ready for the next print.
            print(Cart2[b])  # Print list element [n]
            Con_sleep(Speed)  # slow the animation down a bit.
        a+= 1
        Con_sleep(Speed)  # To reduce screen flicker (Visual effect)

    Con_Pause()  # DEBUG Pause
    return None
    # END Main() <---


# --> START helper functions

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

# Cross platform console clear.
# This function is in alpha and may not work as expected.
def Con_Clear():
    # The system() call allows the programmer to run OS command line batch commands.
    # It is discouraged as there are more appropriate C functions for most tasks.
    # I am only using it in this instance to avoid invoking additional OS API headers and code.
    import os
    if os.sys.stdin and os.sys.stdin.isatty():
        if os.isatty(os.sys.stdout.fileno()):# Clear function doesn't work in Py REPL
            # for windows
            if os.name == 'nt':
                os.system('cls')
            # for mac and linux
            elif os.name == 'posix':
                os.system('clear')
            else:
                return None  # Other OS
        else:
            return None  # REPL - Interactive Linux?
    else:
    return None  # REPL - Interactive Windows?

# Console Pause wrapper.
def Con_Pause():
    dummy = ""
    dummy = input("Press [Enter] key to continue...")
    return None

# Console sleep
def Con_sleep(times: float):
    import time
    time.sleep(times)
    return None

if __name__ == '__main__':
    main()
