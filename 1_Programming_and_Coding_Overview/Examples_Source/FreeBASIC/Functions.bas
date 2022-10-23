'-------------------------------------------------------------------------------
' Name:        Functions.bas
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
' Test if Windows or Unix OS
#ifdef __FB_WIN32__
#include once "windows.bi"
#define OS_Windows 1  ' 1 = True (aka Bool)
#define OS_Unix 0
#endif

#ifdef __FB_UNIX__'__FB_LINUX__
' TODO
#define OS_Unix 1
#define OS_Windows 0  ' 0 = False (aka Bool)
#endif

' Define extra functions so we can place them at the bottom of the page.
Declare Function main_procedure() As Integer
Declare Function Menu_Routine() As Integer
Declare Function Stick_Animation() As Integer
Declare Function File_Write_Example() As Integer
Declare Function File_Read_Example1() As Integer
Declare Function File_Read_Example2() As Integer
Declare Function Mandelbrot_Fractals_Console_ASCII() As Integer


Declare Function Sys_Sound() As Integer
Declare Function DebugMsg(Byref title As String, Byref msg As String) As Integer
Declare Sub Clear_Stdin()
Declare Function Con_Clear() As Integer
Declare Function Con_Pause() As Integer  ' GetKey Version
Declare Function Show_Time_Date() As Integer
Declare Function show_time() As Integer
Declare Function show_date() As Integer

main_procedure()

Function main_procedure() As Integer  ' Main procedure

    Menu_Routine()

    Con_Pause() ' DEBUG Pause
    Return 0
End Function  ' END main_procedure <---

' --> START Application Functions

Function Menu_Routine() As Integer
    While(1)
        ' Forever loop. Needs a break, return, or exit() statement to
        ' exit the loop.FreeBASIC = Exit While
        ' Placing the menu inside of the while loop "Refreshes" the menu
        ' if an incorrect value is supplied.
        Con_Clear()
        ' Clearing the keyboard buffer is me being pedantic :)
        ' FB uses the GC Compiler and MinGW(32) which can leave
        ' stray '\r''\n' in the stdin buffer.
        Clear_Stdin() ' Clear the keyboard buffer
        Dim As Integer options = 0  ' Menu variable.

        Print "=============================================="
        Print "  MAIN MENU"
        Print "=============================================="
        Print "  1 - Stick Animation"
        Print "  2 - File Write Example"
        Print "  3 - File Read Example 1"
        Print "  4 - File Read Example 2"
        Print "  5 - Debug Message Box"
        Print "  6 - System Sound Test"
        Print "  7 - Mandelbrot Fractals"
        Print "  8 - Display time and date"
        Print ""
        Print "  9 - Exit The Application"
        Print "----------------------------------------------"
        Clear_Stdin()
        Input ; "  Enter your menu choice: ", options
        Print ""

        ' Check what choice was entered and act accordingly
        ' We can add as many choices as needed
        If options = 0 Then
            ' Ignore false [Enter]s
        Elseif options = 1 Then
            Stick_Animation()
        Elseif options = 2 Then
            File_Write_Example()
        Elseif options = 3 Then
            File_Read_Example1()
        Elseif options = 4 Then
            File_Read_Example2()
        Elseif options = 5 Then
            ' Send String. "Hello"
            Dim As String aVariable = "This is my message."
            DebugMsg("DEBUGmsg", aVariable)
            ' Send  Int 125
            'dim as Integer iVariable = 125
            'DebugMsg("DEBUGmsg", Str(iVariable))
            ' See also Val(), ValInt()
        Elseif options = 6 Then
            Sys_Sound()
        Elseif options = 7 Then
            Mandelbrot_Fractals_Console_ASCII()
        Elseif options = 8 Then
            Show_Time_Date()
            Con_Pause()
            Con_Clear()
        Elseif options = 9 Then
            Print "Exiting the application..."
            Sleep 1000  ' Allow 1 second for the Exit notice to display.
            Exit While
        Else
            Sys_Sound()
            Print !"Invalid option.\nPlease enter a number from the Menu Options.\n"
            Sleep 1000
        End If
    Wend

    Return 0
End Function

