#-------------------------------------------------------------------------------
# Name:         ozz_sql3.py (based upon basics_2.c, ozz_sql3.h)
# Purpose:      SQLite3 Basic examples wrapper module.
#               Convenience wrapper functions for SQLite version 3.
#
# Platform:     Win64, Ubuntu64
# Depends:      SQLite v3.34.1 plus (dll/so), ctypes, sys, os
# SQLite3.h     SQLITE_VERSION      "3.34.1"
#
# Author:       Axle
#
# Created:      11/05/2023
# Updated:      15/05/2023
# Copyright:    (c) Axle 2023
# Licence:      MIT-0 No Attribution
#-------------------------------------------------------------------------------
# Notes:
# Using the SQLite shared object (.dll, .so) directly as a Run-time library. The
# sqlite3.dll/.so must be in the system or application path.
#
# The Python 3 built in SQLite3 library is a better/safer approach but uses a
# distinctly different API to the default C API which goes against the
# primary goal of exemplifying the same code routines in all 3 languages.
# As such I am using the Ctypes module for direct access to the shared libraries
# (.dll, .so) exposed C API. In essence python types are translated to C types
# for use by the C based shared object, and then C types are converted back to
# Python types when data is returned. This happens by default with most Python
# library modules but occurs in a more opeque manner in the background.
#
# NOTE!
# Not all conversions for data types have been full tested at this time.
# Use this as a guide rather than a fully tested binder.
#
#-------------------------------------------------------------------------------

import ctypes, sys, os

## ====>> Error Constants
# Beware of name conflicts!
from ozz_sql3_constants import *

# https://www.digitalocean.com/community/tutorials/how-to-write-modules-in-python-3
# https://gist.github.com/michalc/a3147997e21665896836e0f4157975cb


## ====>> SQLite3 Ctype structures
# Create an sqlite3 class (struct)
# sqlite3 *p_db;  # database handle (structure).
# typedef struct sqlite3 sqlite3;
class sqlite3(ctypes.Structure):
    _fields_ = ()  # opaque structure
# !!! I am uncertain if I should make this part of the class !!!
def p_sqlite3():  # p_db
    return ctypes.POINTER(sqlite3)()  # Create a C pointer to the class (struct)


# Create an sqlite3_stmt class (struct)
#sqlite3_stmt *statement;  # structure represents a single SQL statement
## CAPI3REF: Prepared Statement Object
# typedef struct sqlite3_stmt sqlite3_stmt;
class sqlite3_stmt(ctypes.Structure):
    _fields_ = ()  # opaque structure
# !!! I am uncertain if I should make this part of the class !!!
def p_sqlite3_stmt():  # p_stmt
    return ctypes.POINTER(sqlite3_stmt)()  # Create a C pointer to the class (struct)


## ====>> Load SQLite 3 library

# The correct library path must be provided here.
# For a more generalised application you can provide the paths in main() and
# pass as arguments.
def get_libsql3_path():
    #print(sys.path)
    #print(sys.path[0])
    # for windows
    if os.name == 'nt':
        f_library_sql3 = os.path.join(sys.path[0], "sqlite3.dll")
        #f_library_sql3 = "D:\\SQLite3Tests\\Py\\sqlite3.dll"
    # for mac and linux
    elif os.name == 'posix':
        #f_library_sql3 = os.path.join(sys.path[0], "libsqlite3.so.0.8.6")  # Not recomended
        f_library_sql3 = "libsqlite3.so"
    else:  # Other OS
        return -1  # OS not defined
    return f_library_sql3

# Load the shared library with the file location from above.
def load_libsql3(f_library_sql3):
    return ctypes.cdll.LoadLibrary(f_library_sql3)

# hlib_sql3. Only used for direct OS operations.
def handle_libsql3(id_lib_sql3):
    return id_lib_sql3._handle


## ====>> Start SQLite3 C API ctype conversions for each function.
# define the types for the C functions. Returns are converted
# to/from C data types/Python data types, ie. bbyte/string.

# functions called from a shared library (dll, so) must be defined using Ctypes.
# This is somewhat advanced but I wanted to show both the background methods
# of constructing a basic binder interface between Python and a C
# shared object (dll, so) as well as keep the same sqlite C API for the examples.
# In practice it will be far easier to use the built in, well tested, and
# safer Python DB-API 2.0 interface or APSW, although the API differs
# somewhat from the native C API.


## CAPI3REF: Error Codes And Messages
#SQLITE_API int sqlite3_errcode(sqlite3 *db);
#SQLITE_API int sqlite3_extended_errcode(sqlite3 *db);

# SQLITE_API const char *sqlite3_errmsg(sqlite3*);
def sqlite3_errmsg(id_lib_sql3, p_db):
    id_lib_sql3.sqlite3_errmsg.argtypes = [ctypes.c_void_p]
    id_lib_sql3.sqlite3_errmsg.restype = ctypes.c_char_p

    return id_lib_sql3.sqlite3_errmsg(p_db).decode('utf-8')


