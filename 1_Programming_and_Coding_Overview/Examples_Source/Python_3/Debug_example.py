#------------------------------------------------------------------------------
# Name:        Debug_example.c
# Purpose:     Example
#
# Platform:    Win64, Ubuntu64
#
# Author:      Axle
# Created:     16/12/2021
# Updated:     18/02/2022
# Copyright:   (c) Axle 2021
# Licence:     MIT No Attribution
#------------------------------------------------------------------------------

# Print a table 5 rows x 5 columns
# with each column numbered 6 to 10
def main():
    ROWS = 5
    COLUMNS = 5
    Row_y = 0
    Col_x = 0
    MyArray = [[None]* COLUMNS for _ in range(ROWS)]

    for Row_y in range(0,  ROWS):
        for Col_x in range(0,  COLUMNS):
            MyArray[Row_y][Col_x] = Col_x +5
            DebugMsg("DEBUG_Col_x", str(Col_x))

    for Row_y in range(0,  ROWS):
        for Col_x in range(0,  COLUMNS):
            print(MyArray[Row_y][Col_x], end="")
        print("")

    input("Press [Enter] to exit.")
    return None

# A simple message box for debugging.
# Takes a String value only, so you will have to convert numbers to String.
def DebugMsg(aTitle, aMessage ):

    import sys
    #import os

    # for windows
    if sys.platform.startswith('win32'):
        import win32api
        reterr = 0
        # MessageBox[W] is for Unicode text, [A] is for ANSI text.
        ## Alternative
        #import ctypes
        #ctypes.windll.user32.MessageBoxW(0, aMessage, aTitle, 0)
        # To attach to the parent console window.
        #MessageBoxA(FindWindowA("ConsoleWindowClass", NULL),msg,title,0);
        reterr = win32api.MessageBox(0, aMessage, aTitle, 1)  # 65536 = MB_SETFOREGROUND
        if(reterr == 0):  # Holds the return value from the command line.
            # MessageBox() Fail
            return 0
        elif(reterr == 1):
            # IDOK
            return 1
        elif(reterr == 2):
            # IDCANCEL (Ctrl + C is dissabled when GUI messagebox is used)
            # So provide an option to break out of the debugging...
            # This will also exit now if the Close X is selected.
            sys.exit(0)  # Clean exit the application.
        else:
            return -1;

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
        Buffer = "xmessage -buttons OK:101,CANCEL:102 -center -title " + Buf_Msg + " \'.:|" + aMessage + "|:.\' >>/dev/null 2>>/dev/null"

        # Send it to the command line.
        reterr = os.system(Buffer)
        # This is the recommended linux way of getting the return from calling
        # a command line application. It has the overhead of including import os
        # under the Linux OS test...
        #if ( os.WIFEXITED(reterr))
            #print("The return value: " + os.WEXITSTATUS(reterr))
        #elif (os.WIFSIGNALED(reterr))
            #print("The program exited because of signal no: " + os.WTERMSIG(reterr))

        # As the return value is stored in the "High Byte" of the 16bit error return,
        # we can use some bitwise shift magic to extract the value of the 8 bit high
        # byte and eliminate the need for the import os library file.
        if(reterr>>8 == 101):
            # ID OK
            pass
        elif(reterr>>8 == 102):
            # ID CANCEL
            sys.exit(0)  # Quit debugging
        elif(reterr>>8 == 1):
            # ID CLOSE X
            pass
        elif(reterr>>8 == 0):
            # ID Default run OK
            pass
        else:  # xmessage failed or not exist.
            # Try Wayland compositor wlmessage.
            Buffer = "wlmessage \' |" + aMessage + "| \' >>/dev/null 2>>/dev/null"
            reterr = os.system(Buffer)
            if(reterr>>8 != 0) & (reterr>>8 != 1):
                # Popup message failed.
                #printf("%d\n", reterr);
                return -1

        return 0
    else:
        pass
        return -1  # Other OS
    return None

if __name__ == '__main__':
    main()