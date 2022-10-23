'##############################################################
' Name:          Practical task
' Purpose:       Employee package delivery tracker
' Requires:      See individual Modules following
' Author:        Axle, Daniel
' Contributors:  Add name(s) for anyone who might have helped here
' Copyright:     Add copyright info here, if any, such as
'                (c) Axle 2021, Daniel 2021
' Licence:       Add license here you'd like to use, such as creative
'                commons, GPL, Mozilla Public License, etc.
'                e.g. MIT
'                https://opensource.org/licenses/MIT
' Created:       07/09/2021 < add the date you start your
'                python file
' Last Modified: 20/10/2021 < add the last date you updated your
'                BASIC source file
' Versioning    ("MAJOR.MINOR.PATCH") Such as Version 1.0.0
'#############################################################
' NOTES:
' FreeBASIC is a modernised version of MS QBASIC, Quick BASIC
' It was created to re-imagine old games on modern hardware.
' https://sourceforge.net/projects/fbeginner/files/
' https://versaweb.dl.sourceforge.net/project/fbeginner/Fbguide%20ch%201-7.pdf
' https://www.freebasic.net/wiki/CommunityTutorials
' ^ an excellent resource to check as you code
'#############################################################
'------------------------------------------------------------
' NOTES:
' Updated 19/05/2022
' Example csv database.
'
' TODO: Error handling and string safe routines.
' TODO: Create a wrapper for a clean user Input method.
' TODO: split statements exceeding 80 character width.
' 
'------------------------------------------------------------

' Declare functions used at the bottom of the page (after the formal entry point).
' The interpreter or compiler must read and know all Functions and Subroutines
' that exist before entering the application main routine. Library function, as
' well as our program functions must be read first at the top of the page
' before entering the application. We can let the Interpreter/compiler know
' they exist by declaring the Function at the top of the page, and moving the
' actual function routine to the bottom of the page.
Declare Function main_procedure() As Integer
Declare Function Enter_Daily_Packages_Delivered() As Integer
Declare Function Produce_Packages_Delivered_Report() As Integer

Declare Sub Clear_Stdin()

Main_Procedure() ' Call to the formal application entry

' ---> START main application
Function Main_Procedure() As Integer ' Formal application entry
    
    while(1)
        ' Forever loop. Needs a break, return, or exit() statement to
        ' exit the loop.FreeBASIC = Exit While
        ' Placing the menu inside of the while loop "Refreshes" the menu
        ' if an incorrect value is supplied.
        '
        ' Clearing the keyboard buffer is me being pedantic :)
        ' FB uses the GC Compiler and MinGW(32) which can leave
        ' stray '\r''\n' in the stdin buffer.
        Clear_Stdin() ' Clear the keyboard buffer
        Dim options As Integer  ' Declares a menu variable as empty.
        
        Print "=============================================="
        Print "MAIN MENU"
        Print "=============================================="
        Print "1 - Enter Daily Packages Delivered"
        Print "2 - Produce Daily Packages Delivered Report"
        Print ""
        Print "9 - Exit The Application"
        Print "----------------------------------------------"
        Clear_Stdin()
        Input ; "Enter your menu choice: ", options
        Print "" ' Line space
        Print "----------------------------------------------"
        ' Print !"\n----------------------------------------------" ' Alternative
        
        ' Check what choice was entered and act accordingly
        ' We can add as many choices as needed
        If options = 1 Then
            Enter_Daily_Packages_Delivered()
        ElseIf options = 2 Then
            Produce_Packages_Delivered_Report()
        ElseIf options = 9 Then
            Print "Exiting the application..."
            Sleep 1000 ' Allow 1 second for the Exit notice to display.
            ' Alternative: Shell "pause"
            Exit While
        else
            Print !"Invalid option.\nPlease enter a number from the Menu Options.\n"
        End If
    Wend
    
    ' The following is optional and waits for user input to keep the OS console
    ' open before exiting the application.
    Shell "pause" 'Wait until a key is pressed
    
    return 0 