# int sqlite3_libversion_number(void);
def sqlite3_libversion_number(id_lib_sql3):

    id_lib_sql3.sqlite3_libversion_number.argtypes = None  # No argements are sent to the C function (aka function(void);)
    id_lib_sql3.sqlite3_libversion_number.restype = ctypes.c_int  # returns char* = ctypes.c_char_p

    return id_lib_sql3.sqlite3_libversion_number()

# const char *sqlite3_libversion(void);
def sqlite3_libversion(id_lib_sql3):

    id_lib_sql3.sqlite3_libversion.argtypes = None  # No argements are sent to the C function (aka function(void);)
    id_lib_sql3.sqlite3_libversion.restype = ctypes.c_char_p  # returns char* = ctypes.c_char_p

    return id_lib_sql3.sqlite3_libversion().decode('utf-8')  # Convert b'' to utf-8 str


# Note that the _open and _prepare functions require access to the class (structures)
# "By Reference" so that the class (struct) can be assigned data values.
# Most other fuctions only have to see/read the data thus the
# ctypes.POINTER(ctypes.POINTER(sqlite3)) vs ctypes.c_void_p

## CAPI3REF: Opening A New Database Connection
## CONSTRUCTOR: sqlite3
## See also: [sqlite3_temp_directory] (Windows)
## open an SQLite database file as specified by the filename argument.
# global p_db
# int sqlite3_open(
#       const char *filename,   /* Database filename (UTF-8) */
#       sqlite3 **ppDb          /* OUT: SQLite db handle */
#       );
def sqlite3_open(id_lib_sql3, db_filename, pp_db):

    id_lib_sql3.sqlite3_open.argtypes = [ctypes.c_char_p, ctypes.POINTER(ctypes.POINTER(sqlite3))]
    id_lib_sql3.sqlite3_open.restype = ctypes.c_int

    # Returns sqlite3 result codes or error code.
    return id_lib_sql3.sqlite3_open(db_filename.encode('utf-8'), ctypes.byref(pp_db))


#SQLITE_API int sqlite3_open_v2(
#       const char *filename,   /* Database filename (UTF-8) */
#       sqlite3 **ppDb,         /* OUT: SQLite db handle */
#       int flags,              /* Flags */
#       const char *zVfs        /* Name of VFS module to use */
#       );
# flags [SQLITE_OPEN_READONLY [SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE]]
def sqlite3_open_v2(id_lib_sql3, db_filename, pp_db, flags, p_zVfs):

    id_lib_sql3.sqlite3_open_v2.argtypes = [ctypes.c_char_p, ctypes.POINTER(ctypes.POINTER(sqlite3)), ctypes.c_int, ctypes.c_void_p]
    id_lib_sql3.sqlite3_open_v2.restype = ctypes.c_int

    # Returns sqlite3 result codes or error code.
    return id_lib_sql3.sqlite3_open_v2(db_filename.encode('utf-8'), ctypes.byref(pp_db), flags, p_zVfs)


## CAPI3REF: Compiling An SQL Statement (SQL statement compiler)
## KEYWORDS: {SQL statement compiler}
## METHOD: sqlite3
## CONSTRUCTOR: sqlite3_stmt
## To execute an SQL statement, it must first be compiled into a byte-code
## program using one of these routines.  Or, in other words, these routines
## are constructors for the [prepared statement] object.

# legacy, deprecated
#SQLITE_API int sqlite3_prepare(
#  sqlite3 *db,            # Database handle
#  const char *zSql,       # SQL statement, UTF-8 encoded
#  int nByte,              # Maximum length of zSql in bytes.
#  sqlite3_stmt **ppStmt,  # OUT: Statement handle
#  const char **pzTail     # OUT: Pointer to unused portion of zSql
#);

# int sqlite3_prepare_v2(
#       sqlite3 *db,            /* Database handle */
#       const char *zSql,       /* SQL statement, UTF-8 encoded */
#       int nByte,              /* Maximum length of zSql in bytes. */
#       sqlite3_stmt **ppStmt,  /* OUT: Statement handle */
#       const char **pzTail     /* OUT: Pointer to unused portion of zSql */
#       );
def sqlite3_prepare_v2(id_lib_sql3, p_db, sql_query, nByte, pp_stmt, pzTail):
    b_sql1 = sql_query.encode('utf-8')

    ## Check pzTail ctypes.POINTER(ctypes.c_char_p), ctypes.byref(pzTail) ?
    id_lib_sql3.sqlite3_prepare_v2.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.c_int, ctypes.POINTER(ctypes.POINTER(sqlite3_stmt)), ctypes.c_void_p]
    id_lib_sql3.sqlite3_prepare_v2.restype = ctypes.c_int

    # Returns sqlite3 result codes or error code.
    return id_lib_sql3.sqlite3_prepare_v2(p_db, b_sql1, nByte, ctypes.byref(pp_stmt), pzTail)

