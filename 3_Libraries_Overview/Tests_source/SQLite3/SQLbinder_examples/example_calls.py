#-------------------------------------------------------------------------------
# Name:         example_calls.py (based upon basics_2.c, ozz_sql3.h)
# Purpose:      SQLite3 Basic examples tests.
#               Tests for convenience wrapper functions for SQLite version 3.
#
# Platform:     Win64, Ubuntu64
# Depends:      Python 3.9-64, SQLite v3.34.1 plus (dll/so), ctypes, ozz_sql3.py
#
# Author:       Axle
#
# Created:      15/05/2023
# Updated:      25/05/2023
# Copyright:    (c) Axle 2023
# Licence:      MIT-0 No Attribution
#-------------------------------------------------------------------------------
# Notes:
# Using the SQLite shared object (.dll, .so) directly as a Run-time library. The
# sqlite3.dll/.so must be in the system or application path.
#
# Python 3 built in SQLite3 library is a better/safer approach but uses a
# distinctly different API to the default C API which goes against the
# primary goal of exemplifying the same code routines in all 3 languages.
# As such I am using the Ctypes module for direct access to the shared libraries
# (.dll, .so) exposed C API. In essence python types are translated to C types
# for use by the C based shared object, and then C types are converted back to
# Python types when data is returned. This happens by default with most Python
# library modules but occurs in a more opeque manner in the background.
#
#-------------------------------------------------------------------------------
#------------------------------------------------------------------------------
# Notes: basics_2.c ozz_sql3.h
#
# The examples are part of a small library ozz_sql3.h and are designed to
# illustrate some of the basics of SQLite 3. They are not organized as and
# application and are designed to be rearranged, modified or used as a base
# from which to create a small application. Some of the example calls have been
# commented out to allow for the basic creation and reteival of some database
# tables. See ozz_sql3.h for a list of example functions and choose your
# own arrangement for the test below, or alternatively create a small app
# to accept user input and returns using the library functions.
# Modify the library functions as required for your own use.
#------------------------------------------------------------------------------
# TODO:
#
# Get the MAX length of Field Names?
# Get the length of a column, row entry?
# This is not a native API for sqlite, so a function would need to be created
# to analyze each individual row/col entry. For now I am just using arbitrary
# static limits of [128, [512], [2048] <- you can increase them if needed.
#
# Check array off by 1s.
#------------------------------------------------------------------------------
# Credits:
# https://stephenscotttucker.medium.com/interfacing-python-with-c-using-ctypes-classes-and-arrays-42534d562ce7
# https://realpython.com/pointers-in-python/
# https://dbader.org/blog/python-ctypes-tutorial
# https://github.com/trolldbois/ctypeslib
# https://www.scaler.com/topics/python-ctypes/
# https://solarianprogrammer.com/2019/07/18/python-using-c-cpp-libraries-ctypes/
#
# Special thanks for assistance with ctypes conversions, especialy the
# sqlite3_column_blob() void* to Python Byte string conversions.
# https://gist.github.com/michalc
#------------------------------------------------------------------------------

#import ctypes, sys, os
#import ozz_sql3
import example_sql3

## ====>> Error Constants
# Beware of name conflicts!
#from ozz_sql3_constants import  *


