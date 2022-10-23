'-------------------------------------------------------------------------------
' Name:         FreeBASIC Comments example
' Purpose:      Illustrate Comments with a simple maths Function
' Requires:
' Usage:        Read
'
'-------------------------------------------------------------------------------
' Author:       Axle
' Copyright:    (c) Axle 2021
' Licence:      MIT No Attribution
' Created:      21/12/2021
' Modified:     19/02/2022
' Versioning    ("MAJOR.MINOR.PATCH") (Semantic Versioning 2.0.0)
' Script V:     0.0.2 (alpha)
'               (Alpha is functional, but missing features; Beta Is functional but
'               may still contain bugs; Release is fully functional and bug free)
' Encoding:     UTF-8
' Compiler V:   FreeBASIC Compiler TDM-GCC 9.2.0 32/64-bit
' OS Scope:     (Windows)
' UI Scope:     (CLI)
'-------------------------------------------------------------------------------
' NOTES:
'
'
'-------------------------------------------------------------------------------

' ---> START Library Imports
' END Library Imports <---
' ---> START Global Defines
Declare Function main_procedure() As Integer
Declare Function SquareRoot(Byval Radicand As Double) As Double
' END Global Defines <---

main_procedure()

Function main_procedure() As Integer
    Dim User_Number As Double
    
    Input "Enter a number to find the Square Root: ", User_Number
    
    Print "Square root of " & User_Number & " is " & SquareRoot(User_Number)
    
    Print "Press any key to continue..."
    Sleep  ' Sleep until a key is pressed
    Return 0
End Function

' ---> START Function Block

'-------------------------------------------------------------------------------
' Babylonian method to find the square root of a number.
' SEE: Wikipedia - Methods of computing square roots
'
' This method uses "approximation" to zone in on the result.
' It starts with a Low approximation and a High approximation.
' It slides both approximations until they (almost) meet at an approximation
' of the square root to Precision decimal places.
'
' Note: The calculation of Low and High may be infinite and never actually meet.
' This is OK as we have enough accuracy for most purposes.
'
Function SquareRoot(Byval Radicand As Double) As Double
    Dim High As Double = Radicand
    Dim Low As Double = 1.000000
    Dim Precision As Double = 0.000001  ' Decides the accuracy level (double float)
    
    While((High - Low) > Precision)  ' Keep sliding until we reach "Precision".
        High = (Low + High)/2  ' Get the average of Low + High for our new High Value.
        Low = Radicand/High  ' Get the divisor for our new low value.
    Wend  ' Continue looping until Low and High are as close as "Precision" allows.
    
    Return High  ' The value of High is our best approximation to return
End Function

' END Function Block <---