End Function' ---> END Main_Procedure <---

' ---> START Application Specific Routines
Function Enter_Daily_Packages_Delivered() As Integer
    
    ' Set some variables to hold values for the application
    ' Also helps for better code readability
    Dim As Integer min_daily_deliveries = 80
    Dim As Integer max_daily_deliveries = 170 ' Unused ???
    Dim As Integer min_weekly_deliveries = 350
    Dim As Integer max_weekly_deliveries = 700
    Dim As Integer good_min_weekly_deliveries = 450
    Dim As Integer good_max_weekly_deliveries = 600
    
    ' ---> START Data Structure 1
    ' Basic 2 dimensional array to store our data
    ' employee_packages_delivered(1, 0) = "WeekNumber" (Key)
    ' employee_packages_delivered(1, 1) = "" (value)
    Dim employee_packages_delivered(7, 1) As String = {_
    {"WeekNumber", ""},_
    {"EmployeeID", ""},_
    {"EmployeeName", ""},_
    {"Monday", ""},_
    {"Tuesday", ""},_
    {"Wednesday", ""},_
    {"Thursday", ""},_
    {"Friday", ""}}
    
    ' Create a Key/Value lookup table structure for employee_packages_delivered
    Type index
        WeekNumber As Integer
        EmployeeID As Integer
        EmployeeName As Integer
        Monday As Integer
        Tuesday As Integer
        Wednesday As Integer
        Thursday As Integer
        Friday As Integer
    End Type
    
    ' Populate the lookup table so we can use the Key to return
    ' and use an integer to locate our data in the 3D array
    ' employee_packages_delivered(key."Key", value)
    Dim key_epd as index
    key_epd.WeekNumber = 0
    key_epd.EmployeeID = 1
    key_epd.EmployeeName = 2
    key_epd.Monday = 3
    key_epd.Tuesday = 4
    key_epd.Wednesday = 5
    key_epd.Thursday = 6
    key_epd.Friday = 7
    
    ' Record the length of the 3D array for enumeration.
    Dim As Integer Length_employee_packages_delivered = 8
    ' END Data Structure 1 <---
    
    ' ---> START Data Structure 2
    ' 3D array to hold Weekly Report.
    Dim weekly_report(2, 1) As String = {_
    {"Employee_1", ""},_
    {"Employee_2", ""},_
    {"Employee_3", ""}}
    
    
    ' Create a key_wr/Value look up table structure for weekly_report
    Type index_wr
        Employee_1 As Integer
        Employee_2 As Integer
        Employee_3 As Integer
    End Type
    
    Dim key_wr As index_wr
    key_wr.Employee_1 = 1
    key_wr.Employee_2 = 2
    key_wr.Employee_3 = 3
    
    ' weekly_report(key."Key", value)
    Dim As Integer Length_weekly_report = 3
    ' END Data Structure 2 <---
    
    ' key/value used to access both data structures.
    ' employee_packages_delivered(n/key_epd."Key", key/value)
    ' (always an int and holds no values, always a char and holds a key and value)
    ' (key_epd."Key", ) is used to access the (key_epd."Key", value)
    ' key also be used to access the key name (n, key)
    ' Value will always be the 2nd element of the array
    Dim As Integer key = 0' (n, key)
    Dim As Integer value = 1' (key_epd."Key", value)
    
    Dim As String char_Input_Buffer = "" ' Temp buffer for inputs.
    Dim As String char_Temp_Buffer = "" ' Temp buffer for input manipulations.
    Dim As String string_Temp_Buffer = "" ' Temp buffer for string manipulations.
    
    ' employee_packages_delivered data structure:
    ' 0 WeekNumber, 1 EmployeeID, and 2 EmployeeName are range 0-2
    ' Days of week are range 3-7
    ' Using these ranges we can loop between employee detail and day entries
    ' See routine: Part A) Enter Daily Packages Delivered, and D) Summary for Employee Week
    Dim As Integer starting_day = 3
    
