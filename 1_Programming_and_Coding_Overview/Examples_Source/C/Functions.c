//------------------------------------------------------------------------------
// Name:        Functions.c
// Purpose:     Examples
//
// Platform:    Win64, Ubuntu64
//
// Author:      Axle
// Created:     21/02/2022
// Updated:     22/02/2022
// Copyright:   (c) Axle 2022
// Licence:     MIT No Attribution
//------------------------------------------------------------------------------


// C std library headers.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Platform specific headers.
// Test if Windows or Unix OS
#ifdef _WIN32
#   define WIN32_LEAN_AND_MEAN
#   include <Windows.h>
#   define OS_Windows 1  // 1 = True (aka Bool)
#   define OS_Unix 0
#   ifdef _WIN64
#       define __USE_MINGW_ANSI_STDIO 1
#   endif
#endif

#ifdef __unix__ // _linux__
#include <unistd.h>
#define OS_Unix 1
#define OS_Windows 0  // 0 = False (aka Bool)
// Unix requires a complex struct in #def to implement a millisecond sleep
// that is comparable to Win-API Sleep() so I have just changed it to a
// 1 second sleep using Unix sleep(). 1 second is the minimum sleep time
// available for Unix sleep(), so it will be a bit slow on Linux.
// The following line replaces all occurrences of Sleep() with sleep().
// Sleep is a Windows function, and sleep lowers case s is a Unix function.
#define Sleep(x) sleep(1)
#endif

// Turn off compiler warnings for unused variables between (Windows/Linux etc.)
#define unused(x) (x) = (x)


// Define extra functions so we can place them at the bottom of the page.
int Menu_Routine(void);
int Stick_Animation(void);
int File_Write_Example(void);
int File_Read_Example1(void);
int File_Read_Example2(void);
int Mandelbrot_Fractals_Console_ASCII(void);


int Sys_Sound(void);
int DebugMsg(char *aTitle, char *aMessage);
int Con_Clear(void);
void S_Pause(void);
int S_getchar(void);
// v Emulates Input function in FreeBASIC and Python 3.
char *Input(char *buf, char *str, int n);
char *S_fgets(char *buf, int n, FILE *stream);
int Show_Time_Date(void);
int show_time(void);
int show_date(void);

int main(int argc, char *argv[])  // Main procedure
    {
    unused(argc);  // Turns off the compiler warning for unused argc, argv
    unused(argv);  // Turns off the compiler warning for unused argc, argv

    Menu_Routine();

    S_Pause();  // DEBUG Pause
    return 0;
    }  // END main() <---


// --> START Application Functions
int Menu_Routine(void)
    {
    while(1)
        {
        // Forever loop. Needs a break, return, or exit() statement to
        // exit the loop.
        Con_Clear();
        // Placing the menu inside of the while loop "Refreshes" the menu
        // if an incorrect value is supplied.
        char inputbuffer[4] = {'\0'};  // Inputs are as string.
        int option = 0;  // Menu variable.

        printf("==============================================\n");
        printf("  MAIN MENU\n");
        printf("==============================================\n");
        printf("  1 - Stick Animation\n");
        printf("  2 - File Write Example\n");
        printf("  3 - File Read Example 1\n");
        printf("  4 - File Read Example 2\n");
        printf("  5 - Debug Msg Box\n");
        printf("  6 - System Sound Test\n");
        printf("  7 - Mandelbrot Fractals\n");
        printf("  8 - Display time and date\n");
        printf("\n");
        printf("  9 - Exit The Application\n");
        printf("----------------------------------------------\n");
        //printf("Enter your menu choice: ");
        // atoi() Convert char/Str to an integer.
        option = atoi(Input(inputbuffer, "  Enter your menu choice: ", 2));
        printf("\n");

        // Check what choice was entered and act accordingly
        // We can add as many choices as needed
        if(option == 0)
            {
            // Ignore false [Enter]s
            }
        else if(option == 1)
            {
            Stick_Animation();
            }
        else if(option == 2)
            {
            File_Write_Example();
            }
        else if(option == 3)
            {
            File_Read_Example1();
            }
        else if(option == 4)
            {
            File_Read_Example2();
            }
        else if(option == 5)
            {
            // Send String. "Hello"
            char aVariable[] = "This is my message.";
            DebugMsg("DEBUGmsg", aVariable);
            // Send  Int 125
            //int iVariable = 125;
            //char Buffer[128] = {'\0'};
            //DebugMsg("DEBUGmsg", itoa(iVariable, Buffer, 10));
            // Alternative conversion atoi(), ftoa(), sprintf().
            }
        else if(option == 6)
            {
            Sys_Sound();
            }
        else if(option == 7)
            {
            Mandelbrot_Fractals_Console_ASCII();
            }
        else if(option == 8)
            {
            Show_Time_Date();
            S_Pause();
            Con_Clear();
            }
        else if(option == 9)
            {
            printf("Exiting the application...\n");
            Sleep(1000);  // Allow 1 second for the Exit notice to display.
            break;
            }
        else
            {
            Sys_Sound();
            printf("Invalid option.\nPlease enter a number from the Menu Options.\n\n");
            Sleep(1000);
            }
        }

    return 0;
    }

