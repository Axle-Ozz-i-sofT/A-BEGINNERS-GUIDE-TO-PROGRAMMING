#******************************************************************************************
#
#   raylib [shapes] example - Draw basic shapes 2d (rectangle, circle, line...)
#
#   Example originally created with raylib 1.0, last time updated with raylib 4.2
#
#   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
#   BSD-like license that allows static linking with closed source software
#
#   Copyright (c) 2014-2022 Ramon Santamaria (@raysan5)
#
#*****************************************************************************************/
# Converted by Axle with assist from electronstudio
# (Original C source Copyright (c) 2016-2022 Ramon Santamaria (@raysan5))
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
## raylib C to raylib_cffi conversions
# DrawRectangleGradientH() becomes draw_rectangle_gradient_h()
#
# see:https://electronstudio.github.io/raylib-python-cffi/pyray.html
#
# NOTES:
# types int, float etc. may meed be explicitly cast in some functions.

from pyray import *
##import pyray as pr

#------------------------------------------------------------------------------------
# Program main entry point
#------------------------------------------------------------------------------------
def main():

    # Initialization
    #--------------------------------------------------------------------------------------
    int_screen_width = 800
    int_screen_height = 450
    
    init_window(int_screen_width, int_screen_height, "raylib [shapes] example - basic shapes drawing")
    
    rotation = 0.0
    
    set_target_fps(60)  # Set our game to run at 60 frames-per-second
    #--------------------------------------------------------------------------------------
    
    # Main game loop
    while (not window_should_close()):  # Detect window close button or ESC key
    
        # Update
        #----------------------------------------------------------------------------------
        rotation += 0.2
        #----------------------------------------------------------------------------------
    
        # Draw
        #----------------------------------------------------------------------------------
        begin_drawing()
    
        clear_background(RAYWHITE)
    
        draw_text("some basic shapes available on raylib", 20, 20, 20, DARKGRAY)
    
        # Circle shapes and lines
        ## NOTE: Check the function definitions as some require int(), some require float()
        ## Althought the following may be auto cast to int when passing to the function.
        ##DrawCircle(int centerX, int centerY, float radius, Color color)
        draw_circle(int(int_screen_width/5), 120, 35, DARKBLUE)
        draw_circle_gradient(int(int_screen_width/5), 220, 60, GREEN, SKYBLUE)
        draw_circle_lines(int(int_screen_width/5), 340, 80, DARKBLUE)
    
        # Rectangle shapes and ines
        
        draw_rectangle(int(int_screen_width/4*2 - 60), 100, 120, 60, RED)
        draw_rectangle_gradient_h(int(int_screen_width/4*2 - 90), 170, 180, 130, MAROON, GOLD)
        draw_rectangle_lines(int(int_screen_width/4*2 - 40), 320, 80, 60, ORANGE)  # NOTE: Uses QUADS internally, not lines
    
        # Triangle shapes and lines
        # Vector2 takes float so dosen't need a cast?
        # Added cast for safety!
        draw_triangle(Vector2(float(int_screen_width/4.0 *3.0), 80.0),
                    Vector2(float(int_screen_width/4.0 *3.0 - 60.0), 150.0),
                    Vector2(float(int_screen_width/4.0 *3.0 + 60.0), 150.0), VIOLET)
    
        draw_triangle_lines(Vector2(float(int_screen_width/4.0*3.0), 160.0),
                        Vector2(float(int_screen_width/4.0*3.0 - 20.0), 230.0),
                        Vector2(float(int_screen_width/4.0*3.0 + 20.0), 230.0), DARKBLUE)
    
        # Polygon shapes and lines
        draw_poly(Vector2(float(int_screen_width/4.0*3), 330), 6, 80, rotation, BROWN)
        draw_poly_lines(Vector2(float(int_screen_width/4.0*3), 330 ), 6, 90, rotation, BROWN);
        draw_poly_lines_ex(Vector2(float(int_screen_width/4.0*3), 330), 6, 85, rotation, 6, BEIGE)

        # Polygon shapes and lines (Alternative :)
        #draw_poly(Vector2(float(int_screen_width/4.0*3), 320), 6, 80, rotation, BROWN)
        #draw_poly_lines(Vector2(float(int_screen_width/4.0*3), 330 ), 6, 90, rotation, BROWN);
        #draw_poly_lines_ex(Vector2(float(int_screen_width/4.0*3), 320), 6, 80, rotation, 6, BEIGE)

        # NOTE: We draw all LINES based shapes together to optimize internal drawing,
        # this way, all LINES are rendered in a single draw pass
        draw_line(18, 42, int_screen_width - 18, 42, BLACK)
        end_drawing()
        #----------------------------------------------------------------------------------
    
    # De-Initialization
    #--------------------------------------------------------------------------------------
    close_window()        # Close window and OpenGL context
    #--------------------------------------------------------------------------------------
    return None

if __name__ == '__main__':
    main()