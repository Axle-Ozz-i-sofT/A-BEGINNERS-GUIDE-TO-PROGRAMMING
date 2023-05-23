#-------------------------------------------------------------------------------
# Name:         example_sql3.py (based upon basics_2.c, ozz_sql3.h)
# Purpose:      SQLite3 Basic examples tests.
#               Tests for convenience wrapper functions for SQLite version 3.
#
# Platform:     Win64, Ubuntu64
# Depends:      SQLite v3.34.1 plus (dll/so), ctypes, ozz_sql3.py
#
# Author:       Axle
#
# Created:      15/05/2023
# Updated:      15/05/2023
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
# Notes:
#
# !!! All routines and examples are based upon tables using only TEXT (String)
# type field entries. The only exception is the system generated rowid.
# See NOTE: ... below !!!
#
# To use multiple type field entries a "linked list" data structure is required
# to be able to accept mixed data types in C.
# SEE: sqlite3_column_type(statement, i) examples.
#
#
# NOT using sqlite3_exec() with callback. All returns are handled in the
# calling routines.
#
# There is no set or defined rule in the standards for what error value of
# error returns must be in an application function. It is implementation
# defined and can differ from function to function. The only (most common)
# rule is returning a value of 0==Success from main() to the OS. This can also
# be written as return EXIT_SUCCESS; and return EXIT_FAILURE;
#
# Function returns can be any value, so I have used a mix of error return
# schemes depending upon the return type of the functions routines. in most
# cases 0, -1, 1, 2 will either define success, fail or a specific error.
# In a commercial application we would more likely return the error code of
# SQLite3 and handle the sql error code directly.
# https://www.sqlite.org/rescode.html
# I have over simplified the returns to a basic Success-TRUE, Fail-FALSE scheme
# for simplicity of the examples.
#
# The routines and functions use an excessive amount of error reporting. Normally
# we only handle actual errors and do so silently in the background of our
# application. I have created error info on both successful returns as well as
# on errors only as a visual guide. If using the sqlit3_open* and sqlite2_close*
# directly from your main application you can also use the sqlite3_errmsg()
# directly. Another option is to return the integer value of sqlite3_errmsg()
# from your function and handle the error from the calling statement as an sql
# error rather than the 0==False(fail), 1==True(success) etc. that I have made
# up for the examples.
#
# Error returns can be handled in a number of ways. I have created my own
# error returns of 0, 1, -1 etc as well as displaying the SQLite3 error codes
# in the functions. This is excessive and only there to exemplify the different
# error returns. In practice we would return the error code number to our
# calling statement. sqlite3_errmsg(db) or rc
# return strcpy((char)error_return, sqlite3_errmsg(db)); or
# return (int)rc;
# Typically sqlite3_initialize(), sqlite3_open_v2(), sqlite3_close(), sqlite3_shutdown()
# would be called from main() where we would be able to retrieve the last
# sqlite3_errmsg(db) directly. We wouldn't open and close the database file
# for every query as it is more appropriate to keep it open for as long as
# repetitive transactions are taking place.
#
# In light of the above take note of the 3 different ways of "Opening" an
# sqlite database file.
# SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_READONLY
# Each has pros and cons with regard to safety when opening a database file. For
# example we may not wish to create a new database file if the wrong file name
# is entered and prefer an error return "db file not found" instead.
#
#
# SQLite3 can only accept 1 command statement at a time. To run multiple
# statements they must use the sqlite3_prepare_v2, sqlite3_step, sqlite3_finalize
# in a loop for each statement. This can be done by sending a formatted array of
# statements and sending each separately in a loop or as a loop in the function.
# This would also be true when sending query statements that have multiple returns
# where an appropriate sized array will need to be supplied.
#
#
# data is stored internally as bytes by default. The length of the bytes and
# the affinity (data type) is stored in the header entry of the data.
# Although stored as bytes, this data could be of any data type.
#
# Internally SQLite 3 stores all data as BYTEs and is recovered as a number of
# BYTEs. The type affinity is used to convert the bytes back to the storage type
# associated with the cell in that column.
#
# The different data types are to allow code conformity with other SQL database
# engines which have static typing. Keeping this in mind it is acceptable to
# use only TEXT for most data storage. Where a numeric value is required
# internally by sqlite such as column or row ID number then I would suggest
# using INTEGER PRIMARY KEY if needed. You will need to INSERT and and SELECT
# providing the correct data type containers in this case.
# I am using TEXT only tables to simplify the examples. You cane see a
# prototype for the more complex mixed data types in the final example.

# In reality most applications will take user input as text, data read from a
# document as text and even transport most data as text. (All data is text in
# python unless specifically stated for example.) Remember that even user
# numeric keyboard input is text until converted to its integer representation
# by the programming language. For example we may take an input from a user in
# C language to a variable int my_number = getchar(); The text arrives from
# the keyboard as a hexadecimal representation of the character '5' as 0x35
# (ASCII decimal 53), int my_text_number = getchar() = 53
# To use it as an actual integer we need to convert the character to an
# integer (53 - 48) int 0 = char dec48 - 48 ( SEE and ASCII chart),
# so to get the integer value (5) from the text character'5' we need to convert
# int my_interger_number = (my_text_number(53 '5') - 48); (== 5)
# Note my use of inverted commas character '5' (hex 0x35) vs integer 5 (hex 0x05)
# to represent character '5' and integer 5.
#
# The above is primarily suggested for a small database app without complex data
# structures as we would know in advance the data type required for each column.
# For a more accurate (and complex) example see the last function example in this
# wrapper library db_list_table_all_types().
#
# White space between column values or after the ',' delimiter will need to be
# managed by the calling application. Alternatively you can modify any routine
# that reads from the database and change strcat(buffer, ", ") to ",".
# ## Removed/Changed all returns from ", " to ","
#
# I am treating this database example in a similar way to a CSV data file
# storage which is less efficient, but I wanted to keep the examples simple
# even if a little slower and less optimised than they would be in commercial
# practice. Ultimately it is up to the programmer to choose the most
# appropriate method of SQLite3 usage for there requirements.
#
# Handling NULL values. The following is a simple example of handling NULL
# pointer values from an empty column entry. In this example I have replaced
# non-existing value with text "[NULL]". This is just an example and we can
# deal with this return in any way that is appropriate for the context of our
# application. I have not handled NULL returns in the examples.
# data = (const char*)sqlite3_column_text( stmt, 0 );
#        printf( "%s\n", data ? data : "[NULL]" );
#
# Note! Some keywords are reserved for SQLIte, for example "rowid".
#
# Using concatenate strcat() can lead to SQL injection! SEE Parameterised
# statements and int sqlite3_bind_text(sqlite3_stmt*,int,const char*,int,void(*)(void*));
# https://www.sqlite.org/c3ref/bind_blob.html
#------------------------------------------------------------------------------
# Credits:
# https://resources.oreilly.com
# https://zetcode.com/db/sqlitec/
# https://gist.github.com/jsok/2936764
# ++
#------------------------------------------------------------------------------
#
#------------------------------------------------------------------------------
# TODO:
# Convenience wrapper functions for SQLite version 3 [Done]
# Remove excess comments.[ Done]
# Move sqlite open/close to main(). [?]
# Return sqlite errors to calling functions? [?]
# alt change sqlite errors to better value set?
#
# Two generalised functions to take and return table data? [Not done]
# I may include a universal function at the end of this document that will
# take most sqlite3 commands and return the appropriate arrays of data.
# Send command/s
# Retrieve command/s
#
# Mark extra error returns as DEBUG. [Done]
# Revise db_delete_table_rowdata()
# Revise db_insert_table_rowdata_rowid() for error handling while loop.
#
# Check array off by 1s.
#------------------------------------------------------------------------------


import ctypes, sys, os
import ozz_sql3

## ====>> Error Constants
# Beware of name conflicts!
from ozz_sql3_constants import  *


#==============================================================================

# Ensure that sqlite3.dll is in the system path or in the working directory
# of the project python script at run-time.

# Get SQLite version. (short function)
# Returns integer version. v3.34.1 = 034001 = 3|034|001
# Mmmmppp, with M being the major version, m the minor, and p the point release.
def sqlite3_get_version0():
    # Get the path to the SQLite3 shared library file (dll/so).
    f_library_sql3 = ozz_sql3.get_libsql3_path()  # Get the path for sqlite.dll/so

    # Load the sqlite3 shared library. The returned ID from loading the sqlite
    # shared library must be passed to other functions.
    id_lib_sql3  = ozz_sql3.load_libsql3(f_library_sql3)

    # Get our SQLite version. Confirmation that sqlite 3 is installed as a
    # shared library and compiling/working correctly.

    # NOTE: I am using the C API interface directly and not as a query. SQLite
    # provides a limited number of helper MACROS that can be accessed directly
    # without opening a databse.

    # The return is already converted to Python UTF-8 string by the function.
    return ozz_sql3.sqlite3_libversion_number(id_lib_sql3)
## END Function



# Get SQLite version. (short function)
# Returns string to version buffer.
# We can call a number of the SQLite C APIs without needing to open a database.
# The SQLite3.dll library is connected to our application at startup.
def sqlite3_get_version1():
    # Get the path to the SQLite3 shared library file (dll/so).
    f_library_sql3 = ozz_sql3.get_libsql3_path()  # Get the path for sqlite.dll/so

    # Load the sqlite3 shared library. The returned ID from loading the sqlite
    # shared library must be passed to other functions.
    id_lib_sql3  = ozz_sql3.load_libsql3(f_library_sql3)

    return ozz_sql3.sqlite3_libversion(id_lib_sql3)
## END Function

