#-------------------------------------------------------------------------------
# Name:        Operators.py
# Purpose:
#
# Platform:    REPL, Win64, Ubuntu64
#
# Author:      Axle
# Created:     20/12/2021
# Updated:     19/02/2022
# Copyright:   (c) Axle 2021
# Licence:     MIT No Attribution
#-------------------------------------------------------------------------------

def main():
    num1: int = 4  # Assignment Operator
    num2: int = 2  # Assignment Operator

    num1 = (int(num1) + int(num2)) / 2  # Assignment '=', Arithmetic '*' and '/', 3 = (4 + 2) / 2
    num1 += 2  # Assignment Operator. Same as num1 = num + 2
    num1 -= 2  # Assignment Operator. 4 = 6 - 2
    num2 += 1  # Python 3 has no Increment Operator. Use variable Assignment += 1
    num2 -= 1  # Python 3 has no Decrement Operator. Use variable Assignment -= 1

    if (num1 == num2):  # Equality Operator
        print("num1 and num2 contain the same value.")
    elif (num1 != num2):  # Inequality Operator
        print("num1 and num2 do not contain the same value.")
    else:
        print("The 2 Equality tests above will never allow the program to reach here.")

    if (num1 < num2):  # Relational Operator
        print("num1 is less than num2.")
    elif (num1 > num2):  # Relational Operator
        print("num1 is greater than num2.")
    else:
        print("The only option left is that they must be equal.")


    if ((num1 < 10) and (num2 < 10)): # Logical AND '&&' and Relational Operators '<'
        print("The value of both variables is less than 10.")
    elif ((num1 > 2) or (num2 > 2)): # Logical OR '||' and Relational Operators '<'
        print("The value of at least 1 of the variables is greater than 2.")
    else:
        print("Neither of the above tests were True and no decision was made.")


    if ((num1 * num2) < 10): # Compound Arithmetic and relational expression inside of an if statement
        print("The product of num1 and num2 is less than 10.")


if __name__ == '__main__':
    main()