' Part A) Enter Employee Details
    ' --> Start of For loop for 3 Employees
    Dim employee_count As Integer
    For employee_count = 0 To Length_weekly_report -1 Step 1
        Print ""
        Print ""
        Print "=============================================="
        Print "Enter details for Employee "; employee_count + 1
        ' employee_count variable starts at 0, so we + 1 to offset the Print
        ' employee number message.
        Print "=============================================="
        Clear_Stdin()
        Input "Enter the current working week number >> ", char_Input_Buffer
        ' Use a temp buffer to build the string "week n"
        ' Create part one of the string and join our input value to the end of the string.
        char_Temp_Buffer = "week " & char_Input_Buffer
        ' Copy the created string to our array.
        employee_packages_delivered(key_epd.WeekNumber, value) = char_Temp_Buffer
        Print "" ' Line Break
        Clear_Stdin()
        Input "Enter the Employee ID >> ", char_Input_Buffer
        ' Copy the input direct to the array.
        employee_packages_delivered(key_epd.EmployeeID, value) = char_Input_Buffer
        Print "" ' Line Break
        Clear_Stdin()
        Input "Enter the employee name >> ", char_Input_Buffer
        ' Copy the input direct to the array.
        employee_packages_delivered(key_epd.EmployeeName, value) = char_Input_Buffer
        Print "----------------------------------------------"
        Print "" ' Line Break
        
' Part A) Enter packages delivered each day
        Print "=============================================="
        Print "Enter packages per day for employee "; employee_packages_delivered(key_epd.EmployeeName, value)
        Print "=============================================="
        
        Dim count As Integer
        For count = 0 To Length_employee_packages_delivered -1 Step 1
            If count >= starting_day Then' element 3
                ' Monday ->
                ' Note: count = Day
                Clear_Stdin()
                Print "Enter employee packages delivered for "; employee_packages_delivered(count, key); " >> ";
                Input "", char_Input_Buffer
                employee_packages_delivered(count, value) = char_Input_Buffer
                Print "" ' Line Break
            End If
        Next count

        Print !"\n----------------------------------------------"
        Print "" ' Line Break

' Part B) and D) Summary for Employee Week
        ' Build our string week_ID_name_for_heading (String concatenation)
        ' "Summary for employee ID:var NAME:var Week:var"
        ' 'Summary for employee'" ID:"43" NAME:"Jackson" Week:"17
        Dim week_ID_name_for_heading As String = _ ' A temporary Buffer to hold and manipulate values.
        " ID:" &_' Use & instead of + to include Integer values.
        employee_packages_delivered(key_epd.EmployeeID, value) &_
        " NAME:" &_
        employee_packages_delivered(key_epd.EmployeeName, value) &_
        " " &_ ' Week:
        employee_packages_delivered(key_epd.WeekNumber, value)

