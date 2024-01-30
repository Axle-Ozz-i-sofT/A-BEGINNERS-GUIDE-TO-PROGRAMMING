#------------------------------------------------------------------------------
# Name:        main_cli.py
# Purpose:     Demonstrate the use of MPV binary as a simple command line
#              media player.
#
# Title:       "MPV CLI Tests"
#
# Platform:    Win64, Ubuntu64
# Depends:     mpv-0.35.1-x86_64, "mpv-0.35.1-x86_64-v3" (mpv.com, mpv.exe)
# Requires:    mpv.com, mpv.exe must be in the application directory or
#              the system path.
#
# Author:      Axle
# Created:     23/06/2023
# Updated:     26/06/2023
# Copyright:   (c) Axle 2023
# Licence:     MIT-0 No Attribution
#------------------------------------------------------------------------------
# NOTES:
# A simple example of invoking the MPV player from the command line.
# Interactive player controls exist only in the On Screen Controls (OSC) in
# the playback window via the mouse or keyboard. The calling application has
# no further interaction after MPV has started.
# This is the most simple method to create a video splash screen without
# complicated application programming.
#
# Note that Multimedia programming is complex by nature and requires a steep
# learning path which is beyond the context of this book.
#------------------------------------------------------------------------------

# mpv.com, mpv.exe (mpv linux) must be provided separately in the path.

import os
import sys
import time
import threading

def main():

    if 1 == Con_IsREPL():
        print("This application is best viewed running from the OS Command interpreter")
        Con_Pause()

    print("Splash screen")

    # Example single thread.
    Splash_blocking()  # Un-comment to test.

    # Remove the block comment """ """ to test.
    """
    # Example 2 threads.
    # Test in a separate thread, non blocking. Note that all child processes
    # must complete before the parent application can quit.
    #ret = 0
    # Declare variable for thread's ID:
    #pthread_t thread_id;  # Un-comment to test.
    # Call Splash_nonblocking() in a separate thread.
    try:
        thread_a = threading.Thread(target=Splash_nonblocking)  # Create a new thread
        thread_a.start()  # Start the thread
    except:
        print("Thread create error", file=sys.stderr)
        #sys.stderr.write("Thread create error\n")
        # Do error tasks.

    # Run a task in the primary thread.
    time.sleep(2)  # Wait for the second thread to start...
    print("Wait cycle...")
    for i in range(1, 25 + 1):
        #print(str(i).zfill(2) , end='', flush=True)
        print(str(i).zfill(2) , end='')
        sys.stdout.flush()  # Because we have no '\n' the buffer needs to be pushed to the console.
        time.sleep(1)
        print("\b\b", end='')

    print("")
    print("Haaa! I finished first!")

    thread_a.join()  # Blocking. Wait for the thread to finish, and recover returns.
    """

    # What if we want to cancel or kill the thread before it has finished?
    # Note that this is poor practice as we should send a signal to the thread
    # with appropriate cleanup tasks for memory etc. See: pthread_cleanup_push()
    # https://www.cs.kent.edu/~ruttan/sysprog/lectures/multi-thread/multi-thread.html
    # https://sites.cs.ucsb.edu/~tyang/class/pthreads/index_sgi.html
    #pthread_cancel(thread_id);  // Will end the thread, but not the threads child process MPV.
    #thread_a.join(0)  # waits for 0 seconds then ends the thread.
    # Not the most elegant solution but we can kill the MPV process.
    # Note! If we just kill MPV without pthread_cancel() the system() function
    # will return and end the thread gracefully.
    #if os.name == 'nt':
    # Windows
    # or mpv.com
    #os.system('Taskkill /IM mpv.exe /F'); // kind of ugly hack here :(
    # Using TaskKill in this instance is safe as I know the child process
    # mpv.exe and the wave file will be cleared from the Windows memory.
    # Otherwise using this could leave data fragments orphaned in the system
    # memory heap.

    #elif os.name == 'posix':
    # Ubuntu
    # killall -9 mpv
    #os.system('pkill -9 mpv'); // kind of ugly hack here :( -9 task kill, -15 sigterm
    #else
    #    pass
    #endif

    Con_Pause()
    return None
    ## END main()

