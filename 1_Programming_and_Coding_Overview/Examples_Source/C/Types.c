//------------------------------------------------------------------------------
// Name:        Types.c
// Purpose:     Examples
//
// Platform:    Win64, Ubuntu64
//
// Author:      Axle
// Created:     07/03/2022
// Updated:     
// Copyright:   (c) Axle 2022
// Licence:     MIT No Attribution
//------------------------------------------------------------------------------

// Required to to enable Long Long in MinGW and MSVCRT.dll
// 
#ifdef _WIN32
#   ifdef _WIN64
#       define __USE_MINGW_ANSI_STDIO 1
#   endif
#endif

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])  // Main procedure
{
    int a = 16;
    // sizeof() returns the width of the type in bytes.
    // sizeof() operator will return a type size_t. size_t will always be the
    // width of the pointer type for the system where it is used.
    // 16 bit, 32 bit, 64 bit. As I am using 64 bit in the example It is 8 bytes
    // wide unsigned long long.
    // long long has a different byte width between Windows x64 and Linux x64
    // So I am using the %zu modifier instead of %llu
    printf("Size of variable a : %zu\n", sizeof(a));
    printf("Size of int data type : %zu\n", sizeof(int));
    printf("Size of char data type : %zu\n", sizeof(char));
    printf("Size of float data type : %zu\n", sizeof(float));
    printf("Size of double data type : %zu\n", sizeof(double));
    printf("Size of short data type : %zu\n", sizeof(short));
    printf("Size of long data type : %zu\n", sizeof(long));
    printf("Size of long long data type : %zu\n", sizeof(long long));
    printf("Size of size_t data type : %zu\n", sizeof(size_t));
    getchar();
    return 0;
}