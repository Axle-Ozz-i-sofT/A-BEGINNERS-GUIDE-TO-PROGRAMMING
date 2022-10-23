//##############################################################
// Name:          Practical task
// Purpose:       Employee package delivery tracker
// Requires:      See individual Modules following
// Author:        Axle, Daniel
// Contributors:  Add name(s) for anyone who might have helped here
// Copyright:     Add copyright info here, if any, such as
//                (c) Axle 2021, , Daniel 2021
// Licence:       Add license here you'd like to use, such as creative
//                commons, GPL, Mozilla Public License, etc.
//                e.g. MIT
//                https://opensource.org/licenses/MIT
// Created:       07/09/2021 < add the date you start your
//                python file
// Last Modified: 20/10/2021 < add the last date you updated your
//                C source file
// Versioning    ("MAJOR.MINOR.PATCH") Such as Version 1.0.0
//#############################################################
// NOTES:
// https://www.w3schools.in/c-tutorial/
// https://www.programiz.com/c-programming
// ^ an excellent resource to check as you code
//#############################################################
//------------------------------------------------------------
// NOTES:
// Updated 06/01/2022
// Example csv database.
//
// TODO: strncpy, strncat for string safety
// TODO: Error handling
// TODO: split statements exceeding 80 character width.
// 
//------------------------------------------------------------

//  ---> Include standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <synchapi.h> // For Sleep() Recommend windows.h instead.

// ---> Precompiler MACROS
// MACROS are replaced by the statement or routine at compile time and make
// our source easier to read. STRINGSIZE is a little easier to understand than
// a magic number like 64
// The unused() macro simply avoids unused parameter warnings.
#define unused(x) (x) = (x)
// Set MAX string length. scanf() also limits the input by using "%60s" .
// Limits input to 60 chars.
#define INPUTSIZE 64
// set MAX string length for building the formatted lines for the csv file.
// employee_packages_delivered[] has 8 elements at MAX 64 characters.
// 8 * 64 = 512 (the minimum safe length to hold all strings from the array).
#define STRINGSIZE 512

// Declare functions used at the bottom of the page (after the formal entry point).
// The interpreter or compiler must read and know all Functions and Subroutines
// that exist before entering the application main routine. Library function, as
// well as our program functions must be read first at the top of the page
// before entering the application. We can let the Interpreter/compiler know
// they exist by declaring the Function at the top of the page, and moving the
// actual function routine to the bottom of the page.
int Enter_Daily_Packages_Delivered();
int Produce_Packages_Delivered_Report();

char *Input(char *str, char *buf, int n, FILE *stream);

// ---> START main application
int main(int argc, char *argv[])
{
    unused(argc);// turns off the compiler warning for unused argc, argv
    unused(argv);// turns off the compiler warning for unused argc, argv
Sleep(4000);
    while(1)
        {
            // Forever loop. Needs a break, return, or exit() statement to
            // exit the loop.
            // Placing the menu inside of the while loop "Refreshes" the menu
            // if an incorrect value is supplied.
            char temp[4] = {'\0'};// Inputs are as char/string.
            int option;  // Declares a menu variable as empty.

            printf("==============================================\n");
            printf("MAIN MENU\n");
            printf("==============================================\n");
            printf("1 - Enter Daily Packages Delivered\n");
            printf("2 - Produce Daily Packages Delivered Report\n");
            printf("\n");
            printf("9 - Exit The Application\n");
            printf("----------------------------------------------\n");
            Input("Enter your menu choice: ", temp, 2, stdin);
            option = atoi(temp);// Convert char/Str to an integer
            printf("----------------------------------------------\n");

            // Check what choice was entered and act accordingly
            // We can add as many choices as needed
            if( option == 1)
                {
                    Enter_Daily_Packages_Delivered();
                }
            else if(option == 2)
                {
                    Produce_Packages_Delivered_Report();
                }
            else if(option == 9)
                {
                    printf("Exiting the application...\n");
                    Sleep(1000);// Allow 1 second for the Exit notice to display.
                    break;
                }
            else
                {
                    printf("Invalid option.\nPlease enter a number from the Menu Options.\n\n");
                }
        }

    // The following is optional and waits for user input to keep the OS console
    // open before exiting the application.
    system("pause");// Wait until a key is pressed

    return 0;
}// ---> END main application <---