int Stick_Animation(void)
    {
    // Stick man animation
    int Len_Animation = 11;  // Keep a record of the array length.
    static char Animation[11][60] =
        {
            {
            "   o           \n"
            "  /|\\          \n"
            "  / \\          \n\0"
            },

            {
            " \\ o /         \n"
            "   |           \n"
            "  / \\          \n\0"
            },

            {
            "  _ o          \n"
            "   /\\          \n"
            "  | \\          \n\0"
            },

            {
            "               \n"
            "   ___\\o       \n"
            "  |)  |        \n\0"
            },

            {
            "    __|        \n"
            "      \\o       \n"
            "      ( \\      \n\0"
            },

            {
            "      \\ /      \n"
            "       |       \n"
            "      /o\\      \n\0"
            },

            {
            "         |__   \n"
            "       o/      \n"
            "      / )      \n\0"
            },

            {
            "               \n"
            "        o/__   \n"
            "        |  (\\  \n\0"
            },

            {
            "           o _ \n"
            "           /\\  \n"
            "           / | \n\0"
            },

            {
            "          \\ o /\n"
            "            |  \n"
            "           / \\ \n\0"
            },

            {
            "            o  \n"
            "           /|\\ \n"
            "           / \\ \n\0"
            }
        };

    int a = 0;
    int b = 0;
    int Speed = 400;  // Delay in milliseconds to slow the animation down.
    unused(Speed);  // Turns off the compiler warning when in Linux.

    // Adjust different Sleep function between Windows Unix.
    int C_repeat = 0;
    if(1 == OS_Unix)  // Linux
        {
        C_repeat = 2;  // For slow Linux sleep(seconds)
        }
    else  // Windows
        {
        C_repeat = 3;
        }

    // Display the animation.
    while(a < C_repeat)
        {
        for(b = 0; b < Len_Animation; b++)  // Loop animation forward
            {
            Con_Clear();
            printf("%s", Animation[b]);
            fflush(stdout);
            Sleep(Speed);
            }
        for(b = Len_Animation -1; b >= 0; b--)  // Loop animation backwards
            {
            Con_Clear();
            printf("%s", Animation[b]);
            fflush(stdout);
            Sleep(Speed);
            }
        a++;
        }
    Con_Clear();
    return 0;
    }

int File_Write_Example(void)
    {
    FILE * fpFileOut;  // File open handle
    // Output file to write to.
    char filename[] = "MyTextFile.txt";

    const int Max_str_Length = 128;
    char string_Temp_Buffer[128] = {'\0'};
    printf("Please enter the text you would like to write to file %s\n", filename);
    Input(string_Temp_Buffer, "Type a line of text ... followed by [Enter]\n", Max_str_Length);

    //==> Open Output file for text append ops.
    // "a+" will create a new file '+' if it does not exist.
    fpFileOut = fopen(filename, "a+");
    if(fpFileOut == NULL)  // (!fpFileOut) alt. Test if file open success.
        {
        printf("ERROR! Cannot open Output file %s\n", filename);
        S_Pause();
        return -1;
        }

    fprintf(fpFileOut, "%s\n", string_Temp_Buffer);  // write the buffer to file.

    // we must always remember to close the file when finished.
    fclose(fpFileOut);

    printf("\nFile write completed...\n");
    S_Pause();
    Con_Clear();
    return 0;
    }

