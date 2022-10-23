'-------------------------------------------------------------------------------
' Name:        Example1.bas
' Purpose:     Example
'
' Platform:    Win64, Ubuntu64
'
' Author:      Axle
'
' Created:     16/12/2021
' Updated:      18/02/2022
' Copyright:   (c) Axle 2021
' Licence:     MIT No Attribution
'-------------------------------------------------------------------------------

Rem FreeBASIC hello World
' Single line comment
/' Multiline
comment '/

Declare Function main_procedure() As Integer
Declare Function sums(num1 As Integer, num2 As Integer) As Integer

main_procedure()

Function main_procedure() As Integer  ' main procedure
    Dim user_name As String  ' Create a string variable.
    Dim As Integer num1, num2, return_value  ' Create Integer Variables.
    
    Input "Please enter your name: ", user_name ' Print text to console, and get user input.
    Print "Hello " & user_name & ".";
    Print " Can you please enter 2 numbers:"
    Input num1  ' Get user input.
    Input num2  ' Get user input.
    return_value = sums(num1, num2)  ' Call our function and send the 2 Integer variables to it.
    Print user_name & " the addition of two numbers is : " & return_value
    
    Sleep  ' Sleep until a key is pressed.
    Return 0
End Function

' Function to add 2 numbers.
Function sums(num1 As Integer, num2 As Integer) As Integer
    Dim num3 As Integer
    num3 = num1 + num2
    Return num3  ' Return the results to the calling statement.
End Function
