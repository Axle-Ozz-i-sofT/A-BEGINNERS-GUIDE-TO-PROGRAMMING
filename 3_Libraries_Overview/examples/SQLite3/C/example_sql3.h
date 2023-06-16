//------------------------------------------------------------------------------
// Name:        example_sql3.c (based upon basics_1.c)
// Purpose:     SQLite3 basic examples.
//              Convenience wrapper functions for SQLite version 3.
//
// Platform:    Win64, Ubuntu64
// Depends:     v3.34.1 plus
//
// Author:      Axle
// Created:     03/05/2023 )19/04/2023)
// Updated:     28/05/2023
// Copyright:   (c) Axle 2023
// Licence:     MIT-0 No Attribution
//------------------------------------------------------------------------------
// Notes:
//
// !!! All routines and examples are based upon tables using only TEXT (String)
// type field entries. The only exception is the system generated rowid.
// See NOTE: ... below !!!
//
// To use multiple type field entries a "linked list" data structure is required
// to be able to accept mixed data types in C.
// SEE: sqlite3_column_type(statement, i) examples.
//
//
// NOT using sqlite3_exec() with callback. All returns are handled in the
// calling routines.
//
// There is no set or defined rule in the standards for what error value of
// error returns must be in an application function. It is implementation
// defined and can differ from function to function. The only (most common)
// rule is returning a value of 0==Success from main() to the OS. This can also
// be written as return EXIT_SUCCESS; and return EXIT_FAILURE;
//
// Function returns can be any value, so I have used a mix of error return
// schemes depending upon the return type of the functions routines. in most
// cases 0, -1, 1, 2 will either define success, fail or a specific error.
// In a commercial application we would more likely return the error code of
// SQLite3 and handle the sql error code directly.
// https://www.sqlite.org/rescode.html
// I have over simplified the returns to a basic Success-TRUE, Fail-FALSE scheme
// for simplicity of the examples.
//
// The routines and functions use an excessive amount of error reporting. Normally
// we only handle actual errors and do so silently in the background of our
// application. I have created error info on both successful returns as well as
// on errors only as a visual guide. If using the sqlit3_open* and sqlite2_close*
// directly from your main application you can also use the sqlite3_errmsg()
// directly. Another option is to return the integer value of sqlite3_errmsg()
// from your function and handle the error from the calling statement as an sql
// error rather than the 0==False(fail), 1==True(success) etc. that I have made
// up for the examples.
//
// Error returns can be handled in a number of ways. I have created my own
// error returns of 0, 1, -1 etc as well as displaying the SQLite3 error codes
// in the functions. This is excessive and only there to exemplify the different
// error returns. In practice we would return the error code number to our
// calling statement. sqlite3_errmsg(p_db) or rc
// return strcpy((char)error_return, sqlite3_errmsg(p_db)); or
// return (int)rc;
// Typically sqlite3_initialize(), sqlite3_open_v2(), sqlite3_close(), sqlite3_shutdown()
// would be called from main() where we would be able to retrieve the last
// sqlite3_errmsg(p_db) directly. We wouldn't open and close the database file
// for every query as it is more appropriate to keep it open for as long as
// repetitive transactions are taking place.
//
// In light of the above take note of the 3 different ways of "Opening" an
// sqlite database file.
// SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_READONLY
// Each has pros and cons with regard to safety when opening a database file. For
// example we may not wish to create a new database file if the wrong file name
// is entered and prefer an error return "p_db file not found" instead.
//
//
// SQLite3 can only accept 1 command statement at a time. To run multiple
// statements they must use the sqlite3_prepare_v2, sqlite3_step, sqlite3_finalize
// in a loop for each statement. This can be done by sending a formatted array of
// statements and sending each separately in a loop or as a loop in the function.
// This would also be true when sending query statements that have multiple returns
// where an appropriate sized array will need to be supplied.
//
//
// data is stored internally as bytes by default. The length of the bytes and
// the affinity (data type) is stored in the header entry of the data.
// Although stored as bytes, this data could be of any data type.
//
// Internally SQLite 3 stores all data as BYTEs and is recovered as a number of
// BYTEs. The type affinity is used to convert the bytes back to the storage type
// associated with the cell in that column.
//
// The different data types are to allow code conformity with other SQL database
// engines which have static typing. Keeping this in mind it is acceptable to
// use only TEXT for most data storage. Where a numeric value is required
// internally by sqlite such as column or row ID number then I would suggest
// using INTEGER PRIMARY KEY if needed. You will need to INSERT and and SELECT
// providing the correct data type containers in this case.
// I am using TEXT only tables to simplify the examples. You cane see a
// prototype for the more complex mixed data types in the final example.

// In reality most applications will take user input as text, data read from a
// document as text and even transport most data as text. (All data is text in
// python unless specifically stated for example.) Remember that even user
// numeric keyboard input is text until converted to its integer representation
// by the programming language. For example we may take an input from a user in
// C language to a variable int my_number = getchar(); The text arrives from
// the keyboard as a hexadecimal representation of the character '5' as 0x35
// (ASCII decimal 53), int my_text_number = getchar() = 53
// To use it as an actual integer we need to convert the character to an
// integer (53 - 48) int 0 = char dec48 - 48 ( SEE and ASCII chart),
// so to get the integer value (5) from the text character'5' we need to convert
// int my_interger_number = (my_text_number(53 '5') - 48); (== 5)
// Note my use of inverted commas character '5' (hex 0x35) vs integer 5 (hex 0x05)
// to represent character '5' and integer 5.
//
// The above is primarily suggested for a small database app without complex data
// structures as we would know in advance the data type required for each column.
// For a more accurate (and complex) example see the last function example in this
// wrapper library db_list_table_all_types().
//
// White space between column values or after the ',' delimiter will need to be
// managed by the calling application. Alternatively you can modify any routine
// that reads from the database and change strcat(buffer, ", ") to ",".
// ## Removed/Changed all returns from ", " to ","
//
// I am treating this database example in a similar way to a CSV data file
// storage which is less efficient, but I wanted to keep the examples simple
// even if a little slower and less optimised than they would be in commercial
// practice. Ultimately it is up to the programmer to choose the most
// appropriate method of SQLite3 usage for there requirements.
//
// Handling NULL values. The following is a simple example of handling NULL
// pointer values from an empty column entry. In this example I have replaced
// non-existing value with text "[NULL]". This is just an example and we can
// deal with this return in any way that is appropriate for the context of our
// application. I have not handled NULL returns in the examples.
// data = (const char*)sqlite3_column_text( p_stmt, 0 );
//        printf( "%s\n", data ? data : "[NULL]" );
//
// Note! Some keywords are reserved for SQLIte, for example "rowid".
//
// Using concatenate strcat() can lead to SQL injection! SEE Parameterised
// statements and int sqlite3_bind_text(sqlite3_stmt*,int,const char*,int,void(*)(void*));
// https://www.sqlite.org/c3ref/bind_blob.html
//------------------------------------------------------------------------------
// Credits:
// https://resources.oreilly.com
// https://zetcode.com/p_db/sqlitec/
// https://gist.github.com/jsok/2936764
// ++
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
// TODO:
// Convenience wrapper functions for SQLite version 3 [Done]
// Remove excess comments.[ Done]
// Move sqlite open/close to main(). [?]
// Return sqlite errors to calling functions? [?]
// alt change sqlite errors to better value set?
//
// Two generalised functions to take and return table data? [Not done]
// I may include a universal function at the end of this document that will
// take most sqlite3 commands and return the appropriate arrays of data.
// Send command/s
// Retrieve command/s
//
// Mark extra error returns as DEBUG. [Done]
// Revise db_delete_table_rowdata()
// Revise db_insert_table_rowdata_rowid() for error handling while loop.
//
// Check array off by 1s.
// db_insert_table_rowdata_rowid() Fails with non contiguous rowids. Get next
// rowid rather than using 1 to n rowid numbers.
//------------------------------------------------------------------------------

#ifndef EXAMPLE_SQL3_H
#define EXAMPLE_SQL3_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <errno.h>
//#include <math.h>
//#include <conio.h>
// Windows APIs are a limited subset of UNIX. aka UNIX APIs do not always
// have an mscrt equivalent capability. Some features of UNIX are available
// using UNIX emulation layers such as QT, MSYS2 and WSL. These UNIX subsystems
// require a slightly different standard library tool chain (headers.h) as well
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

//=========================

// Mixed types(affinity) data structures
// this structure keeps track of the number of elements of binary data stored
// within the tagVARIANT structure under tagVARIANT.bval.*
// tagVARIANT.bval.blen holds the length of bytes in tagVARIANT.bval.bdata[]
// this could be written as tagVARIANT.bval.bdata[tagVARIANT.bval.blen] to
// denote the total number of data bytes.
//
// tagVARIANT.tval must be assigned as a string with the trailing '\0' zero
// terminator to retrieve the length of string, else the length of the array
// must also be returned.
typedef struct struct_bval
    {
    int blen;  // Length of bin data
    unsigned char bdata[30720];  // bin data MAX length 30,720 bytes, 30KiB.
    //unsigned char *bdata = NULL;  // realloc when defining struct tagVARIANT
    } struct_bval;

// Data structures, unions ( data type VARIANT )
// Types:NULL,INTEGER,REAL,TEXT,BLOB (SQLite 3)
// Types: NULL, int, double, char(string), unsigned char or void* (C)
// sqlite3_column_type() returned value type:
// affinities:
// SQLITE_NULL, SQLITE_INTEGER, SQLITE_FLOAT, SQLITE_TEXT, SQLITE_BLOB
// Note:NUMERIC can hold any data type as an integer but is an affinity
// rather than a type. In most instances NUMERIC will convert to int or float.
// I have not used NUMERIC.
// "tagged union"
typedef struct tagVARIANT
    {
    // My made up affinities used in this C structure.
    enum { IS_NULL = 5, IS_INTEGER = 1, IS_FLOAT = 2, IS_TEXT = 3, IS_BLOB = 4 } type;
    union
        {
        void *vval;        // NULL (pointer to) this will denote an empty element.
        int ival;          // INTEGER
        double rval;       // REAL
        char tval[30720];   // TEXT (Max string (row) length 30,720, 30KiB)
        //char *tval = NULL;  // realloc when defining struct tagVARIANT
        struct_bval bval;  // BLOB (binary) structure struct_bval (INT, UCHAR)
        } value;
    } tagVARIANT;


// Note all fields/column name are TEXT typed except for the system rowid.
// If you wish to use INTEGER or other types you will need rewrite the functions
// accordingly. See final examples using BLOBS and mixed data types.
// Values cannot be empty(NULL) for any column.

// Get SQLite3 Version.
int sqlite3_get_version0(void);
// Get SQLite3 Version.
void sqlite3_get_version1(char *ret_version);
// Get SQLite3 Version.
int sqlite3_get_version2(char *ret_version);

// Check if "FileName" exists.
int file_exists(char *db_file_name);
// Check IF "Sqlite 3" database TRUE.
int db_file_exists(char *db_file_name);

// Create empty SQLite 3 database.
int db_file_create(char *db_file_name);
// Delete "FileName".
int db_file_delete(char *db_file_name);

// Check "TableName exists".
int db_table_exists(char *db_file_name, char *db_table_name);

// The only difference between the following 2 functions is the first enumerates
// all tables in the named database file and returns a total count without
// returning the actual table names. We need this count to create a dynamic
// array of the correct size to hold the actual table names for the second function.
// Get the total number of tables in a database.
int db_get_number_tables(char *db_file_name, int *number_tables_ret);
// List all table names to dynamic array the size of return from previous function.
int db_get_tablenames(char *db_file_name, char **db_tablenames);

// Create "TableName" (If NOT Exists). Will create p_db "FileName" if not exists.
int db_table_create(char *db_file_name, char *db_table_name);  // Revise?
// Delete "TableName" if exists.
int db_table_delete(char *db_file_name, char *db_table_name);

// Get number of rows in "TableName".
int db_get_table_number_rows(char *db_file_name, char *db_table_name, int *number_rows_ret);
// Get number of columns in "TableName".
int db_get_table_number_cols(char *db_file_name, char *db_table_name, int *number_cols_ret);
// Get array of column names. List all column names to array as array[col number][column name]
int db_get_table_colnames(char *db_file_name, char *db_table_name, char **db_tbl_col_name);

// Add new row after last rowid.
int db_insert_table_rowdata(char *db_file_name, char *db_tbl_entry);
// Delete row/s using search terms (dangerous). better to confirm rowid!
int db_delete_table_rowdata(char *db_file_name, char *db_row_entry);
// List all rows to array as array[rowid][row data as csv]
int db_list_table_rows_data(char *db_file_name, char *db_table_name, char **db_tbl_data, int number_columns);

// Test if rowid exist in a table.
int db_table_rowid_exists(char *db_file_name, char *db_table_name, int tbl_rowid);
// delete row by "rowid". Following rows will all shift -1 rowid.
int db_delete_table_rowdata_rowid(char *db_file_name, char *db_table_name, int sql_rowid);
// update/replace by rowid. Replace an existing ebtry in place.
int db_replace_table_rowdata_rowid(char *db_file_name, char *db_table_name, int sql_rowid, char *db_field_names, char *db_field_values);
// insert to rowid (all previous and following data moved down one rowid) (Not recommended).
int db_insert_table_rowdata_rowid(char *db_file_name, char *db_table_name, int sql_rowid, char *db_field_names, char *db_field_values, int number_columns,int number_rows);
// read row from rowid
int db_read_table_rowdata_rowid(char *db_file_name, char *db_table_name, int sql_rowid, char *db_tbl_rowid_data, int number_columns);

