//-------------------------------------------------------------------------------
// Name:        Loops.c
// Purpose:     Example
//
// Platform:    Win64, Ubuntu64
//
// Author:      Axle
// Created:     15/02/2022
// Updated:     18/02/2022
// Copyright:   (c) Axle 2022
// Licence:     MIT No Attribution
//-------------------------------------------------------------------------------

#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>

// Define extra functions so we can place them at the bottom of the page.
void S_Pause(void);
int S_getchar(void);

int main()  // Main procedure
    {
    // A simple example of nested loops using for and while.
    const int num = 5;  // length of loops.

    // Set variables for enumerating (Counting through) the loops.
    int a = 0;
    int b = 0;
    int c = 0;
    // Loop level 1.
    for(a = 0; a <= num; a++)
        {
        printf("a%d:\n", a);
        // Loop level 2 (nested).
        for(b = 0; b <= a; b++)
            {
            printf("b%d:", b);
            // Loop level 3 (nested).
            for(c = 0; c <= b; c++)
                {
                printf("%d,", c);
                }
            printf("\n");
            }
        printf("\n");
        }

    S_Pause();

    // Reset our counters to zero.
    a = 0;
    b = 0;
    c = 0;
    // Loop level 1.
    while(a <= num)
        {
        printf("a%d:\n", a);
        // Loop level 2.
        while(b <= a)
            {
            printf("b%d:", b);
            // Loop level 3.
            while(c <= b)
                {
                printf("%d,", c);
                c++;
                }
            printf("\n");
            c = 0;
            b++;
            }
        printf("\n");
        b = 0;
        a++;
        }

    S_Pause();

    /* How loops are created in assembly.
    // Intel asm syntax
    // jmp label: is the equivalent of GOTO LABEL:

            mov eax, 0      ; set a counter to enumerate the loop
            mov ebx, num    ; store var num in ecx register
        top:                ; Label
            cmp eax, ebx    ; Test if eax == num
            je bottom       ; loop exit when while condition True
            BODY            ; ... Code inside the loop
            inc eax         ; Increment the counter +1
            jmp top         ; Jump to label top:
        bottom:             ; Label
    */

    a = 0;  // reset counter a to 0.
    // The while loop will print to 4 as the truth test is before the printf().
    // This is the correct way to implement a loop by using while or for.
    while(1)
        {
        if(a == num)
            {
            break;
            }
        printf("%d\n", a);
        a++;
        }

    /*
    // Assembly output for the above while loop.
    // Intel asm syntax
    .LC0:
            .string "%d\n"
    main:
            push    rbp
            mov     rbp, rsp
            sub     rsp, 16
            mov     DWORD PTR [rbp-8], 5
            mov     DWORD PTR [rbp-4], 0
    .L4:
            mov     eax, DWORD PTR [rbp-4]
            cmp     eax, DWORD PTR [rbp-8]
            je      .L7
            mov     eax, DWORD PTR [rbp-4]
            mov     esi, eax
            mov     edi, OFFSET FLAT:.LC0
            mov     eax, 0
            call    printf
            add     DWORD PTR [rbp-4], 1
            jmp     .L4
    .L7:
            nop
            mov     eax, 0
            leave
            ret
    */

    S_Pause();

    // Recreate a while loop (correctly) using goto (jmp)
    // This is effectively the same code as the while loop above.
    // The use of goto label is strongly discouraged unless there is
    // a specific requirement where a while or for loop is not available.
    // Start while loop
    int d = 0;
    goto L2;  // enter while loop.
L2:
    if(d == num)  // Truth test
        {
        goto L7;  // break out of loop
        }
    printf("%d\n", d);
    d++;  // Increment d.
    goto L2;
L7:  // exit while loop

    /*
    // Exact assembly output for the  goto (while) loop.
    // Intel asm syntax
    // eax, esi, etc are CPU registers.
    // Essentially the internal commands (keywords) or instruction set of the CPU.
    .LC0:
        .string "%d,\n"
    main:                               ; main()
        push    rbp                     ; main()set integers
        mov     rbp, rsp                ; main() sets integers
        sub     rsp, 16                 ; main() sets integers to 16bit
        mov     DWORD PTR [rbp-8], 5    ; Declare variable num = 5
        mov     DWORD PTR [rbp-4], 0    ; Declare variable d = 0
        nop
    .L2:                                ; Start while loop
        mov     eax, DWORD PTR [rbp-4]
        cmp     eax, DWORD PTR [rbp-8]  ; Truth test
        je      .L7                     ; if True exit loop
        mov     eax, DWORD PTR [rbp-4]
        mov     esi, eax
        mov     edi, OFFSET FLAT:.LC0
        mov     eax, 0
        call    printf                  ; print d
        add     DWORD PTR [rbp-4], 1    ; Increment d (+1)
        jmp     .L2                     ; Loop
    .L7:                                ; Exit while loop
        nop
        mov     eax, 0
        leave                           ; Exit main()
        ret                             ; main() return statement
    */

    S_Pause();
    return 0;
    }

// --> START helper functions
// Safe Pause
void S_Pause(void)
    {
    // This function is referred to as a wrapper for S_getchar()
    printf("Press any key to continue...");
    S_getchar();// Uses S_getchar() for safety.
    }

// Safe getcar() removes all artefacts from the stdin buffer.
int S_getchar(void)
    {
    // This function is referred to as a wrapper for getchar()
    int i = 0;
    int ret;
    int ch;
    // The following enumerates all characters in the buffer.
    while((ch = getchar()) != '\n' && ch != EOF )
        {
        // But only keeps and returns the first char.
        if (i < 1)
            {
            ret = ch;
            }
        i++;
        }
    return ret;
    }
