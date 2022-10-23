//------------------------------------------------------------------------------
// Name:        Example1.c
// Purpose:     Example
//
// Platform:    Win64, Ubuntu64
//
// Author:      Axle
// Created:     16/12/2021
// Updated:     18/02/2022
// Copyright:   (c) Axle 2021
// Licence:     MIT No Attribution
//------------------------------------------------------------------------------

// Single line comment
/* Multi-line
comment */

#include <stdio.h>  // Include standard library headers.
#include <stdlib.h>  // An inline comment.

int sums(int num1, int num2);  // Declare the function name.
#define MAX_STRING_SIZE 64  // MAX 64 Characters string length.

int main(int argc, char *argv[])  // Main procedure.
    {
    char user_name[MAX_STRING_SIZE];  // Create a string variable.
    int num1, num2, return_value;  // Create Integer Variables.

    printf("Please enter your name: ");  // Print text to console.
    scanf("%s", user_name);  // Get user input.
    printf("Hello %s. Can you please enter 2 numbers:\n", user_name);
    printf("?");
    scanf("%d", &num1);
    printf("?");
    scanf("%d", &num2);
    // v Call our function and send the 2 Integer variables to it.
    return_value = sums(num1, num2);
    // v Print the returned results.
    printf("\n%s the addition of two numbers is : %d\n", user_name, return_value);  

    printf("Press the [Enter] key to continue...");
    getchar();  // Pause the program until a key is pressed
    return 0;
    }

int sums(int num1, int num2)  // Function to add 2 numbers
    {
    int num3;
    num3 = num1 + num2;
    return num3;  // Return the results to the calling statement
    }
