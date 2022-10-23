/'******************************************************************************************
*
*   raygui - controls test suite
*
*   TEST CONTROLS:
*       - GuiDropdownBox()
*       - GuiCheckBox()
*       - GuiSpinner()
*       - GuiValueBox()
*       - GuiTextBox()
*       - GuiButton()
*       - GuiComboBox()
*       - GuiListView()
*       - GuiToggleGroup()
*       - GuiTextBoxMulti()
*       - GuiColorPicker()
*       - GuiSlider()
*       - GuiSliderBar()
*       - GuiProgressBar()
*       - GuiColorBarAlpha()
*       - GuiScrollPanel()
*
*
*   DEPENDENCIES:
*       raylib 4.0 - Windowing/Input management And drawing.
*       raygui 3.2 - Immediate-mode GUI controls.
*
*   COMPILATION (Windows - MinGW):
*       gcc -o $(NAME_PART).exe $(FILE_NAME) -I../../src -lraylib -lopengl32 -lgdi32 -std=c99
*
*   LICENSE: zlib/libpng
*
*   Copyright (c) 2016-2022 Ramon Santamaria (@raysan5)
*
*********************************************************************************************'/
'' Converted by: Axle (Original C source Copyright (c) 2016-2022 Ramon Santamaria (@raysan5))
'' Date: 22/10/2022
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
''RAYLIB_MODULE_RAYGUI=TRUE RAYLIB_LIBTYPE=SHARED
'' #inclib "raylib"  ' raylib/raylib+raygui combined shared Linux.
'' #inclib "raylibdll"  ' raylib/raylib+raygui combined shared Windows.

#include "raylib.bi"

#define RAYGUI_IMPLEMENTATION
''#define RAYGUI_CUSTOM_ICONS       '' It requires providing gui_icons.h in the same directory
''#include "gui_icons.h"            '' External icons data provided, it can be generated with rGuiIcons tool
#include "raygui.bi"

#undef RAYGUI_IMPLEMENTATION        '' Avoid including raygui implementation again

'' No longer required as this can be done with FB String API
'#include <string.h>                 '' Required for: strcpy()

'Type bool As Boolean

'' NULL pointer for C shared library compatability.
#ifndef NULL
#define NULL Cptr(Any Ptr, 0)
#endif

''------------------------------------------------------------------------------------
'' Program main entry point
''------------------------------------------------------------------------------------
'' Main procedure is not required by FB. I use it for convenience as well as
'' to maintain visual conformity with C and Python 3.
Declare Function main_procedure() As Integer
main_procedure()