# SQLITE_API int sqlite3_prepare_v3(
#       sqlite3 *db,            # Database handle
#       const char *zSql,       # SQL statement, UTF-8 encoded
#       int nByte,              # Maximum length of zSql in bytes.
#       unsigned int prepFlags, # Zero or more SQLITE_PREPARE_ flags
#       sqlite3_stmt **ppStmt,  # OUT: Statement handle
#       const char **pzTail     # OUT: Pointer to unused portion of zSql
#       );
def sqlite3_prepare_v3(id_lib_sql3, p_db, sql_query, nByte, prepFlags, pp_stmt, pzTail):
    b_sql1 = sql_query.encode('utf-8')

    ## Check p_db ctypes.POINTER(sqlite3)
    id_lib_sql3.sqlite3_prepare_v3.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.c_int, ctypes.c_int, ctypes.POINTER(ctypes.POINTER(sqlite3_stmt)), ctypes.c_void_p]
    id_lib_sql3.sqlite3_prepare_v3.restype = ctypes.c_int

    # Returns sqlite3 result codes or error code.
    return id_lib_sql3.sqlite3_prepare_v3(p_db, b_sql1, nByte, prepFlags, ctypes.byref(pp_stmt), pzTail)


## CAPI3REF: Binding Values To Prepared Statements

# SQLITE_API int sqlite3_bind_blob(sqlite3_stmt*, int, const void*, int n, void(*)(void*));
# Handle h_database_open, statement, Index = 1 to limit bin Bytes, length in bytes, Destructor flag
# Destructor flag [SQLITE_STATIC | SQLITE_TRANSIENT] (* (void*))
# NOTE! bvalue must be suplied as a Python 3 byte'string' aka b'\x0b\xff ...'
# If the data is a byte list it must be converted ( byte_string = bytes(byte_list) )
# SEE: isinstance(), type() to test for type and conversion.
#True/False = isinstance("Hello World",str)
#if type("Hello World") == str:
#    True/False
def sqlite3_bind_blob(id_lib_sql3, p_stmt, index, bvalue, nByte, dest_flag):

    id_lib_sql3.sqlite3_bind_blob.argtypes = [ctypes.POINTER(sqlite3_stmt), ctypes.c_int, ctypes.c_void_p, ctypes.c_int, ctypes.c_void_p]  # ctypes.POINTER(ctypes.c_void_p)
    id_lib_sql3.sqlite3_bind_blob.restype = ctypes.c_int

    return id_lib_sql3.sqlite3_bind_blob(p_stmt, index, bvalue, nByte, dest_flag)


#SQLITE_API int sqlite3_bind_blob64(sqlite3_stmt*, int, const void*, sqlite3_uint64,
#                        void(*)(void*));

# SQLITE_API int sqlite3_bind_double(sqlite3_stmt*, int, double);
def sqlite3_bind_double(id_lib_sql3, p_stmt, index, value):

    id_lib_sql3.sqlite3_bind_double.argtypes = [ctypes.POINTER(sqlite3_stmt), ctypes.c_int, ctypes.c_longdouble]
    id_lib_sql3.sqlite3_bind_double.restype = ctypes.c_int

    return id_lib_sql3.sqlite3_bind_double(p_stmt, index, value)


# SQLITE_API int sqlite3_bind_int(sqlite3_stmt*, int, int);
def sqlite3_bind_int(id_lib_sql3, p_stmt, index, value):

    id_lib_sql3.sqlite3_bind_int.argtypes = [ctypes.POINTER(sqlite3_stmt), ctypes.c_int, ctypes.c_int]
    id_lib_sql3.sqlite3_bind_int.restype = ctypes.c_int

    return id_lib_sql3.sqlite3_bind_int(p_stmt, index, value)


# #SQLITE_API int sqlite3_bind_int64(sqlite3_stmt*, int, sqlite3_int64);
def sqlite3_bind_int64(id_lib_sql3, p_stmt, index, value):

    id_lib_sql3.sqlite3_bind_int64.argtypes = [ctypes.POINTER(sqlite3_stmt), ctypes.c_int, c_longlong]
    id_lib_sql3.sqlite3_bind_int64.restype = ctypes.c_int

    return id_lib_sql3.sqlite3_bind_int64(p_stmt, index, value)

##SQLITE_API int sqlite3_bind_null(sqlite3_stmt*, int);
def sqlite3_bind_null(id_lib_sql3, p_stmt, index):

    id_lib_sql3.sqlite3_bind_null.argtypes = [ctypes.POINTER(sqlite3_stmt), ctypes.c_int]
    id_lib_sql3.sqlite3_bind_null.restype = ctypes.c_int

    return id_lib_sql3.sqlite3_bind_null(p_stmt, index)


# SQLITE_API int sqlite3_bind_text(sqlite3_stmt*,int,const char*,int,void(*)(void*));
# Destructor flag [SQLITE_STATIC | SQLITE_TRANSIENT] (* (void*))
def sqlite3_bind_text(id_lib_sql3, p_stmt, index, value, nByte, dest_flag):

    id_lib_sql3.sqlite3_bind_text.argtypes = [ctypes.POINTER(sqlite3_stmt), ctypes.c_int, ctypes.c_char_p, ctypes.c_int, ctypes.POINTER(ctypes.c_void_p)]
    id_lib_sql3.sqlite3_bind_text.restype = ctypes.c_int

    return id_lib_sql3.sqlite3_bind_text(p_stmt, index, value, nByte, dest_flag)


