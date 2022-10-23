//------------------------------------------------------------------------------
// Name:        Arrays
// Purpose:     Example
//
// Platform:    Win64, Ubuntu64
//
// Author:      Axle
// Created:     03/02/2022
// Updated:     19/02/2022
// Copyright:   (c) Axle 2022
// Licence:     MIT No Attribution
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// C preprocessor replaces all occurrences in the source of ROW with 5 etc.
#define ROWS 5
#define COLUMNS 5
#define STRMAX 32  // A buffer of 32 chars to hold our strings.

int main()  // Main procedure
    {
    const int Rows = ROWS;
    const int Columns = COLUMNS;
    char TempBuffer[STRMAX] = {'\0'};  // array of 32 characters, aka String.

    // defining a 2D array of array[Rows][Columns], initialised to empty.
    static char Table[ROWS][COLUMNS][STRMAX] = {'\0'};  // initialised to null
    // static char canvas[5][5][32] = {'\0'};  // Actual values of above.

    int Row_y;  // Counter for rows.
    int Col_x;  // Counter for columns.
    int Offset = 1;  // Origin zero offset (0|1)
    // Fill the array with some data.
    for(Row_y = 0; Row_y < Rows; Row_y++)  // Count through each row.
        {
        for(Col_x = 0; Col_x < Columns; Col_x++)  // Count through each column.
            {
            // Build a string with values for each cell. The offset starts
            // the Row and columns count at 1 instead of 0.
            sprintf(TempBuffer, "[R:%d,C:%d]", Row_y + Offset, Col_x + Offset);
            // Copy the string to the cell position.
            strcpy(Table[Row_y][Col_x], TempBuffer);
            }
        }

    // Print the array to the console.
    for(Row_y = 0; Row_y < Rows; Row_y++)  // Count through each row.
        {
        for(Col_x = 0; Col_x < Columns; Col_x++)  // Count through each column.
            {
            printf("%s ", Table[Row_y][Col_x]);  // No line breaks.
            }
        printf("\n");  // Print a line break to start next row.
        }

    printf("Press the [Enter] key to continue...");
    getchar();  // Pause the program until a key is pressed
    return 0;
    }