// A simple routine that reads a files contents directly to the screen
// using a single buffers.
int File_Read_Example1(void)
    {
    FILE *fpFileIn;  // File open handle
    char filename[] = "MyTextFile.txt";  // Output file.
    char buffer[128] = {'\0'};
    int len_buffer = 128 -4;  // allow space for '\r','\n','\0'
    int cnt1;  // Loop counters.

    fpFileIn = fopen(filename, "r");  // Open file for read ops
    if(fpFileIn == NULL)  //(!fpFileIn) alt. Test if file open success.
        {
        printf("Error in opening Data file : %s\n", filename);
        printf("Maybe the file has not yet been created.\n");
        printf("Please select from the MAIN Menu\n");
        printf("to create a new file.\n");
        S_Pause();
        return 0;
        }

    while(fgets(buffer, len_buffer, fpFileIn) != NULL)
        {
        // Walk each line from the file (returns the string
        // with '\n' at the end).
        // Strip the newline character from the line and
        // replace newline char '\n' '\r' with '\0'.
        //string_Temp_Buffer[strcspn(string_Temp_Buffer, "\r\n")] = '\0';
        // Copy the cleaned line of text into our array.
        printf("%s", buffer);
        cnt1++;  // move to the next line and repeat.
        }

    // It is important to free up resources as soon as they are no longer required.
    fclose(fpFileIn);  // finished file reads, close the file.

    S_Pause();
    Con_Clear();
    return 0;
    }

