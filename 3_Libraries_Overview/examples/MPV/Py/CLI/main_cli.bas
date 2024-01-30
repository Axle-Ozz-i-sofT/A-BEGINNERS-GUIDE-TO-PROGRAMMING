''------------------------------------------------------------------------------
'' Name:        main_cli.bas
'' Purpose:     Demonstrate the use of MPV binary as a simple command line
''              media player.
''
'' Title:       "MPV CLI Tests"
''
'' Platform:    Win64, Ubuntu64
'' Depends:     mpv-0.35.1-x86_64, "mpv-0.35.1-x86_64-v3" (mpv.com, mpv.exe)
'' Requires:    mpv.com, mpv.exe must be in the application directory or
''              the system path.
''
'' Author:      Axle
'' Created:     23/06/2023
'' Updated:     26/06/2023
'' Copyright:   (c) Axle 2023
'' Licence:     MIT-0 No Attribution
''------------------------------------------------------------------------------
'' NOTES:
'' A simple example of invoking the MPV player from the command line.
'' Interactive player controls exist only in the On Screen Controls (OSC) in
'' the playback window via the mouse or keyboard. The calling application has
'' no further interaction after MPV has started.
'' This is the most simple method to create a video splash screen without
'' complicated application programming.
''
'' Note that Multimedia programming is complex by nature and requires a steep
'' learning path which is beyond the context of this book.
''------------------------------------------------------------------------------


'' mpv.com, mpv.exe (mpv linux) must be provided separately in the path.

#lang "fb"

'' If required for other crt types and conversions.
'#include once "crt.bi"
#include "file.bi"
'#include <stdio.h>
'#include <stdlib.h>
'#include <pthread.h>

Declare Function main_procedure() As Integer

Declare Function Splash_blocking() As Integer  '' Synchronous
Declare Sub Splash_nonblocking(none as Integer)  '' Asynchronous (Multi threading).

Declare Function Con_Pause() As Integer
main_procedure()

Function main_procedure() As Integer  ' Main procedure

    print "Splash screen"

    '' Example single thread.
    'Splash_blocking()  '' Un-comment to test.

    '' Remove the block comment /' '/ to test.

    '' Example 2 threads.
    '' This currently isn't working as expected.
    '' test in a separate thread, non blocking. Note that all child processes
    '' must complete before the parent application can quit.
    Dim As Integer ret = 0
    '' Declare variable for thread's ID:
    'pthread_t thread_id  '' Un-comment to test.
    Dim As Any Ptr thread_id
    '' Call Splash_nonblocking() in a separate thread.
    'ret = pthread_create(&thread_id, NULL, Splash_nonblocking, NULL)  '' Un-comment to test.
    '' Note Threadcreate() is the preferred method. Threadcall() is a simplified version.
    thread_id = Threadcall Splash_nonblocking(0)
    If thread_id = 0 Then
        Print "Thread create error"; Err
        '' Do error tasks.
    End If

    '' Run a task in the primary thread.
    '' Important! Deep sleep time, 1 must be enabled to disable key interrupt.
    Sleep 2000, 1  '' Wait for second thread to start...
    print "Wait cycle..."

    Dim As Integer i
    '' Using pipe file console stdout. This is needed to flush !"\b"; as the
    '' newline character !"\n" is required to flush the buffer to the console.
	Dim As Long fp_stdout = FreeFile
	Open Cons For Output As #fp_stdout
    for i = 1 To 25 Step +1
        'print i;
        Print #fp_stdout, Str(i);  '' Print count to stdout.
        FileFlush(fp_stdout)  '' Flush the buffer to the console screen.
        Sleep 1000, 1
        'print !"\b\b\b";
        Print #fp_stdout, !"\b\b\b";  '' Backspace to clear the line before next Print.
    Next i
	fp_stdout = FreeFile  '' Free/close the file handle to stdout.

    print ""
    print "Haaa! I finished first!"

    ThreadWait thread_id  '' Blocking. Wait for the thread to finish, and recover returns.


/'
    '' What if we want to cancel or kill the thread before it has finished?
    '' Note that this is poor practice as we should send a signal to the thread
    '' with appropriate cleanup tasks for memory etc. See: pthread_cleanup_push()
    '' https://www.cs.kent.edu/~ruttan/sysprog/lectures/multi-thread/multi-thread.html
    '' https://sites.cs.ucsb.edu/~tyang/class/pthreads/index_sgi.html
    ''pthread_cancel(thread_id);  // Will end the thread, but not the threads child process MPV.
    '' Not the most elegant solution but we can kill the MPV process.
    '' Note! If we just kill MPV without pthread_cancel() the system() function
    '' will return and end the thread gracefully.
