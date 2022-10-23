//-------------------------------------------------------------------------------
// Name:        Variables.c
// Purpose:     Example
//
// Platform:    Win64, Ubuntu64
//
// Author:      Axle
// Created:     31/01/2022
// Updated:     19/02/2022
// Copyright:   (c) Axle 2022
// Licence:     MIT No Attribution
//-------------------------------------------------------------------------------

#include <stdio.h>  // include standard library headers
#include <stdlib.h>
#include <string.h>

// ---> declare functions
void test_const(void);
void test_local_vs_global(void);
void test_variable(void);
void win_api_test(void);
// ---> MACROS
#define MAXVALUE 128

// ---> Global declare & defines
const int config_max_str_len = 32;
char *my_lstring = "Global string";  // *pointer to a literal.
//char my_lstring[] = "Global string";  // same declaration as previous.
int my_variable_integer = 6;  // an Integer variable
char my_variable_string[64] = {'\0'};  // a String variable.


int main(int argc, char *argv[])  // Main procedure
    {
    // ---> Local declare & defines
    char *my_lstring = "Local string";  // *pointer to a literal.
    //char my_lstring[] = "Global string";  // same declaration as previous.
    int my_variable_integer = 3;  // an Integer variable
    char my_variable_string[64] = {'\0'};  // a String variable.
    char local_lstring[] = "Local to main()";

    // using a simple MACRO
    printf("The Maximum value allowed = %d\n\n", MAXVALUE);

    // ---> Tests
    // config_max_str_len += 1;  // [Error] assignment of read-only variable 'config_max_str_len'
    printf("// Test our Global const in local scope\n");
    printf("main(), config_max_str_len = %d\n", config_max_str_len);  // cannot be altered
    test_const();  // test in a different local scope.
    printf("\n");

    // The Local overrides global variables!
    // Although they have the same name they are different variables.
    // Global variables should be used with great care and be made up of unique names.
    printf("// The Local literal variable declaration overrides the Global variable.\n");
    printf("main(), my_lstring = %s\n", my_lstring);  // Local overrides global!
    printf("main(), my_variable_integer = %d\n", my_variable_integer);  // Local overrides global!
    printf("main(), local_lstring = %s\n", local_lstring);
    test_local_vs_global();  // test in a different local scope.
    printf("\n");

    // The Local overrides global variables!
    printf("// The literal is copied to the local variable in main()...\n");
    strcpy(my_variable_string, "my_variable_string Local to main()");
    printf("main(), my_variable_string = %s\n", my_variable_string);
    test_variable();// test in a different local scope.
    // After changing the value of the Global variable...
    printf("// The variable Local to main() has not altered.\n");
    printf("main(), my_variable_string = %s\n", my_variable_string);
    printf("\n");

    printf("Press the [Enter] key to continue...");
    getchar();  // Pause the program until a key is pressed
    return 0;
    }

void test_const(void)  // This is a void function (aka Subroutine).
    {
    // cannot be altered
    printf("test_const(), config_max_str_len = %d\n", config_max_str_len);
    }

void test_local_vs_global(void)  // This is a void function (aka Subroutine).
    {
    char local_lstring[] = "Local to test_local_vs_global()\n";
    // We have not blocked the global definition with the same local name.
    printf("// and to the Global variable in test_variable().\n");
    printf("test_lstring(), my_lstring = %s\n", my_lstring);
    printf("main(), my_variable_integer = %d\n", my_variable_integer);
    printf("main(), local_lstring = %s\n", local_lstring);
    }

void test_variable(void)  // This is a void function (aka Subroutine).
    {
    printf("test_variable(), my_variable_string = %s\n", my_variable_string);
    // The following copies the "string literal" into the Global variable.
    // The "my_variable_string Global" in the following function is a
    // true string literal as it has no variable associated with it until it
    // is copied into my_variable_string.
    strcpy(my_variable_string, "my_variable_string Global");
    printf("test_variable(), my_variable_string = %s\n", my_variable_string);
    }
