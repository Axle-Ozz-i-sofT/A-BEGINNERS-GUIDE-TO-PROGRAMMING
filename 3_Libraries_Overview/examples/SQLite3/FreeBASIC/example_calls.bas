''------------------------------------------------------------------------------
'' Name:        example_calls.c (based upon basics_1.c)
'' Purpose:     SQLite3 basic examples.
''
'' Platform:    Win64, Ubuntu64
'' Depends:     v3.34.1 plus
''
'' Author:      Axle
'' Created:     03/05/2023 (19/04/2023)
'' Updated:     15/06/2023
'' Copyright:   (c) Axle 2023
'' Licence:     MIT-0 No Attribution
''------------------------------------------------------------------------------
'' Notes: basics_2.c ozz_sql3.h
''
'' The examples are part of a small library ozz_sql3.h and are designed to
'' illustrate some of the basics of SQLite 3. They are not organized as and
'' application and are designed to be rearranged, modified or used as a base
'' from which to create a small application. Some of the example calls have been
'' commented out to allow for the basic creation and reteival of some database
'' tables. See ozz_sql3.h for a list of example functions and choose your
'' own arrangement for the test below, or alternatively create a small app
'' to accept user input and returns using the library functions.
'' Modify the library functions as required for your own use.
''------------------------------------------------------------------------------
'' TODO:
''
'' Get the MAX length of Field Names?
'' Get the length of a column, row entry?
'' This is not a native API for sqlite, so a function would need to be created
'' to analyze each individual row/col entry. For now I am just using arbitrary
'' static limits of [128, [512], [2048] <- you can increase them if needed.
''
'' Check array off by 1s.
''------------------------------------------------------------------------------

'#include <stdio.h>
'#include <stdlib.h>
'#include <string.h>
''#include <errno.h>
''#include <math.h>
''#include <conio.h>
#include once "file.bi"  '' R/W Files for BLOBs


#include once "sqlite3.bi"
#include once "example_sql3.bi"


Declare Function main_procedure() As Integer
'Declare Function Con_Pause() As Integer
main_procedure()


Function main_procedure() As Integer  ' Main procedure

    '' Examples defines.
    Dim As Integer return_code = 0
    Dim As Integer err_return = 0
    '' sqlite version
    Dim As String ver_buffer
    Dim As Integer return_val = 0

    '' Loop counters...
    Dim i As Integer = 0
    Dim As Integer j = 0
    
    '' Note Linux examples will return 3.31.1, 3032001.
	' Test if Windows or Unix OS
#ifdef __FB_WIN32__
    Dim As String char_want_version = "3.34.1"  '' Windows tests
    Dim As Integer int_want_version = 3034001  '' Windows tests
#endif

#ifdef __FB_UNIX__'__FB_LINUX__
    Dim As String char_want_version = "3.31.1"  '' Windows tests
    Dim As Integer int_want_version = 3031001  '' Windows tests
#endif

    '' SQLite does not impose file name extension naming restriction, but it
    '' is sound practice to use a naming convention that is descriptive of the
    '' database version such as .sqlite3 (or .db3 .sqlite3, .db, .db3, .s3db, .sl3)
    Dim As String file_name = "Example_DB.db"  '' The name of a database.db


    '' Short version check NO error returns.( return integer)
    return_val = sqlite3_get_version0()
    Print "Version = "; return_val
    If int_want_version = return_val Then
        Print "Correct version."
    Else
        Print "Incorrect version!"
    End If
    Print "==========================================="

    '' Short version check NO error returns. (return byref string)
    sqlite3_get_version1(ver_buffer)
    Print "Version = "; ver_buffer
    If ver_buffer = char_want_version Then
        Print "Correct version."
    Else
        Print "Incorrect version!"
    End If
    Print "==========================================="

    '' Long version check with error returns.
    err_return = sqlite3_get_version2(ver_buffer)
    If err_return = 0 Then
        Print "Version error return = "; err_return
    Elseif err_return = 1 Then
        Print "Version = "; ver_buffer
        If ver_buffer = char_want_version Then
            Print "Correct version."
        Else
            Print "Incorrect version!"
        End If
    Else  '' == -1
        Print "An internal error occured."
    End If
    Print "==========================================="


    '' File exists? ( check if a file name exists) "Example_DB.db".
    err_return = file_exists(file_name)
    If err_return = 0 Then
        Print "File "; file_name; " Not found."
    Else  '' ==1
        Print"File "; file_name; " WAS found."
    End If
    Print "==========================================="


    '' Test if "SQLite format 3" and if db file exists.
    err_return = db_file_exists(file_name)
    If err_return = 0 Then
        Print file_name; " Not found, or not SQLite3 database."
    Else  '' ==1
        Print file_name; " Is an SQLite3 database."
    End If
    Print "==========================================="

    '' Create SQLITE V3 db file as FileName.db
    '' (or .db3 .sqlite3, .db, .db3, .s3db, .sl3)
    err_return = db_file_create(file_name)
    If err_return = 2 Then
        '' Maybe add different error returns values.
        Print file_name; " already exists."
    Elseif err_return = 1 Then
        Print file_name; " successfully created."
    Else  '' err_return==0
        Print "An internal error occured."
    End If
    Print "==========================================="


    /'
    '' Delete a named database file.
    '' Remove block comments /* ... */ to use.
    err_return = db_file_delete(file_name)
    If err_return = -1 Then
        Print file_name; " was NOT deleted or not exists."
    Elseif err_return = 1 Then
        Print file_name; " was successfully deleted."
    Else '' == 0
        Print "File "; file_name; " delete action terminated by user."
    End If
    Print "==========================================="
    '/


    '' TableName exists?
    Dim As String db_table_name = "Hrs_worked_Tracker"

    err_return = db_table_exists(file_name, db_table_name)
    If err_return = 0 Then
        Print "The Table "; db_table_name; " does NOT exist in "; file_name
    Elseif err_return = 1 Then
        Print "The Table "; db_table_name; " DOES exist in "; file_name
    Else  '' == -1
        Print "There was an unknown error."
    End If
    Print "==========================================="


    '' Get the total number of tables in a named database file.
    Dim As Integer number_tables_ret = 0  '' Variable to hold returned number of tables.

    err_return = db_get_number_tables(file_name, number_tables_ret)
    If err_return = 0 Then
        Print "Could NOT retrieve number of tables from "; file_name
    Elseif err_return = 1 Then
        Print "Retrieved number of tables from from "; file_name
    Else  '' == -1
        Print "There was an unknown error."
    End If
    Print "Number of tables= "; number_tables_ret
    Print "==========================================="


    '' Return an array of table names.

    '' Create a dynamic 2D array.
    '' will return a dynamic array of all TableName in the database.
    Dim db_tbl_names() As String
    Redim db_tbl_names(number_tables_ret) As String
    '' Return an array of table names.
    err_return = db_get_tablenames(file_name, db_tbl_names())
    If err_return = 0 Then
        Print "Could NOT retrieve table names from "; file_name
    Elseif err_return = 1 Then
        Print "Retrieved table names from "; file_name
    Else  '' == -1
        Print "There was an unknown error."
    End If
    Print "Returned table names:"
    For i = 0 To number_tables_ret Step +1
        Print db_tbl_names(i)
    Next i
    '' deallocate dynamic memory.
    Erase db_tbl_names
    Print "==========================================="


    '' create table sqlite3 query statement examples.

    '' Note that SQLite will add a rowid increment automatically. If the following
    '' is added as a field ID_Name INTEGER PRIMARY KEY then this column will
    '' become an alias for rowid. You do not have to add an entry for the
    '' INTEGER PRIMARY KEY column as SQLite3 will automatically add the value.
    '' Note the C string line continuation character \
    '' Note: \ will throw a compiler warning as you can accidentaly comment out
    '' the next line using the continuation char.
    '' We can also use = "My long string "
    ''                   "on 2 lines";
    /' '' Note! this will fail with current function that only handle TEXT
    char *db_table1 = "CREATE TABLE IF NOT EXISTS Hrs_worked_Tracker\
                         (INDEX_ID Integer PRIMARY KEY\
                         , Week TEXT\
                         , Employee_ID TEXT\
                         , Name TEXT\
                         , Monday TEXT\
                         , Tuesday TEXT\
                         , Wednesday TEXT\
                         , Thursday TEXT\
                         , Friday TEXT);";
    '/

    '' A full sqlite3 query (statement) we can also create a query template
    '' here or withing the function and add the data through concatenation.
    '' This is exemplified in later functions.
    '' Note: If the table exist already no error is returned.
    Dim As String db_table1 = "CREATE TABLE IF NOT EXISTS Hrs_worked_Tracker" _
                         "(Week TEXT" _
                         ", Employee_ID TEXT" _
                         ", Name TEXT" _
                         ", Monday TEXT" _
                         ", Tuesday TEXT" _
                         ", Wednesday TEXT" _
                         ", Thursday TEXT" _
                         ", Friday TEXT);"

    err_return = db_table_create(file_name, db_table1)
    If err_return = 0 Then
        Print "Table could not be created in "; file_name
    Elseif err_return = 1 Then
        Print "Table was successfully created in "; file_name
    Else
        Print "There was an unknown error."
    End If
    Print "==========================================="


    '' Recheck if Table EXISTS after creating the empty table.
    'Dim As String db_table_name = "Hrs_worked_Tracker"

    err_return = db_table_exists(file_name, db_table_name)
    If err_return = 0 Then
        Print "The Table "; db_table_name; " does NOT exist in "; file_name
    Elseif err_return = 1 Then
        Print "The Table "; db_table_name; " DOES exist in "; file_name
    Else  '' == -1
        Print "There was an unknown error."
    End If
    Print "==========================================="


    /'
    '' delete table. See Create_empty_db. Drop Table.
    Dim As String db_table2 = "DROP TABLE IF EXISTS Hrs_worked_Tracker;"

    err_return = db_table_delete(file_name, db_table2)
    If err_return = 0 Then
        Print "Table NOT successfully deleted in "; file_name
    Elseif err_return = 1 Then
        Print "Table successfully deleted "; file_name
    Else  '' == -1
        Print "There was an unknown error."
    End If
    Print "==========================================="
    '' recheck if Table EXISTS after deleting the table.
    '/


    /'
    '' add table entry/s (will add the data at the next available rowid)
    '' Comment this out for later test if the entries become to large.
    '' TableName must exist.
    '' Single entry by Column Name:
    '' "INSERT INTO table ( column2 ) VALUES( value2 );"
    '' Full row:
    '' "INSERT INTO table (column1,column2 ,..) VALUES( value1,	value2 ,...);"
    '' Multiple rows:
    '' "INSERT INTO table (column1,column2 ,..) \
    ''              VALUES( value1,	value2 ,...), \
    ''                    (value1,value2 ,...), \
    ''                    ... \
    ''                    (value1,value2 ,...);

    '' Note: FreeBASIC cab use a number of different escape sequences in string literals.
    '' C like !"\"Quote in string\"" or "" """Qoute in string""" or "'Qoute in string'"
    '' " _ <- is the string continuation for multiple lines. (Note the space befor _)
    Dim As String db_tbl_entry = "INSERT INTO Hrs_worked_Tracker " _
                                           "(Week" _
                                           ", Employee_ID" _
                                           ", Name" _
                                           ", Monday" _
                                           ", Tuesday" _
                                           ", Wednesday" _
                                           ", Thursday" _
                                           ", Friday) " _
                                     "VALUES('1'" _
                                           ", '34'" _
                                           ", 'Joe Blogs'" _
                                           ", '7'" _
                                           ", '5'" _
                                           ", '8'" _
                                           ", '7'" _
                                           ", '9');"

    err_return = db_insert_table_rowdata(file_name, db_tbl_entry)
    If err_return = 0 Then
        Print "Row data was NOT entered into "; file_name
    Elseif err_return = 1 Then
        Print "Row data was entered into "; file_name
    Else  '' == -1
        Print "There was an unknown error."
    End If
    Print "==========================================="
    '/

''============================================================================>>

    '' Get number of columns in a named table.
    Dim As Integer number_cols_ret = 0  '' Variable for the returned number of columns.

    err_return = db_get_table_number_cols(file_name, db_table_name, number_cols_ret)
    If err_return = 0 Then
        Print "Could not retrieve table "; db_table_name; " column number from "; file_name
    Elseif err_return = 1 Then
        Print "Retrieved table "; db_table_name; " column number from "; file_name
    Else  '' == -1
        Print "There was an unknown error."
    End If

    Print "Table number of columns:"; number_cols_ret
    Print "==========================================="


    '' Get number of rows in a named table.
    Dim As Integer number_rows_ret = 0  '' Variable for the returned number of rows.

    err_return = db_get_table_number_rows(file_name, db_table_name, number_rows_ret)
    If err_return = 0 Then
        Print "Could not retrieve table "; db_table_name; " row number from "; file_name
    Elseif err_return = 1 Then
        Print "Retrieved table "; db_table_name; " row number from "; file_name
    Else  '' == -1
        Print "There was an unknown error."
    End If

    Print "Table number of rows:"; number_rows_ret
    Print "==========================================="


    '' TODO
    '' Get the MAX length of Field Names?
    '' Get the length of a column, row entry?
    '' Aa function would need to be created to analyze each individual row/col
    '' entry. For now I am just using arbitrary
    '' static limits of [128, [512], [2048] <- you can increase them if needed.
    '' see: sqlite3_column_bytes() to get the dat length.

    Dim db_tbl_data0() As String
    Redim db_tbl_data0(number_cols_ret) As String

    '' Return an array of column names (fields).
    '' Must be "Free"ed after the function call and data use.
    '' Will return a dynamic array of all column names the table.
    err_return = db_get_table_colnames(file_name, db_table_name, db_tbl_data0())
    If err_return = 0 Then
        Print "Could not retrieve table "; db_table_name; " column names from "; file_name
    Elseif err_return = 1 Then
        Print "Retrieved table "; db_table_name; " column names from "; file_name
    Else  '' == -1
        Print "There was an unknown error."
    End If

    Print "Array length = "; number_cols_ret
    Print "Table column names:"
    For i = 0 To number_cols_ret -1 Step +1  '' Watch for "Off by one" -1.
        Print "Col "; i; ":"; db_tbl_data0(i)
    Next i

    Print "==========================================="

    '' deallocate dynamic memory. ( Moved to after table search). db_search_table_rowdata_allfields()
    'Erase db_tbl_data0


''==============================================================================

    '' Retrieve all table entry/s to array[][]
    '' Must be "Free"ed after the function call and data use.
    '' Will return a dynamic array of all data it the table. The returned data
    '' will need to be sorted for inspection or use.
    Dim db_tbl_data1() As String
    Redim db_tbl_data1(number_rows_ret) As String

    ''int number_columns = 8;
    '' The number of columns can also se found internally using
    '' sqlite3_column_count(statement/stmt)
    Dim As Integer number_columns = number_cols_ret  '' From db_get_table_number_cols()

    err_return = db_list_table_rows_data(file_name, db_table_name, db_tbl_data1(), number_columns)
    If err_return = 0 Then
        Print "Could not retrieve table "; db_table_name; " data from "; file_name
    Elseif err_return = 1 Then
        Print "Retrieved table "; db_table_name; " data from "; file_name
    Else  '' == -1
        Print "There was an unknown error."
    End If

    '' Print the table data. Column 0 is the row_id.
    Print "Table data:"
    Print "Array length = "; number_rows_ret
    'For i = 0 To number_rows_ret -1 Step +1
    '    print db_tbl_data1(i)
    'Next i

    '' Simple example to obtain the rowid for other tasks.
    Dim As String concat
    For i = 0 To number_rows_ret -1 Step +1
        Print db_tbl_data1(i)  '' Print the full row data.
        concat = ""
        '' Obtain the first column (rowid) bfor ','
        For j = 0 To Len(db_tbl_data1(i)) -1 Step +1
            If Chr(db_tbl_data1(i)[j]) = "," Then
                Exit For  '' If we reach ',' then we have the value of rowid.
            End If
            concat += Chr(db_tbl_data1(i)[j])  '' concatinate each character from rowid.
        Next j
        Print "rowid = "; concat  '' Print the rowid as char string.
        '' If you wish to use this rowid as an integer you will need to convert,
        '' or cast it to an integer value.

    Next i

    '' deallocate dynamic memory
    Erase db_tbl_data1
    Print "==========================================="


    /'
    '' delete table entry/s by search word (dangerous!)
    '' NOTE!!! This needs to be revised with more narrow focus and safeguards !!!
    '' The following will delete ALL rows containing "1" and "Joe Blogs". It
    '' is appropriate to check the entry row number index_id before deleting.
    Dim As String db_row_entry = "DELETE FROM Hrs_worked_Tracker " _
                                "WHERE Week = '1' AND Name = 'Joe Blogs';"

    err_return = db_delete_table_rowdata(file_name, db_row_entry)
    If err_return = 0 Then
        Print "Row data NOT deleted from "; file_name
    Elseif err_return = 1 Then
        Print "Row data deleted from "; file_name
    Else  '' == -1
        Print "There was an unknown error."
    End If
    Print "==========================================="
    '/

''==============================================================================

    '' Get number of rows in a named table.
    number_rows_ret = 0  '' Variable for the returned number of rows.
    ''int db_get_table_number_rows(char *db_file_name, char *db_table_name, int *number_rows);
    err_return = db_get_table_number_rows(file_name, db_table_name, number_rows_ret)
    If err_return = 0 Then
        Print "Could not retrieve table "; db_table_name; " row number from "; file_name
    Elseif err_return = 1 Then
        Print "Retrieved table "; db_table_name; " row number from "; file_name
    Else  '' == -1
        Print "There was an unknown error."
    End If
    Print "Table number of rows:"; number_rows_ret
    Print "==========================================="


    '' TODO:
    '' Get last rowid. Only works after a db table was opened and not
    '' yet closed.
    ''int last_id = sqlite3_last_insert_rowid(db);
    ''printf("The last Id of the inserted row is %d\n", last_id);


    ''========================================================================>>

    '' Test if rowid exist in a table.
    ''int db_table_rowid_exists(char *file_name, char *db_table_name, int rowid);
    Dim As Integer tbl_rowid = 2

    err_return = db_table_rowid_exists(file_name, db_table_name, tbl_rowid)
    If err_return = 0 Then
        Print "Table "; db_table_name; " rowid does not exist."
    Elseif err_return = 1 Then
        Print "Table "; db_table_name; " rowid does exist."
    Else  '' == -1
        Print "There was an unknown error."
    End If

    Print "Rowid:"; tbl_rowid

    Print "==========================================="


    /'
    '' delete row by "rowid". This does not question or ask for confirmation!
    '' You will need to find the correct rowid before passing it to this example.
    '' SEE: db_list_table_rows_data() for an example.
    '' DB Browser for sqlite does not show the true "rowid" number by default. Right click
    '' on the Browse data column names and select "Show rowid column".
    '' When a rowid is deleted it is not reused until a new create row is used
    '' meaning that rowid numbers will not be contiguous. You will need to test
    '' for the correct rowid before passing it to this finction for deletion.
    Dim As Integer sql_rowid1 = 2
    ''char *db_row_entry = "DELETE FROM Hrs_worked_Tracker WHERE rowid = 2;";
    If sql_rowid1 <= number_rows_ret Then
        ''int db_delete_table_rowdata_rowid(char *db_file_name, char *db_table_name, int sql_rowid);
        err_return = db_delete_table_rowdata_rowid(file_name, db_table_name, sql_rowid1)
        If err_return = 0 Then
            Print "Rowid data NOT deleted from "; file_name
        Elseif err_return = 1 Then
            Print "Rowid data deleted from "; file_name
        Else  '' == -1
            Print "There was an unknown error."
        End If

    Else
        Print "rowid does not exist!"
    End If
    Print "==========================================="
    '/

    Dim As Integer number_rows = 0
    /'
    '' Get/update the current number of rows in the TableName.

    err_return = db_get_table_number_rows(file_name, db_table_name, number_rows_ret)
    If err_return = 0 Then
        Print "Could not retrieve table "; db_table_name; " row number from "; file_name
    Elseif err_return = 1 Then
        Print "Retrieved table "; db_table_name; " row number from "; file_name
    Else  '' == -1
        Print "There was an unknown error."
    End If
    Print "Table number of rows:"; number_rows_ret
    Print "==========================================="

    number_rows = number_rows_ret  '' Used for db_insert_table_rowdata_rowid()


    '' update/replace by rowid. This will replace/overwrite existing row data.
    '' This will replace the existing rowid or INDEX_ID with neww data for each
    '' column name assigned.
    '' Alternative rowid, INDEX_ID
    '' Note using INT/INTEGER with current function will fail!
    '' You will need to find the correct rowid before passing it to this example.
    '' SEE: db_list_table_rows_data() for an example.
    Dim As String db_table_name2 = "Hrs_worked_Tracker"
    Dim As Integer sql_rowid2 = 2
    '' This can be obtained from db_get_table_colnames(). use concatenation to create the following string.
    Dim As String db_field_names = "Week, Employee_ID, Name, Monday, Tuesday, Wednesday, Thursday, Friday"
    '' Values to replace in the rowid.
    Dim As String db_field_values = "'2', '36', 'Jill Blogs', '9', '5', '4', '7', '6'"

    '' Original entry.
    ''char *db_tbl_entry = "REPLACE INTO Hrs_worked_Tracker \
    ''                               (rowid\
    ''                               , Week\
    ''                               , Employee_ID\
    ''                               , Name\
    ''                               , Monday\
    ''                               , Tuesday\
    ''                               , Wednesday\
    ''                               , Thursday\
    ''                               , Friday) \
    ''                         VALUES( 3\
    ''                                , \"2\"\
    ''                               , \"36\"\
    ''                               , \"Jill Blogs\"\
    ''                               , \"9\"\
    ''                               , \"5\"\
    ''                               , \"5\"\
    ''                               , \"7\"\
    ''                               , \"8\");";


    If number_rows >= sql_rowid2 Then
        err_return = db_replace_table_rowdata_rowid(file_name, db_table_name2, sql_rowid2, db_field_names, db_field_values)
        If err_return = 0 Then
            Print "Rowid data was NOT replaced into "; file_name
        Elseif err_return = 1 Then
            Print "Rowid data was replaced into "; file_name
        Else  '' == -1
            Print "There was an unknown error."
        End If
    Else
        Print "rowid does not exist!"
    End If
    Print "==========================================="
    '/


    '' Get/update the current number of rows in the TableName.
    'Dim As Integer number_rows = 0
    err_return = db_get_table_number_rows(file_name, db_table_name, number_rows_ret)
    If err_return = 0 Then
        Print "Could not retrieve table "; db_table_name; " row number from "; file_name
    Elseif err_return = 1 Then
        Print "Retrieved table "; db_table_name; " row number from "; file_name
    Else  '' == -1
        Print "There was an unknown error."
    End If
    Print "Table number of rows:"; number_rows_ret
    Print "==========================================="

    number_rows = number_rows_ret  '' Used for db_insert_table_rowdata_rowid()

    /'
    '' Insert row data into a named table at rowid. (Not recommended)
    '' I had some issues with non contiguous rowid numbers. I have created a test
    '' flag to skip empty rowid. Empty row id remain unchanged and all other
    '' filled rows are moved down one slot. This is a bit hackish and not the
    '' best method. We could use VACUUM to make the rowid index contigious before
    '' each routine requiring rowid maniplulation, or we can copy the table to
    '' a memory file with contiguous rowid, or last copy the enter table to our
    '' application memory and perform the table tasks there before re-writting
    '' the table fresh.
    ''
    '' If rowid doesn't exist does not write.
    '' Copies each row down 1 at a time to create space and new row at rowid in the table.
    '' The new row is placed into the rowid using REPLACE INTO.
    '' the last row is INSERT INTO a new rowid at the end of the table
    '' Copy notes from function to here!!!
    '' Test error handling!!!
    '' Consider remove '\r\, '\n' etc. from strings.

    Dim As String db_table_name3 = "Hrs_worked_Tracker"
    Dim As Integer sql_rowid3 = 3
    '' This can be obtained from db_get_table_colnames(). use concatenation to create the following string.
    Dim As String db_field_names3 = "Week, Employee_ID, Name, Monday, Tuesday, Wednesday, Thursday, Friday"
    Dim As String db_field_values3 = "'2', '36', 'Bill Krats', '2', '3', '4', '5', '6'"

    If number_rows >= sql_rowid3 Then
        err_return = db_insert_table_rowdata_rowid(file_name, db_table_name3, sql_rowid3, db_field_names3, db_field_values3, number_columns, number_rows)
        If err_return = 0 Then
            Print "Row data was NOT inserted into "; file_name
        Elseif err_return = 1 Then
            Print "Row data was inserted into "; file_name
        Else  '' == -1
            Print "There was an unknown error."
        End If
    Else
        Print "rowid does not exist!"
    End If
    Print "==========================================="
    '/


    ''Read row from rowid. returned as csv string.
    Dim As String db_table_name4 = "Hrs_worked_Tracker"
    Dim As Integer sql_rowid4 = 4
    '' This can be obtained from db_get_table_colnames(). use concatenation to create the following string.
    ''char *db_field_names3 = "Week, Employee_ID, Name, Monday, Tuesday, Wednesday, Thursday, Friday";
    ''char *db_field_values3 = "\"2\", \"36\", \"Bill Knight\", \"2\", \"3\", \"4\", \"5\", \"6\"";
    Dim As String db_tbl_rowid_data  '' MAX row data length 2048 characters.
    db_tbl_rowid_data = ""

    If number_rows >= sql_rowid4 Then

        err_return = db_read_table_rowdata_rowid(file_name, db_table_name4, sql_rowid4, db_tbl_rowid_data, number_columns)
        If err_return = 0 Then
            Print "Rowid data was NOT read from "; file_name
        Elseif err_return = 1 Then
            Print "Rowid data was read from "; file_name
        Else  '' == -1
            Print "There was an unknown error."
        End If

        '' Print the table data.
        Print "Table rowid data:"
        Print  db_tbl_rowid_data
        Print "number of columns = "; number_columns

        Dim As Integer csv_row_length = Len(db_tbl_rowid_data)
        Print "csv_row_length characters="; csv_row_length
        Dim As Integer  ch = 0

        For i = 0 To csv_row_length -1 Step +1
            ch = db_tbl_rowid_data[i]
            If ch = Asc(",") Then  '' Find the delimiters
                '' Skip and new line. Removing white-space requires a little more.
                Print ""
            Else
                Print Chr(ch);  '' print each character for the column data.
            '' split data at ','
            End If
        Next i
        Print ""
    Else
        Print "rowid does not exist!"
    End If

    Print "==========================================="

    ''========================================================================<<


    ''========================================================================>>

    Dim db_tbl_data3() As String
    Redim db_tbl_data3(number_rows_ret) As String

    '' Search table entry/s by column (field) name.
    '' Dynamic array must be "Free"ed after the function call and data use.
    '' Will return a dynamic array of all data in the table. The returned data
    '' will need to be sorted for inspection or use.

    '' Get the number of search rows found.
    Dim As Integer ret_array_length1 = 0  '' We cannot get the length of array elements in C
    '' so we need to return the number of array positions that have been
    '' populated from the search. Alternatively we can enumerate to full array
    '' length from number_rows_ret and filter out empty '\0' elements.

    ''char *db_row_search = "SELECT FROM Hrs_worked_Tracker\
    ''                  WHERE ANY = \"Joe Blogs\";";

    '' This needs to be converted to full table search. requires EXACT match.
    '' NOTE! These 2 fields require the use ! and the escape character \"
    '' Unlike data entry and retreival which will accept "'value'".
    Dim As String field_name = !"\"Name\""  '' " 'value' " is not acceptable from C.
    Dim As String db_search_string1 = !"\"Joe Blogs\""  '' !" \"value\" " is acceptable.
    ''char *db_search_string = "\"Blogs\"";  '' research wild cards :)
    ''char temp_buffer[128] = {'\0'};
    ''for( i = 0; i < number_columns; i++)
    ''     {
    ''     strcpy(temp_buffer,db_tbl_data0[i]);
    ''     }


    '' search TableName by column name (field) and search word.
    ''int number_columns2 = 8;
    '' number_columns will be +1 because we are also retrieving the row_id number.
    err_return = db_search_table_rowdata_byfield(file_name, db_table_name, db_tbl_data3(), field_name, db_search_string1, number_columns, ret_array_length1)
    If err_return = 0 Then
        Print "Could not retrieve table "; db_table_name; " search data from "; file_name
    Elseif err_return = 1 Then
        Print "Retrieved table "; db_table_name; " search data from "; file_name
    Else  '' == -1
        Print "There was an unknown error."
    End If

    '' Print returned search result (full array).
    Print "Table column search data:"
    /'
    For i = 0 To number_rows_ret -1 Step +1
        Print db_tbl_data3[i]
    Next i
    '/
    '' Print returned search result (actual number search rows found).
    Print "Array length = "; ret_array_length1
    For i = 0 To ret_array_length1 -1 Step +1
        Print db_tbl_data3(i)
    Next i


    '' deallocate dynamic memory
    Erase db_tbl_data3
    Print "==========================================="


    Dim db_tbl_data4() As String
    Redim db_tbl_data4(number_rows_ret) As String
    'for i = 0 to number_rows_ret -1 Step +1
    '    db_tbl_data4(i) = ""
    'next i

    '' Search all columns (fields) for search string in table name. Must be EXACT
    '' search word match. Returns array in the order of rows found without duplicates.
    '' Dynamic array must be "Free"ed after the function call and data use.
    '' Will return a dynamic array of all found rows in the table. The returned data
    '' will need to be sorted for inspection or use.

    '' Get the number of search result rows found.
    Dim As Integer ret_array_length2 = 0  '' We cannot get the length of array elements in C
    '' so we need to return the number of array positions that have been
    '' populated from the search. Alternatively we can enumerate to full array
    '' length from number_rows_ret and filter out empty '\0' elements.

    ''char *db_search_string2 = "\"Joe Blogs\"";
    ''"\"2\", \"36\", \"Jill Blogs\", \"9\", \"5\", \"4\", \"7\", \"6\""  '' DEBUG
    Dim As String db_search_string2 = !"\"6\""  '' 6, 9, 5, 8
    '' # internalize field/column names ?
    '' switch number_columns next to db_tbl_data0 (column name/field_name)
    err_return = db_search_table_rowdata_allfields(file_name, db_table_name, db_tbl_data4(), db_tbl_data0(), db_search_string2, number_columns, number_rows_ret, ret_array_length2)
    If err_return = 0 Then
        Print "Could not retrieve table "; db_table_name; " search data from "; file_name
    Elseif err_return = 1 Then
        Print "Retrieved table "; db_table_name; " search data from "; file_name
    Else  '' == -1
        Print "There was an unknown error."
    End If

    '' Print returned search result.
    Print "Table ALL column search data:"
    '' Print returned search result (actuall number search rows found).
    Print "Array length = "; ret_array_length2
    For i = 0 To ret_array_length2 -1 Step +1
        Print db_tbl_data4(i)
    Next i

    '' deallocate memory
    Erase db_tbl_data4
    Erase db_tbl_data0

    Print "==========================================="


''Con_Pause()
''==============================================================================
'' START Multiple types examples.

	/'
    '' Creae a table and field for binary BLOBS
    Dim As String db_table_namex = "DATA_Blobs"  '' Table with single column BLOB
    '' Test data.
    Dim As Ubyte bin_data(0 To ...) = {&hff, &hd8, &hff, &he2, &h02, &h1c, &h49, &h43, &h43, &h5f, &h50, &h52, &h4f, &h46, &h49, &h4c}
    Dim As Integer bin_data_len = 16

    '
    '' Creae a table...
    Dim As String db_table5 = "CREATE TABLE IF NOT EXISTS DATA_Blobs" _
                         "(Binary_data BLOB);"

    err_return = db_table_create(file_name, db_table5)
    If err_return = 0 Then
        Print "Table "; db_table_namex; " could not be created in "; file_name
    Elseif err_return = 1 Then
        Print "Table "; db_table_namex; " was successfully created in "; file_name
    Else
        Print "There was an unknown error "; db_table_namex
    End If
    Print "==========================================="


    '' Insert some binary (BLOB) test data.

    '' A variation of the INSERT statement using sqlite3_bind_*().
    '' We can send data/values separately replacing the values into '?' using
    '' various sqlite3_bind*() functions. It will be necessary to know the
    '' data type and infinity before hand and use the select case as I have
    '' in the examples for reading the data.
    Dim As String db_tbl_entry5 = "INSERT INTO DATA_Blobs (Binary_data) VALUES(?);"
    '' Consider remove '\r\, '\n' etc. from strings.

    err_return = db_insert_table_rowdata_bin(file_name, db_tbl_entry5, bin_data(), bin_data_len)
    If err_return = 0 Then
        Print "Row data was NOT entered into "; file_name
    Elseif err_return = 1 Then
        Print "Row data was entered into "; file_name
    Else  '' == -1
        Print "There was an unknown error."
    End If
    Print "==========================================="
    '/

    ''==========================================================================


	/'
    ''int bin_data_len = 16;
    '' Get number of columns in a named table.
    Dim As Integer number_cols_retx = 0

    err_return = db_get_table_number_cols(file_name, db_table_namex, number_cols_retx)
    If err_return = 0 Then
        Print "Could not retrieve table "; db_table_namex; " column number from "; file_name
    Elseif err_return = 1 Then
        Print "Retrieved table "; db_table_namex; " column number from "; file_name
    Else  '' == -1
        Print "There was an unknown error."
    End If
    Print "Table number of columns:"; number_cols_retx
    Print "==========================================="


    '' Get number of rows in a named table.
    Dim As Integer number_rows_retx = 0

    err_return = db_get_table_number_rows(file_name, db_table_namex, number_rows_retx)
    If err_return = 0 Then
        Print "Could not retrieve table "; db_table_namex; " row number from "; file_name
    Elseif err_return = 1 Then
        Print "Retrieved table "; db_table_namex; " row number from "; file_name
    Else  '' == -1
        Print "There was an unknown error."
    End If
    Print "Table number of rows:"; number_rows_retx
    Print "==========================================="



    '' More universal query function for mixed data types.
    '' As you will see this is more complex than using a single data type in
    '' the table columns. Personally for small database requirements I store
    '' everything as TEXT and keep a track of the column affinity (data type)
    '' in my calling application and convert values to other types as required.
    '' The only exception to this is binary data (BLOBs) which would need to be
    '' converted to TEXT using a Base64 encoder. I would not attempt to store
    '' large amounts of binary data in this way. If you do have to store large
    '' binary data sets such as images etc then you will need to make use of the
    '' correct types and SQLite affinities as shown in this function.
    '' This will offer a sound example to build more complex database queries.
    '' See the modified version db_insert_table_rowdata_bin() for hints as how
    '' to insert mixed data types based upon the select case examples and the
    '' VARIANT structure examples.

    ''int j = 0, i = 0;
    '' NOTE: SQLite3 does have it's own internal typless data structure Mem.
    '' typedef struct Mem Mem;
    '' It is an extremely complex data structure that includes many other data
    '' structures defined in the sqlite source. Also it is predomenently used
    '' with the sqlite3_value/_* set of API functions.
    '' typedef struct sqlite3_value sqlite3_value;
    '' It is more convenient to create our own tag struct, union or linked list
    '' for the following example.

    '' We will have to intitialize/assign the dynamic array in a for loop.
    '' Get these values from int *ret_number_columns, int *ret_number_rows
    '' Elements should be the same size as variant_array_len0 unless we
    '' purposefully create an array larger then the number of rows in the table.
    Dim As Integer variant_array_rowlen = number_rows_retx  '' Always check the most recent number of rows in the table
    Dim As Integer variant_array_collen = number_cols_retx + 1  '' The number of columns + 1 for rowid

    '' Track the returned number of used elements in the array.
    Dim As Integer ret_variant_field_elements = 0
    Dim As Integer ret_variant_row_elements = 0
    '' Note: it is possible in a more complex structure to track the array size
    '' and elements used within the structure.

    '' Create 2D dynamic array using number rows and columns
    '' tagVARIANT structure is declared in example_sql3.h

    '' Note! Array is oversized by 1,1
    Dim variant_array() As tagVARIANT
    Redim variant_array(variant_array_rowlen, variant_array_collen) As tagVARIANT

    '' More universal query function for mixed data types.
    '' Note: The variant_array_collen, variant_array_rowlen are not currently
    '' used in this function call as they are calculated within the function.
    err_return = db_list_table_all_types(file_name, db_table_namex, variant_array(), variant_array_collen, variant_array_rowlen, ret_variant_field_elements, ret_variant_row_elements)
    If err_return = 0 Then
        Print "Could not retrieve "; db_table_namex; " table data from "; file_name
    Elseif err_return = 1 Then
        Print "Retrieved table "; db_table_namex; " data from "; file_name
    Else  '' == -1
        Print "There was an unknown error."
    End If

    '' Test the size of returned number of row elements against our array[n]
    '' DEBUG truth test for memory leaks/off by 1/ buffer over runs.
    If variant_array_rowlen < ret_variant_row_elements Then
        Print "Error! Dynamic array is too small for number of rows."
    Elseif variant_array_rowlen > ret_variant_row_elements Then
        Print "Good! Dynamic array is lager than number of rows."
    Elseif variant_array_rowlen = ret_variant_row_elements Then
        Print "Good! Dynamic array same size as number of rows."
    Else
        Print "Unknown error!"  '' should never occur
    End If

    '' Print all mixed data types returned from the table. Each column is sorted
    '' to the relevant type by testing [][].type
    Print "Print mixed table data from variant_array."
    '' To DEBUG test against the original length of the array of VARIANT (bin_data_len = 16)
    Dim As Integer bdata_len2 = 0
    Dim As Integer x = 0
    '' Step through each row element from the array.
    For j = 0 To variant_array_rowlen -1 Step +1
        Print "====> Row element = "; j
        For i = 0 To variant_array_collen -1 Step +1  '' < variant_array_elements
            '' NOTE: Column element 0 == rowid
            Print "|Column element| = "; i
            '' Access each element of tag_VARIANT in variant_array[n][n]...
            '' Use this to select the correct usage of the returned data.
            Select Case variant_array(j,i).sql_type
            Case IS_NULL
                '' Do stuff for NULL pointer, using variant_array[n].value.vval
                '' This will denote an unused array element. It is possible to
                '' use this data structure were we test for null as an empty element
                '' or as and empty type
                Print "NULL= "; variant_array(j,i).value.vval
                Exit Select
            Case IS_INTEGER
                '' Do stuff for INTEGER, using variant_array[n].value.ival
                Print "INTEGER= "; variant_array(j,i).value.ival
                Exit Select
            Case IS_FLOAT
                '' Do stuff for REAL, using variant_array[n].value.rvar
                Print "REAL= "; variant_array(j,i).value.rval
                Exit Select
            Case IS_TEXT
                '' Do stuff for TEXT, using variant_array[n].value.tval
                Print "TEXT= "; variant_array(j,i).value.tval
                Exit Select
            Case IS_BLOB
                '' Do stuff for BLOB, using variant_array[n].value.bvar
                ''May need to enumerate the array bval(n)
                bdata_len2 = variant_array(j,i).value.bval.blen
                
                Print "byte len = "; bdata_len2
                If 16 = bdata_len2 Then
                    Print "Returned bytes same length as original bytes."
                End If

                Print "BLOB= {";
                For x = 0 To bdata_len2 -1
                    '' For C like format use "0x" instead of "&h". Hex(Byte, padding)
                    Print "&h"; Hex(variant_array(j,i).value.bval.bdata(x), 2); ",";  '' As hexidecimal.
                    'print variant_array(j,i).value.bval.bdata(x); ",";  '' As decimal.
                    'print Chr(variant_array(j,i).value.bval.bdata(x));  '' As character.
                Next x
                Print !"\b}"

                '' Compare original with return bytes
                '' Debug compare test.bin_data
                For x = 0 To bdata_len2 -1
                    If bin_data(x) <> variant_array(j,i).value.bval.bdata(x) Then
                        Print "BLOB Data does not match!"
                        Exit For
                    End If
                Next x

                '' Sql Hex entry. Use SQLite DB manager to confirm.
                '' 0000  ff d8 ff e2 02 1c 49 43 43 5f 50 52 4f 46 49 4c  ......ICC_PROFIL
                Exit Select
            Case Else
                '' Report an error, this shouldn't happen!
                Print "##default= "; variant_array(j,i).sql_type; "##"
                Exit Select
            End Select
            Print ""
        Next i
        Print ""
    Next j

    '' Free the dynamic memory!
    Erase variant_array

    Print "==========================================="
	'/

''=============================================================================
    '' Test our original "Hrs_worked_Tracker" TEXT table with retreive all data types.
    '' This if from the first TEXT only table examples.

    '' Retrive current table name and column number.

    Dim As String db_table_namex2 = "Hrs_worked_Tracker"

	/'
    '' Get number of columns in a named table.
    ''int number_cols_retx = 0;  '' Previously defined.
    number_cols_retx = 0
    err_return = db_get_table_number_cols(file_name, db_table_namex2, number_cols_retx)
    If err_return = 0 Then
        Print "Could not retrieve table "; db_table_namex2; " column number from "; file_name
    Elseif err_return = 1 Then
        Print "Retrieved table "; db_table_namex2; " column number from "; file_name
    Else  '' == -1
        Print "There was an unknown error."
    End If
    Print "Table number of columns:"; number_cols_retx
    Print "==========================================="

    '' Retrive current table rows number.
    ''int number_rows_retx = 0;  '' Previously defined.
    number_rows_retx = 0
    err_return = db_get_table_number_rows(file_name, db_table_namex2, number_rows_retx)
    If err_return = 0 Then
        Print "Could not retrieve table "; db_table_namex2; " row number from "; file_name
    Elseif err_return = 1 Then
        Print "Retrieved table "; db_table_namex2; " row number from "; file_name
    Else  '' == -1
        Print "There was an unknown error."
    End If
    Print "Table number of rows:"; number_rows_retx

    Print "==========================================="


    '' retreieve all table data ("Hrs_worked_Tracker")
    '' Previously defined int variant_array_rowlen;int variant_array_collen
    variant_array_rowlen = number_rows_retx  '' Always check the most recent number of rows in the table
    variant_array_collen = number_cols_retx + 1  '' The number of columns + 1 for rowid

    '' Track the returned number of used elements in the array.
    '' Previously defined int ret_variant_field_elements; int ret_variant_row_elements
    ret_variant_field_elements = 0
    ret_variant_row_elements = 0

    '' Create 2D dynamic array using number rows and columns
    '' tagVARIANT structure is declared in example_sql3.h
    ''tagVARIANT **variant_array = NULL;
    
    '' Note! Array is oversized by 1,1
    'Dim variant_array() As tagVARIANT  '' Already declared above
    Redim variant_array(variant_array_rowlen, variant_array_collen) As tagVARIANT

    '' More universal query function for mixed data types.
    '' Note: The variant_array_collen, variant_array_rowlen are not currently
    '' used in this function call as they are calculated within the function.
    err_return = db_list_table_all_types(file_name, db_table_namex2, variant_array(), variant_array_collen, variant_array_rowlen, ret_variant_field_elements, ret_variant_row_elements)
    If err_return = 0 Then
        Print "Could not retrieve "; db_table_namex2; " table data from "; file_name
    Elseif err_return = 1 Then
        Print "Retrieved table "; db_table_namex2; " data from "; file_name
    Else  '' == -1
        Print "There was an unknown error."
    End If

    '' Test the size of returned number of row elements against our array[n]
    '' DEBUG truth test for memory leaks/off by 1/ buffer over runs.
    If variant_array_rowlen < ret_variant_row_elements Then
        Print "Error! Dynamic array is too small for number of rows."
    Elseif variant_array_rowlen > ret_variant_row_elements Then
        Print "Good! Dynamic array is lager than number of rows."
    Elseif variant_array_rowlen = ret_variant_row_elements Then
        Print "Good! Dynamic array same size as number of rows."
    Else
        Print "Unknown error!"  '' should never occur
    End If

    '' Print all mixed data types returned from the table. Each column is sorted
    '' to the relevant type by testing [][].type
    Print "Print mixed table data from variant_array."
    '' To DEBUG test against the original length of the array of VARIANT (bin_data_len = 16)
    ''int bdata_len2 = 0;
    ''int x = 0;
    '' Step through each row element from the array.
    For j = 0 To variant_array_rowlen -1 Step +1  '' or ret_variant_row_elements
        Print "====> Row element = "; j
        '' Step through each column element from the row.
        For i = 0 To variant_array_collen -1 Step +1  '' < ret_variant_field_elements

            Print "|Column element| = "; i
            '' Access each element of tag_VARIANT in variant_array[n][n]...
            '' Use this to select the correct usage of the returned data.
            Select Case variant_array(j,i).sql_type
            Case IS_NULL
                '' Do stuff for NULL pointer, using variant_array[n].value.vval
                '' This will denote an unused array element. It is possible to
                '' use this data structure were we test for null as an empty element
                '' or as and empty type
                Print "NULL= "; variant_array(j,i).value.vval
                Exit Select
            Case IS_INTEGER
                '' Do stuff for INTEGER, using variant_array[n].value.ival
                Print "INTEGER= "; variant_array(j,i).value.ival
                Exit Select
            Case IS_FLOAT
                '' Do stuff for REAL, using variant_array[n].value.rvar
                Print "REAL= "; variant_array(j,i).value.rval
                Exit Select
            Case IS_TEXT
                '' Do stuff for TEXT, using variant_array[n].value.tval
                Print "TEXT= "; variant_array(j,i).value.tval
                Exit Select
            Case IS_BLOB
                '' Do stuff for BLOB, using variant_array[n].value.bvar
                ''May need to enumerate the array bval(n)
                bdata_len2 = variant_array(j,i).value.bval.blen
                
                Print "byte len = "; bdata_len2
                If 16 = bdata_len2 Then
                    Print "Returned bytes same length as original bytes."
                End If

                Print "BLOB= {";
                For x = 0 To bdata_len2 -1
                    '' For C like format use "0x" instead of "&h". Hex(Byte, padding)
                    Print "&h"; Hex(variant_array(j,i).value.bval.bdata(x), 2); ",";  '' As hexidecimal.
                    'print variant_array(j,i).value.bval.bdata(x); ",";  '' As decimal.
                    'print Chr(variant_array(j,i).value.bval.bdata(x));  '' As character.
                Next x
                Print !"\b}"

                '' Compare original with return bytes
                '' Debug compare test.bin_data
                For x = 0 To bdata_len2 -1
                    If bin_data(x) <> variant_array(j,i).value.bval.bdata(x) Then
                        Print "BLOB Data does not match!"
                        Exit For
                    End If
                Next x

                '' Sql Hex entry. Use SQLite DB manager to confirm.
                '' 0000  ff d8 ff e2 02 1c 49 43 43 5f 50 52 4f 46 49 4c  ......ICC_PROFIL
                Exit Select
            Case Else
                '' Report an error, this shouldn't happen!
                Print "##default= "; variant_array(j,i).sql_type; "##"
                Exit Select
            End Select
            Print ""
        Next i
        Print ""
    Next j

    Print "==========================================="
	'/

    ''========================================================================<<


    ''========================================================================>>
    '' ====>> Do insert and retreive from mixed data types table. ===========>>

	/'
    '' You will need this table name available to all of the following examples.
    Dim As String db_table_mixed = "DATA_Mixed"  '' Table with mixed data types.

    '
    Dim As String sql_table_fields

    '' Note: REAL == SQLITE_FLOAT == IS_FLOAT == 2 (int constants)
    sql_table_fields = "CREATE TABLE IF NOT EXISTS " + db_table_mixed + "" _
                              "(Date TEXT" _
                              ", Week INTEGER" _
                              ", Employee_ID INTEGER" _
                              ", Name INTEGER" _
                              ", Avatar BLOB" _
                              ", Monday INTEGER" _
                              ", Tuesday INTEGER" _
                              ", Wednesday INTEGER" _
                              ", Thursday INTEGER" _
                              ", Friday INTEGER);"

    '' Create mixed data table (empty).
    err_return = db_table_create(file_name, sql_table_fields)
    If err_return = 0 Then
        Print "Table "; db_table_namex2; " could not be created in "; file_name
    Elseif err_return = 1 Then
        Print "Table "; db_table_namex2; " was successfully created in "; file_name
    Else
        Print "There was an unknown error "; db_table_namex2
    End If
    Print "==========================================="
    '/

	/'
    '' read image file Tux avatar for BLOB.
    Dim As String file_tux = "Tux.jpg"  '' 16,756 bytes

    Dim As Integer chars_total = 0
    Dim fp_tux As Integer = Freefile()
    'unsigned char *bin_avatar = NULL;
    Dim As Ubyte bin_avatar()

    err_return = Open(file_tux, For Binary Access Read, As fp_tux)  '' For Input == read only.
    If err_return <> 0 Then
        Print "Failed to open file "; file_tux  '' DEBUG
    Else
        '' For obtaining a byte count. aka number of characters in a file.
        'fseek(fp_tux, 0, SEEK_END);  '' Set pointer to end of file.
        'chars_Total = ftell(fp_tux);  '' get counter value.
        'rewind(fp_tux);  '' Set pointer back to the start of the file.
        chars_total = Lof(fp_tux)
        Print "chars_total = "; chars_total  '' 16,756 bytes

        '' Create a dynamic array of size chars_Total.
        'bin_avatar = (unsigned char*)malloc(chars_Total * sizeof(unsigned char));
        Redim bin_avatar(chars_Total) As Ubyte  '' Resize the data array to the size of image file in bytes
        '' Read the binary data into the array.
        'fread(bin_avatar, sizeof(unsigned char), chars_total, fp_tux);
        'Dim As Integer i = 0
        i = 0
        While Not Eof (fp_tux)
            Get #fp_tux, ,bin_avatar(i)
            '' get #fp_tux, 1,char_buffer(i)  '' To begin at first byte.
            i += 1  '' Increment each byte element as the file is read
            '' this total i should be the same as chars_total.
        Wend

        Close #fp_tux '' Close the file.
    End If
	'/

    /'
    '' DEBUG test.
    For i = 0 To chars_Total -1 Step +1
        'print bin_avatar(i); ",";
        '' For C like format use "0x" instead of "&h". Hex(Byte, padding)
        Print "&h"; Hex(bin_avatar(i), 2); ",";
        'print Chr(bin_avatar(i));
        Next i
    Print ""
    '/

    /'
    '' Converting from 8bit byte(int) (2 * oct) to char required 2 * bytes + 1 for string terminator '\0'.
    '' This will create a string of hex pairs 'ffb623 ...'.
    '' SQLite requires the string formated as The x'ffb623 ...' the x will be
    '' added when creating the query.
    'char hex_buffer[4] = {'\0'};
    'char *bin_avatar_hexstr = (char*)malloc((2 * chars_Total +1) * sizeof(char));
    Dim As String bin_avatar_hexstr
    bin_avatar_hexstr = ""  '' Initiate the array for strcat()

    For i = 0 To chars_Total -1 Step +1
        'sprintf(hex_buffer, "%02x", bin_avatar[i]);  '' Convert byte to hex.
        
        'strcat(bin_avatar_hexstr, hex_buffer);  '' add each hex to string.
        bin_avatar_hexstr += Hex(bin_avatar(i), 2)  '' Convert byte to hex, add each hex to string.
    Next i
    '' DEBUG print hex string.
    'print "Hex string:"
    'print bin_avatar_hexstr

    Erase bin_avatar  '' Clear the first read buffer from file read.


    '' The following method inserts the data as part of the query statement.
    '' NOTE! It is better in practice to use the sqlite3_bind_blob() for this.

    '' NOTE: Sqlite date, time, datetime, julianday and strftime are built in
    '' functions and only accessed via the querry statements.
    '' TEXT == YYYY-MM-DD HH:MM:SS == datetime( ... ) (default)
    '' https:''www.sqlite.org/lang_datefunc.html
    '' If you need Date and Time functions outside of sqlite use the C <time.h>
    '' functions. The date time will need to be formated using sprintf() to match
    '' with the ISO date format YYYY-MM-DD HH:MM:SS.


    '' Placing the hex string directly into the query statement. The hex string
    '' must be prefixed with 'x'  x'ffb623'.
    '' The beter and safer way is to use ? placeholder and sqlite3_bind_* API.

    '' Note that this proccess is a little more complicated than python due to
    '' the need for dynamic arrays and data conversions.

    '' Add additional 1024 buffer space for the query statement.
    'char *sql_tbl_entry = (char*)malloc(((2 * chars_Total +1) + 1024) * sizeof(char));
    Dim As String sql_tbl_entry

    sql_tbl_entry = "INSERT INTO " + db_table_mixed + "" _
                           "(Date" _
                           ", Week" _
                           ", Employee_ID" _
                           ", Name" _
                           ", Avatar" _
                           ", Monday" _
                           ", Tuesday" _
                           ", Wednesday" _
                           ", Thursday" _
                           ", Friday)" _
                           "VALUES(datetime('now', 'localtime')" _
                           ", 1" _
                           ", 34" _
                           ", 'Joe Blogs'" _
                           ", x'" + bin_avatar_hexstr + "'" _
                           ", 7" _
                           ", 5" _
                           ", 8" _
                           ", 7" _
                           ", 9);"


    err_return = db_insert_table_rowdata(file_name, sql_tbl_entry)
    If err_return = 0 Then
        Print "Row data was NOT entered into "; file_name
    Elseif err_return = 1 Then
        Print "Row data was entered into "; file_name
    Else  '' == -1
        Print "There was an unknown error."
    End If

    'free(bin_avatar_hexstr);
    'Erase bin_avatar_hexstr '' Clear the large file data from memory
    Print "==========================================="
    '/


    ''=========================================================================

	/'
    '' Retrive the previously inserted mixed data types.

    '' Get number of columns in a named table.
    ''int number_cols_retx = 0;  '' Previously defined.
    number_cols_retx = 0
    err_return = db_get_table_number_cols(file_name, db_table_mixed, number_cols_retx)
    If err_return = 0 Then
        Print "Could not retrieve table "; db_table_mixed; " column number from "; file_name
    Elseif err_return = 1 Then
        Print "Retrieved table "; db_table_mixed; " column number from "; file_name
    Else  '' == -1
        Print "There was an unknown error."
    End If

    Print "Table number of columns:"; number_cols_retx
    Print "==========================================="


    '' Retrive current table rows number.
    ''int number_rows_retx = 0;  '' Previously defined.
    number_rows_retx = 0
    err_return = db_get_table_number_rows(file_name, db_table_mixed, number_rows_retx)
    If err_return = 0 Then
        Print "Could not retrieve table "; db_table_mixed; " row number from "; file_name
    Elseif err_return = 1 Then
        Print "Retrieved table "; db_table_mixed; " row number from "; file_name
    Else  '' == -1
        Print "There was an unknown error."
    End If

    Print "Table number of rows:"; number_rows_retx
    Print "==========================================="



    '' Row and column count trackers.
    variant_array_rowlen = number_rows_retx  '' Always check the most recent number of rows in the table
    variant_array_collen = number_cols_retx + 1  '' The number of columns + 1 for rowid

    Print "variant_array_collen="; variant_array_collen

    ret_variant_field_elements = 0
    ret_variant_row_elements = 0  '' Track the number of used elements in the array

    '' Create 2D dynamic array using number rows and columns
    '' tagVARIANT structure is declared in example_sql3.h
    ''tagVARIANT **variant_array = NULL;
    '' Note! Array is oversized by 1,1
    'Dim variant_array() As tagVARIANT  '' Already declared above
    Redim variant_array(variant_array_rowlen, variant_array_collen) As tagVARIANT

    '' More universal query function for mixed data types.
    '' Note: The variant_array_collen, variant_array_rowlen are not currently
    '' used in this function call as they are calculated within the function.
    err_return = db_list_table_all_types(file_name, db_table_mixed, variant_array(), variant_array_collen, variant_array_rowlen, ret_variant_field_elements, ret_variant_row_elements)
    If err_return = 0 Then
        Print "Could not retrieve "; db_table_namex; " table data from "; file_name
    Elseif err_return = 1 Then
        Print "Retrieved table "; db_table_namex; " data from "; file_name
    Else  '' == -1
        Print "There was an unknown error."
    End If
    Print "==========================================="

    '' Print all mixed data types returned from the table. Each column is sorted
    '' to the relevant type by testing [][].type
    Print "Print mixed table data from variant_array."
    '' To DEBUG test against the original length of the array of VARIANT (bin_data_len = 16)
    ''int bdata_len2 = 0;
    ''int x = 0;
    '' Step through each row element from the array.
    For j = 0 To variant_array_rowlen -1 Step +1  '' or ret_variant_row_elements
        Print "====> Row element = "; j
        '' Step through each column element from the row.
        For i = 0 To variant_array_collen -1 Step +1  '' < ret_variant_field_elements

            Print "|Column element| = "; i
            '' Access each element of tag_VARIANT in variant_array[n][n]...
            '' Use this to select the correct usage of the returned data.
            Select Case variant_array(j,i).sql_type
            Case IS_NULL
                '' Do stuff for NULL pointer, using variant_array[n].value.vval
                '' This will denote an unused array element. It is possible to
                '' use this data structure were we test for null as an empty element
                '' or as and empty type
                Print "NULL= "; variant_array(j,i).value.vval
                Exit Select
            Case IS_INTEGER
                '' Do stuff for INTEGER, using variant_array[n].value.ival
                Print "INTEGER= "; variant_array(j,i).value.ival
                Exit Select
            Case IS_FLOAT
                '' Do stuff for REAL, using variant_array[n].value.rvar
                Print "REAL= "; variant_array(j,i).value.rval
                Exit Select
            Case IS_TEXT
                '' Do stuff for TEXT, using variant_array[n].value.tval
                Print "TEXT= "; variant_array(j,i).value.tval
                Exit Select
            Case IS_BLOB
                '' Do stuff for BLOB, using variant_array[n].value.bvar
                ''May need to enumerate the array bval(n)
                bdata_len2 = variant_array(j,i).value.bval.blen
                
                Print "byte len = "; bdata_len2
                If 16756 = bdata_len2 Then
                    Print "Returned bytes same length as original bytes."
                End If


                Print "BLOB= {";
                For x = 0 To bdata_len2 -1  '' From original file_tux size.
                    '' For C like format use "0x" instead of "&h". Hex(Byte, padding)
                    Print "&h"; Hex(variant_array(j,i).value.bval.bdata(x), 2); ",";  '' As hexidecimal.
                    'print variant_array(j,i).value.bval.bdata(x); ",";  '' As decimal.
                    'print Chr(variant_array(j,i).value.bval.bdata(x));  '' As character.
                Next x
                Print !"\b}"


                '' Compare original with return bytes
                '' Debug compare test.
                '' Requires the tux.jpg read file routine for the array bin_avatar()
                '' Used in the compare test.
                '' Remember to free the dynamic memory. (Erase bin_avatar)
                /'
                For x = 0 To bdata_len2 -1
                    If bin_avatar(x) <> variant_array(j,i).value.bval.bdata(x) Then
                        Print "BLOB Data does not match!"
                        Exit For
                    End If
                Next x
                '/
                

                '' Sql Hex entry. Use SQLite DB manager to confirm.
                '' 0000  ff d8 ff e2 02 1c 49 43 43 5f 50 52 4f 46 49 4c  ......ICC_PROFIL
                Exit Select
            Case Else
                '' Report an error, this shouldn't happen!
                Print "##default= "; variant_array(j,i).sql_type; "##"
                Exit Select
            End Select
            Print ""
        Next i
        Print ""
    Next j


    '' Free the dynamic memory!
    Erase variant_array
    Print "==========================================="
	'/

''============================================================================<<
Con_Pause()

    Return 0
End Function

/'
' Console Pause (GetKey version)
Function Con_Pause() As Integer
    Dim As Long dummy
    Print !"\nPress any key to continue..."
    dummy = Getkey
    Return 0
End Function
'/
