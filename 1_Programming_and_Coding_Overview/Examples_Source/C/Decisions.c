//------------------------------------------------------------------------------
// Name:        Decisions.c
// Purpose:     Example
// Title:       "A Snail's Life"
//
// Platform:    Win64, Ubuntu64
//
// Author:      Axle
// Created:     03/02/2022
// Updated:     19/02/2022
// Copyright:   (c) Axle 2022
// Licence:     MIT No Attribution
//------------------------------------------------------------------------------

// C std library headers.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>  // To seed Random.

// Platform specific headers.
// Test if Windows or Unix OS
#ifdef _WIN32
#define OS_Windows 1  // 1 = True (aka Bool)
#define OS_Unix 0
#endif

#ifdef __unix__  // __linux__
#define OS_Unix 1
#define OS_Windows 0  // 0 = False (aka Bool)
#endif

// Global Constants
#define ROWS 10
#define COLUMNS 40
#define STRMAX 4  // A buffer of 4 chars to hold our strings.

int UpdateScreen(char table[ROWS][COLUMNS][STRMAX]);
int Con_Clear(void);
void S_Pause(void);
int S_getchar(void);

int main()  // Main procedure
    {
    const int Row_start = ROWS - ROWS + 1;  // Set the boundaries inside of the fence.
    const int Row_end = ROWS - 2;  // -2 To account for the fence at top and bottom.
    const int Row_width = Row_end - Row_start;  // ROWS - 2 To account for the fence at each side.
    const int Col_start = COLUMNS - COLUMNS + 1;  // Set the boundaries inside of the fence.
    const int Col_end = COLUMNS - 2;  // -2 To account for the fence at each side.
    const int Col_width = Col_end - Col_start;  // COLUMNS - 2 To account for the fence at each side.

    // Char 'r', 's' are not strings and must use the array[n][n][0] to be entered
    // as the first character of the string. The STRMAX allows us 4 char spaces
    // initialised to 0 to build a string in.
    // In C '' means a single character and "" represents a string.
    // array[n][n][STRMAX] = {'\0', '\0', '\0', '\0',}  // '\0' is null terminator (end of string)
    // array[n][n][STRMAX] = {'v', '\0', '\0', '\0',}  // our string with 1 letter.
    // array[n][n] // to access it as a string "v".
    // Define the characters for the console game.
    const char chr_1 = ' ';
    const char chr_2 = '@';
    const char chr_3 = 'v';
    const char chr_4 = 'w';
    const char chr_5 = '+';

    // Values to random generate grass.
    // 0 - 10 (0=0% fill, 5= 50% fill, 10= 100% fill)
    const int GFill = 5;
    // 0 - 10 Split rand fill between % 0='w' and 10='W'.
    const int GSplit = 4;

    // defining a 2D array of array[Rows][Columns], initialised to empty.
    static char Table[ROWS][COLUMNS][STRMAX] = {'\0'};
    static int Sprite[2][2];  // Sprite[0][0|1] = current [Row|Col]
    int Row_y;  // Counter for rows.
    int Col_x;  // Counter for columns.

    // Use current time as seed for random generator
    srand(time(0));
    // Fill the array with some data.
    for (Row_y = 0; Row_y < ROWS; Row_y++)  // Count through each row.
        {
        for (Col_x = 0; Col_x < COLUMNS; Col_x++)  // Count through each column.
            {
            if ((Col_x >= Col_start) && (Col_x <= Col_end) && (Row_y >= Row_start) && (Row_y <= Row_end))
                {
                // Build a string with values for each cell.
                // rand()%10 returns 0 to 9
                if (rand() % 10 > GFill)  // Total grass GFill = 5(50%) fill.
                    {
                    if (rand() % 10 > GSplit)  //  Split between v and w.
                        {
                        Table[Row_y][Col_x][0] = chr_3;  // Insert a character for grass.
                        }
                    else
                        {
                        Table[Row_y][Col_x][0] = chr_4;  // Insert a character for grass.
                        }
                    Table[Row_y][Col_x][1] = '\0';
                    }
                else
                    {
                    Table[Row_y][Col_x][0] = chr_1;
                    Table[Row_y][Col_x][1] = '\0';
                    }
                }
            else
                {
                Table[Row_y][Col_x][0] = chr_5;  // Insert the fence character.
                Table[Row_y][Col_x][1] = '\0';
                }
            }
        }

    // Variables to track the Sprite (Snail) position.
    // Sprite[Sp_now][Sp_row] <- array format.
    const int Sp_now = 0;// Current sprite position.
    //const int Sp_last = 1;// previous position (Unused).
    const int Sp_row = 0;
    const int Sp_col = 1;

    // Set the snail at a random location and record it in an array.
    Sprite[Sp_now][Sp_row] = (rand() % Row_width) +1;  // +1 for left 0 boundary alignment.
    Sprite[Sp_now][Sp_col] = (rand() % Col_width) +1;  // +1 for top 0 boundary alignment..

    // Update the Table with the sprite.
    Table[Sprite[Sp_now][Sp_row]][Sprite[Sp_now][Sp_col]][0] = chr_2;

    // Begin main movement and life counter loop
    // Note! that Control character 7 '\a' is system dependent and may not make a sound.
    char Input;  // 'N''S''e''w' etc are single characters, not Strings here.
    int Lnrg = 10;  // Start at 10 life points. Reduces by 1 each move.
    while(1)
        {
        UpdateScreen(Table);  // Update the screen.
        printf("+ Life energy = %3d \n", Lnrg);  //  %3d allocates 3 positions.
        if (Lnrg <= 0)
            {
            printf("\a");  // System bell (ASCII Control Chr 7)
            printf("Oh No! You are out of life energy...\n");
            break;
            }

        printf("N,S,E,W to move : Q to Quit\n");
        printf("Type your selection followed by [Enter] >>");
        Input = S_getchar();
        printf("\n");
        Lnrg -= 1;  // Reduce 1 nrg point for each move including hitting the boundary.

        if ((Input == 'N') || (Input == 'n'))
            {
            if (Sprite[Sp_now][Sp_row] - 1 != Row_start -1)
                {
                // The following if else block would be best subed to a function
                // to limit duplication.
                if (Table[Sprite[Sp_now][Sp_row] -1][Sprite[Sp_now][Sp_col]][0] == chr_3)
                    {
                    Lnrg +=1;  // Increase 1 nrg point for small grass.
                    }
                else if (Table[Sprite[Sp_now][Sp_row] -1][Sprite[Sp_now][Sp_col]][0] == chr_4)
                    {
                    Lnrg +=2;  // Increase 2 nrg point for large grass.
                    }
                else
                    {
                    // pass
                    }
                Table[Sprite[Sp_now][Sp_row]][Sprite[Sp_now][Sp_col]][0] = chr_1;
                Sprite[Sp_now][Sp_row] -=1;
                Table[Sprite[Sp_now][Sp_row]][Sprite[Sp_now][Sp_col]][0] = chr_2;
                // Move Lnrg -= 1; here to not reduce Life nrg when hitting the boundary.
                }
            else
                {
                printf("\a");  // System bell (ASCII Control Chr 7)
                }

            }
        else if ((Input == 'S') || (Input == 's'))
            {
            if (Sprite[Sp_now][Sp_row] + 1 != Row_end +1)
                {
                if (Table[Sprite[Sp_now][Sp_row] +1][Sprite[Sp_now][Sp_col]][0] == chr_3)
                    {
                    Lnrg +=1;  // Increase 1 nrg point for small grass.
                    }
                else if (Table[Sprite[Sp_now][Sp_row] +1][Sprite[Sp_now][Sp_col]][0] == chr_4)
                    {
                    Lnrg +=2;  // Increase 2 nrg points for large grass.
                    }
                else
                    {
                    // pass
                    }
                Table[Sprite[Sp_now][Sp_row]][Sprite[Sp_now][Sp_col]][0] = chr_1;
                Sprite[Sp_now][Sp_row] +=1;
                Table[Sprite[Sp_now][Sp_row]][Sprite[Sp_now][Sp_col]][0] = chr_2;
                // Move Lnrg -= 1; here to not reduce Life nrg when hitting the boundary.
                }
            else
                {
                printf("\a");  // System bell (ASCII Control Chr 7)
                }
            }
        else if ((Input == 'E') || (Input == 'e'))
            {
            if (Sprite[Sp_now][Sp_col] + 1 != Col_end +1)
                {
                if (Table[Sprite[Sp_now][Sp_row]][Sprite[Sp_now][Sp_col] +1][0] == chr_3)
                    {
                    Lnrg +=1;  // Increase 1 nrg point for small grass.
                    }
                else if (Table[Sprite[Sp_now][Sp_row]][Sprite[Sp_now][Sp_col] +1][0] == chr_4)
                    {
                    Lnrg +=2;  // Increase 2 nrg points for large grass.
                    }
                else
                    {
                    // pass
                    }
                Table[Sprite[Sp_now][Sp_row]][Sprite[Sp_now][Sp_col]][0] = chr_1;
                Sprite[Sp_now][Sp_col] +=1;
                Table[Sprite[Sp_now][Sp_row]][Sprite[Sp_now][Sp_col]][0] = chr_2;
                // Move Lnrg -= 1; here to not reduce Life nrg when hitting the boundary.
                }
            else
                {
                printf("\a");  // System bell (ASCII Control Chr 7)
                }
            }
        else if ((Input == 'W') || (Input == 'w'))
            {
            if (Sprite[Sp_now][Sp_col] - 1 != Col_start -1)
                {
                if (Table[Sprite[Sp_now][Sp_row]][Sprite[Sp_now][Sp_col] -1][0] == chr_3)
                    {
                    Lnrg +=1;  // Increase 1 nrg point for small grass.
                    }
                else if (Table[Sprite[Sp_now][Sp_row]][Sprite[Sp_now][Sp_col] -1][0] == chr_4)
                    {
                    Lnrg +=2;  // Increase 2 nrg points for large grass.
                    }
                else
                    {
                    // pass
                    }
                Table[Sprite[Sp_now][Sp_row]][Sprite[Sp_now][Sp_col]][0] = chr_1;
                Sprite[Sp_now][Sp_col] -=1;
                Table[Sprite[Sp_now][Sp_row]][Sprite[Sp_now][Sp_col]][0] = chr_2;
                // Move Lnrg -= 1; here to not reduce Life nrg when hitting the boundary.
                }
            else
                {
                printf("\a");  // System bell (ASCII Control Chr 7)
                }
            }
        else if ((Input == 'Q') || (Input == 'q'))
            {
            break;
            }
        else
            {
            // Pass
            }
        }

    S_Pause();
    return 0;
    }

int UpdateScreen(char table[ROWS][COLUMNS][STRMAX])
    {
    Con_Clear();
    int Row_y;  // Counter for rows.
    int Col_x;  // Counter for columns.

    // Print the array to the console.
    for (Row_y = 0; Row_y < ROWS; Row_y++)// Count through each row.
        {
        for (Col_x = 0; Col_x < COLUMNS; Col_x++)  // Count through each column.
            {
            printf("%s", table[Row_y][Col_x]);  // No line breaks.
            }
        printf("\n");  // Print a line break to start next row.
        }
    return 0;
    }

int Con_Clear(void)
    {
    // The system() call allows the programmer to run OS command line batch commands.
    // It is discouraged as there are more appropriate C functions for most tasks.
    // I am only using it in this instance to avoid invoking additional OS API headers and code.
    if (OS_Windows)
        {
        system("cls");
        }
    else if (OS_Unix)
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
