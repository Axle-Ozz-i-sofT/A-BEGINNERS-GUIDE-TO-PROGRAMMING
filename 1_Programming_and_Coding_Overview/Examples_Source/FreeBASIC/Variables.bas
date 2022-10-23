'-------------------------------------------------------------------------------
' Name:        Variables.bas
' Purpose:
'
' Platform:    Win64, Ubuntu64
'
' Author:      Axle
' Created:     31/01/2022
' Updated:     19/02/2022
' Copyright:   (c) Axle 2022
' Licence:     MIT No Attribution
'-------------------------------------------------------------------------------

' ---> declare functions
Declare Function main_procedure() As Integer
Declare Sub test_const()
Declare Sub test_local_vs_global()
Declare Sub test_variable()

' ---> MACROS
#define MAXVALUE 128

' ---> Global declare & defines
Const As Integer config_max_str_len = 32
Dim Shared As String my_lstring
my_lstring = "Global string"  ' Note: String is a keyword and can't be used as a variable.
Dim Shared As Integer my_variable_integer = 6  ' an Integer variable
Dim Shared my_variable_string As String  ' a String variable.

main_procedure()

Function main_procedure() As Integer  ' Main procedure
    
    ' ---> Local declare & defines
    Dim As String my_lstring = "Local string"  ' pointer to a literal.
    Dim As Integer  my_variable_integer = 3  ' an Integer variable
    Dim my_variable_string As String  ' a String variable.
    Dim As String local_lstring = "Local to main_procedure()"
    
    ' using a simple MACRO
    Print "The Maximum value allowed = "; MAXVALUE
    Print ""
    
    ' ---> Tests
    ' config_max_str_len += 1 ' error 119: Cannot modify a constant, before '+'
    Print "' Test our Global const in local scope"
    Print "main_procedure(), config_max_str_len = "; config_max_str_len  ' cannot be altered
    test_const()  ' test in a different local scope.
    Print ""
    
    ' The Local overrides global variables!
    ' Although they have the same name they are different variables.
    ' Global variables should be used with great care and be made up of unique names.
    Print "' The Local literal variable declaration overrides the Global variable."
    Print "main_procedure(), my_lstring = "; my_lstring  ' Local overrides global!
    Print "main_procedure(), my_variable_integer = "; my_variable_integer  ' Local overrides global!
    Print "main_procedure(), local_lstring = "; local_lstring
    test_local_vs_global()  ' test in a different local scope.
    Print ""
    
    ' The Local overrides global variables!
    Print "' The literal is copied to the local variable in main()..."
    my_variable_string = "my_variable_string Local to main()"
    Print "main_procedure(), my_variable_string = "; my_variable_string
    test_variable()  ' test in a different local scope.
    ' After changing the value of the Global variable...
    Print "' The variable Local to main() has not altered."
    Print "main_procedure(), my_variable_string = "; my_variable_string
    Print ""
    
    Print "Press any key to continue..."
    Sleep  ' Sleep until a key is pressed
    Return 0
End Function

Sub test_const()
    ' cannot be altered
    Print "test_const(), config_max_str_len = "; config_max_str_len
End Sub

Sub test_local_vs_global()
    Dim As String local_lstring = "Local to test_local_vs_global()"
    ' We have not blocked the global definition with the same local name.
    Print "' and to the Global variable in test_variable()."
    Print "test_lstring(), my_lstring = "; my_lstring
    Print "main_procedure(), my_variable_integer = "; my_variable_integer
    Print "main_procedure(), local_lstring = "; local_lstring
End Sub

Sub test_variable()
    Print "test_variable(), my_variable_string = "; my_variable_string
    '  the following copies the "string literal" into the Global variable.
    ' the "my_variable_string Global" in the following function is a
    ' true string literal as it has no variable associated with it until it
    ' is copied into my_variable_string.
    my_variable_string = "my_variable_string Global"
    Print "test_variable(), my_variable_string = "; my_variable_string
End Sub