// File read example that copies the file contents into a "Dynamic" array so
// that it can be manipulated. Dynamic arrays are created on the heap.
int File_Read_Example2(void)
    {
    FILE *fpFileIn;  // File open handle
    char filename[] = "MyTextFile.txt";

    // In C we also have to allocate enough char space to hold the string values
    // to be stored. Because we don't know in advance how large the text file
    // will be, we also have to create a dynamic array in memory "On the heap"
    // at run time.
    // An alternative method is to read the contents from the file directly to
    // the screen without the need to test lengths or create a buffer, but I
    // wanted to show how to actually store the data in the application so that
    // it can be manipulated after being read. You could then write the modified
    // text back to the file or to another file; for example "File Copy".

    int Char_Buffer;  // Buffer to hold each character.
    int Total_Lines = 0;  // = Total lines in the text file.
    int Line_Width = 0;  // Count line widths.
    int Max_Width = 0;  // Total width of the longest line.
    int cnt1, cnt2;  // Loop counters.

    // It is possible that the file may not yet exist. Opening it
    // as "r" will return an exception. Let's test if the file exists first.
    fpFileIn = fopen(filename, "r");  // Open file for read ops
    if(fpFileIn == NULL)  //(!fpFileIn) alt. Test if file open success.
        {
        printf("Error in opening Data file : %s\n", filename);
        printf("Maybe the file has not yet been created.\n");
        printf("Please select from the MAIN Menu\n");
        printf("to create a new file.\n");
        S_Pause();
        return 0;
        }
    else  // Continue to process text file...
        {
        // For obtaining a character count to build our dynamic array.
        fseek(fpFileIn, 0, SEEK_END);  // Set pointer to end of file.
        int char_Total = ftell(fpFileIn);  // get counter value.
        rewind(fpFileIn);  // Set pointer back to the start of the file.

        // Read Character by character and check for new line '\n'.
        // I am testing every char in the file rather than testing line by line.
        for(cnt1 = 0; cnt1 < char_Total; cnt1++)
            {
            Char_Buffer = fgetc(fpFileIn);

            if(Char_Buffer == '\n')  // Test if we have encountered a new line and,
                {
                Total_Lines++;  // increment the number of new lines in the file.
                if(Line_Width > Max_Width)
                    {
                    // Find and store the longest line.
                    Max_Width = Line_Width;
                    // Reset the width counter for the next line.
                    Line_Width = 0;
                    }
                }
            Line_Width++;
            }
        // Set pointer to start of file.
        // (Start the next file read from the first character of first line.)
        rewind(fpFileIn);

        // Create a temp buffer with Max_Width size to hold each line.
        char *string_Temp_Buffer;
        if((string_Temp_Buffer = malloc((Max_Width +4) * sizeof(char))) != NULL)
            {
            for(cnt1=0; cnt1 < Max_Width +4; cnt1++)
                {
                string_Temp_Buffer[cnt1] = '\0';  // Initialise the array to nul
                }
            }
        else
            {
            // This constitutes an application failure from which we must close
            // the application. The user should never receive this error! :)
            printf("Error - unable to allocate required memory for temp buffer.\n");
            S_Pause();
            return -1;
            }

        // Now that we know how many lines (Total_Lines) to allocate, and the
        // length of the longest line (Max_Width) we can create a suitable
        // sized array to hold the contents.

        // char Read_Buffer[lines/Total_Lines][Max_Width]
        // NOTE! The extra + 4 characters is required to hold additional control
        // characters beyond the length of the string. The string "Hello/n" is 5
        // characters long. This is stored as 'H','e','l','l','o','\r','\n','\0'
        // The '\r','\n','\0' requires an extra 3 characters.
        // I just add 4 for safety :)
        // I add an extra +1 lines for reading files for a safety buffer.
        // This is somewhat advanced and beyond the scope of a beginner, but I
        // have no other safe option other than to create the array using
        // pointer arithmetic and dynamic memory. There are multiple ways to
        // achieve this beyond what I have shown here.
        char **Read_Buffer;  // Array to hold the content of the text file read.
        if((Read_Buffer = malloc((Total_Lines +1) * sizeof(char *))) != NULL)
            {
            for(cnt1=0; cnt1 < Total_Lines; cnt1++)
                {
                if((Read_Buffer[cnt1] = malloc((Max_Width +4) * sizeof(char))) != NULL)
                    {
                    for(cnt2=0; cnt2 < Max_Width +4; cnt2++)
                        {
                        // Initialise the array to nul
                        // Note: nul is a character 0 '\0', while null is
                        // a pointer to a non existent object.
                        Read_Buffer[cnt1][cnt2] = '\0';
                        }
                    }
                }
            }
        else
            {
            // This constitutes an application failure from which we must close
            // the application. The user should never receive this error! :)
            printf("Error - unable to allocate required memory for array.\n");
            S_Pause();
            return -1;
            }



        // Next we need to read each line into our Read_Buffer and remove
        // the New line chars '\n'.
        int cnt_lines = 0;  // track array line position.
        while(fgets(string_Temp_Buffer, Max_Width +4, fpFileIn) != NULL)
            {
            // Walk each line from the file (returns the string
            // with '\n' at the end).
            // Strip the newline character from the line and
            // replace newline char '\n' '\r' with '\0'.
            //string_Temp_Buffer[strcspn(string_Temp_Buffer, "\r\n")] = '\0';
            // Copy the cleaned line of text into our array.
            strcpy(Read_Buffer[cnt_lines], string_Temp_Buffer);
            cnt_lines++;  // move to next line and repeat.
            }

        // It is important to free up resources as soon as they are no longer required.
        fclose(fpFileIn);  // finished file reads, close the file.

        // Walk through the List and printf each line as text.
        //int cnt1;
        // Note! C Arrays are just integer pointers to the data in memory so we
        // can't accurately test the length of the array at runtime.
        for(cnt1=0; cnt1 < Total_Lines; cnt1++)
            {
            printf("%s", Read_Buffer[cnt1]);
            }
        printf("\n");  // End of line, next line.

        // Important to always "free" the memory as soon as we are finished with it.
        // Not doing so will lead to a memory leak as a new block of memory will be
        // created on the heap each time the array is used.
        free(Read_Buffer);
        free(string_Temp_Buffer);
        }  // END file open if, else test.
    S_Pause();
    Con_Clear();
    return 0;
    }

