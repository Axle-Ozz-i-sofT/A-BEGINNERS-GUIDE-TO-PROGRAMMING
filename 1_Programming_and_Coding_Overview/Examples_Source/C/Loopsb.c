//-------------------------------------------------------------------------------
// Name:        Loopsb.c
// Purpose:     Loops Animation
//
// Platform:    Win64, Ubuntu64
//
// Author:      Axle
// Created:     16/02/2022
// Updated:     18/02/2022
// Copyright:   (c) Axle 2022
// Licence:     MIT No Attribution
//-------------------------------------------------------------------------------
// Check unistd.h Beep(), system("beep")
// https://frank-buss.de/beep/
//------------------------------------------------------------------------------

// C std library headers.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
int Con_Clear(void);
void S_Pause(void);
int S_getchar(void);

int main(int argc, char *argv[])  // Main procedure
    {
    unused(argc);  // Turns off the compiler warning for unused argc, argv
    unused(argv);  // Turns off the compiler warning for unused argc, argv

    // ---> START "Loops Examples"
    // ---> START "Screen animation one routines"
    // Loading screen animation one.
    // [8] allows room for escape sequences such as end of string '\0'
    // '\' is an escape prefix and must be placed in a literal as '\\'
    static char BarRotate[8] = "|\\-/";
    char Notice[] = "Please wait...";  // Notice[] == *Notice
    float RotSpeed = 200;  //200 This is the sleep timer. Lower setting = faster.
    int counts = 15;  // The number of scenes.
    int x = 0;  // loop counters
    int y = 0;  // loop counters
    int z = 0;  // loop counters

    printf("%s\n", Notice);
    // Keep looping through the 4 characters.
    for(x = 0; x < counts ; x++)
        {
        if(y > 3)  // At the fourth (0,1,2,3) character '/' restart from 0
            {
            y = 0;  // reset to 0 and restart character enumeration
            }
        printf("%c", BarRotate[y]);  // '\b' = Backspace control character.
        fflush(stdout);  // If '\n' not found, push the print to the console.
        Sleep(RotSpeed);  //RotSpeed replaced with cross platform wrapper.
        y++;
        printf("\r");  // Backspace key to clear last character.
        }
    Con_Clear();  // Clear the screen for the next animation.

    // ---> START "Screen animation two routines"
    // Loading screen animation two.
    // Create the array containing the 4 strings.
    // Special characters must be preceded by an escape in side of strings '\'
    static char BigRotate1[4][16] =
        {
            {
            '\\',' ',' ','\n',
            ' ','\\',' ','\n',
            ' ',' ','\\','\0'
            },

            {
            ' ','|',' ','\n',
            ' ','|',' ','\n',
            ' ','|',' ', '\0'
            },

            {
            ' ',' ','/','\n',
            ' ','/',' ','\n',
            '/',' ',' ','\0'
            },

            {
            ' ',' ',' ','\n',
            '-','-','-','\n',
            ' ',' ',' ','\0'
            }
        };

    // This is the exact same array as above created in string format.
    static char BigRotate2[4][16] =
        {
            {
            "\\  \n"
            " \\ \n"
            "  \\\0"
            },

            {
            " | \n"
            " | \n"
            " | \0"
            },

            {
            "  /\n"
            " / \n"
            "/  \0"
            },

            {
            "   \n"
            "---\n"
            "   \0"
            }
        };

    while(z < counts)  // Repeat counts times.
        {
        for(x = 0; x < 4; x++)  // loop through all 4 array elements.
            {
            printf("%s\n", Notice);
            printf("%s", BigRotate1[x]);  // Alternative use BigRotate2
            fflush(stdout);  // If '\n' not found, push the print to the console.
            Sleep(RotSpeed);  // Slow the animation down.
            Con_Clear();
            }
        z+= 4;
        }

    Con_Clear();  // Clear the screen for the next animation.
    Sleep(500);  // Just a pause to reduce screen/buffer flicker.

    // ---> START "Screen animation three routines"
    // Loading screen animation three.
    static char Tracer[22] = {'\0'};  // Initiate all to 0.
    for(x = 0; x < 21; x++)  // Populate with space, except for string terminator[22].
        {
        Tracer[x] = ' ';
        }

    int t = 0;  // loop counters
    z = 0;  // Reset loop counters
    int tr_repeats = 0;
    if(1 == OS_Unix)  // Linux
        {
        tr_repeats = 2;  // for slow Linux sleep(seconds)
        }
    else  // Windows
        {
        tr_repeats = 3;
        }

    while(z < tr_repeats)  // abs(n/5) to reduce the repeats.
        {
        while(t < 20)  // Loop forward
            {
            t++;  // Start at 0 +1 to allow room for the tail
            Tracer[t] = '#';  // Add char at current position.
            Tracer[t-1] = ':';  // Add tail at current position -1.
            printf("%s\n", Notice);
            printf("%s", Tracer);  // Print full array/string to the screen.
            fflush(stdout);  // If '\n' not found, push the print to the console.
            Tracer[t-1] = ' ';  // Remove the tail. Next tail will overwrite #.
            Sleep(RotSpeed/4);  // Slow the animation down a bit.
            Con_Clear();  // Clear the screen for the next print.
            }
        while(t > 0)  // Loop backwards
            {
            t--;  // Start at -1 to allow room for the tail at t.
            Tracer[t] = '#';
            Tracer[t+1] = ':';  // t+1 is the right side of '#'.
            printf("%s\n", Notice);
            printf("%s", Tracer);
            fflush(stdout);
            Tracer[t+1] = ' ';
            Sleep(RotSpeed/4);
            Con_Clear();
            }
        Sleep(RotSpeed/4);
        z+= 1;
        }

    Con_Clear();  // Clear the screen for the next animation.
    Sleep(500);  // Just a pause to reduce screen/buffer flicker.

    // ---> START "Stick-man animation routines"
    // Stick-man animation
    // Define our data arrays...
    // This is the original ASCII text file from the net.
    // The arrays were created by hand in Notepad++ on this occasion.
    //  o   \ o /  _ o         __|    \ /     |__        o _  \ o /   o     |
    // /|\    |     /\   ___\o   \o    |    o/    o/__   /\     |    /|\    |
    // / \   / \   | \  /)  |    ( \  /o\  / )    |  (\  / |   / \   / \    |

    int Len_Cart = 11;  // Keep a record of the array length.
    // The second element is the size of the string buffer...
    // Note: We must use an escape character for special characters \\ = '\'.
    static char Cart1[11][60] =  // [11][17*3]
        {
            {
            ' ',' ',' ','o',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','\n',
            ' ',' ','/','|','\\',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','\n',
            ' ',' ','/',' ','\\',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','\n','\0'
            },

            {
            ' ','\\',' ','o',' ','/',' ',' ',' ',' ',' ',' ',' ',' ',' ','\n',
            ' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','\n',
            ' ',' ','/',' ','\\',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','\n','\0'
            },

            {
            ' ',' ','_',' ','o',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','\n',
            ' ',' ',' ','/','\\',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','\n',
            ' ',' ','|',' ','\\',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','\n','\0'
            },

            {
            ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','\n',
            ' ',' ',' ','_','_','_','\\','o',' ',' ',' ',' ',' ',' ',' ','\n',
            ' ',' ','|',')',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ','\n','\0'
            },

            {
            ' ',' ',' ',' ','_','_','|',' ',' ',' ',' ',' ',' ',' ',' ','\n',
            ' ',' ',' ',' ',' ',' ','\\','o',' ',' ',' ',' ',' ',' ',' ','\n',
            ' ',' ',' ',' ',' ',' ','(',' ','\\',' ',' ',' ',' ',' ',' ','\n','\0'
            },

            {
            ' ',' ',' ',' ',' ',' ','\\',' ','/',' ',' ',' ',' ',' ',' ','\n',
            ' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ','\n',
            ' ',' ',' ',' ',' ',' ','/','o','\\',' ',' ',' ',' ',' ',' ','\n','\0'
            },

            {
            ' ',' ',' ',' ',' ',' ',' ',' ',' ','|','_','_',' ',' ',' ','\n',
            ' ',' ',' ',' ',' ',' ',' ','o','/',' ',' ',' ',' ',' ',' ','\n',
            ' ',' ',' ',' ',' ',' ','/',' ',')',' ',' ',' ',' ',' ',' ','\n','\0'
            },

            {
            ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','\n',
            ' ',' ',' ',' ',' ',' ',' ',' ','o','/','_','_',' ',' ',' ','\n',
            ' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ','(','\\',' ',' ','\n','\0'
            },

            {
            ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','o',' ','_',' ','\n',
            ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','/','\\',' ',' ','\n',
            ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','/',' ','|',' ','\n','\0'
            },

            {
            ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','\\',' ','o',' ','/','\n',
            ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ','\n',
            ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','/',' ','\\',' ','\n','\0'
            },

            {
            ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','o',' ',' ','\n',
            ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','/','|','\\',' ','\n',
            ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','/',' ','\\',' ','\n','\0'
            }
        };

    // This is the exact same array as above created in string format.
    // Note: We must use an escape character for special characters \\ = '\'.
    static char Cart2[11][60] =  // [11][18*3] <- 60 just to be safe.
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

    int a = 0;  // Loop counters
    int b = 0;  // Loop counters
    int Speed = 400;  // Delay in milliseconds to slow the animation down.
    unused(Speed);  // Turns off the compiler warning when in Linux
    int C_repeat = 0;  // Repeat the animation for loops 3 times.
    if(1 == OS_Unix)  // Linux
        {
        C_repeat = 2;  // For slow Linux sleep(seconds)
        }
    else  // Windows
        {
        C_repeat = 3;
        }

    while(a < C_repeat)  // Repeat the animation n times.
        {
        for(b = 0; b < Len_Cart; b++)  // Loop animation forward
            {
            Con_Clear();  // Clear the console ready for the next print.
            printf("%s", Cart1[b]);  // Alternative use Cart2
            fflush(stdout);  // If '\n' not found, push the print to the console.
            Sleep(Speed);  // replace with cross platform wrapper.
            }
        for(b = Len_Cart -1; b >= 0; b--)  // Loop animation backwards
            {
            Con_Clear();  // Clear the console ready for the next print.
            printf("%s", Cart1[b]);  // Alternative use Cart2
            fflush(stdout);
            Sleep(Speed);  // replaced with cross platform wrapper.
            }
        a++;
        }

    // END Loops examples <---

    S_Pause();  // DEBUG Pause
    return 0;
    }  // END main() <---

// --> START helper functions

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
    printf("Press any key to continue...");
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
