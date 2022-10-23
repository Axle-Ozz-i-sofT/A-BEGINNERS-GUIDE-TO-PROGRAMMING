//------------------------------------------------------------------------------
// Name:        Statements(ST), expressions(EX) and Procedures(PR)
// Purpose:     Example
//
// Platform:    Win64, Ubuntu64
//
// Author:      Axle
// Created:     31/01/2022
// Updated:     19/02/2022
// Copyright:   (c) Axle 2022
// Licence:     MIT No Attribution
//------------------------------------------------------------------------------

#include <stdio.h>  // include standard library headers
#include <stdlib.h>
#include <string.h>

// You can change the radius and pen size below.
#define RADIUS 10  // Do NOT exceed 20! 80 char MAX Console width.
#define PEN 10  // About 1:1 with the Radius to get single char.

int main()  // PR Main procedure
    {
    // Defining a 2D array of array[Rows][Columns], initialised to empty.
    static char canvas[RADIUS * 5][RADIUS * 5][256] = {'\0'};
    // Changing the following String characters will change the circle display.
    char Foreground[] = "O ";  // EX add space after chr to make circle (MAX 2 chrs)
    char Background[] = ". ";  // EX add space after chr to make circle (MAX 2 chrs)

    const int Radius = RADIUS;  // EX
    const int Tolerance = PEN;  // EX Larger numbers will create a wider drawing pen.

    int Row_yy = 0;  // EX counters for array position.
    int Col_xx = 0;  // EX counters for array position.
    int Row_y;  // ST Counter range of circumference. -Radius to +radius.
    int Col_x;  // ST Counter range of circumference. -Radius to +radius.
    for(Row_y = -Radius; Row_y <= Radius; Row_y++)  // PR + EX
        {
        Col_xx =0;  // EX reset the column counter for each row.
        for(Col_x = -Radius; Col_x <= Radius; Col_x++)  // PR + EX
            {
            // Test if it is at the radius
            int equation = Row_y*Row_y + Col_x*Col_x - Radius*Radius;  // EX
            if (abs(equation) < Tolerance)//ST
                {
                strncpy(canvas[Row_yy][Col_xx], Foreground, 2);  //(a) ST
                //printf("%s", Foreground);//(b) ST
                }
            else
                {
                strncpy(canvas[Row_yy][Col_xx], Background, 2);  //(a) ST
                //printf("%s", Background);  //(b) ST
                }
            //printf("%s", canvas[Row_yy][Col_xx]);  //(c) ST
            Col_xx += 1;  // EX Increment Columns

            }
        Row_yy += 1;  // EX Increment Rows
        //printf("\n");  // ST
        }  // END PR

    // Commenting out the loop below, and then enabling the
    // printf statements above will print the circle directly
    // rather than populating the array first and printing later :)
    // Comment out (b) and enable (c) to print directly from the array/list.
    // Or comment out (a)(c) and enable (b) to print directly without the array/list.

    // Print the rows and columns from our canvas array.
    // The last increment of Row_yy and Col_xx from the loop above
    // contains the correct lengths for the following loop.
    for(Row_y = 0; Row_y < Row_yy; Row_y++)// PR + EX
        {
        for(Col_x = 0; Col_x < Col_xx; Col_x++)  // PR + EX
            {
            printf("%s", canvas[Row_y][Col_x]);  // ST
            }
        printf("\n");  // ST
        }  // END PR

    printf("Press the [Enter] key to continue...");
    getchar();  // Pause the program until a key is pressed
    return 0;  // EX
    }  // END PR
