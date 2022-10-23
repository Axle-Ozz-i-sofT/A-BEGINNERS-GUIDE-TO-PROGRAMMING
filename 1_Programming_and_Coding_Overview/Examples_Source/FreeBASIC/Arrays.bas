'-------------------------------------------------------------------------------
' Name:        Arrays
' Purpose:     Example
'
' Platform:    Win64, Ubuntu64
'
' Author:      Axle
' Created:     03/02/2022
' Updated:     19/02/2022
' Copyright:   (c) Axle 2022
' Licence:     MIT No Attribution
'-------------------------------------------------------------------------------

Const ROW = 5
Const COLUMN = 5

Declare Function main_procedure() As Integer

main_procedure()

Function main_procedure() As Integer  ' ---> Main procedure
    
    Dim As Integer Rows = ROW
    Dim As Integer Columns = COLUMN
    Dim As String TempBuffer = ""  ' array of characters, aka String.
    
    ' defining a 2D array of array[Rows][Columns], initialised to empty.
    Dim As String Table(ROW, COLUMN)  ' initialised to null
    ' static char Table(5, 5)  '  Actual values of above.
    
    Dim As Integer Row_y  ' Counter for rows.
    Dim As Integer Col_x  ' Counter for columns.
    Dim As Integer Offset = 1  '  Origin zero offset (0|1)
    ' Fill the array with some data.
    For Row_y = 0 To Rows -1 Step 1  ' Count through each row.
        For Col_x = 0 To Columns -1 Step 1  ' Count through each column.
            ' Build a string with values for each cell. The offset starts
            ' the Row and columns count at 1 instead of 0.
            TempBuffer = "[R:" & (Row_y + Offset) & ",C:" & (Col_x + Offset) & "] "
            ' Copy the string to the cell position.
            Table(Row_y, Col_x) = TempBuffer
        Next Col_x
    Next Row_y
    
    ' Print the array to the console.
    For Row_y = 0 To Rows -1 Step 1  ' Count through each row.
        For Col_x = 0 To Columns -1 Step 1  ' Count through each column.
            Print Table(Row_y, Col_x);  ' No line breaks.
        Next Col_x
        Print ""  ' Print a line break to start next row.
    Next Row_y
    
    Print "Press any key to continue..."
    Sleep  ' Sleep until a key is pressed
    Return 0
End Function  ' END main_procedure <---