Function main_procedure() As Integer  ' Main procedure
    '' Initialization
    ''---------------------------------------------------------------------------------------
    Dim As Long screenWidth = 690, screenHeight = 560
    
    
    InitWindow(screenWidth, screenHeight, "raygui - controls test suite")
    SetExitKey(0)
    
    '' GUI controls initialization
    ''----------------------------------------------------------------------------------
    Dim As Long dropdownBox000Active = 0
    Dim As Boolean dropDown000EditMode = False
    
    Dim As Long dropdownBox001Active = 0
    Dim As Boolean dropDown001EditMode = False
    
    Dim As Long spinner001Value = 0
    Dim As Boolean spinnerEditMode = False
    
    Dim As Long valueBox002Value = 0
    Dim As Boolean valueBoxEditMode = False
    
    'dim as string textBoxText = "Text box"
    Dim textBoxText As zstring  * 256 => "Text box"
    Dim As Boolean textBoxEditMode = False
    
    Dim As Long listViewScrollIndex = 0
    Dim As Long listViewActive = -1
    
    Dim As Long listViewExScrollIndex = 0
    Dim As Long listViewExActive = 2
    Dim As Long listViewExFocus = -1
    Dim As zstring Ptr listViewExList( 8 ) => { @"This", @"is", @"a", @"list view", @"with", @"disable", @"elements", @"amazing!" }
    
    Dim As String multiTextBoxText = "Multi text box"
    Dim As Boolean multiTextBoxEditMode = False
    Dim As RLColor colorPickerValue = RED  '' RLColor = raylib.h Color
    
    Dim As Long sliderValue = 50
    Dim As Long sliderBarValue = 60
    Dim As Single progressValue = 0.4
    
    Dim As Boolean forceSquaredChecked = False
    
    Dim As Single alphaValue = 0.5
    
    Dim As Long comboBoxActive = 1
    
    Dim As Long toggleGroupActive = 0
    
    Dim As Vector2 viewScroll
    ''----------------------------------------------------------------------------------
    
    '' Custom GUI font loading
    ''Font font = LoadFontEx("fonts/rainyhearts16.ttf", 12, 0, 0)
    ''GuiSetFont(font)
    
    Dim As Boolean exitWindow = False
    Dim As Boolean showMessageBox = False
    
    Dim textInput As zstring  * 256
    Dim As Boolean showTextInputBox = False
    
    Dim textInputFileName As zstring  * 256
    
    SetTargetFPS(60)
    ''--------------------------------------------------------------------------------------
    
    '' Main game loop
    While (Not exitWindow)    '' Detect window close button or ESC key
        '' Update
        ''----------------------------------------------------------------------------------
        exitWindow = WindowShouldClose()
        
        If (IsKeyPressed(KEY_ESCAPE)) Then
            showMessageBox = Not showMessageBox
            'showMessageBox xor= true
        End If
        
        If (IsKeyDown(KEY_LEFT_CONTROL) And IsKeyPressed(KEY_S)) Then
            showTextInputBox = True
        End If
        
        If (IsFileDropped()) Then
            
            Dim As FilePathList droppedFiles = LoadDroppedFiles()
            
            '' ###### check the following ######
            'If ((droppedFiles.count > 0) And IsFileExtension(droppedFiles.paths[0], ".rgs")) GuiLoadStyle(droppedFiles.paths[0])
            If (droppedFiles.count > 0) Andalso IsFileExtension(droppedFiles.paths[0], ".rgs") Then
				GuiLoadStyle(droppedFiles.paths[0])
            End If
            
            UnloadDroppedFiles(droppedFiles)    '' Clear internal buffers
        End If
        
        ''----------------------------------------------------------------------------------
        
        '' Draw
        ''----------------------------------------------------------------------------------
        BeginDrawing()
        
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)))
        
        '' raygui: controls drawing
        ''----------------------------------------------------------------------------------
        If (dropDown000EditMode Orelse dropDown001EditMode) Then
            GuiLock()
        Elseif (Not dropDown000EditMode) Andalso (Not dropDown001EditMode) Then
            GuiUnlock()
        End If
        ''GuiDisable()
        
        '' First GUI column
        ''GuiSetStyle(CHECKBOX, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT)
        forceSquaredChecked = GuiCheckBox(Rectangle( 25, 108, 15, 15 ), "FORCE CHECK!", forceSquaredChecked)
        
        GuiSetStyle(TEXTBOX, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER)
        ''GuiSetStyle(VALUEBOX, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT)
        If (GuiSpinner(Rectangle( 25, 135, 125, 30 ), NULL, @spinner001Value, 0, 100, spinnerEditMode)) Then
            spinnerEditMode = Not spinnerEditMode
        End If
        If (GuiValueBox(Rectangle( 25, 175, 125, 30 ), NULL, @valueBox002Value, 0, 100, valueBoxEditMode)) Then
            valueBoxEditMode = Not valueBoxEditMode
        End If
        GuiSetStyle(TEXTBOX, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT)
        If (GuiTextBox(Rectangle( 25, 215, 125, 30 ), textBoxText, 64, textBoxEditMode)) Then
            textBoxEditMode = Not textBoxEditMode
        End If
        
        GuiSetStyle(BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER)
        
        If (GuiButton(Rectangle( 25, 255, 125, 30 ), GuiIconText(ICON_FILE_SAVE, "Save File"))) Then
            showTextInputBox = True
        End If
        
        '' I am not actually sure what this is for. Axle
        GuiGroupBox(Rectangle( 25, 310, 125, 150 ), "STATES")
        ''GuiLock()
        GuiSetState(STATE_NORMAL)
        If (GuiButton(Rectangle( 30, 320, 115, 30 ), "NORMAL")) Then
        End If
        GuiSetState(STATE_FOCUSED)
        If (GuiButton(Rectangle( 30, 355, 115, 30 ), "FOCUSED")) Then
        End If
        GuiSetState(STATE_PRESSED)
        If (GuiButton(Rectangle( 30, 390, 115, 30 ), "#15#PRESSED")) Then
        End If
        GuiSetState(STATE_DISABLED)
        If (GuiButton(Rectangle( 30, 425, 115, 30 ), "DISABLED")) Then
        End If
        GuiSetState(STATE_NORMAL)
        ''GuiUnlock()
        
        comboBoxActive = GuiComboBox(Rectangle( 25, 470, 125, 30 ), !"ONE;TWO;THREE;FOUR", comboBoxActive)
        
        '' NOTE: GuiDropdownBox must draw after any other control that can be covered on unfolding
        GuiSetStyle(DROPDOWNBOX, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT)
        If (GuiDropdownBox(Rectangle( 25, 65, 125, 30 ), !"#01#ONE;#02#TWO;#03#THREE;#04#FOUR", @dropdownBox001Active, dropDown001EditMode)) Then
            dropDown001EditMode = Not dropDown001EditMode
            'dropDown001EditMode xor= true
        End If
        
        GuiSetStyle(DROPDOWNBOX, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER)
        If (GuiDropdownBox(Rectangle( 25, 25, 125, 30 ), "ONE;TWO;THREE", @dropdownBox000Active, dropDown000EditMode)) Then
            dropDown000EditMode = Not dropDown000EditMode
            'dropDown000EditMode xor= true
        End If
        
        '' Second GUI column
        listViewActive = GuiListView(Rectangle( 165, 25, 140, 140 ), "Charmander;Bulbasaur;#18#Squirtel;Pikachu;Eevee;Pidgey", @listViewScrollIndex, listViewActive)
        listViewExActive = GuiListViewEx(Rectangle( 165, 180, 140, 200 ), @listViewExList(0), 8, @listViewExFocus, @listViewExScrollIndex, listViewExActive)
        
        toggleGroupActive = GuiToggleGroup(Rectangle( 165, 400, 140, 25 ), !"#1#ONE\n#3#TWO\n#8#THREE\n#23#", toggleGroupActive)
        
        '' Third GUI column
        If (GuiTextBoxMulti(Rectangle( 320, 25, 225, 140 ), multiTextBoxText, 256, multiTextBoxEditMode)) Then
            multiTextBoxEditMode = Not multiTextBoxEditMode
        End If
        colorPickerValue = GuiColorPicker(Rectangle( 320, 185, 196, 192 ), NULL, colorPickerValue)
        
        sliderValue = GuiSlider(Rectangle( 355, 400, 165, 20 ), "TEST", TextFormat("%2.2f", Csng(sliderValue)), sliderValue, -50, 100)
        sliderBarValue = GuiSliderBar(Rectangle( 320, 430, 200, 20 ), NULL, TextFormat("%i", Clng(sliderBarValue)), sliderBarValue, 0, 100)
        progressValue = GuiProgressBar(Rectangle( 320, 460, 200, 20 ), NULL, NULL, progressValue, 0, 1)
        
        '' NOTE: View rectangle could be used to perform some scissor test
        Dim As Rectangle view_ = GuiScrollPanel(Rectangle( 560, 25, 100, 160 ), NULL, Rectangle( 560, 25, 200, 400 ), @viewScroll)
        
        GuiPanel(Rectangle( 560, 25 + 180, 100, 160 ), "Panel Info")
        
        GuiGrid(Rectangle( 560, 25 + 180 + 180, 100, 120 ), NULL, 20, 2)
        
        GuiStatusBar(Rectangle( 0, Csng(GetScreenHeight() - 20), Csng(GetScreenWidth()), 20 ), "This is a status bar")
        
        alphaValue = GuiColorBarAlpha(Rectangle( 320, 490, 200, 30 ), NULL, alphaValue)
        
        If (showMessageBox) Then
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(RAYWHITE, 0.8))
            Dim As Long result = GuiMessageBox(Rectangle( Csng(GetScreenWidth()/2 - 125), Csng(GetScreenHeight()/2 - 50), 250, 100 ), GuiIconText(ICON_EXIT, "Close Window"), "Do you really want to exit?", "Yes;No")
            
            If ((result = 0) Orelse (result = 2)) Then
                showMessageBox = False
            Elseif (result = 1) Then
                exitWindow = True
            End If
        End If

        If (showTextInputBox) Then
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(RAYWHITE, 0.8))
            Dim As Long result = GuiTextInputBox(Rectangle( Csng(GetScreenWidth()/2 - 120), Csng(GetScreenHeight()/2 - 60), 240, 140 ), "Save", GuiIconText(ICON_FILE_SAVE, "Save file as..."), "Ok;Cancel", textInput, 255, NULL)
            
            If (result = 1) Then
                '' TODO: Validate textInput value and save
                
                'strcpy(textInputFileName, textInput)
                textInputFileName = textInput
            End If
            
            If ((result = 0) Orelse (result = 1) Orelse (result = 2)) Then
                showTextInputBox = False
                'strcpy(textInput, "\0")
                textInput = ""
            End If
        End If
        ''----------------------------------------------------------------------------------

        EndDrawing()
        ''----------------------------------------------------------------------------------
    Wend
    '' De-Initialization
    ''--------------------------------------------------------------------------------------
    CloseWindow()        '' Close window and OpenGL context
    ''--------------------------------------------------------------------------------------
    
    Return 0
End Function  ' END main_procedure <---
