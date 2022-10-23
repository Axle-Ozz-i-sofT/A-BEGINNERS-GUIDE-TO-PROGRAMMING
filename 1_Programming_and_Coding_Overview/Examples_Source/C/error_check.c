//------------------------------------------------------------------------------
// Name:        error_check.c
// Purpose:     Example
// Title:       "Input Validation Checks"
//
// Platform:    Win64, Ubuntu64
//
// Author:      Axle
// Created:     03/02/2022
// Updated:     19/02/2022
// Copyright:   (c) Axle 2022
// Licence:     MIT No Attribution
//------------------------------------------------------------------------------

// C std library headers.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper functions
void S_Pause(void);
int S_getchar(void);
// v Emulates Input function in FreeBASIC and Python 3.
char *Input(char *buf, char *str, int n);
char *S_fgets(char *buf, int n, FILE *stream);
// Maximum buffer size (Max length of a string + 1 for '\0')
#define MAX_BUFFER 128  // Set a maximum size for buffers and never exceed it.

int main(void)  // Main procedure
    {
    char Input_Buffer[MAX_BUFFER] = {'\0'};  // buffer
    unsigned int Zero_Passlength = 0;  // No Password entered
    unsigned int Min_Passlength = 6;  // Pass length Min limit
    unsigned int Max_Passlength = 12;  // Pass length Max limit
    unsigned int Max_Attempts = 3;  // Attempts limit
    unsigned int Attempts_Counter = 0;  // Attempts count
    unsigned int Opt_Out = 0;  // Opt out question y/n

    // Test that the input data is within the expected limits of 6 to 12.
    // Loops until the user enters the correct data. In real life we would
    // also need an opt out after so many tries.
    // This may appear like a lot of extra code, but it is necessary in C for
    // safety. Unlike many "High Level" languages C does not have built in
    // safeguards so it is up to the coder to be pandantic about safety.
    // If you remove all of my "Comment explanations" you will find it is not
    // that much extra code :)
    // Note. C++, BASIC and Python have far more built in buffer safeguards,
    // but it is still up to the coder to ensure that any external input is
    // within the expected range of data.
    while(strlen(Input_Buffer) < Min_Passlength
        || strlen(Input_Buffer) > Max_Passlength)
        {
        // The next conditional check is part of an unwind to break out of deeply
        // nested loops. Although there are other methods to do this as well, I 
        // wanted to keep a simple method arcoss all 3 languages.
        // The above "While" conditional test should break the loop if the
        // Password is the correct length without the folowing test, but I wanted
        // to show an unwind method just the same :)
        if(Opt_Out == 'Y' || Opt_Out == 'y')
            {
            break;  // Quit asking for a name and break out of the loop "Step 2".
            // If we wanted to reuse the Opt_Out variable again later I suggest
            // uncommenting the following line.
            //Opt_Out = 0;  // Reset Y/N the response variable.
            }
        else
            {
            // Note that we have 2 safety checks for the string that can be entered.
            // the first is the MAX_BUFFER which limits all input to be less
            // than the buffer "n" Input( , , int n). this blocks the possability
            // of a buffer overflow if the user inputs a longer string than the
            // buffer can hold. See the comments in Input().
            Input(Input_Buffer, "Please enter your password.\n Between 6 to 12 letters:"
            , MAX_BUFFER -1);
            // The second part of our test is to see if the data is within the
            // range that is expected.
            if(strlen(Input_Buffer) == Zero_Passlength)  // 0 length string.
                {
                printf("You did not enter your password...\n");
                Attempts_Counter++;
                }
            else if((strlen(Input_Buffer) > Zero_Passlength)
                 && (strlen(Input_Buffer) < Min_Passlength))  // String shorter than 6.
                {
                printf("The password you entered is too short...\n");
                Attempts_Counter++;
                }
            else if(strlen(Input_Buffer) > Max_Passlength)  // String longer than 12.
                {
                printf("The password you entered is too long...\n");
                Attempts_Counter++;
                }
            else  // String is withing range. Success.
                {
                printf("Your Password is %s\n", Input_Buffer);
                }

            // Limit the number of attempts and offer an opt out so that the
            // user is not caught in an endless loop if they decide to not
            // enter a name.
            if(Attempts_Counter >= Max_Attempts)
                {
                // keep asking in the loop untill we get a valid Y/N response.
                while(Opt_Out != 'y' && Opt_Out != 'Y')
                    {
                    printf("\nSorry you have reached the maximum number of tries!\n");
                    printf("Would you like to quit? (Y/N):\n");
                    // Characters are unsigned integers in C: int Ch = 'Y' = (char)89;
                    // Strings are an array of characters char St = "Y"; = pionter
                    // to St[0] = (char)89 = (char)'Y'
                    Opt_Out =  S_getchar();
                    if(Opt_Out =='y' || Opt_Out == 'Y')
                        {
                        break;  // Quit asking for a name and break out "step 1".
                        }
                    else if(Opt_Out == 'n' || Opt_Out == 'N')
                        {
                        Attempts_Counter = 0;  // reset the attempts counter
                        Opt_Out = 0;  // reset the opt out counter
                        break;
                        }
                    else
                        {
                        // ask again until we get a Y/N response.
                        printf("Invalid response!\n");
                        }
                    }
                }
            }

        }

    printf("\nFile read error test\n");
    // Checking the error return of a function. This is always recomended when
    // the function handles data from an unknown source aka anything outside
    // of the source code of you application. This icludes "User Inputs",
    // "Data from a file or database", "Information from the web",
    // "Communication and data transfers to other apps" ++.
    // We can never guarentee the existance of data outside of our application
    // or if it will be the data that we have expected.
    //
    // Description of:
    // FILE *fopen(const char *filename, const char *mode)
    // Description
    // The C library function FILE *fopen(const char *filename, const char
    // *mode) opens the filename pointed to, by filename using the given mode.
    // Parameters:
    // filename -- This is the C string containing the name of the file to be
    // opened.
    // mode -- This is the C string containing a file access mode. It includes:
    // ...
    // Return Value: (<- this is what you need to look for.)
    // This function returns a FILE pointer. Otherwise, NULL is returned and the
    // global variable errno is set to indicate the error.
    // If it fails to open because the file does not exist for example, we need
    // to handle the error and either create the file, or tell the user that the
    // file could not be found, or any other number of options that are apropriate
    // to the context of your application. Don't ever let and error be passed to
    // your user with the horrible "Ding Sound" and the
    // "This program has terminated unexpectedly!" warning in production code. :)
    char *filename = "filename.txt";  // a dummy file name.
    FILE *fp;
    fp = fopen(filename, "r");
    // in this example "filename.txt" does not exist so the variable fp will be
    // sent an error as the return "NULL". (Null pointer is a special form of '0').
    // Yes it sounds weird, but computers have many types of Zero and not all
    // are equal NULL != null != 0 != -0 != FALSE; but are similar.
    if(fp == NULL)
        {
        // Please note that attempting to display a FILE* as an int %d is
        // classed as "undifined behavior" and will throw a compiler warning. You
        // could also use %p for void pointer, but that too is undefined.
        // I have only done this to show that it will display 0 but should not
        // be used or relied upon in production code. Always compare FILE* to
        // NULL. NULL is a special MACRO that represents the Zero value, or the
        // value of memory of a location of FILE * (pointer) to a structure that
        // does not exist.
        // if(fp == NULL) <- correct | if(fp == 0) <- undefined behavior.
        printf("ERROR! Cannot open file %s\n", filename);
        printf("fp * = %d\n", fp);
        // perror() will retreive the error sent to the console "stderr" and
        // print the value to "stdout".
        perror("Error in opening file");
        // Do some error handling tasks to deal with why the file does not exist.
        // Maybe you need to create the file first?
        // If this is a function that you have created you may wish to "return"
        // some usefull information to the function call.
        //return -1;  // -1 Indicates an error on some platforms.
        }
    else
        {
        // No errors, so do some file read operations.
        }
    fclose(fp);  // Always close files when finished, always.

    // It is common in C to use getchar() as a pause to wait for keybord [Enter]
    // to continue. If more than 1 character is entered then the remaining
    // characters are left in the keyboard buffer and appear next time the
    // keyboard buffer is read. including the new line '\n' for enter.
    // To eleiminate this we have to clear the keyboard buffer before or after
    // each entry. My function S_getchar() "Safe Get Character" enumerates all
    // characters in the buffer clearing any unused keyboard scan codes. The
    // next tim we call for a keyboard input there are no leftover artifacts in
    // the buffer :)
    // A another alternative than this is my Input() or S_fgets()
    // "Safe Get String"  based upon getc() "Get Char" which allows us to enter
    // a maximum nuber of characters to be retreived from stdin to our buffer. 
    // S_getchar() returns a single char as int, whereas S_fgets() "Safe Get String"
    // returns a char array (String) of maximum length n.
    S_Pause();
    return 0;
    }  // END main() <---