# https://zetcode.com/db/sqlitec/
# Get SQLite version - query. (long function)
# Returns string to version buffer, as well as int sqlite error codes.
def sqlite3_get_version2(ret_version):
    # Get the path to the SQLite3 shared library file (dll/so).
    f_library_sql3 = ozz_sql3.get_libsql3_path()  # Get the path for sqlite.dll/so

    # Load the sqlite3 shared library. The returned ID from loading the sqlite
    # shared library must be passed to other functions.
    id_lib_sql3  = ozz_sql3.load_libsql3(f_library_sql3)

    # sqlite3 *p_db;  # database handle (structure).
    p_db = ozz_sql3.p_sqlite3()  # Get the sqlite3 structure.

    # sqlite3_stmt *p_stmt;  # structure represents a single SQL statement
    p_stmt = ozz_sql3.p_sqlite3_stmt()  # Get the sqlite3_stmt structure.

    return_code = 0

    p_zVfs = None  # can use NULL = None
    pzTail = None
    NULL = None

    db_filename_ram = ":memory:"
    # Note: :memory: can be used instead of a file for temporary database
    # operations.
    # defualts to: [SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE]
    return_code = ozz_sql3.sqlite3_open(id_lib_sql3, db_filename_ram, p_db)
    # On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    # is returned.
    if return_code != SQLITE_OK:
        # This is error handling code for the sqlite3_prepare_v2 function call.
        print("Cannot open database: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db).decode('utf-8')) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    # Before an SQL statement is executed, it must be first compiled into a
    # byte-code with one of the sqlite3_prepare* functions.
    # The sqlite3_prepare_v2 function takes five parameters. The first parameter
    # is the database handle obtained from the sqlite3_open function. The second
    # parameter is the SQL statement to be compiled. The third parameter is the
    # maximum length of the SQL statement measured in bytes.
    # -1 causes the SQL string to be read up to the first zero terminator which
    # is the end of the string here. (or supply the exact no of bytes.)
    # The fourth parameter is the statement handle. It will point to the
    # pre-compiled statement if the sqlite3_prepare_v2 runs successfully. The last
    # parameter is a pointer to the unused portion of the SQL statement. Only
    # the first statement of the SQL string is compiled, so the parameter points
    # to what is left un-compiled. We pass 0 since the parameter is not important
    # for us SEE: sqlite3_clear_bindings(stmt);.

    sql_query = "SELECT SQLITE_VERSION()"

    # On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    # is returned.
    return_code = ozz_sql3.sqlite3_prepare_v2(id_lib_sql3, p_db, sql_query, -1, p_stmt, pzTail)
    if return_code != SQLITE_OK:
        # This is error handling code for the sqlite3_prepare_v2 function call.
        print("Failed to prepare data: " + str(id_lib_sql3.sqlite3_errmsg(id_lib_sql3, p_db).decode('utf-8')), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    # Call once for each cols/rows. enumerate col/rows for iCol.
    # The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    # that there is another row ready. Our SQL statement returns only one row
    # of data, therefore, we call this function only once.
    return_code = ozz_sql3.sqlite3_step(id_lib_sql3, p_stmt)
    if return_code != SQLITE_ROW:
        print("Step error: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db).decode('utf-8')) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return 0

    # returns data but no error code. Check for NULL returns.
    # Insert the return utf-8 string at ret_version[0]
    str_buffer = ozz_sql3.sqlite3_column_text(id_lib_sql3, p_stmt, 0)
    ret_version.insert(0, str_buffer)

    #sqlite3_bind_*()  # After sqlit3_prepare_v2()
    #sqlite3_clear_bindings(stmt);  # clears leftover statements from sqlite3_prepare.

    # The sqlite3_finalize function destroys the prepared statement object.
    return_code = ozz_sql3.sqlite3_finalize(id_lib_sql3, p_stmt)
    if return_code != SQLITE_OK:
        # This is error handling code.
        print("Failed to finalize data: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db).decode('utf-8')) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        id_lib_sql3.sqlite3_close(p_db)
        return -1

    return_code = ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
    if return_code != SQLITE_OK:
        # This is error handling code. NOTE! As p_db is closed the error code may not be available!
        print("Failed to close database: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db).decode('utf-8')) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        return -1

    return 1
## END Function



# Check if a file name exists by opening the file for read operations. If the
# file does exist it will not be created and return an error.
# 0 == False | 1 == True
# This is a standard file operation and not part of SQLite.
def file_exists(file_name):

    try:
        fp = open(file_name, "r")
    except FileNotFoundError:
        print("Cannot open file " + file_name, file=sys.stderr)  # DEBUG
        return 0

    fp.close()
    return 1


# Check if file exist and is an sqlite3 database.
# Looks for "SQLite format 3" in the 100 byte header of the file.
# SEE: db_file_create()
def db_file_exists(db_file_name):

    try:
        fp = open(db_file_name, "rb")  # Open as read binary.
    except FileNotFoundError:
        print("Cannot open file " + db_file_name, file=sys.stderr)  # DEBUG
        return 0

    # The SQLite 3 header is exacly 100 (0 - 99) bytes long imediately followed
    # by a newline char '\n' == Dec 13 == LF at 101 charcters.
    try:
        line_buffer = fp.read(110)  # read 110 bytes.
        cnt = 0
        for i in line_buffer:  # i will return the ASCII Dec value of the byte.
            if i == 10 or i == 13:  # Search for new line at 101 byte
                #print(i)  # DEBUG
                # (0 - 99 is 100 bytes) so cnt 100 is actually 101
                # we wil take it as 100 bytes found :)
                cnt_chr = cnt
            cnt += 1

        if ( cnt_chr > 100):  # Check if we found more than 100 bytes?
            print("Header too long. " + str(cnt_chr) + " chrs.", file=sys.stderr)  # DEBUG
            fp.close()  # close the file.
            return 0
        else:
            # The first 16 bytes is a string denoting "SQLite format 3". So
            # we will slice the first 16 bytes and convert it to a string.
            temp_buffer = line_buffer[0:16].decode('utf-8')
            search = "SQLite format 3"
            # Test if the string == "SQLite format 3". If true it should be a
            # genuine SQLite 3 databse file.
            if search in temp_buffer:
                fp.close()  # close the file.
                return 1
            else:
                print("\"SQLite format 3\" Header not found!", file=sys.stderr)  # DEBUG
                fp.close()  # close the file.
                return 0
    except:
        #print("Fail with exception!")  # DEBUG
        return 0  # File read error

    return None
## END Function

#==============================================================================
# These are really the same routine for most database operations :)
# You could use this same routine with a little modification to accept and
# return most sqlite3 database queries.
#
# To create a table with the correct SQLite 3 header file information we will
# need to create a table and then delete (DROP) the table. This will leave a db
# file with the first 16 bytes containing "SQLite format 3". We can then use
# our previous db_file_exists() function to test if it is a valid db file.
#
# This function runs 2 separate SQLite queries. The first creates a temporary
# (dummy) table and the second query deletes the temporary table leaving an
# empty SQLite3 database file with a valid header.
def db_file_create(db_file_name):

    # Get the path to the SQLite3 shared library file (dll/so).
    f_library_sql3 = ozz_sql3.get_libsql3_path()  # Get the path for sqlite.dll/so

    # Load the sqlite3 shared library. The returned ID from loading the sqlite
    # shared library must be passed to other functions.
    id_lib_sql3  = ozz_sql3.load_libsql3(f_library_sql3)

    # sqlite3 *p_db;  # database handle (structure).
    p_db = ozz_sql3.p_sqlite3()  # Get the sqlite3 structure.

    # sqlite3_stmt *p_stmt;  # structure represents a single SQL statement
    p_stmt = ozz_sql3.p_sqlite3_stmt()  # Get the sqlite3_stmt structure.

    return_code = 0


    # If the database name exists and is "SQLite format 3", don't try to create a new db.
    # Note: This function is part of the examples_sql3 and not part of ozz_sql3 binder.
    if 1 == db_file_exists(db_file_name):
        print("Database: " + db_file_name + " already exists!", file=sys.stderr)  # DEBUG
        ## stderr occurs after the proccess closes, so can occurer later than expected.
        return 2  # error codes needs to be reviewed.

    p_zVfs = None  # can use NULL = None
    pzTail = None
    NULL = None

    return_code = ozz_sql3.sqlite3_open_v2( id_lib_sql3, db_file_name, p_db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, p_zVfs )
    if return_code != SQLITE_OK:
        # Note: these print returns can be commented out if only the Bool return 0; is required.
        print("Can't open database: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db).decode('utf-8')) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db )
        return -1

    # This is a single query terminated by the ';' semi colon.
    # Multiple statements such as the following must be prepared, stepped and
    # finalized individually.
    # char *sql = "CREATE TABLE IF NOT EXISTS Temp_table(ID INT);"
    #             "DROP TABLE IF EXISTS Temp_table;";
    sql1 = "CREATE TABLE IF NOT EXISTS Temp_table(ID INTEGER);"

    #"create table aTable(field1 int); drop table aTable;
    # We can only send one query at a time to sqlite3.
    # sqlite3_prepare compiles the sql query into the byte code for sqlite.
    return_code = ozz_sql3.sqlite3_prepare_v2(id_lib_sql3, p_db, sql1, -1, p_stmt, pzTail)
    # On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    # is returned.
    if return_code != SQLITE_OK:
        # This is error handling code for the sqlite3_prepare_v2 function call.
        print("Failed to prepare data: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db).decode('utf-8')) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    # The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    # that there is another row ready. Our SQL statement returns only one row
    # of data in this instance, therefore, we call this function only once.
    # If we are writing or reading lines of table then we will need to use
    # sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.
    return_code = ozz_sql3.sqlite3_step(id_lib_sql3, p_stmt );  # run once for one statement
    #    while( sqlite3_step( stmt ) == SQLITE_ROW ) {;}
    if return_code != SQLITE_DONE:  # SQLITE_DONE==101, SQLITE_ROW==100
        print("Step 1 failed: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db).decode('utf-8')) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return 0

    #sqlite3_bind_*()  # After sqlit3_prepare_v2()
    #sqlite3_clear_bindings(stmt);

    # The sqlite3_finalize function destroys the prepared statement object.
    # and commits the changes to the database file.
    return_code = ozz_sql3.sqlite3_finalize(id_lib_sql3, p_stmt )  # Commit to the database.
    if return_code != SQLITE_OK:  # SQLITE_OK==0
        # This is error handling code.
        print("Failed to finalize data: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db).decode('utf-8')) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

##==============================================================================
    # The following deletes the temporary table leaving the SQLite header files intact.

    ## The database file is already open!!!
    #    return_code = sqlite3_open_v2(id_lib_sql3, db_file_name, p_db, SQLITE_OPEN_READWRITE, NULL );
    #    if return_code != SQLITE_OK:
    #        print("Can't open database: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db).decode('utf-8')) + " | " + str(return_code), file=sys.stderr)  # DEBUG
    #        ozz_sql3.sqlite3_close( id_lib_sql3, p_db )
    #        return 0

    sql2 = "DROP TABLE IF EXISTS Temp_table;"  # Delete the table.

    #"create table aTable(field1 int); drop table aTable;"
    return_code = ozz_sql3.sqlite3_prepare_v2(id_lib_sql3, p_db, sql2, -1, p_stmt, pzTail)
    # On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    # is returned.
    if return_code != SQLITE_OK:
        # This is error handling code for the sqlite3_prepare_v2 function call.
        print("Failed to prepare data: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db).decode('utf-8')) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    # The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    # that there is another row ready. Our SQL statement returns only one row
    # of data in this instance, therefore, we call this function only once.
    # If we are writing or reading lines of table then we will need to use
    # sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.
    return_code = ozz_sql3.sqlite3_step(id_lib_sql3, p_stmt )
    #    while( sqlite3_step( stmt ) == SQLITE_ROW ) {;}
    if return_code != SQLITE_DONE:  # SQLITE_DONE==101, SQLITE_ROW==100
        print("Step 2 failed: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db).decode('utf-8')) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return 0

    #sqlite3_bind_*()  # After sqlit3_prepare_v2()
    #sqlite3_clear_bindings(stmt);

    # The sqlite3_finalize function destroys the prepared statement object.
    return_code = ozz_sql3.sqlite3_finalize(id_lib_sql3, p_stmt )  # Commit to the database.
    if return_code != SQLITE_OK:  # SQLITE_OK==0
        # This is error handling code.
        print("Failed to finalize data: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db).decode('utf-8')) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    # The sqlite3_close function closes the database connection.
    return_code = ozz_sql3.sqlite3_close(id_lib_sql3, p_db)   # SQLITE_OK==0
    if return_code != SQLITE_OK:
        # This is error handling code. NOTE! As p_db is closed the error code may not be available!
        print("Failed to close database: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db).decode('utf-8')) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    print("Successfully created " + db_file_name + " SQLite3 database.", file=sys.stderr)  # DEBUG

    return 1
## END Function


# Delete the SQLite3 database file, This is really just a standard file delete
# function with an extra safety y/n check. We could also include db_file_exists()
# to test if it is a valid SQLite database that is being deleted. Always
# include some form of safety check before deleting any files on a users system
# as file deletes cannot be undone.
def db_file_delete(db_file_name):

    str_return_code = 0

    print("Are you sure you want to delete " + db_file_name + " (y/n) ?")
    str_return_code = str(input())
    if ((str_return_code[0:] == 'y') or (str_return_code[0:] == 'Y')):
        if os.path.exists(db_file_name):
            try:
                #os.chmod(filePath, 0777)
                return_code = os.remove(db_file_name)
                print("Successfully deleted " + db_file_name, file=sys.stderr)  # DEBUG
                return 1
            except OSError:
                print("Unable to delete or file not exists." + db_file_name, file=sys.stderr)  # DEBUG
                return -1
        else:
            print("File " + db_file_name + " does not exists.", file=sys.stderr)  # DEBUG
            return -1

    else:  # any not 'y'
        print("File " + db_file_name + " delete aborted by user." , file=sys.stderr)  # DEBUG
        return 0  # Aborted delete file.

    return 0
## END Function


# Test if a table name exist in a database.
# https://fossil-scm.org/home/info/4da2658323cab60e?ln=1945-1951
# https://www.sqlite.org/c3ref/table_column_metadata.html
def db_table_exists(db_file_name, db_table_name):

    # Get the path to the SQLite3 shared library file (dll/so).
    f_library_sql3 = ozz_sql3.get_libsql3_path()  # Get the path for sqlite.dll/so

    # Load the sqlite3 shared library. The returned ID from loading the sqlite
    # shared library must be passed to other functions.
    id_lib_sql3  = ozz_sql3.load_libsql3(f_library_sql3)

    # sqlite3 *p_db;  # database handle (structure).
    p_db = ozz_sql3.p_sqlite3()  # Get the sqlite3 structure.

    # sqlite3_stmt *p_stmt;  # structure represents a single SQL statement
    p_stmt = ozz_sql3.p_sqlite3_stmt()  # Get the sqlite3_stmt structure.

    return_code = 0
    err_ret = 0

    p_zVfs = None  # can use NULL = None
    pzTail = None
    NULL = None
    #c_NULL = ctypes.POINTER(ctypes.c_char_p)()

    return_code = ozz_sql3.sqlite3_open_v2( id_lib_sql3, db_file_name, p_db, SQLITE_OPEN_READONLY, p_zVfs )
    if return_code != SQLITE_OK:
        # Note: these print returns can be commented out if only the Bool return 0; is required.
        print("Can't open database: " + ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

## Alternative method.
# https://cppsecrets.com/users/1128311897114117110109107110505764103109971051084699111109/C00-SQLitesqlite3free.php
# char *sql = "SELECT count(*) FROM sqlite_master WHERE type = 'table' AND name = 'YourTableName';";
# const char *sql = "SELECT 1 FROM sqlite_master where type='table' and name=?";  <--

    # List all table names <- New function
    # !! Recheck with the following !!!
    """
        sql_table_list = "SELECT name FROM sqlite_master WHERE type='table'"

        return_code = ozz_sql3.sqlite3_prepare_v2(id_lib_sql3, p_db, sql_table_list, strlen(sql_table_list), p_stmt, NULL);
        if(return_code == SQLITE_OK)
            {
            # Loop through all the tables
            while(ozz_sql3.sqlite3_step(id_lib_sql3, p_stmt) == SQLITE_ROW)
                {
                if(!strcmp((const char*) ozz_sql3.sqlite3_column_text(id_lib_sql3, p_stmt, 0), table_name))
                    return true;
                }
            }
    """

    #null_ptr = ctypes.POINTER(ctypes.c_int)()
    # ##### This may need a loop!!!!
    # Currently only tested with single table!!!
    # db_handle, db_name, db_table_name, col_name, NULL, ...
    return_code = ozz_sql3.sqlite3_table_column_metadata(id_lib_sql3, p_db, NULL, db_table_name, NULL, NULL, NULL, NULL, NULL, NULL)
    if return_code != SQLITE_OK:
        print("Table did not exist: " + ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        err_ret = 0
    else:
        print("Table exists: " + ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        err_ret = 1

    # The sqlite3_close function closes the database connection.
    return_code = ozz_sql3.sqlite3_close(id_lib_sql3, p_db);   # SQLITE_OK==0
    if return_code != SQLITE_OK:
        # This is error handling code. NOTE! As p_db is closed the error code may not be available!
        print("Failed to close database: " + ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    return err_ret  #Return this function error code.
## END Function


# Get the total number of tables in a named database file.
def db_get_number_tables(db_file_name, number_tables_ret):

    # Get the path to the SQLite3 shared library file (dll/so).
    f_library_sql3 = ozz_sql3.get_libsql3_path()  # Get the path for sqlite.dll/so

    # Load the sqlite3 shared library. The returned ID from loading the sqlite
    # shared library must be passed to other functions.
    id_lib_sql3  = ozz_sql3.load_libsql3(f_library_sql3)

    # sqlite3 *p_db;  # database handle (structure).
    p_db = ozz_sql3.p_sqlite3()  # Get the sqlite3 structure.

    # sqlite3_stmt *p_stmt;  # structure represents a single SQL statement
    p_stmt = ozz_sql3.p_sqlite3_stmt()  # Get the sqlite3_stmt structure.

    return_code = 0
    buffer = ""  # temp buffer [MAX 128 characters]

    table_count = 0

    p_zVfs = None  # can use NULL = None
    pzTail = None
    NULL = None

    return_code = ozz_sql3.sqlite3_open_v2( id_lib_sql3, db_file_name, p_db, SQLITE_OPEN_READONLY, p_zVfs )
    if return_code != SQLITE_OK:
        # Note: these print returns can be commented out if only the Bool return 0; is required.
        print("Can't open database: " + ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    sql_table_list = "SELECT * FROM sqlite_schema WHERE type='table' AND name NOT LIKE 'sqlite_%' ORDER BY 1;"  # sqlite_schema, sqlite_master

    return_code = ozz_sql3.sqlite3_prepare_v2(id_lib_sql3, p_db, sql_table_list, -1, p_stmt, pzTail)
    # On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    # is returned.
    if return_code != SQLITE_OK:
        # This is error handling code for the sqlite3_prepare_v2 function call.
        print("Failed to prepare data: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db).decode('utf-8')) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    # The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    # that there is another row ready. Our SQL statement returns only one row
    # of data at a time.
    # If we are writing or reading multiple lines of table then we will need to
    # use sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.
    while(ozz_sql3.sqlite3_step(id_lib_sql3, p_stmt) == SQLITE_ROW):  # SQLITE_ROW
        buffer = buffer + ozz_sql3.sqlite3_column_text(id_lib_sql3, p_stmt, 1)
        table_count += 1  # Count the number of table names found.

    # insert the return into element[0]
    number_tables_ret.insert(0, table_count)  # Our table count by reference.

    #sqlite3_bind_*()  # After sqlit3_prepare_v2()
    #sqlite3_clear_bindings(stmt);

    # The sqlite3_finalize function destroys the prepared statement object.
    return_code = ozz_sql3.sqlite3_finalize(id_lib_sql3, p_stmt )  # Commit to the database.
    if return_code != SQLITE_OK:  # SQLITE_OK==0
        # This is error handling code.
        print("Failed to finalize data: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db).decode('utf-8')) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    # The sqlite3_close function closes the database connection.
    return_code = ozz_sql3.sqlite3_close(id_lib_sql3, p_db)   # SQLITE_OK==0
    if return_code != SQLITE_OK:
        # This is error handling code. NOTE! As p_db is closed the error code may not be available!
        print("Failed to close database: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db).decode('utf-8')) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    return 1  #Return this function error code.
## END Function


# The same as above function, except this time we return the names of each table
# in the database file.
def db_get_tablenames(db_file_name, db_tablenames):

    # Get the path to the SQLite3 shared library file (dll/so).
    f_library_sql3 = ozz_sql3.get_libsql3_path()  # Get the path for sqlite.dll/so

    # Load the sqlite3 shared library. The returned ID from loading the sqlite
    # shared library must be passed to other functions.
    id_lib_sql3  = ozz_sql3.load_libsql3(f_library_sql3)

    # sqlite3 *p_db;  # database handle (structure).
    p_db = ozz_sql3.p_sqlite3()  # Get the sqlite3 structure.

    # sqlite3_stmt *p_stmt;  # structure represents a single SQL statement
    p_stmt = ozz_sql3.p_sqlite3_stmt()  # Get the sqlite3_stmt structure.

    return_code = 0
    buffer = ""  # temp buffer [MAX 128 characters]

    table_count = 0

    p_zVfs = None  # can use NULL = None
    pzTail = None
    NULL = None

    return_code = ozz_sql3.sqlite3_open_v2( id_lib_sql3, db_file_name, p_db, SQLITE_OPEN_READONLY, p_zVfs )
    if return_code != SQLITE_OK:
        # Note: these print returns can be commented out if only the Bool return 0; is required.
        print("Can't open database: " + ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    sql_table_list = "SELECT * FROM sqlite_schema WHERE type='table' AND name NOT LIKE 'sqlite_%' ORDER BY 1;"  # sqlite_schema, sqlite_master

    return_code = ozz_sql3.sqlite3_prepare_v2(id_lib_sql3, p_db, sql_table_list, -1, p_stmt, pzTail)
    # On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    # is returned.
    if return_code != SQLITE_OK:
        # This is error handling code for the sqlite3_prepare_v2 function call.
        print("Failed to prepare data: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db).decode('utf-8')) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    # Loop through all of the tables.
    while ozz_sql3.sqlite3_step(id_lib_sql3, p_stmt) == SQLITE_ROW:
        db_tablenames.insert(table_count, ozz_sql3.sqlite3_column_text(id_lib_sql3, p_stmt, 1))
        table_count += 1  # Update the next array data element.

    #sqlite3_bind_*()  # After sqlit3_prepare_v2()
    #sqlite3_clear_bindings(stmt);

    # The sqlite3_finalize function destroys the prepared statement object.
    return_code = ozz_sql3.sqlite3_finalize(id_lib_sql3, p_stmt )  # Commit to the database.
    if return_code != SQLITE_OK:  # SQLITE_OK==0
        # This is error handling code.
        print("Failed to finalize data: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db).decode('utf-8')) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    # The sqlite3_close function closes the database connection.
    return_code = ozz_sql3.sqlite3_close(id_lib_sql3, p_db)   # SQLITE_OK==0
    if return_code != SQLITE_OK:
        # This is error handling code. NOTE! As p_db is closed the error code may not be available!
        print("Failed to close database: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db).decode('utf-8')) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    return 1  #Return this function error code.
## END Function



# Insert a table with column identifiers into a database.
# This will NOT create a database file if it does not already exist.
# Must use db_file_create()
# This will NOT overwrite a previous table of the same name if it exists.
# I may need to create a test or modify this function for safety.
def db_table_create(db_file_name, db_table):

    # Get the path to the SQLite3 shared library file (dll/so).
    f_library_sql3 = ozz_sql3.get_libsql3_path()  # Get the path for sqlite.dll/so

    # Load the sqlite3 shared library. The returned ID from loading the sqlite
    # shared library must be passed to other functions.
    id_lib_sql3  = ozz_sql3.load_libsql3(f_library_sql3)

    # sqlite3 *p_db;  # database handle (structure).
    p_db = ozz_sql3.p_sqlite3()  # Get the sqlite3 structure.

    # sqlite3_stmt *p_stmt;  # structure represents a single SQL statement
    p_stmt = ozz_sql3.p_sqlite3_stmt()  # Get the sqlite3_stmt structure.

    return_code = 0

    p_zVfs = None  # can use NULL = None
    pzTail = None
    NULL = None

    return_code = ozz_sql3.sqlite3_open_v2( id_lib_sql3, db_file_name, p_db, SQLITE_OPEN_READWRITE, p_zVfs )
    if return_code != SQLITE_OK:
        # Note: these print returns can be commented out if only the Bool return 0; is required.
        print("Can't open database: " + ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    # In this function I have formatted the entire query (statement) prior to
    # sending it to the function *p_db_table. As you will se in following
    # examples we can also create a generic template for the statement and
    # only send the column names and value data to be constructed into a full
    # query using string concatenation or sqlite3_bind*().

    #"CREATE TABLE IF NOT EXISTS TableName (Col_Name TYPE);"
    # Note: If the table exist already no error is returned.

    # We can only send one query at a time to sqlite3.
    return_code = ozz_sql3.sqlite3_prepare_v2(id_lib_sql3, p_db, db_table, -1, p_stmt, pzTail)
    # On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    # is returned.
    if return_code != SQLITE_OK:
        # This is error handling code for the sqlite3_prepare_v2 function call.
        print("Failed to prepare data: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    # The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    # that there is another row ready. Our SQL statement returns only one row
    # of data at a time.
    # If we are writing or reading multiple lines of table then we will need to
    # use sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.
    return_code = ozz_sql3.sqlite3_step(id_lib_sql3, p_stmt );  # run once for one statement
    #    while( sqlite3_step( p_stmt ) == SQLITE_ROW ) {;}
    if return_code != SQLITE_DONE:  # SQLITE_DONE==101, SQLITE_ROW==100
        print("Step failed: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return 0

    #sqlite3_bind_*()  # After sqlit3_prepare_v2()
    #sqlite3_clear_bindings(stmt);

    # The sqlite3_finalize function destroys the prepared statement object.
    return_code = ozz_sql3.sqlite3_finalize(id_lib_sql3, p_stmt )  # Commit to the database.
    if return_code != SQLITE_OK:  # SQLITE_OK==0
        # This is error handling code.
        print("Failed to finalize data: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    # The sqlite3_close function closes the database connection.
    return_code = ozz_sql3.sqlite3_close(id_lib_sql3, p_db)   # SQLITE_OK==0
    if return_code != SQLITE_OK:
        # This is error handling code. NOTE! As p_db is closed the error code may not be available!
        print("Failed to close database: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    print("Successfully created table in " + db_file_name, file=sys.stderr)  # DEBUG

    return 1
## END Function


# Delete (DROP) table from a named database file.
# Will return -1 if the database file does not exists.
def db_table_delete(db_file_name, db_table_name):

    # Get the path to the SQLite3 shared library file (dll/so).
    f_library_sql3 = ozz_sql3.get_libsql3_path()  # Get the path for sqlite.dll/so

    # Load the sqlite3 shared library. The returned ID from loading the sqlite
    # shared library must be passed to other functions.
    id_lib_sql3  = ozz_sql3.load_libsql3(f_library_sql3)

    # sqlite3 *p_db;  # database handle (structure).
    p_db = ozz_sql3.p_sqlite3()  # Get the sqlite3 structure.

    # sqlite3_stmt *p_stmt;  # structure represents a single SQL statement
    p_stmt = ozz_sql3.p_sqlite3_stmt()  # Get the sqlite3_stmt structure.

    return_code = 0

    p_zVfs = None  # can use NULL = None
    pzTail = None
    NULL = None

    return_code = ozz_sql3.sqlite3_open_v2( id_lib_sql3, db_file_name, p_db, SQLITE_OPEN_READWRITE, p_zVfs )
    if return_code != SQLITE_OK:
        # Note: these print returns can be commented out if only the Bool return 0; is required.
        print("Can't open database: " + ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    # In this function I have formatted the entire query (statement) prior to
    # sending it to the function *p_db_table. As you will see in following
    # examples we can also create a generic template for the statement and
    # only send the column names and value data to be constructed into a full
    # query using string concatenation or sqlite3_bind*().

    # "DROP TABLE IF EXISTS TableName;";
    # We can only send one query at a time to sqlite3.
    return_code = ozz_sql3.sqlite3_prepare_v2(id_lib_sql3, p_db, db_table_name, -1, p_stmt, pzTail)
    # On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    # is returned.
    if return_code != SQLITE_OK:
        # This is error handling code for the sqlite3_prepare_v2 function call.
        print("Failed to prepare data: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    # The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    # that there is another row ready. Our SQL statement returns only one row
    # of data at a time.
    # If we are writing or reading multiple lines of table then we will need to
    # use sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.
    return_code = ozz_sql3.sqlite3_step(id_lib_sql3, p_stmt)  # run once for one statement
    #    while( sqlite3_step( stmt ) == SQLITE_ROW ) {;}
    if return_code != SQLITE_DONE:  # SQLITE_DONE==101, SQLITE_ROW==100
        print("Step failed: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return 0

    #sqlite3_bind_*()  # After sqlit3_prepare_v2()
    #sqlite3_clear_bindings(stmt);

    # The sqlite3_finalize function destroys the prepared statement object.
    return_code = ozz_sql3.sqlite3_finalize(id_lib_sql3, p_stmt )  # Commit to the database.
    if return_code != SQLITE_OK:  # SQLITE_OK==0
        # This is error handling code.
        print("Failed to finalize data: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    # The sqlite3_close function closes the database connection.
    return_code = ozz_sql3.sqlite3_close(id_lib_sql3, p_db)   # SQLITE_OK==0
    if return_code != SQLITE_OK:
        # This is error handling code. NOTE! As p_db is closed the error code may not be available!
        print("Failed to close database: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)

    print("Successfully deleted table in " + db_file_name, file=sys.stderr)  # DEBUG

    return 1
## END Function


#============================================================================>>

# Get the number of rows from a table.
def db_get_table_number_rows(db_file_name, db_table_name, number_rows_ret):

    # Get the path to the SQLite3 shared library file (dll/so).
    f_library_sql3 = ozz_sql3.get_libsql3_path()  # Get the path for sqlite.dll/so

    # Load the sqlite3 shared library. The returned ID from loading the sqlite
    # shared library must be passed to other functions.
    id_lib_sql3  = ozz_sql3.load_libsql3(f_library_sql3)

    # sqlite3 *p_db;  # database handle (structure).
    p_db = ozz_sql3.p_sqlite3()  # Get the sqlite3 structure.

    # sqlite3_stmt *p_stmt;  # structure represents a single SQL statement
    p_stmt = ozz_sql3.p_sqlite3_stmt()  # Get the sqlite3_stmt structure.

    return_code = 0
    row_cnt = 0

    p_zVfs = None  # can use NULL = None
    pzTail = None
    NULL = None

    return_code = ozz_sql3.sqlite3_open_v2( id_lib_sql3, db_file_name, p_db, SQLITE_OPEN_READONLY, p_zVfs )
    if return_code != SQLITE_OK:
        # Note: these print returns can be commented out if only the Bool return 0; is required.
        print("Can't open database: " + ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    # Unlike previous functions, I am using a template to create the statement.
    # I am concatenating the additional information to form the full query
    # statement. This allows us to use a generic template providing only the
    # necessary data. See info in sql injection.

    # SELECT COUNT(*) FROM TableName;  # Returns rows. COUNT(*|ALL|DISTINCT] expression)
    # Alternative select max(rowid) from TableName;
    # See Column name search and counts for the column count usage.
    db_row_search = "SELECT COUNT(*) FROM " + db_table_name + ";"  # Don't use long table name max[128 - 22]

    # We can only send one query at a time to sqlite3.
    return_code = ozz_sql3.sqlite3_prepare_v2(id_lib_sql3, p_db, db_row_search, -1, p_stmt, pzTail)
    # On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    # is returned.
    if return_code != SQLITE_OK:
        # This is error handling code for the sqlite3_prepare_v2 function call.
        print("Failed to prepare data: " + ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    # The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    # that there is another row ready. Our SQL statement returns only one row
    # of data at a time.
    # If we are writing or reading multiple lines of table then we will need to
    # use sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.
    return_code = ozz_sql3.sqlite3_step(id_lib_sql3, p_stmt)
    if return_code != SQLITE_ROW:
        print("Step failed: " + ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db);
        return -1

    row_cnt = ozz_sql3.sqlite3_column_int(id_lib_sql3, p_stmt, 0)  # Retrieve the row count as int.
    # insert the return into element[0]
    number_rows_ret.insert(0, row_cnt)  # Our table count by reference.

    #sqlite3_bind_*()  # After sqlit3_prepare_v2()
    #sqlite3_clear_bindings(stmt);

    # The sqlite3_finalize function destroys the prepared statement object.
    return_code = ozz_sql3.sqlite3_finalize(id_lib_sql3, p_stmt )  # Commit to the database.
    if return_code != SQLITE_OK:  # SQLITE_OK==0
        # This is error handling code.
        print("Failed to finalize data: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    # The sqlite3_close function closes the database connection.
    return_code = ozz_sql3.sqlite3_close(id_lib_sql3, p_db)   # SQLITE_OK==0
    if return_code != SQLITE_OK:
        # This is error handling code. NOTE! As p_db is closed the error code may not be available!
        print("Failed to close database: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)

    print("Successfully retrieved table row number from " + db_file_name, file=sys.stderr)  # DEBUG

    return 1
## END Function


# Get number of columns in a TableName.
def db_get_table_number_cols(db_file_name, db_table_name, number_cols_ret):

    # Get the path to the SQLite3 shared library file (dll/so).
    f_library_sql3 = ozz_sql3.get_libsql3_path()  # Get the path for sqlite.dll/so

    # Load the sqlite3 shared library. The returned ID from loading the sqlite
    # shared library must be passed to other functions.
    id_lib_sql3  = ozz_sql3.load_libsql3(f_library_sql3)

    # sqlite3 *p_db;  # database handle (structure).
    p_db = ozz_sql3.p_sqlite3()  # Get the sqlite3 structure.

    # sqlite3_stmt *p_stmt;  # structure represents a single SQL statement
    p_stmt = ozz_sql3.p_sqlite3_stmt()  # Get the sqlite3_stmt structure.

    return_code = 0
    col_cnt = 0

    p_zVfs = None  # can use NULL = None
    pzTail = None
    NULL = None

    return_code = ozz_sql3.sqlite3_open_v2( id_lib_sql3, db_file_name, p_db, SQLITE_OPEN_READONLY, p_zVfs )
    if return_code != SQLITE_OK:
        # Note: these print returns can be commented out if only the Bool return 0; is required.
        print("Can't open database: " + ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    db_col_search = "SELECT COUNT(*) FROM pragma_table_info(\"" + db_table_name + "\");"  # Don't use long table name max[128 - 43]

    # We can only send one query at a time to sqlite3.
    return_code = ozz_sql3.sqlite3_prepare_v2(id_lib_sql3, p_db, db_col_search, -1, p_stmt, pzTail)
    # On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    # is returned.
    if return_code != SQLITE_OK:
        # This is error handling code for the sqlite3_prepare_v2 function call.
        print("Failed to prepare data: " + ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    # The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    # that there is another row ready. Our SQL statement returns only one row
    # of data at a time.
    # If we are writing or reading multiple lines of table then we will need to
    # use sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.
    return_code = ozz_sql3.sqlite3_step(id_lib_sql3, p_stmt)
    if return_code != SQLITE_ROW:
        fprintf(stderr, "Step failed: " + ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    col_cnt = ozz_sql3.sqlite3_column_int(id_lib_sql3, p_stmt, 0)  # Retrieve the col count as int.
    # insert the return into element[0]
    number_cols_ret.insert(0, col_cnt)  # Our table count by reference.

    #sqlite3_bind_*()  # After sqlit3_prepare_v2()
    #sqlite3_clear_bindings(stmt);

    # The sqlite3_finalize function destroys the prepared statement object.
    return_code = ozz_sql3.sqlite3_finalize(id_lib_sql3, p_stmt )  # Commit to the database.
    if return_code != SQLITE_OK:  # SQLITE_OK==0
        # This is error handling code.
        print("Failed to finalize data: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    # The sqlite3_close function closes the database connection.
    return_code = ozz_sql3.sqlite3_close(id_lib_sql3, p_db)   # SQLITE_OK==0
    if return_code != SQLITE_OK:
        # This is error handling code. NOTE! As p_db is closed the error code may not be available!
        print("Failed to close database: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)

    print("Successfully retrieved table column number from " + db_file_name, file=sys.stderr)  # DEBUG

    return 1
## END Function


# Get the column names as 2D array from a named table.
def db_get_table_colnames(db_file_name, db_table_name, db_tbl_col_name):

    # Get the path to the SQLite3 shared library file (dll/so).
    f_library_sql3 = ozz_sql3.get_libsql3_path()  # Get the path for sqlite.dll/so

    # Load the sqlite3 shared library. The returned ID from loading the sqlite
    # shared library must be passed to other functions.
    id_lib_sql3  = ozz_sql3.load_libsql3(f_library_sql3)

    # sqlite3 *p_db;  # database handle (structure).
    p_db = ozz_sql3.p_sqlite3()  # Get the sqlite3 structure.

    # sqlite3_stmt *p_stmt;  # structure represents a single SQL statement
    p_stmt = ozz_sql3.p_sqlite3_stmt()  # Get the sqlite3_stmt structure.

    return_code = 0
    col_cnt = 0

    p_zVfs = None  # can use NULL = None
    pzTail = None
    NULL = None

    return_code = ozz_sql3.sqlite3_open_v2( id_lib_sql3, db_file_name, p_db, SQLITE_OPEN_READONLY, p_zVfs )
    if return_code != SQLITE_OK:
        # Note: these print returns can be commented out if only the Bool return 0; is required.
        print("Can't open database: " + ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    # There are other methods to do this, so this statement can be revised.
    db_col_search = "PRAGMA table_info('" + db_table_name + "');"  # Don't use long table name max[128 - 22]

    # We can only send one query at a time to sqlite3.
    return_code = ozz_sql3.sqlite3_prepare_v2(id_lib_sql3, p_db, db_col_search, -1, p_stmt, pzTail)
    # On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    # is returned.
    if return_code != SQLITE_OK:
        # This is error handling code for the sqlite3_prepare_v2 function call.
        print("Failed to prepare data: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    col_cnt = 0  # Count number of columns.

    # The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    # that there is another row ready. Our SQL statement returns only one row
    # of data at a time.
    # If we are writing or reading multiple lines of table then we will need to
    # use sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.
    while ozz_sql3.sqlite3_step(id_lib_sql3, p_stmt) != SQLITE_DONE:

        # To handle the return of NULL pointers as a string
        # data = (const char*)sqlite3_column_text( stmt, i );
        #        printf( "%s\n", data ? data : "[NULL]" );
        #        sprintf(buffer[strlen(data)], "%s\n", data ? data : "[NULL]" );

        # insert the return into element[col_cnt] | Our table count by reference as list.
        db_tbl_col_name.insert(col_cnt, ozz_sql3.sqlite3_column_text(id_lib_sql3, p_stmt, 1))
        col_cnt += 1

    #sqlite3_bind_*()  # After sqlit3_prepare_v2()
    #sqlite3_clear_bindings(stmt);

    # The sqlite3_finalize function destroys the prepared statement object.
    # The sqlite3_finalize function destroys the prepared statement object.
    return_code = ozz_sql3.sqlite3_finalize(id_lib_sql3, p_stmt )  # Commit to the database.
    if return_code != SQLITE_OK:  # SQLITE_OK==0
        # This is error handling code.
        print("Failed to finalize data: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    # The sqlite3_close function closes the database connection.
    return_code = ozz_sql3.sqlite3_close(id_lib_sql3, p_db)   # SQLITE_OK==0
    if return_code != SQLITE_OK:
        # This is error handling code. NOTE! As p_db is closed the error code may not be available!
        print("Failed to close database: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)

    print("Successfully retrieved table column names from " + db_file_name, file=sys.stderr)  # DEBUG

    return 1
## END Function

#============================================================================<<


# Insert row data into a named table.
# If the data already exists will create a new row.
# Table unique index rowid is auto generated to the next available position
# in this table.
def db_insert_table_rowdata(db_file_name, db_tbl_entry): # add insert at row_id

    # Get the path to the SQLite3 shared library file (dll/so).
    f_library_sql3 = ozz_sql3.get_libsql3_path()  # Get the path for sqlite.dll/so

    # Load the sqlite3 shared library. The returned ID from loading the sqlite
    # shared library must be passed to other functions.
    id_lib_sql3  = ozz_sql3.load_libsql3(f_library_sql3)

    # sqlite3 *p_db;  # database handle (structure).
    p_db = ozz_sql3.p_sqlite3()  # Get the sqlite3 structure.

    # sqlite3_stmt *p_stmt;  # structure represents a single SQL statement
    p_stmt = ozz_sql3.p_sqlite3_stmt()  # Get the sqlite3_stmt structure.

    return_code = 0

    p_zVfs = None  # can use NULL = None
    pzTail = None
    NULL = None

    return_code = ozz_sql3.sqlite3_open_v2( id_lib_sql3, db_file_name, p_db, SQLITE_OPEN_READWRITE, p_zVfs )
    if return_code != SQLITE_OK:
        # Note: these print returns can be commented out if only the Bool return 0; is required.
        print("Can't open database: " + ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    # In this function I am supplying to full query statement to the function.
    # You can alter this based upon other function using templates :)

    # We can only send one query at a time to sqlite3.
    return_code = ozz_sql3.sqlite3_prepare_v2(id_lib_sql3, p_db, db_tbl_entry, -1, p_stmt, pzTail)
    # On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    # is returned.
    if return_code != SQLITE_OK:
        # This is error handling code for the sqlite3_prepare_v2 function call.
        print("Failed to prepare data: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    # The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    # that there is another row ready. Our SQL statement returns only one row
    # of data at a time.
    # If we are writing or reading multiple lines of table then we will need to
    # use sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.
    return_code = ozz_sql3.sqlite3_step(id_lib_sql3, p_stmt);  # run once for one statement
    #    while( sqlite3_step( stmt ) == SQLITE_ROW ) {;}
    if return_code != SQLITE_DONE:  # SQLITE_DONE==101, SQLITE_ROW==100
        fprintf(stderr, "Step failed: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return 0

    #sqlite3_bind_*()  # After sqlit3_prepare_v2()
    #sqlite3_clear_bindings(stmt);

    # The sqlite3_finalize function destroys the prepared statement object.
    return_code = ozz_sql3.sqlite3_finalize(id_lib_sql3, p_stmt )  # Commit to the database.
    if return_code != SQLITE_OK:  # SQLITE_OK==0
        # This is error handling code.
        print("Failed to finalize data: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    # The sqlite3_close function closes the database connection.
    return_code = ozz_sql3.sqlite3_close(id_lib_sql3, p_db)   # SQLITE_OK==0
    if return_code != SQLITE_OK:
        # This is error handling code. NOTE! As p_db is closed the error code may not be available!
        print("Failed to close database: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)

    print( "Successfully inserted rowdata into table in " + db_file_name, file=sys.stderr)  # DEBUG

    return 1
## END Function


# Delete the row data searched by name matching etc.
# ## Only for tables where all field types are TEXT (String) except rowid. ##
# This function is dangerous and needs to be revised!!!!
def db_delete_table_rowdata(db_file_name, db_row_entry): # add delete at row_id

    # Get the path to the SQLite3 shared library file (dll/so).
    f_library_sql3 = ozz_sql3.get_libsql3_path()  # Get the path for sqlite.dll/so

    # Load the sqlite3 shared library. The returned ID from loading the sqlite
    # shared library must be passed to other functions.
    id_lib_sql3  = ozz_sql3.load_libsql3(f_library_sql3)

    # sqlite3 *p_db;  # database handle (structure).
    p_db = ozz_sql3.p_sqlite3()  # Get the sqlite3 structure.

    # sqlite3_stmt *p_stmt;  # structure represents a single SQL statement
    p_stmt = ozz_sql3.p_sqlite3_stmt()  # Get the sqlite3_stmt structure.

    return_code = 0

    p_zVfs = None  # can use NULL = None
    pzTail = None
    NULL = None

    return_code = ozz_sql3.sqlite3_open_v2( id_lib_sql3, db_file_name, p_db, SQLITE_OPEN_READWRITE, p_zVfs )
    if return_code != SQLITE_OK:
        # Note: these print returns can be commented out if only the Bool return 0; is required.
        print("Can't open database: " + ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    # We can only send one query at a time to sqlite3.
    return_code = ozz_sql3.sqlite3_prepare_v2(id_lib_sql3, p_db, db_row_entry, -1, p_stmt, pzTail)
    # On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    # is returned.
    if return_code != SQLITE_OK:
        # This is error handling code for the sqlite3_prepare_v2 function call.
        print("Failed to prepare data: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    # The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    # that there is another row ready. Our SQL statement returns only one row
    # of data at a time.
    # If we are writing or reading multiple lines of table then we will need to
    # use sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.
    return_code = ozz_sql3.sqlite3_step(id_lib_sql3, p_stmt)  # run once for one statement
    #    while( sqlite3_step( stmt ) == SQLITE_ROW ) {;}
    if return_code != SQLITE_DONE:  # SQLITE_DONE==101, SQLITE_ROW==100
        print("Step failed: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return 0

    #sqlite3_bind_*()  # After sqlit3_prepare_v2()
    #sqlite3_clear_bindings(stmt);

    # The sqlite3_finalize function destroys the prepared statement object.
    return_code = ozz_sql3.sqlite3_finalize(id_lib_sql3, p_stmt )  # Commit to the database.
    if return_code != SQLITE_OK:  # SQLITE_OK==0
        # This is error handling code.
        print("Failed to finalize data: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    # The sqlite3_close function closes the database connection.
    return_code = ozz_sql3.sqlite3_close(id_lib_sql3, p_db)   # SQLITE_OK==0
    if return_code != SQLITE_OK:
        # This is error handling code. NOTE! As p_db is closed the error code may not be available!
        print("Failed to close database: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)

    print( "Successfully removed rowdata from table in " + db_file_name, file=sys.stderr)  # DEBUG

    return 1
## END Function



# List all data from a named table to a dynamic array.
# ## Only for tables where all field types are TEXT (String) except rowid. ##
def db_list_table_rows_data(db_file_name, db_table_name, db_tbl_rowdata, number_columns):

    # Get the path to the SQLite3 shared library file (dll/so).
    f_library_sql3 = ozz_sql3.get_libsql3_path()  # Get the path for sqlite.dll/so

    # Load the sqlite3 shared library. The returned ID from loading the sqlite
    # shared library must be passed to other functions.
    id_lib_sql3  = ozz_sql3.load_libsql3(f_library_sql3)

    # sqlite3 *p_db;  # database handle (structure).
    p_db = ozz_sql3.p_sqlite3()  # Get the sqlite3 structure.

    # sqlite3_stmt *p_stmt;  # structure represents a single SQL statement
    p_stmt = ozz_sql3.p_sqlite3_stmt()  # Get the sqlite3_stmt structure.

    return_code = 0

    number_columns += 1  # We are also including an extra column for the row ID number.

    p_zVfs = None  # can use NULL = None
    pzTail = None
    NULL = None

    return_code = ozz_sql3.sqlite3_open_v2( id_lib_sql3, db_file_name, p_db, SQLITE_OPEN_READONLY, p_zVfs )
    if return_code != SQLITE_OK:
        # Note: these print returns can be commented out if only the Bool return 0; is required.
        print("Can't open database: " + ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    sql_concat = ""

    # SELECT rowid, * FROM # To include table row ID number.
    sql = "SELECT rowid, * FROM "   # Note the space after FROM

    sql_concat = sql_concat + sql  # Add SQL query statement.
    sql_concat = sql_concat + db_table_name  # Add table name to statement.
    # aka sql = "SELECT rowid, * FROM " + db_table_name

    # char *p_db_table1 = "DROP TABLE IF EXISTS WFH_Tracker;";
    #"create table aTable(field1 int); drop table aTable;
    # We can only send one query at a time to sqlite3.
    return_code = ozz_sql3.sqlite3_prepare_v2(id_lib_sql3, p_db, sql_concat, -1, p_stmt, pzTail)
    # On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    # is returned.
    if return_code != SQLITE_OK:
        # This is error handling code for the sqlite3_prepare_v2 function call.
        print("Failed to prepare data: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    # The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    # that there is another row ready. Our SQL statement returns only one row
    # of data at a time, therefore, we call this function only once at a time.
    # If we are writing or reading multiple lines of table then we will need to
    # use sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.

    cnt_row = 0  # array row.
    cnt_col = 0
    buffer1 = ""  # temp buffer [MAX 128 characters]
    buffer2 = ""  # temp buffer [MAX 128 characters]

    while ozz_sql3.sqlite3_step(id_lib_sql3, p_stmt) == SQLITE_ROW:
        # Can use sqlite3_column_count(statement/stmt) in place of number_columns.
        for cnt_col in range(number_columns):  # Count 0 to number_columns-1
            # To handle the return of NULL pointers as a string.
            # data = (const char*)sqlite3_column_text( stmt, cnt_col );
            #        printf( "%s\n", data ? data : "[NULL]" );
            #        sprintf(buffer[strlen(data)], "%s\n", data ? data : "[NULL]" );

            # Copy each entry to a buffer. Each entry is a column.
            buffer1 = ozz_sql3.sqlite3_column_text(id_lib_sql3, p_stmt, cnt_col)  # cnt_col == array column.

            # Concatenate buffer1 to our buffer2.
            buffer2 = buffer2 + buffer1  # buffer2 holds each row...
            if cnt_col < number_columns -1: # Dont add ', ' after last column.
                buffer2 = buffer2 + ","  # add separator token between each col.
            else:  # Add line return, end of row.
                pass  #strcat(p_db_tbl_rowdata[cnt_row], "\n");

        db_tbl_rowdata.insert(cnt_row, buffer2)  # Copy the full row to the list.
        buffer2 = ""  # Clear old row data from the buffer for next concatenate.
        cnt_row += 1  # Next row...

    #sqlite3_bind_*()  # After sqlit3_prepare_v2()
    #sqlite3_clear_bindings(stmt);

    # The sqlite3_finalize function destroys the prepared statement object.
    return_code = ozz_sql3.sqlite3_finalize(id_lib_sql3, p_stmt )  # Commit to the database.
    if return_code != SQLITE_OK:  # SQLITE_OK==0
        # This is error handling code.
        print("Failed to finalize data: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    # The sqlite3_close function closes the database connection.
    return_code = ozz_sql3.sqlite3_close(id_lib_sql3, p_db)   # SQLITE_OK==0
    if return_code != SQLITE_OK:
        # This is error handling code. NOTE! As p_db is closed the error code may not be available!
        print("Failed to close database: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)

    print("Successfully retrieved table data from " + db_file_name, file=sys.stderr)  # DEBUG

    return 1
## END Function



# delete row by "rowid"
# Delete the row data by rowid.
def db_delete_table_rowdata_rowid(db_file_name, db_table_name, sql_rowid):

    # Get the path to the SQLite3 shared library file (dll/so).
    f_library_sql3 = ozz_sql3.get_libsql3_path()  # Get the path for sqlite.dll/so

    # Load the sqlite3 shared library. The returned ID from loading the sqlite
    # shared library must be passed to other functions.
    id_lib_sql3  = ozz_sql3.load_libsql3(f_library_sql3)

    # sqlite3 *p_db;  # database handle (structure).
    p_db = ozz_sql3.p_sqlite3()  # Get the sqlite3 structure.

    # sqlite3_stmt *p_stmt;  # structure represents a single SQL statement
    p_stmt = ozz_sql3.p_sqlite3_stmt()  # Get the sqlite3_stmt structure.

    return_code = 0

    p_zVfs = None  # can use NULL = None
    pzTail = None
    NULL = None

    return_code = ozz_sql3.sqlite3_open_v2( id_lib_sql3, db_file_name, p_db, SQLITE_OPEN_READWRITE, p_zVfs )
    if return_code != SQLITE_OK:
        # Note: these print returns can be commented out if only the Bool return 0; is required.
        print("Can't open database: " + ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    #"DELETE FROM TableName WHERE rowid = n;"
    sql1 = "DELETE FROM "
    sql2 = " WHERE rowid = "
    sql3 = ";"  #

    # Concatenate our query statement and data.
    sql_concat = sql1 + db_table_name + sql2 + str(sql_rowid) + sql3

    # We can only send one query at a time to sqlite3.
    return_code = ozz_sql3.sqlite3_prepare_v2(id_lib_sql3, p_db, sql_concat, -1, p_stmt, pzTail)
    # On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    # is returned.
    if return_code != SQLITE_OK:
        # This is error handling code for the sqlite3_prepare_v2 function call.
        print("Failed to prepare data: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    # The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    # that there is another row ready. Our SQL statement returns only one row
    # of data at a time, therefore, we call this function only once at a time.
    # If we are writing or reading multiple lines of table then we will need to
    # use sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.
    return_code = ozz_sql3.sqlite3_step(id_lib_sql3, p_stmt )  # run once for one statement
    #    while( sqlite3_step( stmt ) == SQLITE_ROW ) {;}
    if return_code != SQLITE_DONE:  # SQLITE_DONE==101, SQLITE_ROW==100
        print("Step failed: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return 0

    #sqlite3_bind_*()  # After sqlit3_prepare_v2()
    #sqlite3_clear_bindings(stmt);

    # The sqlite3_finalize function destroys the prepared statement object.
    return_code = ozz_sql3.sqlite3_finalize(id_lib_sql3, p_stmt )  # Commit to the database.
    if return_code != SQLITE_OK:  # SQLITE_OK==0
        # This is error handling code.
        print("Failed to finalize data: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    # The sqlite3_close function closes the database connection.
    return_code = ozz_sql3.sqlite3_close(id_lib_sql3, p_db)   # SQLITE_OK==0
    if return_code != SQLITE_OK:
        # This is error handling code. NOTE! As p_db is closed the error code may not be available!
        print("Failed to close database: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)

    print( "Successfully removed rowdata from table in " + db_file_name, file=sys.stderr)  # DEBUG

    return 1
## END Function


# update/replace row data by rowid in a named table.
# If the data already exists it will "overwrite" the row.
def db_replace_table_rowdata_rowid(db_file_name, db_table_name, sql_rowid, db_field_names, db_field_values):

    # Get the path to the SQLite3 shared library file (dll/so).
    f_library_sql3 = ozz_sql3.get_libsql3_path()  # Get the path for sqlite.dll/so

    # Load the sqlite3 shared library. The returned ID from loading the sqlite
    # shared library must be passed to other functions.
    id_lib_sql3  = ozz_sql3.load_libsql3(f_library_sql3)

    # sqlite3 *p_db;  # database handle (structure).
    p_db = ozz_sql3.p_sqlite3()  # Get the sqlite3 structure.

    # sqlite3_stmt *p_stmt;  # structure represents a single SQL statement
    p_stmt = ozz_sql3.p_sqlite3_stmt()  # Get the sqlite3_stmt structure.

    return_code = 0

    p_zVfs = None  # can use NULL = None
    pzTail = None
    NULL = None

    return_code = ozz_sql3.sqlite3_open_v2( id_lib_sql3, db_file_name, p_db, SQLITE_OPEN_READWRITE, p_zVfs )
    if return_code != SQLITE_OK:
        # Note: these print returns can be commented out if only the Bool return 0; is required.
        print("Can't open database: " + ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    # "Week, Employee_ID, Name, Monday, Tuesday, Wednesday, Thursday, Friday";
    # "\"2\", \"36\", \"Bill Knight\", \"2\", \"3\", \"4\", \"5\", \"6\"";

    # Our template for building the query statement.
    sql1 = "REPLACE INTO "
    sql2 = " (rowid, "
    sql3 = ") VALUES("
    sql4 = ", "
    sql5 = ");"

    # Concatenate our query statement and data.
    sql_concat = sql1 + db_table_name + sql2 + db_field_names + sql3 + str(sql_rowid) + sql4 + db_field_values + sql5

    # We can only send one query at a time to sqlite3.
    return_code = ozz_sql3.sqlite3_prepare_v2(id_lib_sql3, p_db, sql_concat, -1, p_stmt, pzTail)
    # On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    # is returned.
    if return_code != SQLITE_OK:
        # This is error handling code for the sqlite3_prepare_v2 function call.
        print("Failed to prepare data: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    # The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    # that there is another row ready. Our SQL statement returns only one row
    # of data at a time, therefore, we call this function only once at a time.
    # If we are writing or reading multiple lines of table then we will need to
    # use sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.
    return_code = ozz_sql3.sqlite3_step(id_lib_sql3, p_stmt)  # run once for one statement
    #    while( sqlite3_step( p_stmt ) == SQLITE_ROW ) {;}
    if return_code != SQLITE_DONE:  # SQLITE_DONE==101, SQLITE_ROW==100
        fprintf(stderr, "Step failed: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return 0

    #sqlite3_bind_*()  # After sqlit3_prepare_v2()
    #sqlite3_clear_bindings(stmt);

    # The sqlite3_finalize function destroys the prepared statement object.
    return_code = ozz_sql3.sqlite3_finalize(id_lib_sql3, p_stmt )  # Commit to the database.
    if return_code != SQLITE_OK:  # SQLITE_OK==0
        # This is error handling code.
        print("Failed to finalize data: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    # The sqlite3_close function closes the database connection.
    return_code = ozz_sql3.sqlite3_close(id_lib_sql3, p_db)   # SQLITE_OK==0
    if return_code != SQLITE_OK:
        # This is error handling code. NOTE! As p_db is closed the error code may not be available!
        print("Failed to close database: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)

    print( "Successfully inserted rowdata into table in " + db_file_name, file=sys.stderr)  # DEBUG

    return 1
## END Function


# Insert row data into a named table at rowid.
# The current data at the rowid is written into a temporary buffer, the new row
# data is then written to that rowid.
# Each row is "shuffled" down one rowid at at time until the last rowid.
# The final row of data is then inserted into a new (empty) rowid at the end
# of the table.
#
# Note this is considered poor practice in data base management as data is
# generally not stored in any particular order and it is up to the calling
# application to sort the data according to rules such as "By date" or "By Name".
#
# In a large data base file this will be a slow task and could be prone to errors.
# It can also disrupt the association of the rowid between different tables
# in a relational database.

# Table unique index rowid is auto generated in this table.
def db_insert_table_rowdata_rowid(db_file_name, db_table_name, sql_rowid, db_field_names, db_field_values, number_columns, number_rows):

    # Get the path to the SQLite3 shared library file (dll/so).
    f_library_sql3 = ozz_sql3.get_libsql3_path()  # Get the path for sqlite.dll/so

    # Load the sqlite3 shared library. The returned ID from loading the sqlite
    # shared library must be passed to other functions.
    id_lib_sql3  = ozz_sql3.load_libsql3(f_library_sql3)

    # sqlite3 *p_db;  # database handle (structure).
    p_db = ozz_sql3.p_sqlite3()  # Get the sqlite3 structure.

    # sqlite3_stmt *p_stmt;  # structure represents a single SQL statement
    p_stmt = ozz_sql3.p_sqlite3_stmt()  # Get the sqlite3_stmt structure.

    return_code = 0

    p_zVfs = None  # can use NULL = None
    pzTail = None
    NULL = None

    Count_Rows = sql_rowid  # start counting from the first insert rowid.

    #buffer = ""  # Temp column read buffer [MAX 128 characters]
    # This 2 dimension array will hold the temporary data for the rows being read
    # and then writen to the next line.
    db_field_values_temp = [None] * 2  # buffer to hold each line to move down +1

    # This is the new data (row) to be inserted at the rowid.
    db_field_values_temp[0] = db_field_values  # Copy first (new row) to R/W buffer.

    sql_concat = ""  # temp buffer [MAX 512 characters]
    rowid_buffer = 0  # convert int to char for statement concat.

    return_code = ozz_sql3.sqlite3_open_v2( id_lib_sql3, db_file_name, p_db, SQLITE_OPEN_READWRITE, p_zVfs )
    if return_code != SQLITE_OK:
        # Note: these print returns can be commented out if only the Bool return 0; is required.
        print("Can't open database: " + ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    # Starts at rowid up to last row + 1. +2 alows for the '<' ie. +1
    while Count_Rows < number_rows +2:

##===================================================== Start step 1 read rowid
        # rowid copy to string
        rowid_buffer = str(sql_rowid) # convert int to str

        # The +1 at the end of the table is empty and will create a read error,
        # so we will skip this and go directly to writing the last +1 new line
        # directly from the buffer.
        if Count_Rows < number_rows +1:
            sql_concat = ""  # Clear the buffer
            # "SELECT rowid, * FROM "  # Note that I am ignoring the rowid
            sql1 = "SELECT * FROM "  # Note the space after FROM
            sql2 = " WHERE rowid =  "
            sql3 = ";"

            # Add SQL query statement, Add table name to statement, Add filter,
            # This is the first replace rowid, Finish the sql statement.
            sql_concat = sql1 + db_table_name + sql2 + rowid_buffer + sql3

            # We can only send one query at a time to sqlite3.
            return_code = ozz_sql3.sqlite3_prepare_v2(id_lib_sql3, p_db, sql_concat, -1, p_stmt, pzTail)
            # On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
            # is returned.
            if return_code != SQLITE_OK:
                # This is error handling code for the sqlite3_prepare_v2 function call.
                print("Failed to prepare data: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
                ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
                return -1

            buffer = ""  # Clear the buffer for the next row.
            # All of the data is collected in a single string with each column
            # separated with the ',' delimiter.

            # ### This needs to be revised for error handling ###
            # I may need to change this to while (1) and
            # if (return_code == SQLITE_ROW){}
            #else if (return_code == SQLITE_DONE || return_code == SQLITE_DONE){ break;}
            # else {error return;}
            while ozz_sql3.sqlite3_step(id_lib_sql3, p_stmt) == SQLITE_ROW:
                #while (1)

                #return_code = sqlite3_step(stmt);
                #if (return_code == SQLITE_ROW)
                #{
                # Can use sqlite3_column_count(statement/stmt) in place of number_columns.
                for i in range(number_columns):  # Count 0 to number_columns-1
                #for (int i = 0; i < number_columns; i++)  # Count 0 to number_columns-1

                    # To handle the return of NULL pointers as a string
                    # data = (const char*)sqlite3_column_text( stmt, i );
                    #        printf( "%s\n", data ? data : "[NULL]" );
                    #        sprintf(buffer[strlen(data)], "%s\n", data ? data : "[NULL]" );

                    # copy (concat) each entry to a buffer. Each entry is a col.
                    # This is a little different to the C and FB version. The buffer
                    # is concatenated then copied as a row to the 2 element list.
                    buffer = buffer + "\""
                    buffer = buffer + ozz_sql3.sqlite3_column_text(id_lib_sql3, p_stmt, i)

                    # concat buffer to our return array.
                    # Here we are reading the row into [1] of the temporary R/W buffer.
                    buffer = buffer + "\""

                    if i < number_columns -1: # Don't add ', ' after last column.
                        buffer = buffer + ","
                    else:  # Add line return, end of row.
                        pass  #strcat(p_db_tbl_rowdata[cnt_row], "\n");
                    ## END for
                db_field_values_temp[1] = buffer
                #buffer = ""  # Clear the buffer for the next row.

                #}
                #else if ((return_code == SQLITE_DONE) || (return_code == SQLITE_OK))  # ???? SQLITE_OK
                #    {
                # No more rows to read, but we need to write the last row at this rowid!
                #send flag or continue past sqlite3_finalize.
                #    Flag_More_Rows = 0;
                #    break;
                #    }
                #else
                #    {
                # Unknown error!!!
                # Create error return code!
                #Flag_More_Rows = 0;
                #    break;
                #    }
                ## END while

            #sqlite3_bind_*()  # After sqlit3_prepare_v2()
            #sqlite3_clear_bindings(stmt);

            # The sqlite3_finalize function destroys the prepared statement object.
            return_code = ozz_sql3.sqlite3_finalize(id_lib_sql3, p_stmt )  # Commit to the database.
            if return_code != SQLITE_OK:  # SQLITE_OK==0
                # This is error handling code.
                print("Failed to finalize data: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
                ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
                return -1

            ## END Skip reading +1 empty line (if Count_Rows < number_rows +1:)
##=============================================================== end step 1
##==================================================== Start step 2. Write rowid

        # Clear the buffer from the last statement. strcat() will concat on to
        # the previous statement otherwise.
        sql_concat = ""

        sql01a = "REPLACE INTO "
        sql01b = "INSERT INTO "
        sql02a = " (rowid, "
        sql02b = " ("
        sql03 = ") VALUES("
        sql04 = ", "
        sql05 = ");"

        # Some logic to handle the last row inserted to a new rowid.
        if (Count_Rows < number_rows +2):
            sql_concat = sql_concat + sql01a  # "REPLACE INTO "
        else:  # New row (original last row +1)
            sql_concat = sql_concat + sql01b  # "INSERT INTO "

        sql_concat = sql_concat + db_table_name  # Add table name to statement.

        if (Count_Rows < number_rows +2):
            sql_concat = sql_concat + sql02a  # " (rowid, "
        else:
            sql_concat = sql_concat + sql02b  # " ("

        sql_concat = sql_concat + db_field_names  # Add feild name (column name).

        if (Count_Rows < number_rows +2):
            sql_concat = sql_concat + sql03   # ") VALUES(\""
            sql_concat = sql_concat + rowid_buffer  # This is the fist rowid for new values [As SQL INTEGER]
            sql_concat = sql_concat + sql04  #  ", " delimit rowid, col_values, ...
        else:
            sql_concat = sql_concat + sql03  # ") VALUES("

        # Add the last row read into the same rowid
        sql_concat = sql_concat + db_field_values_temp[0]  # Add the values.
        sql_concat = sql_concat + sql05  # Finish the statement with ");"

        # We can only send one query at a time to sqlite3.
        return_code = ozz_sql3.sqlite3_prepare_v2(id_lib_sql3, p_db, sql_concat, -1, p_stmt, pzTail)
        # On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
        # is returned.
        if return_code != SQLITE_OK:
            # This is error handling code for the sqlite3_prepare_v2 function call.
            print("XFailed to prepare data: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
            ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
            return -1

        # The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
        # that there is another row ready. Our SQL statement returns only one row
        # of data at a time, therefore, we call this function only once at a time.
        # If we are writing or reading multiple lines of table then we will need to
        # use sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.
        return_code = ozz_sql3.sqlite3_step(id_lib_sql3, p_stmt)  # run once for one statement
        #    while( sqlite3_step( stmt ) == SQLITE_ROW ) {;}
        if return_code != SQLITE_DONE:  # SQLITE_DONE==101, SQLITE_ROW==100
            print("Step failed: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
            ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
            return 0

        #sqlite3_bind_*()  # After sqlit3_prepare_v2()
        #sqlite3_clear_bindings(stmt);

        # The sqlite3_finalize function destroys the prepared statement object.
        return_code = ozz_sql3.sqlite3_finalize(id_lib_sql3, p_stmt )  # Commit to the database.
        if return_code != SQLITE_OK:  # SQLITE_OK==0
            # This is error handling code.
            print("Failed to finalize data: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
            ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
            return -1

        # Copy the last read [1] back to position [0] for next read write cycle.
        # This works a little like a last in first out buffer (LIFO).
        db_field_values_temp[0] = db_field_values_temp[1]
        sql_rowid += 1  # Start read/write next rowid..
        Count_Rows += 1  # So we don't count past the existing rows in the table.

        ## End while Continue loop until last table row. (while Count_Rows < number_rows +2:)
##================================================================== End step 2

    # The sqlite3_close function closes the database connection.
    return_code = ozz_sql3.sqlite3_close(id_lib_sql3, p_db)   # SQLITE_OK==0
    if return_code != SQLITE_OK:
        # This is error handling code. NOTE! As p_db is closed the error code may not be available!
        print("Failed to close database: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)

    print( "Successfully inserted rowdata into table in " + db_file_name, file=sys.stderr)  # DEBUG

    return 1
## END Function


# Read row from rowid in named table.
def db_read_table_rowdata_rowid(db_file_name, db_table_name, sql_rowid, db_tbl_rowid_data, number_columns):

    # Get the path to the SQLite3 shared library file (dll/so).
    f_library_sql3 = ozz_sql3.get_libsql3_path()  # Get the path for sqlite.dll/so

    # Load the sqlite3 shared library. The returned ID from loading the sqlite
    # shared library must be passed to other functions.
    id_lib_sql3  = ozz_sql3.load_libsql3(f_library_sql3)

    # sqlite3 *p_db;  # database handle (structure).
    p_db = ozz_sql3.p_sqlite3()  # Get the sqlite3 structure.

    # sqlite3_stmt *p_stmt;  # structure represents a single SQL statement
    p_stmt = ozz_sql3.p_sqlite3_stmt()  # Get the sqlite3_stmt structure.

    return_code = 0

    p_zVfs = None  # can use NULL = None
    pzTail = None
    NULL = None

    sql_concat = ""  # temp buffer [MAX 512 characters]
    buffer = ""  # Temp column read buffer [MAX 128 characters]

    return_code = ozz_sql3.sqlite3_open_v2( id_lib_sql3, db_file_name, p_db, SQLITE_OPEN_READONLY, p_zVfs )
    if return_code != SQLITE_OK:
        # Note: these print returns can be commented out if only the Bool return 0; is required.
        print("Can't open database: " + ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    # rowid copy to string
    rowid_buffer = str(sql_rowid)  # convert int to char for statement concat.

    # strcat() must be proceeded by a strcpy, We must also clear the
    #buffer with an empty string with "" for each read iteration.
    #strcpy(sql_concat, "");
    #char *sql1 = "SELECT rowid, * FROM ";  # Note the space after FROM
    sql1 = "SELECT * FROM "  # Note the space after FROM
    sql2 = " WHERE rowid =  "  #
    sql3 = ";"  #

    # Add SQL query statement, Add table name to statement, Add filter,
    # rowid to read from, Finish the sql statement
    sql_concat = sql1 + db_table_name + sql2 + rowid_buffer + sql3

    # We can only send one query at a time to sqlite3.
    return_code = ozz_sql3.sqlite3_prepare_v2(id_lib_sql3, p_db, sql_concat, -1, p_stmt, pzTail)
    # On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    # is returned.
    if return_code != SQLITE_OK:
        # This is error handling code for the sqlite3_prepare_v2 function call.
        print("Failed to prepare data: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    while ozz_sql3.sqlite3_step(id_lib_sql3, p_stmt) == SQLITE_ROW:
        # Can use sqlite3_column_count(statement/stmt) in place of number_columns.
        for i in range(number_columns):  # Count 0 to number_columns-1
            # To handle the return of NULL pointers as a string
            # data = (const char*)sqlite3_column_text( stmt, i );
            #        printf( "%s\n", data ? data : "[NULL]" );
            #        sprintf(buffer[strlen(data)], "%s\n", data ? data : "[NULL]" );

            # copy each entry to a buffer. Each entry is a col.
            buffer = buffer + ozz_sql3.sqlite3_column_text(id_lib_sql3, p_stmt, i)  # i == array column.
            #printf("DEBUG %s\n", buffer);
            # concat buffer to our return array.

            if i < number_columns -1: # Don't add ', ' after last column.
                # Remove space after ','
                buffer = buffer +  ","  # add separator token between each col.
            else:  # Add line return, end of row.
                pass  #strcat(p_db_tbl_rowdata[cnt_row], "\n");
            ## END for
        db_tbl_rowid_data.insert(0, buffer)
        ## END while

    #sqlite3_bind_*()  # After sqlit3_prepare_v2()
    #sqlite3_clear_bindings(stmt);

    # The sqlite3_finalize function destroys the prepared statement object.
    return_code = ozz_sql3.sqlite3_finalize(id_lib_sql3, p_stmt )  # Commit to the database.
    if return_code != SQLITE_OK:  # SQLITE_OK==0
        # This is error handling code.
        print("Failed to finalize data: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    # The sqlite3_close function closes the database connection.
    return_code = ozz_sql3.sqlite3_close(id_lib_sql3, p_db)   # SQLITE_OK==0
    if return_code != SQLITE_OK:
        # This is error handling code. NOTE! As p_db is closed the error code may not be available!
        print("Failed to close database: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)

    print( "Successfully read rowdata from table in " + db_file_name, file=sys.stderr)  # DEBUG

    return 1
## END Function


# Search for a string in a field name and return array of found rows.
# As this only searches a single column we should not encounter duplicate rowid.
# ## Only for tables where all field types are TEXT (String) except rowid. ##
def db_search_table_rowdata_byfield(db_file_name, db_table_name, db_tbl_row_search, field_name, db_search_string, number_columns, ret_array_length):

    # Get column field names as array[][].
    # get number of columns.
    # return row length (number of array items).

    # Get the path to the SQLite3 shared library file (dll/so).
    f_library_sql3 = ozz_sql3.get_libsql3_path()  # Get the path for sqlite.dll/so

    # Load the sqlite3 shared library. The returned ID from loading the sqlite
    # shared library must be passed to other functions.
    id_lib_sql3  = ozz_sql3.load_libsql3(f_library_sql3)

    # sqlite3 *p_db;  # database handle (structure).
    p_db = ozz_sql3.p_sqlite3()  # Get the sqlite3 structure.

    # sqlite3_stmt *p_stmt;  # structure represents a single SQL statement
    p_stmt = ozz_sql3.p_sqlite3_stmt()  # Get the sqlite3_stmt structure.

    return_code = 0

    p_zVfs = None  # can use NULL = None
    pzTail = None
    NULL = None

    return_code = ozz_sql3.sqlite3_open_v2( id_lib_sql3, db_file_name, p_db, SQLITE_OPEN_READONLY, p_zVfs )
    if return_code != SQLITE_OK:
        # Note: these print returns can be commented out if only the Bool return 0; is required.
        print("Can't open database: " + ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    sql_concat = ""  # Not required.

    sql1 = "SELECT rowid, * FROM "  # Note the space after FROM
    sql2 = " WHERE "  # Note the space before and after WHERE
    sql3 = " = "  # Note the space before and after =
    sql4 = ";"

    # Add SQL query statement, Add table name to statement, Add last part of query statement,
    # Add field name (column name), add " = ", Add last part of query statement,
    # Finish the statement with ";".
    sql_concat = sql1 + db_table_name + sql2 + field_name + sql3 + db_search_string + sql4

    # We can only send one query at a time to sqlite3.
    return_code = ozz_sql3.sqlite3_prepare_v2(id_lib_sql3, p_db, sql_concat, -1, p_stmt, pzTail)
    # On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    # is returned.
    if return_code != SQLITE_OK:
        # This is error handling code for the sqlite3_prepare_v2 function call.
        print("Failed to prepare data: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    # The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    # that there is another row ready. Our SQL statement returns only one row
    # of data at a time, therefore, we call this function only once at a time.
    # If we are writing or reading multiple lines of table then we will need to
    # use sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.

    cnt_row = 0
    #int cnt_col = 0;

    while ozz_sql3.sqlite3_step(id_lib_sql3, p_stmt) == SQLITE_ROW:
        #printf("DEBUGz\n");
        #printf("Value= %s\n", sqlite3_column_text(stmt, 1));
        buffer = ""  # Clear temp buffer.
        # Can use sqlite3_column_count(statement/stmt) in place of number_columns.
        for i in range(number_columns):
            # To handle the return of NULL pointers as a string
            # data = (const char*)sqlite3_column_text( stmt, i );
            #        printf( "%s\n", data ? data : "[NULL]" );
            #        sprintf(buffer[strlen(data)], "%s\n", data ? data : "[NULL]" );

            # copy each entry to a buffer. Each entry is a col.
            buffer = buffer + ozz_sql3.sqlite3_column_text(id_lib_sql3, p_stmt, i)  # i == array column.
            #printf("Value: %s\n", buffer);  # DEBUG
            # concat buffer to our return array.

            if i < number_columns -1: # Dont add ', ' after last column.
                buffer = buffer + ","  # add separator token between each col.
            else:  # Add line return, end of row.
                pass  # buffer = buffer + "\n"
            #cnt_col++;  # Need 3D array.
            ## END for
        # insert the return into element[cnt_row]
        db_tbl_row_search.insert(cnt_row, buffer)  # Our row csv string by reference.
        #cnt_col = 0;
        cnt_row += 1
        ## END while

    # Retyrn byref nuber of found rows as element[0] in the list.
    ret_array_length.insert(0, cnt_row)  # return the length of array of found rows.

    #sqlite3_bind_*()  # After sqlit3_prepare_v2()
    #sqlite3_clear_bindings(stmt);

    # The sqlite3_finalize function destroys the prepared statement object.
    return_code = ozz_sql3.sqlite3_finalize(id_lib_sql3, p_stmt )  # Commit to the database.
    if return_code != SQLITE_OK:  # SQLITE_OK==0
        # This is error handling code.
        print("Failed to finalize data: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    # The sqlite3_close function closes the database connection.
    return_code = ozz_sql3.sqlite3_close(id_lib_sql3, p_db)   # SQLITE_OK==0
    if return_code != SQLITE_OK:
        # This is error handling code. NOTE! As p_db is closed the error code may not be available!
        print("Failed to close database: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)

    print( "Successfully retrieved table search data from " + db_file_name, file=sys.stderr)  # DEBUG

    return 1
## END Function


# Search for string in all field names of the table and return array of found rows.
# Duplicate row_id are filtered out.
# This needs to be stepped through each column and filter for duplicate rowid.
# ## Only for tables where all field types are TEXT (String) except rowid. ##
def db_search_table_rowdata_allfields(db_file_name, db_table_name, db_tbl_row_search, db_tbl_col_name, db_search_string, number_columns, ret_array_length):

    # Get the path to the SQLite3 shared library file (dll/so).
    f_library_sql3 = ozz_sql3.get_libsql3_path()  # Get the path for sqlite.dll/so

    # Load the sqlite3 shared library. The returned ID from loading the sqlite
    # shared library must be passed to other functions.
    id_lib_sql3  = ozz_sql3.load_libsql3(f_library_sql3)

    # sqlite3 *p_db;  # database handle (structure).
    p_db = ozz_sql3.p_sqlite3()  # Get the sqlite3 structure.

    # sqlite3_stmt *p_stmt;  # structure represents a single SQL statement
    p_stmt = ozz_sql3.p_sqlite3_stmt()  # Get the sqlite3_stmt structure.

    return_code = 0

    p_zVfs = None  # can use NULL = None
    pzTail = None
    NULL = None

    # Some of these may be redundent.
    cnt_col = 0  # Counter to step though name of each column (field)
    i = 0  # Loop counters.
    j = 0  # Loop counters.

    # Be careful with this when looping fields!!! <- revise!
    # We are also including an extra column for the row ID number.
    number_columns2 = number_columns + 1

    cnt_row = 0  # Used to count through each row of found data to test for duplicate rows.
    buffer = ""  # temp buffer [MAX 128 characters]
    token_buf = ""  # temp buffer [MAX 128 characters]
    ch = 0  # character buffer.
    x = 0  # while loop to retrieve row_id up to first ','
    # Flag to skip writing duplicate row to return array. Also skips ret array increment.
    row_id_exists = 0
    sql_concat = ""  # Build sql query statement.

    return_code = ozz_sql3.sqlite3_open_v2( id_lib_sql3, db_file_name, p_db, SQLITE_OPEN_READONLY, p_zVfs )
    if return_code != SQLITE_OK:
        # Note: these print returns can be commented out if only the Bool return 0; is required.
        print("Can't open database: " + ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    # I need to loop over the columns for full search...
    # WHERE "field/column_name" = will need to be replaced in each loop.
    # This search routine will only work for tables with type TEXT!

    # Be careful of the + 1 in number_columns2 due to extra row_id column.
    # ====> START Loop each column name (field).
    for cnt_col in range(number_columns2 -1):
    #for(cnt_col = 0; cnt_col < number_columns2 -1; cnt_col++)

        # Clear the buffer from the last statement. strcat() will concat on to
        # the previous statement otherwise.
        sql_concat = ""

        # This can be replaced with sprintf()
        # The \"name\" may not be required? !!!
        sql1 = "SELECT rowid, * FROM "  # Note the space after FROM
        sql2 = " WHERE \""  # Note the space before and after WHERE
        sql3 = "\" = "  # Note the space before and after =
        sql4 = ";"

        sql_concat = sql1 + db_table_name + sql2 + db_tbl_col_name[cnt_col] + sql3 + db_search_string + sql4

        #strcat(sql_concat, sql1);  # Add SQL query statement.
        #strcat(sql_concat, db_table_name);  # Add table name to statement.
        #strcat(sql_concat, sql2);  # Add last part of query statement.
        #strcat(sql_concat, db_tbl_col_name[cnt_col]);  # Add feild name (column name) from p_db_tbl_col_name[][] array.
        #strcat(sql_concat, sql3);  # add " = "
        #strcat(sql_concat, db_search_string);  # Add last part of query statement.
        #strcat(sql_concat, sql4);  # Finish the statement with ";"

        # We can only send one query at a time to sqlite3.
        return_code = ozz_sql3.sqlite3_prepare_v2(id_lib_sql3, p_db, sql_concat, -1, p_stmt, pzTail)
        # On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
        # is returned.
        if return_code != SQLITE_OK:
            # This is error handling code for the sqlite3_prepare_v2 function call.
            print("Failed to prepare data: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
            ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
            return -1

        # The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
        # that there is another row ready. Our SQL statement returns only one row
        # of data at a time, therefore, we call this function only once at a time.
        # If we are writing or reading multiple lines of table then we will need to
        # use sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.
        while ozz_sql3.sqlite3_step(id_lib_sql3, p_stmt) == SQLITE_ROW:
            buffer = ""  # Clear the buffer for next row
            # Can use sqlite3_column_count(statement/stmt) in place of number_columns.
            for i in range(number_columns2):
                # To handle the return of NULL pointers as a string
                # data = (const char*)sqlite3_column_text( stmt, i );
                #        printf( "%s\n", data ? data : "[NULL]" );
                #        sprintf(buffer[strlen(data)], "%s\n", data ? data : "[NULL]" );

                # copy each entry to a buffer. Each entry is a col.
                buffer = buffer + ozz_sql3.sqlite3_column_text(id_lib_sql3, p_stmt, i)  # i == array column.

                #====> START Test for repeat row_id ====>
                # Do string compare test to see if the first column row_id
                # already exists in our return search array. Skip copying that
                # row if row_id already exists.
                # This does not order the array entries by row_id and are entered
                # in the order they are found during the search.
                if i == 0:
                    for j in range(cnt_row):
                        # extract the first col item row_id up to token ','
                        token_buf = db_tbl_row_search[j].split(',')

                        # Compare if rowid is already found in our return search array.
                        # If rowid is already present, skip copying that row.
                        if buffer == token_buf:
                            # If True 0 then skip copying this row to p_db_tbl_row_search[].
                            row_id_exists = 1
                        ## END for j
                    ## END if (i == 0)
                #====> END Test for reapeat row_id ====>

                if row_id_exists == 0:  # if row not exist already, write search found to return buffer.

                    # Concat buffer to our return array. This will copy each column of the row
                    # separated by the ', ' comma-space character. aka CSV format.
                    # No return character '\n' is created as we use a separate array
                    # element for each row. To print (write to file) as CSV add
                    # the '\n' after each array element. See the loop that prints
                    # this in main().

                    if i < number_columns2 -1: # Dont add ', ' after last column.
                        buffer = buffer + ','
                        #strcat(p_db_tbl_row_search[cnt_row], ",");  # add separator token between each col.
                        ## END if

                    ## END if row_id_exists
                ## END for i
            # insert the return into element[cnt_row]
            db_tbl_row_search.insert(cnt_row, buffer)  # Our table count by reference.
            if row_id_exists == 0:
                # We wrote this row to the return array, so increment +1
                cnt_row += 1
                ## END if

            # else Skip this row count for return array. row_id_exists == 1.
            row_id_exists = 0;  # Reset for next search column name.
            ## END while (sqlite3_step())

        #sqlite3_bind_*()  # After sqlit3_prepare_v2()
        #sqlite3_clear_bindings(stmt);

        # The sqlite3_finalize function destroys the prepared statement object.
        return_code = ozz_sql3.sqlite3_finalize(id_lib_sql3, p_stmt )  # Commit to the database.
        if return_code != SQLITE_OK:  # SQLITE_OK==0
            # This is error handling code.
            print("Failed to finalize data: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
            ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
            return -1
        ## END for cnt_col, loop/walk each column (field) Name.

    ret_array_length.insert(0, cnt_row)  # return the length of array of found rows.

    # The sqlite3_close function closes the database connection.
    return_code = ozz_sql3.sqlite3_close(id_lib_sql3, p_db)   # SQLITE_OK==0
    if return_code != SQLITE_OK:
        # This is error handling code. NOTE! As p_db is closed the error code may not be available!
        print("Failed to close database: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)

    print( "Successfully retrieved table search data from " + db_file_name, file=sys.stderr)  # DEBUG

    return 1
## END Function

#==============================================================================

# Insert binary row data into a named table.
# If the data already exists will create a new row.
# Table unique index ID INT is auto generated in this table.
def db_insert_table_rowdata_bin(db_file_name, db_tbl_entry, bin_data, bin_data_len):
#int db_insert_table_rowdata_bin(char *db_file_name, char *db_tbl_entry, void *bin_data, int bin_data_len)

    # Get the path to the SQLite3 shared library file (dll/so).
    f_library_sql3 = ozz_sql3.get_libsql3_path()  # Get the path for sqlite.dll/so

    # Load the sqlite3 shared library. The returned ID from loading the sqlite
    # shared library must be passed to other functions.
    id_lib_sql3  = ozz_sql3.load_libsql3(f_library_sql3)

    # sqlite3 *p_db;  # database handle (structure).
    p_db = ozz_sql3.p_sqlite3()  # Get the sqlite3 structure.

    # sqlite3_stmt *p_stmt;  # structure represents a single SQL statement
    p_stmt = ozz_sql3.p_sqlite3_stmt()  # Get the sqlite3_stmt structure.

    return_code = 0

    p_zVfs = None  # can use NULL = None
    pzTail = None
    NULL = None

    return_code = ozz_sql3.sqlite3_open_v2( id_lib_sql3, db_file_name, p_db, SQLITE_OPEN_READWRITE, p_zVfs )
    if return_code != SQLITE_OK:
        # Note: these print returns can be commented out if only the Bool return 0; is required.
        print("Can't open database: " + ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    # We can only send one query at a time to sqlite3.
    return_code = ozz_sql3.sqlite3_prepare_v2(id_lib_sql3, p_db, db_tbl_entry, -1, p_stmt, pzTail)
    # On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    # is returned.
    if return_code != SQLITE_OK:
        # This is error handling code for the sqlite3_prepare_v2 function call.
        print("Failed to prepare data: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    # NOTE! bin_data must be suplied as a Python 3 byte'string' aka b'\x0b\xff ...'
    # If the data is a byte list it must be converted ( byte_string = bytes(byte_list) )
    # SEE: isinstance(), type() to test for type and conversion.
    #True/False = isinstance("Hello World",str)
    #if type("Hello World") == str:
    #    True/False
    return_code = ozz_sql3.sqlite3_bind_blob(id_lib_sql3, p_stmt, 1, bin_data, bin_data_len, SQLITE_STATIC);  # SQLITE_TRANSIENT
    if return_code != SQLITE_OK:
        # This is error handling code for the sqlite3_prepare_v2 function call.
        fprintf(stderr, "Failed to bind data: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    # The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    # that there is another row ready. Our SQL statement returns only one row
    # of data at a time, therefore, we call this function only once at a time.
    # If we are writing or reading multiple lines of table then we will need to
    # use sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.
    return_code = ozz_sql3.sqlite3_step(id_lib_sql3, p_stmt)  # run once for one statement
    #    while( sqlite3_step( stmt ) == SQLITE_ROW ) {;}
    if return_code != SQLITE_DONE:  # SQLITE_DONE==101, SQLITE_ROW==100

        print( "Step failed: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return 0

    #sqlite3_bind_*()  # After sqlit3_prepare_v2()
    #sqlite3_clear_bindings(p_stmt);

    # The sqlite3_finalize function destroys the prepared statement object.
    return_code = ozz_sql3.sqlite3_finalize(id_lib_sql3, p_stmt )  # Commit to the database.
    if return_code != SQLITE_OK:  # SQLITE_OK==0
        # This is error handling code.
        print("Failed to finalize data: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    # The sqlite3_close function closes the database connection.
    return_code = ozz_sql3.sqlite3_close(id_lib_sql3, p_db)   # SQLITE_OK==0
    if return_code != SQLITE_OK:
        # This is error handling code. NOTE! As p_db is closed the error code may not be available!
        print("Failed to close database: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)

    print( "Successfully inserted rowdata into table in " + db_file_name, file=sys.stderr)  # DEBUG

    return 1
## END Function


# NOTE: SQLite3 does have it's own internal typless data structure Mem.
# typedef struct Mem Mem;
# It is an extremely complex data structure that includes many other data
# structures defined in the sqlite source. Also it is predomenently used
# with the sqlite3_value/_* set of API functions.
# typedef struct sqlite3_value sqlite3_value;
# It is more convenient to create our own tag struct, union or linked list
# for the following example.

# List all rows from mixed data types.
# Types:NULL,INTEGER,REAL,TEXT,BLOB
# sqlite3_column_type() returned values:
# SQLITE_NULL, SQLITE_INTEGER, SQLITE_FLOAT, SQLITE_TEXT, SQLITE_BLOB
# https://www.sqlite.org/c3ref/column_blob.html
# data = (const char*)sqlite3_column_text( stmt, 0 );
#        printf( "%s\n", data ? data : "[NULL]" );
# Note that number_elements should generally return number_rows.
def db_list_table_all_types(db_file_name, db_table_name, variant_structure, number_columns, number_rows, ret_number_fields, ret_number_elements):

    number_columns = number_columns  # not used at this time
    number_rows = number_rows  # not used at this time

    # Get the path to the SQLite3 shared library file (dll/so).
    f_library_sql3 = ozz_sql3.get_libsql3_path()  # Get the path for sqlite.dll/so

    # Load the sqlite3 shared library. The returned ID from loading the sqlite
    # shared library must be passed to other functions.
    id_lib_sql3  = ozz_sql3.load_libsql3(f_library_sql3)

    # sqlite3 *p_db;  # database handle (structure).
    p_db = ozz_sql3.p_sqlite3()  # Get the sqlite3 structure.

    # sqlite3_stmt *p_stmt;  # structure represents a single SQL statement
    p_stmt = ozz_sql3.p_sqlite3_stmt()  # Get the sqlite3_stmt structure.

    return_code = 0

    p_zVfs = None  # can use NULL = None
    pzTail = None
    NULL = None

    # Define our type flags.
    IS_NULL = 5
    IS_INTEGER = 1
    IS_FLOAT = 2
    IS_TEXT = 3
    IS_BLOB = 4

    # Row and column counters.
    num_cols = 0
    max_cols = 0
    num_rows = 0

    return_code = ozz_sql3.sqlite3_open_v2( id_lib_sql3, db_file_name, p_db, SQLITE_OPEN_READONLY, p_zVfs )
    if return_code != SQLITE_OK:
        # Note: these print returns can be commented out if only the Bool return 0; is required.
        print("Can't open database: " + ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    # "select * from TableName"
    sql1 = "SELECT rowid, * FROM "  # Note the space after FROM
    sql2 = ";"

    sql_concat = sql1 + db_table_name + sql2

    return_code = ozz_sql3.sqlite3_prepare_v2(id_lib_sql3, p_db, sql_concat, -1, p_stmt, pzTail)
    if return_code != SQLITE_OK:
        # This is error handling code for the sqlite3_prepare_v2 function call.
        print("Failed to prepare data: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    while ozz_sql3.sqlite3_step(id_lib_sql3, p_stmt) != SQLITE_DONE:  # or == SQLITE_OK
        num_cols = ozz_sql3.sqlite3_column_count(id_lib_sql3, p_stmt)
        for i in range(num_cols):
            caseis = ozz_sql3.sqlite3_column_type(id_lib_sql3, p_stmt, i)  # (p_stmt, cidx (col index)
            ## match case is introduced in Python 3.10 onwards.
            #match ozz_sql3.sqlite3_column_type(id_lib_sql3, p_stmt, i):  # (p_stmt, cidx (col index)
            # Note: sqlite3_column_type() ? Default datatype of the result
            if caseis == SQLITE_NULL:
                # Do stuff for NULL pointer, using variant_structure[n].value.vval
                # This will denote an unused array element. It is possible to
                # use this data structure were we test for null as an empty element
                # or as and empty type
                variant_structure[num_rows][i]['typeof'] = IS_NULL
                ozz_sql3.sqlite3_column_text(id_lib_sql3, p_stmt, i)
                variant_structure[num_rows][i]['data'] = None

            elif caseis == SQLITE_INTEGER:
                variant_structure[num_rows][i]['typeof'] = IS_INTEGER
                variant_structure[num_rows][i]['data'] = ozz_sql3.sqlite3_column_int(id_lib_sql3, p_stmt, i)

            elif caseis == SQLITE_FLOAT:  # REAL
                variant_structure[num_rows][i]['typeof'] = IS_FLOAT
                variant_structure[num_rows][i]['data'] = ozz_sql3.sqlite3_column_double(id_lib_sql3, p_stmt, i)

            elif caseis == SQLITE_TEXT:
                variant_structure[num_rows][i]['typeof'] = IS_TEXT
                variant_structure[num_rows][i]['data'] = ozz_sql3.sqlite3_column_text(id_lib_sql3, p_stmt, i)

            elif caseis == SQLITE_BLOB:
                variant_structure[num_rows][i]['typeof'] = IS_BLOB
                variant_structure[num_rows][i]['data'] = ozz_sql3.sqlite3_column_blob(id_lib_sql3, p_stmt, i)
                variant_structure[num_rows][i]['length'] = ozz_sql3.sqlite3_column_bytes(id_lib_sql3, p_stmt, i)

                #0000  ff d8 ff e2 02 1c 49 43 43 5f 50 52 4f 46 49 4c  ......ICC_PROFIL

            else:  # _case:  # default
                # Report an error, this shouldn't happen!
                variant_structure[num_rows][i]['typeof'] = IS_NULL
                variant_structure[num_rows][i]['data'] = None

            ## END match case (switch case)
            ## END for i

        if num_cols > max_cols:
            #Retrieve the longest column length. In most cases num_columns
            # should always be the same length for each row.
            max_cols = num_cols
        num_rows+= 1
        ## END while

    # Only counts the longest column returned.
    ret_number_fields.insert(0, max_cols)
    ret_number_elements.insert(0, num_rows)

    #sqlite3_bind_*()  # After sqlit3_prepare_v2()
    #sqlite3_clear_bindings(stmt);

    # The sqlite3_finalize function destroys the prepared statement object.
    return_code = ozz_sql3.sqlite3_finalize(id_lib_sql3, p_stmt )  # Commit to the database.
    if return_code != SQLITE_OK:  # SQLITE_OK==0
        # This is error handling code.
        print("Failed to finalize data: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)
        return -1

    # The sqlite3_close function closes the database connection.
    return_code = ozz_sql3.sqlite3_close(id_lib_sql3, p_db)   # SQLITE_OK==0
    if return_code != SQLITE_OK:
        # This is error handling code. NOTE! As p_db is closed the error code may not be available!
        print("Failed to close database: " + str(ozz_sql3.sqlite3_errmsg(id_lib_sql3, p_db)) + " | " + str(return_code), file=sys.stderr)  # DEBUG
        ozz_sql3.sqlite3_close(id_lib_sql3, p_db)

    return 1
    ## END Function



# ====> Convenience helper functions
