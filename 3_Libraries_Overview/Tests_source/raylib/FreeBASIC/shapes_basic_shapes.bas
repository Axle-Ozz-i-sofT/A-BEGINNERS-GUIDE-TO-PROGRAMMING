/'*****************************************************************************************
*
*   raylib [shapes] example - Draw basic shapes 2d (rectangle, circle, line...)
*
*   Example originally created with raylib 1.0, last time updated with raylib 4.2
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2014-2022 Ramon Santamaria (@raysan5)
*
*****************************************************************************************'/
'' Converted by: Axle
'' Date: 26/10/2022
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
    
    InitWindow(screenWidth, screenHeight, "raylib [shapes] example - basic shapes drawing")
    
    Dim As Single rotation = 0.0
    
    SetTargetFPS(60)               '' Set our game to run at 60 frames-per-second
    ''--------------------------------------------------------------------------------------
    
    '' Main game loop
    While (Not WindowShouldClose())    '' Detect window close button or ESC key
        '' Update
        ''----------------------------------------------------------------------------------
        rotation += 0.2  '' Polygon shapes and lines rotation
        ''----------------------------------------------------------------------------------
        
        '' Draw
        ''----------------------------------------------------------------------------------
        BeginDrawing()
        
        ClearBackground(RAYWHITE)
        
        DrawText("some basic shapes available on raylib", 20, 20, 20, DARKGRAY)
        
        '' Circle shapes and lines
        DrawCircle(screenWidth/5, 120, 35, DARKBLUE)
        DrawCircleGradient(screenWidth/5, 220, 60, GREEN, SKYBLUE)
        DrawCircleLines(screenWidth/5, 340, 80, DARKBLUE)
        
        '' Rectangle shapes and ines
        DrawRectangle(Int(screenWidth/4*2 - 60), 100, 120, 60, RED)
        DrawRectangleGradientH(Int(screenWidth/4*2 - 90), 170, 180, 130, MAROON, GOLD)
        DrawRectangleLines(Int(screenWidth/4*2 - 40), 320, 80, 60, ORANGE)  '' NOTE: Uses QUADS internally, not lines
        
        '' Triangle shapes and lines
        DrawTriangle(Vector2(screenWidth/4.0 *3.0, 80.0), _
        Vector2(screenWidth/4.0 *3.0 - 60.0, 150.0), _
        Vector2(screenWidth/4.0 *3.0 + 60.0, 150.0), VIOLET)
        
        DrawTriangleLines(Vector2(screenWidth/4.0*3.0, 160.0), _
        Vector2(screenWidth/4.0*3.0 - 20.0, 230.0), _
        Vector2(screenWidth/4.0*3.0 + 20.0, 230.0), DARKBLUE)

        '' Polygon shapes and lines
        DrawPoly(Vector2(screenWidth/4.0*3, 330), 6, 80, rotation, BROWN)
        DrawPolyLines(Vector2(screenWidth/4.0*3, 330 ), 6, 90, rotation, BROWN)
        DrawPolyLinesEx(Vector2(screenWidth/4.0*3, 330), 6, 85, rotation, 6, BEIGE)

        '' Polygon shapes and lines (Alternative :)
        'DrawPoly(Vector2(screenWidth/4.0*3, 320), 6, 80, rotation, BROWN)
        'DrawPolyLines(Vector2(screenWidth/4.0*3, 330 ), 6, 90, rotation, BROWN)
        'DrawPolyLinesEx(Vector2(screenWidth/4.0*3, 320), 6, 80, rotation, 6, BEIGE)
        
        '' NOTE: We draw all LINES based shapes together to optimize internal drawing,
        '' this way, all LINES are rendered in a single draw pass
        DrawLine(18, 42, screenWidth - 18, 42, BLACK)
        EndDrawing()
        ''----------------------------------------------------------------------------------
    Wend
    
    '' De-Initialization
    ''--------------------------------------------------------------------------------------
    CloseWindow()        '' Close window and OpenGL context
    ''--------------------------------------------------------------------------------------
    
    Return 0
End Function  ' END main_procedure <---
