#*******************************************************************************************
#
#   raygui - controls test suite
#
#   TEST CONTROLS:
#       - GuiDropdownBox()
#       - GuiCheckBox()
#       - GuiSpinner()
#       - GuiValueBox()
#       - GuiTextBox()
#       - GuiButton()
#       - GuiComboBox()
#       - GuiListView()
#       - GuiToggleGroup()
#       - GuiTextBoxMulti()
#       - GuiColorPicker()
#       - GuiSlider()
#       - GuiSliderBar()
#       - GuiProgressBar()
#       - GuiColorBarAlpha()
#       - GuiScrollPanel()
#
#
#   DEPENDENCIES:
#       raylib 4.0 - Windowing/input management and drawing.
#       raygui 3.2 - Immediate-mode GUI controls.
#
#   COMPILATION (Windows - MinGW):
#       gcc -o $(NAME_PART).exe $(FILE_NAME) -I../../src -lraylib -lopengl32 -lgdi32 -std=c99
#
#   LICENSE: zlib/libpng
#
#   Copyright (c) 2016-2022 Ramon Santamaria (@raysan5)
#
#*********************************************************************************************/
# Converted by Axle with assist from electronstudio
#  (Original C source Copyright (c) 2016-2022 Ramon Santamaria (@raysan5))
# Date: 26/10/2022
# raylib_cffi version: 4.2.1.1
# C raylib version raylib-4.2.0
# C raygui version raylib-3.2
#
## Using pyray API ##
# Variable naming = Hungarian_snake_case
# str_variable
# int_variable, int, float, complex
# list_variable, tuple, range
# dict_variable
# set_variable, frozenset
# bool_variable
# byte_variable, bytearray, memoryview
# None_variable
#
# raylib cffi types
# struct_variable
# pchar char variable[n]
# ppchar char *variable[n]
# pcint int *variable
#
# see:https://electronstudio.github.io/raylib-python-cffi/pyray.html
#
#NOTES:
# See list to C 2D array list_to_2d_c_array()
# Clean this up as a generic function

from pyray import *
##import pyray as pr

