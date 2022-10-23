'-------------------------------------------------------------------------------
' Name:        Decisions.bas
' Purpose:     Example
' Title:       "A Snail's Life"
'
' Platform:    Win64, Ubuntu64
'
' Author:      Axle
' Created:     03/02/2022
' Updated:     19/02/2022
' Copyright:   (c) Axle 2022
' Licence:     MIT No Attribution
'-------------------------------------------------------------------------------


#ifdef __FB_WIN32__
#define OS_Windows 1
#define OS_Unix 0
#endif

#ifdef __FB_UNIX__'__FB_LINUX__
' TODO
#define OS_Unix 1
#define OS_Windows 0
#endif

' Global Constants
#define ROWS 10
#define COLUMNS 40

Declare Function UpdateScreen(table() As String) As Integer
Declare Function Con_Clear() As Integer
Declare Function Con_Pause() As Integer

Declare Function main_procedure() As Integer
main_procedure()

Function main_procedure() As Integer  ' Main procedure
    
    '#ifdef OS_Windows
    ' Windows code
    '#else
    ' GNU/Linux code
    '#endif
    Const As Integer Row_start = ROWS - ROWS + 1  ' Set the boundaries inside of the fence.
    Const As Integer Row_end = ROWS - 2  ' -2 To account for the fence at top and bottom.
    Const As Integer Row_width = Row_end - Row_start  ' ROWS - 2 To account for the fence at each side.
    Const As Integer Col_start = COLUMNS - COLUMNS + 1  ' Set the boundaries inside of the fence.
    Const As Integer Col_end = COLUMNS - 2  ' -2 To account for the fence at each side.
    Const As Integer Col_width = Col_end - Col_start  ' COLUMNS - 2 To account for the fence at each side.
    
    ' Similar to the C example I am using characters as their integer value (See ASCII Char Chart).
    ' I am converting a single string character to its Asc-ii value. aka integer.
    ' Asc("a"), Chr(97)
    ' Define the characters for the console game.
    Const As Integer chr_1 = Asc(" ")  ' Dec 32
    Const As Integer chr_2 = Asc("@")  ' Dec 64
    Const As Integer chr_3 = Asc("v")  ' Dec 118
    Const As Integer chr_4 = Asc("w")  ' Dec 119
    Const As Integer chr_5 = Asc("+")  ' Dec 43
    
    ' Values to randomly generate grass.
    ' 0 - 10 (0=0% fill, 5= 50% fill, 10= 100% fill)
    Const As Integer GFill = 5
    ' 0 - 10 Split rand fill between % 0='w' and 10='W'.
    Const As Integer GSplit = 4
    
    ' Defining a 2D array of array[Rows][Columns], initialised to empty.
    Static As String Table(ROWS, COLUMNS)
    Static As Integer Sprite(2, 2)  ' Sprite[0][0|1] = current [Row|Col]
    Dim As Integer Row_y  ' Counter for rows.
    Dim As Integer Col_x  ' Counter for columns.
    
    ' Use current time as seed for the random generator.
    Randomize  ' without arguments defaults to system time.
    ' Fill the array with some data.
    For Row_y = 0 To ROWS -1 Step 1  ' Count through each row.
        For Col_x = 0 To COLUMNS -1 Step 1  ' Count through each column.
            If ((Col_x >= Col_start) And (Col_x <= Col_end) And (Row_y >= Row_start) And (Row_y <= Row_end)) Then
                ' Build a string with values for each cell.
                ' Int(Rnd * 10) returns 0 to 9
                If (Int(Rnd * 10) > GFill) Then  ' Total grass GFill = 5(50%) fill.
                    If (Int(Rnd * 10) > GSplit) Then  '  Split between v and w.
                        Table(Row_y, Col_x) = Chr(chr_3)  ' Returns as a formatted string.
                    Else
                        Table(Row_y, Col_x) = Chr(chr_4)  ' Insert a character for grass.
                    End If
                Else
                    Table(Row_y, Col_x) = Chr(chr_1)  ' Insert a space character.
                End If
            Else
                Table(Row_y, Col_x) = Chr(chr_5)  ' Insert the fence character.
            End If
        Next Col_x
    Next Row_y
    
    ' Variables to track the Sprite (Snail) position.
    ' Sprite[Sp_now][Sp_row] <- array format.
    Const As Integer Sp_now = 0  ' Current sprite position.
    'Const As Integer Sp_last = 1  ' previous position (Unused).
    Const As Integer Sp_row = 0
    Const As Integer Sp_col = 1
    
    ' Set the snail at a random location and record it in an array.
    Sprite(Sp_now, Sp_row) = Int(Rnd * Row_width) +1  ' +1 for left 0 boundary.
    Sprite(Sp_now, Sp_col) = Int(Rnd * Col_width) +1  ' +1 for top 0 boundary.
    
    ' Update the Table with the sprite.
    Table(Sprite(Sp_now, Sp_row), Sprite(Sp_now, Sp_col)) = Chr(chr_2)
    
    ' Begin main movement and life counter loop.
    ' Note! that Control character 7 '\a' is system dependent and may not make a sound.
    Dim As String Inputs  ' 'N''S''e''w'
    Dim As Integer Lnrg = 10  ' Start at 10 life points. Reduces by 1 each move.
    While(1)
        UpdateScreen(Table())  ' Update the screen.
        Print "+ Life energy= " & Left(Str(Lnrg), 3)  ' Left( , 3) allocates 3 positions.
        If (Lnrg <= 0) Then
            Print !"\a"  ' System bell (ASCII Control Chr 7)
            Print "Oh No! You are out of life energy..."
            Exit While
        End If
        Print "N,S,E,W to move : Q to Quit"
        Input "Type your selection followed by [Enter] >>", Inputs
        Print ""
        Lnrg -= 1  ' Reduce 1 nrg point for each move including hitting the boundary.
        
        If (Left(Inputs, 1) = "N") Or (Left(Inputs, 1) = "n") Then
            If (Sprite(Sp_now, Sp_row) - 1 <> Row_start -1) Then
                ' The following if else block would be best subed to a function
                ' to limit duplication.
                If (Table(Sprite(Sp_now, Sp_row) -1, Sprite(Sp_now, Sp_col)) = Chr(chr_3)) Then
                    Lnrg +=1  ' Increase 1 nrg point for small grass.
                Elseif (Table(Sprite(Sp_now, Sp_row) -1, Sprite(Sp_now, Sp_col)) = Chr(chr_4)) Then
                    Lnrg +=2  ' Increase 2 nrg point for large grass.
                Else
                    ' pass
                End If
                Table(Sprite(Sp_now, Sp_row), Sprite(Sp_now, Sp_col)) = Chr(chr_1)
                Sprite(Sp_now, Sp_row) -=1
                Table(Sprite(Sp_now, Sp_row), Sprite(Sp_now, Sp_col)) = Chr(chr_2)
                ' Move Lnrg -= 1; here to not reduce Life nrg when hitting the boundary.
            Else
                Print !"\a"  ' System bell (ASCII Control Chr 7)
            End If
        Elseif (Left(Inputs, 1) = "S") Or (Left(Inputs, 1) = "s") Then
            If (Sprite(Sp_now, Sp_row) + 1 <> Row_end +1) Then
                If (Table(Sprite(Sp_now, Sp_row) +1, Sprite(Sp_now, Sp_col)) = Chr(chr_3)) Then
                    Lnrg +=1  ' Increase 1 nrg point for small grass.
                Elseif (Table(Sprite(Sp_now, Sp_row) +1, Sprite(Sp_now, Sp_col)) = Chr(chr_4)) Then
                    Lnrg +=2  ' Increase 2 nrg points for large grass.
                Else
                    ' pass
                End If
                Table(Sprite(Sp_now, Sp_row), Sprite(Sp_now, Sp_col)) = Chr(chr_1)
                Sprite(Sp_now, Sp_row) +=1
                Table(Sprite(Sp_now, Sp_row), Sprite(Sp_now, Sp_col)) = Chr(chr_2)
                ' Move Lnrg -= 1; here to not reduce Life nrg when hitting the boundary.
            Else
                Print !"\a"  ' System bell (ASCII Control Chr 7)
            End If
        Elseif (Left(Inputs, 1) = "E") Or (Left(Inputs, 1) = "e") Then
            If (Sprite(Sp_now, Sp_col) + 1 <> Col_end +1) Then
                If (Table(Sprite(Sp_now, Sp_row), Sprite(Sp_now, Sp_col) +1) = Chr(chr_3)) Then
                    Lnrg +=1  ' Increase 1 nrg point for small grass.
                Elseif (Table(Sprite(Sp_now, Sp_row), Sprite(Sp_now, Sp_col) +1) = Chr(chr_4)) Then
                    Lnrg +=2  ' Increase 2 nrg points for large grass.
                Else
                    ' pass
                End If
                Table(Sprite(Sp_now, Sp_row), Sprite(Sp_now, Sp_col)) = Chr(chr_1)
                Sprite(Sp_now, Sp_col) +=1
                Table(Sprite(Sp_now, Sp_row), Sprite(Sp_now, Sp_col)) = Chr(chr_2)
                ' Move Lnrg -= 1; here to not reduce Life nrg when hitting the boundary.
            Else
                Print !"\a"  ' System bell (ASCII Control Chr 7)
            End If
        Elseif (Left(Inputs, 1) = "W") Or (Left(Inputs, 1) = "w") Then
            If (Sprite(Sp_now, Sp_col) - 1 <> Col_start -1) Then
                If (Table(Sprite(Sp_now, Sp_row), Sprite(Sp_now, Sp_col) -1) = Chr(chr_3)) Then
                    Lnrg +=1  ' Increase 1 nrg point for small grass.
                Elseif (Table(Sprite(Sp_now, Sp_row), Sprite(Sp_now, Sp_col) -1) = Chr(chr_4)) Then
                    Lnrg +=2  ' Increase 2 nrg points for large grass.
                Else
                    ' pass
                End If
                Table(Sprite(Sp_now, Sp_row), Sprite(Sp_now, Sp_col)) = Chr(chr_1)
                Sprite(Sp_now, Sp_col) -=1
                Table(Sprite(Sp_now, Sp_row), Sprite(Sp_now, Sp_col)) = Chr(chr_2)
                ' Move Lnrg -= 1; here to not reduce Life nrg when hitting the boundary.
            Else
                Print !"\a"  ' System bell (ASCII Control Chr 7)
            End If
        Elseif (Left(Inputs, 1) = "Q") Or (Left(Inputs, 1) = "q") Then
            Exit While
        Else
            ' Pass
        End If
        
    Wend
    
    Con_Pause()
    Return 0
End Function  ' END main_procedure <---

Function UpdateScreen(table() As String) As Integer
    Con_Clear()
    Dim As Integer Row_y  ' Counter for rows.
    Dim As Integer Col_x  ' Counter for columns.
    
    ' Print the array to the console.
    For Row_y = 0 To ROWS -1 Step 1  ' Count through each row.
        For Col_x = 0 To COLUMNS -1 Step 1  ' Count through each column.
            Print table(Row_y, Col_x);  ' No line breaks.
        Next Col_x
        Print ""  ' Print a line break to start next row.
    Next Row_y
    Return 0
End Function

Function Con_Clear() As Integer
    ' The system() call allows the programmer to run OS command line batch commands.
    ' It is discouraged as there are more appropriate C functions for most tasks.
    ' I am only using it in this instance to avoid invoking additional OS API headers and code.
    If (OS_Windows) Then
        Shell "cls"
    Elseif (OS_Unix) Then
        Shell "clear"
    End If
    Return 0
End Function

Function Con_Pause() As Integer
    Dim As Integer dummy
    Input "Press [Enter] key to continue...", dummy
    
    Return 0
End Function




