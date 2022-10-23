'-------------------------------------------------------------------------------
' Name:        Types.bas
' Purpose:     Examples
'
' Platform:    Win64, Ubuntu64
'
' Author:      Axle
' Created:     07/03/2022
' Updated:
' Copyright:   (c) Axle 2022
' Licence:     MIT No Attribution
'-------------------------------------------------------------------------------

Declare Function main_procedure() As Integer

main_procedure()

Function main_procedure() As Integer  ' Main procedure
    
	Dim As Integer a = 16
	' SizeOf() operator will return the byte width of the type.
	Print "Size of variable a : "; Sizeof(a)
	Print "Size of Integer data type : "; Sizeof(Integer)
	Print "Size of String data type : "; Sizeof(String)
	Print "Size of Single(float) data type : "; Sizeof(Single)
	Print "Size of Double data type : "; Sizeof(Double)
	Print "Size of Short data type : "; Sizeof(Short)
	Print "Size of Long data type : "; Sizeof(Long)
	Print "Size of longInt data type : "; Sizeof(Longint)
    
    Getkey
    Return 0
End Function  ' END main_procedure <---