## This is not a plug an play application! Use the examples as a guide.
## Activate only the required examples to write and read from a database as
## required.
def main():
    pass

    return_code = 0  # return codes and error codes returned from functions.

    db_filename_ram = ":memory:"  # Using a temporary "In RAM" database.

    ver_buffer = []  # empty list (strings are imutable when passed to a function in python).
    err_return = 0;
    char_want_version = "3.34.1"
    int_want_version = 3034001


    """
    ## Basic hello world ===================================================>>

    # Ensure that sqlite3.dll is in the system path or in the working directory
    # of the project python script at run-time.

    # Get the path to the SQLite3 shared library file (dll/so).
    #f_library = ozz_sql3.get_libsql3_path()  # Get the path for sqlite.dll/so

    # Load the sqlite3 shared library. The returned ID from loading the sqlite
    # shared library must be passed to other functions.
    #id_lib_sql3  = ozz_sql3.load_libsql3(f_library_sql3)
    #print(type(id_lib_sql3))  # DEBUG (ctypes.c_void_p)

    # Get the OS handle of the CDLL object (Not used here). This is similar to
    # the python ID object but contains the real OS handle of the opened library.
    #hlib_sql3 = ozz_sql3.handle_libsql3(id_lib_sql3)
    #print(type(hlib_sql3))  # DEBUG


    # Get our SQLite version. Confirmation that sqlite 3 is installed as a
    # shared library and compiling/working correctly.

    # NOTE: I am using the C API interface directly and not as a query. SQLite
    # provides a limited number of helper MACROS that can be accessed directly
    # without opening a databse.

    # The return is already converted to Python UTF-8 string by the function.
    buffer1 = ozz_sql3.sqlite3_libversion(id_lib_sql3)
    print("1 SQLite Version:" + buffer1)
    print("===========================================")


    # Get version long
    p_db = ozz_sql3.p_sqlite3()  # Get the sqlite structure.

    p_stmt = ozz_sql3.p_sqlite3_stmt()  # Get the sqlite_stmt structure.

    # This ctypes prototype must be provided at the start of any python
    # function retreiving error messages.
    # const char *sqlite3_errmsg(sqlite3*);
    id_lib_sql3.sqlite3_errmsg.argtypes = [ctypes.c_void_p]
    id_lib_sql3.sqlite3_errmsg.restype = ctypes.c_char_p

    return_code = ozz_sql3.sqlite3_open(id_lib_sql3, db_filename_ram, p_db)
    # On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    # is returned.
    if return_code != SQLITE_OK:
        # This is error handling code for the sqlite3_prepare_v2 function call.
        print("Cannot open database: " + str(id_lib_sql3.sqlite3_errmsg(p_db).decode('utf-8')) + " | " + str(return_code), file=sys.stderr);  # DEBUG
        id_lib_sql3.sqlite3_close(p_db)
        return -1
    #return 1

    sql_query = "SELECT SQLITE_VERSION()"

    return_code = ozz_sql3.sqlite3_prepare_v2(id_lib_sql3, p_db, sql_query, -1, p_stmt, None)
    if return_code != SQLITE_OK:
        # This is error handling code for the sqlite3_prepare_v2 function call.
        print("Failed to prepare data: " + str(id_lib_sql3.sqlite3_errmsg(p_db).decode('utf-8')), file=sys.stderr);  # DEBUG
        id_lib_sql3.sqlite3_close(p_db)
        return -1

    # Call once for each cols/rows. enumerate col/rows for iCol.
    return_code = ozz_sql3.sqlite3_step(id_lib_sql3, p_stmt)
    if return_code != SQLITE_ROW:
        print("Step error: " + str(id_lib_sql3.sqlite3_errmsg(p_db).decode('utf-8')) + " | " + str(return_code), file=sys.stderr);  # DEBUG
        id_lib_sql3.sqlite3_close(p_db)
        return -1

    # returns data but no error code. Check for NULL returns.
    buffer2 = ozz_sql3.sqlite3_column_text(id_lib_sql3, p_stmt, 0)

    return_code = ozz_sql3.sqlite3_finalize(id_lib_sql3, p_db, p_stmt)
    if return_code != SQLITE_OK:
        # This is error handling code.
        print("Failed to finalize data: " + str(id_lib_sql3.sqlite3_errmsg(p_db).decode('utf-8')) + " | " + str(return_code), file=sys.stderr);  # DEBUG
        id_lib_sql3.sqlite3_close(p_db)
        return -1

    return_code = ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
    if return_code != SQLITE_OK:
        # This is error handling code. NOTE! As p_db is closed the error code may not be available!
        print("Failed to close database: " + str(id_lib_sql3.sqlite3_errmsg(p_db).decode('utf-8')) + " | " + str(return_code), file=sys.stderr);  # DEBUG
        return -1

    print("2 SQLite Version:" + buffer2)
    print("===========================================")

    ## Note that ctypes does not release the the SQLite shared object until the
    ## calling function is closed. In this case it keeps SQLite 3 library open
    ## for the durration of the application
    ## END Basic hello world ================================================<<
    """

    # SQLite does not impose file name extension naming restriction, but it
    # is sound practice to use a naming convention that is descriptive of the
    # database version such as .sqlite3 (or .db3 .sqlite3, .db, .db3, .s3db, .sl3)
    file_name = "Example_DB.db"  # The name of a database.db

    ## Short version check NO error returns.( return integer)
    return_val = example_sql3.sqlite3_get_version0()
    print("Version = " + str(return_val))
    if ( int_want_version == return_val):
        print("Correct version.")
    else:
        print("Incorrect version!")
    print("===========================================")


    ## Short version check NO error returns. (return string)
    ver_string = example_sql3.sqlite3_get_version1()
    print("Version = " + ver_string)
    if ( ver_string == char_want_version):
        print("Correct version.")
    else:
        print("Incorrect version!")
    print("===========================================")


    ## Long version check with error returns.
    # Only lists and dictionaries are mutable. To return a value by reference
    # (pseudo, as Python does not have varables or memory pointers, only objects)
    # we need to use a list. We can use the first element list[0] as a string return
    # in a similar way to a pointer by reference to the original buffer.
    # Note that I am returning an integer via "return" and a string via arguments.
    err_return = example_sql3.sqlite3_get_version2(ver_buffer);  # Note ver_buffer is a list.
    if (err_return == 0):
        print("Version error return = " + str(err_return))
    elif (err_return == 1):
        print("Version = " + ver_buffer[0])  # we only returned a single string at element[0].
        if ( ver_buffer[0] == char_want_version):
            print("Correct version.")
        else:
            print("Incorrect version!")
    else:  # err_return == -1
        print("An internal error occured.")
    print("===========================================")


    ## File exists? ( check if a file name exists) "Example_DB.db".
    err_return = example_sql3.file_exists(file_name)
    if err_return == 0:
        print("File " + file_name + " Not found.")
    else:  # ==1
        print("File " + file_name + " WAS found");
    print("===========================================")


    ## Test if "SQLite format 3" and if db file exists.
    err_return = example_sql3.db_file_exists(file_name)
    if err_return == 0:
        print(file_name + " Not found, or not SQLite3 database.")
    else:  # ==1
        print(file_name + " Is an SQLite3 database.")
    print("===========================================")



    ## Create SQLITE V3 db file as FileName.db
    # (or .db3 .sqlite3, .db, .db3, .s3db, .sl3)
    err_return = example_sql3.db_file_create(file_name)
    if err_return == 2:
        # Maybe add different error returns values.
        print(file_name + " already exists.")
    elif err_return == 1:
        print(file_name + " successfully created." )
    else:  # err_return==0
        print("An internal error occured.")
    print("===========================================")

    """
    ## Delete a named database file.
    # Remove block comments /* ... */ to use.
    err_return = example_sql3.db_file_delete(file_name)
    if err_return == -1:
        print(file_name + " was NOT deleted or not exists.")
    elif err_return == 1:
        print(file_name + " was successfully deleted.")
    else: # == 0
        print(" File " + file_name + " delete action terminated by user.")
    print("===========================================")
    """


    # TableName exists?
    db_table_name = "Hrs_worked_Tracker"
    err_return = example_sql3.db_table_exists(file_name, db_table_name)
    if err_return == 0:
        print("The Table " + db_table_name + " does NOT exist in " + file_name)
    elif err_return == 1:
        print("The Table " + db_table_name + " DOES exist in " + file_name)
    else:  # == -1
        print("There was an unknown error.")
    print("===========================================")


    ## !!!!!!! This has to be rechecked for correct return number_tables_ret !!!!
    ## Get the total number of tables in a named database file.
    # Python can't pass object names by reference as they are imutable, so we
    # we must use a list wich is mutable inside of a function. The List only
    # has a single integer element for our value.
    number_tables_ret = [0]  # Variable to hold returned number of tables.

    err_return = example_sql3.db_get_number_tables(file_name, number_tables_ret)
    if err_return == 0:
        print("Could NOT retrieve number of tables from " + file_name)
    elif err_return == 1:
        print("Retrieved number of tables from from " + file_name)
    else:  # == -1
        print("There was an unknown error.")
    print("Number of tables=" + str(number_tables_ret[0]))  # element[0]
    print("===========================================")


    ## Return an array of table names.

    # Unlike C we can easily create an empty 1D list of strings.
    db_tbl_names = []

    # Return an array of table names.
    err_return = example_sql3.db_get_tablenames(file_name, db_tbl_names)
    if err_return == 0:
        print("Could NOT retrieve table names from " + file_name)
    elif err_return == 1:
        print("Retrieved table names from " + file_name)
    else:  # == -1
        print("There was an unknown error.")

    print("Returned table names:")
    #for i in db_tbl_names:
    #    print(i)
    for i in range(int(number_tables_ret[0])):
        print(db_tbl_names[i])

    #db_tbl_names = None ?
    print("===========================================")



    ## create table sqlite3 query statement examples.

    # Note that SQLite will add a rowid increment automatically. If the following
    # is added as a field ID_Name INTEGER PRIMARY KEY then this column will
    # become an alias for rowid. You do not have to add an entry for the
    # INTEGER PRIMARY KEY column as SQLite3 will automatically add the value.
    # Note the C string line continuation character \
    # Note: \ will throw a compiler warning as you can accidentaly comment out
    # the next line using the continuation char.
    # We can also use = "My long string "
    #                   "on 2 lines";

     # Note! this will fail with current function that only handle TEXT
    ##char *db_table1 = "CREATE TABLE IF NOT EXISTS Hrs_worked_Tracker\
    ##                     (INDEX_ID INTEGER PRIMARY KEY\
    ##                     , Week TEXT\
    ##                     , Employee_ID TEXT\
    ##                     , Name TEXT\
    ##                     , Monday TEXT\
    ##                     , Tuesday TEXT\
    ##                     , Wednesday TEXT\
    ##                     , Thursday TEXT\
    ##                     , Friday TEXT);";



    # A full sqlite3 query (statement) we can also create a query template
    # here or withing the function and add the data through concatenation.
    # This is exemplified in later functions.
    # Note: If the table exist already no error is returned.
    db_table1 = "CREATE TABLE IF NOT EXISTS Hrs_worked_Tracker\
                         (Week TEXT\
                         , Employee_ID TEXT\
                         , Name TEXT\
                         , Monday TEXT\
                         , Tuesday TEXT\
                         , Wednesday TEXT\
                         , Thursday TEXT\
                         , Friday TEXT);"


    err_return = example_sql3.db_table_create(file_name, db_table1)
    if err_return == 0:
        print("Table could not be created in " + file_name)
    elif err_return == 1:
        print("Table was successfully created in " + file_name)
    else:  # == -1
        print("There was an unknown error.")
    print("===========================================")


    ## Recheck if Table EXISTS after creating the empty table.
    err_return = example_sql3.db_table_exists(file_name, db_table_name)
    if err_return == 0:
        print("The Table " + db_table_name + " does NOT exist in " + file_name)
    elif err_return == 1:
        print("The Table " + db_table_name + " DOES exist in " + file_name)
    else:  # == -1
        print("There was an unknown error.")
    print("===========================================")


    """
    ## delete table. See Create_empty_db. Drop Table.
    db_table2 = "DROP TABLE IF EXISTS Hrs_worked_Tracker;"

    err_return = example_sql3.db_table_delete(file_name, db_table2)
    if err_return == 0:
        print("Table NOT successfully deleted in " + file_name)
    elif err_return == 1:
        print("Table successfully deleted " + file_name)
    else:  # == -1
        print("There was an unknown error.")
    print("===========================================")
    # recheck if Table EXISTS after deleting the table.
    """


    ## add table entry/s (will add the data at the next available rowid)
    # Comment this out for later test if the entries become to large.
    # TableName must exist.
    # Single entry by Column Name:
    # "INSERT INTO table ( column2 ) VALUES( value2 );"
    # Full row:
    # "INSERT INTO table (column1,column2 ,..) VALUES( value1,	value2 ,...);"
    # Multiple rows:
    # "INSERT INTO table (column1,column2 ,..) \
    #              VALUES( value1,	value2 ,...), \
    #                    (value1,value2 ,...), \
    #                    ... \
    #                    (value1,value2 ,...);

    db_tbl_entry = "INSERT INTO Hrs_worked_Tracker \
                                           (Week\
                                           , Employee_ID\
                                           , Name\
                                           , Monday\
                                           , Tuesday\
                                           , Wednesday\
                                           , Thursday\
                                           , Friday) \
                                     VALUES(\"1\"\
                                           , \"34\"\
                                           , \"Joe Blogs\"\
                                           , \"7\"\
                                           , \"5\"\
                                           , \"8\"\
                                           , \"7\"\
                                           , \"9\");"


    err_return = example_sql3.db_insert_table_rowdata(file_name, db_tbl_entry)
    if err_return == 0:
        print("Row data was NOT entered into " + file_name)
    elif err_return == 1:
        print("Row data was entered into " + file_name)
    else:  # == -1
        print("There was an unknown error.")
    print("===========================================")