' Part B) and D)
        'https://www.freebasic.net/forum/viewtopic.php?t=1626 ' atoi()
        Print "=============================================="
        Print "Summary for employee "; week_ID_name_for_heading ' DEBUG created in Part B, D
        Print "=============================================="
        Dim As Integer day_within_limits_flag = 0
        ' A variable to set up a flag that allows for a split if-else block
        ' inside of the following loop
        Dim As Integer total_deliveries = 0
        For count = 0 To Length_employee_packages_delivered -1 Step +1
            If count >= starting_day Then' 3 Monday ->
                ' Add all deliveries for the weekly total.
                If ValInt(employee_packages_delivered(count, value)) = 0 Then ' error check
                    total_deliveries = total_deliveries + 0
                Else
                    total_deliveries = total_deliveries + ValInt(employee_packages_delivered(count, value))
                End If
                
                If ValInt(employee_packages_delivered(count, value)) < min_daily_deliveries Then
                    ' See min_daily_deliveries variable at start of program
                    ' where we set this amount.
                    day_within_limits_flag = 1
                    ' Flag is set to true, so we can skip the following if that
                    ' is outside of this loop.
                    Print employee_packages_delivered(key_epd.EmployeeName, value);" has not delivered enough packages on "; employee_packages_delivered(count, key)
                ElseIf ValInt(employee_packages_delivered(count, value)) > max_daily_deliveries Then
                    ' See max_daily_deliveries variable at start of program
                    ' where we set this amount.
                    day_within_limits_flag = 1
                    Print employee_packages_delivered(key_epd.EmployeeName, value);" has delivered too many packages on ";employee_packages_delivered(count, key)
                End If
            End If
            
        Next count

        If day_within_limits_flag = 0 Then
            ' The if-else block part 2.  If flag is set to 1 (True) we
            ' can skip this.
            Print employee_packages_delivered(key_epd.EmployeeName, value);" has delivered within the expected daily packages."
        End If
        Print employee_packages_delivered(key_epd.EmployeeName, value);" delivered a total of";total_deliveries;" packages in ";employee_packages_delivered(key_epd.WeekNumber, value)
        
' Part B)
        weekly_report(employee_count, value) = Str(total_deliveries)' update the weekly report data structure.
        If total_deliveries < min_weekly_deliveries Then
            ' See min_weekly_deliveries variable at start of program
            ' where we set this amount.
            Print employee_packages_delivered(key_epd.EmployeeName, value);" did not deliver enough packages in week ";employee_packages_delivered(key_epd.WeekNumber, value)
        ElseIf total_deliveries > max_weekly_deliveries Then
            ' See max_weekly_deliveries variable at start of program
            ' where we set this amount.
            Print employee_packages_delivered(key_epd.EmployeeName, value);" delivered too many packages in week ";employee_packages_delivered(key_epd.WeekNumber, value)
        Else
            Print employee_packages_delivered(key_epd.EmployeeName, value);" has delivered the expected weekly packages."
        End If
        
        Print "----------------------------------------------"
        Print ""  ' Line Break

' Part C) Write to CSV
        ' Note! The file write is still inside our for loop and appends the data
        ' for each employee until the 3 employee records are reached.
        ' The file is opened and then closed for each employee in this example.
        Const file_csv As String = "DailyDeliveries_DB.csv" ' Input file.
        ' Output file to write to.  Notice we save not as *.txt but to *.csv
        '==> Open Output file for text append ops.
        Dim FileOut As Integer = FreeFile()
        If 0 <> Open(file_csv, For Append, As FileOut) Then
            Print "ERROR! Cannot open Output file " & file_csv
            Shell "pause"' Wait until a key is pressed
            Return -1
        End If

        string_Temp_Buffer = "" ' reset/clear the buffer
        
        Dim count2 As Integer
        For count2 = 0 To Length_employee_packages_delivered -1 Step 1
            ' Join the elements of employee_packages_delivered to a
            ' single ',' delimited string.
            ' join each element to the end of buffer string
            string_Temp_Buffer = string_Temp_Buffer & employee_packages_delivered(count2, value)
            If count2 < Length_employee_packages_delivered -1 Then
                string_Temp_Buffer = string_Temp_Buffer & ","' place the csv separator after each element
            Else ' The last element [8] will skip adding the separator and add a new line char instead.
                ' Print # appends the newline \n
            End If
        Next count2
        ' Write the buffer of joined elements to the next line of the open file (append).
        Print #FileOut, string_Temp_Buffer
        ' We must always remember to close the file when finished.
        Close #FileOut

    Next employee_count' End of For loop for 3 Employees <--
    
