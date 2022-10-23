'-------------------------------------------------------------------------------
' Name:        error_check.bas
' Purpose:     Examples
'
' Platform:    Win64, Ubuntu64
'
' Author:      Axle
' Created:     22/02/2022
' Updated:     19/05/2022
' Copyright:   (c) Axle 2022
' Licence:     MIT No Attribution
'-------------------------------------------------------------------------------
Declare Function main_procedure() As Integer
Declare Function Con_Pause() As Integer  ' GetKey Version
main_procedure()

Function main_procedure() As Integer  ' Main procedure
    ' the MAX size of a String type in FB is 2GiB and based upon "Dynamic Memory".
    ' We CAN set a "Static Memmory" size with MyString(128) but is generally
    ' not recomended. https://documentation.help/FreeBASIC/TblVarTypes.html
    Dim As String Input_Buffer = ""
    Dim As UInteger Zero_Passlength = 0  ' No Password entered
    Dim As UInteger Min_Passlength = 6  ' Pass Min length limit
    Dim As UInteger Max_Passlength = 12  ' Pass Max length limit
    Dim As UInteger Max_Attempts = 3  ' Attempts limit
    Dim As UInteger Attempts_Counter = 0  ' Attempts count
    Dim As String Opt_Out = ""  ' Opt out question y/n

    ' Test that the input data is within the expected limits of 6 to 12.
    ' Loops until the user enters the correct data. In real life we would
    ' also need an opt out after so many tries.
    ' This may appear like a lot of extra code, but it is necessary to keep a
    ' level of safety.
    ' If you remove all of my "Comment explanations" you will find it is not
    ' that much extra code :)
    ' C++, BASIC and Python have far more built in buffer
    ' safeguards than C, but it is still up to the coder to ensure that any
    ' external input is within the expected range of data.
    While(Len(Input_Buffer) < Min_Passlength) _
        Or (Len(Input_Buffer) > Max_Passlength)
        ' The next conditional check is part of an unwind to break out of deeply
        ' nested loops. Although there are other methods to do this as well, I 
        ' wanted to keep a simple method arcoss all 3 languages.
        ' The above "While" conditional test should break the loop if the Password
        ' is the correct length without the folowing test, but I wanted to show
        ' an unwind method just the same :)
        If(Opt_Out = "Y") or (Opt_Out = "y") Then
            Exit While  ' Quit asking for a name and break out of the loop "Step 2".
            ' If we wanted to reuse the Opt_Out variable again later I suggest
            ' uncommenting the following line.
            'Opt_Out = ""  ' Reset Y/N the response variable.
        else
            ' Note that FreeBASIC has built in limit for the input string length
            ' so we only have to check the the return is withing the range and
            ' data type expected.
            Print "Please enter your password."
            Input " Between 6 to 12 letters:", Input_Buffer
            ' The second part of our test is to see if the data is within the
            ' range that is expected.
            If(Len(Input_Buffer) = Zero_Passlength) Then  ' 0 length string.
                Print "You did not enter your password..."
                Attempts_Counter += 1
            ElseIf((Len(Input_Buffer) > Zero_Passlength) _
                And (Len(Input_Buffer) < Min_Passlength)) Then  ' String shorter than 6.
                Print "The password you entered is too short..."
                Attempts_Counter += 1
            ElseIf(Len(Input_Buffer) > Max_Passlength) Then  ' String longer than 12.
                Print "The password you entered is too long..."
                Attempts_Counter += 1
            Else  ' String is withing range. Success.
                Print "Your Password is " & Input_Buffer
            End If

            ' Limit the number of attempts and offer an opt out so that the
            ' user is not caught in an endless loop if they decide to not
            ' enter a name.
            If(Attempts_Counter >= Max_Attempts) Then
                ' keep asking in the loop untill we get a valid Y/N response.
                While(Opt_Out <> "y") And (Opt_Out <> "Y")
                    Print !"\nSorry you have reached the maximum number of tries!\n"
                    Print "Would you like to quit? (Y/N):";

                    Input Opt_Out
                    If(Opt_Out = "y") Or (Opt_Out = "Y") Then
                        Exit While  ' Quit asking for a name and break out "step 1".
                    ElseIf(Opt_Out = "n") Or (Opt_Out = "N") Then
                        Attempts_Counter = 0  ' reset the attempts counter
                        Opt_Out = ""  ' reset the opt out counter (3 more tries)
                        Exit While
                    Else
                        ' ask again until we get a Y/N response.
                        Print "Invalid response!"
                    End If
                WEnd
            End If
        End If
    WEnd

    Print ""
    Print "File read error test 1"
    ' Checking the error return of a function. This is always recomended when
    ' the function handles data from an unknown source aka anything outside
    ' of the source code of you application. This icludes "User Inputs",
    ' "Data from a file or database", "Information from the web",
    ' "Communication and data transfers to other apps" ++.
    ' We can never guarentee the existance of data outside of our application
    ' or if it will be the data that we have expected.
    '
    ' Description of:
    ' result = Open(filename For Input [encoding_type] [lock_type] As [#]filenumber)
    ' Description
    ' https://documentation.help/FreeBASIC/KeyPgOpen.html
    ' Return Value: (<- this is what you need to look for.)
    ' In the first usage, Open returns zero (0) on success and a non-zero error
    ' code otherwise.
    ' https://documentation.help/FreeBASIC/TblRuntimeErrors.html
    ' 2 - File not found signal
    ' If it fails to open because the file does not exist for example, we need
    ' to handle the error and either create the file, or tell the user that the
    ' file could not be found, or any other number of options that are apropriate
    ' to the context of your application. Don't ever let and error be passed to
    ' your user with the horrible "Ding Sound" and the
    ' "This program has terminated unexpectedly!" warning in production code. :)
    Const filename As String = "filename.txt"  ' a dummy file name.
    Dim As Integer errorvalue = 0  ' Variable to store retured error codes.
    ' In some instances error codes can be retreived with the MACRO Err .
    '==> Open file for text read ops.
    Dim As Long Fp
    Fp = Freefile()  ' Similat to a FILE pointer in C.
    ' in this example "filename.txt" does not exist so Open( ...) will
    ' send an error as the return. A return of 0 means that there were no
    ' errors in opening the file. Any other value indicates an error.
    ' Open file for text read ops.
    errorvalue = Open(filename, For Input, As #Fp)
    If (errorvalue <> 0) Then  'If (Err <> 0) Then  ' Alternative method
        Print "Err = " & Err  ' the use of Err must come before any Print.
        Print "errorvalue = " & errorvalue
        Print "ERROR! Cannot open file " & filename
        Con_Pause() ' Wait until a key is pressed
        ' Do some error handling tasks to deal with why the file does not exist.
        ' Maybe you need to create the file first?
        ' If this is a function that you have created you may wish to "return"
        ' some usefull information to the function call.
        'Return -1  '-1 Indicates an error on some platforms.
    Else
        ' No errors, so do some file read operations.
        Close #Fp  ' Always close files when finished, always.
    End If

    Print ""
    Print "File read error test 2"
    ' Alternative method.
    ' This is also a common error check method in FreeBASIC
    ' Err will report the last runtime error. You must check it, or store the
    ' RetError = Err before any other internal function are called as the error
    ' value will be reset.
    ' Open file for text read ops.
    If Open(filename, For Input, As #Fp) <> 0 Then
        errorvalue = Err  ' Savee a copy of Err as it will be reset with next Print.
        Print "Err = " & Err  ' The use of Err must come before any Print.
        Print "ERROR! Cannot open file " & filename
        Print "Err of last Print statement = " & Err
        Print "errorvalue = " & errorvalue
        Con_Pause() ' Wait until a key is pressed
        ' Do some error handling tasks to deal with why the file does not exist.
        ' Maybe you need to create the file first?
        ' If this is a function that you have created you may wish to "return"
        ' some usefull information to the function call.
        'Return -1  '-1 Indicates an error on some platforms.
    Else
        ' No errors, so do some file read operations.
        Close #Fp  ' Always close files when finished, always.
    End If

    ' You will often see the folowing format for opening a file. I recomend
    ' only using the function version as above Open("file.ext", For Input, As #f)
    ' As this method is for QBasic and will throw a runtime error if not
    ' managed and compiled with the correct compiler switches.
    ' There are many different ways to open files for reading and writting in
    ' FreeBASIC.
    ''Dim f As Integer
    ''f = FreeFile
    ''Open "file.ext" For Input As #f
    ''If Err>0 Then Print "Error opening the file":End
    
    Close  ' Will close ALL open file handles (pointers).
    
    ' My Console Pause function wrapper uses GetKey function which is safe.
    Con_Pause()
    Return 0
End Function  ' END main_procedure <---

' Console Pause (GetKey version)
' GetKey returns the first key pressed and entered into the keyboard buffer.
' It does not wait for [Return]. The Snancode (Key press) is removed from the
' stdin buffer unlike C where we have to manually loop through the buffer and
' discard additional characters.
' This is just a simple wrapper for GetKey with a print statement. It is
' benificial over time to create your own personal function library for common
' task as it removes the need to type in the 3 lines of code below every time
' you need a pause. If you look at my Input() function from the C example you
' can see that I have reduced some 15 lines of code to a single function call.
' This forms the basic principles for the creation of "Modular Code" and code
' libraries. We can use a library function instead of repeadedly writting
' common ("Boiler Plate") code in our main() source.
Function Con_Pause() As Integer
    Dim As Long dummy
    Print !"\nPress any key to continue..."
    dummy = Getkey
    Return 0
End Function
