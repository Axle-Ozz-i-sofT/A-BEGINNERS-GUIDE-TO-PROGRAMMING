//-------------------------------------------------------------------------------
// Name:        Operators.c
// Purpose:     Example
//
// Platform:    Win64, Ubuntu64
//
// Author:      Axle
// Created:     20/12/2021
// Updated:     19/02/2022
// Copyright:   (c) Axle 2021
// Licence:     MIT No Attribution
//-------------------------------------------------------------------------------

#include <stdio.h>  // include standard library headers
#include <stdlib.h>

int main(int argc, char *argv[])  // Main procedure
    {
    int num1 = 4;  // Assignment Operator.
    int num2 = 2;  // Assignment Operator.

    num1 = (num1 + num2) / 2;  // Assignment '=', Arithmetic '+' and '/', 3 = (4 + 2) / 2
    num1 += 2;  // Assignment Operator. Same as num1 = num + 2
    num1 -= 2;  // Assignment Operator. 4 = 6 - 2
    num2++;  // Increment Operator. num2 = num2 + 1
    num2--;  // Decrement Operator. num2 = num2 - 1

    if (num1 == num2)  // Equality Operator
        {
        printf("num1 and num2 contain the same value.\n");
        }
    else if (num1 != num2)  // Inequality Operator
        {
        printf("num1 and num2 do not contain the same value.\n");
        }
    else
        {
        printf("The 2 Equality tests above will never allow the program to reach here.\n");
        }

    if (num1 < num2)  // Relational Operator
        {
        printf("num1 is less than num2.\n");
        }
    else if (num1 > num2)  // Relational Operator
        {
        printf("num1 is greater than num2.\n");
        }
    else
        {
        printf("The only option left is that they must be equal.\n");
        }

    if ((num1 < 10) && (num2 < 10))  // Logical AND '&&' and Relational Operators '<'
        {
        printf("The value of both variables is less than 10.\n");
        }
    else if ((num1 > 2) || (num2 > 2))  // Logical OR '||' and Relational Operators '<'
        {
        printf("The value of at least 1 of the variables is greater than 2.\n");
        }
    else
        {
        printf("Neither of the above tests were True and no decision was made.\n");
        }

    if ((num1 * num2) < 10)  // Compound Arithmetic and relational expression inside of an if statement
        {
        printf("The product of num1 and num2 is less than 10.\n");
        }

    printf("Press the [Enter] key to continue...");
    getchar();  // Pause the program until a key is pressed
    return 0;
    }
