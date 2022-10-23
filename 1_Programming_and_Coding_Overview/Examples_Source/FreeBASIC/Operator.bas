'-------------------------------------------------------------------------------
' Name:        Operators.bas
' Purpose:
'
' Platform:    Win64, Ubuntu64
'
' Author:      Axle
' Created:     20/12/2021
' Updated:     19/02/2022
' Copyright:   (c) Axle 2021
' Licence:     MIT No Attribution
'-------------------------------------------------------------------------------

Declare Function main_procedure() As Integer

main_procedure()

Function main_procedure() As Integer  ' Main procedure
    Dim num1 As Integer = 4  ' Assignment Operator
    Dim num2 As Integer = 2  ' Assignment Operator
    
    
    num1 = (num1 + num2) / 2  ' Assignment '=', Arithmetic '*' and '/', 3 = (4 + 2) / 2
    num1 += 2  ' Assignment Operator. Same as num1 = num + 2
    num1 -= 2  ' Assignment Operator. 4 = 6 - 2
    num2 += 1  ' FreeBASIC has no Increment Operator. Use variable Assignment += 1
    num2 -= 1  ' FreeBASIC has no Decrement Operator. Use variable Assignment -= 1
    
    If (num1 = num2) Then  ' Equality Operator
        Print "num1 and num2 contain the same value."
    Elseif (num1 <> num2) Then  ' Inequality Operator
        Print "num1 and num2 do not contain the same value."
    Else
        Print "The 2 Equality tests above will never allow the program to reach here."
    End If
    
    If (num1 < num2) Then  ' Relational Operator
        Print "num1 is less than num2."
    Elseif (num1 > num2) Then  ' Relational Operator
        Print "num1 is greater than num2."
    Else
        Print "The only option left is that they must be equal."
    End If
    
    If ((num1 < 10) And (num2 < 10)) Then  ' Logical AND '&&' and Relational Operators '<'
        Print "The value of both variables is less than 10."
    Elseif ((num1 > 2) Or (num2 > 2)) Then  ' Logical OR '||' and Relational Operators '<'
        Print "The value of at least 1 of the variables is greater than 2."
    Else
        Print "Neither of the above tests were True and no decision was made."
    End If
    
    If ((num1 * num2) < 10) Then  ' Compound Arithmetic and relational expression inside of an if statement
        Print "The product of num1 and num2 is less than 10."
    End If
    
    Print "Press any key to continue..."
    Sleep  ' Sleep until a key is pressed
    Return 0
End Function
