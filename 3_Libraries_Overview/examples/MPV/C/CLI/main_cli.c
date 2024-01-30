//------------------------------------------------------------------------------
// Name:        main_cli.c
// Purpose:     Demonstrate the use of MPV binary as a simple command line
//              media player.
//
// Title:       "MPV CLI Tests"
//
// Platform:    Win64, Ubuntu64
//
// Compiler:    GCC V9.x.x, MinGw-64, libc (ISO C99)
// Depends:     mpv-0.35.1-x86_64, "mpv-0.35.1-x86_64-v3" (mpv.com, mpv.exe)
// Requires:    mpv.com, mpv.exe must be in the application directory or
//              the system path.
//
// Author:      Axle
// Created:     20/06/2023
// Updated:     21/06/2023
// Version:     0.0.1.2
// Copyright:   (c) Axle 2022
// Licence:     MIT-0 No Attribution
//------------------------------------------------------------------------------
// NOTES:
// A simple example of invoking the MPV player from the command line.
// Interactive player controls exist only in the On Screen Controls (OSC) in
// the playback window via the mouse or keyboard. The calling application has
// no further interaction after MPV has started.
// This is the most simple method to create a video splash screen without
// complicated application programming.
//
// Note that Multimedia programming is complex by nature and requires a steep
// learning path which is beyond the context of this book.
//
// If using a virtual machine such as VirtualBox you will need to switch on
// 3D Video acceleration to remove the graphics library errors.
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#ifdef __unix__ // _linux__ (__linux__)
#include <unistd.h>
#endif

// Turn off compiler warnings for unused variables between (Windows/Linux etc.)
#define unused(x) (x) = (x)

int Splash_blocking(void);  // Synchronous
void *Splash_nonblocking(void* p);  // Asynchronous (Multi threading).

int Con_Sleep(int seconds);
// Safe Pause
void S_Pause(void);
// Safe getchar() removes all artefacts from the stdin buffer.
int S_getchar(void);

int main(int argc, char *argv[])
    {
    unused(argc);  // Turns off the compiler warning for unused argc, argv
    unused(argv);  // Turns off the compiler warning for unused argc, argv

    printf("Splash screen\n");

    // Example single thread.
    //Splash_blocking();  // Un-comment to test.

    // Remove the block comment /* */ to test.
    //
    // Example 2 threads.
    // test in a separate thread, non blocking. Note that all child processes
    // must complete before the parent application can quit.
    int ret = 0;
    // Declare variable for thread's ID:
    pthread_t thread_id;  // Un-comment to test.
    // Call Splash_nonblocking() in a separate thread.
    ret = pthread_create(&thread_id, NULL, Splash_nonblocking, NULL);  // Un-comment to test.
    if( ret !=0 )
        {
        perror("Thread create error");
        // Do error tasks.
        }
    // Run a task in the primary thread.
    Con_Sleep(1);  // Wait for the second thread to start...
    printf("Wait cycle...\n");
    for (int i = 1; i < 25 +1; i++)
        {
        printf("%02d", i );
        fflush(stdout);  // Because we have no '\n' the buffer needs to be pushed to the console.
        Con_Sleep(1);
        printf("\b\b");
        }
    printf("\n");
    printf("Haaa! I finished first!\n");

    pthread_join(thread_id,NULL);  // Blocking. Wait for the thread to finish, and recover returns.
    //

/*
    // What if we want to cancel or kill the thread before it has finished?
    // Note that this is poor practice as we should send a signal to the thread
    // with appropriate cleanup tasks for memory etc. See: pthread_cleanup_push()
    // https://www.cs.kent.edu/~ruttan/sysprog/lectures/multi-thread/multi-thread.html
    // https://sites.cs.ucsb.edu/~tyang/class/pthreads/index_sgi.html
    //pthread_cancel(thread_id);  // Will end the thread, but not the threads child process MPV.
    // Not the most elegant solution but we can kill the MPV process.
    // Note! If we just kill MPV without pthread_cancel() the system() function
    // will return and end the thread gracefully.
#ifdef _WIN32
    // Windows
    // or mpv.com
    system("Taskkill /IM mpv.exe /F"); // kind of ugly hack here :(
    // Using TaskKill in this instance is safe as I know the child process
    // WavBeep.exe and the wave file will be cleared from the Windows memory.
    // Otherwise using this could leave data fragments orphaned in the system
    // memory heap.

#elif __linux__
    // Ubuntu
    // killall -9 mpv
    system("pkill -9 mpv"); // kind of ugly hack here :( -9 task kill, -15 sigterm
#else
#endif
*/
    S_Pause();

    return 0;
    }

