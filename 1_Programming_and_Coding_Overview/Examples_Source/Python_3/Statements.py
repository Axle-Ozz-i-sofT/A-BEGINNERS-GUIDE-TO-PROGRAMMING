#-------------------------------------------------------------------------------
# Name:        Statements(ST), expressions(EX) and Procedures(PR)
# Purpose:     Example
#
# Platform:    REPL, Win64, Ubuntu64
#
# Author:      Axle
# Created:     31/01/2022
# Updated:     19/02/2022
# Copyright:   (c) Axle 2022
# Licence:     MIT No Attribution
#-------------------------------------------------------------------------------

# You can change the radius and pen size below.
RADIUS = 10  # Do NOT exceed 20! 80 char MAX Console width.
PEN = 10  # About 1:1 with the Radius to get single char.

def main():  # Main procedure

    # Defining a 2D array of array[Rows][Columns], initialised to empty.
    # Radius *2 for circumference, *2 for double width character "O ", +1 for safety = *5
    canvas = [[None]* RADIUS * 5 for _ in range(RADIUS * 5)]
    # Changing the following String characters will change the circle display.
    Foreground = "O "  # EX add space after chr to make circle (MAX 2 chrs)
    Background = ". "  # EX add space after chr to make circle (MAX 2 chrs)

    Radius = RADIUS
    Tolerance = PEN  # EX Larger number will create a wider drawing pen

    Row_yy = 0  # EX counters for array position
    Col_xx = 0  # EX counters for array position
    Row_y = 0  # ST Counter range of circumference. -Radius to +radius
    Col_x = 0  # ST Counter range of circumference. -Radius to +radius
    equation = 0  # EX
    for Row_y in range(-Radius,  Radius +1):  # PR + EX
        Col_xx =0  # EX reset the column counter for each row.
        for Col_x in range(-Radius,  Radius +1):  # PR + EX
            # Test if it is at the radius
            equation = Row_y*Row_y + Col_x*Col_x - Radius*Radius  # EX
            if (abs(equation) < Tolerance):  # ST
                canvas[Row_yy][Col_xx] = Foreground[:2]  # ST
            else:
                canvas[Row_yy][Col_xx] = Background[:2]  # ST

            #print(canvas[Row_yy][Col_xx], end="")  # ST
            Col_xx += 1  # EX Increment Columns

        Row_yy += 1  # EX Increment Rows
        #print("")  # ST

    # Commenting out the loop below, and then enabling the
    # printf statements above will print the circle directly
    # rather than populating the array first and printing later :)
    # Comment out (b) and enable (c) to print directly from the array/list.
    # Or comment out (a)(c) and enable (b) to print directly without the array/list.

    # Print the rows and columns from our canvas array.
    # The last increment of Row_yy and Col_xx from the loop above
    # contains the correct lengths for the following loop.
    for Row_y in range(0,  Row_yy):  # PR + EX
        for Col_x in range(0,  Col_xx):  # PR + EX
            print(canvas[Row_y][Col_x], end="")  # ST

        print("")  # ST

    input("Press [Enter] to exit.")
    return None  # EX
    # END PR

if __name__ == '__main__':
    main()
