#------------------------------------------------------------------------------
# Name:        Example1.c
# Purpose:     Example
#
# Platform:    Win64, Ubuntu64
#
# Author:      Axle
# Created:     16/12/2021
# Updated:     18/02/2022
# Copyright:   (c) Axle 2021
# Licence:     MIT No Attribution
#------------------------------------------------------------------------------
# Standard headers (Modules) are included by default

# Single line comment
## Highlighted comment

def main(): # Main procedure
    user_name: str  # Create a string variable.
    num1: int; num2: int; return_value: int # Create Integer Variables.

    print("Please enter your name: ", end='')  # Print text to console.
    user_name = str(input())  # Get user input
    print("Hello ", user_name, end=' ')
    print("Can you please enter 2 numbers:")
    num1 = int(input("?"))
    num2 = int(input("?"))
    return_value = sums(num1, num2)  # Call our function and send the 2 Integer variables to it.
    print(user_name, " the addition of two numbers is : ", return_value)

    input("press [Enter] to continue...")  # Pause the program until a key is pressed.
    return None

def sums(num1, num2):  # Function to add 2 numbers.
    num3: int
    num3 = num1 + num2
    return int(num3)  # Return the results to the calling statement.

if __name__ == '__main__':
    main()