Function Stick_Animation() As Integer
    ' Stick man animation
    Dim As Integer Len_Animation = 11  ' Keep a record of the array length.
    Dim As String Animation(11) => { _
    !"   o           \n" _
    !"  /|\\          \n" _
    !"  / \\          ", _
    !" \\ o /         \n" _
    !"   |           \n" _
    !"  / \\          ", _
    !"  _ o          \n" _
    !"   /\\          \n" _
    !"  | \\          ", _
    !"               \n" _
    !"   ___\\o       \n" _
    !"  |)  |        ", _
    !"    __|        \n" _
    !"      \\o       \n" _
    !"      ( \\      ", _
    !"      \\ /      \n" _
    !"       |       \n" _
    !"      /o\\      ", _
    !"         |__   \n" _
    !"       o/      \n" _
    !"      / )      ", _
    !"               \n" _
    !"        o/__   \n" _
    !"        |  (\\  ", _
    !"           o _ \n" _
    !"           /\\  \n" _
    !"           / | ", _
    !"          \\ o /\n" _
    !"            |  \n" _
    !"           / \\ ", _
    !"            o  \n" _
    !"           /|\\ \n" _
    !"           / \\ "}

    Dim As Integer a = 0
    Dim As Integer b = 0
    Dim As Integer repeat = 3
    Dim As Integer Speed = 400

    While(a < repeat)
        For b = 0 To Len_Animation -1 Step 1  ' Loop animation forward
            Con_Clear()
            Print Animation(b)
            Sleep(Speed)
        Next b
        For b = Len_Animation -1 To 0 Step -1  ' Loop animation backwards
            Con_Clear()
            Print Animation(b)
            Sleep(Speed)
        Next b
        a+= 1
        Sleep(Speed)
    Wend
    Con_Clear()
    Return 0
End Function

Function File_Write_Example() As Integer

    Const filename As String = "MyTextFile.txt" ' Output file.
    ' Output file to write to.
    '==> Open Output file for text append ops.
    Dim FileOut As Integer = Freefile()
    If 0 <> Open(filename, For Append, As FileOut) Then
        Print "ERROR! Cannot open Output file " & filename
        Con_Pause() ' Wait until a key is pressed
        Return -1
    End If

    Dim As String string_Temp_Buffer

    Print "Please enter the text you would like to write to file " & filename
    Print "Type a line of text ... followed by [Enter]"
    Input "", string_Temp_Buffer

    ' Write the buffer to a new line of the open file (append).
    Print #FileOut, string_Temp_Buffer
    ' We must always remember to close the file when finished.
    Close #FileOut

    Print !"\nFile write completed..."
    Print "Press [Enter] to return to the MAIN MENU..."
    Con_Pause() 'wait until a key is pressed
    Con_Clear()
    Return 0
End Function

