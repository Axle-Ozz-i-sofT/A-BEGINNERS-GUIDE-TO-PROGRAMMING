//------------------------------------------------------------------------------
// Name:        C99 to C17 Comments example
// Purpose:     Illustrate Comments with a simple math Function
// Requires:    <stdio.h>, <stdlib.h>
// Usage:       Read
//
//------------------------------------------------------------------------------
// Author:      Axle
// Copyright:   (c) Axle 2021
// Licence:     MIT No Attribution
// Created:     21/12/2021
// Modified:
// Versioning   ("MAJOR.MINOR.PATCH") (Semantic Versioning 2.0.0)
// Script V:    0.0.2 (alpha)
//              Alpha is functional, but missing features; Beta Is functional
//              but may still contain bugs; Release is fully functional and
//              bug free)
// Encoding:    ANSI
// Compiler V:  TDM-GCC 9.2.0 32/64-bit
// OS Scope:    (Windows)
// UI Scope:    (CLI)
//------------------------------------------------------------------------------
// NOTES:
//
//
//------------------------------------------------------------------------------

// ---> START Library Imports
#include <stdio.h>
#include <stdlib.h>
// END Library Imports <---
// ---> START Global Defines
double SquareRoot(double Radicand);
// END Global Defines <---

int main(int argc, char *argv[])
    {
    double User_Number = 0;

    printf("Enter a number to find the Square Root: ");
    scanf("%lf", &User_Number);  // "%lf" means double float

    // I have placed the Function call inside of the print statement.
    // This eliminates the need to create a variable to use as a "Temp Buffer"
    // to hold the return value before sending it to the print statement :)
    printf("Square root of %lf is %lf\n", User_Number, SquareRoot(User_Number));

    return 0;
    }

// ---> START Function Block

/*-Block-Comment----------------------------------------------------------------
Babylonian method to find the square root of a number.
SEE: Wikipedia - Methods of computing square roots

This method uses "approximation" to zone in on the result.
It starts with a Low approximation and a High approximation.
It slides both approximations until they (almost) meet at an approximation
of the square root to Precision decimal places.

Note: The calculation of Low and High may be infinite and never actually meet.
This is OK as we have enough accuracy for most purposes.
*/
double SquareRoot(double Radicand)
    {
    double High = Radicand;
    double Low = 1;
    double Precision = 0.000001;  // Decides the accuracy level (double float)

    while((High - Low) > Precision)  // Keep sliding until we reach "Precision".
        {  // v Get the average of Low + High for our new High Value.
        High = (Low + High)/2;
        Low = Radicand/High;  // Get the divisor for our new low value.
        }
    // Continue looping until Low and High are as close as "Precision" allows.

    return High;  // The value of High is our best approximation to return
    }

// END Function Block <---