// ---> START Application Specific Routines
int Enter_Daily_Packages_Delivered()
{
    // Set some variables to hold values for the application
    // Also helps for better code readability
    int min_daily_deliveries = 80;
    int max_daily_deliveries = 170;// Unused ???
    int min_weekly_deliveries = 350;
    int max_weekly_deliveries = 700;
    int good_min_weekly_deliveries = 450;
    int good_max_weekly_deliveries = 600;

    // ---> START Data Structure 1
    // Basic 3 dimensional array to store our data
    // employee_packages_delivered[1][0] = "WeekNumber" (Key)
    // employee_packages_delivered[1][1] = "" (value)
    // employee_packages_delivered[n][n][64] MAX length of string is 64
    char employee_packages_delivered[8][2][INPUTSIZE] =
    {
        {{"WeekNumber"}, {'\0'}},
        {{"EmployeeID"}, {'\0'}},
        {{"EmployeeName"}, {'\0'}},
        {{"Monday"}, {'\0'}},
        {{"Tuesday"}, {'\0'}},
        {{"Wednesday"}, {'\0'}},
        {{"Thursday"}, {'\0'}},
        {{"Friday"}, {'\0'}}
    };

    // Creat a Key/Value look up table structure for employee_packages_delivered
    typedef struct index
    {
        int WeekNumber;
        int EmployeeID;
        int EmployeeName;
        int Monday;
        int Tuesday;
        int Wednesday;
        int Thursday;
        int Friday;
    } index;

    // Populate the look up table so we can use the Key to return
    // and use an integer to locate our data in the 3D array
    // employee_packages_delivered[key."Key"][value]
    struct index key_epd =
    {
        .WeekNumber = 0,
        .EmployeeID = 1,
        .EmployeeName = 2,
        .Monday = 3,
        .Tuesday = 4,
        .Wednesday = 5,
        .Thursday = 6,
        .Friday = 7
    };

    // Record the length of the 3D array for enumeration.
    int Length_employee_packages_delivered = 8;
    // END Data Structure 1 <---

    // ---> START Data Structure 2
    // 3D array to hold Weekly Report.
    char weekly_report[3][2][INPUTSIZE] =
    {
        {{"Employee_1"}, {'\0'}},
        {{"Employee_2"}, {'\0'}},
        {{"Employee_3"}, {'\0'}},
    };

    // Create a key_wr/Value look up table structure for weekly_report
    typedef struct index_wr
    {
        int Employee_1;
        int Employee_2;
        int Employee_3;
    } index_wr;

    struct index_wr key_wr =
    {
        .Employee_1 = 1,
        .Employee_2 = 2,
        .Employee_3 = 3
    };
    // weekly_report[key."Key"][value]
    int Length_weekly_report = 3;
    // END Data Structure 2 <---

    // key/value used to access both data structures.
    // employee_packages_delivered[n/key_epd."Key"][key/value]
    // [always an int and holds no values][always a char and holds a key and value]
    // [key_epd."Key"][] is used to access the [key_epd."Key"][value]
    // key also be used to access the key name [n][key]
    // Value will always be the 2nd element of the array
    int key = 0;// [n][key]
    int value = 1;// [key_epd."Key"][value]

    char char_Input_Buffer[INPUTSIZE] = {'\0'};// Temp buffer for inputs.
    char char_Temp_Buffer[INPUTSIZE] = {'\0'};// Temp buffer for input manipulations.
    char string_Temp_Buffer[STRINGSIZE] = {'\0'};// 512 characters long. 64 * 8 = 512

    // employee_packages_delivered data structure:
    // 0 WeekNumber, 1 EmployeeID, and 2 EmployeeName are range 0-2
    // Days of week are range 3-7
    // Using these ranges we can loop between employee detail and day entries
    // See routine: Part A) Enter Daily Packages Delivered, and D) Summary for Employee Week
    int starting_day = 3;

// Part A) Enter Employee Details
    // --> Start of For loop for 3 Employees
    int employee_count;
    for(employee_count = 0; employee_count < Length_weekly_report; employee_count++)
        {
            printf("\n");
            printf("\n");
            printf("==============================================\n");
            printf("Enter details for Employee %d\n", (employee_count + 1));
            // employee_count variable starts at 0, so we + 1 to offset the printf
            // employee number message.
            printf("==============================================\n");
            Input("Enter the current working week number >> ", char_Input_Buffer, 3, stdin);
            // Use a temp buffer to build the string "week n"
            // Create part one of the string...
            strcpy(char_Temp_Buffer, "week ");
            // Join our input value to the end of the string.
            strcat(char_Temp_Buffer, char_Input_Buffer);
            // Copy the created string to our array.
            strcpy(employee_packages_delivered[key_epd.WeekNumber][value], char_Temp_Buffer);
            printf("\n");  // Line Break
            Input("Enter the Employee ID >> ", char_Input_Buffer, INPUTSIZE, stdin);
            // Copy the input direct to the array.
            strcpy(employee_packages_delivered[key_epd.EmployeeID][value], char_Input_Buffer);
            printf("\n");  // Line Break
            Input("Enter the employee name >> ", char_Input_Buffer, INPUTSIZE, stdin);
            // Copy the iput direct to the array.
            strcpy(employee_packages_delivered[key_epd.EmployeeName][value], char_Input_Buffer);
            printf("----------------------------------------------\n");
            printf("\n");  // Line Break

// Part A) Enter packages delivered each day
            printf("==============================================\n");
            printf("Enter packages per day for employee %s\n", employee_packages_delivered[key_epd.EmployeeName][value]);
            printf("==============================================\n");

            int count;
            for(count = 0; count < Length_employee_packages_delivered; count++)
                {
                    if(count >= starting_day)// element 3
                        {
                            // Monday ->
                            // Note: count = Day
                            printf("Enter employee packages delivered for %s >> ", employee_packages_delivered[count][key]);// Day == count
                            Input("", char_Input_Buffer, INPUTSIZE, stdin);
                            strcpy(employee_packages_delivered[count][value], char_Input_Buffer);
                            printf("\n");  // Line Break
                        }
                }
            printf("\n----------------------------------------------\n");
            printf("\n");  // Line Break

// Part B) and D) Summary for Employee Week
            // Build our string week_ID_name_for_heading (String concatenation)
            // "Summary for employee ID:%s NAME:%s Week:%s"
            // 'Summary for employee'" ID:"43" NAME:"Jackson" Week:"17
            char week_ID_name_for_heading[128] = {'\0'};  // A temporary Buffer to hold and manipulate values.
            strcat(week_ID_name_for_heading, " ID:");
            strcat(week_ID_name_for_heading, employee_packages_delivered[key_epd.EmployeeID][value]);
            strcat(week_ID_name_for_heading, " NAME:");
            strcat(week_ID_name_for_heading, employee_packages_delivered[key_epd.EmployeeName][value]);
            strcat(week_ID_name_for_heading, " ");// Week:
            strcat(week_ID_name_for_heading, employee_packages_delivered[key_epd.WeekNumber][value]);
            

// Part B) and D)
            printf("==============================================\n");
            printf("Summary for employee%s\n", week_ID_name_for_heading); // DEBUG created in Part B, D
            printf("==============================================\n");
            int day_within_limits_flag = 0;
            // A variable to set up a flag that allows for a split if-else block
            // inside of the following loop.
            int total_deliveries = 0;
            for(count = 0; count < Length_employee_packages_delivered; count++)
                {
                    if(count >= starting_day)  // 3 Monday ->
                        {
                            // Add all deliveries for the weekly total.
                            if(atoi(employee_packages_delivered[count][value]) == 0) // error check
                                {
                                    total_deliveries = total_deliveries + 0;
                                }
                            else
                                {
                                    total_deliveries = total_deliveries + atoi(employee_packages_delivered[count][value]);
                                }
                            //total_deliveries = total_deliveries + int(employee_packages_delivered[Day])

                            if(atoi(employee_packages_delivered[count][value]) < min_daily_deliveries)
                                {
                                    // See min_daily_deliveries variable at start of program
                                    // where we set this amount.
                                    day_within_limits_flag = 1;
                                    // Flag is set to true, so we can skip the following if that
                                    // is outside of this loop.
                                    printf("%s has not delivered enough packages on %s\n", employee_packages_delivered[key_epd.EmployeeName][value], employee_packages_delivered[count][key]);
                                }
                            else if(atoi(employee_packages_delivered[count][value]) > max_daily_deliveries)
                                {
                                    // See max_daily_deliveries variable at start of program
                                    // where we set this amount.
                                    day_within_limits_flag = 1;
                                    printf("%s has delivered too many packages on %s\n", employee_packages_delivered[key_epd.EmployeeName][value], employee_packages_delivered[count][key]);
                                }
                            else
                                {
                                    // pass
                                }
                        }
                    else
                        {
                            // pass
                        }

                }

            if(day_within_limits_flag == 0)
                {
                    // The if-else block part 2.  If flag is set to 1 (True) we
                    // can skip this.
                    printf("%s has delivered within the expected daily packages.\n", employee_packages_delivered[key_epd.EmployeeName][value]);
                }
            printf("\n%s delivered a total of %d packages in %s\n", employee_packages_delivered[key_epd.EmployeeName][value], total_deliveries, employee_packages_delivered[key_epd.WeekNumber][value]);

// Part B)
            strcpy(weekly_report[employee_count][value], itoa(total_deliveries, char_Temp_Buffer, 10));// update the weekly report data structure.
            if(total_deliveries < min_weekly_deliveries)
                {
                    // See min_weekly_deliveries variable at start of program
                    // where we set this amount.
                    printf("%s did not deliver enough packages in week %s\n", employee_packages_delivered[key_epd.EmployeeName][value], employee_packages_delivered[key_epd.WeekNumber][value]);
                }
            else if(total_deliveries > max_weekly_deliveries)
                {
                    // See max_weekly_deliveries variable at start of program
                    // where we set this amount.
                    printf("%s delivered too many packages in week %s\n", employee_packages_delivered[key_epd.EmployeeName][value], employee_packages_delivered[key_epd.WeekNumber][value]);
                }
            else
                {
                    printf("%s has delivered the expected weekly packages.\n", employee_packages_delivered[key_epd.EmployeeName][value]);
                }
            printf("----------------------------------------------\n");
            printf("\n");  // Line Break

// Part C) Write to CSV
            // Note! The file write is still inside our for loop and appends the data
            // for each employee until the 3 employee records are reached.
            // The file is opened and then closed for each employee in this example.
            FILE * FileOut; // File open handle
            char file_csv[] = "DailyDeliveries_DB.csv";// Input file. // This is the same as char *file_csv =
            // Output file to write to.  Notice we save not as *.txt but to *.csv
            //==> Open Output file for text append ops.
            FileOut = fopen(file_csv, "a+"); 
            if(FileOut == NULL)// (!FileIn) alt. Test if file open success.
                {
                    printf("ERROR! Cannot open Output file %s\n", file_csv);
                    system("pause");// Wait until a key is pressed
                    return -1;
                }

            strcpy(string_Temp_Buffer, ""); // reset/clear the buffer

            int count2;
            for(count2 = 0; count2 < Length_employee_packages_delivered; count2++)
                {
                    // Join the elements of employee_packages_delivered to a
                    // single ',' delimited string.
                    // join each element to the end of buffer string
                    strcat(string_Temp_Buffer, employee_packages_delivered[count2][value]);
                    if(count2 < (Length_employee_packages_delivered -1))
                        {
                            strcat(string_Temp_Buffer, ",");// place the csv separator after each element
                        }
                    else // The last element [8] will skip adding the separator and add a new line char instead.
                        {
                            // fprintf appends the newline \n
                        }
                }
            fprintf(FileOut, "%s\n", string_Temp_Buffer);// write the buffer of joined elements to the next line of the open file (append)
            fclose(FileOut);// we must always remember to close the file when finished
        } // End of For loop for 3 Employees <--

// Part E) Employee Weekly Report
    int not_enough_deliveries = 0;
    int too_many_deliveries = 0;
    int good_number_of_deliveries = 0;
    int employee_number_counter_2 = 0;

    for(employee_number_counter_2 = 0; employee_number_counter_2 < Length_weekly_report; employee_number_counter_2++)
        {
            if(atoi(weekly_report[employee_number_counter_2][value]) < min_weekly_deliveries)
                {
                    // See min_weekly_deliveries variable at start of program
                    // where we set this amount.
                    not_enough_deliveries = not_enough_deliveries + 1;
                }
            else if(atoi(weekly_report[employee_number_counter_2][value]) > max_weekly_deliveries)
                {
                    // See max_weekly_deliveries variable at start of program
                    // where we set this amount.
                    too_many_deliveries = too_many_deliveries + 1;
                }
            else if((atoi(weekly_report[employee_number_counter_2][value]) > good_min_weekly_deliveries) && (atoi(weekly_report[employee_number_counter_2][value]) < good_max_weekly_deliveries))
                {
                    // Axle: I will correct this later.
                    // Python recommends max. 99 characters to one line, so split this
                    // statement across three lines
                    // see https://www.python.org/dev/peps/pep-0008/#maximum-line-length
                    good_number_of_deliveries = good_number_of_deliveries + 1;
                }
            else
                {
                    // pass
                }
        }

// Part E)
    printf("=================================================\n");
    printf("Weekly Employee Report\n");
    printf("=================================================\n");
    printf("%d employees delivered less than 350 packages a week\n", not_enough_deliveries);
    printf("%d employees delivered more than 700 packages a week\n", too_many_deliveries);
    printf("%d employees delivered between 450-600 packages a week\n", good_number_of_deliveries);
    printf("-------------------------------------------------\n");
    printf("\n");  // Line Break
    printf("Press [Enter] to return to the MAIN MENU...\n");
    system("pause");// Wait until a key is pressed.

    return 0;
}// END of Enter_Daily_Packages_Delivered <---

