'-------------------------------------------------------------------------------
' Name:        Loops.bas
' Purpose:     Example
'
' Platform:    Win64, Ubuntu64
'
' Author:      Axle
' Created:     15/02/2022
' Updated:     19/02/2022
' Copyright:   (c) Axle 2022
' Licence:     MIT No Attribution
'-------------------------------------------------------------------------------

' Define extra functions so we can place them at the bottom of the page.
Declare Function main_procedure() As Integer
Declare Function Con_Pause() As Integer

main_procedure()

Function main_procedure() As Integer ' Main procedure
    
    ' A simple example of nested loops using for and while.
    Const As Integer num = 5 '  length of loops.
    
    ' Set variables for enumerating (Counting through) the loops.
    Dim As Integer a = 0
    Dim As Integer b = 0
    Dim As Integer c = 0
    ' Loop level 1.
    For a = 0 To num Step 1
        Print "a" & a & ":"
        ' Loop level 2 (nested).
        For b = 0 To a Step 1
            Print "b" & b & ":";
            ' Loop level 3 (nested).
            For c = 0 To b Step 1
                Print  c & ",";
            Next c
            Print ""
        Next b
        Print ""
    Next a
    
    'S_Pause()
    Con_Pause()
    
    ' Reset our counters to zero.
    a = 0
    b = 0
    c = 0
    ' Loop level 1.
    While(a <= num)
        Print "a" & a & ":"
        ' Loop level 2 (nested).
        While(b <= a)
            Print "b" & b & ":";
            ' Loop level 3 (nested).
            While(c <= b)
                Print c & ",";
                c+=1
            Wend
            Print ""
            c = 0
            b+=1
        Wend
        Print ""
        b = 0
        a+=1
    Wend
    
    Con_Pause()
    
    /' How loops are created in assembly.
    // Intel Asm syntax
    // jmp label: Is the equivelent of Goto LABEL:
    
    mov eax, 0      ; set a counter To enumerate the Loop
    mov ebx, num    ; store Var num in ecx register
    top:                ; Label
    cmp eax, ebx    ; Test If eax == num
    je bottom       ; Loop Exit when While condition True
    BODY            ; ... Code inside the Loop
    inc eax         ; Increment the counter +1
    jmp top         ; Jump To label top:
    bottom:             ; Label
    '/
    
    a = 0 '  reset counter a to 0.
    ' The while loop will print to 4 as the truth test is before the printf().
    ' This is the correct way to implement a loop by using while or for.
    While(1)
        If a = num Then
            Exit While
        End If
        Print a
        a+=1
    Wend
    
    /'
    ' Assembly Output For the above While loop.
    ' Intel Asm syntax
    .LC0:
    .string "%d,\n"
    main:
    push    rbp
    mov     rbp, rsp
    Sub     rsp, 16
    mov     DWORD Ptr [rbp-8], 5
    mov     DWORD Ptr [rbp-4], 0
    .L4:
    mov     eax, DWORD Ptr [rbp-4]
    cmp     eax, DWORD Ptr [rbp-8]
    je      .L7
    mov     eax, DWORD Ptr [rbp-4]
    mov     esi, eax
    mov     edi, OFFSET FLAT:.LC0
    mov     eax, 0
    Call    printf
    Add     DWORD Ptr [rbp-4], 1
    jmp     .L4
    .L7:
    nop
    mov     eax, 0
    leave
    ret
    '/
    
    Con_Pause()
    
    ' Recreate a while loop (correctly) using goto (jmp)
    ' This is the same as the while loop above.
    ' Start do while loop
    Dim As Integer d = 0
    Goto L2  ' enter while loop.
    L2:
    If d = num Then  ' Truth test
        Goto L7  ' break out of loop
    End If
    Print d
    d+=1  ' Increment d.
    Goto L2
    L7:  ' exit while loop
    
    /'
    ' Exact assembly output For the  Goto (While) loop.
    ' Intel Asm syntax
    ' eax, esi, etc are CPU registers.
    ' Essentially the internal commands (keywords) or instruction set of the CPU.
    .LC0:
    .string "%d\n"
    main:                               ; main()
    push    rbp                     ; main()set integers
    mov     rbp, rsp                ; main() sets integers
    Sub     rsp, 16                 ; main() sets integers To 16bit
    mov     DWORD Ptr [rbp-8], 5    ; Declare variable num = 5
    mov     DWORD Ptr [rbp-4], 0    ; Declare variable d = 0
    nop
    .L2:                                ; Start While Loop
    mov     eax, DWORD Ptr [rbp-4]
    cmp     eax, DWORD Ptr [rbp-8]  ; Truth test
    je      .L7                     ; If True Exit Loop
    mov     eax, DWORD Ptr [rbp-4]
    mov     esi, eax
    mov     edi, OFFSET FLAT:.LC0
    mov     eax, 0
    Call    printf                  ; Print d
    Add     DWORD Ptr [rbp-4], 1    ; Increment d (+1)
    jmp     .L2                     ; Loop
    .L7:                                ; Exit While Loop
    nop
    mov     eax, 0
    leave                           ; Exit main()
    ret                             ; main() Return statement
    '/
    
    
    Con_Pause()
    Return 0
End Function  ' END main_procedure <---

Function Con_Pause() As Integer
    Dim As Long dummy
    Print("Press any key to continue...")
    dummy = Getkey
    Return 0
End Function