// returns the number of rows "Found" in returned array...
// Search TableName by field/column Name using search word. Returns array of rows found (prefixed with rowid).
int db_search_table_rowdata_byfield(char *db_file_name, char *db_table_name, char **db_tbl_row_search, char *field_name, char *db_search_string, int number_columns, int *ret_array_length);
// Search TableName by ALL field/column Name using search word. Return array of rows found (prefixed with rowid). aka full table search
int db_search_table_rowdata_allfields(char *db_file_name, char *db_table_name, char **db_tbl_row_search, char **db_tbl_col_name, char *db_search_string, int number_columns, int number_rows, int *ret_array_length);

// This is the final and more advanced SQLite3 query using multiple data types VARIANT.
// I have used a binary BLOB entry for the example.
int db_insert_table_rowdata_bin(char *db_file_name, char *db_tbl_entry, void *bin_data, int bin_data_len);
// int more universal query function for mixed data types.
int db_list_table_all_types(char *db_file_name, char *db_table_name, tagVARIANT **variant_structure, int number_columns, int number_rows, int *ret_number_fields, int *ret_number_elements);

// ====> Convenience helper functions (Not really required)
int Con_Sleep(int seconds); // Cross platform sleep()
int Con_Clear(void); // Cross platform clear console screen
void S_Pause(void); // Cross platform console pause until Enter
void S_Clear_Input_Buffer(void); // Safe Clear the Input Buffer of '\n' after f/scanf() ###
int S_getchar(void); // Cross platform safe get character (clears the input buffer)
// See also safe Input() function. Functions.c

//==============================================================================

// Get SQLite version. (short function)
// Returns integer version. v3.34.1 = 034001 = 3|034|001
// Mmmmppp, with M being the major version, m the minor, and p the point release.
int sqlite3_get_version0(void)
    {
    return sqlite3_libversion_number();
    }

// Get SQLite version. (short function)
// Returns string to version buffer.
// We can call a number of the SQLite C APIs without needing to open a database.
// The SQLite3.dll library is connected to our application at startup.
void sqlite3_get_version1(char *ret_version)
    {
    strcpy(ret_version, sqlite3_libversion());
    }