#SQLITE_API int sqlite3_bind_text16(sqlite3_stmt*, int, const void*, int, void(*)(void*));
#SQLITE_API int sqlite3_bind_text64(sqlite3_stmt*, int, const char*, sqlite3_uint64,
#                         void(*)(void*), unsigned char encoding);
#SQLITE_API int sqlite3_bind_value(sqlite3_stmt*, int, const sqlite3_value*);


"""
#SQLITE_API int sqlite3_bind_pointer(sqlite3_stmt*, int, void*, const char*,void(*)(void*));
# Destructor flag [SQLITE_STATIC | SQLITE_TRANSIENT] (* (void*))
def sqlite3_bind_pointer(id_lib_sql3, p_stmt, index, p_void_unknown, p_char_unknown, dest_flag):

    id_lib_sql3.sqlite3_bind_pointer.argtypes = [ctypes.POINTER(sqlite3_stmt), ctypes.c_int, ctypes.c_void_p, ctypes.c_char_p, ctypes.POINTER(ctypes.c_void_p)]
    id_lib_sql3.sqlite3_bind_pointer.restype = ctypes.c_int

    return sqlite3_bind_pointer(p_stmt, index, value, p_void_unknown, p_char_unknown, dest_flag)
"""

#SQLITE_API int sqlite3_bind_zeroblob(sqlite3_stmt*, int, int n);
#SQLITE_API int sqlite3_bind_zeroblob64(sqlite3_stmt*, int, sqlite3_uint64);

# CAPI3REF: Number Of SQL Parameters
#SQLITE_API int sqlite3_bind_parameter_count(sqlite3_stmt*);
#CAPI3REF: Name Of A Host Parameter


## CAPI3REF: Number Of Columns In A Result Set
#SQLITE_API int sqlite3_column_count(sqlite3_stmt *pStmt);
def sqlite3_column_count(id_lib_sql3, p_stmt):

    id_lib_sql3.sqlite3_column_count.argtypes = [ctypes.POINTER(sqlite3_stmt)]
    id_lib_sql3.sqlite3_column_count.restype = ctypes.c_int

    return id_lib_sql3.sqlite3_column_count(p_stmt)


## CAPI3REF: Column Names In A Result Set
#SQLITE_API const char *sqlite3_column_name(sqlite3_stmt*, int N);
# N = column count starting at 0
def sqlite3_column_name(id_lib_sql3, p_stmt, N):

    id_lib_sql3.sqlite3_column_name.argtypes = [ctypes.POINTER(sqlite3_stmt), ctypes.c_int]
    id_lib_sql3.sqlite3_column_name.restype = ctypes.c_char_p

    return id_lib_sql3.sqlite3_column_name(p_stmt, N).decode('utf-8')  # Convert b'' to utf-8 str


## CAPI3REF: Source Of Data In A Query Result

# SQLITE_API const char *sqlite3_column_database_name(sqlite3_stmt*,int);
def sqlite3_column_database_name(id_lib_sql3, p_stmt, N):

    id_lib_sql3.sqlite3_column_database_name.argtypes = [ctypes.POINTER(sqlite3_stmt), ctypes.c_int]
    id_lib_sql3.sqlite3_column_database_name.restype = ctypes.c_char_p

    return id_lib_sql3.sqlite3_column_database_name(p_stmt, N).decode('utf-8')  # Convert b'' to utf-8 str

#SQLITE_API const void *sqlite3_column_database_name16(sqlite3_stmt*,int);

# SQLITE_API const char *sqlite3_column_table_name(sqlite3_stmt*,int);
def sqlite3_column_table_name(id_lib_sql3, p_stmt, N):

    id_lib_sql3.sqlite3_column_table_name.argtypes = [ctypes.POINTER(sqlite3_stmt), ctypes.c_int]
    id_lib_sql3.sqlite3_column_table_name.restype = ctypes.c_char_p

    return id_lib_sql3.sqlite3_column_table_name(p_stmt, N).decode('utf-8')  # Convert b'' to utf-8 str

#SQLITE_API const void *sqlite3_column_table_name16(sqlite3_stmt*,int);

# SQLITE_API const char *sqlite3_column_origin_name(sqlite3_stmt*,int);
def sqlite3_column_origin_name(id_lib_sql3, p_stmt, N):

    id_lib_sql3.sqlite3_column_origin_name.argtypes = [ctypes.POINTER(sqlite3_stmt), ctypes.c_int]
    id_lib_sql3.sqlite3_column_origin_name.restype = ctypes.c_char_p

    return id_lib_sql3.sqlite3_column_origin_name(p_stmt, N).decode('utf-8')  # Convert b'' to utf-8 str