#============================================================================>>

    ## Get number of columns in a named table.
    number_cols_ret = []  # Variable for the returned number of columns.
    #int db_get_table_number_rows(char *db_file_name, char *db_table_name, int *number_rows);
    err_return = example_sql3.db_get_table_number_cols(file_name, db_table_name, number_cols_ret)
    if err_return == 0:
        print("Could not retrieve table " + db_table_name + " column number from " + file_name);
    elif err_return == 1:
        print("Retrieved table " + db_table_name + " column number from " + file_name)
    else:  # == -1
        print("There was an unknown error.")

    print("Table number of columns:" + str(number_cols_ret[0]))

    print("===========================================")



    ## Get number of rows in a named table.
    number_rows_ret = []  # Variable for the returned number of rows.
    #int db_get_table_number_rows(char *db_file_name, char *db_table_name, int *number_rows);
    err_return = example_sql3.db_get_table_number_rows(file_name, db_table_name, number_rows_ret)
    if err_return == 0:
        printf("Could not retrieve table " + db_table_name + " row number from " + file_name)
    elif err_return == 1:
        print("Retrieved table " + db_table_name + " row number from " + file_name)
    else:  # == -1
        printf("There was an unknown error.\n");

    print("Table number of rows:" +str(number_rows_ret[0]))

    print("===========================================")

    ## TODO
    # Get the MAX length of Field Names?
    # Get the length of a column, row entry?
    # This is not a native API for sqlite, so a function would need to be created
    # to analyze each individual row/col entry. For now I am just using arbitrary
    # static limits of [128, [512], [2048] <- you can increase them if needed.


    ## Return an array of column names (fields).
    # Unlike C Python has a VARIANT like object structure that can accept any type.
    db_tbl_data0 = []  ## List of string table names

    err_return = example_sql3.db_get_table_colnames(file_name, db_table_name, db_tbl_data0)
    if err_return == 0:
        printf("Could not retrieve table " + db_table_name + " column names from " + file_name)
    elif err_return == 1:
        print("Retrieved table " + db_table_name + " column names from " + file_name)
    else:  # == -1
        print("There was an unknown error.")

    print("Array length = " + str(number_cols_ret))
    print("Table column names:")

    #for i in db_tbl_data0:
    #    print(i)
    for i in range(int(number_cols_ret[0])):
        print(str(i) + ":" + db_tbl_data0[i])

    #db_tbl_data0 = None  # Clear data from the object. ( I am reusing this in a later example)
    print("===========================================")


#==============================================================================

    ## Retrieve all table entry/s to array[][]

    # Unlike C Python has a VARIANT like object structure that can accept any type.
    # Data is stored as a list(rows) of csv strings(all columns from the row).
    db_tbl_data1 = []

    #int number_columns = 8;
    # The number of columns can also se found internally using
    # sqlite3_column_count(statement/stmt)
    number_columns = int(number_cols_ret[0])  # From db_get_table_number_cols()

    err_return = example_sql3.db_list_table_rows_data(file_name, db_table_name, db_tbl_data1, number_columns)
    if err_return == 0:
        print("Could not retrieve table " + db_table_name + " data from " + file_name)
    elif err_return == 1:
        print("Retrieved table " + db_table_name + " data from " + file_name)
    else:  # == -1
        print("There was an unknown error.")

    # Print the table data. Column 0 is the row_id.
    print("Table data:")
    print("Array length = " + str(number_rows_ret[0]))
    #for i in db_tbl_data1:
    #    print(i)
    for i in range(int(number_rows_ret[0])):
        print(db_tbl_data1[i])


    db_tbl_data1 = None  # Clear data from the list.
    print("===========================================")



    """
    # delete table entry/s by search word (dangerous!)
    # NOTE!!! This needs to be revised with more narrow focus and safeguards !!!
    # The following will delete ->ALL<- rows containing "1" and "Joe Blogs".
    # It is appropriate to check the entry row number index_id before deleting.
    db_row_entry = "DELETE FROM Hrs_worked_Tracker\
                    WHERE Week = \"1\" AND Name = \"Joe Blogs\";"

    err_return = example_sql3.db_delete_table_rowdata(file_name, db_row_entry)
    if err_return == 0:
        print("Row data NOT deleted from " + file_name)
    elif err_return == 1:
        print("Row data deleted from " + file_name)
    else:  # == -1
        print("There was an unknown error.")
    print("===========================================")
    """