' Part E) Employee Weekly Report
    Dim As Integer not_enough_deliveries = 0
    Dim As Integer too_many_deliveries = 0
    Dim As Integer good_number_of_deliveries = 0
    Dim As Integer employee_number_counter_2 = 0
    
    for employee_number_counter_2 = 0 To Length_weekly_report-1 Step 1
        If ValInt(weekly_report(employee_number_counter_2, value)) < min_weekly_deliveries Then
            ' See min_weekly_deliveries variable at start of program
            ' where we set this amount.
            not_enough_deliveries = not_enough_deliveries + 1
        ElseIf ValInt(weekly_report(employee_number_counter_2, value)) > max_weekly_deliveries Then
            ' See max_weekly_deliveries variable at start of program
            ' where we set this amount.
            too_many_deliveries = too_many_deliveries + 1
        ElseIf (ValInt(weekly_report(employee_number_counter_2, value)) > good_min_weekly_deliveries) And (ValInt(weekly_report(employee_number_counter_2, value)) < good_max_weekly_deliveries) Then
            ' Axle: I will correct this later.
            ' Python recommends max. 99 characters to one line, so split this
            ' statement across three lines
            ' see https:'www.python.org/dev/peps/pep-0008/#maximum-line-length
            good_number_of_deliveries = good_number_of_deliveries + 1
        Else
            ' pass
        End If
    Next employee_number_counter_2
    
' Part E)
    Print "================================================="
    Print "Weekly Employee Report"
    Print "================================================="
    Print not_enough_deliveries;" employees delivered less than 350 packages a week"
    Print too_many_deliveries;" employees delivered more than 700 packages a week"
    Print good_number_of_deliveries;" employees delivered between 450-600 packages a week"
    Print "-------------------------------------------------"
    Print ""  ' Line Break
    Print "Press [Enter] to return to the MAIN MENU..."
    Shell "pause"' Wait until a key is pressed.

    return 0
End Function ' END of Enter_Daily_Packages_Delivered <---

' ==============================================================================
' https://documentation.help/FreeBASIC/ProPgVarLenArrays.html
'   Dim array(Any)
'   ReDim array(n elements)
' On the "Heap".
' ==============================================================================
Function Produce_Packages_Delivered_Report() As Integer
    '' Note!! An array(9) has 10 elements 0 To 9.
    '' As we don't use counter < length in for loops in basic, we must use length -1
    ''
    '' Dim As Integer length = 5
    '' For y = 0 To length -1 Step 1 ' (aka When y = 5-1)
    ''    0,1,2,3,4
    '' Next y
    