# SQLITE_API const void *sqlite3_column_origin_name16(sqlite3_stmt*,int);
def sqlite3_column_origin_name(id_lib_sql3, p_stmt, N):

    id_lib_sql3.sqlite3_column_origin_name.argtypes = [ctypes.POINTER(sqlite3_stmt), ctypes.c_int]
    id_lib_sql3.sqlite3_column_origin_name.restype = ctypes.c_char_p

    return id_lib_sql3.sqlite3_column_origin_name(p_stmt, N).decode('utf-8')  # Convert b'' to utf-8 str


## CAPI3REF: Declared Datatype Of A Query Result

# const char *sqlite3_column_decltype(sqlite3_stmt*,int);
def sqlite3_column_decltype(id_lib_sql3, p_stmt, N):

    id_lib_sql3.sqlite3_column_decltype.argtypes = [ctypes.POINTER(sqlite3_stmt), ctypes.c_int]
    id_lib_sql3.sqlite3_column_decltype.restype = ctypes.c_char_p

    return id_lib_sql3.sqlite3_column_decltype(p_stmt, N).decode('utf-8')  # Convert b'' to utf-8 str

#const void *sqlite3_column_decltype16(sqlite3_stmt*,int);


## CAPI3REF: Evaluate An SQL Statement
## ** METHOD: sqlite3_stmt
## this function must be called one or more times to evaluate the statement.

def sqlite3_step(id_lib_sql3, p_stmt):
    # int sqlite3_step(sqlite3_stmt*);
    id_lib_sql3.sqlite3_step.argtypes = [ctypes.c_void_p]
    id_lib_sql3.sqlite3_step.restype = ctypes.c_int

    # Returns sqlite3 result codes or error code.
    return id_lib_sql3.sqlite3_step(p_stmt)

# CAPI3REF: Number of columns in a result set
# See also: [sqlite3_column_count()]
# SQLITE_API int sqlite3_data_count(sqlite3_stmt *pStmt);


## CAPI3REF: Result Values From A Query
## KEYWORDS: {column access functions}
## METHOD: sqlite3_stmt
## ^These routines return information about a single column of the current
## result row of a query.  ^In every case the first argument is a pointer
## to the [prepared statement] that is being evaluated (the [sqlite3_stmt*]
## that was returned from [sqlite3_prepare_v2()] or one of its variants)
## and the second argument is the index of the column for which information
## should be returned. ^The leftmost column of the result set has the index 0.
## ^The number of columns in the result can be determined using
## [sqlite3_column_count()].

## sqlite3_column_type() returns:
## [SQLITE_INTEGER],[SQLITE_FLOAT],[SQLITE_TEXT],[SQLITE_BLOB],or[SQLITE_NULL]

## NOTE: All integer values are expanded to the largest size 64-bit and truncated
## 32-bit if sqlite3_column_int dicarding any overflow. Treat all SQLITE_INTEGER
## as 64-bit sqlite3_column_int64 unless you are sure that you are not using
## larger than 32-bit integers in your application.

# SQLITE_API const void *sqlite3_column_blob(sqlite3_stmt*, int iCol);
def sqlite3_column_blob(id_lib_sql3, p_stmt, iCol):
    id_lib_sql3.sqlite3_column_blob.argtypes = [ctypes.c_void_p, ctypes.c_int]
    id_lib_sql3.sqlite3_column_blob.restype = ctypes.c_char_p  # c_ubyte

    # iCol refers to the current column in the return data. Use a loop with
    # iCol enumerated for each col/row of data. sqlite3_column_* returns one
    # column at a time.
    # Returns void* (unsigned char*).
    return id_lib_sql3.sqlite3_column_blob(p_stmt, iCol)

# SQLITE_API double sqlite3_column_double(sqlite3_stmt*, int iCol);
def sqlite3_column_double(id_lib_sql3, p_stmt, iCol):
    # const unsigned char *sqlite3_column_text(sqlite3_stmt*, int iCol);
    id_lib_sql3.sqlite3_column_double.argtypes = [ctypes.c_void_p, ctypes.c_int]
    id_lib_sql3.sqlite3_column_double.restype = ctypes.c_longdouble  ## ctypes.c_double

    # iCol refers to the current column in the return data. Use a loop with
    # iCol enumerated for each col/row of data. sqlite3_column_* returns one
    # column at a time.
    # Returns float.
    return id_lib_sql3.sqlite3_column_double(p_stmt, iCol)

# SQLITE_API int sqlite3_column_int(sqlite3_stmt*, int iCol);
# preferable to use sqlite3_column_int64
def sqlite3_column_int(id_lib_sql3, p_stmt, iCol):
    id_lib_sql3.sqlite3_column_int.argtypes = [ctypes.c_void_p, ctypes.c_int]
    id_lib_sql3.sqlite3_column_int.restype = ctypes.c_int  # c_ubyte

    # iCol refers to the current column in the return data. Use a loop with
    # iCol enumerated for each col/row of data. sqlite3_column_* returns one
    # column at a time.
    # Returns int (32-bit).
    return id_lib_sql3.sqlite3_column_int(p_stmt, iCol)