#==============================================================================

    ## Get number of rows in a named table.
    number_rows_ret = []  # Variable for the returned number of rows.
    #int db_get_table_number_rows(char *db_file_name, char *db_table_name, int *number_rows);
    err_return = example_sql3.db_get_table_number_rows(file_name, db_table_name, number_rows_ret)
    if err_return == 0:
        printf("Could not retrieve table " + db_table_name + " row number from " + file_name)
    elif err_return == 1:
        print("Retrieved table " + db_table_name + " row number from " + file_name)
    else:  # == -1
        printf("There was an unknown error.\n");

    print("Table number of rows:" +str(number_rows_ret[0]))

    print("===========================================")


    ## TODO
    # Tset get last rowid. Only works after a db table was opened and not
    # yet closed.
    #int last_id = sqlite3_last_insert_rowid(db);
    #printf("The last Id of the inserted row is %d\n", last_id);


    #========================================================================>>

    """
    ## delete row by "rowid". This does not question or ask for confirmation!
    sql_rowid1 = 2
    print("Numer of rows=" + str(number_rows_ret[0]))
    print("Delete row_id=" + str(sql_rowid1))
    #char *db_row_entry = "DELETE FROM Hrs_worked_Tracker WHERE rowid = 2;";

    # If the the row number is greater than the rows available stop.
    if int(sql_rowid1) <= int(number_rows_ret[0]):
        #int db_delete_table_rowdata_rowid(char *db_file_name, char *db_table_name, int sql_rowid);
        err_return = example_sql3.db_delete_table_rowdata_rowid(file_name, db_table_name, sql_rowid1)
        if err_return == 0:
            print("Rowid data NOT deleted from " + file_name)
        elif err_return == 1:
            print("Rowid data deleted from " + file_name)
        else:  # == -1
            print("There was an unknown error.")

    else:
        print("rowid does not exist!")

    print("===========================================")
    """

    """
    ## update/replace by rowid. This will replace/overwrite existing row data.
    # This will replace the existing rowid or INDEX_ID with neww data for each
    # column name assigned.
    # Alternative rowid, INDEX_ID
    # Note using INT/INTEGER with current function will fail!

    db_table_name2 = "Hrs_worked_Tracker"
    sql_rowid2 = 2
    # This can be obtained from db_get_table_colnames(). use concatenation to create the following string.
    db_field_names = "Week, Employee_ID, Name, Monday, Tuesday, Wednesday, Thursday, Friday"

    # Modify the the values to replace the old data.
    db_field_values = "\"2\", \"36\", \"Jill Blogs\", \"9\", \"5\", \"4\", \"7\", \"6\""

    # Original entery.
    #char *db_tbl_entry = "REPLACE INTO Hrs_worked_Tracker \
    #                               (rowid\
    #                               , Week\
    #                               , Employee_ID\
    #                               , Name\
    #                               , Monday\
    #                               , Tuesday\
    #                               , Wednesday\
    #                               , Thursday\
    #                               , Friday) \
    #                         VALUES( 3\
    #                                , \"2\"\
    #                               , \"36\"\
    #                               , \"Jill Blogs\"\
    #                               , \"9\"\
    #                               , \"5\"\
    #                               , \"5\"\
    #                               , \"7\"\
    #                               , \"8\");";


    # If the the row number is greater than the rows available stop.
    if number_rows_ret[0] >= sql_rowid2:
        err_return = example_sql3.db_replace_table_rowdata_rowid(file_name, db_table_name2, sql_rowid2, db_field_names, db_field_values)
        if err_return == 0:
            print("Rowid data was NOT replaced into " + file_name)
        elif err_return == 1:
            print("Rowid data was replaced into " + file_name)
        else:  # == -1
            print("There was an unknown error.")

    else:
        print("rowid does not exist!")
    print("===========================================")
    """


    ## Get the current number of rows in the TableName.
    number_rows = 0
    number_rows_ret = []  # Variable for the returned number of rows.
    err_return = example_sql3.db_get_table_number_rows(file_name, db_table_name, number_rows_ret)
    if err_return == 0:
        print("Could not retrieve table " + db_table_name + " row number from " + file_name)
    elif err_return == 1:
        print("Retrieved table " + db_table_name + " row number from " + file_name)
    else:  # == -1
        print("There was an unknown error.\n");

    print("Table number of rows:" +str(number_rows_ret[0]))

    print("===========================================")
    number_rows = int(number_rows_ret[0])  # Used for db_insert_table_rowdata_rowid()


    """
    ## insert by rowid?
    # If rowid doesn't exist does not write.
    # Copies each row down 1 at a time to create space and new row at rowid in the table.
    # The new row is placed into the rowid using REPLACE INTO.
    # the last row is INSERT INTO a new rowid at the end of the table
    # Copy notes from function to here!!!
    # Test error handling!!!
    # Consider remove '\r\, '\n' etc. from strings.

    db_table_name3 = "Hrs_worked_Tracker"
    sql_rowid3 = 3
    # This can be obtained from db_get_table_colnames(). use concatenation to create the following string.
    db_field_names3 = "Week, Employee_ID, Name, Monday, Tuesday, Wednesday, Thursday, Friday"
    db_field_values3 = "\"2\", \"36\", \"Bill Knight\", \"2\", \"3\", \"4\", \"5\", \"6\""

    # If the the row number is greater than the rows available stop.
    if number_rows >= sql_rowid3:
        #int db_insert_table_rowdata_rowid(char *db_file_name, char *db_table_name, int sql_rowid, char *db_field_names, char *db_field_values);
        err_return = example_sql3.db_insert_table_rowdata_rowid(file_name, db_table_name3, sql_rowid3, db_field_names3, db_field_values3, number_columns, number_rows)
        if err_return == 0:
            print("Row data was NOT inserted into " + file_name)
        elif err_return == 1:
            print("Row data was inserted into " + file_name)
        else:  # == -1
            print("There was an unknown error.")

    else:
        print("rowid does not exist!")
    print("===========================================")
    """


    ##Read row from rowid. returned as csv string.

    db_table_name4 = "Hrs_worked_Tracker"
    sql_rowid4 = 4
    # This can be obtained from db_get_table_colnames(). use concatenation to create the following string.
    #char *db_field_names3 = "Week, Employee_ID, Name, Monday, Tuesday, Wednesday, Thursday, Friday";
    #char *db_field_values3 = "\"2\", \"36\", \"Bill Knight\", \"2\", \"3\", \"4\", \"5\", \"6\"";
    db_tbl_rowid_data = []  # Only a list is mutable. The row string is placed in list element[0]

    # If the the row number is greater than the rows available stop.
    if number_rows >= sql_rowid4:
        err_return = example_sql3.db_read_table_rowdata_rowid(file_name, db_table_name4, sql_rowid4, db_tbl_rowid_data, number_columns)
        if err_return == 0:
            print("Rowid data was NOT read from " + file_name)
        elif err_return == 1:
            print("Rowid data was read from " + file_name)
        else:  # == -1
            print("There was an unknown error.")

        # Print the table data.
        print("Table rowid data:")
        print(db_tbl_rowid_data[0])
        print("number of columns = " + str(number_columns))

        csv_row_length = len(db_tbl_rowid_data[0])
        print("csv_row_length characters=" + str(csv_row_length))

        # Split the string into a list of column values at ','. Note that we have
        # recieved a single csv string in list element[0]. We are then using
        # string slicing to split the string into a new list at the delimiter ','.
        list_buffer = db_tbl_rowid_data[0].split(',')

        for i in range(len(list_buffer)):
            print("col[" + str(i) + "]=" + list_buffer[i])

    else:
        print("rowid does not exist!")

    print("===========================================")

    #========================================================================<<
    #========================================================================>>

    ## Search TableName by column name (field) and search word.

    # Unlike C Python has a VARIANT like object structure that can accept any type.
    db_tbl_data3 = []  ## number_rows_ret not required in python.

    # Get the number of search rows found.
    # Although Python does not need this with list I will use the same method here.
    ret_array_length1 = []  # We cannot get the length of array elements in C
    # so we need to return the number of array positions that have been
    # populated from the search. Alternatively we can enumerate to full array
    # length from number_rows_ret and filter out empty '\0' elements.

    #char *db_row_search = "SELECT FROM Hrs_worked_Tracker\
    #                  WHERE ANY = \"Joe Blogs\";";

    # This needs to be converted to full table search. requires EXACT match.
    # #### The inverted commas are emitted in many examples. More research !! ###
    field_name = "\"Name\""  # " 'value' " is not acceptable from C.
    db_search_string1 = "\"Joe Blogs\""  # " \"value\" " is acceptable.
    #char *db_search_string = "\"Blogs\"";  # research wild cards :)
    #char temp_buffer[128] = {'\0'};
    #for( i = 0; i < number_columns; i++)
    #     {
    #     strcpy(temp_buffer,db_tbl_data0[i]);
    #     }

    # Note number_columns retrieved earlier.

    #int number_columns2 = 8;
    # number_columns will be +1 because we are also retrieving the row_id number.
    err_return = example_sql3.db_search_table_rowdata_byfield(file_name, db_table_name, db_tbl_data3, field_name, db_search_string1, number_columns, ret_array_length1)
    if err_return == 0:
        print("Could not retrieve table " + db_table_name + " search data from " + file_name)
    elif err_return == 1:
        print("Retrieved table " + db_table_name + " search data from " + file_name)
    else:  # == -1
        print("There was an unknown error.")

    # Print returned search result (full array).
    print("Table column search data:")

    # Print returned search result (actual number search rows found).
    print("Array length = " + str(ret_array_length1[0]))
    for i in range(int(ret_array_length1[0])):
    #for( i = 0; i < ret_array_length1; i++)
        print(db_tbl_data3[i])


    db_tbl_data3 = None  # free list data if no longer required.
    print("===========================================")



    ## Search all columns (fields) for search string in table name. Must be EXACT
    ## search word match. Returns array in the order of rows found without duplicates.

    # Unlike C Python has a VARIANT like object structure that can accept any type.
    db_tbl_data4 = []

    # Get the number of search result rows found.
    ret_array_length2 = []  # We cannot get the length of array elements in C
    # so we need to return the number of array positions that have been
    # populated from the search. Alternatively we can enumerate to full array
    # length from number_rows_ret and filter out empty '\0' elements.

    #char *db_search_string2 = "\"Joe Blogs\"";
    #"\"2\", \"36\", \"Jill Blogs\", \"9\", \"5\", \"4\", \"7\", \"6\""  # DEBUG
    db_search_string2 = "\"9\""  # 6, 9

    err_return = example_sql3.db_search_table_rowdata_allfields(file_name, db_table_name, db_tbl_data4, db_tbl_data0, db_search_string2, number_columns, ret_array_length2)
    if err_return == 0:
        printf("Could not retrieve table " + db_table_name + " search data from " + file_name)
    elif err_return == 1:
        print("Retrieved table " + db_table_name + " search data from " + file_name)
    else:  # == -1
        print("There was an unknown error.")

    # Print returned search result.
    print("Table ALL column search data:")
    # Print returned search result (actuall number search rows found).
    print("Array length = " + str(ret_array_length2[0]))
    for i in range(int(ret_array_length2[0])):
    #for( i = 0; i < ret_array_length2; i++)
        print( db_tbl_data4[i])

    # deallocate memory
    db_tbl_data4 = None
    db_tbl_data0 = None
    print("===========================================")