' Part G)
    ' fields = ['Week Number', 'Employee ID', 'Employee Name', 'Monday Hrs',
    ' 'Tuesday  Hrs', 'Wednesday Hrs', 'Thursday Hrs', 'Friday Hrs']
    
    Const file_csv As String = "DailyDeliveries_DB.csv"' Input file.
    ' Our earlier CSV formatted file. Notice we open *.csv and not *.txt.
    ' Build our dynamic 2D/3D array here.
    ' in FreeBASIC we also have to allocate enough char space to hold the string values to be stored.
    ' Because we don't know in advance how large the file will be,
    ' we also have to create a dynamic array in memory "On the heap" at run time.

    Dim As String csv_list_Buffer(Any,Any) ' Array declared on dynamic memory (On the heap).
    
    Dim temp_buffer As String ' Temporary buffer
    
    Dim As Integer x, y ' To enumerate and set array parameters.
    Dim As Integer row_len = 0 ' = lines in the text/csv file (To be calculated).
    Dim As Integer col_len = 8 ' The 8 elements of employee_packages_delivered.
    
    Dim FileIn As Integer = Freefile()
    ' It is possible that the file may not yet exist. Opening it
    ' as "read/Input" will return an error. Let's test if the file exists first.
    If Open(file_csv, For Input, As FileIn) <> 0 Then
        Print "ERROR! Cannot open Output file " & file_csv
        Print "Maybe the CSV file has not yet been created."
        Print "Please select Option 1 from the MAIN Menu"
        Print "to start the data entry."
        Print "Press [Enter] to return to the MAIN MENU..."
        Shell "pause"'wait until a key is pressed
        Return 0
        
    Else' Continue to process csv file...
        
        ' Check the file for the number of lines/Rows.
        ' Line Input # will read one line (up to '\r''\n') at a time.
        While Not Eof (FileIn)
            Line Input #FileIn, temp_buffer
            row_len = row_len + 1
        Wend
        Seek #FileIn, 1 ' Set pointer back to the start of the file.
        
        ' Now that we know how many lines/rows to allocate, we can create a suitable sized array to hold the contents.
        ' We already know the number of columns (= 8).
        ' Using ReDim we can set the required size of the dynamic array on the heap at runtime.
        ' (8 -1 = 7) | 0,1,2,3,4,5,6,7 = 8 elements
        Redim csv_list_Buffer(row_len -1, col_len -1)
        ' Read the file into Data Structure 3 (2 dimensional Dynamic Array)
        ' at its correct array location, removing the delimiters ',' and New line chars '\n'
        ' The following method is designed for CSV files and automatically removes the ',' delimiter and newline characters.

        For y = 0 To row_len -1
            For x = 0 To col_len -1
                Input #FileIn, csv_list_Buffer(y,x) ' add each value to the correct array position by (row, column).
            Next x
        Next y
        
        ' It is important to free up resources as soon as they are no longer required.
        Close #FileIn' finished file reads, close the file.
        
        Print "=============================================="
        Print "Packages Delivered Report"
        Dim  options As Integer
        Clear_Stdin()
        Input "How many reports would you like to display >> ",  options
        Print !"\n----------------------------------------------"
        
        ' Check if the report number is more than the entries available.
        Dim int_rep_number As Integer
        If row_len <  options Then' Note! C Arrays are just integer pointers so we can't accurately test the length at runtime.
            int_rep_number = row_len
        Else
            int_rep_number =  options
        End If

        ' Calculate the position of the last item in the list
        ' minus our report number to display.
        ' I have recorded the length of Rows and don't have to recalculate it's value
        ' remember that a list with 5 elements has an index
        ' from [0] to [4], thus the -1
        Dim As Integer report_start = row_len
        Dim As Integer report_stop = row_len - int_rep_number
        
        ' Walk through the List in reverse order and Print each
        ' line(Row) as text.
        ' Walk over list Step -1 step at a time (or in other words, reversed)
        Dim As Integer rpt_cnt, j
        For rpt_cnt = report_start -1 To report_stop Step -1 ' Step Backward (+1 to fix alignment)
            ' csv_list_Buffer steps backward through the number of rows.
            
            For j = 0 To col_len -1 Step 1
                ' Step through each element(column) in the row in a forward direction.
                Print csv_list_Buffer(rpt_cnt, j);!"\t"; ' '\t' = TAB, "%s " = single space
                ' Print each cell value in the row. This will repeat for
                ' the number of columns in j.
            Next j
            Print ""  ' End of row [j] columns, next row/line
        Next rpt_cnt
        
        Print "Press [Enter] to return to the MAIN MENU..."
        Shell "pause"'wait until a key is pressed
        'system("pause");
        
        ' Important to always "free" the memory as soon as we are finished with it.
        ' Not doing so will lead to a memory leak as a new block of memory will be
        ' created on the heap each time the dynamic array is used.
        ' I am uncertain if dynamic memory (On the Heap) is freed when the Function exits,
        ' so I am just playing safe ¯\_(''/)_/¯
        Erase csv_list_Buffer
    End If' END file open if, else test.
    
    Return 0
End Function
' ---> END Application Specific Routines <---

' ---> START Helper routines

' A wrapper to flush/clear the keyboard input buffer
Sub Clear_Stdin()
    While Inkey <> ""  '' loop until the Inkey buffer is empty
    Wend
End Sub

' END Helper routines <---

' ---> Script Exit <---
'-------------------------------------------------------------
' NOTES/TODO:
' I Keep the extra notes block here as I often drop unused or temporary working
' out down here until I am finished.
'
'
'
'-------------------------------------------------------------
