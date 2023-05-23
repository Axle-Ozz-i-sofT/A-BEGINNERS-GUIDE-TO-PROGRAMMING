//------------------------------------------------------------------------------
// Name:        sql_hello_world.c (based upon basics_2.c, ozz_sql3.h)
// Purpose:     SQLite3 Hello world.
//
// Platform:    Win64, Ubuntu64
// Depends:     SQLite v3.34.1 plus
//
// Author:      Axle
// Created:     06/05/2023 (19/04/2023)
// Updated:
// Copyright:   (c) Axle 2023
// Licence:     MIT-0 No Attribution
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <errno.h>
//#include <math.h>
// Windows APIs are a limited subset of UNIX. aka UNIX APIs do not always
// have an mscrt equivilent capability. Some features of UNIX are available
// using UNIX emulation layers such as QT, MSYS2 amd WSL. These UNIX subsystems
// require a slightly diferent standard library tool chain (headers.h) as well
// as the subsystem shared objects to be available, but don't provide a direct
// conversion from UNIX. Note that the UNIX subsystem libraries are quite large
// and often introduce unnecessary overheads on Windows, so it is better to
// write for the native windows APIs or MinGW which connects directly to
// the Windows CRT unless you are attempting to port a specific UNIX app to
// use on Windows and have no other options.
// This also applies to other languages such as Python.
#ifdef __unix__ // _linux__ (__linux__)
// On Unix this is a standard library header.
#include <unistd.h>
// If used on Windows it has a different meaning and definitions relating to
// the UNIX subsystems for windows and is non standard..
#endif
#include "sqlite3.h"

int sqlite3_get_version2(char *ret_version);

int main(int argc, char *argv[])
    {
    char ver_buffer[32] = {'\0'};
    //sqlite3 *db;  // database handle (structure).
    //sqlite3_stmt *stmt;  // structure represents a single SQL statement
    //const char *data = NULL;
    int return_code = 0;

    // Get our SQLite version. Confirmation that sqlite 3 is installed as a
    // shared library and compiling/working correctly.
    // Ensure that sqlite3.dll is in the system path or in the working directory
    // of the project executable at run-time.
    // NOTE: I am using the C API interface directly and not as a query. SQLite
    // provides a limited number of helper MACROS that can be accessed directly
    // with out opening a databse.
    printf("1 SQLite Version:%s\n", sqlite3_libversion());
    printf("===========================================\n");

    // Long version check with error returns.
    // This shows the basic steps of an SQLite 3 query statement using an in
    // memory temporary database (:memory:) to get the version number.
    return_code = sqlite3_get_version2(ver_buffer);
    if (return_code == 0)
        {
        printf("Version error return = %d\n", return_code);
        }
    else if (return_code == 1)
        {
        printf("2 SQLite Version:%s\n", ver_buffer);
        }
    else  // == -1
        {
        printf("An internal error occured.\n");
        }
    printf("===========================================\n");

    printf("Press Enter to continue...\n");
    getchar();
    return 0;
    }  // END main()

// Modified from:
// https://zetcode.com/db/sqlitec/
// Get SQLite version - query. (long function)
// Returns string to ret_version buffer, as well as int sqlite error codes.
int sqlite3_get_version2(char *ret_version)
    {
    sqlite3 *p_db;  // database handle (structure).
    sqlite3_stmt *statement;  // structure represents a single SQL statement
    int return_code = 0;  // API result codes and error codes.
    // Result and error codes can be found here:
    // https://www.sqlite.org/rescode.html

    // return_code is the return error codes.
    // Note: :memory: can be used instead of a file for temporary database
    // operations.
    return_code = sqlite3_open(":memory:", &p_db);  // Open Memory (RAM) data base.
    if (return_code != SQLITE_OK)  // int 0
        {
        fprintf(stderr, "Cannot open database: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        return -1;
        }

    // START single SQLite3 query statement ==================================>>
    // Only a single query statement can be executed at a time. The 3 functions
    // sqlite3_prepare_v2, sqlite3_step and sqlite3_finalize must be used as
    // a group in a routine for each sqlite query.
    // Prepare -> Do Query -> Finalise and commit.

    // Before an SQL statement is executed, it must be first compiled into a
    // byte-code with one of the sqlite3_prepare* functions.
    // The sqlite3_prepare_v2 function takes five parameters. The first parameter
    // is the database handle obtained from the sqlite3_open function. The second
    // parameter is the SQL statement to be compiled. The third parameter is the
    // maximum length of the SQL statement measured in bytes.
    // -1 causes the SQL string to be read up to the first zero terminator which
    // is the end of the string here. (or supply the exact no of bytes.)
    // The fourth parameter is the statement handle. It will point to the
    // pre-compiled statement if the sqlite3_prepare_v2 runs successfully. The last
    // parameter is a pointer to the unused portion of the SQL statement. Only
    // the first statement of the SQL string is compiled, so the parameter points
    // to what is left un-compiled. We pass 0 since the parameter is not important
    // for us SEE: sqlite3_clear_bindings(stmt);.
    return_code = sqlite3_prepare_v2(p_db, "SELECT SQLITE_VERSION()", -1, &statement, 0);
    // On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    // is returned.
    if (return_code != SQLITE_OK)
        {
        // This is error handling code for the sqlite3_prepare_v2 function call.
        fprintf(stderr, "Failed to prepare data: %s | %s\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    // The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    // that there is another row ready. Our SQL statement returns only one row
    // of data in this case, therefore, we call this function only once.
    // If we expected multiple lines of data (rows, columns) we would need to
    // recover each table cel as a step within a loop until end of data
    // (!=SQLITE_ROW).
    return_code = sqlite3_step(statement);
    if (return_code == SQLITE_ROW)
        {
        // const unsigned char *sqlite3_column_text(sqlite3_stmt*, int iCol);
        // iCol refers to the current column in the return data. In this case
        // there is only one column of return value, so we know the zero column
        // contains the version number.
        strcpy(ret_version, (const char*)sqlite3_column_text(statement, 0));
        }
    else
        {
        fprintf(stderr, "Step error: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return 0;
        }

    // The sqlite3_finalize function destroys the prepared statement object and
    // commits the changes to the databse file.
    return_code = sqlite3_finalize(statement);
    if (return_code != SQLITE_OK)
        {
        // This is error handling code.
        fprintf(stderr, "Failed to finalize data: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    // The sqlite3_close function closes the database connection.
    return_code = sqlite3_close(p_db);
    if (return_code != SQLITE_OK)
        {
        // This is error handling code.
        fprintf(stderr, "Failed to close database: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        return -1;
        }

    return 1;
    }

// See the Basic SQLite 3 example source code library provided with the book.