#==============================================================================
# START Multiple types examples.

    ## Basic explainer; Maniplulating byte data.
    ## Not part of the data base query examples!
    """
    ## Original C byte array
    #unsigned char bin_data[16] = {0xff, 0xd8, 0xff, 0xe2, 0x02, 0x1c, 0x49, 0x43, 0x43, 0x5f, 0x50, 0x52, 0x4f, 0x46, 0x49, 0x4c};
    ## Python bytes list as hexidecimal or (integers)
    ## [255,216,255,226,2,28,73,67,67,95,80,82,79,70,73,76] Native values for a byte.
    ## Byte list
    bin_data_list = [0xff, 0xd8, 0xff, 0xe2, 0x02, 0x1c, 0x49, 0x43, 0x43, 0x5f, 0x50, 0x52, 0x4f, 0x46, 0x49, 0x4c]
    print(bin_data_list)  # as list
    for i in range(len(bin_data_list)):
        print(bin_data_list[i], end=',')  # As byte decimal (native).
    print()
    for i in range(len(bin_data_list)):
        print(hex(bin_data_list[i]), end=',')  # As Hexidecimal.
    print("\n=============")

    ## Byte string
    bin_data_bstring = b"\xff\xd8\xff\xe2\x02\x1c\x49\x43\x43\x5f\x50\x52\x4f\x46\x49\x4c"
    for i in range(len(bin_data_bstring)):
        print(bin_data_bstring[i], end=',')  # As byte decimal (native)
    print()
    for i in range(len(bin_data_bstring)):
        print(hex(bin_data_bstring[i]), end=',')  # As Hexidecimal
    print("\n=============")


    bytes_string2 = bytes(bin_data_list)  # Convert byte list to byte string.
    print(bytes_string2)
    for i in range(len(bytes_string2)):
        print(bytes_string2[i], end=',')  # As byte decimal (native).
    print()
    for i in range(len(bytes_string2)):
        print(hex(bytes_string2[i]), end=',')  # As Hexidecimal.
    print("\n=============")

    bytes_list2 = list(bin_data_bstring)  # Convert byte string to byte list.
    for i in range(len(bytes_list2)):
        print(bytes_list2[i], end=',')  # As byte decimal (native).
    print()
    for i in range(len(bytes_list2)):
        print(hex(bytes_list2[i]), end=',')  # As Hexidecimal.
    print("\n=============")

    print(bytes_string2.hex())
    # print(hex(bytes_string2))  ## Fails
    # print(bin_data_list.hex())  ## Fails
    print(bin_data_bstring.hex())
    """

#==============================================================================
    # Define our type flags. Renamed to avoid name conflicts with sqlite built
    # in type constants.
    # Note:NUMERIC can hold any data type as an integer but is an affinity
    # rather than a type. In most instances NUMERIC will convert to int or float.
    # I have not used NUMERIC.
    IS_NULL = 5
    IS_INTEGER = 1
    IS_FLOAT = 2  ## REAL
    IS_TEXT = 3
    IS_BLOB = 4

    # Blob table name.
    db_table_namex1 = "DATA_Blobs"  # Table with single column BLOB


    ## Creae a table and field for binary BLOBS
    db_table5 = "CREATE TABLE IF NOT EXISTS DATA_Blobs\
                         (Binary_data BLOB);"

    err_return = example_sql3.db_table_create(file_name, db_table5)
    if err_return == 0:
        print("Table " + db_table_namex1 + " could not be created " + file_name)
    elif err_return == 1:
        print("Table " + db_table_namex1 + " was successfully created in " + file_name)
    else:
        printf("There was an unknown error " + db_table_namex1 )
    print("===========================================")




    ## Insert some binary (BLOB) test data.

    # bin_data...
    #bin_data_blist = [0xff, 0xd8, 0xff, 0xe2, 0x02, 0x1c, 0x49, 0x43, 0x43, 0x5f, 0x50, 0x52, 0x4f, 0x46, 0x49, 0x4c]
    ## list must be converted to ( byte_string = bytes(byte_list) ):
    # bin_data_bstring = bytes(bin_data_blist)

    bin_data_bstring = b"\xff\xd8\xff\xe2\x02\x1c\x49\x43\x43\x5f\x50\x52\x4f\x46\x49\x4c"

    # SEE: isinstance(), type() to test for type and conversion.
    #True/False = isinstance("Hello World",str)
    #if type("Hello World") == str:
    #    True/False

    # we can get the length from the byte string or provide it manually.
    bin_data_len = len(bin_data_bstring)
    print(bin_data_len)
    #bin_data_len = 16

    # A variation of the INSERT statement using sqlite3_bind_*().
    # We can send data/values separately replacing the values into '?' using
    # various sqlite3_bind*() functions. It will be necessary to know the
    # data type and infinity before hand and use the select case as I have
    # in the examples for reading the data.
    # "INSERT INTO DATA_Blobs (Binary_data) VALUES(\?);" <- ? not escaped in Python.
    db_tbl_entry5 = "INSERT INTO DATA_Blobs (Binary_data) VALUES(?);"

    # NOTE! bin_data must be suplied as a Python 3 byte'string' aka b'\x0b\xff ...'
    # If the data is a byte list it must be converted ( byte_string = bytes(byte_list) )
    err_return = example_sql3.db_insert_table_rowdata_bin(file_name, db_tbl_entry5, bin_data_bstring, bin_data_len)
    if err_return == 0:
        print("Row data was NOT entered into " + file_name)
    elif err_return == 1:
        print("Row data was entered into " + file_name)
    else:  # == -1
        print("There was an unknown error.")
    print("===========================================")


