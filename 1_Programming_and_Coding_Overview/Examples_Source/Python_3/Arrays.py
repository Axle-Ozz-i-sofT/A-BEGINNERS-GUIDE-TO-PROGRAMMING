#-------------------------------------------------------------------------------
# Name:        Arrays
# Purpose:     Example
#
# Platform:    REPL, Win64, Ubuntu64
#
# Author:      Axle
# Created:     03/02/2022
# Updated:     19/02/2022
# Copyright:   (c) Axle 2022
# Licence:     MIT No Attribution
#-------------------------------------------------------------------------------

ROW = 5
COLUMN = 6

def main():

    Rows = ROW
    Columns = COLUMN
    TempBuffer = ""  # array of characters, aka String.

    # defining a 2D array of array[Rows][Columns], initialised to empty.
    Table = [[None]* COLUMN for _ in range(ROW)]  # initialised to null
    # static char Table(5, 6)  # Actual values of above.

    Row_y = 0  # Counter for rows.
    Col_x = 0  # Counter for columns.
    Offset = 1  # Origin zero offset (0|1)
    # Fill the array with some data.
    for Row_y in range(0,  Rows):  # Count through each row.
        for Col_x in range(0,  Columns):  # Count through each column.
            # Build a string with values for each cell. The offset starts
            # the Row and columns count at 1 instead of 0.
            TempBuffer = "[R:" + str(Row_y + Offset) + ",C:" + str(Col_x + Offset) + "] "
            # Copy the string to the cell position.
            Table[Row_y][Col_x] = TempBuffer

    # Print the array to the console.
    for Row_y in range(0,  Rows):  # Count through each row.
        for Col_x in range(0,  Columns):  # Count through each column.
            print(Table[Row_y][Col_x], end='')  # No line breaks.
        print("")  # Print a line break to start next row.

    input("Press [Enter] to exit.")
    return None
# END main <---

if __name__ == '__main__':
    main()
