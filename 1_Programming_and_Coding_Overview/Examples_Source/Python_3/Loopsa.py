#-------------------------------------------------------------------------------
# Name:        Loops.py
# Purpose:     Example
#
# Platform:    REPL, Win64, Ubuntu64
#
# Author:      Axle
# Created:     15/02/2022
# Updated:     19/02/2022
# Copyright:   (c) Axle 2022
# Licence:     MIT No Attribution
#-------------------------------------------------------------------------------

def main():

    # A simple example of nested loops using for and while.
    num = 5 #  length of loops.

    # Set variables for enumerating (Counting through) the loops.
    a = 0
    b = 0
    c = 0
    # Loop level 1.
    for a in range(0, num +1):
        print("a" + str(a) + ":")
        # Loop level 2 (nested).
        for b in range(0, a +1):
            print("b" + str(b) + ":", end="")
            # Loop level 3 (nested).
            for c in range(0, b +1):
                print(str(c) + ",", end="")
            print("")
        print("")

    Con_Pause()

    # Reset our counters to zero.
    a = 0
    b = 0
    c = 0
    # Loop level 1.
    while(a <= num):
        print("a" + str(a) + ":")
        # Loop level 2 (nested).
        while(b <= a):
            print("b" + str(b) + ":", end="")
            # Loop level 3 (nested).
            while(c <= b):
                print(str(c) + ",", end="")
                c+=1
            print("")
            c = 0
            b+=1
        print("")
        b = 0
        a+=1

    Con_Pause()

    # How loops are created in assembly.
    # Intel Asm syntax
    # jmp label: Is the equivalent of Goto LABEL:

    #       mov eax, 0      ; store var num in ecx register
    #       mov ebx, num
    #   top:                ; Label
    #       cmp eax, ebx    ; Test if eax == num
    #       je bottom       ; loop exit when while condition True
    #       BODY            ; ... Code inside the loop
    #       inc eax         ; Increment the counter +1
    #       jmp top         ; Jump to label top:
    #   bottom:             ; Label

    a = 0  #  reset counter a to 0.
    # The while loop will print to 4 as the truth test is before the printf().
    # This is the correct way to implement a loop by using while or for.
    while(1):
        if a == num:
            break
        print(str(a))
        a+=1

    Con_Pause()
    return None

def Con_Pause():
    dummy = ""
    dummy = input("Press [Enter] key to continue...")

    return None

if __name__ == '__main__':
    main()