#==============================================================================

    """
    ## Get number of columns in a named table.
    number_cols_retx = []

    err_return = example_sql3.db_get_table_number_cols(file_name, db_table_namex1, number_cols_retx)
    if err_return == 0:
        printf("Could not retrieve table " + db_table_namex1 + " column number from " + file_name)
    elif err_return == 1:
        print("Retrieved table " + db_table_namex1 + " column number from " + file_name)
    else:  # == -1
        print("There was an unknown error.")

    print("\nTable number of columns:" + str(number_cols_retx[0]))
    print("===========================================")


    # Get number of rows in a named table.
    number_rows_retx = []

    err_return = example_sql3.db_get_table_number_rows(file_name, db_table_namex1, number_rows_retx);
    if err_return == 0:
        print("Could not retrieve table " + db_table_namex1 + " row number from " + file_name)
    elif err_return == 1:
        print("Retrieved table " + db_table_namex1 + " row number from " + file_name)
    else:  # == -1
        print("There was an unknown error.")

    print("\nTable number of rows:" + str(number_rows_retx[0]))
    print("===========================================")



    ## More universal query function for mixed data types.
    # As you will see this is more complex than using a single data type in
    # the table columns. Personally for small database requirements I store
    # everything as TEXT and keep a track of the column affinity (data type)
    # in my calling application and convert values to other types as required.
    # The only exception to this is binary data (BLOBs) which would need to be
    # converted to TEXT using a Base64 encoder. I would not attempt to store
    # large amounts of binary data in this way. If you do have to store large
    # binary data sets such as images etc then you will need to make use of the
    # correct types and SQLite affinities as shown in this function.
    # This will offer a sound example to build more complex database queries.
    # See the modified version db_insert_table_rowdata_bin() for hints as how
    # to insert mixed data types based upon the select case examples and the
    # VARIANT structure examples.


    # NOTE: SQLite3 does have it's own internal typeless data structure Mem.
    # typedef struct Mem Mem;
    # It is an extremely complex data structure that includes many other data
    # structures defined in the sqlite source. Also it is predomenently used
    # with the sqlite3_value/_* set of API functions.
    # typedef struct sqlite3_value sqlite3_value;
    # It is more convenient to create our own tag struct, union or linked list
    # for the following example.

    # We will have to intitialize/assign the dynamic array in a for loop.
    # Get these values from ret_number_columns, ret_number_rows.
    # Elements should be the same size as variant_array_len0 unless we
    # purposefully create an array larger then the number of rows in the table.
    variant_array_rowlen = number_rows_retx[0]  # Always check the most recent number of rows in the table.
    variant_array_collen = number_cols_retx[0] + 1  # The number of columns + 1 for rowid.

    # Track the returned number of used elements in the array.
    ret_variant_field_elements = []
    ret_variant_row_elements = []
    # Note: it is possible in a more complex structure to track the array size
    # and elements used within the structure.


    # Unlike C Python already uses "Objects" that are similar to the tagVARIANT
    # type so we don't have to recreate the C structure and union. I will use
    # a list of dict so that we can easily identify the type contained in the
    # Python object. We can identify basic python types useing type(object[n])
    # but these do not always align to the SQLite type afinity required to
    # correctly identify the data, so I will use the SQLite data types.

    # Basic data container 2D list of [row][col][dict]:
    # data_list = [[{'typeof': None, 'data': None}], [{'typeof': None, 'data': None}], ...]
    # data_list[1][2]['typeof'] >> None
    # data_list[1][2]['data'] >> None
    # data_list[1][2]['typeof'] = 'TEXT'
    # data_list[1][2]['data'] = "Hello world"
    # print(data_list[1][2]['typeof']) >> TEXT
    # print(str(data_list[1][2]['data']) >> Hello world

    # Added BLOB byte 'length' as legacy but not really required.

    # We have to declare and initialise the empty list of dict here and send
    # it to the function. I have shown the prototype for this using col length
    # and row length.

    variant_array = []  # Declare an empty list.
    for i in range(variant_array_rowlen):
        variant_array.insert(i, [])  # Initialise the rows As list
        for j in range(variant_array_collen):
            # Initialise each column with a dict. [row][col][dict]
            variant_array[i].insert(j, {'typeof': None, 'data': None, 'length': 0})

    ## More universal query function for mixed data types.
    # Note: The variant_array_collen, variant_array_rowlen are not currently
    # used in this function call as they are calculated within the function.
    err_return = example_sql3.db_list_table_all_types(file_name, db_table_namex1, variant_array, variant_array_collen, variant_array_rowlen, ret_variant_field_elements, ret_variant_row_elements)
    if err_return == 0:
        print("Could not retrieve " + db_table_namex1 +" table data from " + file_name)
    elif err_return == 1:
        print("Retrieved table " + db_table_namex1 +" data from " + file_name)
    else:  # == -1
        print("There was an unknown error.")

    # Debug test the size of returned number of row elements against our array[n]
    if variant_array_rowlen < ret_variant_row_elements[0]:
        print("Error! Dynamic array is too small for number of rows.")
    elif variant_array_rowlen > ret_variant_row_elements[0]:
        print("Good! Dynamic array is lager than number of rows.")
    elif variant_array_rowlen == ret_variant_row_elements[0]:
        print("Good! Dynamic array same size as number of rows.")
    else:
        print("Unknown error!")  # should never occur



    print("\nPrint mixed table data from variant_array.")
    #bdata_len2 = 0 # The original length of the array of VARIANT  (bin_data_len = 16)
    # Step through each row from the list.
    for j in range(variant_array_rowlen):  # or ret_variant_row_elements
        print("\nRow element = " + str(j))

        # Step through each list element[][i] containing a dict. In this case
        # each element[][i] contains 3 dict items.
        for i in range(variant_array_collen):  # < ret_variant_field_elements
            print("Column element = " + str(i))
            # The first dict itme is the data type. Use this to select the correct
            # usage of the returned data.
            caseis = variant_array[j][i]['typeof']
            ## C like match case is introduced in Python 3.10 onwards.
            #match variant_array[j][i]['typeof']:
            #switch (variant_array[j][i].type)  # C equivalent
            if caseis == IS_NULL:
                # Do stuff for NULL pointer (or None).
                # This will denote an unused array element. It is possible to
                # use this data structure were we test for null as an empty element
                # or as and empty type
                print("IS_NULL")
                print(variant_array[j][i]['data'])
            elif caseis == IS_INTEGER:
                # Do stuff for INTEGER.
                print("IS_INTEGER")
                print(str(variant_array[j][i]['data']))
            elif caseis == IS_FLOAT:  ## REAL
                # Do stuff for REAL.
                print("IS_FLOAT (REAL)")  ## REAL
                print(str(variant_array[j][i]['data']))
            elif caseis == IS_TEXT:
                # Do stuff for TEXT.
                print("IS_TEXT")
                print(str(variant_array[j][i]['data']))
            elif caseis == IS_BLOB:
                # Do stuff for BLOB.
                # NOTE: print-ing a byte string will give undefined results
                # due to the effect of control characters.
                print("IS_BLOB")
                byte_string = variant_array[j][i]['data']
                print("bytestring = ", end='')  # Can't concatenate bytes b''
                print(byte_string)  # As byte string.
                # Python can calculate the length of a byte string.
                print("byte_string len = ", len(byte_string))
                # Legacy length of byte array...
                print("array len = " + str(variant_array[j][i]['length']))

                # Debug test.
                if 16 == len(byte_string):
                    print("Returned bytes same length as original bytes.")

                print("BLOB = {", end='')  # Can't concatenate bytes b''
                for x in range(variant_array[j][i]['length']):
                    print(byte_string[x], end=',')  # As byte decimal (native)
                print("\b}")  # Note: Backspace does not work in all terminals.
                print("BLOB = {", end='')
                for x in range(variant_array[j][i]['length']):
                    print(hex(byte_string[x]), end=',')  # As Hexidecimal
                print("\b}")  # Note: Backspace does not work in all terminals.

                # Debug test
                if bin_data_bstring == byte_string:
                    print("BLOB data is the same as the original.")

                # Sql Hex entry. Use SQLite DB manager to confirm.
                #0000  ff d8 ff e2 02 1c 49 43 43 5f 50 52 4f 46 49 4c  ......ICC_PROFIL
            else:
            #_case:  # default:
                # Report an error, this shouldn't happen!
                print("IS_ERR")
                printf("##default=" + str(variant_array[j][i]['typeof']) + "##")
                #break;
            ## END if [match case (switch case)]
            print("")  # Line break
            ## END for i
        print("")  # Line break
        ## END for j


    variant_array = None  # Crear the list data.
    print("===========================================")
    """