// --> START User defined functions and wrappers

// Safe Pause. A "Pause" wrapper for S_getchar().
void S_Pause(void)
    {
    // This function is referred to as a wrapper for S_getchar()
    printf("\nPress [Enter] to continue...");
    S_getchar();  // Uses S_getchar() for safety.
    }

// Safe getcar() removes all artefacts from the stdin buffer. This function
// is how we must always retreive keyboard single character input using a loop
// to enumerate all characters in the stdin buffer keeping only the first
// character and discarding any other characters including the '\n' [Enter].
// This finction is a wrapper that reduces the boiler plate code to a single
// line so we dont have to write the same loop every time we ask for a character
// from the keyboard. Modular function libraries are the key to convenient
// coding in C. C++ (High level and OOP) includes many of these libraries by
// default. C++ is a more convenient and safer upgrade to writting in C.
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

// Console Input (stdin).
// This a a wrapper function that emulates the "Input()" function found
// in BASIC and Python 3.
// buf is the return buffer, str is printed to the screen.
// returns a pointer to buf, str is the input message...
// n is the MAX number of characters allowed. This is considered a safe input
// function as it limits the length of the input string to n reducing the risk
// of a buffer overflow.
// buf must be at least n + 1 for '\0'
char *Input(char *buf, char *str, int n)
    {
    FILE *stream = stdin;
    char *empty = "";
    int ret;
    // Test if an empty string has been sent. If so...
    ret = strcmp(str, empty);
    if(ret != 0)  // Don't print an empty string.
        {
        printf("%s", str);  // Otherwise, Print the Input Message.
        }
    return S_fgets(buf, n, stream);  // Call my safe get string function.
    // and passes the truncated string back to the original function call.
    }

// Safe fgets() removes all artefacts from the stdin buffer.
// buf must be at least n + 1 for '\0'
// Returns a pointer to char array (String) of length n.
char *S_fgets(char *buf, int n, FILE *stream)
    {
    int i = 0;
    int ch;
    //memset(buf, 0, n);
    // The following enumerates all characters in the keybord buffer.
    // The while loop will exit if it encounters an [Enter] '\n' or
    // the end of the file/stream EOF.
    while((ch = getc(stream)) != '\n' && ch != EOF )
        {
        // But only keeps and returns n chars.
        // This is my safe string feature that only returns n charaters length
        // in the returned string. No buffer overflows for me :)
        // All additional charcters entered by the user are discarded.
        // It should be noted that the coder should promt the user to
        // "Enter a MAX of n characters".
        if (i < n)
            {
            buf[i] = ch;
            }
        i++;
        }
    buf[i] = '\0';
    return buf;  // returns the truncated string.
    }