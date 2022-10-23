'-------------------------------------------------------------------------------
' Name:        Statements(ST), expressions(EX) and Procedures(PR)
' Purpose:     Example
'
' Platform:    Win64, Ubuntu64
'
' Author:      Axle
' Created:     31/01/2022
' Updated:     19/02/2022
' Copyright:   (c) Axle 2022
' Licence:     MIT No Attribution
'-------------------------------------------------------------------------------

' You can change the radius and pen size below.
Const RADIUS = 10  ' Do NOT exceed 20! 80 char MAX Console width.
Const PEN = 10  ' About 1:1 with the Radius to get single char.
Declare Function main_procedure() As Integer

main_procedure()

Function main_procedure() As Integer  ' Main procedure
    
    ' Defining a 2D array of array[Rows][Columns], initialised to empty.
    ' Radius *2 for circumference, *2 for double width character "O ", +1 for safety = *5
    Dim As String canvas(RADIUS * 5, RADIUS * 5)
    ' Changing the following String characters will change the circle display.
    Dim As String Foreground = "O "  ' EX add space after chr to make circle (MAX 2 chrs)
    Dim As String Background = ". "  ' EX add space after chr to make circle (MAX 2 chrs)
    
    Const As Integer Radius = RADIUS
    Const As Integer Tolerance = PEN  ' EX Larger number will create a wider drawing pen
    
    Dim As Integer Row_yy = 0  ' EX counters for array position
    Dim As Integer Col_xx = 0  ' EX counters for array position
    Dim As Integer Row_y  ' ST Counter range of circumference. -Radius to +radius
    Dim As Integer Col_x  ' ST Counter range of circumference. -Radius to +radius
    Dim As Integer equation
    For Row_y = -Radius To Radius Step 1  ' PR + EX
        Col_xx =0  ' EX reset the column counter for each row.
        For Col_x = -Radius To Radius Step 1  ' PR + EX
            ' Test if it is at the radius
            equation = Row_y*Row_y + Col_x*Col_x - Radius*Radius  ' EX
            If (Abs(equation) < Tolerance) Then 'ST
                'Print Foreground;
                canvas(Row_yy, Col_xx) = Left(Foreground, 2)  ' ST
            Else
                'Print Background;
                canvas(Row_yy, Col_xx) = Left(Background, 2)  ' ST
            End If
            
            'Print canvas(Row_yy, Col_xx);  ' ST
            Col_xx += 1  ' EX Increment Columns
        Next Col_x
        
        Row_yy += 1  ' EX Increment Rows
        'Print ""  ' ST
    Next Row_y
    
    ' Commenting out the loop below, and then enabling the
    ' printf statements above will print the circle directly
    ' rather than populating the array first and printing later :)
    ' Comment out (b) and enable (c) to print directly from the array/list.
    ' Or comment out (a)(c) and enable (b) to print directly without the array/list.
    
    ' Print the rows and columns from our canvas array.
    ' The last increment of Row_yy and Col_xx from the loop above
    ' contains the correct lengths for the following loop.
    For Row_y = 0 To Row_yy Step 1  ' PR + EX
        For Col_x = 0 To Col_xx Step 1  ' PR + EX
            Print canvas(Row_y, Col_x);  ' ST
        Next Col_x
        Print ""  ' ST
    Next  Row_y
    
    Print "Press any key to continue..."
    Sleep  ' Sleep until a key is pressed
    Return 0  ' EX
End Function  ' END PR
