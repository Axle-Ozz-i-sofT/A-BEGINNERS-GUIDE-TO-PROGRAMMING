#******************************************************************************************
#
#   raylib [models] example - Draw some basic geometric shapes (cube, sphere, cylinder...)
#
#   Example originally created with raylib 1.0, last time updated with raylib 3.5
#
#   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
#   BSD-like license that allows static linking with closed source software
#
#   Copyright (c) 2014-2022 Ramon Santamaria (@raysan5)
#
#******************************************************************************************
# Converted by Axle
#  (Original C source Copyright (c) 2016-2022 Ramon Santamaria (@raysan5))
# Date: 27/10/2022
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
# pchar_variable char variable[n]
# ppchar_variable char *variable[n]
# pcint_variable int *variable
#
# see:https://electronstudio.github.io/raylib-python-cffi/pyray.html
#
# NOTES:
# See ffi.new()
# 

from pyray import *

#------------------------------------------------------------------------------------
# Program main entry point
#------------------------------------------------------------------------------------
def main():

    # Initialization
    #--------------------------------------------------------------------------------------
    int_screen_width = 800
    int_screen_height = 450

    init_window(int_screen_width, int_screen_height, "raylib [models] example - geometric shapes")

    # Define the camera to look into our 3d world
    struct_camera_3d = Camera3D()                           # Create Camera3D object
    struct_camera_3d.position = Vector3( 0.0, 10.0, 10.0 )  # Camera position
    struct_camera_3d.target = Vector3( 0.0, 0.0, 0.0 )      # Camera looking at point
    struct_camera_3d.up = Vector3( 0.0, 1.0, 0.0 )          # Camera up vector (rotation towards target)
    struct_camera_3d.fovy = 45.0                            # Camera field-of-view Y
    struct_camera_3d.projection = CAMERA_PERSPECTIVE        # Camera mode type

    set_target_fps(60)    # Set our game to run at 60 frames-per-second
    #--------------------------------------------------------------------------------------

    # Main game loop
    while (not window_should_close()):    # Detect window close button or ESC key
        # Update
        #----------------------------------------------------------------------------------
        # TODO: Update your variables here
        #----------------------------------------------------------------------------------

        # Draw
        #----------------------------------------------------------------------------------
        begin_drawing()

        clear_background(RAYWHITE)

        begin_mode_3d(struct_camera_3d)

        draw_cube      (Vector3(-4.0, 0.0, 2.0), 2.0, 5.0, 2.0, RED)
        draw_cube_wires(Vector3(-4.0, 0.0, 2.0), 2.0, 5.0, 2.0, GOLD)
        draw_cube_wires(Vector3(-4.0, 0.0, -2.0), 3.0, 6.0, 2.0, MAROON)

        draw_sphere      (Vector3(-1.0, 0.0, -2.0), 1.0, GREEN)
        draw_sphere_wires(Vector3(1.0, 0.0, 2.0), 2.0, 16, 16, LIME)

        draw_cylinder      (Vector3(4.0, 0.0, -2.0), 1.0, 2.0, 3.0, 4, SKYBLUE)
        draw_cylinder_wires(Vector3(4.0, 0.0, -2.0), 1.0, 2.0, 3.0, 4, DARKBLUE)
        draw_cylinder_wires(Vector3(4.5, -1.0, 2.0), 1.0, 1.0, 2.0, 6, BROWN)

        draw_cylinder      (Vector3(1.0, 0.0, -4.0), 0.0, 1.5, 3.0, 8, GOLD)
        draw_cylinder_wires(Vector3(1.0, 0.0, -4.0), 0.0, 1.5, 3.0, 8, PINK)

        # Available in next release
        #draw_capsule(Vector3(-3.0, 1.5, -4.0), Vector3(-4.0, -1.0, -4.0), 1.2, 8, 8, VIOLET)
        #draw_capsule_wires(Vector3(-3.0, 1.5, -4.0), Vector3(-4.0, -1.0, -4.0), 1.2, 8, 8, PURPLE)

        draw_grid(10, 1.0)  # Draw a grid

        end_mode_3d()

        draw_fps(10, 10)

        end_drawing()
        #----------------------------------------------------------------------------------


    # De-Initialization
    #--------------------------------------------------------------------------------------
    close_window();        # Close window and OpenGL context
    #--------------------------------------------------------------------------------------

    return None

if __name__ == '__main__':
    main()