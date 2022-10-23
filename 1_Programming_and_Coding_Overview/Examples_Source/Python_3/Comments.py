#-------------------------------------------------------------------------------
# Name:         Python 3 Comments example
# Purpose:      Illustrate Comments with a simple math Function
# Requires:
# Usage:        Read
#
#-------------------------------------------------------------------------------
# Author:       Axle
# Copyright:    (c) Axle 2021
# Licence:      MIT No Attribution
# Created:      21/12/2021
# Modified:     19/02/2022
# Versioning    ("MAJOR.MINOR.PATCH") (Semantic Versioning 2.0.0)
# Script V:     0.0.2 (alpha)
#               (Alpha is functional, but missing features; Beta Is functional but
#               may still contain bugs; Release is fully functional and bug free)
# Encoding:     utf-8
# Python V:     V3.9.x
# OS Scope:     See individual Modules (Windows, Unix, Python REPL)
# UI Scope:     See individual Modules (CLI)
#-------------------------------------------------------------------------------
# NOTES:
# Note: labels such as "var: float" or "() -> float" are Variable Type hints,
# and have no effect on the application.
#
#
#-------------------------------------------------------------------------------

## ---> START Library Imports
## END Library Imports <---
## ---> START Global Defines

def main():
    # ---> START Library Imports
    # END Library Imports <---
    User_Number: float

    print("Enter a number to find the Square Root: ", end='')
    User_Number = float(input())

    print("Square root of ", User_Number, " is ", SquareRoot(User_Number))

    input("press [Enter] to continue...")  # pause the program until a key is pressed

    return None

## ---> START Function Block

#-------------------------------------------------------------------------------
# Babylonian method to find the square root of a number.
# SEE: Wikipedia - Methods of computing square roots
#
# This method uses "approximation" to zone in on the result.
# It starts with a Low approximation and a High approximation.
# It slides both approximations until they (almost) meet at an approximation
# of the square root to Precision decimal places.
#
# Note: The calculation of Low and High may be infinite and never actually meet.
# This is OK as we have enough accuracy for most purposes.
def SquareRoot(Radicand: float) -> float:
    High: float = float(Radicand)
    Low: float = 1.000000
    Precision: float = 0.000001  # Decides the accuracy level (double float)

    while((High - Low) > Precision):  # Keep sliding until we reach "Precision".
        High = (Low + High)/2  # Get the average of Low + High for our new High Value.
        Low = Radicand/High  # Get the divisor for our new low value.
        # Continue looping until Low and High are as close as "Precision" allows.

    return High  # The value of High is our best approximation to return

## END Function Block <---
## END Global Defines <---
if __name__ == '__main__':  # Program entry point
    main()

## Script Exit
exit()