# SQLITE_API sqlite3_int64 sqlite3_column_int64(sqlite3_stmt*, int iCol);
def sqlite3_column_int64(id_lib_sql3, p_stmt, iCol):
    id_lib_sql3.sqlite3_column_int64.argtypes = [ctypes.c_void_p, ctypes.c_int]
    id_lib_sql3.sqlite3_column_int64.restype = ctypes.c_longlong

    # iCol refers to the current column in the return data. Use a loop with
    # iCol enumerated for each col/row of data. sqlite3_column_* returns one
    # column at a time.
    # Returns int (64-bit).
    return id_lib_sql3.sqlite3_column_int64(p_stmt, iCol)

# SQLITE_API const unsigned char *sqlite3_column_text(sqlite3_stmt*, int iCol);
def sqlite3_column_text(id_lib_sql3, p_stmt, iCol):
    id_lib_sql3.sqlite3_column_text.argtypes = [ctypes.c_void_p, ctypes.c_int]
    id_lib_sql3.sqlite3_column_text.restype = ctypes.c_char_p  # ctypes.POINTER(c_ubyte)?

    # iCol refers to the current column in the return data. Use a loop with
    # iCol enumerated for each col/row of data. sqlite3_column_* returns one
    # column at a time.
    # Returns UTF-8 string.
    return id_lib_sql3.sqlite3_column_text(p_stmt, iCol).decode('utf-8')


#SQLITE_API const void *sqlite3_column_text16(sqlite3_stmt*, int iCol);

"""
## The following function has a seperate API usage to the other sqlite3_column_
## exemplified here. typedef struct sqlite3_value sqlite3_value;
#SQLITE_API sqlite3_value *sqlite3_column_value(sqlite3_stmt*, int iCol);
def sqlite3_column_value(id_lib_sql3, p_stmt, iCol):
    id_lib_sql3.sqlite3_column_value.argtypes = [ctypes.c_void_p, ctypes.c_int]
    id_lib_sql3.sqlite3_column_value.restype = ctypes.c_char_p

    # iCol refers to the current column in the return data. Use a loop with
    # iCol enumerated for each col/row of data. sqlite3_column_* returns one
    # column at a time.
    # Returns UTF-8 string.
    return id_lib_sql3.sqlite3_column_value(p_stmt, iCol).decode('utf-8')
"""


# SQLITE_API int sqlite3_column_bytes(sqlite3_stmt*, int iCol);
def sqlite3_column_bytes(id_lib_sql3, p_stmt, iCol):
    id_lib_sql3.sqlite3_column_bytes.argtypes = [ctypes.c_void_p, ctypes.c_int]
    id_lib_sql3.sqlite3_column_bytes.restype = ctypes.c_int

    # iCol refers to the current column in the return data. Use a loop with
    # iCol enumerated for each col/row of data. sqlite3_column_* returns one
    # column at a time.
    # Returns integer (32-bit).
    return id_lib_sql3.sqlite3_column_bytes(p_stmt, iCol)

#SQLITE_API int sqlite3_column_bytes16(sqlite3_stmt*, int iCol);

# SQLITE_API int sqlite3_column_type(sqlite3_stmt*, int iCol);
# SQLITE_INTEGER  =   1
# SQLITE_FLOAT    =   2
# SQLITE_BLOB     =   4
# SQLITE_NULL     =   5
# SQLITE_TEXT = 3
def sqlite3_column_type(id_lib_sql3, p_stmt, iCol):
    id_lib_sql3.sqlite3_column_type.argtypes = [ctypes.c_void_p, ctypes.c_int]
    id_lib_sql3.sqlite3_column_type.restype = ctypes.c_int

    # iCol refers to the current column in the return data. Use a loop with
    # iCol enumerated for each col/row of data. sqlite3_column_* returns one
    # column at a time.
    # Returns int
    return id_lib_sql3.sqlite3_column_type(p_stmt, iCol)


## CAPI3REF: Reset A Prepared Statement Object

# SQLITE_API int sqlite3_reset(sqlite3_stmt *pStmt);
def sqlite3_reset(id_lib_sql3, p_stmt):
    id_lib_sql3.sqlite3_reset.argtypes = [ctypes.c_void_p]
    id_lib_sql3.sqlite3_reset.restype = ctypes.c_int

    # Returns int (32-bit).
    return id_lib_sql3.sqlite3_reset(p_stmt, iCol)


## CAPI3REF: Destroy A Prepared Statement Object
def sqlite3_finalize(id_lib_sql3, p_stmt):
    # int sqlite3_finalize(sqlite3_stmt *pStmt);
    id_lib_sql3.sqlite3_finalize.argtypes = [ctypes.c_void_p]
    id_lib_sql3.sqlite3_finalize.restype = ctypes.c_int

    # The sqlite3_finalize function destroys the prepared statement object and
    # commits the changes to the database file.
    # Returns sqlite3 result codes or error code.
    return id_lib_sql3.sqlite3_finalize(p_stmt)


## CAPI3REF: Closing A Database Connection
## DESTRUCTOR: sqlite3
## The sqlite3_close_v2() interface is intended for use with host languages
## that are garbage collected, and where the order in which destructors are
## called is arbitrary.