int Splash_blocking(void)  // Synchronous
    {
    printf("MPV Command line Tests.\nSynchronous.\n");
    // Note: *Variable is the same as Variable[]
    #ifdef _WIN32
    char mpv_player_com[] = "mpv.com";  // Invoke command line playback statistics.
    //char mpv_player_exe[] = "mpv.exe";  // Don't open a console.
    #endif
    #ifdef __unix__ // _linux__ (__linux__)
    char mpv_player_com[] = "mpv";  // Invoke command line playback statistics.
    //char mpv_player_exe[] = "mpv";  // Don't open a console.
    char buffer[1024] = {'\0'};
    char *options = "--geometry=800x600 --script-opts=osc-visibility=always";  // Start options.
    // https://mpv.io/manual/master/#options
    // Attempt starting MPV with other options.
    // --geometry=800x600+20+40
    // --loop-file=1 (Loop 2 times)
    // --script-opts=osc-visibility=always
    // --include=mpv.conf
    // --title="MPV Console Tests"
    // --keep-open=yes|no|always
    // --start=00:10 --length=10 --end=40
    // --pause
    // --volume=20 (0-100%)
    // --ontop
    // --border, --no-border
    // --window-maximized=<yes|no>
    // --fullscreen
    // -v (Verbose console output)
    // --msg-level=all=trace to see all messages mpv outputs.
    // "pexels-street-donkey-3706265-1920x1080-30fps~1.mp4"
    char media_file[] = "Cascading_water.mp4";

    // Concatenate our start up script.
    sprintf(buffer, "%s %s %s", mpv_player_com, options, media_file);

    // Send the script to the command line interpreter (console).
    // NOTE: there are many OS dependent ways to launch another application
    // both blocking and non-blocking. System is just a lazy cross platform way :)
    system(buffer);  // Synchronous blocking.

    return 0;
    }

void *Splash_nonblocking(void* p)  // Asynchronous (Multi threading).
    {
    printf("MPV Command line Tests.\nAsynchronous.\n");
    // Note: *Variable is the same as Variable[]
    #ifdef _WIN32
    //char mpv_player_com[] = "mpv.com";  // Invoke command line playback statistics.
    char mpv_player_exe[] = "mpv.exe";  // Don't open a console.
    #endif
    #ifdef __unix__ // _linux__ (__linux__)
    //char mpv_player_com[] = "mpv";  // Invoke command line playback statistics.
    char mpv_player_exe[] = "mpv";  // Don't open a console.
    #endif
    char buffer[1024] = {'\0'};
    // This first start up script works fine with mpv.exe, but Linux version does
    // not have the separate executable so we have to use "--no-terminal".
    //char *options = "--no-border --geometry=800x600 --script-opts=osc-visibility=always";  // Start options.
    char *options = "--no-border --no-terminal --geometry=800x600 --script-opts=osc-visibility=always";  // Start options.
    // https://mpv.io/manual/master/#options
    // Attempt starting MPV with other options.
    // --geometry=800x600+20+40
    // --loop-file=1 (Loop 2 times)
    // --script-opts=osc-visibility=always
    // --include=mpv.conf
    // --title="MPV Console Tests"
    // --keep-open=yes|no|always
    // --start=00:10 --length=10 --end=40
    // --pause
    // --volume=20 (0-100%)
    // --ontop
    // --border, --no-border
    // --window-maximized=<yes|no>
    // --fullscreen
    // -v (Verbose console output)
    // --msg-level=all=trace to see all messages mpv outputs.
    // "pexels-street-donkey-3706265-1920x1080-30fps~1.mp4"
    char media_file[] = "Cascading_water.mp4";

    // Concatenate our start up script.
    sprintf(buffer, "%s %s %s", mpv_player_exe, options, media_file);

    // Send the script to the command line interpreter (console).
    // NOTE: there are many OS dependent ways to launch another application
    // both blocking and non-blocking. System is just a lazy cross platform way :)
    system(buffer);  // Synchronous blocking.

    return 0;
    }

// ====> Convenience helper functions

int Con_Sleep(int seconds)
    {
    // #include <stdlib.h>
    // Cross platform sleep in seconds
#if defined(_WIN32) // Windows 32-bit and 64-bit
    seconds = seconds * 1000;
    _sleep( seconds );  // Note _sleep is deprecated
#elif defined(__unix__) // _linux__ (__linux__)
    sleep(seconds);
#endif
    return 0;
    }

// Safe Pause
void S_Pause(void)
    {
    // This function is referred to as a wrapper for S_getchar()
    printf("Press Enter to continue...");
    S_getchar();// Uses S_getchar() for safety.
    }

// Safe getchar() removes all artefacts from the stdin buffer.
int S_getchar(void)
    {
    // This function is referred to as a wrapper for getchar()
    int i = 0;
    int ret;
    int ch;

    // The following enumerates all characters in the buffer.
    while(((ch = getchar()) !='\n') && (ch != EOF ))
        {
        // But only keeps and returns the first char.
        if (i < 1)
            {
            ret = ch;
            }
        i++;
        }
    return ret;
    }