Function File_Read_Example1() As Integer
    Const filename As String = "MyTextFile.txt"  ' Input file.
    Dim string_Temp_Buffer As String  ' Temporary buffer
    'Dim As Integer Total_Lines = 0 ' = lines in the text file (To be calculated).

    Dim FileIn As Integer = Freefile()
    ' It is possible that the file may not yet exist. Opening it
    ' as "read/Input" will return an error. Let's test if the file exists first.
    If Open(filename, For Input, As #FileIn) <> 0 Then
        Print "ERROR! Cannot open Output file " & filename
        Print "Maybe the file has not yet been created."
        Print "Please select from the MAIN Menu"
        Print "to create a new file."
        Print "Press [Enter] to return to the MAIN MENU..."
        Con_Pause() 'wait until a key is pressed
        Return 0
    Else  ' Continue to process file...
        ' Line Input # will read up to one line (including '\r''\n') at a time.
        While Not Eof (FileIn)
            Line Input #FileIn, string_Temp_Buffer
            Print string_Temp_Buffer
        Wend

        ' It is important to free up resources as soon as they are no longer required.
        Close #FileIn' finished file reads, close the file.
    End If

    Print !"\nPress [Enter] to return to the MAIN MENU..."
    Con_Pause()  'wait until a key is pressed
    Con_Clear()
    Return 0
End Function

Function File_Read_Example2() As Integer
    Const filename As String = "MyTextFile.txt"  ' Input file.

    ' in FreeBASIC we also have to allocate enough char space to hold the string
    ' values to be stored. Because we don't know in advance how large the file
    ' will be, we also have to create a dynamic array in memory "On the heap"
    ' at run time.

    Dim As Integer cnt1  ' To enumerate and set array parameters.
    Dim As Integer Total_Lines = 0  ' To build our dynamic array size.

    Dim FileIn As Integer = Freefile()
    ' It is possible that the file may not yet exist. Opening it
    ' as "read/Input" will return an error. Let's test if the file exists first.
    If Open(filename, For Input, As #FileIn) <> 0 Then
        Print "ERROR! Cannot open Output file " & filename
        Print "Maybe the file has not yet been created."
        Print "Please select from the MAIN Menu"
        Print "to create a new file."
        Print "Press [Enter] to return to the MAIN MENU..."
        Con_Pause()  'wait until a key is pressed
        Return 0

    Else  ' Continue to process file...
        ' Create a temp buffer to hold each line.
        ' In free basic Type String is dynamically resized up to 2GiB
        Dim As String String_Temp_Buffer

        ' Check the file for the number of lines.
        ' Line Input # will read one line (up to '\r''\n') at a time.
        While Not Eof (FileIn)
            Line Input #FileIn, String_Temp_Buffer
            Total_Lines = Total_Lines + 1
        Wend
        Seek #FileIn, 1  ' Set pointer back to the start of the file.

        ' Build our dynamic array here.
        ' Now that we now how many lines to allocate, we can create a suitable
        ' sized array to hold the contents. Using ReDim we can set the required
        ' size of the dynamic array on the heap at runtime.
        ' Array declared in dynamic memory (On the heap).
        Dim As String Read_Buffer(Any)
        Redim Read_Buffer(Total_Lines)

        ' Read the file into the Dynamic Array at its correct line location,
        ' removing the new line chars '\r''\n'.
        For cnt1 = 0 To Total_Lines Step 1
            ' add each value to the correct array position by (Line/cnt1).
            Line Input #FileIn, Read_Buffer(cnt1)
        Next cnt1

        ' It is important to free up resources as soon as they are no longer required.
        Close #FileIn  ' finished file reads, close the file.

        ' Print the lines from the array.
        For cnt1 = 0 To Total_Lines Step 1
            Print Read_Buffer(cnt1)
        Next cnt1

        Print !"\nPress [Enter] to return to the MAIN MENU..."
        Con_Pause()  'wait until a key is pressed

        ' Important to always "free" the memory as soon as we are finished with it.
        ' Not doing so will lead to a memory leak as a new block of memory will
        ' be created on the heap each time the dynamic array is used.
        ' I am uncertain if dynamic memory (On the Heap) is freed when the
        ' Function exits, so I am just playing safe ¯\_(''/)_/¯
        Erase Read_Buffer
    End If' END file open if, else test.
    Con_Clear()
    Return 0
End Function


Function Mandelbrot_Fractals_Console_ASCII() As Integer
    /'
    Credits:
    https://cs.nyu.edu/~perlin/
    Ken Perlin
    Professor of Computer Science
    NYU Future Reality Lab F
    >
    Original Source:
    main(k){float i,j,r,x,y=-16;While(puts(""),y++<15)For(x
    =0;x++<84;putchar(" .:-;!/>)|&IH%*#"[k&15]))For(i=k=r=0;
    j=r*r-i*i-2+x/25,i=2*r*i+y/10,j*j+i*i<11&&k++<111;r=j);}
    >
    Info On Mandelbrot sets
    https://mathworld.wolfram.com/MandelbrotSet.html
    '/

    ' Although this describes a series of planes in 3D layers, the calculations
    ' are graphed to a 2D plane and use colour depth to describe points in the
    ' lower layers (planes).

    Con_Clear()
    Dim As Integer k = 1  ' First print character; default = 1 (0 to leave blank).
    Dim As String colours = " .:-;!/>)|&IH%*#"  ' 16 colours

    Dim As Single i=0
    Dim As Single j=0
    Dim As Single r=0
    Dim As Single x = 0
    Dim As Single y = -16

    ' zoom_x, zoom_y are relative and both must be changed as a percentage.
    Dim As Single zoom_x = 25.00  ' Default = 25,+zoom-In/-zoom-Out
    Dim As Single zoom_y = 10.00  ' Default = 10,+zoom-In/-zoom-Out

    Dim As Single offset_x = -2.00  ' Default = -2.00, -pan-L/+pan-R
    Dim As Single offset_y = 0.00  ' Default = 0.00, -pan-U/+pan-D

    While(y < 15)  ' Loop #1
        y+= 1
        Print ""  ' Line break '\n'.

        For x = 0 To 88-1 Step 1  ' Loop #2, (<84 == the screen print width.)
            ' Select colour level (Bitwise AND) from 16 colours, then print.
            Print Chr(colours[k And 15]);

            i=0
            k=0
            r=0
            While(1)  ' Loop #3
                ' Calculate x fractal.
                j = ((r*r) - (i*i) + ((x/zoom_x) + offset_x))
                ' Calculate y fractal.
                i = ((2*r*i) + ((y/zoom_y) + offset_y))

                ' Test for x,y divergence to infinity (lemniscates).
                ' In a sense this relates to the period between depth layers
                ' and the scale at which they diverge to infinity.
                ' The default values offer the most visually appealing balance,
                ' meaning they are easier for our brain to interpret.
                If(j*j+i*i > 11) Then  ' Default = 11
                    Exit While
                End If

                ' Test depth level (Colour).
                k+= 1
                If(k > 111) Then  ' Default = 111.
                    Exit While
                End If

                r=j  ' Start next calculation from current fractal.
            Wend
        Next x
    Wend
    Con_Pause()
    Con_Clear()
    Return 0
End Function


' --> START helper functions

Function Sys_Sound() As Integer

    If(OS_Windows = 1) Then
        'Print !"\a"
        Shell "rundll32 user32.dll,MessageBeep"
        'Shell "rundll32.exe Kernel32.dll,Beep 750,300"
    Elseif(OS_Unix = 1) Then
        Shell "paplay /usr/share/sounds/ubuntu/notifications/Blip.ogg"
        'Shell "paplay /usr/share/sounds/ubuntu/notifications/Rhodes.ogg"
        'Shell "paplay /usr/share/sounds/ubuntu/notifications/Slick.ogg"
        'Shell "paplay /usr/share/sounds/ubuntu/notifications/'Soft delay.ogg'"
        'Shell "paplay /usr/share/sounds/ubuntu/notifications/Xylo.ogg"
    Else
        Print !"\a"
    End If
    Return 0
End Function

Function DebugMsg(Byref aTitle As String, Byref aMessage As String) As Integer
    ' Requires:"windows.bi" (#include once "windows.bi").
    ' Not attached to parent console window.
    ' https://docs.microsoft.com/en-us/windows/win32/api/winuser
    ' /nf-winuser-messageboxa
    ' May throw a compiler warning... MessageBoxA Not found.
    If(OS_Windows = 1) Then
        #ifdef __FB_WIN32__
            MessageBox(NULL, aMessage, aTitle, MB_OK)  ' MB_OK|MB_SETFOREGROUND
        #endif
    Elseif(OS_Unix = 1) Then
        ' http://manpages.ubuntu.com/manpages/trusty/man1/xmessage.1.html
        ' apt-get install x11-utils
        'system("xmessage -center 'Hello, World!'");
        ' Else try wayland
        ' https://github.com/Tarnyko/wlmessage
        'system("wlmessage 'Hello, World!'");
        Dim As Integer reterr = 0
        Dim As String Buffer = ""
        Dim As String Buf_Msg = ""
        ' Place title text in 'apostrophe'.
        Buf_Msg = "\'" & aTitle & "\'"

        ' Build our command line statement.
        ' xmessage [-options] [message ...]
        ' NOTE! ">>/dev/null 2>>/dev/null" suppresses the console output.
        Buffer = "xmessage -center -title " & Buf_Msg & " '.:|" & aMessage & "|:.' >>/dev/null 2>>/dev/null"

        ' Send it to the command line.
        reterr = Shell(Buffer)
        If(reterr <> 0) And (reterr <> 1) Then  ' xmessage failed or not exist.
            ' Try Wayland compositor wlmessage.
            Buffer = "wlmessage ' |" & Str(aMessage) & "| ' >>/dev/null 2>>/dev/null"
            reterr = Shell(Buffer)
            If(reterr <> 0) And (reterr <> 1) Then
                ' Popup message failed.
                'printf("%d\n", reterr);
                Return -1
            End If
        End If

        Return 0
    Else
        ' OS Unknown
    End If
    Return 0
End Function

' A wrapper to flush/clear the keyboard input buffer
Sub Clear_Stdin()
    While Inkey <> ""  ' loop until the Inkey buffer is empty
    Wend
End Sub

' Console Clear
Function Con_Clear() As Integer
    ' The system() call allows the programmer to run OS command line batch commands.
    ' It is discouraged as there are more appropriate C functions for most tasks.
    ' I am only using it in this instance to avoid invoking additional OS API headers and code.
    If (OS_Windows) Then
        Shell "cls"
    Elseif (OS_Unix) Then
        Shell "clear"
    End If
    Return 0
End Function

' Console Pause (GetKey version)
Function Con_Pause() As Integer
    Dim As Long dummy
    Print !"\nPress any key to continue..."
    dummy = Getkey
    Return 0
End Function

' Wrapper for the 2 functions time/date.
' We can create convenience wrapper functions to "wrap" a set of more
' complex tasks in a single function call.
' This is helpful if it is a common set of tasks that are called regularly
' throughout an application.
Function Show_Time_Date() As Integer
    show_time()
    Print " - ";
    show_date()
    Print ""
    Return 0
End Function

' Display current system time.
Function show_time() As Integer
    Print Time;
    Return 0
End Function

' Display current system date.
Function show_date() As Integer
    ' Note: FB returns format mm-dd-yyyy
    ' This is due to the QBasic American origins.
    ' You can use the C functions in the C example to change this.
    Print Date;
    Return 0
End Function
