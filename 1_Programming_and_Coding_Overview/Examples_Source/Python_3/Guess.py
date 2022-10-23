#-------------------------------------------------------------------------------
# Name:        guess.py
# Purpose:     Guess a number
#
# Platform:    REPL, Win64, Ubuntu64
#
# Author:      Axle
#
# Created:     18/12/2021
# Updated:     19/02/2022
# Copyright:   (c) Axle 2021
# Licence:     MIT No Attribution
#-------------------------------------------------------------------------------

# Standard headers (Modules) are included by default

def main():  # Main procedure
    Number_To_Guess: int = 8  # Create our variables.
    User_Guess: int  # Create our variables.

    print("Hello User... Can you guess my number?")
    print("Enter a number between 1 and 10")

    while 1:  # Loop forever or until a 'Break" statement is reached.
        User_Guess = int(input("Enter your guess: "))  # Get the user input from the console.

        if (User_Guess < 1) or (User_Guess > 10):  # Conditional statement using Logical or.
            print("Not a number between 1 and 10!")
        elif (User_Guess < Number_To_Guess):  # Conditional statement.
            print("A little higher...")
        elif (User_Guess > Number_To_Guess):  # Conditional statement.
            print("A little lower...")
        else:  # If nothing else found
            break  # Breaks out of the while loop (Logically the same as = 8).

    print("Congratulation! You guessed the number.")

    input("press [Enter] to continue...")  # Pause the program until a key is pressed.
    return None

if __name__ == '__main__':
    main()
