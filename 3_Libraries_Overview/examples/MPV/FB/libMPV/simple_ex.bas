''------------------------------------------------------------------------------
'' Name:        simple_ex.bas (Based upon MPV simple.c)
'' Purpose:     Demonstrate the use of a basic graphical (windowed) multimedia
''              player using libmpv. libmpv is part of the MPV Media Player
''              application and development environment.
''
'' Title:       "MPV API Tests"
''
'' Platform:    Win64, Ubuntu64
'' Depends:     mpv-dev-x86_64-20230611 (libmpv-2.dll, libmpv.so)
''
'' Author:      Axle
'' Created:     23/06/2023
'' Updated:     26/06/2023
'' Copyright:   (c) Axle 2023
'' Licence:     MIT-0 No Attribution
''------------------------------------------------------------------------------
'' NOTES:
'' This is a simple example of how to invoke the MPV player window using libmpv.
'' This is not indifferent from using the mpv.com and mpv.exe player from the
'' command line, except with more control over the player. The player controls
'' are embedded into the built in playback window On Screen Control (OSC) so it
'' is easy to use MPV as a  simple video splash screen or basic player. libmpv
'' can also be controlled from  the calling application by making use of the OS
'' callbacks and sending commands to MPV.
'' The most ideal, and quite complex method is to embed the MPV player window,
'' or control directly within a window using a GUI or graphics library such as SDL.
''
'' Note that multimedia rendering, manipulation and playback programming can be
'' a quite challenging task where it is almost a programming speciality of its
'' own like game programming. I am only going to show the basics to get the
'' library up and running so that you have a base environment to experiment with.
'' I would also recommend gaining some experience with using the MPV CLI
'' (mpv.com and mpv.exe). Also take some time to become familiar with FFMpeg.
'' FFMpeg is a multimedia library used in many projects. MPV makes use of the
'' ffmpeg library, but adds a more user friendly layer.
''------------------------------------------------------------------------------


'' Header client.bi created by Axle FBFrog
'' (Binary .dll, .so must be provided separately in the path)
#include once "client.bi"

'' If required for other crt types and conversions.
'#include once "crt.bi"  '' libmpv gui player header.

''https://github.com/mpv-player/mpv-examples/blob/master/libmpv/README.md
''https://mpv.io/manual/master/
''https://github.com/flaviotordini/media/blob/master/src/mpv/mediampv.cpp
''https://github.com/mpv-player/mpv-examples/blob/master/libmpv/streamcb/simple-streamcb.c

''https://github.com/mpv-player/mpv/blob/master/libmpv/client.h <-###
''https://github.com/mpv-player/mpv-examples/tree/master/libmpv <-###

Declare Function main_procedure() As Integer
Declare Sub check_error(Byval status As Long)

Declare Function Con_Pause() As Integer

main_procedure()

