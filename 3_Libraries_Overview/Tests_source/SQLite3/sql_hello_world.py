#-------------------------------------------------------------------------------
# Name:         sql_hello_world.py (based upon basics_2.c, ozz_sql3.h)
# Purpose:      SQLite3 Hello world.
#
# Platform:     Win64, Ubuntu64
# Depends:      SQLite v3.34.1 plus (dll/so), ctypes, sys, os
#
# Author:       Axle
#
# Created:      08/05/2023
# Updated:      10/05/2023
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

import ctypes
import sys, os

def main():  # Formal application entry after first IF

    return_code = 0
    ver_buffer = []  # empty list (strings are imutable when passed to a function in python).

    ## Get the shared library path.
    # for windows
    if os.name == 'nt':
        f_library = os.path.join(sys.path[0], "sqlite3.dll")
        #print(sys.path)
        #print(sys.path[0])
        #f_library = "D:\\SQLite3Tests\\Py\\sqlite3.dll"
    # for mac and linux
    elif os.name == 'posix':
        #f_library = os.path.join(sys.path[0], "libsqlite3.so.0.8.6")  # Not recomended
        f_library = "libsqlite3.so"
    else:  # Other OS
        pass


    idlib_sql3  = ctypes.cdll.LoadLibrary(f_library)  # Load the sqlite3 shared library.
    #print(type(idlib_sql3))  # DEBUG
    hlib_sql3 = idlib_sql3._handle  # get the OS handle of the CDLL object (Not used here).

    # define the ctype types for the C function:
    # const char *sqlite3_libversion(void);
    idlib_sql3.sqlite3_libversion.argtypes = None  # No argements are sent to the C function (aka function(void);)
    idlib_sql3.sqlite3_libversion.restype = ctypes.c_char_p  # returns char* = ctypes.c_char_p

    # Get our SQLite version. Confirmation that sqlite 3 is installed as a
    # shared library and compiling/working correctly.
    # Ensure that sqlite3.dll is in the system path or in the working directory
    # of the project python script at run-time.
    # NOTE: I am using the C API interface directly and not as a query. SQLite
    # provides a limited number of helper MACROS that can be accessed directly
    # without opening a databse.

    # returns char which is detected as bytes? Correctly a byte is unsigned char,
    # not char. That being said SQLite generally returns unsighned char* from
    # most of the C API functions. c_char_p is meant to be char* and c_ubyte is
    # unsigned char* (aka binary BYTE) so I am not sure what is going on here.
    #bbytes = idlib_sql3.sqlite3_libversion()  # returns u_byte??
    #print(type(bbytes))  # DEBUG

    buffer = idlib_sql3.sqlite3_libversion().decode('utf-8')  # Convert b'' to utf-8 str
    print("1 SQLite Version:" + buffer)
    print("===========================================")


    # Long version check with error returns.
    # This shows the basic steps of an SQLite 3 query statement using an in
    # memory temporary database (:memory:) to get the version number.
    # Note that I am returning 2 changed values from the function which is
    # why I am using a list to return the ver_buffer[0] string.
    return_code = sqlite3_get_version2(ver_buffer)
    if return_code == 0:
        print("Version error return = " + str(return_code))
    elif return_code == 1:
        print("2 SQLite Version:" + ver_buffer[0])  # the version in element[0]
    else:  # == -1
        print("An internal error occured.")
    print("===========================================")

    Con_Pause()  # DEBUG Pause
    return None
## END main()