#SQLITE_API int sqlite3_close(sqlite3*);
def sqlite3_close(id_lib_sql3, p_db):

    id_lib_sql3.sqlite3_close.argtypes = [ctypes.c_void_p]
    id_lib_sql3.sqlite3_close.restype = ctypes.c_int

    # The sqlite3_close function closes the database connection.
    # Returns sqlite3 result codes or error code.
    return id_lib_sql3.sqlite3_close(p_db)

# SQLITE_API int sqlite3_close_v2(sqlite3*);
def sqlite3_close_v2(id_lib_sql3, p_db):

    id_lib_sql3.sqlite3_close_v2.argtypes = [ctypes.c_void_p]
    id_lib_sql3.sqlite3_close_v2.restype = ctypes.c_int

    # The sqlite3_close_v2 function closes the database connection.
    # Returns sqlite3 result codes or error code.
    return id_lib_sql3.sqlite3_close_v2(p_db)


## CAPI3REF: Suspend Execution For A Short Time
# SQLITE_API int sqlite3_sleep(int); (milliseconds)
def sqlite3_sleep(id_lib_sql3, p_db, tsleep):

    id_lib_sql3.sqlite3_sleep.argtypes = [ctypes.c_int]
    id_lib_sql3.sqlite3_sleep.restype = ctypes.c_int

    # Returns sqlite3 result codes or error code.
    return id_lib_sql3.sqlite3_sleep(tsleep)

## CAPI3REF: Find The Database Handle Of A Prepared Statement (sqlite3_stmt)
# SQLITE_API sqlite3 *sqlite3_db_handle(sqlite3_stmt*);
def sqlite3_db_handle(id_lib_sql3, p_stmt):

    id_lib_sql3.sqlite3_db_handle.argtypes = [ctypes.c_void_p]
    id_lib_sql3.sqlite3_db_handle.restype = ctypes.c_int

    # Returns hlib_sql3 (Used for OS ops)
    # Same as handle_libsql3()
    return id_lib_sql3.sqlite3_db_handle(p_stmt)

# CAPI3REF: Return The Filename For A Database Connection (sqlite3)
#SQLITE_API const char *sqlite3_db_filename(sqlite3 *db, const char *zDbName);

# CAPI3REF: Determine if a database is read-only
#SQLITE_API int sqlite3_db_readonly(sqlite3 *db, const char *zDbName);





## ====>> Future

# CAPI3REF: Last Insert Rowid
# SQLITE_API sqlite3_int64 sqlite3_last_insert_rowid(sqlite3*);
def sqlite3_last_insert_rowid(id_lib_sql3, p_db):

    id_lib_sql3.sqlite3_last_insert_rowid.argtypes = [ctypes.c_void_p]  # ctypes.CDLL
    id_lib_sql3.sqlite3_last_insert_rowid.restype = ctypes.c_longlong

    return id_lib_sql3.sqlite3_db_handle(p_db)


## CAPI3REF: Extract Metadata About A Column Of A Table
## METHOD: sqlite3
##
## ^(The sqlite3_table_column_metadata(X,D,T,C,....) routine returns
## information about column C of table T in database D
## on [database connection] X.)^  ^The sqlite3_table_column_metadata()
## interface returns SQLITE_OK and fills in the non-NULL pointers in
## the final five arguments with appropriate values if the specified
## column exists.  ^The sqlite3_table_column_metadata() interface returns
## SQLITE_ERROR if the specified column does not exist.
## ^If the column-name parameter to sqlite3_table_column_metadata() is a
## NULL pointer, then this routine simply checks for the existence of the
## table and returns SQLITE_OK if the table exists and SQLITE_ERROR if it
## does not.  If the table name parameter T in a call to
## sqlite3_table_column_metadata(X,D,T,C,...) is NULL then the result is
## undefined behavior.
##
## ^The column is identified by the second, third and fourth parameters to
## this function. ^(The second parameter is either the name of the database
## (i.e. "main", "temp", or an attached database) containing the specified
## table or NULL.)^ ^If it is NULL, then all attached databases are searched
## for the table using the same algorithm used by the database engine to
## resolve unqualified table references.
##
## ^The third and fourth parameters to this function are the table and column
## name of the desired column, respectively.
##
## ^Metadata is returned by writing to the memory locations passed as the 5th
## and subsequent parameters to this function. ^Any of these arguments may be
## NULL, in which case the corresponding element of metadata is omitted.
#SQLITE_API int sqlite3_table_column_metadata(
#  sqlite3 *db,                /* Connection handle */
#  const char *zDbName,        /* Database name or NULL */
#  const char *zTableName,     /* Table name */
#  const char *zColumnName,    /* Column name */
#  char const **pzDataType,    /* OUTPUT: Declared data type */
#  char const **pzCollSeq,     /* OUTPUT: Collation sequence name */
#  int *pNotNull,              /* OUTPUT: True if NOT NULL constraint exists */
#  int *pPrimaryKey,           /* OUTPUT: True if column part of PK */
#  int *pAutoinc               /* OUTPUT: True if column is auto-increment */
#);

