#-------------------------------------------------------------------------------
# Name:        Functions.py
# Purpose:     Examples
#
# Platform:    REPL*, Win64, Ubuntu64
#
# Author:      Axle
# Created:     22/02/2022
# Updated:
# Copyright:   (c) Axle 2022
# Licence:     MIT No Attribution
#-------------------------------------------------------------------------------
# * This example is best run in a native OS console window.
#-------------------------------------------------------------------------------


def main():

    Menu_Routine()


    Con_Pause()  # DEBUG Pause
    return None
    # END Main() <---

# --> START Application Functions

def Menu_Routine():
    while(True):
        # Forever loop. Needs a break, return, or exit() statement to
        # exit the loop.
        Con_Clear()
        # Placing the menu inside of the while loop "Refreshes" the menu
        # if an incorrect value is supplied.

        option = ""  # Menu variable.

        print("==============================================")
        print("  MAIN MENU")
        print("==============================================")
        print("  1 - Stick Animations")
        print("  2 - File Write Example")
        print("  3 - File Read Example 1")
        print("  4 - File Read Example 2")
        print("  5 - Debug Msg Box")
        print("  6 - System Sound Test")
        print("  7 - Mandelbrot Fractals")
        print("  8 - Show time and date")
        print("")
        print("  9 - Exit The Application")
        print("-------------------------------------------------")
        options = input("  Enter your menu choice: ")
        print("")

        # Check what choice was entered and act accordingly.
        # We can add as many choices as needed.
        if options == '0':
            pass# Ignore false [Enter]s
        elif options == '1':
            Stick_Animations()
        elif options == '2':
            File_Write_Example()
        elif options == '3':
            File_Read_Example1()
        elif options == '4':
            File_Read_Example2()
        elif options == '5':
            # Send String. "Hello"
            aVariable = "This is my message."
            DebugMsg("DEBUGmsg", aVariable);
            # Send  Int 125
            #iVariable = 125
            #DebugMsg("DEBUGmsg", str(iVariable))
        elif options == '6':
            Sys_Sound()
        elif options == '7':
            Mandelbrot_Fractals_Console_ASCII()
        elif options == '8':
            Show_Time_Date()
            Con_Pause()  # wait until a key is pressed
            Con_Clear()
        elif options == '9':
            print("Exiting the application...")
            Con_sleep(1)  # Allow 1 second for the Exit notice to display.
            break
        else:
            Sys_Sound()
            print("Invalid option.\nPlease enter a number from the Menu Options.\n")
            Con_sleep(1)

    return None

