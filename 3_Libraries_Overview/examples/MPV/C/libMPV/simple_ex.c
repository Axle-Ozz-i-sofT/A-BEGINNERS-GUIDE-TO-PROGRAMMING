//------------------------------------------------------------------------------
// Name:        simple_ex.c
// Purpose:     Demonstrate the use of a basic graphical (windowed) multimedia
//              player using libmpv. libmpv is part of the MPV Media Player
//              application and development environment.
//
// Title:       "MPV API Tests"
//
// Platform:    Win64, Ubuntu64
//
// Compiler:    GCC V9.x.x, MinGw-64, libc (ISO C99)
// Depends:     mpv-dev-x86_64-20230611 (libmpv-2.dll, libmpv.so)
// Requires:
//
// Author:      Axle
// Created:     20/06/2023
// Updated:
// Version:     0.0.1.0
// Copyright:   (c) Axle 2022
// Licence:     MIT-0 No Attribution
//------------------------------------------------------------------------------
// NOTES:
// This is a simple example of how to invoke the MPV player window using libmpv.
// This is not indifferent from using the mpv.com and mpv.exe player from the
// command line, except with more control over the player. The player controls
// are embedded into the built in playback window On Screen Control (OSC) so it
// is easy to use MPV as a simple video splash screen or basic player. libmpv
// can also be controlled from  the calling application by making use of the OS
// callbacks and sending commands to MPV.
// The most ideal, and quite complex method is to embed the MPV player window,
// or control directly within a window using a GUI or graphics library such as SDL.
//
// Note that multimedia rendering, manipulation and playback programming can be
// a quite challenging task where it is almost a programming speciality of its
// own like game programming. I am only going to show the basics to get the
// library up and running so that you have a base environment to experiment with.
// I would also recommend gaining some experience with using the MPV CLI
// (mpv.com and mpv.exe). Also take some time to become familiar with FFMpeg.
// FFMpeg is a multimedia library used in many projects. MPV makes use of the
// ffmpeg library, but adds a more user friendly layer.
//------------------------------------------------------------------------------
// Build with: gcc -o simple simple.c `pkg-config --libs --cflags mpv`

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(_WIN32) // Windows 32-bit and 64-bit
    #include "client.h"  // libmpv gui player header.
#elif defined(__unix__) // _linux__ (__linux__)
    #include <mpv/client.h>  // libmpv gui player header.
#endif



//https://github.com/mpv-player/mpv-examples/blob/master/libmpv/README.md
//https://mpv.io/manual/master/
//https://github.com/flaviotordini/media/blob/master/src/mpv/mediampv.cpp
//https://github.com/mpv-player/mpv-examples/blob/master/libmpv/streamcb/simple-streamcb.c

//https://github.com/mpv-player/mpv/blob/master/libmpv/client.h <-###
//https://github.com/mpv-player/mpv-examples/tree/master/libmpv <-###

static inline void check_error(int status);

int Con_Sleep(int seconds);
// Safe Pause
void S_Pause(void);
// Safe getchar() removes all artefacts from the stdin buffer.
int S_getchar(void);