#------------------------------------------------------------------------------------
# Program main entry point
#------------------------------------------------------------------------------------
def main():

    # Initialization
    #--------------------------------------------------------------------------------------
    int_screen_width = 690
    int_screen_height = 560
    
    str_wind_title = "raygui - controls test suite"

    #init_window(int_screen_width, int_screen_height, "raygui - controls test suite")
    init_window(int_screen_width, int_screen_height, str_wind_title)
    set_exit_key(0)
    
    # GUI controls initialization
    #----------------------------------------------------------------------------------
    int_dropdown_box_000_active = 0
    bool_drop_down_000_edit_mode = False
    
    bool_dropdown_box_001_active = 0
    bool_drop_down_001_edit_mode = False
    
    #int_spinner_001_value = 0
    pcint_spinner_001_value = ffi.new("int *")
    bool_spinner_edit_mode = False
    
    #int_value_box_002_value = 0
    pcint_value_box_002_value = ffi.new("int *")
    bool_value_box_edit_mode = False
    
    #char textBoxText[64] = "Text box";
    pchar_text_box_text = ffi.new("char[]", 64)
    pchar_text_box_text = b"Text box"
    bool_text_box_edit_mode = False
    
    int_list_view_scroll_index = 0
    int_list_view_active = -1
    
    int_list_view_ex_scroll_index = 0
    int_list_view_ex_active = 2
    int_list_view_ex_focus = -1
    
    ## const char *listViewExList[8] = { "This", "is", "a", "list view", "with", "disable", "elements", "amazing!" };
    list1 = ["This", "is", "a", "list view", "with", "disable", "elements", "amazing!"]
    #list1 = "Hello world"  # will break into a array of char 
    
    ## Use the following 3 lines or use list_to_2d_c_array()
    #c_array_2d = [ffi.new("char[]", x.encode('utf-8')) for x in list1]
    #ppchar_list_view_ex_list = ffi.new("char *[]", c_array_2d)
    # keep 'items' alive as long as you need 'ppchar_list_view_ex_list'
    ppchar_list_view_ex_list = list_to_2d_c_array(list1)  ## Use this line with the function.
    
    
    #char multiTextBoxText[256] = "Multi text box";
    pchar_multi_str_text_box_text = ffi.new("char[]", 256)
    pchar_multi_str_text_box_text = b"Multi text box"
    bool_multi_bool_text_box_edit_mode = False
    struct_color_picker_value = Color(230, 41, 55, 255)  ## class pyray.Color(r, g, b, a) (C struct)
    
    int_slider_value = 50
    int_slider_bar_value = 60
    float_progress_value = 0.4
    
    float_force_squared_checked = False
    
    float_alpha_value = 0.5
    
    int_combo_box_active = 1
    
    int_toggle_group_active = 0
    
    struct_view_scroll = Vector2( 0.0, 0.0 )  ## class pyray.Vector2(x, y) (C struct)
    
    # Moved from Main game loop
    struct_dropped_files = FilePathList  # class pyray.FilePathList(capacity, count, paths) (C struct)
    
    struct_view = Rectangle(0.0, 0.0, 0.0, 0.0)  ## class pyray.Rectangle(x, y, width, height) (C struct)
    #----------------------------------------------------------------------------------
    
    # Custom GUI font loading
    #Font font = LoadFontEx("fonts/rainyhearts16.ttf", 12, 0, 0);
    #GuiSetFont(font);
    
    bool_exit_window = False
    bool_show_message_box = False
    
    #char textInput[256] = { 0 };
    pchar_text_input = ffi.new("char[]", 256)
    pchar_text_input = b""
    bool_show_text_input_box = False
    
    #char textInputFileName[256] = { 0 };
    pchar_text_input_file_name = ffi.new("char[]", 256)
    pchar_text_input_file_name = b""

    set_target_fps(60)
    #--------------------------------------------------------------------------------------
    
    # Main game loop
    while (False == bool_exit_window):    # Detect window close button or ESC key
        # Update
        #----------------------------------------------------------------------------------
        bool_exit_window = window_should_close()
    
        if (is_key_pressed(KEY_ESCAPE)):
            bool_show_message_box = not bool_show_message_box
    
        if (is_key_down(KEY_LEFT_CONTROL) and is_key_pressed(KEY_S)):
            bool_show_text_input_box = true
    
        if (is_file_dropped()):
            struct_dropped_files = load_dropped_files()
    
            if ((struct_dropped_files.count > 0) and is_file_extension(struct_dropped_files.paths[0], ".rgs")):
                gui_load_tyle(struct_dropped_files.paths[0])
    
            unload_dropped_files(struct_dropped_files)    # Clear internal buffers
    
        #----------------------------------------------------------------------------------
    
        # Draw
        #----------------------------------------------------------------------------------
        begin_drawing()
    
        ## Python does not have usigned int so we have to convert the 2's compliment by bitshift left + 1<<32
        clear_background(get_color(2**32 + (gui_get_style(DEFAULT, BACKGROUND_COLOR))))
    
        # raygui: controls drawing
        #----------------------------------------------------------------------------------
        if bool_drop_down_000_edit_mode or bool_drop_down_001_edit_mode:
            gui_lock()
        elif (not bool_drop_down_000_edit_mode) and (not bool_drop_down_001_edit_mode):
            gui_unlock()
        #gui_disable()
    
        # First GUI column
        #gui_set_style(CHECKBOX, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT)
        float_force_squared_checked = gui_check_box(Rectangle( 25, 108, 15, 15 ), "FORCE CHECK!", float_force_squared_checked)
    
        gui_set_style(TEXTBOX, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER)
        #gui_set_style(VALUEBOX, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT)
        # int_spinner_001_value requires return of *value aka by reference ? :(
        if (gui_spinner(Rectangle( 25, 135, 125, 30 ), None, pcint_spinner_001_value, 0, 100, bool_spinner_edit_mode)):
            bool_spinner_edit_mode = not bool_spinner_edit_mode
        if (gui_value_box(Rectangle( 25, 175, 125, 30 ), None, pcint_value_box_002_value, 0, 100, bool_value_box_edit_mode)):
            bool_value_box_edit_mode = not bool_value_box_edit_mode
    
        gui_set_style(TEXTBOX, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT)
    
        if (gui_text_box(Rectangle( 25, 215, 125, 30 ), pchar_text_box_text, 64, bool_text_box_edit_mode)):
            bool_text_box_edit_mode = not bool_text_box_edit_mode
    
        gui_set_style(BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER)
    
        if (gui_button(Rectangle( 25, 255, 125, 30 ), gui_icon_text(ICON_FILE_SAVE, "Save File"))):
            bool_show_text_input_box = True
    
        gui_group_box(Rectangle( 25, 310, 125, 150 ), "STATES")
        #gui_lock()
        gui_set_state(STATE_NORMAL)
        if (gui_button(Rectangle( 30, 320, 115, 30 ), "NORMAL")):
            pass
        gui_set_state(STATE_FOCUSED)
        if (gui_button(Rectangle( 30, 355, 115, 30 ), "FOCUSED")):
            pass
        gui_set_state(STATE_PRESSED)
        if (gui_button(Rectangle( 30, 390, 115, 30 ), "#15#PRESSED")):
            pass
        gui_set_state(STATE_DISABLED)
        if (gui_button(Rectangle( 30, 425, 115, 30 ), "DISABLED")):
            pass
        gui_set_state(STATE_NORMAL)
        #gui_unlock()
    
        int_combo_box_active = gui_combo_box(Rectangle( 25, 470, 125, 30 ), "ONE;TWO;THREE;FOUR", int_combo_box_active)
    
        # NOTE: gui_dropdown_box must draw after any other control that can be covered on unfolding
        gui_set_style(DROPDOWNBOX, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT)
        if (gui_dropdown_box(Rectangle( 25, 65, 125, 30 ), "#01#ONE;#02#TWO;#03#THREE;#04#FOUR", bool_dropdown_box_001_active, bool_drop_down_001_edit_mode)):
            bool_drop_down_001_edit_mode = not bool_drop_down_001_edit_mode
    
        gui_set_style(DROPDOWNBOX, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
        if (gui_dropdown_box(Rectangle( 25, 25, 125, 30 ), "ONE;TWO;THREE", int_dropdown_box_000_active, bool_drop_down_000_edit_mode)):
            bool_drop_down_000_edit_mode = not bool_drop_down_000_edit_mode
    
        # Second GUI column
        int_list_struct_view_active = gui_list_view(Rectangle( 165, 25, 140, 140 ), "Charmander;Bulbasaur;#18#Squirtel;Pikachu;Eevee;Pidgey", int_list_view_scroll_index, int_list_view_active)
        int_list_struct_view_ex_active = gui_list_view_ex(Rectangle( 165, 180, 140, 200 ), ppchar_list_view_ex_list, 8, int_list_view_ex_focus, int_list_view_ex_scroll_index, int_list_view_ex_active)
    
        int_toggle_group_active = gui_toggle_group(Rectangle( 165, 400, 140, 25 ), "#1#ONE\n#3#TWO\n#8#THREE\n#23#", int_toggle_group_active)
    
        # Third GUI column
        if (gui_text_box_multi(Rectangle( 320, 25, 225, 140 ), pchar_multi_str_text_box_text, 256, bool_multi_bool_text_box_edit_mode)):
            bool_multi_bool_text_box_edit_mode = not bool_multi_bool_text_box_edit_mode
        struct_color_picker_value = gui_color_picker(Rectangle( 320, 185, 196, 192 ), None, struct_color_picker_value)

        # See C function "int vsprintf(char *str, const char *format, va_list arg)" for TextFormat("string format", arg)
        # Use type_cast() to send the required types. use round(float, .places) to reduce float length.
        #int_slider_value = gui_slider(Rectangle( 355, 400, 165, 20 ), "TEST", text_format("%2.2f", float(int_slider_value)), int_slider_value, -50, 100)
        #int_slider_value = gui_slider(Rectangle( 355, 400, 165, 20 ), "TEST", str(int(int_slider_value)), int(int_slider_value), -50, 100)
        int_slider_value = gui_slider(Rectangle( 355, 400, 165, 20 ), "TEST", str(round(float(int_slider_value), 2)), round(float(int_slider_value), 2), -50, 100)
        #int_slider_bar_value = gui_slider_bar(Rectangle( 320, 430, 200, 20 ), None, text_format("%i", int(int_slider_bar_value)), int_slider_bar_value, 0, 100)
        int_slider_bar_value = gui_slider_bar(Rectangle( 320, 430, 200, 20 ), None, str(int(int_slider_bar_value)), int(int_slider_bar_value), 0, 100)
        float_progress_value = gui_progress_bar(Rectangle( 320, 460, 200, 20 ), None, None, float_progress_value, 0, 1)
    
        # NOTE: View rectangle could be used to perform some scissor test
        struct_view = gui_scroll_panel(Rectangle( 560, 25, 100, 160 ), None, Rectangle( 560, 25, 200, 400 ), struct_view_scroll)
    
        gui_panel(Rectangle( 560, 25 + 180, 100, 160 ), "Panel Info")
    
        gui_grid(Rectangle( 560, 25 + 180 + 180, 100, 120 ), None, 20, 2)
    
        gui_status_bar(Rectangle( 0, float(get_screen_height()) - 20, float(get_screen_width()), 20 ), "This is a status bar")
    
        float_alpha_value = gui_color_bar_alpha(Rectangle( 320, 490, 200, 30 ), None, float_alpha_value)
    
        if (bool_show_message_box):
            draw_rectangle(0, 0, get_screen_width(), get_screen_height(), fade(RAYWHITE, 0.8))
            int_result = gui_message_box(Rectangle( float(get_screen_width())/2 - 125, float(get_screen_height())/2 - 50, 250, 100 ), gui_icon_text(ICON_EXIT, "Close Window"), "Do you really want to exit?", "Yes;No")
    
            if ((int_result == 0) or (int_result == 2)):
                bool_show_message_box = false
            elif (result == 1):
                bool_exit_window = True
    
        if (bool_show_text_input_box):
            draw_rectangle(0, 0, get_screen_width(), get_screen_height(), fade(RAYWHITE, 0.8));
            int_result = gui_text_input_box(Rectangle( float(get_screen_width())/2 - 120, float(get_screen_height())/2 - 60, 240, 140 ), "Save", gui_icon_text(ICON_FILE_SAVE, "Save file as..."), "Ok;Cancel", pchar_text_input, 255, None)
    
            if (int_result == 1):
                # TODO: Validate str_text_input value and save
    
                pchar_text_input_file_name = pchar_text_input  # strcpy(,)
    
            if ((int_result == 0) or (int_result == 1) or (int_result == 2)):
                bool_show_text_input_box = False
                pchar_text_input = b""  # strcpy(str_text_input, "\0")
    
        #----------------------------------------------------------------------------------
    
        end_drawing()
        #----------------------------------------------------------------------------------
    
    # De-Initialization
    #--------------------------------------------------------------------------------------
    close_window()        # Close window and OpenGL context
    #--------------------------------------------------------------------------------------
    return None

## Convert list to 2D C char *Array[] (See: Line 108 initialization list1 = [])
def list_to_2d_c_array(py_list):
    c_array_2d = [ffi.new("char[]", x.encode('utf-8')) for x in py_list]
    return c_array_2d

if __name__ == '__main__':
    main()