# --> START Application Functions
def Stick_Animations():
    # Stick man animation
    Len_Animation = 11  # Keep a record of the list length.
    Animation = [
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

    a = 0
    b = 0
    repeat = 3
    Speed = 0.3

    while(a < repeat):
        for b in range(0, Len_Animation):  # Loop animation forward.
            Con_Clear()
            print(Animation[b])
            Con_sleep(Speed)
        Con_sleep(Speed)
        for b in range(Len_Animation-1, -1, -1):  # Loop animation backwards.
            Con_Clear()
            print(Animation[b])
            Con_sleep(Speed)
        a+= 1
        Con_sleep(Speed)
    Con_Clear()
    return None

def File_Write_Example():

    filename = "MyTextFile.txt"
    # Output file to write to.
    # ==> Open Output file for text append ops
    string_Temp_Buffer = ""

    with open(filename, "a") as outfile:
        print("Please enter the text you would like to write to file " + filename)
        print("Type a line of text ... followed by [Enter]")
        string_Temp_Buffer = input()

        # append a new line character \n
        string_Temp_Buffer = string_Temp_Buffer + '\n'
        # Write/append 'a' string_Temp_Buffer to file.
        outfile.write(string_Temp_Buffer)

    print("\nFile write completed...")
    Con_Pause()  # wait until a key is pressed
    Con_Clear()
    return None

# A simple routine that reads a files contents directly to the screen
# using a single buffer.
def File_Read_Example1():
    filename = "MyTextFile.txt"  # Input file.
    string_Temp_Buffer = ""

    # It is possible that the file may not yet exist. Opening it
    # as "r" will return an exception. Let's test if the file exists first.
    try:
        with open(filename, "r") as infile:  # Open the CSV File.
            for string_Temp_Buffer in infile:
                # Strip the newline character from the line.
                string_Temp_Buffer = string_Temp_Buffer.strip('\n')
                print(string_Temp_Buffer)

        print("")
        Con_Pause()  # wait until a key is pressed
        Con_Clear()
        return None

    except FileNotFoundError:  # If the file does not yet exist.
        print("\nERROR! Cannot open Output file " + filename)
        print("Maybe the file has not yet been created.")
        print("Please select from the MAIN Menu")
        print("to create a new file.")
        Con_Pause()
        return None


def File_Read_Example2():

    filename = "MyTextFile.txt"
    # main Data List.
    Read_Buffer = []

    # It is possible that the file may not yet exist. Opening it
    # as "r" will return an exception. Let's test if the file exists first.
    try:
        with open(filename, "r") as file:  # Open the CSV File.
            # Walk each line from the file.
            for buffer in file:
                # Strip the newline character from the line.
                buffer = buffer.strip('\n')
                # Append each line to the list.
                Read_Buffer.append(buffer)

        # Walk through the List and print each line as text.
        for cnt1 in range(len(Read_Buffer)):
            # ' Print the lines from the array.
            print(Read_Buffer[cnt1])

        print("")
        Con_Pause()  # wait until a key is pressed
        Con_Clear()
        return None

    except FileNotFoundError:  # If the file does not yet exist.
        print("The CSV file has not yet been created.")
        print("Please select Option 1 from the MAIN Menu")
        print("to start the data entry.")
        input("Press [Enter] to return to the MAIN MENU...")
        return None


def Mandelbrot_Fractals_Console_ASCII():

    # Credits:
    # https://cs.nyu.edu/~perlin/
    # Ken Perlin
    # Professor of Computer Science
    # NYU Future Reality Lab F
    # >
    # Original Source:
    # main(k){float i,j,r,x,y=-16;while(puts(""),y++<15)for(x
    # =0;x++<84;putchar(" .:-;!/>)|&IH%*#"[k&15]))for(i=k=r=0;
    # j=r*r-i*i-2+x/25,i=2*r*i+y/10,j*j+i*i<11&&k++<111;r=j);}
    # >
    # Info on Mandelbrot sets
    # https://mathworld.wolfram.com/MandelbrotSet.html

    # Although this describes a series of planes in 3D layers, the calculations
    # are graphed to a 2D plane and use colour depth to describe points in the
    # lower layers (planes).

    Con_Clear()
    k = 1  # First print character; default = 1 (0 to leave blank).
    colours = " .:-;!/>)|&IH%*#"  # 16 colours

    i = 0.0
    j = 0.0
    r = 0.0
    x = 0.0
    y = -16

    # zoom_x, zoom_y are relative and both must be changed as a percentage.
    zoom_x = 25.00  # Default = 25,+zoom-In/-zoom-Out
    zoom_y = 10.00  # Default = 10,+zoom-In/-zoom-Out

    offset_x = -2.00  # Default = -2.00, -pan-L/+pan-R
    offset_y = 0.00  # Default = 0.00, -pan-U/+pan-D

    while(y < 15):  # Loop #1
        y+= 1
        print("")  # Line break '\n'.

        for x in range(0, 88 -1):  # Loop #2, (<84 == the screen print width.)
            # Select colour level (Bitwise AND) from 16 colours, then print.
            print (colours[(k & 15)], end= "")

            i=0
            k=0
            r=0
            while(1):  # Loop #3
                # Calculate x fractal.
                j = ((r*r) - (i*i) + ((x/zoom_x) + offset_x))
                # Calculate y fractal.
                i = ((2*r*i) + ((y/zoom_y) + offset_y))

                # Test for x,y divergence to infinity (lemniscates).
                # In a sense this relates to the period between depth layers
                # and the scale at which they diverge to infinity.
                # The default values offer the most visually appealing balance,
                # meaning they are easier for our brain to interpret.
                if(j*j+i*i > 11):  # Default = 11
                    break
                # Test depth level (Colour).
                k+= 1
                if(k > 111):  # Default = 111.
                    break

                r=j  # Start next calculation from current fractal.
            # End Loop #3
        # End Loop #2
    # End Loop #1
    Con_Pause()
    Con_Clear()
    return None


# --> START helper functions

# A Popup Message Box
def DebugMsg(aTitle, aMessage ):

    import sys

    # for windows
    if sys.platform.startswith('win32'):
        import win32api
        win32api.MessageBox(0, aMessage, aTitle, 0)  # 65536 = MB_SETFOREGROUND
        # MessageBox[W] is for Unicode text, [A] is for ANSI text.
        ## Alternative
        #import ctypes
        #ctypes.windll.user32.MessageBoxW(0, aMessage, aTitle, 0)
        return 0
    # for linux
    elif sys.platform.startswith('linux'):
        import os
        # http://manpages.ubuntu.com/manpages/trusty/man1/xmessage.1.html
        # apt-get install x11-utils
        #system("xmessage -center 'Hello, World!'");
        # Else try wayland
        # https://github.com/Tarnyko/wlmessage
        #system("wlmessage 'Hello, World!'");
        reterr = 0
        Buffer = ""
        Buf_Msg = ""
        # Place title text in 'apostrophe'.
        Buf_Msg = "\'" + aTitle + "\'"

        # Build our command line statement.
        # xmessage [-options] [message ...]
        # NOTE! ">>/dev/null 2>>/dev/null" suppresses the console output.
        Buffer = "xmessage -center -title " + Buf_Msg + " \'.:|" + aMessage + "|:.\' >>/dev/null 2>>/dev/null"

        # Send it to the command line.
        reterr = os.system(Buffer)
        if(reterr != 0) & (reterr != 1):  # xmessage failed or not exist.
            # Try Wayland compositor wlmessage.
            Buffer = "wlmessage \' |" + aMessage + "| \' >>/dev/null 2>>/dev/null"
            reterr = os.system(Buffer)
            if(reterr != 0) & (reterr != 1):
                # Popup message failed.
                #printf("%d\n", reterr);
                return -1

        return 0
    else:
        pass
        return -1  # Other OS
    return 0

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
    # It is discouraged as there are more appropriate functions for most tasks.
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
    return None

def Sys_Sound():
    # The system() call allows the programmer to run OS command line batch
    # commands. It is discouraged as there are more appropriate functions
    # for most tasks. I am only using it in this instance to avoid invoking
    # additional OS API headers and code.
    import os
    if os.sys.stdin and os.sys.stdin.isatty():
        if os.isatty(os.sys.stdout.fileno()):
            # for windows
            if os.name == 'nt':
                os.system("rundll32 user32.dll,MessageBeep")
            # for mac and linux
            elif os.name == 'posix':
                os.system("paplay /usr/share/sounds/ubuntu/notifications/Blip.ogg")
                ## Rhodes.ogg,Slick.ogg,'Soft delay.ogg',Xylo.ogg
            else:  # Other OS
                print("\a", end="")
        else:  # REPL - Interactive Linux?
            os.system("paplay /usr/share/sounds/ubuntu/notifications/Blip.ogg")
    else:  # REPL - Interactive Windows?
        os.system("rundll32 user32.dll,MessageBeep")
    return None

# Console Pause wrapper.
def Con_Pause():
    dummy = ""
    print("")
    dummy = input("Press [Enter] key to continue...")
    return None

# Console sleep
def Con_sleep(times: float):
    import time
    time.sleep(times)
    return None

# Wrapper for the 2 functions time/date.
# We can can create convenience wrapper functions to "wrap" a set of more
# complex tasks in a single function call.
# This is helpful if it is a common set of tasks that are called regularly
# throughout an application.
def Show_Time_Date():
    show_time()
    print(" - ", end="")
    show_date()
    print("")
    return 0

# Display current system time.
def show_time():
    import time
    t = time.localtime()
    current_time = time.strftime("%H:%M:%S", t)
    print(current_time, end="")
    return None

# Display current system date.
def show_date():
    import time
    d = time.localtime()
    current_date = time.strftime("%Y/%m/%d", d)
    print(current_date, end="")
    return None

if __name__ == '__main__':
    main()
