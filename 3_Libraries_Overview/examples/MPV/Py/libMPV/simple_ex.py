#------------------------------------------------------------------------------
# Name:        simple_exe.py
# Purpose:     Demonstrate the use of a basic graphical (windowed) multimedia
#              player using libmpv. libmpv is part of the MPV Media Player
#              application and development environment.
#
# Title:       "MPV API Tests"
#
# Platform:    Win64, Ubuntu64
# Depends:     mpv-dev-x86_64-20230611 (libmpv-2.dll, libmpv.so), mpv.py
# Requires:    mpv.py, libmpv-2.dll|so must be in the application directory or
#              the system path.
#
# Author:      Axle
# Created:     04/07/2023
# Updated:
# Copyright:   (c) Axle 2023
# Licence:     MIT-0 No Attribution
#------------------------------------------------------------------------------
# NOTES:
# This is a simple example of how to invoke the MPV player window using libmpv.
# This is not indifferent from using the mpv.com and mpv.exe player from the
# command line, except with more control over the player. The player controls
# are embedded into the built in playback window On Screen Control (OSC) so it
# is easy to use MPV as a simple video splash screen or basic player. libmpv
# can also be controlled from  the calling application by making use of the OS
# callbacks and sending commands to MPV.
# The most ideal, and quite complex method is to embed the MPV player window,
# or control directly within a window using a GUI or graphics library such as SDL.
#
# Note that multimedia rendering, manipulation and playback programming can be
# a quite challenging task where it is almost a programming speciality of its
# own like game programming. I am only going to show the basics to get the
# library up and running so that you have a base environment to experiment with.
# I would also recommend gaining some experience with using the MPV CLI
# (mpv.com and mpv.exe). Also take some time to become familiar with FFMpeg.
# FFMpeg is a multimedia library used in many projects. MPV makes use of the
# ffmpeg library, but adds a more user friendly layer.
#
# At this time the mpv player callback routine is not implemented. mpv.py
# uses a different API to the official libmpv and documentation is scarce.
#------------------------------------------------------------------------------

import os
import sys
import time

#https://github.com/jaseg/python-mpv
## Place mpv.py in the project directory next to you python script.
## Change the following line in "mpv.py' Line: 38 as ctypes.util.find_library()
## cannot find the libmpv.dll on Windows.
## dll = ctypes.util.find_library('mpv-2.dll') or ctypes.util.find_library('mpv-1.dll')
## dll = os.path.join(sys.path[0], "libmpv-2.dll")

import mpv


def main():

    # Warn that it is best to run from the OS console.
    if 1 == Con_IsREPL():
        print("This application is best viewed running from the OS Command interpreter")
        Con_Pause()

    # "pexels-street-donkey-3706265-1920x1080-30fps~1.mp4"
    filename = "Cascading_water.mp4"

    # Start the MPV library instance libmpv-2.dll/libmpv.so
    try:
        ctx = mpv.MPV()  # config='yes'|True
    except:
        print("failed creating context.")
        return 1

    #mpv_set_option_string(ctx, "include", "mpv.conf"))
    # The following OPTIONS and Properties command are a little ambiguous.
    # OPTIONS are typically set before player start and
    # Properties during playback.
    # Note the hyphen vs underscore.
    #ctx['some-option'] = "yes|no", True|False  # From OPTIONS (Startup?)
    #ctx.some_property = "yes|no", True|False  # From Properties? (runtime?)
    # Many of these options can be configured in the configuration files.
    #ctx.include = 'Drive:\\FullPath\\mpv.conf'
    ctx.include = 'mpv.conf'

    # Enable default key bindings, so the user can actually interact with
    # the player (and e.g. close the window).
    #ctx['input-default-bindings'] = True  # 'yes' ?
    ctx.input_default_bindings = 'yes'
    #ctx['input-vo-keyboard'] = True  # 'yes' ?
    ctx.input_vo_keyboard = True  # 'yes' ? (input-vo-keyboard -> input_vo_keyboard)
    # Set the Window title.
    #ctx['title'] = 'MPV API Tests'
    ctx.title = 'MPV API Tests'
    # The following currently fails.
    #ctx['focus-on-open']
    ctx.focus_on_open = True  # ??
    #ctx['ontop']
    # Keep the window open. If there is no video playing mpv will send
    # MPV_EVENT_NONE and the window will close automatically without interactive
    # control to close the application. This can be handled in other ways when
    # using a GUI window from a graphics library instead of the built in
    # window
    #ctx['keep-open'] = 'yes'
    ctx.keep_open = True  # 'yes'
    # Set the static window dimensions. Default is auto size to the video playing.
    #ctx['geometry'] = '800x600'
    ctx.geometry = '800x600'
    # Loop (play the video) x number of times.
    #ctx['loop-file'] = '1'  # 0, 1 = 2 times, 2 = 3 times.
    ctx.loop_file = '1'  # 0, 1 = 2 times, 2 = 3 times.
    # osc-visibility=always | Must be done in mpc.config, or osc.config
    # In mpv.py must be set --osc=True to show the OSC.
    #ctx['osc'] = True  # 1
    ctx.osc = True
    # Set the play start position.
    #ctx['start'] = "00:10"
    #ctx.start = "00:10"
    # Set to start paused.
    #ctx['pause'] = ''
    #ctx.pause = ''
    # Rotate the video.
    #ctx['video-rotate'] = '90'
    #ctx.video_rotate = '90'
    # set the start volume percent.
    #ctx['volume'] = '20'  # 0 - 100%
    #ctx.volume = '20'  # 0 - 100%
    # Start in fullscreen mode.
    #ctx['fullscreen'] = ''
    #ctx.fullscreen = ''

    # Done setting up start options.

    # Starts the MPV Player instance.
    ctx.play(filename)
    ctx.wait_until_playing() # <- Yes, Non Blocking
    #ctx.wait_for_playback()  # Wait Blocking
    #https://mpv.readthedocs.io/en/latest/api.html#mpv.Mpv.command
    #https://docs.platypush.tech/_modules/platypush/plugins/media/mpv.html

    # mpv_command() controls the MPV player from here on. Not yet implemented.

    # Sending additional control commands should be done from a separate thread
    # to the generic call backs in the loop below. It is expected that libmpv
    # will be used within the context of a GUI or graphical windowing environment.
    # SEE asynchronous API, mpv_command_async().
    # https://mpv.io/manual/master/#list-of-input-commands
    # SEE: synchronous vs asynchronous
    # Asynchronous is a non-blocking architecture, passes arguments and continues without waiting.
    # Synchronous is a blocking architecture, passes arguments and waits for return before continuing.

    # Let it play, and wait until the user quits.
    do_task = 1  # Flag to set end while on MPV_EVENT_SHUTDOWN.
    count = 0
    ## MPV Player callbacks not yet implemented.
    while (do_task):

        ## DEBUG routine
        time.sleep(1)
        print(count)
        count += 1
        if count > 5:
            do_task = 0

    #ctx.wait_for_playback()  # Wait Blocking (aka wait for player to finish).
    #ctx.detach_destroy()
    #ctx.terminate_destroy()
    ctx.terminate()  # Shut down player and all libmpv(dll/so) contexts.

    Con_Pause()
    return None
    ## END main()

## Not implemented.
def check_error(status):
    if (status < 0):
        print("mpv API error: ", mpv_error_string(status))  ## TODO
        sys.exit(1)

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