# Modified from:
# https://zetcode.com/db/sqlitec/
# Get SQLite version - query. (long function)
# Returns string to ret_version buffer (list), as well as int sqlite error codes.
# Python interface to sqlite3.dll/.so using cytpes module.
def sqlite3_get_version2(ret_version):  # Note -> int is an IDE type hint

    SQLITE_OK = 0  # Define the sqlite error codes
    SQLITE_ROW = 100  # Define the sqlite error codes
    str_buffer = ""

    file = ":memory:"  # Using a temporary "In RAM" database.
    b_file = file.encode('utf-8')  # encode our string to C byte array.

    ## Move all of the following to a global definition in a large project.
    # for windows
    if os.name == 'nt':
        f_library = os.path.join(sys.path[0], "sqlite3.dll")
        #f_library = "D:\\SQLite3Tests\\Py\\sqlite3.dll"
    # for mac and linux
    elif os.name == 'posix':
        f_library = os.path.join(sys.path[0], "libsqlite3.so.0.8.6")  # Not recomended
        f_library = "libsqlite3.so"
    else:  # Other OS
        pass

    idlib_sql3  = ctypes.cdll.LoadLibrary(f_library) # load the sqlite3 shared object.
    #print(type(idlib_sql3))  # DEBUG
    hlib_sql3 = idlib_sql3._handle  # get the OS handle of the CDLL object (Not used here).

    # Create an sqlite3 class (struct)
    # sqlite3 *p_db;  # database handle (structure).
    class sqlite3(ctypes.Structure):
        _fields_ = ()  # opaque structure

    p_db = ctypes.POINTER(sqlite3)()  # Create a C pointer to the class (struct)

    # Create an sqlite3_stmt class (struct)
    #sqlite3_stmt *statement;  # structure represents a single SQL statement
    class sqlite3_stmt(ctypes.Structure):
        _fields_ = ()  # opaque structure

    statement = ctypes.POINTER(sqlite3_stmt)()  # Create a C pointer to the class (struct)

    # define the types for the C functions:
    # const char *sqlite3_libversion(void);
    #idlib_sql3.sqlite3_libversion.argtypes = None  # No argements are sent to the C function (aka function(void);)
    #idlib_sql3.sqlite3_libversion.restype = ctypes.c_char_p  # returns char* = ctypes.c_char_p

    # functions called from a shared library (dll, so) must be defined using Ctypes.
    # This is somewhat advanced but wanted to show both the background methods
    # of constructing a basic binder interface between Python and a C
    # shared object (dll, so) as well as keep the same sqlite C API for the example.
    # In practice it will be far easier to use the built in, well tested and
    # safer Python DB-API 2.0 interface or APSW, although the API differs
    # somewhat from the native C API.

    # note that the first 2 functions require access to the class (structures)
    # "By Reference" so that the class (strcut) can be assigned data values.
    # the following fuctions only have to see/read the data thus the
    # ctypes.POINTER(ctypes.POINTER(sqlite3)) vs ctypes.c_void_p
    # this needs to be confirmed! i may have to use ** for all?

    # int sqlite3_open(
    #                   const char *filename,   /* Database filename (UTF-8) */
    #                   sqlite3 **ppDb          /* OUT: SQLite db handle */
    #                   );
    idlib_sql3.sqlite3_open.argtypes = [ctypes.c_char_p, ctypes.POINTER(ctypes.POINTER(sqlite3))]
    idlib_sql3.sqlite3_open.restype = ctypes.c_int
    # int sqlite3_open_v2(
    #                       const char *filename,   /* Database filename (UTF-8) */
    #                       sqlite3 **ppDb,         /* OUT: SQLite db handle */
    #                       int flags,              /* Flags */
    #                       const char *zVfs        /* Name of VFS module to use */
    #                       );

    # int sqlite3_prepare_v2(
    #                           sqlite3 *db,            /* Database handle */
    #                           const char *zSql,       /* SQL statement, UTF-8 encoded */
    #                           int nByte,              /* Maximum length of zSql in bytes. */
    #                           sqlite3_stmt **ppStmt,  /* OUT: Statement handle */
    #                           const char **pzTail     /* OUT: Pointer to unused portion of zSql */
    #                           );
    idlib_sql3.sqlite3_prepare_v2.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.c_int, ctypes.POINTER(ctypes.POINTER(sqlite3_stmt)), ctypes.POINTER(ctypes.c_char_p)]
    idlib_sql3.sqlite3_prepare_v2.restype = ctypes.c_int

    # int sqlite3_step(sqlite3_stmt*);
    idlib_sql3.sqlite3_step.argtypes = [ctypes.c_void_p]
    idlib_sql3.sqlite3_step.restype = ctypes.c_int

    # const unsigned char *sqlite3_column_text(sqlite3_stmt*, int iCol);
    idlib_sql3.sqlite3_column_text.argtypes = [ctypes.c_void_p, ctypes.c_int]
    idlib_sql3.sqlite3_column_text.restype = ctypes.c_char_p  # c_ubyte

    # int sqlite3_finalize(sqlite3_stmt *pStmt);
    idlib_sql3.sqlite3_finalize.argtypes = [ctypes.c_void_p]
    idlib_sql3.sqlite3_finalize.restype = ctypes.c_int

    # int sqlite3_close(sqlite3*);
    idlib_sql3.sqlite3_close.argtypes = [ctypes.c_void_p]
    idlib_sql3.sqlite3_close.restype = ctypes.c_int

    # const char *sqlite3_errmsg(sqlite3*);
    idlib_sql3.sqlite3_errmsg.argtypes = [ctypes.c_void_p]
    idlib_sql3.sqlite3_errmsg.restype = ctypes.c_char_p

    return_code = 0  # API result codes and error codes.
    # Result and error codes can be found here:
    # https://www.sqlite.org/rescode.html

    # return_code is the return error codes.
    # Note: :memory: can be used instead of a file for temporary database
    # operations.

    return_code = idlib_sql3.sqlite3_open(b_file, ctypes.byref(p_db))  # Open Memory (RAM) data base.
    if return_code != SQLITE_OK:  # int 0
        print("Cannot open database: " + str(idlib_sql3.sqlite3_errmsg(p_db).decode('utf-8')) + " | " + str(return_code), file=sys.stderr);  # DEBUG
        return -1

    # START single SQLite3 query statement ==================================>>
    # Only a single query statement can be executed at a time. The 3 functions
    # sqlite3_prepare_v2, sqlite3_step and sqlite3_finalize must be used as
    # a group in a routine for each sqlite query.
    # Prepare -> Do Query -> Finalise and commit.

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

    sql1 = "SELECT SQLITE_VERSION()"
    b_sql1 = sql1.encode('utf-8')

    return_code = idlib_sql3.sqlite3_prepare_v2(p_db, b_sql1, -1, ctypes.byref(statement), None)
    # On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    # is returned.
    if return_code != SQLITE_OK:
        # This is error handling code for the sqlite3_prepare_v2 function call.
        print("Failed to prepare data: " + str(idlib_sql3.sqlite3_errmsg(p_db).decode('utf-8')), file=sys.stderr);  # DEBUG
        idlib_sql3.sqlite3_close(p_db)
        return -1

    # The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    # that there is another row ready. Our SQL statement returns only one row
    # of data in this case, therefore, we call this function only once.
    # If we expected multiple lines of data (rows, columns) we would need to
    # recover each table cel as a step within a loop until end of data
    # (!=SQLITE_ROW).

    return_code = idlib_sql3.sqlite3_step(statement)
    if return_code == SQLITE_ROW:
        # const unsigned char *sqlite3_column_text(sqlite3_stmt*, int iCol);
        # iCol refers to the current column in the return data. In this case
        # there is only one column of return value, so we know the zero column
        # contains the version number.
        str_buffer = idlib_sql3.sqlite3_column_text(statement, 0)
        ret_version.append( str(str_buffer.decode('utf-8')))
        #print("DEBUG" + ret_version[0])
    else:
        print("Step error: " + str(idlib_sql3.sqlite3_errmsg(p_db).decode('utf-8')) + " | " + str(return_code), file=sys.stderr);  # DEBUG
        idlib_sql3.sqlite3_close(p_db)
        return 0

    # The sqlite3_finalize function destroys the prepared statement object and
    # commits the changes to the databse file.
    return_code = idlib_sql3.sqlite3_finalize(statement)
    if return_code != SQLITE_OK:
        # This is error handling code.
        print("Failed to finalize data: " + str(idlib_sql3.sqlite3_errmsg(p_db).decode('utf-8')) + " | " + str(return_code), file=sys.stderr);  # DEBUG
        idlib_sql3.sqlite3_close(p_db)
        return -1

    # The sqlite3_close function closes the database connection.
    return_code = idlib_sql3.sqlite3_close(p_db)
    if return_code != SQLITE_OK:
        # This is error handling code.
        print("Failed to close database: " + str(idlib_sql3.sqlite3_errmsg(p_db).decode('utf-8')) + " | " + str(return_code), file=sys.stderr);  # DEBUG
        return -1

    return 1
## END Function

## See the Basic SQLite 3 example source code library provided with the book.

# Console Pause wrapper.
def Con_Pause():
    dummy = ""
    print("")
    dummy = input("Press [Enter] key to continue...")
    return None

if __name__ == '__main__':
    main()
