/'*****************************************************************************************
*
*   raylib [models] example - Draw some basic geometric shapes (cube, sphere, cylinder...)
*
*   Example originally created with raylib 1.0, last time updated with raylib 3.5
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2014-2022 Ramon Santamaria (@raysan5)
*
*****************************************************************************************'/
'' Converted by: Axle
'' Date: 27/10/2022
'' raylib V4.2.0
'' raygui V3.2
'' WIITD/raylib-freebasic binders 22/10/2022

'' If not included in the raylib.bi or raygui.bi
'' Depends upon the shared library used...
'' raygui stand alone shared lib
'' -DRAYGUI_IMPLEMENTATION -DBUILD_LIBTYPE_SHARED (Win)
'' -DRAYGUI_IMPLEMENTATION -lraylib (nix)
'' #inclib "raygui"  ' Raygui stand alone shared Unix.
'' #inclib "rayguidll"  ' Raygui stand alone shared Win.
''
'' raylib+raygui shared
'' RAYLIB_MODULE_RAYGUI=TRUE RAYLIB_LIBTYPE=SHARED
'' #inclib "raylib"  ' raylib/raylib+raygui combined shared Linux.
'' #inclib "raylibdll"  ' raylib/raylib+raygui combined shared Windows.

#include once "raylib.bi"

''------------------------------------------------------------------------------------
'' Program main entry point
''------------------------------------------------------------------------------------
Declare Function main_procedure() As Integer
main_procedure()

Function main_procedure() As Integer  ' Main procedure
    '' Initialization
    ''--------------------------------------------------------------------------------------
    Const As Long screenWidth = 800, screenHeight = 450

    InitWindow(screenWidth, screenHeight, "raylib [models] example - geometric shapes")

    '' Define the camera to look into our 3d world
    dim as Camera3D camera_3D
    camera_3D.position = vector3(0.0, 10.0, 10.0)    ' Camera position
    camera_3D.target = vector3(0.0, 0.0, 0.0)        ' Camera looking at point
    camera_3D.up = vector3(0.0, 1.0, 0.0)            ' Camera up vector (rotation towards target)
    camera_3D.fovy = 45.0                            ' Camera field-of-view Y
    camera_3D.projection = CAMERA_PERSPECTIVE        ' Camera mode type

    SetTargetFPS(60)               '' Set our game to run at 60 frames-per-second
    ''--------------------------------------------------------------------------------------

    '' Main game loop
    while (not WindowShouldClose())    '' Detect window close button or ESC key
        '' Update
        ''----------------------------------------------------------------------------------
        '' TODO: Update your variables here
        ''----------------------------------------------------------------------------------

        '' Draw
        ''----------------------------------------------------------------------------------
        BeginDrawing()

            ClearBackground(RAYWHITE)

            BeginMode3D(camera_3D)

                DrawCube     (Vector3(-4.0, 0.0, 2.0), 2.0, 5.0, 2.0, RED)
                DrawCubeWires(Vector3(-4.0, 0.0, 2.0), 2.0, 5.0, 2.0, GOLD)
                DrawCubeWires(Vector3(-4.0, 0.0, -2.0), 3.0, 6.0, 2.0, MAROON)

                DrawSphere     (Vector3(-1.0, 0.0, -2.0), 1.0, GREEN)
                DrawSphereWires(Vector3(1.0, 0.0, 2.0), 2.0, 16, 16, LIME)

                DrawCylinder     (Vector3(4.0, 0.0, -2.0), 1.0, 2.0, 3.0, 4, SKYBLUE)
                DrawCylinderWires(Vector3(4.0, 0.0, -2.0), 1.0, 2.0, 3.0, 4, DARKBLUE)
                DrawCylinderWires(Vector3(4.5, -1.0, 2.0), 1.0, 1.0, 2.0, 6, BROWN)

                DrawCylinder     (Vector3(1.0, 0.0, -4.0), 0.0, 1.5, 3.0, 8, GOLD)
                DrawCylinderWires(Vector3(1.0, 0.0, -4.0), 0.0, 1.5, 3.0, 8, PINK)

                'Available in next release.
                'DrawCapsule     (Vector3(-3.0, 1.5, -4.0), Vector3(-4.0, -1.0, -4.0), 1.2, 8, 8, VIOLET)
                'DrawCapsuleWires(Vector3(-3.0, 1.5, -4.0), Vector3(-4.0, -1.0, -4.0), 1.2, 8, 8, PURPLE)

                DrawGrid(10, 1.0)  '' Draw a grid

            EndMode3D()

            DrawFPS(10, 10)

        EndDrawing()
        ''----------------------------------------------------------------------------------
    wend

    '' De-Initialization
    ''--------------------------------------------------------------------------------------
    CloseWindow()        '' Close window and OpenGL context
    ''--------------------------------------------------------------------------------------

    Return 0
End Function  ' END main_procedure <---