'-------------------------------------------------------------------------------
' Name:        guess.bas
' Purpose:     FreeBASIC Guess a number
'
' Platform:    Win64, Ubuntu64
'
' Author:      Axle
'
' Created:     18/12/2021
' Updated:     19/02/2022
' Copyright:   (c) Axle 2021
' Licence:     MIT No Attribution
'-------------------------------------------------------------------------------

Declare Function main_procedure() As Integer

main_procedure()

Function main_procedure() As Integer  ' Main procedure
    Dim Number_To_Guess As Integer = 8  ' Create our variables.
    Dim User_Guess As Integer  ' Create our variables.
    
    Print "Hello User... Can you guess my number?"
    Print "please enter a number between 1 and 10"
    
    While (1)  ' Loop forever or until an "Exit While" statement is reached.
        Input "enter your guess:",User_Guess  ' Get the user input from the console.
        
        If (User_Guess < 1) Or (User_Guess > 10) Then  ' Conditional statement using Logical "Or".
            Print "Not a number between 1 and 10"
        Elseif (User_Guess < Number_To_Guess) Then  ' Conditional statement.
            Print "A little higher..."
        Elseif (User_Guess > Number_To_Guess) Then  ' Conditional statement.
            Print "A little Lower..."
        Else  ' If nothing else found.
            Exit While  '  Breaks out of the while loop (Logically the same as = 8).
        End If
        
    Wend
    
    Print "Congratulations! you guessed the correct answer."
    
    
    Print "Press any key to continue..."
    Sleep  ' Sleep until a key is pressed
    Return 0
End Function
