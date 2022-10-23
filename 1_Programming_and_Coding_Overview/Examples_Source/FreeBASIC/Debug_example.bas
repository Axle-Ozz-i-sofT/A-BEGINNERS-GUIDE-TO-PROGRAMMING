'-------------------------------------------------------------------------------
' Name:        Debug)example.bas
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

Declare Function main_procedure() As Integer
Declare Function DebugMsg(Byref aTitle As String, Byref aMessage As String) As Integer

main_procedure()
' Print a table 5 rows x 5 columns
' with each column numbered 6 to 10
Function main_procedure() As Integer
    Dim As UInteger ROWS = 5
    Dim As UInteger COLUMNS = 5
    Dim As UInteger Row_y = 0
    Dim As UInteger Col_x = 0
    Dim As UInteger MyArray(5, 5)

    For Row_y = 0 To ROWS -1 Step 1   ' Count through each row.
        For Col_x = 0 to COLUMNS -1 Step 1
            MyArray(Row_y, Col_x) = Col_x +5
            ' Unlike C BASIC is a high level language and we can simply
            ' use Str() to convert a number to a string.
            DebugMsg("DEBUG_Col_x", Str(Col_x))
        Next Col_x
    Next Row_y

    for Row_y = 0 to ROWS -1 Step 1   ' Count through each row.
        for Col_x = 0 To COLUMNS -1 Step 1
            Print MyArray(Row_y, Col_x);
        Next Col_x
        Print ""
    Next Row_y

    Print "Press any key to continue..."
    Sleep  ' Sleep until a key is pressed
    Return 0
End Function  'main_procedure

' A simple message box for debugging.
' Takes a String value only, so you will have to convert numbers to String.
Function DebugMsg(Byref aTitle As String, Byref aMessage As String) As Integer
    ' Requires:"windows.bi" (#include once "windows.bi").
    ' Not attached to parent console window.
    ' https://docs.microsoft.com/en-us/windows/win32/api/winuser
    ' /nf-winuser-messageboxa
    ' May throw a compiler warning... MessageBoxA Not found.

    If(OS_Windows = 1) Then
        #ifdef __FB_WIN32__
        Dim As Integer reterr = 0  ' Holds the return value from the command line.
        reterr = MessageBox(NULL, aMessage, aTitle, MB_OKCANCEL)
        If reterr = 0 Then
            ' MessageBox() Fail
            Return 0
        Elseif reterr = 1 then
            ' IDOK
            Return 1
        Elseif reterr = 2 then
            ' IDCANCEL (Ctrl +C is dissabled when GUI messagebox is used)
            ' So provide an option to break out of the debugging...
            ' This will also exit now if the Close X is selected.
            ' Please note that the "End" statement may not clean up all memory
            ' and its use is discouraged. Only use it in debugging and
            ' not in production code.
            End
        else
            Return -1
        End If
        ' To attached to parent console window.
        'MessageBoxA(FindWindowA("ConsoleWindowClass", NULL),msg,title,0);
        #endif
    Elseif(OS_Unix = 1) Then
        ' http://manpages.ubuntu.com/manpages/trusty/man1/xmessage.1.html
        ' apt-get install x11-utils
        'system("xmessage -center 'Hello, World!'");
        ' Else try wayland
        ' https://github.com/Tarnyko/wlmessage
        'system("wlmessage 'Hello, World!'");
        
		Dim As Integer reterr = 0  ' Holds the return value from the command line.
        Dim As String Buffer = ""
        Dim As String Buf_Msg = ""
        ' Place title text in 'apostrophe'.
        Buf_Msg = "'" & aTitle & "'"

        ' Build our command line statement.
        ' xmessage [-options] [message ...]
        ' NOTE! ">>/dev/null 2>>/dev/null" suppresses the console output.
        Buffer = "xmessage -buttons OK:101,CANCEL:102 -center -title " & Buf_Msg & " '.:|" & aMessage & "|:.' >>/dev/null 2>>/dev/null"
		' Sometimes the application gets to the message window before the console
		' window has fully opened. This leaves the message as a background window.
		' Commonly referred to as a "Race Condition", so I have placed a 1/10
		' second sleep to give the parent console window time to open.
		Sleep 100
        ' Send it to the command line.
        reterr = Shell(Buffer)
        If(reterr = 101) Then
            ' ID OK
        ElseIf(reterr = 102) Then
            ' ID CANCEL
            End  ' Quit debugging
        ElseIf(reterr = 1) Then
            ' ID CLOSE X
        ElseIf(reterr = 0) Then
            ' ID Default run OK
        Else  ' xmessage failed or not exist.
            ' Try Wayland compositor wlmessage.
            Buffer = "wlmessage ' |" & aMessage & "| ' >>/dev/null 2>>/dev/null"
            reterr = Shell(Buffer)
            If(reterr <> 0) And (reterr <> 1) Then
                ' Popup message failed.
                'Print  reterr
                Return -1
            End If
        End If
    Else
        ' OS Unknown
        Return -1
    End If
    Return 0
End Function
