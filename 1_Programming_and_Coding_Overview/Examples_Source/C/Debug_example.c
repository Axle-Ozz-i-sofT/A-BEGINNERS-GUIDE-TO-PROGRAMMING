//-------------------------------------------------------------------------------
// Name:        Debug_example.c
// Purpose:     Example
//
// Platform:    Win64, Ubuntu64
//
// Author:      Axle
// Created:     15/02/2022
// Updated:     18/02/2022
// Copyright:   (c) Axle 2022
// Licence:     MIT No Attribution
//-------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>

// Platform specific headers.
// Test if Windows or Linux OS
#ifdef _WIN32
#include <Windows.h>
#define OS_Windows 1  // 1 = True (aka Bool)
#define OS_Unix 0
#elif __linux__ // __unix__
#include <unistd.h>
#define OS_Unix 1
#define OS_Windows 0  // 0 = False (aka Bool)
#else
#error "OS Not Supported!"
#include <stophere>
#endif

int DebugMsg(char *aTitle, char *aMessage);
// Print a table 5 rows x 5 columns
// with each column numbered 6 to 10
int main(void)
    {
    int ROWS = 5;
    int COLUMNS = 5;
    int Row_y = 0;
    int Col_x = 0;
    static int MyArray[5][5];

    for (Row_y = 0; Row_y < ROWS; Row_y++)
        {
        for (Col_x = 0; Col_x < COLUMNS; Col_x++)
            {
            MyArray[Row_y][Col_x] = Col_x +5;
            // DEBUG Lines START -->
            // C Is a low level language so we have to know the type and convert
            // it to a string. In this case we are converting an int to a
            // char array aka String. "%d" is for int, "%f" is for float.
            char str_buffer[128] = {'\0'};  // Max string/num length 128 chars.
            sprintf(str_buffer, "%d", Col_x);  // Convert int Number to String.
            DebugMsg("DEBUG Col_x", str_buffer);  // send converted number.
            // DEBUG Lines END <--
            }
        }

    for (Row_y = 0; Row_y < ROWS; Row_y++)
        {
        for (Col_x = 0; Col_x < COLUMNS; Col_x++)
            {
            printf("%d",MyArray[Row_y][Col_x]);
            }
        printf("\n");
        }
    return 0;
    }


// A simple message box for debugging.
// Takes a String value only, so you will have to convert numbers to String.
int DebugMsg(char *aTitle, char *aMessage)
    {
    // Require a temporary buffer to hold converted string.
    // Number conversions. Convert Int, Float, bin, hex to ASCII String.
    //
    // int sprintf(char *str, const char *format, ...)
    // NOTE! sprintf uses the same print formatting as printf...
    // Search "C library function - sprintf()" for the full description.
    //
    // int iVariable = 238;
    //char Buffer[128] = {'\0'};
    //sprintf(Buffer, "%d", iVariable) // "%f" float|double float.
    //DebugMsg("DEBUGmsg", Buffer);
    int reterr = 0;  // Holds the return value from the command line.

    if(OS_Windows)
        {
        // Requires:winuser.h (#include <Windows.h>),User32.dll
        // Not attached to parent console window.
        // https://docs.microsoft.com/en-us/windows/win32/api/winuser
        // /nf-winuser-messageboxa
        // May throw a compiler warning... MessageBoxA Not found.
        reterr = MessageBoxA(0, aMessage, aTitle, 1);  // 65536, MB_SETFOREGROUND
        if(reterr == 0)  // Holds the return value from the command line.
            {
            // MessageBox() Fail
            return 0;
            }
        else if(reterr == 1)
            {
            // IDOK
            return 1;
            }
        else if(reterr == 2)
            {
            // IDCANCEL (Ctrl + C is dissabled when GUI messagebox is used)
            // So provide an option tt break out of the debugging...
            // This will also exit now if the Close X is selected.
            exit(0);  // Clean exit the application.
            }
        else
            {
            return -1;
            }
        // To attach to the parent console window.
        //MessageBoxA(FindWindowA("ConsoleWindowClass", NULL),msg,title,0);
        }
    else if(OS_Unix)
        {
        // http://manpages.ubuntu.com/manpages/trusty/man1/xmessage.1.html
        // apt-get install x11-utils
        //system("xmessage -center 'Hello, World!'");
        // Else try wayland
        // https://github.com/Tarnyko/wlmessage
        //system("wlmessage 'Hello, World!'");
        char Buffer[128] = {'\0'};
        char Buf_Msg[128] = {'\0'};
        // Place title text in 'apostrophe'.
        strcpy(Buf_Msg, "\'");
        strcat(Buf_Msg, aTitle);
        strcat(Buf_Msg, "\'");

        // Build our command line statement.
        // xmessage [-options] [message ...]
        strcpy(Buffer, "xmessage -buttons OK:101,CANCEL:102 -center -title ");
        strcat(Buffer, Buf_Msg);
        strcat(Buffer, " \'.:|" );
        strcat(Buffer, aMessage );
        // NOTE! ">>/dev/null 2>>/dev/null" suppresses the console output.
        strcat(Buffer, "|:.\' >>/dev/null 2>>/dev/null" );
        // Send it to the command line.
        reterr = system(Buffer);
        /*
                // This is the recommended linux way of getting the return from calling
                // a command line application. It has the overhead of including wait.h
                // under the Linux OS test...
                //#elif __linux__
                //#include <wait.h>

                if ( WIFEXITED(reterr))
                {
                printf("The return value: %d\n", WEXITSTATUS(reterr));
                }
                else if (WIFSIGNALED(reterr))
                {
                printf("The program exited because of signal (signal no:%d)\n", WTERMSIG(reterr));
                }
        */
        // As the return value is stored in the "High Byte" of the 16bit error return,
        // we can use some bitwise shift magic to extract the value of the 8 bit high
        // byte and eliminate the need for the wait.h library file.
        if(reterr>>8 == 101)
            {
            // ID OK
            //printf("reterr 101 = %d\n", reterr>>8);
            }
        else if(reterr>>8 == 102)
            {
            // ID CANCEL
            //printf("reterr 102 = %d\n", reterr>>8);
            exit(0);  // Clean exit the application.
            }
        else if(reterr>>8 == 1)
            {
            // ID CLOSE X
            //printf("reterr 1 = %d\n", reterr>>8);
            }
        else if(reterr>>8 == 0)
            {
            // ID Default run OK
            //printf("reterr Default = %d\n", reterr>>8);
            }
        else
            {
            // FAIL! Try Wayland...
            // I have left the Wayland version as a default message as
            // few systems are using wayland at this time. To add the cancel
            // routines just follow the -buttons OK:010,CANCEL:102 and error
            // tests as per xmessage.
            // Try Wayland compositor wlmessage.
            strcpy(Buffer, "wlmessage \' |" );
            strcat(Buffer, aMessage );
            strcat(Buffer, "| \' >>/dev/null 2>>/dev/null" );
            reterr = system(Buffer);
            if(reterr != 0) && (reterr != 1)
                {
                // Popup message failed.
                //printf("%d\n", reterr>>8);
                return -1;
                }
            }
        // All above If tests will default to this return 0; upon success.
        return 0;
        }
    else
    {
        // OS Unknown
    }

    return 0;
    }