int main(int argc, char *argv[])
    {
    /*
    if (argc != 2) {
        printf("pass a single media file as argument\n");
        return 1;
    }
    */

    // "pexels-street-donkey-3706265-1920x1080-30fps~1.mp4"
    const char *filename = "Cascading_water.mp4";

    // Start the MPV library instance libmpv-2.dll/libmpv.so
    mpv_handle *ctx = mpv_create();
    if (!ctx)
        {
        printf("failed creating context.\n");
        return 1;
        }

    // You can call mpv_set_property() (or mpv_set_property_string() and
    // * other variants ( before mpv 0.21.0 mpv_set_option())

    // --include=<configuration-file>
    // Many of these options can be configured in the configuration files.
    // https://mpv.io/manual/stable/#configuration-files
    // https://mpv.io/manual/stable/#options
    // https://github.com/mpv-player/mpv-examples/tree/master/libmpv#where-are-the-docs
    // https://mpv.io/manual/master/#list-of-input-commands  <-###
    check_error(mpv_set_option_string(ctx, "include", "mpv.conf"));
    // Enable default key bindings, so the user can actually interact with
    // the player (and e.g. close the window).
    check_error(mpv_set_option_string(ctx, "input-default-bindings", "yes"));
    check_error(mpv_set_option_string(ctx, "input-vo-keyboard", "yes"));
    // Set the Window title.
    check_error(mpv_set_option_string(ctx, "title", "MPV API Tests"));
    // Keep the window open. If there is no video playing mpv will send
    // MPV_EVENT_NONE and the window will close automatically without interactive
    // control to close the application. This can be handled in other ways when
    // using a GUI window from a graphics library instead of the built in
    // window.
    check_error(mpv_set_option_string(ctx, "keep-open", "yes"));
    // Set the static window dimensions. Default is auto size to the video playing.
    check_error(mpv_set_option_string(ctx, "geometry", "800x600"));
    // Loop (play the video) x number of times.
    check_error(mpv_set_option_string(ctx, "loop-file", "1"));  // 0, 1 = 2 times, 2 = 3 times.
    // osc-visibility=always | Must be done in mpc.config, or osc.config
    // Set the play start position.
    //check_error(mpv_set_option_string(ctx, "start", "00:10"));
    // Set to start paused.
    //check_error(mpv_set_option_string(ctx, "pause", ""));
    // Rotate the video.
    //check_error(mpv_set_option_string(ctx, "video-rotate", "90"));
    // set the start volume percent.
    //check_error(mpv_set_option_string(ctx, "volume", "20"));  // 0 - 100%
    // Start in fullscreen mode.
    //check_error(mpv_set_option_string(ctx, "fullscreen", ""));

    // This is used by the internal mpv_node structure which holds an array of
    // configurations and commands. It is an internal data structure for MPV.
    // The following sets the on screen controls to =ON=1.
    // This is a more advanced version of mpv_set_option_string()
    int val1 = 1;
    check_error(mpv_set_option(ctx, "osc", MPV_FORMAT_FLAG, &val1));

    // Done setting up start options.

    // Starts the MPV Player instance.
    check_error(mpv_initialize(ctx));
    // mpv_command() controls the MPV player from here on.

    // Play this file. NULL a terminator for the list of strings (aka END of commands).
    // Commands are taken as a comma separated list of commands (*arg) similar to above.
    const char *cmd1[] = {"loadfile", filename, NULL};  // argv[1]

    /*
     * Send a command to the player. Commands are the same as those used in
     * input.conf, except that this function takes parameters in a pre-split
     * form.
     *
     * The commands and their parameters are documented in input.rst.
     *
     * Does not use OSD and string expansion by default (unlike mpv_command_string()
     * and input.conf).
     *
     * @param[in] args NULL-terminated list of strings. Usually, the first item
     *                 is the command, and the following items are arguments.
     * @return error code
     */
    //MPV_EXPORT int mpv_command(mpv_handle *ctx, const char **args);

    check_error(mpv_command(ctx, cmd1));

    //_sleep(5000);
    //const char *cmd2[] = {"cycle", "pause", NULL};
    //check_error(mpv_command(ctx, cmd2));

    // Sending additional control commands should be done from a separate thread
    // to the generic call backs in the loop below. It is expected that libmpv
    // will be used within the context of a GUI or graphical windowing environment.
    // SEE asynchronous API, mpv_command_async().
    // https://mpv.io/manual/master/#list-of-input-commands
    // SEE: synchronous vs asynchronous
    // Asynchronous is a non-blocking architecture, passes arguments and continues without waiting.
    // Synchronous is a blocking architecture, passes arguments and waits for return before continuing.

    // script-opts=osc-visibility=always
    //const char *cmd3[] = {"script-opts=osc-visibility", "always", NULL};  // argv[1]
    //check_error(mpv_command(ctx, cmd3));

    // Let it play, and wait until the user quits.
    int do_task = 1;  // Flag to set end while on MPV_EVENT_SHUTDOWN.
    while (do_task)
        {

        // It is also possible to integrate client API
        //  * usage in other event loops (e.g. GUI toolkits) with the
        //  * mpv_set_wakeup_callback() function, and then polling for events by calling
        //  * mpv_wait_event() with a 0 timeout.

        // Event IDs must be captured from the OS event queue or a Graphics,
        // GUI toolkit event queue. This applies to all keyboard and mouse events.
        // In this case the OSC would be switched to off I suspect.

        // mpv_event,[mpv_event_id event_id, int error, uint64_t reply_userdata, oid *data]
        mpv_event *event = mpv_wait_event(ctx, 10000);

        printf("event: %s\n", mpv_event_name(event->event_id));  // DEBUG
        // This loop only monitors a small amount of event data. Keycode and mouse
        // events are not included and will need to be acquired separately.
        // Avoid sending mpv_command() from this loop, but instead create a separate
        // thread for this task. See: pthreads.h, or if using a Graphics/GUI library
        // you may be able to use the threading API as well as keyboard event
        // handling from that library.
        // SEE: synchronous vs asynchronous
        switch (event->event_id)
            {
            case MPV_EVENT_NONE:  //              = 0
                printf("MPV_EVENT_NONE\n");  // DEBUG
                // Happens when the player quits. Use in keep-open mode to access
                // the MPV_EVENT_SHUTDOWN, or do a shutdown task with mpv_destroy()
                // or mpv_terminate_destroy().
                //printf("event: %s\n", mpv_event_name(event->event_id));  // DEBUG
                do_task = 0;
                break;

            case MPV_EVENT_SHUTDOWN:  //          = 1
                printf("MPV_EVENT_SHUTDOWN\n");  // DEBUG
                //printf("event: %s\n", mpv_event_name(event->event_id));  // DEBUG
                do_task = 0;
                break;

            case MPV_EVENT_LOG_MESSAGE:  //       = 2
                printf("MPV_EVENT_LOG_MESSAGE\n");  // DEBUG
                //printf("event: %s\n", mpv_event_name(event->event_id));  // DEBUG
                break;

            case MPV_EVENT_GET_PROPERTY_REPLY:  // = 3
                printf("MPV_EVENT_GET_PROPERTY_REPLY\n");  // DEBUG
                //printf("event: %s\n", mpv_event_name(event->event_id));  // DEBUG
                break;

            case MPV_EVENT_SET_PROPERTY_REPLY:  // = 4
                printf("MPV_EVENT_NONE\n");  // DEBUG
                //printf("event: %s\n", mpv_event_name(event->event_id));  // DEBUG
                break;

            case MPV_EVENT_COMMAND_REPLY:  //     = 5
                printf("MPV_EVENT_SET_PROPERTY_REPLY\n");  // DEBUG
                //printf("event: %s\n", mpv_event_name(event->event_id));  // DEBUG
                break;

            case MPV_EVENT_START_FILE:  //        = 6
                printf("MPV_EVENT_START_FILE\n");  // DEBUG
                //printf("event: %s\n", mpv_event_name(event->event_id));  // DEBUG
                break;

            case MPV_EVENT_END_FILE:  //          = 7
                printf("MPV_EVENT_END_FILE\n");  // DEBUG
                //printf("event: %s\n", mpv_event_name(event->event_id));  // DEBUG
                //do_task = 0;
                break;

            case MPV_EVENT_FILE_LOADED:  //       = 8
                printf("MPV_EVENT_FILE_LOADED\n");  // DEBUG
                //printf("event: %s\n", mpv_event_name(event->event_id));  // DEBUG
                break;

#if MPV_ENABLE_DEPRECATED
            case MPV_EVENT_IDLE:  //              = 11
                printf("MPV_EVENT_IDLE\n");  // DEBUG
                //printf("event: %s\n", mpv_event_name(event->event_id));  // DEBUG
                break;

            case MPV_EVENT_TICK:  //              = 14
                printf("MPV_EVENT_TICK\n");  // DEBUG
                //printf("event: %s\n", mpv_event_name(event->event_id));  // DEBUG
                break;
#endif

            case MPV_EVENT_CLIENT_MESSAGE:  //    = 16
                printf("MPV_EVENT_CLIENT_MESSAGE\n");  // DEBUG
                //printf("event: %s\n", mpv_event_name(event->event_id));  // DEBUG
                break;

            case MPV_EVENT_VIDEO_RECONFIG:  //    = 17
                printf("MPV_EVENT_VIDEO_RECONFIG\n");  // DEBUG
                //printf("event: %s\n", mpv_event_name(event->event_id));  // DEBUG
                break;

            case MPV_EVENT_AUDIO_RECONFIG:  //    = 18
                printf("MPV_EVENT_AUDIO_RECONFIG\n");  // DEBUG
                //printf("event: %s\n", mpv_event_name(event->event_id));  // DEBUG
                break;

            case MPV_EVENT_SEEK:  //              = 20
                printf("MPV_EVENT_SEEK\n");  // DEBUG
                //printf("event: %s\n", mpv_event_name(event->event_id));  // DEBUG
                break;

            case MPV_EVENT_PLAYBACK_RESTART:  //  = 21
                printf("MPV_EVENT_PLAYBACK_RESTART\n");  // DEBUG
                //printf("event: %s\n", mpv_event_name(event->event_id));  // DEBUG
                break;

            case MPV_EVENT_PROPERTY_CHANGE:  //   = 22
                printf("MPV_EVENT_PROPERTY_CHANGE\n");  // DEBUG
                //printf("event: %s\n", mpv_event_name(event->event_id));  // DEBUG
                break;

            case MPV_EVENT_QUEUE_OVERFLOW:  //    = 24
                printf("MPV_EVENT_QUEUE_OVERFLOW\n");  // DEBUG
                //printf("event: %s\n", mpv_event_name(event->event_id));  // DEBUG
                break;

            case MPV_EVENT_HOOK:  //              = 25
                printf("MPV_EVENT_HOOK\n");  // DEBUG
                //printf("event: %s\n", mpv_event_name(event->event_id));  // DEBUG
                break;

            default:
                printf("Defualt. No Value.\n");
                //printf("event: %s\n", mpv_event_name(event->event_id));  // DEBUG
                break;

            }  // END while
        /*
        mpv_event *event = mpv_wait_event(ctx, 10000);
        printf("event: %s\n", mpv_event_name(event->event_id));
        if (event->event_id == MPV_EVENT_SHUTDOWN)
            {
            break;
            }
        }
        */
        }

    mpv_terminate_destroy(ctx);  // Shut down all libmpv(dll/so) contexts.
    
    S_Pause();
    return 0;
    }

static inline void check_error(int status)
    {
    if (status < 0)
        {
        printf("mpv API error: %s\n", mpv_error_string(status));
        exit(1);
        }
    }

// ====> Convenience helper functions

int Con_Sleep(int seconds)
    {
    // #include <stdlib.h>
    // Cross platform sleep in seconds
#ifdef _WIN32 // Windows 32-bit and 64-bit
    seconds = seconds * 1000;
    _sleep( seconds );  // Note _sleep is deprecated
#endif
#ifdef __unix__ // _linux__ (__linux__)
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