// =============================================================================
// https://www.programiz.com/c-programming/c-dynamic-memory-allocation
// https://www.geeksforgeeks.org/dynamic-memory-allocation-in-c-using-malloc-calloc-free-and-realloc/
// On the "Heap".
// =============================================================================
int Produce_Packages_Delivered_Report()
{
// Part G)
    // fields = ['Week Number', 'Employee ID', 'Employee Name', 'Monday Hrs',
    // 'Tuesday  Hrs', 'Wednesday Hrs', 'Thursday Hrs', 'Friday Hrs']

    FILE *FileIn;// File open handle
    char file_csv[] = "DailyDeliveries_DB.csv";// Input file.
    // Our earlier CSV formatted file. Notice we open *.csv and not *.txt.
    // Build our dynamic 2D/3D array here.
    // In C we also have to allocate enough char space to hold the string values to be stored.
    // Because we don't know in advance how large the csv file will be,
    // we also have to create a dynamic array in memory "On the heap" at run time.

    char char_Temp_Buffer[INPUTSIZE] = {'\0'};// Temp buffer for string manipulations
    char string_Temp_Buffer[STRINGSIZE] = {'\0'};// 512 characters long. 64 * 8 = 512

    int char_buffer;// Temporary buffer to walk the file and count the '\n' newline characters.
    int row_len = 0;// = lines in the text/csv file.
    int col_len = 8;// the 8 elements of employee_packages_delivered.
    int string = 64;// the buffer length to hold the original input data.
    int cnt1, cnt2, cnt3;

    // It is possible that the file may not yet exist. Opening it
    // as "r" will return an exception. Let's test if the file exists first.
    FileIn = fopen(file_csv, "r");// Open file for read ops
    if(FileIn == NULL)//(!FileIn) alt. Test if file open success.
        {
            printf("Error in opening Data file : %s\n", file_csv);
            printf("Maybe the CSV file has not yet been created.\n");
            printf("Please select Option 1 from the MAIN Menu\n");
            printf("to start the data entry.\n");
            printf("Press [Enter] to return to the MAIN MENU...\n");
            system("pause");
            return 0;
        }
    else// Continue to process csv file...
        {
            // For obtaining a byte count. aka number of characters in a file.
            fseek(FileIn, 0, SEEK_END);// Set pointer to end of file.
            int chars_Total = ftell(FileIn);// get counter value.
            rewind(FileIn);// Set pointer back to the start of the file.

            //Read character by character and check for new line
            // I am testing every character in the csv file rather than testing line by line.
            int cnt_chr;
            for(cnt_chr = 0; cnt_chr < chars_Total; cnt_chr++)
                {
                    char_buffer = fgetc(FileIn);

                    if(char_buffer == '\n')// Test if we have encountered a new line and,
                        {
                            row_len++;// increment the number of new lines (row_len) in the file.
                        }
                }
            rewind(FileIn);// Set pointer to start of file (start the next file read from the first character).

            // Now that we now how many lines/row_len to allocate, we can create a suitable sized array to hold the contents.
            // We already know the number of columns, and the MAX length of the values.
            // char csv_list_Buffer[lines/row_len][col_len][INPUTSIZE]// columns = Length_employee_packages_delivered,INPUTSIZE = 64
            // This is somewhat advanced and beyond the scope of a beginner, but I have no other safe option other than to create
            // the array using pointer arithmetic and dynamic memory. There are multiple ways to achieve this beyond what I have shown here.
            char ***csv_list_Buffer;// Array to hold csv file read.
            if((csv_list_Buffer = malloc(row_len * sizeof(char **))) != NULL)
                {
                    for(cnt1=0; cnt1 < row_len; cnt1++)
                        {
                            if((csv_list_Buffer[cnt1]=malloc(col_len * sizeof(char*))) != NULL)
                                {
                                    for(cnt2=0; cnt2 < col_len; cnt2++)
                                        {
                                            if((csv_list_Buffer[cnt1][cnt2]=malloc(string * sizeof(char*))) != NULL)
                                                {
                                                    for(cnt3=0; cnt3 < string; cnt3++)
                                                        {
                                                            csv_list_Buffer[cnt1][cnt2][cnt3] = '\0';// Initialise the 3D array to nul
                                                        }
                                                }
                                        }
                                }
                        }
                }
            else
                {
                    //This constitutes an application failure from which we must close the application.
                    // The user should never receive this error! :)
                    printf("Error - unable to allocate required memory for csv table.\n");
                    return -1;
                }

            // Next we need to read each line to a buffer and then split each value to
            // its correct array location, removing the delimiters ',' and New line chars '\n'
            // Clearing the buffer is not usually required. I just do it for safety when doing string manipulations.
            memset(char_Temp_Buffer, '\0', INPUTSIZE);// Clear the buffers of previous characters
            memset(string_Temp_Buffer, '\0', STRINGSIZE);// Clear the buffers of previous characters

            int cnt_rows = 0;// track array row position
            int cnt_columns = 0;// track array column position

            while(fgets(string_Temp_Buffer, STRINGSIZE, FileIn) != NULL)
                {
                    // Walk each line from the file (returns ',' in the string
                    // with '\n' at the end).
                    // Strip the newline character from the line.
                    string_Temp_Buffer[strcspn(string_Temp_Buffer, "\r\n")] = '\0';// replace newline char '\n' with '\0'
                    cnt_columns = 0;// reset the column counter to 0

                    // Use the token ',' to split the string into the original values.
                    // get the first token.
                    char* token = strtok(string_Temp_Buffer, ",");

                    // walk through other tokens
                    while( token != NULL )
                        {
                            strcpy(csv_list_Buffer[cnt_rows][cnt_columns], token);// Copy the delimited token to the csv 3D array column.
                            token = strtok(NULL, ",");
                            cnt_columns++;// Increment the array position for next column (next token)
                        }

                    cnt_rows++; // move to next line/row and repeat.
                }
            // It is important to free up resources as soon as they are no longer required.
            fclose( FileIn );// finished file reads, close the file.

            printf("==============================================\n");
            printf("Packages Delivered Report\n");
            printf("How many reports would you like to display >> ");
            char temp[8] = {'\0'};
            int options;
            Input("", temp, 8, stdin);
            options = atoi(temp);
            printf("\n----------------------------------------------\n");


            // Check if the report number is more than the entries available.
            int int_rep_number;
            if(row_len < options)// Note! C Arrays are just integer pointers so we can't accurately test the length at runtime.
                {
                    int_rep_number = row_len;
                }
            else
                {
                    int_rep_number = options;
                }

            // Calculate the position of the last item in the list
            // minus our report number to display.
            // Note! C Arrays are just integer pointers so we can't accurately test the length at runtime.
            //We have to record the length as an int variable and use it throughout  the application when required.
            // remember that a list with 5 elements has an index
            // from [0] to [4], thus the -1
            int report_start = row_len - 1;
            int report_stop = row_len - int_rep_number;

            // Walk through the List in reverse order and printf each
            // line(Row) as text.
            // Walk over list rpt_cnt-- step at a time (or in other words, reversed)
            int rpt_cnt, j;
            // Note! C Arrays are just integer pointers so we can't accurately test the length at runtime.
            for(rpt_cnt = report_start; rpt_cnt >= report_stop; rpt_cnt--)
                {
                    // csv_list_Buffer steps backward through the number of rows.
                    for(j = 0; j < col_len; j++)
                        {
                            // Step through each element(column) in the row in forward direction.
                            printf("%s\t", csv_list_Buffer[rpt_cnt][j]);// '\t' = TAB, "%s " = single space
                            // printf each cell value in the row. This will repeat for
                            // the number of col_len in j.
                        }
                    printf("\n");  // End of row [j] col_len, next row/line
                }

            printf("Press [Enter] to return to the MAIN MENU...\n");
            system("pause");

            // Important to always "free" the memory as soon as we are finished with it.
            // Not doing so will lead to a memory leak as a new block of memory will be
            // created on the heap each time the 3D array is used.
            free(csv_list_Buffer);
        }// END file open if, else test.

    return 0;
}
// ---> END Application Specific Routines <---

// ---> START Helper routines

// A wrapper to simplify the fgets() function,
// string formatting and to emulate the Python
// Input() Function
char *Input(char *str, char *buf, int n, FILE *stream)
{
    char *empty = "";
    int ret;
    memset(buf, 0, n);
    ret = strcmp(str, empty);
    if(ret != 0)// Don't print an empty string.
    {
        printf("%s", str);// Input Message
    }
    fseek(stdin, 0, SEEK_END);
    if(fgets (buf, n, stream) != NULL)// if(fgets(buf, sizeof(buf), stdin))
        {
            buf[strcspn(buf, "\r\n")] = 0;// remove end of line
        }
    return buf;
}

// END Helper routines <---

// ---> Script Exit <---
//-------------------------------------------------------------
// NOTES/TODO:
// I Keep the extra notes block here as I often drop unused or temporary working
// out down here until I am finished.
//
//
//
//-------------------------------------------------------------