Function main_procedure() As Integer  ' Main procedure
    
    '' Start the MPV library instance libmpv-2.dll/libmpv.so
	Dim ctx As mpv_handle Ptr = mpv_create()
	If ctx = 0 Then
		Print "failed creating context."
		Return 1
    End If
    
    '' You can call mpv_set_property() (or mpv_set_property_string() and
    '' * other variants ( before mpv 0.21.0 mpv_set_option())
    
    '' --include=<configuration-file>
    '' Many of these options can be configured in the configuration files.
    '' https://mpv.io/manual/stable/#configuration-files
    '' https://mpv.io/manual/stable/#options
    '' https://github.com/mpv-player/mpv-examples/tree/master/libmpv#where-are-the-docs
    '' https://mpv.io/manual/master/#list-of-input-commands  <-###
	check_error(mpv_set_option_string(ctx, "include", "mpv.conf"))
    
    '' Enable default key bindings, so the user can actually interact with
    '' the player (and e.g. close the window).
	check_error(mpv_set_option_string(ctx, "input-default-bindings", "yes"))
	check_error(mpv_set_option_string(ctx, "input-vo-keyboard", "yes"))
    '' Set the Window title.
	check_error(mpv_set_option_string(ctx, "title", "MPV API Tests"))
    '' Keep the window open. If there is no video playing mpv will send
    '' MPV_EVENT_NONE and the window will close automatically without interactive
    '' control to close the application. This can be handled in other ways when
    '' using a GUI window from a graphics library instead of the built in
    '' window.
	check_error(mpv_set_option_string(ctx, "keep-open", "yes"))
    '' Set the static window dimensions. Default is auto size to the video playing.
	check_error(mpv_set_option_string(ctx, "geometry", "800x600"))
    '' Loop (play the video) x number of times.
	check_error(mpv_set_option_string(ctx, "loop-file", "1"))
    '' osc-visibility=always | Must be done in mpc.config, or osc.config
    '' Set the play start position.
    'check_error(mpv_set_option_string(ctx, "start", "00:10"))
    '' Set to start paused.
    'check_error(mpv_set_option_string(ctx, "pause", ""))
    '' Rotate the video.
    'check_error(mpv_set_option_string(ctx, "video-rotate", "90"))
    '' set the start volume percent.
    'check_error(mpv_set_option_string(ctx, "volume", "20"))  '' 0 - 100%
    '' Start in fullscreen mode.
    'check_error(mpv_set_option_string(ctx, "fullscreen", ""))
    '' Start with no window border or title bar.
    'check_error(mpv_set_option_string(ctx, "border", "no"))
    
    '' This is used by the internal mpv_node structure which holds an array of
    '' configurations and commands. It is an internal data structure for MPV.
    '' The following sets the on screen controls to =ON=1.
    '' This is a more advanced version of mpv_set_option_string()
	Dim val1 As Integer = 1
	check_error(mpv_set_option(ctx, "osc", MPV_FORMAT_FLAG, @val1))
    
    '' Done setting up options.
    
    '' Starts the MPV Player instance.
	check_error(mpv_initialize(ctx))
    
    '' mpv_command() controls the MPV player from here on.
    
    '' Create String/zSring literals for constructing the mpvPlayer command (cmd1()).
    '' Note that you need to select the correct conversions between String/zString.
    '' cmd1() is sent to mpv_command() as a zString Ptr Array() so all String
    '' types must be converted to zString Ptr type. aka we can use zString for everything
    '' or we can convert FB Strings to zString before sending the array to MPV
    '' as arguments.
    
    '' Play this file. NULL a terminator for the list of strings (aka END of commands).
    '' Commands are taken as a comma separated list of commands (*arg) similar to above.
    'Dim As zstring * 128 loadfile = "loadfile"  '' Use @loadfile
    Dim As String loadfile = "loadfile"  '' Use StrPtr(loadfile)
    
    '' "pexels-street-donkey-3706265-1920x1080-30fps~1.mp4"
	'dim As zstring * 128 filename = "Cascading_water.mp4"  '' Use @filename
    Dim As String filename = "Cascading_water.mp4"  '' Use StrPtr(filename)
    
    'Dim As Any Ptr NULL = 0  '' NULL is already defined in BF C.
    'Dim As zstring ptr none = NULL  '' Variable "none" as NULL*
    'Const As Any Ptr none = 0  '' Variable defined as NULL*
    Dim As String none = ""  '' Empty string {'\0'} StrPtr("") = NULL* or DEC0
    
    
    'dim As zString Ptr cmd1(2) = {@"loadfile", @"Cascading_water.mp4", NULL}  '' Using @String. (Not the same as using String literal)
    'Dim As zString Ptr cmd1(2) = {@loadfile, @filename, none}  '' From zString literal (aka char array[])
    'Dim As zString Ptr cmd1(0 To ...) = {@loadfile, @filename, none}  '' From zString literal (aka char array[])
    'Dim As zString Ptr cmd1(0 To ...) = {Strptr(loadfile), Strptr(filename), Strptr(none)}  '' From String literal cast to zString Ptr
    
    '' Create a String array() from String literals.
    Dim As String cmd(0 To ...) = {loadfile, filename, none}  '' As String Array() from String literal.
    Dim as zString Ptr cmd1(2)  '' Create zString Ptr Array()
    Dim As Integer j = 0
    '' Convert String array() to zSring Ptr Array()
    For j = 0 To 2 Step +1
        cmd1(j) = Strptr(cmd(j))  '' Covert String element to zSring Ptr element.
        'Print *cast(zstring ptr, cmd1(j))  '' DEBUG check valid zString Ptr Array()
    Next j
    
    'Print "DEBUG zString mpv Player cmd"  '' DEBUG Check is valid zString Ptr Array()
    'For j = 0 To 2 Step +1  '' DEBUG
    '    'Print "Element:"; j; " |"; *cmd1(j); "|"  '' DEBUG
    '    Print "Element:"; j; " |"; *cast(zstring ptr, cmd1(j)); "|"  '' DEBUG
    'Next j  '' DEBUG
    'Con_Pause()  '' DEBUG Pause for testing zString array() construction.
    
    ''
    ' Send a command to the player. Commands are the same as those used in
    ' input.conf, except that this function takes parameters in a pre-split
    ' form.
    '
    ' The commands and their parameters are documented in input.rst.
    '
    ' Does not use OSD and string expansion by default (unlike mpv_command_string()
    ' and input.conf).
    '
    ' @param[in] args NULL-terminated list of strings. Usually, the first item
    '                 is the command, and the following items are arguments.
    ' @return error code
    ''
    ''MPV_EXPORT int mpv_command(mpv_handle *ctx, const char **args);
    check_error(mpv_command(ctx, @cmd1(0)))  '' <- this is working.
    
    
    'Sleep 5000, 1  '' Alow the MPV player to load before sending commands to
    '' the active player. This would normally occur in the main application loop.
    '' eg. case: on ID_BUTTON_DOWN; Do "pause';
    'Dim As zString Ptr cmd2(2) = {@"cycle", @"pause", Strptr(none)}
    'check_error(mpv_command(ctx, @cmd2(0)))
    
    '' Sending additional control commands should be done from a separate thread
    '' to the generic call backs in the loop below. It is expected that libmpv
    '' will be used within the context of a GUI or graphical windowing environment.
    '' SEE asynchronous API, mpv_command_async().
    '' https://mpv.io/manual/master/#list-of-input-commands
    '' SEE: synchronous vs asynchronous
    '' Asynchronous is a non-blocking architecture, passes arguments and continues without waiting.
    '' Synchronous is a blocking architecture, passes arguments and waits for return before continuing.
    
    '' script-opts=osc-visibility=always
    'const char *cmd3[] = {"script-opts=osc-visibility", "always", NULL};  // argv[1]
    'check_error(mpv_command(ctx, cmd3));
    
    '' Let it play, and wait until the user quits.
    Dim do_task As Integer = 1  '' True/False flag to end main loop/application.
	While do_task
        
        '' It is also possible to integrate the client API...
        ''  * usage in other event loops (e.g. GUI toolkits) with the
        ''  * mpv_set_wakeup_callback() function, and then polling for events by calling
        ''  * mpv_wait_event() with a 0 timeout.
        
        '' Event IDs must be captured from the OS event queue or a Graphics,
        '' GUI toolkit event queue. This applies to all keyboard and mouse events.
        '' In this case the OSC would be switched to off I suspect.
        
        '' mpv_event,[mpv_event_id event_id, int error, uint64_t reply_userdata, oid *data]
		Dim event As mpv_event Ptr = mpv_wait_event(ctx, 10000)  '' Get any queued MPV events.
		Print "event: "; *Cast(zString Ptr, mpv_event_name(event->event_id))  '' DEBUG
        '' This loop only monitors a small amount of event data. Keycode and mouse
        '' events are not included and will need to be acquired separately.
        '' Avoid sending mpv_command() from this loop, but instead create a separate
        '' thread for this task. See: pthreads.h, or if using a Graphics/GUI library
        '' you may be able to use the threading API as well as keyboard event
        '' handling from that library.
        '' SEE: synchronous vs asynchronous
        Select Case event->event_id
        
        Case MPV_EVENT_NONE  ''              = 0
            Print "MPV_EVENT_NONE"  '' DEBUG
            ' Happens when the player quits. Use in keep-open mode to access
            '' the MPV_EVENT_SHUTDOWN, or do a shutdown task with mpv_destroy()
            '' or mpv_terminate_destroy().
            ''printf("event: %s\n", *cast(zstring ptr, mpv_event_name(event->event_id)));  '' DEBUG
            do_task = 0  '' Set while flag to 0 and end application.
            Exit Select
            
        Case MPV_EVENT_SHUTDOWN  ''          = 1
            Print "MPV_EVENT_SHUTDOWN"  '' DEBUG
            ''printf("event: %s\n", *cast(zstring ptr, mpv_event_name(event->event_id)));  '' DEBUG
            do_task = 0  '' Set while flag to 0 and end application.
            Exit Select
            
        Case MPV_EVENT_LOG_MESSAGE  ''       = 2
            Print "MPV_EVENT_LOG_MESSAGE"  '' DEBUG
            ''/printf("event: %s\n", *cast(zstring ptr, mpv_event_name(event->event_id)));  '' DEBUG
            Exit Select
            
        Case MPV_EVENT_GET_PROPERTY_REPLY  '' = 3
            Print "MPV_EVENT_GET_PROPERTY_REPLY"  '' DEBUG
            ''printf("event: %s\n", *cast(zstring ptr, mpv_event_name(event->event_id)));  '' DEBUG
            Exit Select
            
        Case MPV_EVENT_SET_PROPERTY_REPLY  '' = 4
            Print "MPV_EVENT_NONE"  '' DEBUG
            ''printf("event: %s\n", *cast(zstring ptr, mpv_event_name(event->event_id)));  '' DEBUG
            Exit Select
            
        Case MPV_EVENT_COMMAND_REPLY  ''     = 5
            Print "MPV_EVENT_SET_PROPERTY_REPLY"  '' DEBUG
            ''printf("event: %s\n", *cast(zstring ptr, mpv_event_name(event->event_id)));  '' DEBUG
            Exit Select
            
        Case MPV_EVENT_START_FILE  ''        = 6
            Print "MPV_EVENT_START_FILE"  '' DEBUG
            ''printf("event: %s\n", *cast(zstring ptr, mpv_event_name(event->event_id)));  '' DEBUG
            Exit Select
            
        Case MPV_EVENT_END_FILE  ''          = 7
            Print "MPV_EVENT_END_FILE"  '' DEBUG
            ''printf("event: %s\n", *cast(zstring ptr, mpv_event_name(event->event_id)));  '' DEBUG
            ''do_task = 0
            Exit Select
            
        Case MPV_EVENT_FILE_LOADED  ''       = 8
            Print "MPV_EVENT_FILE_LOADED"  '' DEBUG
            ''printf("event: %s\n", *cast(zstring ptr, mpv_event_name(event->event_id)));  '' DEBUG
            Exit Select
            
            #if MPV_ENABLE_DEPRECATED
        Case MPV_EVENT_IDLE  ''              = 11
            Print "MPV_EVENT_IDLE"  '' DEBUG
            ''printf("event: %s\n", *cast(zstring ptr, mpv_event_name(event->event_id)));  '' DEBUG
            Exit Select
            
        Case MPV_EVENT_TICK  ''              = 14
            Print "MPV_EVENT_TICK"  '' DEBUG
            ''printf("event: %s\n", *cast(zstring ptr, mpv_event_name(event->event_id)));  '' DEBUG
            Exit Select
            #endif
            
        Case MPV_EVENT_CLIENT_MESSAGE  ''    = 16
            Print "MPV_EVENT_CLIENT_MESSAGE"  '' DEBUG
            ''printf("event: %s\n", *cast(zstring ptr, mpv_event_name(event->event_id)));  '' DEBUG
            Exit Select
            
        Case MPV_EVENT_VIDEO_RECONFIG  ''    = 17
            Print "MPV_EVENT_VIDEO_RECONFIG"  '' DEBUG
            ''printf("event: %s\n", *cast(zstring ptr, mpv_event_name(event->event_id)));  '' DEBUG
            Exit Select
            
        Case MPV_EVENT_AUDIO_RECONFIG  ''    = 18
            Print "MPV_EVENT_AUDIO_RECONFIG"  '' DEBUG
            ''printf("event: %s\n", *cast(zstring ptr, mpv_event_name(event->event_id)));  '' DEBUG
            Exit Select
            
        Case MPV_EVENT_SEEK  ''              = 20
            Print "MPV_EVENT_SEEK"  '' DEBUG
            ''printf("event: %s\n", *cast(zstring ptr, mpv_event_name(event->event_id)));  '' DEBUG
            Exit Select
            
        Case MPV_EVENT_PLAYBACK_RESTART  ''  = 21
            Print "MPV_EVENT_PLAYBACK_RESTART"  '' DEBUG
            ''printf("event: %s\n", *cast(zstring ptr, mpv_event_name(event->event_id)));  '' DEBUG
            Exit Select
            
        Case MPV_EVENT_PROPERTY_CHANGE  ''   = 22
            Print "MPV_EVENT_PROPERTY_CHANGE"  '' DEBUG
            ''printf("event: %s\n", *cast(zstring ptr, mpv_event_name(event->event_id)));  '' DEBUG
            Exit Select
            
        Case MPV_EVENT_QUEUE_OVERFLOW:  ''    = 24
        Print "MPV_EVENT_QUEUE_OVERFLOW"  '' DEBUG
        ''printf("event: %s\n", *cast(zstring ptr, mpv_event_name(event->event_id)));  '' DEBUG
        Exit Select
        
    Case MPV_EVENT_HOOK  ''              = 25
        Print "MPV_EVENT_HOOK"  '' DEBUG
        ''printf("event: %s\n", *cast(zstring ptr, mpv_event_name(event->event_id)));  '' DEBUG
        Exit Select
        
    Case Else
        Print "Default. No Value."
        ''printf("event: %s\n", *cast(zstring ptr, mpv_event_name(event->event_id)));  '' DEBUG
        Exit Select
    End Select
Wend

mpv_terminate_destroy(ctx)  '' Shut down all libmpv(dll/so) contexts.

Con_Pause()  '' DEBUG Pause
Return 0
End Function  ' END main_procedure <---


'' Error check routine. 
Sub check_error(Byval status As Long)
	If status < 0 Then
		Print "mpv API error: "; *Cast(zString Ptr, mpv_error_string(status))  '' DEBUG
        '' Handle error values appropriately...
        End 1
		'Return 1
    End If
End Sub


'' Console Pause (GetKey version)
Function Con_Pause() As Integer
    Dim As Long dummy
    Print !"\nPress any key to continue..."
    dummy = Getkey
    Return 0
End Function