#==============================================================================
    """
    ## Test our original "Hrs_worked_Tracker" TEXT table with retreive all data types.
    ## This if from the first TEXT only table examples.

    ## Retrive current table column number.

    db_table_namex2 = db_table_name  # "Hrs_worked_Tracker"

    # Get number of columns in a named table.
    #number_cols_retx = []

    err_return = example_sql3.db_get_table_number_cols(file_name, db_table_namex2, number_cols_retx)
    if err_return == 0:
        printf("Could not retrieve table " + db_table_namex2 + " column number from " + file_name)
    elif err_return == 1:
        print("Retrieved table " + db_table_namex2 + " column number from " + file_name)
    else:  # == -1
        print("There was an unknown error.")

    print("\nTable number of columns:" + str(number_cols_retx[0]))
    print("===========================================")


    # Retrive current table rows number.
    #number_rows_retx = []

    err_return = example_sql3.db_get_table_number_rows(file_name, db_table_namex2, number_rows_retx);
    if err_return == 0:
        print("Could not retrieve table " + db_table_namex2 + " row number from " + file_name)
    elif err_return == 1:
        print("Retrieved table " + db_table_namex2 + " row number from " + file_name)
    else:  # == -1
        print("There was an unknown error.")

    print("\nTable number of rows:" + str(number_rows_retx[0]))
    print("===========================================")



    ## retreieve all table data ("DATA_Blobs")
    variant_array_rowlen = number_rows_retx[0]  # Always check the most recent number of rows in the table
    variant_array_collen = number_cols_retx[0] + 1  # The number of columns + 1 for rowid

    ret_variant_field_elements = []
    ret_variant_row_elements = []  # Track the number of used elements in the array

    variant_array = []  # Declare an empty list. Note was previously cleared to None.
    for i in range(variant_array_rowlen):
        variant_array.insert(i, [])  # Initialise the rows As list
        for j in range(variant_array_collen):
            # Initialise each column with a dict. [row][col][dict]
            variant_array[i].insert(j, {'typeof': None, 'data': None, 'length': 0})

    err_return = example_sql3.db_list_table_all_types(file_name, db_table_namex2, variant_array, variant_array_collen, variant_array_rowlen, ret_variant_field_elements, ret_variant_row_elements)
    if err_return == 0:
        print("Could not retrieve " + db_table_namex2 +" table data from " + file_name)
    elif err_return == 1:
        print("Retrieved table " + db_table_namex2 +" data from " + file_name)
    else:  # == -1
        print("There was an unknown error.")

    # Debug test the size of returned number of row elements against our array[n]
    if variant_array_rowlen < ret_variant_row_elements[0]:
        print("Error! Dynamic array is too small for number of rows.")
    elif variant_array_rowlen > ret_variant_row_elements[0]:
        print("Good! Dynamic array is lager than number of rows.")
    elif variant_array_rowlen == ret_variant_row_elements[0]:
        print("Good! Dynamic array same size as number of rows.")
    else:
        print("Unknown error!")  # should never occur

    print("\nPrint mixed table data from variant_array.")
    for j in range(variant_array_rowlen):  # or ret_variant_row_elements
        print("\nRow element = " + str(j))
        for i in range(variant_array_collen):  # < ret_variant_field_elements
            print("Column element = " + str(i))
            # access each element of tag_VARIANT in variant_array[n]
            caseis = variant_array[j][i]['typeof']
            ## C like match case is introduced in Python 3.10 onwards.
            #match variant_array[j][i]['typeof']:
            #switch (variant_array[j][i].type)
            if caseis == IS_NULL:
                # Do stuff for NULL pointer, using variant_array[n].value.vval
                # This will denote an unused array element. It is possible to
                # use this data structure were we test for null as an empty element
                # or as and empty type
                print("IS_NULL")
                print(variant_array[j][i]['data'])
            elif caseis == IS_INTEGER:
                # Do stuff for INTEGER, using variant_array[j][i]['data']
                print("IS_INTEGER")
                print(str(variant_array[j][i]['data']))
            elif caseis == IS_FLOAT:  ## REAL
                # Do stuff for REAL, using variant_array[j][i]['data']
                print("IS_FLOAT (REAL)")  ## REAL
                print(str(variant_array[j][i]['data']))
            elif caseis == IS_TEXT:
                # Do stuff for TEXT, using variant_array[j][i]['data']
                print("IS_TEXT")
                print(str(variant_array[j][i]['data']))
            elif caseis == IS_BLOB:
                # Do stuff for BLOB, using variant_array[j][i]['data']
                # NOTE: print-ing a byte string will give undefined results
                # due to the effect of control characters (Dec 0 to 31) as well
                # as NON 7-bit ASCII print characters (Dec 128 to 255)
                print("IS_BLOB")
                byte_string = variant_array[j][i]['data']
                print("bytestring = ", end='')  # Can't concatenate bytes b''
                print(byte_string)  # As byte string.
                # Python can calculate the length of a byte string.
                print("byte_string len = ", len(byte_string))
                # Legacy length of byte array...
                print("array len = " + str(variant_array[j][i]['length']))

                # Debug test.
                # (bin_data_len = 16) # The original length of the array of VARIANT.
                if 16 == len(byte_string):
                    print("Returned bytes same length as original bytes.")

                print("BLOB = {", end='')  # Can't concatenate bytes b''
                for x in range(variant_array[j][i]['length']):
                    print(byte_string[x], end=',')  # As byte decimal (native)
                print("\b}")  # Note: Backspace does not work in all terminals.
                print("BLOB = {", end='')
                for x in range(variant_array[j][i]['length']):
                    print(hex(byte_string[x]), end=',')  # As Hexidecimal Unformated.
                print("\b}")  # Note: Backspace does not work in all terminals.

                # Debug test
                if bin_data_bstring == byte_string:
                    print("BLOB data is the same as the original.")

                #0000  ff d8 ff e2 02 1c 49 43 43 5f 50 52 4f 46 49 4c  ......ICC_PROFIL
            else:
            #_case:  # default:
                # Report an error, this shouldn't happen!
                print("IS_ERR")
                printf("##default=" + str(variant_array[j][i]['typeof']) + "##")
                #break;
            ## END if [match case (switch case)]
            print("")  # Line break
            ## END for i
        print("")  # Line break
        ## END for j


    variant_array = None  # Clear the list data.
    print("===========================================")
    """
    #========================================================================<<

    #========================================================================>>
    ## ====>> Do insert and retreive from mixed data types table. ===========>>


    ## You will need this table name available to all of the following examples.
    db_table_mixed = "DATA_Mixed"  # Table with mixed data types.




    # Note: REAL == SQLITE_FLOAT == IS_FLOAT == 2 (int constants)
    sql_table_fields = "CREATE TABLE IF NOT EXISTS " + db_table_mixed + "\
                (Date TEXT\
                , Week INTEGER\
                , Employee_ID INTEGER\
                , Name INTEGER\
                , Avatar BLOB\
                , Monday INTEGER\
                , Tuesday INTEGER\
                , Wednesday INTEGER\
                , Thursday INTEGER\
                , Friday INTEGER);"

    ## Create mixed data table (empty).
    err_return = example_sql3.db_table_create(file_name, sql_table_fields)
    if err_return == 0:
        print("Table " + db_table_mixed + " could not be created " + file_name)
    elif err_return == 1:
        print("Table " + db_table_mixed + " was successfully created in " + file_name)
    else:
        printf("There was an unknown error " + sql_table_fields )
    print("===========================================")




    ## read image file Tux avatar for BLOB.
    file_tux = "Tux.jpg"
    fp_tux = open(file_tux, "rb")
    bstr_avatar = fp_tux.read()  ## 30000
    #print(bstr_avatar)
    fp_tux.close()

    ## The following method inserts the data as part of the query statement.
    ## For this use we need to convert the byte string into a formated string
    ## of Hex value pairs x'ffb623 ... '. If using the parametrized statements
    ## as previously shown in the single blob column above
    ## ( ? in the query plus the sqlite3_bind_*) then the BLOB data must be
    ## provided as a byte string b"\xff\xd8\xff\xe2\x02 ...".

    # Convert bytes to string of hex. The x'...' will be added in the query.
    hex_avatar = bstr_avatar.hex()
    #hex_avatar = bin_data_bstring.hex()
    #print(hex_avatar)

    ## NOTE: Sqlite date, time, datetime, julianday and strftime are built in
    ## functions and only accessed via the querry statements.
    ## TEXT == YYYY-MM-DD HH:MM:SS == datetime( ... ) (default)
    ## https://www.sqlite.org/lang_datefunc.html
    ## If you need Date and Time functions outside of sqlite use the Python 3
    ## module "import datetime". It can use the same format as sqlite.


    # Placing the hex string directly into the query statement. The hex string
    # must be prefixed with 'x'  x'ffb623'.
    # The beter and safer way is to use ? placeholder and sqlite3_bind_* API.
    sql_tbl_entry = "INSERT INTO " + db_table_mixed + "\
                (Date\
                , Week\
                , Employee_ID\
                , Name\
                , Avatar\
                , Monday\
                , Tuesday\
                , Wednesday\
                , Thursday\
                , Friday)\
                VALUES(datetime('now', 'localtime')\
                , 1\
                , 34\
                , \"Joe Blogs\"\
                , x\'" + hex_avatar + "\'\
                , 7\
                , 5\
                , 8\
                , 7\
                , 9);"


    err_return = example_sql3.db_insert_table_rowdata(file_name, sql_tbl_entry)
    if err_return == 0:
        print("Row data was NOT entered into " + file_name)
    elif err_return == 1:
        print("Row data was entered into " + file_name)
    else:  # == -1
        print("There was an unknown error.")
    print("===========================================")


    """
    ## Retrive the previously inserted mixed data types.

    db_table_namex2 = db_table_mixed  # "Hrs_worked_Tracker"

    ## Get number of columns in a named table.
    number_cols_retx = []

    err_return = example_sql3.db_get_table_number_cols(file_name, db_table_namex2, number_cols_retx)
    if err_return == 0:
        printf("Could not retrieve table " + db_table_namex2 + " column number from " + file_name)
    elif err_return == 1:
        print("Retrieved table " + db_table_namex2 + " column number from " + file_name)
    else:  # == -1
        print("There was an unknown error.")

    print("\nTable number of columns:" + str(number_cols_retx[0]))
    print("===========================================")


    ## Get number of rows in a named table.
    number_rows_retx = []

    err_return = example_sql3.db_get_table_number_rows(file_name, db_table_namex2, number_rows_retx);
    if err_return == 0:
        print("Could not retrieve table " + db_table_namex2 + " row number from " + file_name)
    elif err_return == 1:
        print("Retrieved table " + db_table_namex2 + " row number from " + file_name)
    else:  # == -1
        print("There was an unknown error.")

    print("\nTable number of rows:" + str(number_rows_retx[0]))
    print("===========================================")


    # Row and column count trackers.
    variant_array_rowlen = number_rows_retx[0]  # Always check the most recent number of rows in the table
    variant_array_collen = number_cols_retx[0] + 1  # The number of columns + 1 for rowid

    ret_variant_field_elements = []
    ret_variant_row_elements = []  # Track the number of used elements in the array

    ## Declare an empty list. Note was previously cleared to None.
    variant_array = []
    for i in range(variant_array_rowlen):
        variant_array.insert(i, [])  # Initialise the rows As list
        for j in range(variant_array_collen):
            # Initialise each column with a dict. [row][col][dict]
            variant_array[i].insert(j, {'typeof': None, 'data': None, 'length': 0})

    ## retreive the list of dict...
    err_return = example_sql3.db_list_table_all_types(file_name, db_table_namex2, variant_array, variant_array_collen, variant_array_rowlen, ret_variant_field_elements, ret_variant_row_elements)
    if err_return == 0:
        print("Could not retrieve " + db_table_namex2 +" table data from " + file_name)
    elif err_return == 1:
        print("Retrieved table " + db_table_namex2 +" data from " + file_name)
    else:  # == -1
        print("There was an unknown error.")


    # Debug test the size of returned number of row elements against our array[n]
    if variant_array_rowlen < ret_variant_row_elements[0]:
        print("Error! Dynamic array is too small for number of rows.")
    elif variant_array_rowlen > ret_variant_row_elements[0]:
        print("Good! Dynamic array is lager than number of rows.")
    elif variant_array_rowlen == ret_variant_row_elements[0]:
        print("Good! Dynamic array same size as number of rows.")
    else:
        print("Unknown error!")  # should never occur

    ## Print out or use our returned data from the VARIANT (variant_array).
    print("\nPrint mixed table data from variant_array.")
    for j in range(variant_array_rowlen):  # or ret_variant_row_elements
        print("\nRow element = " + str(j))
        for i in range(variant_array_collen):  # < ret_variant_field_elements
            print("Column element = " + str(i))
            # access each element of tag_VARIANT in variant_array[n]
            caseis = variant_array[j][i]['typeof']
            ## C like match case is introduced in Python 3.10 onwards.
            #match variant_array[j][i]['typeof']:
            #switch (variant_array[j][i].type)
            if caseis == IS_NULL:
                # Do stuff for NULL pointer, using variant_array[n].value.vval
                # This will denote an unused array element. It is possible to
                # use this data structure were we test for null as an empty element
                # or as and empty type
                print("IS_NULL")
                print(variant_array[j][i]['data'])
            elif caseis == IS_INTEGER:
                # Do stuff for INTEGER, using variant_array[j][i]['data']
                print("IS_INTEGER")
                print(str(variant_array[j][i]['data']))
            elif caseis == IS_FLOAT:  ## REAL
                # Do stuff for REAL, using variant_array[j][i]['data']
                print("IS_FLOAT (REAL)")  ## REAL
                print(str(variant_array[j][i]['data']))
            elif caseis == IS_TEXT:
                # Do stuff for TEXT, using variant_array[j][i]['data']
                print("IS_TEXT")
                print(str(variant_array[j][i]['data']))
            elif caseis == IS_BLOB:
                # Do stuff for BLOB, using variant_array[j][i]['data']
                # NOTE: print-ing a byte string will give undefined results
                # due to the effect of control characters (Dec 0 to 31) as well
                # as NON 7-bit ASCII print characters (Dec 128 to 255).
                print("IS_BLOB")
                byte_string = variant_array[j][i]['data']
                print("bytestring = ", end='')  # Can't concatenate bytes b''
                print(byte_string)  # As byte string.
                # Python can calculate the length of a byte string.
                print("byte_string len = ", len(byte_string))
                # Legacy length of byte array...
                print("array len = " + str(variant_array[j][i]['length']))
                print("bstr_avatar len = " + str(len(bstr_avatar)))

                # Debug length test.
                if variant_array[j][i]['length'] == len(bstr_avatar):
                    print("Returned bytes same length as original bytes.")

                # Formated binaries.
                print("BLOB int = {", end='')  # Can't concatenate bytes b''
                for x in range(variant_array[j][i]['length']):
                    print(byte_string[x], end=',')  # As byte decimal (native).
                print("\b}")  # Note: Backspace does not work in all terminals.

                print("BLOB hex = {", end='')
                for x in range(variant_array[j][i]['length']):
                    #print(hex(byte_string[x]), end=',')  # As Hexidecimal.
                    #print('{:#04x}'.format(byte_string[x]), end=',')  # with '0' padding.
                    print('{:02x}'.format(byte_string[x]), end=' ')  # with '0' padding.
                print("\b}")  # Note: Backspace does not work in all terminals.

            else:
            #_case:  # default
            #default:
                # Report an error, this shouldn't happen!
                print("IS_ERR")
                printf("##default=" + str(variant_array[j][i]['typeof']) + "##")
                #break;
            ## END if [match case (switch case)]
            print("")  # Line break
            ## END for i
        print("")  # Line break
        ## END for j


    #variant_array = None  # Crear the list data.
    print("===========================================")
    """



    return None
## END main






# Console Pause wrapper.
def Con_Pause():
    dummy = ""
    print("")
    dummy = input("Press [Enter] key to continue...")
    return None

if __name__ == '__main__':
    main()