int Mandelbrot_Fractals_Console_ASCII(void)
    {
    /*
    Credits:
    https://cs.nyu.edu/~perlin/
    Ken Perlin
    Professor of Computer Science
    NYU Future Reality Lab F
    >
    Original Source:
    main(k){float i,j,r,x,y=-16;while(puts(""),y++<15)for(x
    =0;x++<84;putchar(" .:-;!/>)|&IH%*#"[k&15]))for(i=k=r=0;
    j=r*r-i*i-2+x/25,i=2*r*i+y/10,j*j+i*i<11&&k++<111;r=j);}
    >
    Info on Mandelbrot sets
    https://mathworld.wolfram.com/MandelbrotSet.html
    */

    // Although this describes a series of planes in 3D layers, the calculations
    // are graphed to a 2D plane and use colour depth to describe points in the
    // lower layers (planes).

    Con_Clear();
    int k = 1;  // First print character; default = 1 (0 to leave blank).
    char colours[] = " .:-;!/>)|&IH%*#";  // 16 colours

    float i = 0;
    float j = 0;
    float r = 0;
    float x = 0;
    float y = -16;

    // zoom_x, zoom_y are relative and both must be changed as a percentage.
    float zoom_x = 25.00;  // Default = 25,+zoom-In/-zoom-Out
    float zoom_y = 10.00;  // Default = 10,+zoom-In/-zoom-Out

    float offset_x = -2.00;  // Default = -2.00, -pan-L/+pan-R
    float offset_y = 0.00;  // Default = 0.00, -pan-U/+pan-D

    while(y < 15)  // Loop #1
        {
        y++;
        puts("");  // Line break '\n'.

        for(x = 0; x < 88; x++)  // Loop #2, (<84 == the screen print width.)
            {
            // Select colour level (Bitwise AND) from 16 colours, then print.
            putchar(colours[k&15]);

            i=k=r=0;
            while(1)  // Loop #3
                {
                // Calculate x fractal.
                j = ((r*r) - (i*i) + ((x/zoom_x) + offset_x));
                // Calculate y fractal.
                i = ((2*r*i) + ((y/zoom_y) + offset_y));

                // Test for x,y divergence to infinity (lemniscates).
                // In a sense this relates to the period between depth layers
                // and the scale at which they diverge to infinity.
                // The default values offer the most visually appealing balance,
                // meaning they are easier for our brain to interpret.
                if(j*j+i*i > 11)  // Default = 11
                    {
                    break;
                    }

                // Test depth level (Colour).
                k++;
                if(k > 111)  // Default = 111.
                    {
                    break;
                    }

                r=j;  // Start next calculation from current fractal.
                }
            }
        }
    S_Pause();
    Con_Clear();
    return 0;
    }


// --> START helper functions

// System alert sound (Bell '\a')
int Sys_Sound(void)
    {
    if(OS_Windows == 1)
        {
        //printf("%c", '\a');
        system("rundll32 user32.dll,MessageBeep");
        //system("rundll32.exe Kernel32.dll,Beep 750,300");
        }
    else if(OS_Unix == 1)
        {
        system("paplay /usr/share/sounds/ubuntu/notifications/Blip.ogg");
        //system("paplay /usr/share/sounds/ubuntu/notifications/Rhodes.ogg");
        //system("paplay /usr/share/sounds/ubuntu/notifications/Slick.ogg");
        //system("paplay /usr/share/sounds/ubuntu/notifications/'Soft delay.ogg'");
        //system("paplay /usr/share/sounds/ubuntu/notifications/Xylo.ogg");
        }
    else
        {
        printf("%c", '\a');
        }
    return 0;
    }

// A simple message box for debugging.
int DebugMsg(char *aTitle, char *aMessage)
    {

    if(OS_Windows)
        {
        // Requires:winuser.h (include Windows.h),User32.dll
        // Not attached to parent console window.
        // https://docs.microsoft.com/en-us/windows/win32/api/winuser
        // /nf-winuser-messageboxa
        #ifdef __WIN32__
        int reterr = 0;
        // May throw a compiler warning... MessageBoxA Not found.
        reterr = MessageBoxA(0, aMessage, aTitle, 0);  // 65536, MB_SETFOREGROUND
        if(reterr == 0)
            {
            // MessageBox() Fail
            return -1;
            }
        #endif // __WIN32__
        // Attached to parent console window.
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
        int reterr = 0;
        char Buffer[128] = {'\0'};
        char Buf_Msg[128] = {'\0'};
        // Place title text in 'apostrophe'.
        strcpy(Buf_Msg, "\'");
        strcat(Buf_Msg, aTitle);
        strcat(Buf_Msg, "\'");

        // Build our command line statement.
        // xmessage [-options] [message ...]
        strcpy(Buffer, "xmessage -center -title ");
        strcat(Buffer, Buf_Msg);
        strcat(Buffer, " \'.:|" );
        strcat(Buffer, aMessage );
        // NOTE! ">>/dev/null 2>>/dev/null" suppresses the console output.
        strcat(Buffer, "|:.\' >>/dev/null 2>>/dev/null" );
        // Send it to the command line.
        reterr = system(Buffer);
        if(reterr != 0) && (reterr != 1) // xmessage failed or not exist.
            {
            // Try Wayland compositor wlmessage.
            strcpy(Buffer, "wlmessage \' |" );
            strcat(Buffer, aMessage );
            strcat(Buffer, "| \' >>/dev/null 2>>/dev/null" );
            reterr = system(Buffer);
            if(reterr != 0) && (reterr != 1)
                {
                // Popup message failed.
                //printf("%d\n", reterr);
                return -1;
                }
            }
        return 0;
        }

    return 0;
    }