def sqlite3_table_column_metadata(id_lib_sql3, p_db, zDbName, zTableName, zColumnName, pzDataType, pzCollSeq, pNotNull, pPrimaryKey, pAutoinc):
    ## !!! this binder is not complete nor fully tested !!!
    ## Check pointers for **pzDataType, **pzCollSeq and data types.
    ## ctypes.c_char_p will accept string or ctypes but wont accept None
    ## None (or Null.pointer) requires ctypes.c_void_p
    ## Check pzTail ctypes.POINTER(ctypes.c_char_p), ctypes.byref(pzTail)

    if zDbName != None:
        zDbName = zDbName.encode('utf-8')

    if zTableName != None:
        zTableName = zTableName.encode('utf-8')

    if zColumnName != None:
        zColumnName = zColumnName.encode('utf-8')

    # Other arguments not implimented !

    #[ctypes.c_void_p, ctypes.c_char_p, ctypes.c_int, ctypes.c_int, ctypes.POINTER(ctypes.POINTER(sqlite3_stmt)), ctypes.c_void_p]
    id_lib_sql3.sqlite3_table_column_metadata.argtypes = \
    [ctypes.c_void_p, \
    ctypes.c_void_p, \
    ctypes.c_void_p, \
    ctypes.c_void_p, \
    ctypes.POINTER(ctypes.c_char_p), \
    ctypes.POINTER(ctypes.c_char_p), \
    ctypes.POINTER(ctypes.c_int), \
    ctypes.POINTER(ctypes.c_int), \
    ctypes.POINTER(ctypes.c_int)]
    id_lib_sql3.sqlite3_table_column_metadata.restype = ctypes.c_int

    return id_lib_sql3.sqlite3_table_column_metadata(\
    p_db, \
    zDbName, \
    zTableName, \
    zColumnName, \
    pzDataType, \
    pzCollSeq, \
    pNotNull, \
    pPrimaryKey, \
    pAutoinc)


# CAPI3REF: Set the Last Insert Rowid value.
# SQLITE_API void sqlite3_set_last_insert_rowid(sqlite3*,sqlite3_int64);

# CAPI3REF: Count The Number Of Rows Modified
# SQLITE_API int sqlite3_changes(sqlite3*);

# CAPI3REF: Total Number Of Rows Modified
# SQLITE_API int sqlite3_total_changes(sqlite3*);

# CAPI3REF: Interrupt A Long-Running Query
# SQLITE_API void sqlite3_interrupt(sqlite3*);

# CAPI3REF: Convenience Routines For Running Queries
# SEE: sqlite3_malloc(), sqlite3_free_table()
#SQLITE_API int sqlite3_get_table(
#  sqlite3 *db,          /* An open database */
#  const char *zSql,     /* SQL to be evaluated */
#  char ***pazResult,    /* Results of the query */
#  int *pnRow,           /* Number of result rows written here */
#  int *pnColumn,        /* Number of result columns written here */
#  char **pzErrmsg       /* Error msg written here */
#);
#SQLITE_API void sqlite3_free_table(char **result);

# CAPI3REF: Formatted String Printing Functions
#SQLITE_API char *sqlite3_mprintf(const char*,...);
#SQLITE_API char *sqlite3_vmprintf(const char*, va_list);
#SQLITE_API char *sqlite3_snprintf(int,char*,const char*, ...);
#SQLITE_API char *sqlite3_vsnprintf(int,char*,const char*, va_list);

# CAPI3REF: Memory Allocation Subsystem

# CAPI3REF: Memory Allocator Statistics

# CAPI3REF: Pseudo-Random Number Generator

# CAPI3REF: Run-time Limits
# CAPI3REF: Run-Time Limit Categories

# CAPI3REF: Retrieving Statement SQL (sqlite3_stmt)
# Future
# CAPI3REF: Determine If An SQL Statement Writes The Database
# Future
# CAPI3REF: Query The EXPLAIN Setting For A Prepared Statement
# Future
# CAPI3REF: Determine If A Prepared Statement Has Been Reset
# Future

# CAPI3REF: Dynamically Typed Value Object
# KEYWORDS: {protected sqlite3_value} {unprotected sqlite3_value}
#typedef struct sqlite3_value sqlite3_value;
# Future

#CAPI3REF: SQL Function Context Object
#typedef struct sqlite3_context sqlite3_context;
# Future

# CAPI3REF: Free Memory Used By A Database Connection
#SQLITE_API int sqlite3_db_release_memory(sqlite3*);

# CAPI3REF: Impose A Limit On Heap Size
#SQLITE_API sqlite3_int64 sqlite3_soft_heap_limit64(sqlite3_int64 N);
#SQLITE_API sqlite3_int64 sqlite3_hard_heap_limit64(sqlite3_int64 N);

# CAPI3REF: Extract Metadata About A Column Of A Table

# CAPI3REF: String Comparison

# CAPI3REF: String Globbing

# CAPI3REF: String LIKE Matching