def Splash_blocking():  # Synchronous

    print("MPV Command line Tests.\nSynchronous.")

    # for windows
    if os.name == 'nt':
        mpv_player_com = "mpv.com "  # Invoke command line playback statistics.
        #mpv_player_exe = "mpv.exe "  # Don't open a console.
    # for mac and linux
    elif os.name == 'posix':
        mpv_player_com = "mpv "  # Invoke command line playback statistics.
        #mpv_player_exe = "mpv "  # Don't open a console.
    else:
        return None  # Other OS

    options = "--geometry=800x600 --script-opts=osc-visibility=always "  # Start options.
    # https://mpv.io/manual/master/#options
    # Attempt starting MPV with other options.
    # --geometry=800x600+20+40
    # --loop-file=1 (Loop 2 times)
    # --script-opts=osc-visibility=always
    # --include=mpv.conf
    # --title="MPV Console Tests"
    # --keep-open=yes|no|always
    # --start=00:10 --length=10 --end=40
    # --pause
    # --volume=20 (0-100%)
    # --ontop
    # --border, --no-border
    # --window-maximized=<yes|no>
    # --fullscreen
    # -v (Verbose console output)
    # --msg-level=all=trace to see all messages mpv outputs.
    # "pexels-street-donkey-3706265-1920x1080-30fps~1.mp4"
    media_file = "Cascading_water.mp4"

    # Concatenate our startup script.
    buffer = mpv_player_com + options + media_file

    # Send the script to the command line interpreter (console).
    # NOTE: there are many OS dependent ways to launch another application
    # both blocking and non-blocking. System is just a lazy cross platform way :)
    os.system(buffer)  # Synchronous blocking.

    return None
    ## END Splash_blocking()

def Splash_nonblocking():  # Asynchronous (Multi threading).

    print("MPV Command line Tests.\nAsynchronous.")

    # for windows
    if os.name == 'nt':
        #mpv_player_com = "mpv.com "  # Invoke command line playback statistics.
        mpv_player_exe = "mpv.exe "  # Don't open a console.
    # for mac and linux
    elif os.name == 'posix':
        #mpv_player_com = "mpv "  # Invoke command line playback statistics.
        mpv_player_exe = "mpv "  # Don't open a console.
    else:
        return None  # Other OS

    # This first startup script works fine with mpv.exe, but Linux version does
    # not have the separate executable so we have to use "--no-terminal".
    #options = "--no-border --geometry=800x600 --script-opts=osc-visibility=always"  # Start options.
    options = "--no-border --no-terminal --geometry=800x600 --script-opts=osc-visibility=always "  # Start options.
    # https://mpv.io/manual/master/#options
    # Attempt starting MPV with other options.
    # --geometry=800x600+20+40
    # --loop-file=1 (Loop 2 times)
    # --script-opts=osc-visibility=always
    # --include=mpv.conf
    # --title="MPV Console Tests"
    # --keep-open=yes|no|always
    # --start=00:10 --length=10 --end=40
    # --pause
    # --volume=20 (0-100%)
    # --ontop
    # --border, --no-border
    # --window-maximized=<yes|no>
    # --fullscreen
    # -v (Verbose console output)
    # --msg-level=all=trace to see all messages mpv outputs.
    # "pexels-street-donkey-3706265-1920x1080-30fps~1.mp4"
    media_file = "Cascading_water.mp4"

    # Concatenate our startup script.
    buffer = mpv_player_exe + options + media_file

    # Send the script to the command line interpreter (console).
    # NOTE: there are many OS dependent ways to launch another application
    # both blocking and non-blocking. System is just a lazy cross platform way :)
    os.system(buffer)  # Synchronous blocking.

    return None
    ## END Splash_nonblocking()

# ====> Convenience helper functions

# Test if we are inside of the REPL interactive interpreter.
# This function is in alpha and may not work as expected.
def Con_IsREPL():
    import os
    if os.sys.stdin and os.sys.stdin.isatty():
        if os.isatty(os.sys.stdout.fileno()):
            return 0  # OS Command Line
        else:
            return 1  # REPL - Interactive Linux?
    else:
        return 1  # REPL - Interactive Windows?
    return None

# Console Pause wrapper.
def Con_Pause():
    dummy = ""
    print("")
    dummy = input("Press [Enter] key to continue...")
    return None

if __name__ == '__main__':
    main()