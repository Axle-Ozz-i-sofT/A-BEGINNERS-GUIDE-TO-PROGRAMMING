//------------------------------------------------------------------------------
// Name:        guess.c
// Purpose:     Guess a number
//
// Platform:    Win64, Ubuntu64
//
// Author:      Axle
//
// Created:     18/12/2021
// Updated:     19/02/2022
// Copyright:   (c) Axle 2021
// Licence:     MIT No Attribution
//------------------------------------------------------------------------------

#include <stdio.h>  // include standard library headers
//#include <stdlib.h>

int main(int argc, char *argv[])  // Main procedure
    {
    int Number_To_Guess = 8;  // Create our variables.
    int User_Guess = 0;  // Create our variables.

    printf("Hello User.. Can you guess my number?\n");
    printf("Enter a number between 1 and 10\n");

    while(1)  // Loop forever or until a 'Break" statement is reached.
        {
        printf("Enter your guess: ");
        scanf("%d", &User_Guess);  // Get the user input from the console.
        // v Conditional statement using || Logical Or
        if ((User_Guess < 1) || (User_Guess > 10))
            {
            printf("Not a number between 1 and 10!\n");
            }
        else if (User_Guess < Number_To_Guess)  // Conditional statement
            {
            printf("A little higher...\n");
            }
        else if (User_Guess > Number_To_Guess)  // Conditional statement
            {
            printf("A little Lower...\n");
            }
        else  // If nothing else found
            {
            break;  // Breaks out of the while loop (Logically the same as == 8)
            }
        }

    printf("Congratulation! You guessed the number.\n");

    printf("Press the [Enter] key to continue...");
    getchar();  // Pause the program until a key is pressed
    return 0;
    }