// Console Clear
int Con_Clear(void)
    {
    // The system() call allows the programmer to run OS CLI batch commands.
    // It is discouraged as there are more appropriate C functions for most tasks.
    // I am only using it in this instance to avoid invoking additional OS API
    // headers and code.
    if(OS_Windows)
        {
        system("cls");
        }
    else if(OS_Unix)
        {
        system("clear");
        }
    return 0;
    }

// Safe Pause
void S_Pause(void)
    {
    // This function is referred to as a wrapper for S_getchar()
    printf("\nPress [Enter] to continue...");
    S_getchar();  // Uses S_getchar() for safety.
    }

// Safe getcar() removes all artefacts from the stdin buffer.
int S_getchar(void)
    {
    // This function is referred to as a wrapper for getchar()
    int i = 0;
    int ret;
    int ch;
    // The following enumerates all characters in the buffer.
    while((ch = getchar()) != '\n' && ch != EOF )
        {
        // But only keeps and returns the first char.
        if (i < 1)
            {
            ret = ch;
            }
        i++;
        }
    return ret;
    }

// Console Input (stdin).
// buf is the return buffer, str is printed to the screen.
char *Input(char *buf, char *str, int n)
    {
    FILE *stream = stdin;
    char *empty = "";
    int ret;
    ret = strcmp(str, empty);
    if(ret != 0)  // Don't print an empty string.
        {
        printf("%s", str);  // Input Message
        }
    return S_fgets(buf, n, stream);
    }

// Safe fgets() removes all artefacts from the stdin buffer.
// buf must be at least n + 1 for '\0'
char *S_fgets(char *buf, int n, FILE *stream)
    {
    int i = 0;
    int ch;
    //memset(buf, 0, n);
    // The following enumerates all characters in the buffer.
    while((ch = getc(stream)) != '\n' && ch != EOF )
        {
        // But only keeps and returns n chars.
        if (i < n)
            {
            buf[i] = ch;
            }
        i++;
        }
    buf[i] = '\0';
    return buf;
    }

// Wrapper for the 2 functions time/date.
// We can create convenience wrapper functions to "wrap" a set of more
// complex tasks in a single function call.
// This is helpful if it is a common set of tasks that are called regularly
// throughout an application.
/*
#include<stdio.h>
#include<time.h>
void datetime(void)
    {
        time_t t;
        time(&t);
        printf(ctime(&t));
    }
*/
int Show_Time_Date(void)
    {
/*
struct tm {
int tm_sec; // seconds, range 0 to 59
int tm_min; // minutes, range 0 to 59
int tm_hour; // hours, range 0 to 23
int tm_mday; // day of the month, range 1 to 31
int tm_mon; // month, range 0 to 11
int tm_year; // The number of years since 1900
int tm_wday; // day of the week, range 0 to 6
int tm_yday; // day in the year, range 0 to 365
int tm_isdst; // daylight saving time
};
*/
    show_time();
    printf(" - ");
    show_date();
    printf("\n");
    return 0;
    }

// Display current system time.
int show_time(void)
    {
    // Windows Command-line
    //system("ECHO %time% - %date%");
    //system("TIME /T");
    // Unix command-line
    //system("date +%T");

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("%02d:%02d:%02d", tm.tm_hour, tm.tm_min, tm.tm_sec);

    return 0;
    }

// Display current system date.
int show_date(void)
    {
    // Windows command-line
    //system("ECHO %time% - %date%");
    //system("DATE /T");
    // Unix command-line
    //system("date +%F");

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("%d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

    return 0;
    }