#ifdef __FB_WIN32__
    '' Windows
    '' or mpv.com
    Shell "Taskkill /IM mpv.exe /F" '' kind of ugly hack here :(
    '' Using TaskKill in this instance is safe as I know the child process
    '' mpv.exe and the wave file will be cleared from the Windows memory.
    '' Otherwise using this could leave data fragments orphaned in the system
    '' memory heap.
#endif
#ifdef __FB_LINUX__  ''__FB_UNIX__, __FB_LINUX__
    '' Ubuntu
    '' killall -9 mpv
    Shell "pkill -9 mpv" '' kind of ugly hack here :( -9 task kill, -15 sigterm
#endif
'/

    Con_Pause()
    Return 0
End Function  ' END main_procedure <---

Function Splash_blocking() As Integer  '' Synchronous

    print !"MPV Command line Tests.\nSynchronous.\n"  '' Note ! = use escape sequences.

    '' Please note the spaces in the strings to create the cmd arguments.
	#ifdef __FB_WIN32__
    Dim As String mpv_player_com = "mpv.com "  '' Invoke command line playback statistics.
    'Dim As String mpv_player_exe = "mpv.exe "  '' Don't open a console.
	#endif
	#ifdef __FB_UNIX__'__FB_LINUX__
    Dim As String mpv_player_com = "mpv "  '' Invoke command line playback statistics.
    'Dim As String mpv_player_exe = "mpv "  '' Don't open a console.
	#endif

    Dim As String buffer
    Dim As String options = "--geometry=800x600 --script-opts=osc-visibility=always "  '' Start options.
    '' https://mpv.io/manual/master/#options
    '' Attempt starting MPV with other options.
    '' --geometry=800x600+20+40
    '' --loop-file=1 (Loop 2 times)
    '' --script-opts=osc-visibility=always
    '' --include=mpv.conf
    '' --title="MPV Console Tests"
    '' --keep-open=yes|no|always
    '' --start=00:10 --length=10 --end=40
    '' --pause
    '' --volume=20 (0-100%)
    '' --ontop
    '' --border, --no-border
    '' --window-maximized=<yes|no>
    '' --fullscreen
    '' -v (Verbose console output)
    '' --msg-level=all=trace to see all messages mpv outputs.
    '' "pexels-street-donkey-3706265-1920x1080-30fps~1.mp4"
    Dim As String media_file = "Cascading_water.mp4"

    '' Concatenate our startup script.
    buffer = mpv_player_com + options + media_file

    '' Send the script to the command line interpreter (console).
    Shell buffer  '' Synchronous blocking.

    Return 0
End Function  ' END main_procedure <---


Sub Splash_nonblocking(none as Integer)  '' Asynchronous (Multi threading).

    print !"MPV Command line Tests.\nAsynchronous.\n"  '' Note ! = use escape sequences.
    '' Please note the spaces in the strings to create the cmd arguments.
	#ifdef __FB_WIN32__
    'Dim As String mpv_player_com = "mpv.com "  '' Invoke command line playback statistics.
    Dim As String mpv_player_exe = "mpv.exe "  '' Don't open a console.
	#endif
	#ifdef __FB_UNIX__'__FB_LINUX__
    'Dim As String mpv_player_com = "mpv "  '' Invoke command line playback statistics.
    Dim As String mpv_player_exe = "mpv "  '' Don't open a console.
	#endif

    Dim As String buffer
    '' This first startup script works fine with mpv.exe, but Linux version does
    '' not have the separate executable so we have to use "--no-terminal".
    'Dim As String options = "--no-border --geometry=800x600 --script-opts=osc-visibility=always "  '' Start options.
    Dim As String options = "--no-border --no-terminal --geometry=800x600 --script-opts=osc-visibility=always "  '' Start options.
    '' https://mpv.io/manual/master/#options
    '' A tempt starting MPV with other options.
    '' --geometry=800x600+20+40
    '' --loop-file=1 (Loop 2 times)
    '' --script-opts=osc-visibility=always
    '' --include=mpv.conf
    '' --title="MPV Console Tests"
    '' --keep-open=yes|no|always
    '' --start=00:10 --length=10 --end=40
    '' --pause
    '' --volume=20 (0-100%)
    '' --ontop
    '' --border, --no-border
    '' --window-maximized=<yes|no>
    '' --fullscreen
    '' -v (Verbose console output)
    '' --msg-level=all=trace to see all messages mpv outputs.
    '' "pexels-street-donkey-3706265-1920x1080-30fps~1.mp4"
    Dim As String media_file = "Cascading_water.mp4"


    '' Concatenate our startup script.
    buffer = mpv_player_exe + options + media_file

    '' Send the script to the command line interpreter (console).
    Shell buffer  '' Synchronous blocking.

End Sub  ' END main_procedure <---


'' Console Pause (GetKey version)
Function Con_Pause() As Integer
    Dim As Long dummy
    Print !"\nPress any key to continue..."
    dummy = Getkey
    Return 0
End Function
