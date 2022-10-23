'-------------------------------------------------------------------------------
' Name:        Loopsb.bas
' Purpose:     Loop animation
'
' Platform:    Win64, Ubuntu64
'
' Author:      Axle
' Created:     16/02/2022
' Updated:     19/02/2022
' Copyright:   (c) Axle 2022
' Licence:     MIT No Attribution
'-------------------------------------------------------------------------------

' Test if Windows or Unix OS
#ifdef __FB_WIN32__
#define OS_Windows 1  ' 1 = True (aka Bool)
#define OS_Unix 0
#endif

#ifdef __FB_UNIX__'__FB_LINUX__
' TODO
#define OS_Unix 1
#define OS_Windows 0  ' 0 = False (aka Bool)
#endif

' Define extra functions so we can place them at the bottom of the page.

Declare Function Con_Clear() As Integer
Declare Function Con_Pause() As Integer  ' GetKey Version
Declare Function main_procedure() As Integer

main_procedure()

Function main_procedure() As Integer  ' Main procedure
    
    ' Loading screen animation one.
    ' '\' is an escape prefix and must be placed in a literal as '\\'
    Dim As String BarRotate = "|\-/"
    Dim As String Notice = "Please wait..."
    Dim As Integer RotSpeed = 500  ' This is the sleep timer. Lower setting = faster.
    Dim As Integer counts = 15  ' The number of scenes.
    Dim As Integer x = 0  ' loop counters
    Dim As Integer y = 0  ' loop counters
    Dim As Integer z = 0  ' loop counters
    
    Print Notice
    ' Keep looping through the 4 characters.
    For x = 0 To counts -1 Step 1
        If(y > 3) Then  ' At the fourth (0,1,2,3) character '/' restart from 0
            y = 0  ' reset to 0 and restart character enumeration.
        End If
        Print Chr(BarRotate[y]);  ' '\b' = Backspace control character.
        Sleep(RotSpeed)  ' replaced with cross platform wrapper.
        y+= 1
        Print !"\r";  ' Carriage Return to overwrite the last character.
    Next x
    
    Con_Clear()
    Sleep(500)  ' Just a pause to reduce screen/buffer flicker.
    
    ' Loading screen animation two.
    ' Create the array containing the 4 strings.
    ' Special characters must be preceded by an escape inside of strings '\'
    ' ' _' is a line continuation so we can break a long line over several.
    Dim As String BigRotate2(4) => { _
    !"\\  \n" _
    !" \\ \n" _
    !"  \\", _
    !" | \n" _
    !" | \n" _
    " | ", _
    !"  /\n" _
    !" / \n" _
    "/  ", _
    !"   \n" _
    !"---\n" _
    "   "}
    
    While(z < counts)  ' Repeat counts times.
        For x = 0 To 4 -1 Step 1  ' loop through all 4 array elements.
            Print Notice
            Print BigRotate2(x);  ' Alternative use BigRotate2
            Sleep(Abs(RotSpeed/2))  ' Slow the animation down.
            Con_Clear()
        Next x
        z+= 4
    Wend
    
    Con_Clear()  ' Clear the screen for the next animation.
    Sleep(500)  ' Just a pause to reduce screen/buffer flicker.
    
    ' Loading screen animation 3.
    Dim As String Tracer = ""  ' Initiate all to 0.
    For x = 0 To 21 -1 Step 1  ' Populate with space, except for string terminator[22].
        Tracer+= " "
    Next
    
    Dim As Integer t = 0  ' loop counters
    z = 0  ' Reset loop counters
    While(z < (Abs(counts/5)))  ' abs(n/5) to reduce the repeats.
        While(t < 20)  ' Loop forward...
            t += 1  ' Start at 0 +1 to allow room for the tail.
            Tracer[t] = Asc("#")  ' Add mark at current position.
            Tracer[t-1] = Asc(":")  ' Add tail at current position -1.
            Print Notice
            Print Tracer;  ' Print full array/string to the screen.
            Tracer[t-1] = Asc(" ")  ' Remove the tail. Next tail (+1) will overwrite the Marker.
            Sleep(RotSpeed/4)  ' slow the animation down a bit
            Con_Clear()  ' Clear the screen for the next print.
        Wend
        While(t > 0)  ' Loop backwards...
            t -= 1  ' Start at -1 to allow room for the tail at t.
            Tracer[t] = Asc("#")
            Tracer[t+1] = Asc(":")  ' t+1 is the right side of '#'.
            Print Notice
            Print Tracer;
            Tracer[t+1] = Asc(" ")
            Sleep(RotSpeed/4)
            Con_Clear()
        Wend
        z+= 1
    Wend
    
    Con_Clear()  ' Clear the screen for the next animation.
    Sleep(500)  ' Just a pause to reduce screen/buffer flicker.
    
    ' Stick-man animation
    ' Define our data arrays...
    ' This is the original ASCII text file from the net.
    ' The arrays were created by hand in Notepad++ on this occasion.
    '  o   \ o /  _ o         __|    \ /     |__        o _  \ o /   o
    ' /|\    |     /\   ___\o   \o    |    o/    o/__   /\     |    /|\
    ' / \   / \   | \  /)  |    ( \  /o\  / )    |  (\  / |   / \   / \
    
    Dim As Integer Len_Cart = 11  ' Keep a record of the array length.
    ' Note: the '!' before a string literal allows the use of escape chars '\'
    ' ' _' is a line continuation so we can break a long line over several.
    Dim As String Cart2(11) => { _
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
    
    Dim As Integer a = 0  ' loop counters
    Dim As Integer b = 0  ' loop counters
    Dim As Integer repeat = 3  ' Repeat the animation for loops 3 times.
    Dim As Integer Speed = 400  ' Delay in milliseconds to slow the animation down.
    
    While(a < repeat)  ' Repeat the animation n times.
        For b = 0 To Len_Cart -1 Step 1  ' Loop animation forward
            Con_Clear()  ' Clear the console ready for the next print.
            Print Cart2(b)  ' Print array element [n]
            Sleep(Speed)  ' Replace with cross platform wrapper.
        Next b
        For b = Len_Cart -1 To 0 Step -1  ' Loop animation backwards
            Con_Clear()  ' Clear the console ready for the next print.
            Print Cart2(b)  ' Print array element [n]
            Sleep(Speed)  ' Replace with cross platform wrapper.
        Next b
        a+= 1
        Sleep(Speed)
    Wend
    
    Con_Pause() ' DEBUG Pause
    Return 0
End Function  ' END main_procedure <---


' --> START helper functions

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
    Print("Press any key to continue...")
    dummy = Getkey
    Return 0
End Function