// https://zetcode.com/p_db/sqlitec/
// Get SQLite version - query. (long function)
// Returns string to version buffer, as well as int sqlite error codes.
int sqlite3_get_version2(char *ret_version)
    {
    sqlite3 *p_db;  // database handle (structure).
    sqlite3_stmt *p_stmt;  // structure represents a single SQL statement
    int return_code = 0;

    char * db_filename_ram = ":memory:";
    // return_code is the return error codes.
    // Note: :memory: can be used instead of a file for temporary database
    // operations.
    // defaults to: [SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE]
    return_code = sqlite3_open(db_filename_ram, &p_db);  // Open Memory (RAM) data base.
    if (return_code != SQLITE_OK)  // int 0
        {
        fprintf(stderr, "Cannot open database: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

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
    // for us SEE: sqlite3_clear_bindings(p_stmt);.
    return_code = sqlite3_prepare_v2(p_db, "SELECT SQLITE_VERSION()", -1, &p_stmt, 0);
    // On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    // is returned.
    if (return_code != SQLITE_OK)
        {
        // This is error handling code for the sqlite3_prepare_v2 function call.
        fprintf(stderr, "Failed to prepare data: %s\n", sqlite3_errmsg(p_db));  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }


    // The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    // that there is another row ready. Our SQL statement returns only one row
    // of data, therefore, we call this function only once.
    return_code = sqlite3_step(p_stmt);
    if (return_code != SQLITE_ROW)
        {
        fprintf(stderr, "Step error: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return 0;
        }

    strcpy(ret_version, (const char*)sqlite3_column_text(p_stmt, 0));

    //sqlite3_bind_*()  // After sqlit3_prepare_v2()
    //sqlite3_clear_bindings(p_stmt);  // clears leftover statements from sqlite3_prepare.

    // The sqlite3_finalize function destroys the prepared statement object.
    return_code = sqlite3_finalize(p_stmt);
    if (return_code != SQLITE_OK)
        {
        // This is error handling code.
        fprintf(stderr, "Failed to finalize data: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    // The sqlite3_close function closes the database connection.
    sqlite3_close(p_db);
    if (return_code != SQLITE_OK)
        {
        // This is error handling code.
        fprintf(stderr, "Failed to close database: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    return 1;
    }

//==============================================================================

// Check if a file name exists by opening the file for read operations. If the
// file does exist it will not be created and return an error.
// 0 == False | 1 == True
// This is a standard file operation and not part of SQLite.
int file_exists(char *file_name)
    {
    FILE *fp = fopen(file_name, "r");
    if (fp == NULL)
        {
        fprintf(stderr, "Cannot open file %s.\n", file_name);  // DEBUG
        //fclose(fp);
        return 0;
        }

    fclose(fp);
    return 1;
    }


// Check if file exist and is an sqlite3 database.
// Looks for "SQLite format 3" in the 100 byte header of the file.
// SEE: db_file_create()
int db_file_exists(char *db_file_name)
    {
    int cnt_chr = 0;
    int char_buffer;
    char t_buffer[8] = {'\0'};
    char header[128] = {'\0'};  // sqlite3 header = 100 characters.
    FILE *fp = fopen(db_file_name, "rb");
    if (fp == NULL)
        {
        fprintf(stderr, "Cannot open file %s.\n", db_file_name);  // DEBUG
        //fclose(fp);// close the file.
        return 0;
        }
    else
        {
        while(char_buffer != EOF)
            {
            char_buffer = fgetc(fp);
            // The SQLite 3 header is exactly 100 (0 - 99) bytes long immediately followed
            // by a newline char '\n' == Dec 13 == LF at 101 characters.
            if((char_buffer == '\n') ||(char_buffer == '\r') || (cnt_chr > 105))// Test if we have encountered a new line and,
                {
                break;
                }

            cnt_chr++;
            // The first 16 bytes is a string denoting "SQLite format 3". So
            // we will keep the first 16 bytes and convert it to a string.
            if (cnt_chr < 16)  // co
                {
                sprintf(t_buffer, "%c", char_buffer);  // DEBUG
                strcat(header, t_buffer);
                }
            }
        }

    if (cnt_chr > 100)  // Check if we found more than 100 bytes?
        {
        fprintf(stderr, "Header too long. %d chrs.\n", cnt_chr);  // DEBUG
        fclose(fp);// close the file.
        return 0;
        }
    else
        {
        // Test if the string == "SQLite format 3". If true it should be a
        // genuine SQLite 3 database file.
        char *search = "SQLite format 3";
        if (strstr(header, search) == NULL)  // NULL == 0 (type char*)
            {
            fprintf(stderr, "\"SQLite format 3\" Header not found!\n");  // DEBUG
            fclose(fp);// close the file.
            return 0;
            }
        }

    fclose(fp);// close the file.
    return 1;
    }

//==============================================================================
// These are really the same routine for most database operations :)
// You could use this same routine with a little modification to accept and
// return most sqlite3 database queries.
//
// To create a table with the correct SQLite 3 header file information we will
// need to create a table and then delete (DROP) the table. This will leave a p_db
// file with the first 16 bytes containing "SQLite format 3". We can then use
// our previous db_file_exists() function to test if it is a valid p_db file.
//
// This function runs 2 separate SQLite queries. The first creates a temporary
// (dummy) table and the second query deletes the temporary table leaving an
// empty SQLite3 database file with a valid header.
int db_file_create(char *db_file_name)
    {
    sqlite3 *p_db;  // database handle (structure).
    sqlite3_stmt *p_stmt;  // structure represents a single SQL statement
    //const char *data = NULL;
    int return_code = 0;

    // If the databse name exists and is "SQLite format 3", don't try to creat a new p_db.
    if (db_file_exists(db_file_name))
        {
        fprintf(stderr, "Database: %s already exists!\n", db_file_name);  // DEBUG
        return 2;  // error codes needs to be reviewed.
        }

    return_code = sqlite3_open_v2( db_file_name, &p_db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL );
    if ( return_code != SQLITE_OK)
        {
        // Note: these print returns can be commented out if only the Bool return 0; is required.
        fprintf(stderr, "Can't open database: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close( p_db );
        return -1;
        }

    // This is a single query terminated by the ';' semi colon.
    // Multiple statements such as the following must be prepared, stepped and
    // finalised individually.
    // char *sql = "CREATE TABLE IF NOT EXISTS Temp_table(ID INT);"
    //             "DROP TABLE IF EXISTS Temp_table;";
    char *sql1 = "CREATE TABLE IF NOT EXISTS Temp_table(ID INTEGER);";

    //"create table aTable(field1 int); drop table aTable;
    // We can only send one query at a time to sqlite3.
    // sqlite3_prepare compiles the sql query into the byte code for sqlite.
    return_code = sqlite3_prepare_v2(p_db, sql1, -1, &p_stmt, 0);
    // On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    // is returned.
    if (return_code != SQLITE_OK)
        {
        // This is error handling code for the sqlite3_prepare_v2 function call.
        fprintf(stderr, "Failed to prepare data: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    // The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    // that there is another row ready. Our SQL statement returns only one row
    // of data in this instance, therefore, we call this function only once.
    // If we are writing or reading lines of table then we will need to use
    // sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.
    return_code = sqlite3_step( p_stmt );  // run once for one statement
    /*    while( sqlite3_step( p_stmt ) == SQLITE_ROW ) {;} */
    if (return_code != SQLITE_DONE)  // SQLITE_DONE==101, SQLITE_ROW==100
        {
        fprintf(stderr, "Step 1 failed: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return 0;
        }

    //sqlite3_bind_*()  // After sqlit3_prepare_v2()
    //sqlite3_clear_bindings(p_stmt);

    // The sqlite3_finalize function destroys the prepared statement object.
    // and commits the changes to the database file.
    return_code = sqlite3_finalize( p_stmt );  // Commit to the database.
    if (return_code != SQLITE_OK)  // SQLITE_OK==0
        {
        // This is error handling code.
        fprintf(stderr, "Failed to finalize data: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }


//==============================================================================
    // The following deletes the temporary table leaving the SQLite header files intact.

    // The database file is already open...
    /*    return_code = sqlite3_open_v2( db_file_name, &p_db, SQLITE_OPEN_READWRITE, NULL );
        if ( return_code != SQLITE_OK)
            {
            fprintf(stderr, "Can't open database: %s | %d\n", sqlite3_errmsg(p_db), return_code);
            sqlite3_close( p_db );
            return 0;
            }
    */
    char *sql2 = "DROP TABLE IF EXISTS Temp_table;";  // Delete the table.

    //"create table aTable(field1 int); drop table aTable;
    return_code = sqlite3_prepare_v2(p_db, sql2, -1, &p_stmt, 0);
    // On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    // is returned.
    if (return_code != SQLITE_OK)
        {
        // This is error handling code for the sqlite3_prepare_v2 function call.
        fprintf(stderr, "Failed to prepare data: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    // The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    // that there is another row ready. Our SQL statement returns only one row
    // of data in this instance, therefore, we call this function only once.
    // If we are writing or reading lines of table then we will need to use
    // sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.
    return_code = sqlite3_step( p_stmt );
    /*    while( sqlite3_step( p_stmt ) == SQLITE_ROW ) {;}*/
    if (return_code != SQLITE_DONE)  // SQLITE_DONE==101, SQLITE_ROW==100
        {
        fprintf(stderr, "Step 2 failed: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return 0;
        }

    //sqlite3_bind_*()  // After sqlit3_prepare_v2()
    //sqlite3_clear_bindings(p_stmt);

    // The sqlite3_finalize function destroys the prepared statement object.
    return_code = sqlite3_finalize( p_stmt );  // Commit to the database.
    if (return_code != SQLITE_OK)  // SQLITE_OK==0
        {
        // This is error handling code.
        fprintf(stderr, "Failed to finalize data: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    // The sqlite3_close function closes the database connection.
    return_code = sqlite3_close(p_db);   // SQLITE_OK==0
    if (return_code != SQLITE_OK)
        {
        // This is error handling code. NOTE! As p_db is closed the error code may not be available!
        fprintf(stderr, "Failed to close database: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }


    fprintf(stderr, "Successfully created %s DQLite3 database.\n", db_file_name);  // DEBUG

    return 1;
    }

// ####### This needs to be reviewed !!!!
// Delete the SQLite3 database file, This is really just a standard file delete
// function with an extra safety y/n check. We could also include db_file_exists()
// to test if it is a valid SQLite database that is being deleted. Always
// include some form of safety check before deleting any files on a users system
// as file deletes cannot be undone.
int db_file_delete(char *db_file_name)
    {
    int return_code = 0;

    printf("Are you sure you want to delete %s (y/n) ?\n", db_file_name);
    return_code = S_getchar();
    if (( return_code == 'y') || ( return_code == 'Y'))
        {

        return_code = remove(db_file_name);
        if(return_code == 0)
            {
            fprintf(stderr, "Successfully deleted %s.\n", db_file_name);  // DEBUG
            return 1;
            }
        else
            {
            fprintf(stderr, "Unable to delete or not exists %s.\n", db_file_name);  // DEBUG
            return -1;
            }
        }
    else  // any not 'y'
        {
        fprintf(stderr, "File %s aborted by user.\n", db_file_name);  // DEBUG
        return 0;  // Aborted delete file.
        }


    return 0;
    }

// Test if a table name exist in a database.
// https://fossil-scm.org/home/info/4da2658323cab60e?ln=1945-1951
// https://www.sqlite.org/c3ref/table_column_metadata.html
int db_table_exists(char *db_file_name, char *db_table_name)
    {

    sqlite3 *p_db;     // database handle (structure).
    int return_code = 0;      // sqlite3 return codes.
    int err_ret = 0; // This function error codes.

    return_code = sqlite3_open_v2( db_file_name, &p_db, SQLITE_OPEN_READONLY, NULL );
    if ( return_code != SQLITE_OK)
        {
        // Note: these print returns can be commented out if only the Bool return 0; is required.
        fprintf(stderr, "Can't open database: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close( p_db );
        return -1;
        }

// Alternative method.
// https://cppsecrets.com/users/1128311897114117110109107110505764103109971051084699111109/C00-SQLitesqlite3free.php
// char *sql = "SELECT count(*) FROM sqlite_master WHERE type = 'table' AND name = 'YourTableName';";
// const char *sql = "SELECT 1 FROM sqlite_master where type='table' and name=?";  <--

// List all table names <- New function
// !! Recheck with the following !!!
   /*
    const char* sql_table_list = "SELECT name FROM sqlite_master WHERE type='table'";

    return_code = sqlite3_prepare_v2(p_db, sql_table_list, strlen(sql_table_list), &statement, NULL);
    if(return_code == SQLITE_OK)
        {
        // Loop through all the tables
        while(sqlite3_step(statement) == SQLITE_ROW)
            {
            if(!strcmp((const char*) sqlite3_column_text(statement, 0), table_name))
                return true;
            }
        }
    */

    // ##### This may need a loop!!!!
    // Currently only tested with single table!!!
    // db_handle, db_name, db_table_name, col_name, NULL, ...
    return_code = sqlite3_table_column_metadata(p_db, NULL, db_table_name, NULL, NULL, NULL, NULL, NULL, NULL);
    if (return_code != SQLITE_OK )
        {
        fprintf(stderr, "Table did not exist:  %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        err_ret = 0;
        }
    else
        {
        fprintf(stderr, "Table exists: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        err_ret = 1;
        }

    // The sqlite3_close function closes the database connection.
    return_code = sqlite3_close(p_db);   // SQLITE_OK==0
    if (return_code != SQLITE_OK)
        {
        // This is error handling code. NOTE! As p_db is closed the error code may not be available!
        fprintf(stderr, "Failed to close database: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    return err_ret;  //Return this function error code.
    }

// Get the total number of tables in a named database file.
int db_get_number_tables(char *db_file_name, int *number_tables_ret)
    {

    sqlite3 *p_db;  // database handle (structure).
    sqlite3_stmt *p_stmt;  // structure represents a single SQL statement
    int return_code = 0;
    char buffer[128] = {'\0'};  // temp buffer [MAX 128 characters]
    strcpy(buffer, "");  // before strcat()
    int table_count = 0;

    return_code = sqlite3_open_v2( db_file_name, &p_db, SQLITE_OPEN_READONLY, NULL );
    if ( return_code != SQLITE_OK)
        {
        // Note: these print returns can be commented out if only the Bool return 0; is required.
        fprintf(stderr, "Can't open database: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close( p_db );
        return -1;
        }

    const char* sql_table_list = "SELECT * FROM sqlite_schema WHERE type='table' AND name NOT LIKE 'sqlite_%' ORDER BY 1;";  // sqlite_schema, sqlite_master

    return_code = sqlite3_prepare_v2(p_db, sql_table_list, -1, &p_stmt, NULL);
    // On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    // is returned.
    if (return_code != SQLITE_OK)
        {
        // This is error handling code for the sqlite3_prepare_v2 function call.
        fprintf(stderr, "Failed to prepare data: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    // The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    // that there is another row ready. Our SQL statement returns only one row
    // of data at a time.
    // If we are writing or reading multiple lines of table then we will need to
    // use sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.
    while(sqlite3_step(p_stmt) == SQLITE_ROW)  // SQLITE_ROW
        {
        strcat(buffer, (const char*)sqlite3_column_text(p_stmt, 1));
        table_count++;  // Count the number of table names found.
        }

    *number_tables_ret = table_count;  // populate the return buffer.

    //sqlite3_bind_*()  // After sqlit3_prepare_v2()
    //sqlite3_clear_bindings(p_stmt);

    // The sqlite3_finalize function destroys the prepared statement object.
    return_code = sqlite3_finalize( p_stmt );  // Commit to the database.
    if (return_code != SQLITE_OK)  // SQLITE_OK==0
        {
        // This is error handling code.
        fprintf(stderr, "Failed to finalize data: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    // The sqlite3_close function closes the database connection.
    return_code = sqlite3_close(p_db);   // SQLITE_OK==0
    if (return_code != SQLITE_OK)
        {
        // This is error handling code. NOTE! As p_db is closed the error code may not be available!
        fprintf(stderr, "Failed to close database: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    return 1;  //Return this function error code.
    }


// The same as above function, except this time we return the names of each table
// in the database file.
int db_get_tablenames(char *db_file_name, char **db_tablenames)
    {

    sqlite3 *p_db;  // database handle (structure).
    sqlite3_stmt *p_stmt;  // structure represents a single SQL statement
    int return_code = 0;
    char buffer[128] = {'\0'};  // temp buffer [MAX 128 characters]
    strcpy(buffer, "");  // before strcat()
    int table_count = 0;  // Needed to assign the data to each array element.

    return_code = sqlite3_open_v2( db_file_name, &p_db, SQLITE_OPEN_READONLY, NULL );
    if ( return_code != SQLITE_OK)
        {
        // Note: these print returns can be commented out if only the Bool return 0; is required.
        fprintf(stderr, "Can't open database: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close( p_db );
        return -1;
        }

    const char* sql_table_list = "SELECT * FROM sqlite_schema WHERE type='table' AND name NOT LIKE 'sqlite_%' ORDER BY 1;";  // sqlite_schema, sqlite_master

    return_code = sqlite3_prepare_v2(p_db, sql_table_list, -1, &p_stmt, NULL);
    // On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    // is returned.
    if (return_code != SQLITE_OK)
        {
        // This is error handling code for the sqlite3_prepare_v2 function call.
        fprintf(stderr, "Failed to prepare data: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    // Loop through all of the tables.
    while(sqlite3_step(p_stmt) == SQLITE_ROW)
        {
        strcat(db_tablenames[table_count], (const char*)sqlite3_column_text(p_stmt, 1));
        table_count++;  // Update the next array data element.
        }

    // *number_tables_ret = table_count;  // populate the return buffer.

    //sqlite3_bind_*()  // After sqlit3_prepare_v2()
    //sqlite3_clear_bindings(p_stmt);

    // The sqlite3_finalize function destroys the prepared statement object.
    return_code = sqlite3_finalize( p_stmt );  // Commit to the database.
    if (return_code != SQLITE_OK)  // SQLITE_OK==0
        {
        // This is error handling code.
        fprintf(stderr, "Failed to finalize data: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    // The sqlite3_close function closes the database connection.
    return_code = sqlite3_close(p_db);   // SQLITE_OK==0
    if (return_code != SQLITE_OK)
        {
        // This is error handling code. NOTE! As p_db is closed the error code may not be available!
        fprintf(stderr, "Failed to close database: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    return 1;  //Return this function error code.
    }

// Insert a table with column identifiers into a database.
// This will NOT create a database file if it does not already exist.
// Must use db_file_create()
// This will NOT overwrite a previous table of the same name if it exists.
// I may need to create a test or modify this function for safety.
int db_table_create(char *db_file_name, char *db_table)
    {
    sqlite3 *p_db;  // database handle (structure).
    sqlite3_stmt *p_stmt;  // structure represents a single SQL statement
    //const char *data = NULL;
    int return_code = 0;


    return_code = sqlite3_open_v2( db_file_name, &p_db, SQLITE_OPEN_READWRITE, NULL );  // SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE
    if ( return_code != SQLITE_OK)
        {
        // Note: these print returns can be commented out if only the Bool return 0; is required.
        fprintf(stderr, "Can't open database: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close( p_db );
        return -1;
        }

    // In this function I have formatted the entire query (statement) prior to
    // sending it to the function *db_table. As you will se in following
    // examples we can also create a generic template for the statement and
    // only send the column names and value data to be constructed into a full
    // query using string concatenation or sqlite3_bind*().

    //"CREATE TABLE IF NOT EXISTS TableName (Col_Name TYPE);"
    // Note: If the table exist already no error is returned.
    // We can only send one query at a time to sqlite3.
    return_code = sqlite3_prepare_v2(p_db, db_table, -1, &p_stmt, 0);
    // On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    // is returned.
    if (return_code != SQLITE_OK)
        {
        // This is error handling code for the sqlite3_prepare_v2 function call.
        fprintf(stderr, "Failed to prepare data: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    // The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    // that there is another row ready. Our SQL statement returns only one row
    // of data at a time.
    // If we are writing or reading multiple lines of table then we will need to
    // use sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.
    return_code = sqlite3_step( p_stmt );  // run once for one statement
    /*    while( sqlite3_step( p_stmt ) == SQLITE_ROW ) {;} */
    if (return_code != SQLITE_DONE)  // SQLITE_DONE==101, SQLITE_ROW==100
        {
        fprintf(stderr, "Step failed: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return 0;
        }

    //sqlite3_bind_*()  // After sqlit3_prepare_v2()
    //sqlite3_clear_bindings(p_stmt);

    // The sqlite3_finalize function destroys the prepared statement object.
    return_code = sqlite3_finalize( p_stmt );  // Commit to the database.
    if (return_code != SQLITE_OK)  // SQLITE_OK==0
        {
        // This is error handling code.
        fprintf(stderr, "Failed to finalize data: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    // The sqlite3_close function closes the database connection.
    return_code = sqlite3_close(p_db);   // SQLITE_OK==0
    if (return_code != SQLITE_OK)
        {
        // This is error handling code. NOTE! As p_db is closed the error code may not be available!
        fprintf(stderr, "Failed to close database: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }


    fprintf(stderr, "Successfully created table in %s.\n", db_file_name);  // DEBUG

    return 1;
    }

// Delete (DROP) table from a named database file.
// Will return -1 if the database file does not exists.
int db_table_delete(char *db_file_name, char *db_table_name)
    {
    sqlite3 *p_db;  // database handle (structure).
    sqlite3_stmt *p_stmt;  // structure represents a single SQL statement
    //const char *data = NULL;
    int return_code = 0;

    return_code = sqlite3_open_v2( db_file_name, &p_db, SQLITE_OPEN_READWRITE, NULL );
    if ( return_code != SQLITE_OK)
        {
        // Note: these print returns can be commented out if only the Bool return 0; is required.
        fprintf(stderr, "Can't open database: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close( p_db );
        return -1;
        }

    // In this function I have formatted the entire query (statement) prior to
    // sending it to the function *db_table. As you will see in following
    // examples we can also create a generic template for the statement and
    // only send the column names and value data to be constructed into a full
    // query using string concatenation or sqlite3_bind*().

    // "DROP TABLE IF EXISTS TableName;";
    // We can only send one query at a time to sqlite3.
    return_code = sqlite3_prepare_v2(p_db, db_table_name, -1, &p_stmt, 0);
    // On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    // is returned.
    if (return_code != SQLITE_OK)
        {
        // This is error handling code for the sqlite3_prepare_v2 function call.
        fprintf(stderr, "Failed to prepare data: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    // The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    // that there is another row ready. Our SQL statement returns only one row
    // of data at a time.
    // If we are writing or reading multiple lines of table then we will need to
    // use sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.
    return_code = sqlite3_step( p_stmt );  // run once for one statement
    /*    while( sqlite3_step( p_stmt ) == SQLITE_ROW ) {;} */
    if (return_code != SQLITE_DONE)  // SQLITE_DONE==101, SQLITE_ROW==100
        {
        fprintf(stderr, "Step failed: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return 0;
        }

    //sqlite3_bind_*()  // After sqlit3_prepare_v2()
    //sqlite3_clear_bindings(p_stmt);

    // The sqlite3_finalize function destroys the prepared statement object.
    return_code = sqlite3_finalize( p_stmt );  // Commit to the database.
    if (return_code != SQLITE_OK)  // SQLITE_OK==0
        {
        // This is error handling code.
        fprintf(stderr, "Failed to finalize data: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    // The sqlite3_close function closes the database connection.
    return_code = sqlite3_close(p_db);   // SQLITE_OK==0
    if (return_code != SQLITE_OK)
        {
        // This is error handling code. NOTE! As p_db is closed the error code may not be available!
        fprintf(stderr, "Failed to close database: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    fprintf(stderr, "Successfully deleted table %s in %s.\n", db_table_name, db_file_name);  // DEBUG

    return 1;
    }


//============================================================================>>

// Get the number of rows from a table.
int db_get_table_number_rows(char *db_file_name, char *db_table_name, int *number_rows_ret)
    {

    sqlite3 *p_db;  // database handle (structure).
    sqlite3_stmt *p_stmt;  // structure represents a single SQL statement
    //const char *data = NULL;
    int return_code = 0;
    int row_cnt = 0;

    return_code = sqlite3_open_v2( db_file_name, &p_db, SQLITE_OPEN_READONLY, NULL );
    if ( return_code != SQLITE_OK)
        {
        // Note: these print returns can be commented out if only the Bool return 0; is required.
        fprintf(stderr, "Can't open database: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close( p_db );
        return -1;
        }

    // Unlike previous functions, I am using a template to create the statement.
    // I am concatenating the additional information to form the full query
    // statement. This allows us to use a generic template providing only the
    // necessary data. See info in sql injection.

    // SELECT COUNT(*) FROM TableName;  // Returns rows. COUNT(*|ALL|DISTINCT] expression)
    // Alternative select max(rowid) from TableName;
    // See Column name search and counts for the column count usage.
    char db_row_search[128] = "SELECT COUNT(*) FROM ";  // Don't use long table name max[128 - 22]
    strcat( db_row_search, db_table_name);
    strcat( db_row_search, ";");

    // We can only send one query at a time to sqlite3.
    return_code = sqlite3_prepare_v2(p_db, db_row_search, -1, &p_stmt, 0);
    // On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    // is returned.
    if (return_code != SQLITE_OK)
        {
        // This is error handling code for the sqlite3_prepare_v2 function call.
        fprintf(stderr, "Failed to prepare data: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    // The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    // that there is another row ready. Our SQL statement returns only one row
    // of data at a time.
    // If we are writing or reading multiple lines of table then we will need to
    // use sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.
    return_code = sqlite3_step(p_stmt);
    if (return_code != SQLITE_ROW)
        {
        fprintf(stderr, "Step failed: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    row_cnt = sqlite3_column_int(p_stmt, 0);  // Retrieve the row count as int.
    *number_rows_ret = row_cnt;  // Populate int number_rows from calling function.

    //sqlite3_bind_*()  // After sqlit3_prepare_v2()
    //sqlite3_clear_bindings(p_stmt);

    // The sqlite3_finalize function destroys the prepared statement object.
    return_code = sqlite3_finalize( p_stmt );  // Commit to the database.
    if (return_code != SQLITE_OK)  // SQLITE_OK==0
        {
        // This is error handling code.
        fprintf(stderr, "Failed to finalize data: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    // The sqlite3_close function closes the database connection.
    return_code = sqlite3_close(p_db);   // SQLITE_OK==0
    if (return_code != SQLITE_OK)
        {
        // This is error handling code. NOTE! As p_db is closed the error code may not be available!
        fprintf(stderr, "Failed to close database: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    fprintf(stderr, "Successfully retrieved table row number from %s.\n", db_file_name);  // DEBUG

    return 1;
    }

// Get number of columns in a TableName.
int db_get_table_number_cols(char *db_file_name, char *db_table_name, int *number_cols_ret)
    {

    sqlite3 *p_db;  // database handle (structure).
    sqlite3_stmt *p_stmt;  // structure represents a single SQL statement
    //const char *data = NULL;
    int return_code = 0;
    int col_cnt = 0;

    return_code = sqlite3_open_v2( db_file_name, &p_db, SQLITE_OPEN_READWRITE, NULL );
    if ( return_code != SQLITE_OK)
        {
        // Note: these print returns can be commented out if only the Bool return 0; is required.
        fprintf(stderr, "Can't open database: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close( p_db );
        return -1;
        }

    char db_col_search[128] = "SELECT COUNT(*) FROM pragma_table_info(\"";  // Don't use long table name max[128 - 43]
    strcat( db_col_search, db_table_name);
    strcat( db_col_search, "\");");

    // We can only send one query at a time to sqlite3.
    return_code = sqlite3_prepare_v2(p_db, db_col_search, -1, &p_stmt, 0);
    // On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    // is returned.
    if (return_code != SQLITE_OK)
        {
        // This is error handling code for the sqlite3_prepare_v2 function call.
        fprintf(stderr, "Failed to prepare data: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    // The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    // that there is another row ready. Our SQL statement returns only one row
    // of data at a time.
    // If we are writing or reading multiple lines of table then we will need to
    // use sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.
    return_code = sqlite3_step(p_stmt);
    if (return_code != SQLITE_ROW)
        {
        fprintf(stderr, "Step failed: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    col_cnt = sqlite3_column_int(p_stmt, 0);  // Retrieve the row count as int.
    *number_cols_ret = col_cnt;  // Populate int number_rows from calling function.

    //sqlite3_bind_*()  // After sqlit3_prepare_v2()
    //sqlite3_clear_bindings(p_stmt);

    // The sqlite3_finalize function destroys the prepared statement object.
    return_code = sqlite3_finalize( p_stmt );  // Commit to the database.
    if (return_code != SQLITE_OK)  // SQLITE_OK==0
        {
        // This is error handling code.
        fprintf(stderr, "Failed to finalize data: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    // The sqlite3_close function closes the database connection.
    return_code = sqlite3_close(p_db);   // SQLITE_OK==0
    if (return_code != SQLITE_OK)
        {
        // This is error handling code. NOTE! As p_db is closed the error code may not be available!
        fprintf(stderr, "Failed to close database: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    fprintf(stderr, "Successfully retrieved table column number from %s.\n", db_file_name);  // DEBUG

    return 1;
    }

// Get the column names as 2D array from a named table.
int db_get_table_colnames(char *db_file_name, char *db_table_name, char **db_tbl_col_name)
    {

    sqlite3 *p_db;  // database handle (structure).
    sqlite3_stmt *p_stmt;  // structure represents a single SQL statement
    //const char *data = NULL;
    int return_code = 0;


    return_code = sqlite3_open_v2( db_file_name, &p_db, SQLITE_OPEN_READONLY, NULL );
    if ( return_code != SQLITE_OK)
        {
        // Note: these print returns can be commented out if only the Bool return 0; is required.
        fprintf(stderr, "Can't open database: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close( p_db );
        return -1;
        }

    // There are other methods to do this, so this statement can be revised.
    char db_row_search2[128] = "PRAGMA table_info('";  // Don't use long table name max[128 - 22]
    strcat( db_row_search2, db_table_name);
    strcat( db_row_search2, "');");

    // We can only send one query at a time to sqlite3.
    return_code = sqlite3_prepare_v2(p_db, db_row_search2, -1, &p_stmt, 0);
    // On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    // is returned.
    if (return_code != SQLITE_OK)
        {
        // This is error handling code for the sqlite3_prepare_v2 function call.
        fprintf(stderr, "Failed to prepare data: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    int col_cnt = 0;  // Count number of columns.

    // The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    // that there is another row ready. Our SQL statement returns only one row
    // of data at a time.
    // If we are writing or reading multiple lines of table then we will need to
    // use sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.
    while (sqlite3_step(p_stmt) != SQLITE_DONE)
        {
        // To handle the return of NULL pointers as a string
        // data = (const char*)sqlite3_column_text( p_stmt, i );
        //        printf( "%s\n", data ? data : "[NULL]" );
        //        sprintf(buffer[strlen(data)], "%s\n", data ? data : "[NULL]" );

        strcat(db_tbl_col_name[col_cnt], (const char*)sqlite3_column_text(p_stmt, 1));
        // db_tbl_col_name[col_cnt][String_MAX_Length (2048)]
        col_cnt++;
        }

    //sqlite3_bind_*()  // After sqlit3_prepare_v2()
    //sqlite3_clear_bindings(p_stmt);

    // The sqlite3_finalize function destroys the prepared statement object.
    return_code = sqlite3_finalize( p_stmt );  // Commit to the database.
    if (return_code != SQLITE_OK)  // SQLITE_OK==0
        {
        // This is error handling code.
        fprintf(stderr, "Failed to finalize data: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    // The sqlite3_close function closes the database connection.
    return_code = sqlite3_close(p_db);   // SQLITE_OK==0
    if (return_code != SQLITE_OK)
        {
        // This is error handling code. NOTE! As p_db is closed the error code may not be available!
        fprintf(stderr, "Failed to close database: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    fprintf(stderr, "Successfully retrieved table column names from %s.\n", db_file_name);  // DEBUG

    return 1;
    }

//============================================================================<<


// Insert row data into a named table.
// If the data already exists will create a new row.
// Table unique index rowid is auto generated to the next available position
// in this table.
int db_insert_table_rowdata(char *db_file_name, char *db_tbl_entry) // add insert at row_id
    {
    sqlite3 *p_db;  // database handle (structure).
    sqlite3_stmt *p_stmt;  // structure represents a single SQL statement
    //const char *data = NULL;
    int return_code = 0;

    return_code = sqlite3_open_v2( db_file_name, &p_db, SQLITE_OPEN_READWRITE, NULL );
    if ( return_code != SQLITE_OK)
        {
        // Note: these print returns can be commented out if only the Bool return 0; is required.
        fprintf(stderr, "Can't open database: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close( p_db );
        return -1;
        }

    // In this function I am supplying to full query statement to the function.
    // You can alter this based upon other function using templates :)

    // We can only send one query at a time to sqlite3.
    return_code = sqlite3_prepare_v2(p_db, db_tbl_entry, -1, &p_stmt, 0);
    // On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    // is returned.
    if (return_code != SQLITE_OK)
        {
        // This is error handling code for the sqlite3_prepare_v2 function call.
        fprintf(stderr, "Failed to prepare data: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    // The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    // that there is another row ready. Our SQL statement returns only one row
    // of data at a time.
    // If we are writing or reading multiple lines of table then we will need to
    // use sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.
    return_code = sqlite3_step( p_stmt );  // run once for one statement
    /*    while( sqlite3_step( p_stmt ) == SQLITE_ROW ) {;} */
    if (return_code != SQLITE_DONE)  // SQLITE_DONE==101, SQLITE_ROW==100
        {
        fprintf(stderr, "Step failed: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return 0;
        }

    //sqlite3_bind_*()  // After sqlit3_prepare_v2()
    //sqlite3_clear_bindings(p_stmt);

    // The sqlite3_finalize function destroys the prepared statement object.
    return_code = sqlite3_finalize( p_stmt );  // Commit to the database.
    if (return_code != SQLITE_OK)  // SQLITE_OK==0
        {
        // This is error handling code.
        fprintf(stderr, "Failed to finalize data: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    // The sqlite3_close function closes the database connection.
    return_code = sqlite3_close(p_db);   // SQLITE_OK==0
    if (return_code != SQLITE_OK)
        {
        // This is error handling code. NOTE! As p_db is closed the error code may not be available!
        fprintf(stderr, "Failed to close database: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    fprintf(stderr, "Successfully inserted rowdata into table in %s.\n", db_file_name);  // DEBUG

    return 1;
    }


// Delete the row data searched by name matching etc.
// ## Only for tables where all field types are TEXT (String) except rowid. ##
// This function is dangerous and needs to be revised!!!!
int db_delete_table_rowdata(char *db_file_name, char *db_row_entry) // add delete at row_id
    {
    sqlite3 *p_db;  // database handle (structure).
    sqlite3_stmt *p_stmt;  // structure represents a single SQL statement
    //const char *data = NULL;
    int return_code = 0;

    return_code = sqlite3_open_v2( db_file_name, &p_db, SQLITE_OPEN_READWRITE, NULL );
    if ( return_code != SQLITE_OK)
        {
        // Note: these print returns can be commented out if only the Bool return 0; is required.
        fprintf(stderr, "Can't open database: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close( p_db );
        return -1;
        }

    // We can only send one query at a time to sqlite3.
    return_code = sqlite3_prepare_v2(p_db, db_row_entry, -1, &p_stmt, 0);
    // On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    // is returned.
    if (return_code != SQLITE_OK)
        {
        // This is error handling code for the sqlite3_prepare_v2 function call.
        fprintf(stderr, "Failed to prepare data: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    // The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    // that there is another row ready. Our SQL statement returns only one row
    // of data at a time.
    // If we are writing or reading multiple lines of table then we will need to
    // use sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.
    return_code = sqlite3_step( p_stmt );  // run once for one statement
    /*    while( sqlite3_step( p_stmt ) == SQLITE_ROW ) {;} */
    if (return_code != SQLITE_DONE)  // SQLITE_DONE==101, SQLITE_ROW==100
        {
        fprintf(stderr, "Step failed: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return 0;
        }

    //sqlite3_bind_*()  // After sqlit3_prepare_v2()
    //sqlite3_clear_bindings(p_stmt);

    // The sqlite3_finalize function destroys the prepared statement object.
    return_code = sqlite3_finalize( p_stmt );  // Commit to the database.
    if (return_code != SQLITE_OK)  // SQLITE_OK==0
        {
        // This is error handling code.
        fprintf(stderr, "Failed to finalize data: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    // The sqlite3_close function closes the database connection.
    return_code = sqlite3_close(p_db);   // SQLITE_OK==0
    if (return_code != SQLITE_OK)
        {
        // This is error handling code. NOTE! As p_db is closed the error code may not be available!
        fprintf(stderr, "Failed to close database: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    fprintf(stderr, "Successfully removed rowdata from table in %s.\n", db_file_name);  // DEBUG

    return 1;
    }


// List all data from a named table to a dynamic array.
// ## Only for tables where all field types are TEXT (String) except rowid. ##
int db_list_table_rows_data(char *db_file_name, char *db_table_name, char **db_tbl_rowdata, int number_columns)
    {

    number_columns += 1;  // We are also including an extra column for the row ID number.
    sqlite3 *p_db;  // database handle (structure).
    sqlite3_stmt *p_stmt;  // structure represents a single SQL statement
    //const char *data = NULL;
    int return_code = 0;

    return_code = sqlite3_open_v2( db_file_name, &p_db, SQLITE_OPEN_READONLY, NULL );
    if ( return_code != SQLITE_OK)
        {
        // Note: these print returns can be commented out if only the Bool return 0; is required.
        fprintf(stderr, "Can't open database: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close( p_db );
        return -1;
        }

    char sql_concat[128] = {'\0'};
    strcpy(sql_concat, "");

    // SELECT rowid, * FROM // To include table row ID number.
    char *sql = "SELECT rowid, * FROM ";  // Note the space after FROM

    strcat(sql_concat, sql);  // Add SQL query statement.
    strcat(sql_concat, db_table_name);  // Add table name to statement.

    // char *db_table1 = "DROP TABLE IF EXISTS Hrs_worked_Tracker;";
    //"create table aTable(field1 int); drop table aTable;
    // We can only send one query at a time to sqlite3.
    return_code = sqlite3_prepare_v2(p_db, sql_concat, -1, &p_stmt, 0);
    // On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    // is returned.
    if (return_code != SQLITE_OK)
        {
        // This is error handling code for the sqlite3_prepare_v2 function call.
        fprintf(stderr, "Failed to prepare data: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    // The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    // that there is another row ready. Our SQL statement returns only one row
    // of data at a time, therefore, we call this function only once at a time.
    // If we are writing or reading multiple lines of table then we will need to
    // use sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.

    int cnt_row = 0;  // array row.
    int cnt_col = 0;
    char buffer[128] = {'\0'};  // temp buffer [MAX 128 characters]

    while (sqlite3_step(p_stmt) == SQLITE_ROW)
        {
        // Can use sqlite3_column_count(statement/p_stmt) in place of number_columns.
        for (cnt_col = 0; cnt_col < number_columns; cnt_col++)  // Count 0 to number_columns-1
            {
            // To handle the return of NULL pointers as a string.
            // data = (const char*)sqlite3_column_text( p_stmt, cnt_col );
            //        printf( "%s\n", data ? data : "[NULL]" );
            //        sprintf(buffer[strlen(data)], "%s\n", data ? data : "[NULL]" );

            // copy each entry to a buffer. Each entry is a column.
            strcpy(buffer, (const char*)sqlite3_column_text(p_stmt, cnt_col));  // cnt_col == array column.

            // Concatenate buffer to our return array.
            strcat(db_tbl_rowdata[cnt_row], buffer);
            if (cnt_col < number_columns -1) // Dont add ', ' after last column.
                {
                strcat(db_tbl_rowdata[cnt_row], ",");  // add separator token between each col.
                }
            else  // Add line return, end of row.
                {
                ;//strcat(db_tbl_rowdata[cnt_row], "\n");
                }
            //cnt_col++;  // Need 3D array.
            }

        cnt_row++;
        }

    //sqlite3_bind_*()  // After sqlit3_prepare_v2()
    //sqlite3_clear_bindings(p_stmt);

    // The sqlite3_finalize function destroys the prepared statement object.
    return_code = sqlite3_finalize( p_stmt );  // Commit to the database.
    if (return_code != SQLITE_OK)  // SQLITE_OK==0
        {
        // This is error handling code.
        fprintf(stderr, "Failed to finalize data: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    // The sqlite3_close function closes the database connection.
    return_code = sqlite3_close(p_db);   // SQLITE_OK==0
    if (return_code != SQLITE_OK)
        {
        // This is error handling code. NOTE! As p_db is closed the error code may not be available!
        fprintf(stderr, "Failed to close database: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    fprintf(stderr, "Successfully retrieved table data from %s.\n", db_file_name);  // DEBUG

    return 1;
    }


// Test if rowid exist in a table.
int db_table_rowid_exists(char *db_file_name, char *db_table_name, int tbl_rowid)
    {
    sqlite3 *p_db;  // database handle (structure).
    sqlite3_stmt *p_stmt;  // structure represents a single SQL statement
    //const char *data = NULL;
    int return_code = 0;
    int err_ret = 0;

    return_code = sqlite3_open_v2( db_file_name, &p_db, SQLITE_OPEN_READONLY, NULL );
    if ( return_code != SQLITE_OK)
        {
        // Note: these print returns can be commented out if only the Bool return 0; is required.
        fprintf(stderr, "Can't open database: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close( p_db );
        return -1;
        }

    char sql_concat[512] = {'\0'};  // temp buffer [MAX 128 characters]
    char rowid_buffer[24] = {'\0'};  // convert int to char for statement concat.
    // Clear the buffer from the last statement. strcat() will concat on to
    // the previous statement otherwise.
    strcpy(sql_concat, "");

    sprintf(rowid_buffer, "%d", tbl_rowid); // convert rowid int to string.

    // SELECT EXISTS(SELECT 1 FROM myTbl WHERE WHERE rowid = tbl_rowid);
    // "SELECT rowid, * FROM "

    // Both of the following queries will return a correct result.
    char *sql1 = "SELECT EXISTS(SELECT 1 FROM ";
    char *sql2 = " WHERE rowid = ";
    char *sql3 = ");";

    //char *sql1 = "SELECT Count() FROM ";
    //char *sql2 = " WHERE rowid = ";
    //char *sql3 = ";";

    strcat(sql_concat, sql1);  // Add SQL query statement.
    strcat(sql_concat, db_table_name);  // Add SQL query statement.
    strcat(sql_concat, sql2);  // Add SQL query statement.
    strcat(sql_concat, rowid_buffer);  // Add SQL query statement.
    strcat(sql_concat, sql3);

    // We can only send one query at a time to sqlite3.
    return_code = sqlite3_prepare_v2(p_db, sql_concat, -1, &p_stmt, 0);
    // On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    // is returned.
    if (return_code != SQLITE_OK)
        {
        // This is error handling code for the sqlite3_prepare_v2 function call.
        fprintf(stderr, "Failed to prepare data: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    return_code = sqlite3_step( p_stmt );  // run once for one statement
    if (return_code != SQLITE_ROW)  // SQLITE_DONE==101, SQLITE_ROW==100
        {
        fprintf(stderr, "Step failed: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    return_code = sqlite3_column_int(p_stmt, 0);
    if (return_code == 0)
        {
        err_ret = 0;
        }
    else  // ==1
        {
        err_ret = 1;
        }

    //sqlite3_bind_*()  // After sqlit3_prepare_v2()
    //sqlite3_clear_bindings(p_stmt);

    // The sqlite3_finalize function destroys the prepared statement object.
    return_code = sqlite3_finalize( p_stmt );  // Commit to the database.
    if (return_code != SQLITE_OK)  // SQLITE_OK==0
        {
        // This is error handling code.
        fprintf(stderr, "Failed to finalize data: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    // The sqlite3_close function closes the database connection.
    return_code = sqlite3_close(p_db);   // SQLITE_OK==0
    if (return_code != SQLITE_OK)
        {
        // This is error handling code. NOTE! As p_db is closed the error code may not be available!
        fprintf(stderr, "Failed to close database: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    return err_ret;
    }


// delete row by "rowid"
// Delete the row data by rowid.
int db_delete_table_rowdata_rowid(char *db_file_name, char *db_table_name, int sql_rowid)
    {
    sqlite3 *p_db;  // database handle (structure).
    sqlite3_stmt *p_stmt;  // structure represents a single SQL statement
    //const char *data = NULL;
    int return_code = 0;

    return_code = sqlite3_open_v2( db_file_name, &p_db, SQLITE_OPEN_READWRITE, NULL );
    if ( return_code != SQLITE_OK)
        {
        // Note: these print returns can be commented out if only the Bool return 0; is required.
        fprintf(stderr, "Can't open database: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close( p_db );
        return -1;
        }

    char sql_concat[512] = {'\0'};  // temp buffer [MAX 128 characters]
    char rowid_buffer[24] = {'\0'};  // convert int to char for statement concat.
    // Clear the buffer from the last statement. strcat() will concat on to
    // the previous statement otherwise.
    strcpy(sql_concat, "");

    sprintf(rowid_buffer, "%d", sql_rowid); // convert rowid int to string.

    //"DELETE FROM TableName WHERE rowid = n;"

    char *sql1 = "DELETE FROM ";  //
    char *sql2 = " WHERE rowid = ";  //
    char *sql3 = ";";  //

    // This can be replaced with sprintf()
    strcat(sql_concat, sql1);  // Add SQL query statement.
    strcat(sql_concat, db_table_name);  // Add table name to statement.
    strcat(sql_concat, sql2);  // Add " WHERE rowid =  "
    strcat(sql_concat, rowid_buffer);  // Add rowid.
    strcat(sql_concat, sql3);  // Finish the statement with ";"

    // We can only send one query at a time to sqlite3.
    return_code = sqlite3_prepare_v2(p_db, sql_concat, -1, &p_stmt, 0);
    // On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    // is returned.
    if (return_code != SQLITE_OK)
        {
        // This is error handling code for the sqlite3_prepare_v2 function call.
        fprintf(stderr, "Failed to prepare data: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    // The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    // that there is another row ready. Our SQL statement returns only one row
    // of data at a time, therefore, we call this function only once at a time.
    // If we are writing or reading multiple lines of table then we will need to
    // use sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.
    return_code = sqlite3_step( p_stmt );  // run once for one statement
    //    while( sqlite3_step( p_stmt ) == SQLITE_ROW ) {;}
    if (return_code != SQLITE_DONE)  // SQLITE_DONE==101, SQLITE_ROW==100
        {
        fprintf(stderr, "Step failed: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return 0;
        }

    //sqlite3_bind_*()  // After sqlit3_prepare_v2()
    //sqlite3_clear_bindings(p_stmt);

    // The sqlite3_finalize function destroys the prepared statement object.
    return_code = sqlite3_finalize( p_stmt );  // Commit to the database.
    if (return_code != SQLITE_OK)  // SQLITE_OK==0
        {
        // This is error handling code.
        fprintf(stderr, "Failed to finalize data: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    // The sqlite3_close function closes the database connection.
    return_code = sqlite3_close(p_db);   // SQLITE_OK==0
    if (return_code != SQLITE_OK)
        {
        // This is error handling code. NOTE! As p_db is closed the error code may not be available!
        fprintf(stderr, "Failed to close database: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    fprintf(stderr, "Successfully removed rowdata into table in %s.\n", db_file_name);  // DEBUG

    return 1;
    }


// update/replace row data by rowid in a named table.
// If the data already exists it will "overwrite" the row.
int db_replace_table_rowdata_rowid(char *db_file_name, char *db_table_name, int sql_rowid, char *db_field_names, char *db_field_values)
//int db_insert_table_rowdata(char *db_file_name, char *db_tbl_entry) // add insert at row_id
    {
    sqlite3 *p_db;  // database handle (structure).
    sqlite3_stmt *p_stmt;  // structure represents a single SQL statement
    //const char *data = NULL;
    int return_code = 0;

    return_code = sqlite3_open_v2( db_file_name, &p_db, SQLITE_OPEN_READWRITE, NULL );
    if ( return_code != SQLITE_OK)
        {
        // Note: these print returns can be commented out if only the Bool return 0; is required.
        fprintf(stderr, "Can't open database: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close( p_db );
        return -1;
        }

    char sql_concat[512] = {'\0'};  // temp buffer [MAX 128 characters]
    char rowid_buffer[24] = {'\0'};  // convert int to char for statement concat.
    // Clear the buffer from the last statement. strcat() will concat on to
    // the previous statement otherwise.
    strcpy(sql_concat, "");

    sprintf(rowid_buffer, "%d", sql_rowid); // convert int to str

    // "Week, Employee_ID, Name, Monday, Tuesday, Wednesday, Thursday, Friday";
    // "\"2\", \"36\", \"Bill Knight\", \"2\", \"3\", \"4\", \"5\", \"6\"";

    // Our template for building the query statement.
    char *sql1 = "REPLACE INTO ";  //
    char *sql2 = " (rowid, ";  //
    char *sql3 = ") VALUES(";  //
    char *sql4 = ", ";
    char *sql5 = ");";

    // This can be replaced with sprintf()
    strcat(sql_concat, sql1);  // Add SQL query statement.
    strcat(sql_concat, db_table_name);  // Add table name to statement.
    strcat(sql_concat, sql2);  // Add " ("
    strcat(sql_concat, db_field_names);  // Add field name (column name).
    strcat(sql_concat, sql3);  // Add ") VALUES("
    strcat(sql_concat, rowid_buffer);  // convert rowid int to str char
    strcat(sql_concat, sql4);
    strcat(sql_concat, db_field_values);  // Add last part of query statement.
    strcat(sql_concat, sql5);  // Finish the statement with ");"

    // We can only send one query at a time to sqlite3.
    return_code = sqlite3_prepare_v2(p_db, sql_concat, -1, &p_stmt, 0);
    // On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    // is returned.
    if (return_code != SQLITE_OK)
        {
        // This is error handling code for the sqlite3_prepare_v2 function call.
        fprintf(stderr, "Failed to prepare data: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    // The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    // that there is another row ready. Our SQL statement returns only one row
    // of data at a time, therefore, we call this function only once at a time.
    // If we are writing or reading multiple lines of table then we will need to
    // use sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.
    return_code = sqlite3_step( p_stmt );  // run once for one statement
    /*    while( sqlite3_step( p_stmt ) == SQLITE_ROW ) {;} */
    if (return_code != SQLITE_DONE)  // SQLITE_DONE==101, SQLITE_ROW==100
        {
        fprintf(stderr, "Step failed: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return 0;
        }

    //sqlite3_bind_*()  // After sqlit3_prepare_v2()
    //sqlite3_clear_bindings(p_stmt);

    // The sqlite3_finalize function destroys the prepared statement object.
    return_code = sqlite3_finalize( p_stmt );  // Commit to the database.
    if (return_code != SQLITE_OK)  // SQLITE_OK==0
        {
        // This is error handling code.
        fprintf(stderr, "Failed to finalize data: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    // The sqlite3_close function closes the database connection.
    return_code = sqlite3_close(p_db);   // SQLITE_OK==0
    if (return_code != SQLITE_OK)
        {
        // This is error handling code. NOTE! As p_db is closed the error code may not be available!
        fprintf(stderr, "Failed to close database: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    fprintf(stderr, "Successfully inserted rowdata into table in %s.\n", db_file_name);  // DEBUG

    return 1;
    }


// Insert row data into a named table at rowid. (Not recommended)
// I had some issues with non contiguous rowid numbers. I have created a test
// flag to skip empty rowid. Empty row id remain unchanged and all other
// filled rows are moved down one slot. This is a bit hackish and not the
// best method. We could use VACUUM to make the rowid index contiguous before
// each routine requiring rowid manipulation, or we can copy the table to
// a memory file with contiguous rowid, or last copy the enter table to our
// application memory and perform the table tasks there before re-writing
// the table fresh.
//
// The current data at the rowid is written into a temporary buffer, the new row
// data is then written to that rowid.
// Each row is "shuffled" down one rowid at at time until the last rowid.
// The final row of data is then inserted into a new (empty) rowid at the end
// of the table.
//
// Note this is considered poor practice in data base management as data is
// generally not stored in any particular order and it is up to the calling
// application to sort the data according to rules such as "By date" or "By Name".
//
// In a large data base file this will be a slow task and could be prone to errors.
// It can also disrupt the association of the rowid between different tables
// in a relational database.
//
// Table unique index rowid is auto generated in this table.
int db_insert_table_rowdata_rowid(char *db_file_name, char *db_table_name, int sql_rowid, char *db_field_names, char *db_field_values, int number_columns,int number_rows)
    {
    sqlite3 *p_db;  // database handle (structure).
    sqlite3_stmt *p_stmt;  // structure represents a single SQL statement
    //const char *data = NULL;
    int return_code = 0;  // Error and return codes.
    // Test and skip missing rowid between read buffer and write buffer.
    // It's a bit messy and complicated :/
    int step_ret = 0;  // Used for flags in missing rowid.
    int Count_Rows = sql_rowid;  // start counting from the first insert rowid.
    int test_rowid = 0;  // Used for flags in missing rowid.
    int W_Flag = 0;  //  Used for flags in missing rowid.


    char buffer[128] = {'\0'};  // Temp column read buffer [MAX 128 characters]
    // This 2 dimension array will hold the temporary data for the rows being read
    // and then writen to the next line.
    char db_field_values_temp[2][2048] = {'\0'};  // buffer to hold each line to move down +1

    // This is the new data (row) to be inserted at the rowid.
    strcpy(db_field_values_temp[0], db_field_values);  // Copy first (new row) to R/W buffer.

    char sql_concat[512] = {'\0'};  // temp buffer [MAX 512 characters]
    char rowid_buffer[24] = {'\0'};  // convert int to char for statement concat.

    return_code = sqlite3_open_v2( db_file_name, &p_db, SQLITE_OPEN_READWRITE, NULL );
    if ( return_code != SQLITE_OK)
        {
        // Note: these print returns can be commented out if only the Bool return 0; is required.
        fprintf(stderr, "Can't open database: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close( p_db );
        return -1;
        }

    // Starts at rowid up to last row + 1. +2 alows for the '<' ie. +1
    while (Count_Rows < number_rows +2)
        {

        //================================================= Start step 1 read rowid
        // rowid copy to string
        sprintf(rowid_buffer, "%d", sql_rowid); // convert int to str

        // The +1 at the end of the table is empty and will create a read error,
        // so we will skip this and go directly to writing the last +1 new line
        // directly from the buffer.
        if (Count_Rows < number_rows +1)
            {
            // strcat() must be proceeded by a strcpy, We must also clear the
            //buffer with an empty string with "" for each read iteration.
            strcpy(sql_concat, "");
            // "SELECT rowid, * FROM "  // Note that I am ignoring the rowid
            // I need to work from rowid to reorganise the table.

            //char *sql1 = "SELECT * FROM ";  // Note the space after FROM
            char *sql1 = "SELECT rowid, * FROM ";  // Note the space after FROM
            char *sql2 = " WHERE rowid = ";  //
            char *sql3 = ";";  //

            strcat(sql_concat, sql1);  // Add SQL query statement.
            strcat(sql_concat, db_table_name);  // Add table name to statement.
            strcat(sql_concat, sql2);  // Add filter.
            strcat(sql_concat, rowid_buffer);  // This is the first replace rowid.
            strcat(sql_concat, sql3);  // Finish the sql statement

            // We can only send one query at a time to sqlite3.
            return_code = sqlite3_prepare_v2(p_db, sql_concat, -1, &p_stmt, 0);
            // On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
            // is returned.
            if (return_code != SQLITE_OK)
                {
                // This is error handling code for the sqlite3_prepare_v2 function call.
                fprintf(stderr, "Read, Failed to prepare data: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
                sqlite3_close(p_db);
                return -1;
                }

            strcpy(db_field_values_temp[1], "");  // clear the row buffer for next read strcat()
            // All of the data is collected in a single string with each column
            // separated with the ',' delimiter.

            // This while isn't required as step only retrieves a single row.
            //while (sqlite3_step(p_stmt) == SQLITE_ROW)
            step_ret = sqlite3_step(p_stmt);

            test_rowid = sqlite3_column_int(p_stmt, 0);
            if (( test_rowid != 0) || (step_ret == SQLITE_ROW))
                {
                W_Flag = 0;  // Reset the wite flag if rowid has an entry.
                // Can use sqlite3_column_count(statement/p_stmt) in place of number_columns.
                // Changed to i = 1 to number_columns +1. The first rowid column is skipped.
                for (int i = 1; i < number_columns +1; i++)  // Count 0 to number_columns-1
                    {
                    // To handle the return of NULL pointers as a string
                    // data = (const char*)sqlite3_column_text( p_stmt, i );
                    //        printf( "%s\n", data ? data : "[NULL]" );
                    //        sprintf(buffer[strlen(data)], "%s\n", data ? data : "[NULL]" );

                    // Copy each entry to a buffer. Each entry is a col.
                    strcpy(buffer, (const char*)sqlite3_column_text(p_stmt, i));  // i == array column.

                    // concat buffer to our return array.
                    // Here we are reading the row into [1] of the temporary R/W buffer.
                    strcat(db_field_values_temp[1], "\"");
                    strcat(db_field_values_temp[1], buffer);
                    strcat(db_field_values_temp[1], "\"");
                    if (i < number_columns +1 -1) // Don't add ', ' after last column.
                        {
                        strcat(db_field_values_temp[1], ",");  // add separator token between each col.
                        }

                    }

                }
            else
                {
                // Skip read data on empty rowid.
                W_Flag++;  // Flag to write last line. Can use test_rowid instead.
                number_rows++;  // Correct the total row count to match last system rowid
                }

            //sqlite3_bind_*()  // After sqlit3_prepare_v2()  // DEBUG
            //sqlite3_clear_bindings(p_stmt);  // DEBUG

            // The sqlite3_finalize function destroys the prepared statement object.
            return_code = sqlite3_finalize( p_stmt );  // Commit to the database.
            if (return_code != SQLITE_OK)  // SQLITE_OK==0
                {
                // This is error handling code.
                fprintf(stderr, "Failed to finalize data: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
                sqlite3_close(p_db);
                return -1;
                }

            }  // END Skip reading +1 empty line
        //=========================================================== end step 1
        //================================================ Start step 2. Write rowid

        if (W_Flag < 1)  // Skip writing empty buffer data.
            {
            // Clear the buffer from the last statement. strcat() will concat on to
            // the previous statement otherwise.
            strcpy(sql_concat, "");

            char *sql01a = "REPLACE INTO ";  //
            char *sql01b = "INSERT INTO ";  //
            char *sql02a = " (rowid, ";  //
            char *sql02b = " (";  //
            char *sql03 = ") VALUES(";  //
            char *sql04 = ",";  // SQL as integer
            char *sql05 = ");";

            // Some logic to handle the last row inserted to a new rowid.
            if (Count_Rows < number_rows +2)
                {
                strcat(sql_concat, sql01a);  // "REPLACE INTO "
                }
            else  // New row (original last row +1)
                {
                strcat(sql_concat, sql01b);  // "INSERT INTO "
                }

            strcat(sql_concat, db_table_name);  // Add table name to statement.

            if (Count_Rows < number_rows +2)
                {
                strcat(sql_concat, sql02a);  // " (rowid, "
                }
            else
                {
                strcat(sql_concat, sql02b);  // " ("
                }

            strcat(sql_concat, db_field_names);  // Add field name (column name).

            if (Count_Rows < number_rows +2)
                {
                strcat(sql_concat, sql03);  // ") VALUES(\""
                strcat(sql_concat, rowid_buffer);  // This is the fist rowid for new values [As SQL INTEGER]
                strcat(sql_concat, sql04);  // delimit rowid, col_values, ...
                }
            else
                {
                strcat(sql_concat, sql03);  // ") VALUES("
                }

            // Add the last row read into the same rowid
            strcat(sql_concat, db_field_values_temp[0]);  // Add the values.
            strcat(sql_concat, sql05);  // Finish the statement with ");"


            // We can only send one query at a time to sqlite3.
            return_code = sqlite3_prepare_v2(p_db, sql_concat, -1, &p_stmt, 0);
            // On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
            // is returned.
            if (return_code != SQLITE_OK)
                {
                // This is error handling code for the sqlite3_prepare_v2 function call.
                fprintf(stderr, "Write, Failed to prepare data: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
                sqlite3_close(p_db);
                return -1;
                }

            // The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
            // that there is another row ready. Our SQL statement returns only one row
            // of data at a time, therefore, we call this function only once at a time.
            // If we are writing or reading multiple lines of table then we will need to
            // use sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.
            return_code = sqlite3_step( p_stmt );  // run once for one statement
            //    while( sqlite3_step( p_stmt ) == SQLITE_ROW ) {;}
            if (return_code != SQLITE_DONE)  // SQLITE_DONE==101, SQLITE_ROW==100
                {
                fprintf(stderr, "Step failed: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
                sqlite3_close(p_db);
                return 0;
                }

            //sqlite3_bind_*()  // After sqlit3_prepare_v2()
            //sqlite3_clear_bindings(p_stmt);

            // The sqlite3_finalize function destroys the prepared statement object.
            return_code = sqlite3_finalize( p_stmt );  // Commit to the database.
            if (return_code != SQLITE_OK)  // SQLITE_OK==0
                {
                // This is error handling code.
                fprintf(stderr, "Failed to finalize data: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
                sqlite3_close(p_db);
                return -1;
                }

            // Copy the last read [1] back to position [0] for next read write cycle.
            // This works a little like a last in first out buffer (LIFO).
            strcpy( db_field_values_temp[0], db_field_values_temp[1]);

            }

        sql_rowid++;  // Start read/write next rowid..
        Count_Rows++;  // So we don't count past the existing rows in the table.
        //============================================================== End step 2

        }  // Continue loop until last table row. (while Count_Rows < number_rows(++) +2:)


    // The sqlite3_close function closes the database connection.
    return_code = sqlite3_close(p_db);   // SQLITE_OK==0
    if (return_code != SQLITE_OK)
        {
        // This is error handling code. NOTE! As p_db is closed the error code may not be available!
        fprintf(stderr, "Failed to close database: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    fprintf(stderr, "Successfully inserted rowdata into table in %s.\n", db_file_name);  // DEBUG

    return 1;
    }


// Read row from rowid in named table.
int db_read_table_rowdata_rowid(char *db_file_name, char *db_table_name, int sql_rowid, char *db_tbl_rowid_data, int number_columns)
    {

    sqlite3 *p_db;  // database handle (structure).
    sqlite3_stmt *p_stmt;  // structure represents a single SQL statement
    //const char *data = NULL;
    int return_code = 0;

    char sql_concat[512] = {'\0'};  // temp buffer [MAX 512 characters]
    char buffer[128] = {'\0'};  // Temp column read buffer [MAX 128 characters]
    char rowid_buffer[24] = {'\0'};  // convert int to char for statement concat.

    return_code = sqlite3_open_v2( db_file_name, &p_db, SQLITE_OPEN_READONLY, NULL );
    if ( return_code != SQLITE_OK)
        {
        // Note: these print returns can be commented out if only the Bool return 0; is required.
        fprintf(stderr, "Can't open database: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close( p_db );
        return -1;
        }

    // rowid copy to string
    sprintf(rowid_buffer, "%d", sql_rowid);

    // strcat() must be proceeded by a strcpy, We must also clear the
    //buffer with an empty string with "" for each read iteration.
    strcpy(sql_concat, "");
    //char *sql1 = "SELECT rowid, * FROM ";  // Note the space after FROM
    char *sql1 = "SELECT * FROM ";  // Note the space after FROM
    char *sql2 = " WHERE rowid =  ";  //
    char *sql3 = ";";  //

    strcat(sql_concat, sql1);  // Add SQL query statement.
    strcat(sql_concat, db_table_name);  // Add table name to statement.
    strcat(sql_concat, sql2);  // Add filter.
    strcat(sql_concat, rowid_buffer);  // rowid to read from.
    strcat(sql_concat, sql3);  // Finish the sql statement

    // We can only send one query at a time to sqlite3.
    return_code = sqlite3_prepare_v2(p_db, sql_concat, -1, &p_stmt, 0);
    // On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    // is returned.
    if (return_code != SQLITE_OK)
        {
        // This is error handling code for the sqlite3_prepare_v2 function call.
        fprintf(stderr, "Failed to fetch data: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    while (sqlite3_step(p_stmt) == SQLITE_ROW)
        {
        //printf("DEBUG\n");  // DEBUG
        // Can use sqlite3_column_count(statement/p_stmt) in place of number_columns.
        for (int i = 0; i < number_columns; i++)  // Count 0 to number_columns-1
            {
            // To handle the return of NULL pointers as a string
            // data = (const char*)sqlite3_column_text( p_stmt, i );
            //        printf( "%s\n", data ? data : "[NULL]" );
            //        sprintf(buffer[strlen(data)], "%s\n", data ? data : "[NULL]" );

            // copy each entry to a buffer. Each entry is a col.
            strcpy(buffer, (const char*)sqlite3_column_text(p_stmt, i));  // i == array column.
            //printf("DEBUG %s\n", buffer);
            // concat buffer to our return array.
            strcat(db_tbl_rowid_data, buffer);

            if (i < number_columns -1) // Don't add ', ' after last column.
                {
                // Remove space after ','
                strcat(db_tbl_rowid_data, ",");  // add separator token between each col.
                }
            else  // Add line return, end of row.
                {
                ;  //strcat(db_tbl_rowdata[cnt_row], "\n");
                }
            }
        }

    //sqlite3_bind_*()  // After sqlit3_prepare_v2()
    //sqlite3_clear_bindings(p_stmt);

    // The sqlite3_finalize function destroys the prepared statement object.
    return_code = sqlite3_finalize( p_stmt );  // Commit to the database.
    if (return_code != SQLITE_OK)  // SQLITE_OK==0
        {
        // This is error handling code.
        fprintf(stderr, "Failed to finalize data: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    // The sqlite3_close function closes the database connection.
    return_code = sqlite3_close(p_db);   // SQLITE_OK==0
    if (return_code != SQLITE_OK)
        {
        // This is error handling code. NOTE! As p_db is closed the error code may not be available!
        fprintf(stderr, "Failed to close database: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    fprintf(stderr, "Successfully read rowdata from table in %s.\n", db_file_name);  // DEBUG

    return 1;

    }


// Search for a string in a field name and return array of found rows.
// As this only searches a single column we should not encounter duplicate rowid.
// ## Only for tables where all field types are TEXT (String) except rowid. ##
int db_search_table_rowdata_byfield(char *db_file_name, char *db_table_name, char **db_tbl_row_search, char *field_name, char *db_search_string, int number_columns, int *ret_array_length)
    {

    // Get column field names as array[][].
    // get number of columns.
    // return row length (number of array items).

    number_columns += 1;  // We are also including an extra column for the row ID number.
    sqlite3 *p_db;  // database handle (structure).
    sqlite3_stmt *p_stmt;  // structure represents a single SQL statement
    //const char *data = NULL;
    int return_code = 0;

    return_code = sqlite3_open_v2( db_file_name, &p_db, SQLITE_OPEN_READONLY, NULL );
    if ( return_code != SQLITE_OK)
        {
        // Note: these print returns can be commented out if only the Bool return 0; is required.
        fprintf(stderr, "Can't open database: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close( p_db );
        return -1;
        }

    char sql_concat[128] = {'\0'};
    strcpy(sql_concat, "");  // Set the buffer with strcpy() before strcat().

    // This can be replaced with sprintf()
    char *sql1 = "SELECT rowid, * FROM ";  // Note the space after FROM
    char *sql2 = " WHERE ";  // Note the space before and after WHERE
    char *sql3 = " = ";  // Note the space before and after =
    char *sql4 = ";";

    strcat(sql_concat, sql1);  // Add SQL query statement.
    strcat(sql_concat, db_table_name);  // Add table name to statement.
    strcat(sql_concat, sql2);  // Add last part of query statement.
    strcat(sql_concat, field_name);  // Add field name (column name).
    strcat(sql_concat, sql3);  // add " = "
    strcat(sql_concat, db_search_string);  // Add last part of query statement.
    strcat(sql_concat, sql4);  // Finish the statement with ";"

    // We can only send one query at a time to sqlite3.
    return_code = sqlite3_prepare_v2(p_db, sql_concat, -1, &p_stmt, 0);
    // On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    // is returned.
    if (return_code != SQLITE_OK)
        {
        // This is error handling code for the sqlite3_prepare_v2 function call.
        fprintf(stderr, "Failed to prepare data: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    // The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    // that there is another row ready. Our SQL statement returns only one row
    // of data at a time, therefore, we call this function only once at a time.
    // If we are writing or reading multiple lines of table then we will need to
    // use sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.

    int cnt_row = 0;
    //int cnt_col = 0;
    char buffer[128] = {'\0'};  // temp buffer [MAX 128 characters]

    while (sqlite3_step(p_stmt) == SQLITE_ROW)
        {
        //printf("DEBUGz\n");
        //printf("Value= %s\n", sqlite3_column_text(p_stmt, 1));

        // Can use sqlite3_column_count(statement/p_stmt) in place of number_columns.
        for (int i = 0; i < number_columns; i++)  // Count 0 to number_columns-1
            {
            // To handle the return of NULL pointers as a string
            // data = (const char*)sqlite3_column_text( p_stmt, i );
            //        printf( "%s\n", data ? data : "[NULL]" );
            //        sprintf(buffer[strlen(data)], "%s\n", data ? data : "[NULL]" );

            // copy each entry to a buffer. Each entry is a col.
            strcpy(buffer, (const char*)sqlite3_column_text(p_stmt, i));  // i == array column.
            //printf("Value: %s\n", buffer);  // DEBUG
            // concat buffer to our return array.
            strcat(db_tbl_row_search[cnt_row], buffer);
            if (i < number_columns -1) // Dont add ', ' after last column.
                {
                strcat(db_tbl_row_search[cnt_row], ",");  // add separator token between each col.
                }
            else  // Add line return, end of row.
                {
                ;//strcat(db_tbl_row_search[cnt_row], "\n");
                }
            //cnt_col++;  // Need 3D array.
            }
        //cnt_col = 0;
        cnt_row++;
        }

    *ret_array_length = cnt_row;  // return the length of array of found rows.

    //sqlite3_bind_*()  // After sqlit3_prepare_v2()
    //sqlite3_clear_bindings(p_stmt);

    // The sqlite3_finalize function destroys the prepared statement object.
    return_code = sqlite3_finalize( p_stmt );  // Commit to the database.
    if (return_code != SQLITE_OK)  // SQLITE_OK==0
        {
        // This is error handling code.
        fprintf(stderr, "Failed to finalize data: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    // The sqlite3_close function closes the database connection.
    return_code = sqlite3_close(p_db);   // SQLITE_OK==0
    if (return_code != SQLITE_OK)
        {
        // This is error handling code. NOTE! As p_db is closed the error code may not be available!
        fprintf(stderr, "Failed to close database: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    fprintf(stderr, "Successfully retrieved table search data from %s.\n", db_file_name);  // DEBUG
    //printf( "DEBUG\n%s\n", *db_tbl_row_search);  // DEBUG

    return 1;
    }


// Search for string in all field names of the table and return array of found rows.
// Duplicate row_id are filtered out.
// This needs to be stepped through each column and filter for duplicate rowid.
// ## Only for tables where all field types are TEXT (String) except rowid. ##
int db_search_table_rowdata_allfields(char *db_file_name, char *db_table_name, char **db_tbl_row_search, char **db_tbl_col_name, char *db_search_string, int number_columns, int number_rows, int *ret_array_length)
    {

    int cnt_col = 0;  // Counter to step though name of each column (field)
    int i = 0, j = 0; // Loop counters.
    // Be careful with this when looping fields!!! <- revise!
    // We are also including an extra column for the row ID number.
    int number_columns2 = number_columns +1;
    sqlite3 *p_db;  // database handle (structure).
    sqlite3_stmt *p_stmt;  // structure represents a single SQL statement
    //const char *data = NULL;
    int return_code = 0; // sql return codes
    int cnt_row = 0;  // Used to count through each row of found data to test for duplicate rows.
    char buffer[128] = {'\0'};  // temp buffer [MAX 128 characters]
    char token_buf[128] = {'\0'};  // temp buffer [MAX 128 characters]
    int ch = 0;  // character buffer.
    int x = 0;  // while loop to retrieve row_id up to first ','
    // Flag to skip writing duplicate row to return array. Also skips ret array increment.
    int row_id_exists = 0;
    char sql_concat[512] = {'\0'};  // Build sql query statement.

    // The element array(n)[0] should not be empty for the tests, but will still work in C.
    // Getting a warning that the db_search_string[i] is not a string, but works fine.
    //char *delimiter = ",";
    //for(i = 0; i < (number_rows); i++)
    //    {
    //    strcpy(db_tbl_row_search[i], delimiter);  // Check pointer?
    //    }


    return_code = sqlite3_open_v2( db_file_name, &p_db, SQLITE_OPEN_READONLY, NULL );
    if ( return_code != SQLITE_OK)
        {
        // Note: these print returns can be commented out if only the Bool return 0; is required.
        fprintf(stderr, "Can't open database: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close( p_db );
        return -1;
        }

    // I need to loop over the columns for full search...
    // WHERE "field/column_name" = will need to be replaced in each loop.
    // This search routine will only work for tables with type TEXT!

    // Be careful of the + 1 in number_columns2 due to extra row_id column.
    // rowid column is not counted when accessing the field names in the search.
    // The return values from the search DO include the rowid.
    // ====> START Loop each column name (field).
    for(cnt_col = 0; cnt_col < number_columns2 -1; cnt_col++)
        {
        // Clear the buffer from the last statement. strcat() will concat on to
        // the previous statement otherwise.
        strcpy(sql_concat, "");

        // This can be replaced with sprintf()
        // The \"name\" may not be required? !!!
        char *sql1 = "SELECT rowid, * FROM ";  // Note the space after FROM
        char *sql2 = " WHERE \"";  // Note the space before and after WHERE
        char *sql3 = "\" = ";  // Note the space before and after =
        char *sql4 = ";";

        strcat(sql_concat, sql1);  // Add SQL query statement.
        strcat(sql_concat, db_table_name);  // Add table name to statement.
        strcat(sql_concat, sql2);  // Add last part of query statement.
        strcat(sql_concat, db_tbl_col_name[cnt_col]);  // Add field name (column name) from db_tbl_col_name[][] array.
        strcat(sql_concat, sql3);  // add " = "
        strcat(sql_concat, db_search_string);  // Add last part of query statement.
        strcat(sql_concat, sql4);  // Finish the statement with ";"

        // We can only send one query at a time to sqlite3.
        return_code = sqlite3_prepare_v2(p_db, sql_concat, -1, &p_stmt, 0);
        // On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
        // is returned.
        if (return_code != SQLITE_OK)
            {
            // This is error handling code for the sqlite3_prepare_v2 function call.
            fprintf(stderr, "Failed to fetch data: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
            sqlite3_close(p_db);
            return -1;
            }

        // The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
        // that there is another row ready. Our SQL statement returns only one row
        // of data at a time, therefore, we call this function only once at a time.
        // If we are writing or reading multiple lines of table then we will need to
        // use sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.
        while (sqlite3_step(p_stmt) == SQLITE_ROW)
            {
            // Can use sqlite3_column_count(statement/p_stmt) in place of number_columns.
            for (i = 0; i < number_columns2; i++)  // Count 0 to number_columns-1
                {
                // To handle the return of NULL pointers as a string
                // data = (const char*)sqlite3_column_text( p_stmt, i );
                //        printf( "%s\n", data ? data : "[NULL]" );
                //        sprintf(buffer[strlen(data)], "%s\n", data ? data : "[NULL]" );

                // copy each entry to a buffer. Each entry is a col.
                strcpy(buffer, (const char*)sqlite3_column_text(p_stmt, i));  // i == array column.

                //====> START Test for repeat row_id ====>
                // This can be improved in efficiency !!!!
                // Do string compare test to see if the first column row_id
                // already exists in our return search array. Skip copying that
                // row if row_id already exists.
                // This does not order the array entries by row_id and are entered
                // in the order they are found during the search.
                if (i == 0)
                    {
                    for (j = 0; j < cnt_row; j++)  // Search previous entries for duplicate row_id
                        {

                        x = 0;  // reset the character counter.
                        ch = 0;  // Reset ch. May not be required anymore.
                        // extract the first col item row_id up to token ','
                        while (1)
                            {
                            ch = db_tbl_row_search[j][x];  // ###### First col and',' is non existent in array!!!!!
                            if (ch != ',')
                                {
                                token_buf[x] = ch;  // copy int characters to the token buffer.
                                x++;
                                }
                            else
                                {
                                // Found ',' so loop the rest of the column data
                                // without testing again until next column name if (i==0).
                                break;
                                }
                            }
                        token_buf[x +1] = '\0';  // add the "String" terminator char for safety.

                        // Compare if rowid is already found in our return search array.
                        // If rowid is already present, skip copying that row.
                        if( 0 == strcmp(buffer, token_buf))
                            {
                            // If True 0 then skip copying this row to db_tbl_row_search[].
                            row_id_exists = 1;
                            }
                        //else
                        //    {
                        //    strcpy(db_tbl_row_search[cnt_row], "");  // Clear the ',' ready for strcat()
                        //    }
                        }
                    }
                //====> END Test for reapeat row_id ====>

                if (row_id_exists == 0)  // if row not exist already, write search found to return buffer.
                    {
                    // Concat buffer to our return array. This will copy each column of the row
                    // separated by the ', ' comma-space character. aka CSV format.
                    // No return character '\n' is created as we use a separate array
                    // element for each row. To print (write to file) as CSV add
                    // the '\n' after each array element. See the loop that prints
                    // this in main().
                    strcat(db_tbl_row_search[cnt_row], buffer);
                    if (i < number_columns2 -1) // Dont add ', ' after last column.
                        {
                        strcat(db_tbl_row_search[cnt_row], ",");  // add separator token between each col.
                        }
                    }
                }

            if (row_id_exists == 0)
                {
                // We wrote this row to the return array, so increment +1
                cnt_row++;
                }
            // else Skip this row count for return array. row_id_exists == 1.
            row_id_exists = 0;  // Reset for next search column name.
            }

        //sqlite3_bind_*()  // After sqlit3_prepare_v2()
        //sqlite3_clear_bindings(p_stmt);

        // The sqlite3_finalize function destroys the prepared statement object.
        return_code = sqlite3_finalize( p_stmt );  // Commit to the database.
        if (return_code != SQLITE_OK)  // SQLITE_OK==0
            {
            // This is error handling code.
            fprintf(stderr, "Failed to finalize data: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
            sqlite3_close(p_db);
            return -1;
            }

        }  // END loop/walk each column (field) Name.

    *ret_array_length = cnt_row;  // return the length of array of found rows.

    // The sqlite3_close function closes the database connection.
    return_code = sqlite3_close(p_db);   // SQLITE_OK==0
    if (return_code != SQLITE_OK)
        {
        // This is error handling code. NOTE! As p_db is closed the error code may not be available!
        fprintf(stderr, "Failed to close database: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    fprintf(stderr, "Successfully retrieved table search data from %s.\n", db_file_name);  // DEBUG

    return 1;
    }

//==============================================================================
// START Multiple types examples.

// Insert binary row data into a named table.
// If the data already exists will create a new row.
// Table unique index ID INT is auto generated in this table.
int db_insert_table_rowdata_bin(char *db_file_name, char *db_tbl_entry, void *bin_data, int bin_data_len)
//int db_insert_table_rowdata(char *db_file_name, char *db_tbl_entry) // add insert at row_id
    {
    sqlite3 *p_db;  // database handle (structure).
    sqlite3_stmt *p_stmt;  // structure represents a single SQL statement
    //const char *data = NULL;
    int return_code = 0;

    return_code = sqlite3_open_v2( db_file_name, &p_db, SQLITE_OPEN_READWRITE, NULL );
    if ( return_code != SQLITE_OK)
        {
        // Note: these print returns can be commented out if only the Bool return 0; is required.
        fprintf(stderr, "Can't open database: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close( p_db );
        return -1;
        }

    // We can only send one query at a time to sqlite3.
    return_code = sqlite3_prepare_v2(p_db, db_tbl_entry, -1, &p_stmt, 0);
    // On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    // is returned.
    if (return_code != SQLITE_OK)
        {
        // This is error handling code for the sqlite3_prepare_v2 function call.
        fprintf(stderr, "Failed to prepare data: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    return_code = sqlite3_bind_blob(p_stmt, 1, bin_data, bin_data_len, SQLITE_STATIC);  // SQLITE_TRANSIENT
    if (return_code != SQLITE_OK)
        {
        // This is error handling code for the sqlite3_prepare_v2 function call.
        fprintf(stderr, "Failed to bind data: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    // The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    // that there is another row ready. Our SQL statement returns only one row
    // of data at a time, therefore, we call this function only once at a time.
    // If we are writing or reading multiple lines of table then we will need to
    // use sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.
    return_code = sqlite3_step( p_stmt );  // run once for one statement
    /*    while( sqlite3_step( p_stmt ) == SQLITE_ROW ) {;} */
    if (return_code != SQLITE_DONE)  // SQLITE_DONE==101, SQLITE_ROW==100
        {
        fprintf(stderr, "Step failed: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return 0;
        }

    //sqlite3_bind_*()  // After sqlit3_prepare_v2()
    //sqlite3_clear_bindings(p_stmt);

    // The sqlite3_finalize function destroys the prepared statement object.
    return_code = sqlite3_finalize( p_stmt );  // Commit to the database.
    if (return_code != SQLITE_OK)  // SQLITE_OK==0
        {
        // This is error handling code.
        fprintf(stderr, "Failed to finalize data: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    // The sqlite3_close function closes the database connection.
    return_code = sqlite3_close(p_db);   // SQLITE_OK==0
    if (return_code != SQLITE_OK)
        {
        // This is error handling code. NOTE! As p_db is closed the error code may not be available!
        fprintf(stderr, "Failed to close database: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    fprintf(stderr, "Successfully inserted rowdata into table in %s.\n", db_file_name);  // DEBUG

    return 1;
    }


// NOTE: SQLite3 does have it's own internal typless data structure Mem.
// typedef struct Mem Mem;
// It is an extremely complex data structure that includes many other data
// structures defined in the sqlite source. Also it is predominantly used
// with the sqlite3_value/_* set of API functions.
// typedef struct sqlite3_value sqlite3_value;
// It is more convenient to create our own tag struct, union or linked list
// for the following example.

// List all rows from mixed data types.
// Types:NULL,INTEGER,REAL,TEXT,BLOB
// sqlite3_column_type() returned values:
// SQLITE_NULL, SQLITE_INTEGER, SQLITE_FLOAT, SQLITE_TEXT, SQLITE_BLOB
// https://www.sqlite.org/c3ref/column_blob.html
// data = (const char*)sqlite3_column_text( p_stmt, 0 );
//        printf( "%s\n", data ? data : "[NULL]" );
// Note that number_elements should generally return number_rows.
int db_list_table_all_types(char *db_file_name, char *db_table_name, tagVARIANT **variant_structure, int number_columns, int number_rows, int *ret_number_fields, int *ret_number_elements)
    {
    number_columns = number_columns;  // not used at this time
    number_rows = number_rows;  // not used at this time
    sqlite3 *p_db;
    sqlite3_stmt *p_stmt;
    int return_code = 0;
    char sql_concat[512] = {'\0'};  // Build sql query statement.
    int i;
    int num_cols = 0;
    int max_cols = 0;
    int num_rows = 0;
    int bytes_blob = 0;

    return_code = sqlite3_open_v2( db_file_name, &p_db, SQLITE_OPEN_READONLY, NULL );
    if ( return_code != SQLITE_OK)
        {
        // Note: these print returns can be commented out if only the Bool return 0; is required.
        fprintf(stderr, "Can't open database: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close( p_db );
        return -1;
        }
    /*
    return_code = sqlite3_open(db_file_name, &p_db);
    if ( return_code != SQLITE_OK)
    {
    // Note: these print returns can be commented out if only the Bool return 0; is required.
    fprintf(stderr, "Can't open database: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
    sqlite3_close( p_db );
    return -1;
    }
    */

    strcpy(sql_concat, "");

    // This can be replaced with sprintf()
    // "select * from TableName"
    char *sql1 = "SELECT rowid, * FROM ";  // Note the space after FROM
    char *sql2 = ";";

    strcat(sql_concat, sql1);  // Add SQL query statement.
    strcat(sql_concat, db_table_name);  // Add table name to statement.
    strcat(sql_concat, sql2);  // Finish the statement with ";"

    return_code = sqlite3_prepare_v2(p_db, sql_concat, -1, &p_stmt, NULL);  // select all from tbl
    if (return_code != SQLITE_OK)
        {
        // This is error handling code for the sqlite3_prepare_v2 function call.
        fprintf(stderr, "Failed to prepare data: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    while (sqlite3_step(p_stmt) != SQLITE_DONE)  // or == SQLITE_OK
        {
        num_cols = sqlite3_column_count(p_stmt);

        for (i = 0; i < num_cols; i++)
            {
            switch (sqlite3_column_type(p_stmt, i))  // (p_stmt, cidx (col index)
                {
                // Note: sqlite3_column_type()	?	Default datatype of the result
                case SQLITE_NULL:
                    // Do stuff for NULL pointer, using variant_array[n].value.vval
                    // This will denote an unused array element. It is possible to
                    // use this data structure were we test for null as an empty element
                    // or as and empty type
                    variant_structure[num_rows][i].type = IS_NULL;
                    sqlite3_column_text(p_stmt, i);  // NULL to read column
                    variant_structure[num_rows][i].value.vval = NULL;
                    break;
                case SQLITE_INTEGER:
                    variant_structure[num_rows][i].type = IS_INTEGER;
                    variant_structure[num_rows][i].value.ival = sqlite3_column_int(p_stmt, i);
                    break;
                case SQLITE_FLOAT:  // REAL
                    variant_structure[num_rows][i].type = IS_FLOAT;
                    variant_structure[num_rows][i].value.rval = sqlite3_column_double(p_stmt, i);
                    break;
                case SQLITE_TEXT:

                    variant_structure[num_rows][i].type = IS_TEXT;
                    strcpy(variant_structure[num_rows][i].value.tval, (const char*)sqlite3_column_text(p_stmt, i));
                    break;
                case SQLITE_BLOB:
                    variant_structure[num_rows][i].type = IS_BLOB;
                    // MAX 2048 bytes (30719)
                    bytes_blob = sqlite3_column_bytes(p_stmt, i);
                    variant_structure[num_rows][i].value.bval.blen = bytes_blob;
                    //printf("Bytes_blob=%d\n", bytes_blob);  // DEBUG
                    // We must use memcpy() or similar function for this.
                    memcpy(variant_structure[num_rows][i].value.bval.bdata, sqlite3_column_blob(p_stmt, i), bytes_blob);
                    //0000  ff d8 ff e2 02 1c 49 43 43 5f 50 52 4f 46 49 4c  ......ICC_PROFIL
                    break;
                default:
                    // Report an error, this shouldn't happen!
                    //printf("##default= %d##\n", variant_array[j][i].type);
                    variant_structure[num_rows][i].type = IS_NULL;  // ???
                    break;
                }
            }

        if(num_cols > max_cols)
            {
            //Retrieve the longest column length. In most cases num_columns
            // should always be the same length for each row.
            max_cols = num_cols;
            }
        num_rows++;
        }

    // Only counts the longest column returned.
    *ret_number_fields = max_cols;
    *ret_number_elements = num_rows;

    //sqlite3_bind_*()  // After sqlit3_prepare_v2()
    //sqlite3_clear_bindings(p_stmt);

    sqlite3_finalize(p_stmt);
    if (return_code != SQLITE_OK)  // SQLITE_OK==0
        {
        // This is error handling code.
        fprintf(stderr, "Failed to finalize data: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    // The sqlite3_close function closes the database connection.
    return_code = sqlite3_close(p_db);   // SQLITE_OK==0
    if (return_code != SQLITE_OK)
        {
        // This is error handling code. NOTE! As p_db is closed the error code may not be available!
        fprintf(stderr, "Failed to close database: %s | %d\n", sqlite3_errmsg(p_db), return_code);  // DEBUG
        sqlite3_close(p_db);
        return -1;
        }

    return 1;
    }


// ====> Convenience helper functions

int Con_Sleep(int seconds)
    {
    // #include <stdlib.h>
    // Cross platform sleep in seconds
#ifdef _WIN32 // Windows 32-bit and 64-bit
    seconds = seconds * 1000;
    _sleep( seconds );  // Note _sleep is deprecated
#endif
#ifdef __unix__ // _linux__ (__linux__)
    sleep(seconds);
#endif
    return 0;
    }

// Console Clear
int Con_Clear(void)
    {
    // The system() call allows the programmer to run OS CLI batch commands.
    // It is discouraged as there are more appropriate C functions for most tasks.
    // I am only using it in this instance to avoid invoking additional OS API
    // headers and code.
#ifdef _WIN32 // Windows 32-bit and 64-bit
    system("cls");
#endif
#ifdef __unix__ // _linux__ (__linux__)
    system("clear");
#endif
    return 0;
    }

// Safe Pause
void S_Pause(void)
    {
    // This function is referred to as a wrapper for S_getchar()
    printf("Press Enter to continue...");
    S_getchar();// Uses S_getchar() for safety.
    }

// Must have characters or \n in the buffer or it blocks.
// Used after scanf() which can leave '\n' in the input buffer.
void S_Clear_Input_Buffer(void)
    {
    S_getchar();
    }

// Safe getcar() removes all artefacts from the stdin buffer.
int S_getchar(void)
    {
    // This function is referred to as a wrapper for getchar()
    int i = 0;
    int ret;
    int ch;

    // The following enumerates all characters in the buffer.
    while(((ch = getchar()) !='\n') && (ch != EOF ))
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


// End of header include guard.
#endif