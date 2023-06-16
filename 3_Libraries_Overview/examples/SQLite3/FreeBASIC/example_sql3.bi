''------------------------------------------------------------------------------
'' Name:        example_sql3.bi (based upon basics_1.c)
'' Purpose:     SQLite3 basic examples.
''              Convenience wrapper functions for SQLite version 3.
''
'' Platform:    Win64, Ubuntu64
'' Depends:     v3.34.1 plus
''
'' Author:      Axle
'' Created:     29/05/2023 (19/04/2023)
'' Updated:     15/06/2023
'' Copyright:   (c) Axle 2023
'' Licence:     MIT-0 No Attribution
''------------------------------------------------------------------------------
'' Notes:
''
'' !!! All routines and examples are based upon tables using only TEXT (String)
'' type field entries. The only exception is the system generated rowid.
'' See NOTE: ... below !!!
''
'' To use multiple type field entries a "linked list" data structure is required
'' to be able to accept mixed data types in C.
'' SEE: sqlite3_column_type(statement, i) examples.
''
''
'' NOT using sqlite3_exec() with callback. All returns are handled in the
'' calling routines.
''
'' There is no set or defined rule in the standards for what error value of
'' error returns must be in an application function. It is implementation
'' defined and can differ from function to function. The only (most common)
'' rule is returning a value of 0==Success from main() to the OS. This can also
'' be written as return EXIT_SUCCESS; and return EXIT_FAILURE;
''
'' Function returns can be any value, so I have used a mix of error return
'' schemes depending upon the return type of the functions routines. in most
'' cases 0, -1, 1, 2 will either define success, fail or a specific error.
'' In a commercial application we would more likely return the error code of
'' SQLite3 and handle the sql error code directly.
'' https:''www.sqlite.org/rescode.html
'' I have over simplified the returns to a basic Success-TRUE, Fail-FALSE scheme
'' for simplicity of the examples.
''
'' The routines and functions use an excessive amount of error reporting. Normally
'' we only handle actual errors and do so silently in the background of our
'' application. I have created error info on both successful returns as well as
'' on errors only as a visual guide. If using the sqlit3_open* and sqlite2_close*
'' directly from your main application you can also use the sqlite3_errmsg()
'' directly. Another option is to return the integer value of sqlite3_errmsg()
'' from your function and handle the error from the calling statement as an sql
'' error rather than the 0==False(fail), 1==True(success) etc. that I have made
'' up for the examples.
''
'' Error returns can be handled in a number of ways. I have created my own
'' error returns of 0, 1, -1 etc as well as displaying the SQLite3 error codes
'' in the functions. This is excessive and only there to exemplify the different
'' error returns. In practice we would return the error code number to our
'' calling statement. sqlite3_errmsg(p_db) or rc
'' return strcpy((char)error_return, sqlite3_errmsg(p_db)); or
'' return (int)rc;
'' Typically sqlite3_initialize(), sqlite3_open_v2(), sqlite3_close(), sqlite3_shutdown()
'' would be called from main() where we would be able to retrieve the last
'' sqlite3_errmsg(p_db) directly. We wouldn't open and close the database file
'' for every query as it is more appropriate to keep it open for as long as
'' repetitive transactions are taking place.
''
'' In light of the above take note of the 3 different ways of "Opening" an
'' sqlite database file.
'' SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_READONLY
'' Each has pros and cons with regard to safety when opening a database file. For
'' example we may not wish to create a new database file if the wrong file name
'' is entered and prefer an error return "p_db file not found" instead.
''
''
'' SQLite3 can only accept 1 command statement at a time. To run multiple
'' statements they must use the sqlite3_prepare_v2, sqlite3_step, sqlite3_finalize
'' in a loop for each statement. This can be done by sending a formatted array of
'' statements and sending each separately in a loop or as a loop in the function.
'' This would also be true when sending query statements that have multiple returns
'' where an appropriate sized array will need to be supplied.
''
''
'' data is stored internally as bytes by default. The length of the bytes and
'' the affinity (data type) is stored in the header entry of the data.
'' Although stored as bytes, this data could be of any data type.
''
'' Internally SQLite 3 stores all data as BYTEs and is recovered as a number of
'' BYTEs. The type affinity is used to convert the bytes back to the storage type
'' associated with the cell in that column.
''
'' The different data types are to allow code conformity with other SQL database
'' engines which have static typing. Keeping this in mind it is acceptable to
'' use only TEXT for most data storage. Where a numeric value is required
'' internally by sqlite such as column or row ID number then I would suggest
'' using INTEGER PRIMARY KEY if needed. You will need to INSERT and and SELECT
'' providing the correct data type containers in this case.
'' I am using TEXT only tables to simplify the examples. You cane see a
'' prototype for the more complex mixed data types in the final example.

'' In reality most applications will take user input as text, data read from a
'' document as text and even transport most data as text. (All data is text in
'' python unless specifically stated for example.) Remember that even user
'' numeric keyboard input is text until converted to its integer representation
'' by the programming language. For example we may take an input from a user in
'' C language to a variable int my_number = getchar(); The text arrives from
'' the keyboard as a hexadecimal representation of the character '5' as 0x35
'' (ASCII decimal 53), int my_text_number = getchar() = 53
'' To use it as an actual integer we need to convert the character to an
'' integer (53 - 48) int 0 = char dec48 - 48 ( SEE and ASCII chart),
'' so to get the integer value (5) from the text character'5' we need to convert
'' int my_interger_number = (my_text_number(53 '5') - 48); (== 5)
'' Note my use of inverted commas character '5' (hex 0x35) vs integer 5 (hex 0x05)
'' to represent character '5' and integer 5.
''
'' The above is primarily suggested for a small database app without complex data
'' structures as we would know in advance the data type required for each column.
'' For a more accurate (and complex) example see the last function example in this
'' wrapper library db_list_table_all_types().
''
'' White space between column values or after the ',' delimiter will need to be
'' managed by the calling application. Alternatively you can modify any routine
'' that reads from the database and change strcat(buffer, ", ") to ",".
'' ## Removed/Changed all returns from ", " to ","
''
'' I am treating this database example in a similar way to a CSV data file
'' storage which is less efficient, but I wanted to keep the examples simple
'' even if a little slower and less optimised than they would be in commercial
'' practice. Ultimately it is up to the programmer to choose the most
'' appropriate method of SQLite3 usage for there requirements.
''
'' Handling NULL values. The following is a simple example of handling NULL
'' pointer values from an empty column entry. In this example I have replaced
'' non-existing value with text "[NULL]". This is just an example and we can
'' deal with this return in any way that is appropriate for the context of our
'' application. I have not handled NULL returns in the examples.
'' data = (const char*)sqlite3_column_text( p_stmt, 0 );
''        printf( "%s\n", data ? data : "[NULL]" );
''
'' Note! Some keywords are reserved for SQLIte, for example "rowid".
''
'' Using concatenate strcat() can lead to SQL injection! SEE Parameterised
'' statements and int sqlite3_bind_text(sqlite3_stmt*,int,const char*,int,void(*)(void*));
'' https:''www.sqlite.org/c3ref/bind_blob.html
''------------------------------------------------------------------------------
'' Credits:
'' https:''resources.oreilly.com
'' https:''zetcode.com/p_db/sqlitec/
'' https:''gist.github.com/jsok/2936764
'' ++
''------------------------------------------------------------------------------
''
''------------------------------------------------------------------------------
'' TODO:
'' Convenience wrapper functions for SQLite version 3 [Done]
'' Remove excess comments.[ Done]
'' Move sqlite open/close to main(). [?]
'' Return sqlite errors to calling functions? [?]
'' alt change sqlite errors to better value set?
''
'' Two generalised functions to take and return table data? [Not done]
'' I may include a universal function at the end of this document that will
'' take most sqlite3 commands and return the appropriate arrays of data.
'' Send command/s
'' Retrieve command/s
''
'' Mark extra error returns as DEBUG. [Done]
'' Revise db_delete_table_rowdata()
'' Revise db_insert_table_rowdata_rowid() for error handling while loop.
''
'' Check array off by 1s.
''------------------------------------------------------------------------------

#ifndef __EXAMPLE_SQL3_bi__
#define __EXAMPLE_SQL3_bi__

'#include once <stdio.h>
'#include once <stdlib.h>
'#include once<string.h>
''#include once <errno.h>
''#include once <math.h>
''#include once <conio.h>
'' Windows APIs are a limited subset of UNIX. aka UNIX APIs do not always
'' have an mscrt equivalent capability. Some features of UNIX are available
'' using UNIX emulation layers such as QT, MSYS2 amd WSL. These UNIX subsystems
'' require a slightly different standard library tool chain (headers.h) as well
'' as the subsystem shared objects to be available, but don't provide a direct
'' conversion from UNIX. Note that the UNIX subsystem libraries are quite large
'' and often introduce unnecessary overheads on Windows, so it is better to
'' write for the native windows APIs or MinGW which connects directly to
'' the Windows CRT unless you are attempting to port a specific UNIX app to
'' use on Windows and have no other options.
'' This also applies to other languages such as Python.
'#ifdef __unix__ '' _linux__ (__linux__)
'' On Unix this is a standard library header.
'#include <unistd.h>
'' If used on Windows it has a different meaning and definitions relating to
'' the UNIX subsystems for windows and is non standard..
'#endif

/'
#pragma once

#include once "crt/long.bi"
#include once "crt/stdio.bi"
#include once "crt/stdlib.bi"
#include once "crt/string.bi"

#ifdef __FB_UNIX__
	#include Once "unistd.bi"
#endif

#include once "crt/stdarg.bi"
'/
#include once "crt/string.bi"  '' For memcpy() used in BLOBs

    


#include once "sqlite3.bi"

#include once "file.bi"

'extern "C"
'end extern



#Ifndef NULL
'#Define NULL 0
    'Dim NULL = *Cptr(Any Ptr, 0)  '' This is the correct Ctypes definition of NULL*
    Const NULL As Any Ptr = 0  '' Correct FB definition of NULL. I think it is the same as above.
    'Const NULL As Any Ptr = 0
    'Dim NULL As Integer = 0  '' This is also often used in FB aka Zero==0
#Endif


' Console Pause (GetKey version)
Function Con_Pause() As Integer
    Dim As Long dummy
    Print !"\nPress any key to continue..."
    dummy = Getkey
    Return 0
End Function

''=========================
/'
Dim As Integer IS_NULL = 5
Dim As Integer IS_INTEGER = 1
Dim As Integer IS_FLOAT = 2
Dim As Integer IS_TEXT = 3
Dim As Integer IS_BLOB = 4
'/

'' Mixed types(affinity) data structures
'' this structure keeps track of the number of elements of binary data stored
'' within the tagVARIANT structure under tagVARIANT.bval.*
'' tagVARIANT.bval.blen holds the length of bytes in tagVARIANT.bval.bdata[]
'' this could be written as tagVARIANT.bval.bdata[tagVARIANT.bval.blen] to
'' denote the total number of data bytes.
''
'' tagVARIANT.tval must be assigned as a string with the trailing '\0' zero
'' terminator to retrieve the length of string, else the length of the array
'' must also be returned.

Type struct_bval
	blen As Long  '' Length of bin data
	bdata(0 To 30719) As Ubyte  '' bin data MAX length 30,720 bytes, 30KiB
    '' bdata can also be redim(ed) if larger size is required.
    '' It is possible to use Z/String in place of UByte array but less appropriate.
End Type

'' Data structures, unions ( data type VARIANT )
'' Types:NULL,INTEGER,REAL,TEXT,BLOB (SQLite 3)
'' Types: NULL, int, double, char(string), unsigned char or void* (C)
'' sqlite3_column_type() returned value type:
'' affinities:
'' SQLITE_NULL, SQLITE_INTEGER, SQLITE_FLOAT, SQLITE_TEXT, SQLITE_BLOB
'' Note:NUMERIC can hold any data type as an integer but is an affinity
'' rather than a type. In most instances NUMERIC will convert to int or float.
'' I have not used NUMERIC.
'' "tagged union"

'' My made up affinities used in this C structure. Used to identify the type
'' affinity of retrieved data.
Type tagVARIANT_sql_type As Long
Enum
	IS_NULL = 5
	IS_INTEGER = 1
	IS_FLOAT = 2
	IS_TEXT = 3
	IS_BLOB = 4
End Enum


Union tagVARIANT_value
	vval As Any Ptr     '' NULL/void (pointer to) this will denote an empty element.
	ival As Long        '' INTEGER
	rval As Double      '' REAL
	tval As zstring * 30720  '' TEXT (Max string (row) length 30,720, 30KiB)
    '' tval can also be redim(ed) if larger size is required.
    '' zstring * 30720 may be able to be replaced with a FB String type.
    '' FB String will hold ASCII BYTEs 0 to 255 by converting/concatenating
    '' the binary bytes to Asc() Example:
    '' for n = 0 to bin_length; String_type += Chr(UByte_type(n)); Next
	bval As struct_bval  '' BLOB (binary) structure struct_bval (INT, UCHAR)
End Union

'' The combined structure and union.
Type tagVARIANT
	sql_type As tagVARIANT_sql_type
	value As tagVARIANT_value
End Type

'' Note all fields/column name are TEXT typed except for the system rowid.
'' If you wish to use INTEGER or other types you will need rewrite the functions
'' accordingly. See final examples using BLOBS and mixed data types.
'' Values cannot be empty(NULL) for any column.
/'
'' Get SQLite3 Version.
Declare Function sqlite3_get_version0() As Long
'' Get SQLite3 Version.
Declare Sub sqlite3_get_version1(Byval ret_version As zstring Ptr)
'' Get SQLite3 Version.
Declare Function sqlite3_get_version2(Byval ret_version As zstring Ptr) As Long

'' Check if "FileName" exists.
Declare Function file_exists(Byval db_file_name As zstring Ptr) As Long
'' Check IF "Sqlite 3" database TRUE.
Declare Function db_file_exists(Byval db_file_name As zstring Ptr) As Long

'' Create empty SQLite 3 database.
Declare Function db_file_create(Byval db_file_name As zstring Ptr) As Long
'' Delete "FileName".
Declare Function db_file_delete(Byval db_file_name As zstring Ptr) As Long

'' Check "TableName exists".
Declare Function db_table_exists(Byval db_file_name As zstring Ptr, Byval db_table_name As zstring Ptr) As Long

'' The only difference between the following 2 functions is the first enumerates
'' all tables in the named database file and returns a total count without
'' returning the actual table names. We need this count to create a dynamic
'' array of the correct size to hold the actual table names for the second function.
'' Get the total number of tables in a database.
Declare Function db_get_number_tables(Byval db_file_name As zstring Ptr, Byval number_tables_ret As Long Ptr) As Long
'' List all table names to dynamic array the size of return from previous function.
Declare Function db_get_tablenames(Byval db_file_name As zstring Ptr, Byval db_tablenames As zstring Ptr Ptr) As Long

'' Create "TableName" (If NOT Exists). Will create p_db "FileName" if not exists.
Declare Function db_table_create(Byval db_file_name As zstring Ptr, Byval db_table_name As zstring Ptr) As Long  '' Revise?
'' Delete "TableName" if exists.
Declare Function db_table_delete(Byval db_file_name As zstring Ptr, Byval db_table_name As zstring Ptr) As Long

'' Get number of rows in "TableName".
Declare Function db_get_table_number_rows(Byval db_file_name As zstring Ptr, Byval db_table_name As zstring Ptr, Byval number_rows_ret As Long Ptr) As Long
'' Get number of columns in "TableName".
Declare Function db_get_table_number_cols(Byval db_file_name As zstring Ptr, Byval db_table_name As zstring Ptr, Byval number_cols_ret As Long Ptr) As Long
'' Get array of column names. List all column names to array as array[col number][column name]
Declare Function db_get_table_colnames(Byval db_file_name As zstring Ptr, Byval db_table_name As zstring Ptr, Byval db_tbl_col_name As zstring Ptr Ptr) As Long

'' Add new row after last rowid.
Declare Function db_insert_table_rowdata(Byval db_file_name As zstring Ptr, Byval db_tbl_entry As zstring Ptr) As Long
'' Delete row/s using search terms (dangerous). better to confirm rowid!
Declare Function db_delete_table_rowdata(Byval db_file_name As zstring Ptr, Byval db_row_entry As zstring Ptr) As Long
'' List all rows to array as array[rowid][row data as csv]
Declare Function db_list_table_rows_data(Byval db_file_name As zstring Ptr, Byval db_table_name As zstring Ptr, Byval db_tbl_data As zstring Ptr Ptr, Byval number_columns As Long) As Long

'' delete row by "rowid". Following rows will all shift -1 rowid.
Declare Function db_delete_table_rowdata_rowid(Byval db_file_name As zstring Ptr, Byval db_table_name As zstring Ptr, Byval sql_rowid As Long) As Long
'' update/replace by rowid. Relpace an existing ebtry in place.
Declare Function db_replace_table_rowdata_rowid(Byval db_file_name As zstring Ptr, Byval db_table_name As zstring Ptr, Byval sql_rowid As Long, Byval db_field_names As zstring Ptr, Byval db_field_values As zstring Ptr) As Long
'' insert to rowid (all previous and following data moved down one rowid).
Declare Function db_insert_table_rowdata_rowid(Byval db_file_name As zstring Ptr, Byval db_table_name As zstring Ptr, Byval sql_rowid As Long, Byval db_field_names As zstring Ptr, Byval db_field_values As zstring Ptr, Byval number_columns As Long, Byval number_rows As Long) As Long
'' read row from rowid
Declare Function db_read_table_rowdata_rowid(Byval db_file_name As zstring Ptr, Byval db_table_name As zstring Ptr, Byval sql_rowid As Long, Byval db_tbl_rowid_data As zstring Ptr, Byval number_columns As Long) As Long

'' returns the number of rows "Found" in returned array...
'' Search TableName by field/column Name using search word. Returns array of rows found (prefixed with rowid).
Declare Function db_search_table_rowdata_byfield(Byval db_file_name As zstring Ptr, Byval db_table_name As zstring Ptr, Byval db_tbl_row_search As zstring Ptr Ptr, Byval field_name As zstring Ptr, Byval db_search_string As zstring Ptr, Byval number_columns As Long, Byval ret_array_length As Long Ptr) As Long
'' Search TableName by ALL field/column Name using search word. Return array of rows found (prefixed with rowid). aka full table search
Declare Function db_search_table_rowdata_allfields(Byval db_file_name As zstring Ptr, Byval db_table_name As zstring Ptr, Byval db_tbl_row_search As zstring Ptr Ptr, Byval db_tbl_col_name As zstring Ptr Ptr, Byval db_search_string As zstring Ptr, Byval number_columns As Long, Byval ret_array_length As Long Ptr) As Long

'' This is the final and more advanced SQLite3 query using multiple data types VARIANT.
'' I have used a binary BLOB entry for the example.
Declare Function db_insert_table_rowdata_bin(Byval db_file_name As zstring Ptr, Byval db_tbl_entry As zstring Ptr, Byval bin_data As Any Ptr, Byval bin_data_len As Long) As Long
'' int more universal query function for mixed data types.
Declare Function db_list_table_all_types(Byval db_file_name As zstring Ptr, Byval db_table_name As zstring Ptr, Byval variant_structure As tagVARIANT Ptr Ptr, Byval number_columns As Long, Byval number_rows As Long, Byval ret_number_fields As Long Ptr, Byval ret_number_elements As Long Ptr) As Long

'' ====> Convenience helper functions (Not really required)
'/

''==============================================================================

'' Get SQLite version. (short function)
'' Returns integer version. v3.34.1 = 034001 = 3|034|001
'' Mmmmppp, with M being the major version, m the minor, and p the point release.
Function sqlite3_get_version0() As Integer
    Return sqlite3_libversion_number()
End Function

'' Get SQLite version. (short function)
'' Returns string to version buffer.
'' We can call a number of the SQLite C APIs without needing to open a database.
'' The SQLite3.dll library is connected to our application at startup.
Sub sqlite3_get_version1(Byref ret_version As String)
    '' Note the ret_version is converted to an FB String.
    '' Copy the "value" of converted string return into the Value of ret_version "ByRef".
	ret_version = *Cast(zString Ptr, sqlite3_libversion())  '' This should be correct.
    '' or
    'ret_version = *cptr(zstring ptr, sqlite3_libversion())  '' This should also be correct.
End Sub

'' https:''zetcode.com/p_db/sqlitec/
'' Get SQLite version - query. (long function)
'' Returns string to version buffer, as well as int sqlite error codes.
Function sqlite3_get_version2(Byref ret_version As String) As Integer
	Dim p_db As sqlite3 Ptr  '' database handle (structure).
	Dim p_stmt As sqlite3_stmt Ptr   '' structure represents a single SQL statement
    Dim fpErr As Integer
	Dim return_code As Integer = 0 = 0
	Dim db_filename_ram As String = ":memory:"
    '' return_code is the return error codes.
    '' Note: :memory: can be used instead of a file for temporary database
    '' operations.
    '' defaults to: [SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE]
	return_code = sqlite3_open(db_filename_ram, @p_db)  '' Open Memory (RAM) data base.
    If return_code <> SQLITE_OK Then  '' integer 0
        '' This is error handling code for the sqlite3_prepare_v2 function call.
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        Print #fpErr, "Cannot open database: "; *Cast(zString Ptr,sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
		Return -1
	End If

    '' Before an SQL statement is executed, it must be first compiled into a
    '' byte-code with one of the sqlite3_prepare* functions.
    '' The sqlite3_prepare_v2 function takes five parameters. The first parameter
    '' is the database handle obtained from the sqlite3_open function. The second
    '' parameter is the SQL statement to be compiled. The third parameter is the
    '' maximum length of the SQL statement measured in bytes.
    '' -1 causes the SQL string to be read up to the first zero terminator which
    '' is the end of the string here. (or supply the exact no of bytes.)
    '' The fourth parameter is the statement handle. It will point to the
    '' pre-compiled statement if the sqlite3_prepare_v2 runs successfully. The last
    '' parameter is a pointer to the unused portion of the SQL statement. Only
    '' the first statement of the SQL string is compiled, so the parameter points
    '' to what is left un-compiled. We pass 0 since the parameter is not important
    '' for us SEE: sqlite3_clear_bindings(p_stmt);.
    return_code = sqlite3_prepare_v2(p_db, "SELECT SQLITE_VERSION()", -1, @p_stmt, 0)
    '' On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    '' is returned.
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        Print #fpErr, !"Failed to prepare data: "; *Cast(zString Ptr,sqlite3_errmsg(p_db)); !" | "; return_code; !"\n"  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return 0
	End If

    '' The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    '' that there is another row ready. Our SQL statement returns only one row
    '' of data, therefore, we call this function only once.
    '' If we expected multiple lines of data (rows, columns) we would need to
    '' recover each table cell as a step within a loop until end of data
    '' (<>SQLITE_ROW).
    return_code = sqlite3_step(p_stmt)
	If return_code <> SQLITE_ROW Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        Print #fpErr, "Step error: "; *Cast(zString Ptr,sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
		Return 0
	End If

    '' I am a little uncertain as to if I am changing the global value of
    '' ret_version or setting a pointer to the SQLite c char* string.
        '' const unsigned char *sqlite3_column_text(sqlite3_stmt*, int iCol);
        '' iCol refers to the current column in the return data. In this case
        '' there is only one column of return value, so we know the zero column
        '' contains the version number.
        '' Cast( Type, expression )  '' The return from the dll uses C data types.
        '' But is an unsigned char* so we have to convert it to a char*
        '' sqlite3_errmsg(byval as sqlite3 ptr) as const zstring ptr
        '' Note: I have found this conversion somewhat ambiguous.


    ret_version = *Cptr(zstring Ptr, sqlite3_column_text(p_stmt, 0))  '' This should be correct.
    '@ret_version = cptr(zstring ptr, sqlite3_column_text(p_stmt, 0))
    'ret_version = *Cast(zString ptr, sqlite3_libversion())  '' This should be correct.
    '@ret_version = Cast(zString ptr, sqlite3_libversion())

    ''sqlite3_bind_*()  '' After sqlit3_prepare_v2()
    ''sqlite3_clear_bindings(p_stmt);  '' clears leftover statements from sqlite3_prepare.

    '' The sqlite3_finalize function destroys the prepared statement object.
	return_code = sqlite3_finalize(p_stmt)
    If return_code <> SQLITE_OK Then
        '' This is error handling code.
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        Print #fpErr, "Failed to finalize data: "; *Cast(zString Ptr,sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
		Return -1
	End If

    '' The sqlite3_close function closes the database connection.
	sqlite3_close(p_db)
	If return_code <> SQLITE_OK Then
        '' This is error handling code.
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        Print #fpErr, "Failed to close database: "; *Cast(zString Ptr,sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
		Return -1
	End If

    Return 1
End Function


''==============================================================================

'' Check if a file name exists by opening the file for read operations. If the
'' file does exist it will not be created and return an error.
'' 0 == False | 1 == True
'' This is a standard file operation and not part of SQLite.
Function file_exists(Byval file_name As String) As Integer

    Dim fp As Integer = Freefile()
    Dim fpErr As Integer
    Dim ret_code As Integer 

    ret_code = Open(file_name, For Input, As fp)  '' For Input == read only.
    If Ret_code <> 0 Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        Print #fpErr, "Cannot open file "; file_name  '' DEBUG
        Close #fpErr  '' DEBUG
        Return 0
    End If

    Close #fp
    Return 1
End Function


'' Check if file exist and is an sqlite3 database.
'' Looks for "SQLite format 3" in the 100 byte header of the file.
'' SEE: db_file_create()
Function db_file_exists(Byval db_file_name As String) As Integer

    Dim fpErr As Integer
    Dim fp As Integer = Freefile()
    Dim ret_code As Integer

    Dim As Integer cnt_chr = 0
    Dim As Byte char_buffer

    Dim As String header  '' sqlite3 header = 100 characters.
    Dim As String t_buffer
    Dim As String search = "SQLite format 3"  '' Header is 16 bytes long.

    ret_code = Open( db_file_name, For Binary Access Read, As fp)
    If Ret_code <> 0 Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        Print #fpErr, "Cannot open file "; db_file_name  '' DEBUG
        Close #fpErr  '' DEBUG
        Return 0
    Else
        '' The SQLite 3 header is exacly 100 (0 - 99) bytes long immediately followed
        '' by a newline char '\n' == Dec 13 == LF at 101 characters.

        '' I should be using Get() for binary file read. Line Input is over
        '' reading the line by 4 bytes showing a length of 106 when CR, 13, '\n'
        '' is at byte 101 byte[100]. I had expected the text based input to still
        '' pick up the end of line character at array position [100] but this
        '' is not the case in this instance.
        '' I will re-write this routine using Get().
        
        '' ALT Line Input #fp, header  '' Undefined behaviour.
        ''Print len(header)  '' DEBUG Undefined behaviour.
        '' DEBUG Print
        'dim as integer i = 0
        'for i = 0 to 105 step 1
        '    Print i; " "; header[i]  ''  Chr(), Asc()
        'next i
        While Not Eof (fp)
            Get #fp, ,char_buffer
            If (char_buffer = 13) Or (char_buffer = 10) Or (cnt_chr > 105) Then  '' Test if we have encountered a new line and,
                Exit While
            End If

            cnt_chr += 1

            If cnt_chr < 16 Then
                t_buffer = t_buffer + Str(Chr(char_buffer))  '' Concatenate first 16 bytes to string.
            End If
        Wend
    End If


    '' The header file 100 bytes is followed by a '\n' at 101 bytes
    If cnt_chr = 101 Then
    '' ALT If int(header[100]) <> 13 Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        Print #fpErr, "Header incorrect length."  '' DEBUG
        Close #fpErr  '' DEBUG
        Close #fp  '' close the file.
        Return 0
    Else
        '' The first 16 bytes is a string denoting "SQLite format 3". So
        '' we will keep the first 16 bytes and convert it to a string.
        '' ALT->
        /'
        Dim As Integer i = 0
        For i = 0 To 15 Step 1
            'print i  '' DEBUG Note the NULL terminator 0x00
            t_buffer = t_buffer + Str(Chr(header[i]))  '' Concatenate first 16 bytes to string.
        Next i
        'print t_buffer;"|"  ''DEBUG Note the NULL terminator 0x00 becomes a blank space instead of a string terminator.
        '/  '' <-END ALT
        '' Test if the string == "SQLite format 3". If true it should be a
        ' genuine SQLite 3 database file.

        If Instr(t_buffer, search) = 0 Then
            fpErr = Freefile()  '' DEBUG
            Open Err For Input As #fpErr  '' DEBUG
            Print #fpErr, "SQLite format 3 Header not found!"  '' DEBUG
            Close #fpErr  '' DEBUG
            Close #fp  '' close the file.
            Return 0
        End If

    End If

    '' START Alternative method ->
    /'
    ret_code = Open( db_file_name, For Binary Access Read, As fp)
    If Ret_code <> 0 Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        Print #fpErr, "Cannot open file "; db_file_name  '' DEBUG
        Close #fpErr  '' DEBUG
        Return 0
    Else
        ALT Line Input #fp, header  '' Undefined behavior.
        ''Print len(header)  '' DEBUG Undefined behavior.
        '' DEBUG Print
        'dim as integer i = 0
        'for i = 0 to 105 step 1
        '    Print i; " "; header[i]  ''  Chr(), Asc()
        'next i
    End If

    If Int(header[100]) <> 13 Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        Print #fpErr, "Header incorrect length."  '' DEBUG
        Close #fpErr  '' DEBUG
        Close #fp  '' close the file.
        Return 0
    Else
        Dim As Integer i = 0
        For i = 0 To 15 Step 1
            'print i  '' DEBUG Note the NULL terminator 0x00
            t_buffer = t_buffer + Str(Chr(header[i]))  '' Concatenate first 16 bytes to string.
        Next i
        'print t_buffer;"|"  ''DEBUG Note the NULL terminator 0x00 becomes a blank space instead of a string terminator.
        If Instr(t_buffer, search) = 0 Then
            fpErr = Freefile()  '' DEBUG
            Open Err For Input As #fpErr  '' DEBUG
            Print #fpErr, "SQLite format 3 Header not found!"  '' DEBUG
            Close #fpErr  '' DEBUG
            Close #fp  '' close the file.
            Return 0
        End If
    End If
    '/
    '' <- END Alternative

    Close #fp  '' close the file.
    Return 1
End Function


''==============================================================================
'' These are really the same routine for most database operations :)
'' You could use this same routine with a little modification to accept and
'' return most sqlite3 database queries.
''
'' To create a table with the correct SQLite 3 header file information we will
'' need to create a table and then delete (DROP) the table. This will leave a p_db
'' file with the first 16 bytes containing "SQLite format 3". We can then use
'' our previous db_file_exists() function to test if it is a valid p_db file.
''
'' This function runs 2 separate SQLite queries. The first creates a temporary
'' (dummy) table and the second query deletes the temporary table leaving an
'' empty SQLite3 database file with a valid header.

Function db_file_create(Byval db_file_name As String) As Integer

	Dim p_db As sqlite3 Ptr  '' database handle (structure).
	Dim p_stmt As sqlite3_stmt Ptr   '' structure represents a single SQL statement
    Dim fpErr As Integer
    'Dim NULL = *Cptr(Any Ptr, 0)  '' This is the correct Ctypes definition of NULL*
    'Dim NULL as any ptr = 0  '' Correct FB definition of NULL. I think it is the same as above.
    'Dim NULL As Integer = 0  '' This is also often used in FB aka Zero==0
    
	Dim return_code As Integer = 0

    '' If the database name exists and is "SQLite format 3", don't try to creat a new p_db.
    If db_file_exists(db_file_name) Then
            fpErr = Freefile()  '' DEBUG
            Open Err For Input As #fpErr  '' DEBUG
            Print #fpErr, "Database: "; db_file_name; " already exists!"  '' DEBUG
            Close #fpErr  '' DEBUG
            Return 2  '' error codes needs to be reviewed.
        End If

    '' defualts to: [SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE]
    '' return_code = sqlite3_open_v2(db_file_name, @p_db, &h00000002 or &h00000004, NULL)
	return_code = sqlite3_open_v2( db_file_name, @p_db, SQLITE_OPEN_READWRITE Or SQLITE_OPEN_CREATE, NULL )
    If return_code <> SQLITE_OK Then  '' integer 0
        '' This is error handling code for the sqlite3_prepare_v2 function call.
        '' Note: these print returns can be commented out if only the Bool return 0|1; is required.
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        Print #fpErr, "Can't open database: "; *Cast(zString Ptr,sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
		Return -1
	End If

    '' This is a single query terminated by the ';' semi colon.
    '' Multiple statements such as the following must be prepared, stepped and
    '' finalized individually.
    '' char *sql = "CREATE TABLE IF NOT EXISTS Temp_table(ID INT);"
    ''             "DROP TABLE IF EXISTS Temp_table;";
    Dim As String sql1 = "CREATE TABLE IF NOT EXISTS Temp_table(ID INTEGER);"

    ''"create table aTable(field1 int); drop table aTable;
    '' We can only send one query at a time to sqlite3.
    '' sqlite3_prepare compiles the sql query into the byte code for sqlite.
    return_code = sqlite3_prepare_v2(p_db, sql1, -1, @p_stmt, NULL)
    '' On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    '' is returned.
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        Print #fpErr, "Failed to prepare data: "; *Cast(zString Ptr,sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
	End If

    '' The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    '' that there is another row ready. Our SQL statement returns only one row
    '' of data in this instance, therefore, we call this function only once.
    '' If we are writing or reading lines of table then we will need to use
    '' sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.
    return_code = sqlite3_step( p_stmt )  '' run once for one statement
    /'    while( sqlite3_step( p_stmt ) == SQLITE_ROW ) {;} '/
    If return_code <> SQLITE_DONE Then  '' SQLITE_DONE==101, SQLITE_ROW==100
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        Print #fpErr, "Step 1 failed: "; *Cast(zString Ptr,sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return 0
	End If

    ''sqlite3_bind_*()  '' After sqlit3_prepare_v2()
    ''sqlite3_clear_bindings(p_stmt);

    '' The sqlite3_finalize function destroys the prepared statement object.
    '' and commits the changes to the database file.
    return_code = sqlite3_finalize( p_stmt )  '' Commit to the database.
    If return_code <> SQLITE_OK Then  '' SQLITE_OK==0
        '' This is error handling code.
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        Print #fpErr, "Failed to finalize data: "; *Cast(zString Ptr,sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

''==============================================================================
    '' The following deletes the temporary table leaving the SQLite header files intact.

    '' The database file is already open...
    /'    return_code = sqlite3_open_v2( db_file_name, &p_db, SQLITE_OPEN_READWRITE, NULL );
	return_code = sqlite3_open_v2( db_file_name, @p_db, SQLITE_OPEN_READWRITE, NULL )
    If return_code <> SQLITE_OK Then  '' integer 0
        '' This is error handling code for the sqlite3_prepare_v2 function call.
        '' Note: these print returns can be commented out if only the Bool return 0|1; is required.
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        Print #fpErr, "Can't open database: "; *Cast(zString Ptr,sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
		Return -1
	End If
    '/

    '' Consider Dim sql2 As zString ptr = "DROP TABLE IF EXISTS Temp_table;"
    Dim As String sql2 = "DROP TABLE IF EXISTS Temp_table;"  '' Delete the table.

    ''"create table aTable(field1 int); drop table aTable;
    return_code = sqlite3_prepare_v2(p_db, sql2, -1, @p_stmt, NULL)
    '' On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    '' is returned.
    If return_code <> SQLITE_OK Then
        '' This is error handling code for the sqlite3_prepare_v2 function call.
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        Print #fpErr, "Failed to prepare data: "; *Cast(zString Ptr,sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
		Return -1
	End If

    '' The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    '' that there is another row ready. Our SQL statement returns only one row
    '' of data in this instance, therefore, we call this function only once.
    '' If we are writing or reading lines of table then we will need to use
    '' sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.
    return_code = sqlite3_step( p_stmt )
    /'    while( sqlite3_step( p_stmt ) == SQLITE_ROW ) {;}'/
    If return_code <> SQLITE_DONE Then  '' SQLITE_DONE==101, SQLITE_ROW==100
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        Print #fpErr, "Step 2 failed: "; *Cast(zString Ptr,sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
		Return 0
	End If

    ''sqlite3_bind_*()  '' After sqlit3_prepare_v2()
    ''sqlite3_clear_bindings(p_stmt);

    '' The sqlite3_finalize function destroys the prepared statement object.
    return_code = sqlite3_finalize( p_stmt )  '' Commit to the database.
    If return_code <> SQLITE_OK Then  '' SQLITE_OK==0
        '' This is error handling code.
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        Print #fpErr, "Failed to finalize data: "; *Cast(zString Ptr,sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    '' The sqlite3_close function closes the database connection.
    return_code = sqlite3_close(p_db)   '' SQLITE_OK==0
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        Print #fpErr, "Failed to close database: "; *Cast(zString Ptr,sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    Print "Successfully created "; db_file_name; " DQLite3 database."  '' DEBUG

    Return 1
End Function


'' ####### This needs to be reviewed !!!!
'' Delete the SQLite3 database file, This is really just a standard file delete
'' function with an extra safety y/n check. We could also include db_file_exists()
'' to test if it is a valid SQLite database that is being deleted. Always
'' include some form of safety check before deleting any files on a users system
'' as file deletes cannot be undone.
Function db_file_delete(Byval db_file_name As String) As Integer
    Dim promt As String
    Dim return_code As Integer = 0
    Dim fpErr As Integer
    fpErr = Freefile()  '' DEBUG
    Open Err For Input As #fpErr  '' DEBUG

    Print "Are you sure you want to delete "; db_file_name; " (y/n) ?";
    Input promt
    If ( promt = "y") Or ( promt = "Y") Then  '' orelse
        return_code = Kill(db_file_name)
        If return_code = 0 Then
            Print #fpErr, "Successfully deleted "; db_file_name  '' DEBUG
            Close #fpErr  '' DEBUG
            Return 1
        Else
            Print #fpErr, "Unable to delete or not exists "; db_file_name  '' DEBUG
            Close #fpErr  '' DEBUG
            Return -1
        End If

    Else  '' any not 'y'
        Print #fpErr, "File "; db_file_name; " aborted by user."  '' DEBUG
        Close #fpErr  '' DEBUG
        Return 0  '' Aborted delete file.
    End If

    Return 0
End Function


'' Test if a table name exist in a database.
'' https:''fossil-scm.org/home/info/4da2658323cab60e?ln=1945-1951
'' https:''www.sqlite.org/c3ref/table_column_metadata.html
Function db_table_exists(Byval db_file_name As String, Byval db_table_name As String) As Integer

	Dim p_db As sqlite3 Ptr  '' database handle (structure).
	Dim p_stmt As sqlite3_stmt Ptr '' structure represents a single SQL statement
    Dim return_code As Integer = 0  '' sqlite3 return codes.
    Dim err_ret As Integer = 0
    Dim fpErr As Integer  '' This function error codes.

    '' defualts to: [SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE]
    '' return_code = sqlite3_open_v2(db_file_name, @p_db, &h00000002 or &h00000004, NULL)
	return_code = sqlite3_open_v2( db_file_name, @p_db, SQLITE_OPEN_READWRITE Or SQLITE_OPEN_CREATE, NULL )
    If return_code <> SQLITE_OK Then  '' integer 0
        '' This is error handling code for the sqlite3_prepare_v2 function call.
        '' Note: these print returns can be commented out if only the Bool return 0|1; is required.
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        Print #fpErr, "Can't open database: "; *Cast(zString Ptr,sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
		Return -1
	End If

    '' Alternative method.
    '' https:''cppsecrets.com/users/1128311897114117110109107110505764103109971051084699111109/C00-SQLitesqlite3free.php
    '' char *sql = "SELECT count(*) FROM sqlite_master WHERE type = 'table' AND name = 'YourTableName';";
    '' const char *sql = "SELECT 1 FROM sqlite_master where type='table' and name=?";  <--

    '' List all table names <- New function
    '' !! Recheck with the following !!!
    /'
        Dim As String sql_table_list = "SELECT name FROM sqlite_master WHERE type='table'"

        return_code = sqlite3_prepare_v2(p_db, sql_table_list, Len(sql_table_list), @p_stmt, NULL)
        If return_code = SQLITE_OK Then

            '' Loop through all the tables
            While (sqlite3_step(p_stmt) = SQLITE_ROW)
                Dim As String buffer *Cast(zString Ptr,sqlite3_column_text(p_stmt, 0))

                If buffer <> table_name
                    Return True
                End If
                Wend
        End If
    '/

    '' ##### This may need a loop!!!!
    '' Currently only tested with single table!!!
    '' db_handle, db_name, db_table_name, col_name, NULL, ...
    return_code = sqlite3_table_column_metadata(p_db, NULL, db_table_name, NULL, NULL, NULL, NULL, NULL, NULL)
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        Print #fpErr, "Table did not exist: "; *Cast(zString Ptr,sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        err_ret = 0
    Else
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        Print #fpErr, "Table exists: "; *Cast(zString Ptr,sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        err_ret = 1
    End If

    '' The sqlite3_close function closes the database connection.
    return_code = sqlite3_close(p_db)   '' SQLITE_OK==0
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Failed to close database: "; *Cast(zString Ptr,sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    Return err_ret  ''Return this function error code.
End Function


'' Get the total number of tables in a named database file.
Function db_get_number_tables(Byval db_file_name As String, Byref number_tables_ret As Integer) As Integer

	Dim p_db As sqlite3 Ptr  '' database handle (structure).
	Dim p_stmt As sqlite3_stmt Ptr '' structure represents a single SQL statement
    Dim return_code As Integer = 0  '' sqlite3 return codes.
    'dim err_ret as Integer = 0
    Dim fpErr As Integer  '' This function error codes.

    Dim As String buffer  '' temp buffer [MAX 128 characters]
    'strcpy(buffer, "");  '' before strcat()
    Dim As Integer table_count = 0

    return_code = sqlite3_open_v2( db_file_name, @p_db, SQLITE_OPEN_READONLY, NULL )
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Can't open database: "; *Cast(zString Ptr,sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    Const As String sql_table_list = "SELECT * FROM sqlite_schema WHERE type='table' AND name NOT LIKE 'sqlite_%' ORDER BY 1;"  '' sqlite_schema, sqlite_master

    return_code = sqlite3_prepare_v2(p_db, sql_table_list, -1, @p_stmt, NULL)
    '' On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    '' is returned.
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Failed to prepare data: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    '' The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    '' that there is another row ready. Our SQL statement returns only one row
    '' of data at a time.
    '' If we are writing or reading multiple lines of table then we will need to
    '' use sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.
    While sqlite3_step(p_stmt) = SQLITE_ROW  '' SQLITE_ROW == 100
        buffer = buffer + *Cast(zString Ptr, sqlite3_column_text(p_stmt, 1))
        table_count += 1  '' Count the number of table names found.
    Wend

    number_tables_ret = table_count  '' populate the return buffer.

    ''sqlite3_bind_*()  '' After sqlit3_prepare_v2()
    ''sqlite3_clear_bindings(p_stmt);

    '' The sqlite3_finalize function destroys the prepared statement object.
    return_code = sqlite3_finalize( p_stmt )  '' Commit to the database.
    If return_code <> SQLITE_OK Then  '' SQLITE_OK==0
        '' This is error handling code.
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Failed to finalize data: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    '' The sqlite3_close function closes the database connection.
    return_code = sqlite3_close(p_db)   '' SQLITE_OK==0
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Failed to close database: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    Return 1  ''Return this function error code.
End Function


'' The same as above function, except this time we return the names of each table
'' in the database file.
'' Note: not using ByRef to pass the array.
Function db_get_tablenames(Byval db_file_name As String, db_tablenames() As String) As Integer

	Dim p_db As sqlite3 Ptr  '' database handle (structure).
	Dim p_stmt As sqlite3_stmt Ptr '' structure represents a single SQL statement
    Dim return_code As Integer = 0  '' sqlite3 return codes.
    'dim err_ret as Integer = 0
    Dim fpErr As Integer  '' This function error codes.

    Dim As String buffer  '' temp buffer [MAX 128 characters]
    'strcpy(buffer, "");  '' before strcat()
    Dim As Integer table_count = 0

    return_code = sqlite3_open_v2( db_file_name, @p_db, SQLITE_OPEN_READONLY, NULL )
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Can't open database: "; *Cast(zString Ptr,sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    Const As String sql_table_list = "SELECT * FROM sqlite_schema WHERE type='table' AND name NOT LIKE 'sqlite_%' ORDER BY 1;"  '' sqlite_schema, sqlite_master

    return_code = sqlite3_prepare_v2(p_db, sql_table_list, -1, @p_stmt, NULL)
    '' On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    '' is returned.
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code.
        Print #fpErr, "Failed to prepare data: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If


    '' Loop through all of the tables.
    While sqlite3_step(p_stmt) = SQLITE_ROW
        db_tablenames(table_count) = *Cast(zString Ptr, sqlite3_column_text(p_stmt, 1))
        
        'strcat(db_tablenames[table_count], (const char*)sqlite3_column_text(p_stmt, 1))
        table_count += 1  '' Update the next array data element.
    Wend

    '' number_tables_ret = table_count  '' populate the return buffer.

    ''sqlite3_bind_*()  '' After sqlit3_prepare_v2()
    ''sqlite3_clear_bindings(p_stmt);

    '' The sqlite3_finalize function destroys the prepared statement object.
    return_code = sqlite3_finalize( p_stmt )  '' Commit to the database.
    If return_code <> SQLITE_OK Then  '' SQLITE_OK==0
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Failed to finalize data: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    '' The sqlite3_close function closes the database connection.
    return_code = sqlite3_close(p_db)   '' SQLITE_OK==0
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Failed to close database: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    Return 1  ''Return this function error code.
End Function


'' Insert a table with column identifiers into a database.
'' This will NOT create a database file if it does not already exist.
'' Must use db_file_create()
'' This will NOT overwrite a previous table of the same name if it exists.
'' I may need to create a test or modify this function for safety.
Function db_table_create(Byval db_file_name As String, Byval db_table As String) As Integer

	Dim p_db As sqlite3 Ptr  '' database handle (structure).
	Dim p_stmt As sqlite3_stmt Ptr '' structure represents a single SQL statement
    Dim return_code As Integer = 0  '' sqlite3 return codes.
    'dim err_ret as Integer = 0
    Dim fpErr As Integer  '' This function error codes.

    return_code = sqlite3_open_v2( db_file_name, @p_db, SQLITE_OPEN_READWRITE, NULL )
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Can't open database: "; *Cast(zString Ptr,sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    '' In this function I have formatted the entire query (statement) prior to
    '' sending it to the function *db_table. As you will se in following
    '' examples we can also create a generic template for the statement and
    '' only send the column names and value data to be constructed into a full
    '' query using string concatenation or sqlite3_bind*().

    ''"CREATE TABLE IF NOT EXISTS TableName (Col_Name TYPE);"
    '' Note: If the table exist already no error is returned.
    '' We can only send one query at a time to sqlite3.
    return_code = sqlite3_prepare_v2(p_db, db_table, -1, @p_stmt, NULL)
    '' On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    '' is returned.
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code.
        Print #fpErr, "Failed to prepare data: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    '' The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    '' that there is another row ready. Our SQL statement returns only one row
    '' of data at a time.
    '' If we are writing or reading multiple lines of table then we will need to
    '' use sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.
    return_code = sqlite3_step( p_stmt )  '' run once for one statement
    /'    while( sqlite3_step( p_stmt ) == SQLITE_ROW ) {;} '/
    If return_code <> SQLITE_DONE Then  '' SQLITE_DONE==101, SQLITE_ROW==100
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code.
        Print #fpErr, "Step failed: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return 0
    End If

    ''sqlite3_bind_*()  '' After sqlit3_prepare_v2()
    ''sqlite3_clear_bindings(p_stmt);

    '' The sqlite3_finalize function destroys the prepared statement object.
    return_code = sqlite3_finalize( p_stmt )  '' Commit to the database.
    If return_code <> SQLITE_OK Then  '' SQLITE_OK==0
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Failed to finalize data: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    '' The sqlite3_close function closes the database connection.
    return_code = sqlite3_close(p_db)   '' SQLITE_OK==0
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Failed to close database: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    fpErr = Freefile()  '' DEBUG
    Open Err For Input As #fpErr  '' DEBUG
    Print #fpErr, "Successfully created table in "; db_file_name  '' DEBUG
    Close #fpErr  '' DEBUG

    Return 1
End Function


'' Delete (DROP) table from a named database file.
'' Will return -1 if the database file does not exists.
Function db_table_delete(Byval db_file_name As String, Byval db_table_name As String) As Integer

    Dim p_db As sqlite3 Ptr  '' database handle (structure).
	Dim p_stmt As sqlite3_stmt Ptr '' structure represents a single SQL statement
    Dim return_code As Integer = 0  '' sqlite3 return codes.
    'dim err_ret as Integer = 0
    Dim fpErr As Integer  '' This function error codes.

    return_code = sqlite3_open_v2( db_file_name, @p_db, SQLITE_OPEN_READWRITE, NULL )
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Can't open database: "; *Cast(zString Ptr,sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    '' In this function I have formatted the entire query (statement) prior to
    '' sending it to the function *db_table. As you will see in following
    '' examples we can also create a generic template for the statement and
    '' only send the column names and value data to be constructed into a full
    '' query using string concatenation or sqlite3_bind*().

    '' "DROP TABLE IF EXISTS TableName;";
    '' We can only send one query at a time to sqlite3.
    return_code = sqlite3_prepare_v2(p_db, db_table_name, -1, @p_stmt, NULL)
    '' On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    '' is returned.
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code.
        Print #fpErr, "Failed to prepare data: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    '' The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    '' that there is another row ready. Our SQL statement returns only one row
    '' of data at a time.
    '' If we are writing or reading multiple lines of table then we will need to
    '' use sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.
    return_code = sqlite3_step( p_stmt )  '' run once for one statement
    /'    while( sqlite3_step( p_stmt ) == SQLITE_ROW ) {;} '/
    If return_code <> SQLITE_DONE Then  '' SQLITE_DONE==101, SQLITE_ROW==100
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code.
        Print #fpErr, "Step failed: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return 0
    End If

    ''sqlite3_bind_*()  '' After sqlit3_prepare_v2()
    ''sqlite3_clear_bindings(p_stmt);

    '' The sqlite3_finalize function destroys the prepared statement object.
    return_code = sqlite3_finalize( p_stmt )  '' Commit to the database.
    If return_code <> SQLITE_OK Then  '' SQLITE_OK==0
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Failed to finalize data: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    '' The sqlite3_close function closes the database connection.
    return_code = sqlite3_close(p_db)   '' SQLITE_OK==0
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Failed to close database: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    fpErr = Freefile()  '' DEBUG
    Open Err For Input As #fpErr  '' DEBUG
    Print #fpErr, "Successfully deleted table ";db_table_name ; " in "; db_file_name  '' DEBUG
    Close #fpErr  '' DEBUG

    Return 1
End Function


''============================================================================>>

'' Get the number of rows from a table.
Function db_get_table_number_rows(Byval db_file_name As String, Byval db_table_name As String, Byref number_rows_ret As Integer) As Integer

    Dim p_db As sqlite3 Ptr  '' database handle (structure).
	Dim p_stmt As sqlite3_stmt Ptr '' structure represents a single SQL statement
    Dim return_code As Integer = 0  '' sqlite3 return codes.
    Dim row_cnt As Integer = 0
    Dim fpErr As Integer  '' This function error codes.

    return_code = sqlite3_open_v2( db_file_name, @p_db, SQLITE_OPEN_READONLY, NULL )
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Can't open database: "; *Cast(zString Ptr,sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    '' Unlike previous functions, I am using a template to create the statement.
    '' I am concatenating the additional information to form the full query
    '' statement. This allows us to use a generic template providing only the
    '' necessary data. See info in sql injection.

    '' SELECT COUNT(*) FROM TableName;  '' Returns rows. COUNT(*|ALL|DISTINCT] expression)
    '' Alternative select max(rowid) from TableName;
    '' See Column name search and counts for the column count usage.
    Dim As String db_row_search = "SELECT COUNT(*) FROM "  '' Don't use long table name max[128 - 22]
    db_row_search += db_table_name
    'strcat( db_row_search, db_table_name);
    db_row_search += ";"
    'strcat( db_row_search, ";");

    '' We can only send one query at a time to sqlite3.
    return_code = sqlite3_prepare_v2(p_db, db_row_search, -1, @p_stmt, NULL)
    '' On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    '' is returned.
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code.
        Print #fpErr, "Failed to prepare data: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    '' The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    '' that there is another row ready. Our SQL statement returns only one row
    '' of data at a time.
    '' If we are writing or reading multiple lines of table then we will need to
    '' use sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.
    return_code = sqlite3_step(p_stmt)
    If return_code <> SQLITE_ROW Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code.
        Print #fpErr, "Step failed: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    row_cnt = sqlite3_column_int(p_stmt, 0)  '' Retrieve the row count as int.
    number_rows_ret = row_cnt  '' Populate int number_rows from calling function.

    ''sqlite3_bind_*()  '' After sqlit3_prepare_v2()
    ''sqlite3_clear_bindings(p_stmt);

    '' The sqlite3_finalize function destroys the prepared statement object.
    return_code = sqlite3_finalize( p_stmt )  '' Commit to the database.
    If return_code <> SQLITE_OK Then  '' SQLITE_OK==0
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Failed to finalize data: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    '' The sqlite3_close function closes the database connection.
    return_code = sqlite3_close(p_db)   '' SQLITE_OK==0
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Failed to close database: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    fpErr = Freefile()  '' DEBUG
    Open Err For Input As #fpErr  '' DEBUG
    Print #fpErr, "Successfully retrieved table row number from "; db_file_name  '' DEBUG
    Close #fpErr  '' DEBUG

    Return 1
End Function


'' Get number of columns in a TableName.
Function db_get_table_number_cols(Byval db_file_name As String, Byval db_table_name As String, Byref number_cols_ret As Integer) As Integer

    Dim p_db As sqlite3 Ptr  '' database handle (structure).
	Dim p_stmt As sqlite3_stmt Ptr '' structure represents a single SQL statement
    Dim return_code As Integer = 0  '' sqlite3 return codes.
    Dim col_cnt As Integer = 0
    Dim fpErr As Integer  '' This function error codes.

    return_code = sqlite3_open_v2( db_file_name, @p_db, SQLITE_OPEN_READONLY, NULL )
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Can't open database: "; *Cast(zString Ptr,sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    Dim As String db_col_search = "SELECT COUNT(*) FROM pragma_table_info('"  '' Don't use long table name max[128 - 43]
    db_col_search += db_table_name
    'strcat( db_col_search, db_table_name);
    db_col_search += "');"
    'strcat( db_col_search, "');")

    '' We can only send one query at a time to sqlite3.
    return_code = sqlite3_prepare_v2(p_db, db_col_search, -1, @p_stmt, NULL)
    '' On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    '' is returned.
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code.
        Print #fpErr, "Failed to prepare data: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If


    '' The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    '' that there is another row ready. Our SQL statement returns only one row
    '' of data at a time.
    '' If we are writing or reading multiple lines of table then we will need to
    '' use sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.
    return_code = sqlite3_step(p_stmt)
    If return_code <> SQLITE_ROW Then  '' SQLITE_DONE==101, SQLITE_ROW==100
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code.
        Print #fpErr, "Step failed: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    col_cnt = sqlite3_column_int(p_stmt, 0)  '' Retrieve the row count as int.
    number_cols_ret = col_cnt  '' Populate int number_rows from calling function.

    ''sqlite3_bind_*()  '' After sqlit3_prepare_v2()
    ''sqlite3_clear_bindings(p_stmt);

    '' The sqlite3_finalize function destroys the prepared statement object.
    return_code = sqlite3_finalize( p_stmt )  '' Commit to the database.
    If return_code <> SQLITE_OK Then  '' SQLITE_OK==0
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Failed to finalize data: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    '' The sqlite3_close function closes the database connection.
    return_code = sqlite3_close(p_db)   '' SQLITE_OK==0
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Failed to close database: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    fpErr = Freefile()  '' DEBUG
    Open Err For Input As #fpErr  '' DEBUG
    Print #fpErr, "Successfully retrieved table column number from "; db_file_name  '' DEBUG
    Close #fpErr  '' DEBUG

    Return 1
End Function


'' Get the column names as 2D array from a named table.
'' Note: not using ByRef to pass the array.
Function db_get_table_colnames(Byval db_file_name As String, Byval db_table_name As String, db_tbl_col_name() As String) As Integer

    Dim p_db As sqlite3 Ptr  '' database handle (structure).
	Dim p_stmt As sqlite3_stmt Ptr '' structure represents a single SQL statement
    Dim return_code As Integer = 0  '' sqlite3 return codes.
    Dim fpErr As Integer  '' This function error codes.

    return_code = sqlite3_open_v2( db_file_name, @p_db, SQLITE_OPEN_READONLY, NULL )
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Can't open database: "; *Cast(zString Ptr,sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    '' There are other methods to do this, so this statement can be revised.
    Dim As String db_row_search2 = "PRAGMA table_info('"  '' Don't use long table name max[128 - 22]
    db_row_search2 += db_table_name
    'strcat( db_row_search2, db_table_name);
    db_row_search2 += "');"
    'strcat( db_row_search2, "');");

    '' We can only send one query at a time to sqlite3.
    return_code = sqlite3_prepare_v2(p_db, db_row_search2, -1, @p_stmt, NULL)
    '' On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    '' is returned.
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code.
        Print #fpErr, "Failed to prepare data: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    Dim As Integer col_cnt = 0  '' Count number of columns.

    '' The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    '' that there is another row ready. Our SQL statement returns only one row
    '' of data at a time.
    '' If we are writing or reading multiple lines of table then we will need to
    '' use sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.
    While sqlite3_step(p_stmt) <> SQLITE_DONE
        '' To handle the return of NULL pointers as a string
        '' data = (const char*)sqlite3_column_text( p_stmt, i );
        ''        printf( "%s\n", data ? data : "[NULL]" );
        ''        sprintf(buffer[strlen(data)], "%s\n", data ? data : "[NULL]" );

        db_tbl_col_name(col_cnt) = *Cast(zString Ptr, sqlite3_column_text(p_stmt, 1))

        '' db_tbl_col_name[col_cnt][String_MAX_Length (2048)]
        col_cnt += 1
        Wend

    ''sqlite3_bind_*()  '' After sqlit3_prepare_v2()
    ''sqlite3_clear_bindings(p_stmt);

    '' The sqlite3_finalize function destroys the prepared statement object.
    return_code = sqlite3_finalize( p_stmt )  '' Commit to the database.
    If return_code <> SQLITE_OK Then  '' SQLITE_OK==0
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Failed to finalize data: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    '' The sqlite3_close function closes the database connection.
    return_code = sqlite3_close(p_db)   '' SQLITE_OK==0
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Failed to close database: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    fpErr = Freefile()  '' DEBUG
    Open Err For Input As #fpErr  '' DEBUG
    Print #fpErr, "Successfully retrieved table column names from "; db_file_name  '' DEBUG
    Close #fpErr  '' DEBUG

    Return 1
End Function



''============================================================================<<


'' Insert row data into a named table.
'' If the data already exists will create a new row.
'' Table unique index rowid is auto generated to the next available position
'' in this table.
Function db_insert_table_rowdata(Byval db_file_name As String, Byval db_tbl_entry As String) As Integer

    Dim p_db As sqlite3 Ptr  '' database handle (structure).
	Dim p_stmt As sqlite3_stmt Ptr '' structure represents a single SQL statement
    Dim return_code As Integer = 0  '' sqlite3 return codes.
    Dim fpErr As Integer  '' This function error codes.

    return_code = sqlite3_open_v2( db_file_name, @p_db, SQLITE_OPEN_READWRITE, NULL )
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Can't open database: "; *Cast(zString Ptr,sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    '' In this function I am supplying to full query statement to the function.
    '' You can alter this based upon other functions using templates :)

    '' We can only send one query at a time to sqlite3.
    return_code = sqlite3_prepare_v2(p_db, db_tbl_entry, -1, @p_stmt, NULL)
    '' On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    '' is returned.
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code.
        Print #fpErr, "Failed to prepare data: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    '' The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    '' that there is another row ready. Our SQL statement returns only one row
    '' of data at a time.
    '' If we are writing or reading multiple lines of table then we will need to
    '' use sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.
    return_code = sqlite3_step( p_stmt )  '' run once for one statement
    /'    while( sqlite3_step( p_stmt ) == SQLITE_ROW ) {;} '/
    If return_code <> SQLITE_DONE Then  '' SQLITE_DONE==101, SQLITE_ROW==100
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code.
        Print #fpErr, "Step failed: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return 0
    End If

    ''sqlite3_bind_*()  '' After sqlit3_prepare_v2()
    ''sqlite3_clear_bindings(p_stmt);

    '' The sqlite3_finalize function destroys the prepared statement object.
    return_code = sqlite3_finalize( p_stmt )  '' Commit to the database.
    If return_code <> SQLITE_OK Then  '' SQLITE_OK==0
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Failed to finalize data: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    '' The sqlite3_close function closes the database connection.
    return_code = sqlite3_close(p_db)   '' SQLITE_OK==0
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Failed to close database: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    fpErr = Freefile()  '' DEBUG
    Open Err For Input As #fpErr  '' DEBUG
    Print #fpErr, "Successfully inserted rowdata into table in "; db_file_name  '' DEBUG
    Close #fpErr  '' DEBUG

    Return 1
End Function


'' Delete the row data searched by name matching etc.
'' ## Only for tables where all field types are TEXT (String) except rowid. ##
'' This function is dangerous and needs to be revised!!!!
Function db_delete_table_rowdata(Byval db_file_name As String, Byval db_row_entry As String) As Integer '' add delete at row_id

    Dim p_db As sqlite3 Ptr  '' database handle (structure).
	Dim p_stmt As sqlite3_stmt Ptr '' structure represents a single SQL statement
    Dim return_code As Integer = 0  '' sqlite3 return codes.
    Dim fpErr As Integer  '' This function error codes.

    return_code = sqlite3_open_v2( db_file_name, @p_db, SQLITE_OPEN_READWRITE, NULL )
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Can't open database: "; *Cast(zString Ptr,sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    '' We can only send one query at a time to sqlite3.
    return_code = sqlite3_prepare_v2(p_db, db_row_entry, -1, @p_stmt, NULL)
    '' On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    '' is returned.
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code.
        Print #fpErr, "Failed to prepare data: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    '' The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    '' that there is another row ready. Our SQL statement returns only one row
    '' of data at a time.
    '' If we are writing or reading multiple lines of table then we will need to
    '' use sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.
    return_code = sqlite3_step( p_stmt )  '' run once for one statement
    /'    while( sqlite3_step( p_stmt ) == SQLITE_ROW ) {;} '/
    If return_code <> SQLITE_DONE Then  '' SQLITE_DONE==101, SQLITE_ROW==100
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code.
        Print #fpErr, "Step failed: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return 0
    End If

    ''sqlite3_bind_*()  '' After sqlit3_prepare_v2()
    ''sqlite3_clear_bindings(p_stmt);

    '' The sqlite3_finalize function destroys the prepared statement object.
    return_code = sqlite3_finalize( p_stmt )  '' Commit to the database.
    If return_code <> SQLITE_OK Then  '' SQLITE_OK==0
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Failed to finalize data: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    '' The sqlite3_close function closes the database connection.
    return_code = sqlite3_close(p_db)   '' SQLITE_OK==0
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Failed to close database: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    fpErr = Freefile()  '' DEBUG
    Open Err For Input As #fpErr  '' DEBUG
    Print #fpErr, "Successfully removed rowdata from table in "; db_file_name  '' DEBUG
    Close #fpErr  '' DEBUG

    Return 1
End Function


'' List all data from a named table to a dynamic array.
'' ## Only for tables where all field types are TEXT (String) except rowid. ##
Function db_list_table_rows_data(Byval db_file_name As String, Byval db_table_name As String, db_tbl_rowdata() As String, Byval number_columns As Integer) As Integer

    number_columns += 1  '' We are also including an extra column for the row ID number.
    Dim p_db As sqlite3 Ptr  '' database handle (structure).
	Dim p_stmt As sqlite3_stmt Ptr '' structure represents a single SQL statement
    Dim return_code As Integer = 0  '' sqlite3 return codes.
    Dim fpErr As Integer  '' This function error codes.

    return_code = sqlite3_open_v2( db_file_name, @p_db, SQLITE_OPEN_READWRITE, NULL )
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Can't open database: "; *Cast(zString Ptr,sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    Dim As String sql_concat = ""
    'strcpy(sql_concat, "");

    '' SELECT rowid, * FROM '' To include table row ID number.
    Dim As String sql = "SELECT rowid, * FROM "  '' Note the space after FROM

    sql_concat += sql
    'strcat(sql_concat, sql);  '' Add SQL query statement.
    sql_concat += db_table_name
    'strcat(sql_concat, db_table_name);  '' Add table name to statement.

    '' char *db_table1 = "DROP TABLE IF EXISTS Hrs_worked_Tracker;";
    ''"create table aTable(field1 int); drop table aTable;
    '' We can only send one query at a time to sqlite3.
    return_code = sqlite3_prepare_v2(p_db, sql_concat, -1, @p_stmt, NULL)
    '' On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    '' is returned.
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code.
        Print #fpErr, "Failed to prepare data: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    '' The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    '' that there is another row ready. Our SQL statement returns only one row
    '' of data at a time, therefore, we call this function only once at a time.
    '' If we are writing or reading multiple lines of table then we will need to
    '' use sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.

    Dim As Integer cnt_row = 0  '' array row.
    Dim As Integer cnt_col = 0
    Dim As String buffer  '' temp buffer [MAX 128 characters]

    While (sqlite3_step(p_stmt) = SQLITE_ROW)
        '' Can use sqlite3_column_count(statement/p_stmt) in place of number_columns.
        For cnt_col = 0 To number_columns -1 Step +1  '' Count 0 to number_columns-1
            '' To handle the return of NULL pointers as a string.
            '' data = (const char*)sqlite3_column_text( p_stmt, cnt_col );
            ''        printf( "%s\n", data ? data : "[NULL]" );
            ''        sprintf(buffer[strlen(data)], "%s\n", data ? data : "[NULL]" );

            '' copy each entry to a buffer. Each entry is a column.
            'strcpy(buffer, (const char*)sqlite3_column_text(p_stmt, cnt_col)); 
            buffer = *Cast(zString Ptr, sqlite3_column_text(p_stmt, cnt_col))  '' cnt_col == array column.

            '' Concatenate buffer to our return array.
            'strcat(db_tbl_rowdata[cnt_row], buffer);
            db_tbl_rowdata(cnt_row) += buffer
            If cnt_col < number_columns -1 Then '' Don't add ', ' after last column.
                db_tbl_rowdata(cnt_row) += ","  '' add separator token between each col.
            Else  '' Add line return, end of row.
                ''strcat(db_tbl_rowdata[cnt_row], "\n");
            End If
            ''cnt_col++;  '' Need 3D array.
        Next cnt_col

        cnt_row += 1
    Wend

    ''sqlite3_bind_*()  '' After sqlit3_prepare_v2()
    ''sqlite3_clear_bindings(p_stmt);

    '' The sqlite3_finalize function destroys the prepared statement object.
    return_code = sqlite3_finalize( p_stmt )  '' Commit to the database.
    If return_code <> SQLITE_OK Then  '' SQLITE_OK==0
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Failed to finalize data: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    '' The sqlite3_close function closes the database connection.
    return_code = sqlite3_close(p_db)   '' SQLITE_OK==0
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Failed to close database: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    fpErr = Freefile()  '' DEBUG
    Open Err For Input As #fpErr  '' DEBUG
    Print #fpErr, "Successfully retrieved table data from "; db_file_name  '' DEBUG
    Close #fpErr  '' DEBUG

    Return 1
End Function


'' Test if rowid exist in a table.
Function db_table_rowid_exists(Byval db_file_name As String, Byval db_table_name As String, Byval tbl_rowid As Integer) As Integer

    Dim p_db As sqlite3 Ptr  '' database handle (structure).
	Dim p_stmt As sqlite3_stmt Ptr '' structure represents a single SQL statement
    Dim return_code As Integer = 0  '' sqlite3 return codes.
    Dim err_ret As Integer = 0
    Dim fpErr As Integer  '' This function error codes.


    return_code = sqlite3_open_v2( db_file_name, @p_db, SQLITE_OPEN_READONLY, NULL )
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Can't open database: "; *Cast(zString Ptr,sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If


    Dim sql_concat As String  '' temp buffer [MAX 128 characters]
    Dim rowid_buffer As String  '' convert int to char for statement concat.
    '' Clear the buffer from the last statement. strcat() will concat on to
    '' the previous statement otherwise.
    sql_concat = ""

    rowid_buffer =  Str(tbl_rowid) '' convert rowid int to string.

    '' SELECT EXISTS(SELECT 1 FROM myTbl WHERE WHERE rowid = tbl_rowid);
    '' "SELECT rowid, * FROM "

    '' Both of the following queries will return a correct result.
    Dim As String sql1 = "SELECT EXISTS(SELECT 1 FROM "
    Dim As String sql2 = " WHERE rowid = "
    Dim As String sql3 = ");"

    ''Dim As String sql1 = "SELECT Count() FROM "
    ''Dim As String sql2 = " WHERE rowid = "
    ''Dim As String sql3 = ";"

    'strcat(sql_concat, sql1);  // Add SQL query statement.
    sql_concat += sql1
    'strcat(sql_concat, db_table_name);  // Add SQL query statement.
    sql_concat += db_table_name
    'strcat(sql_concat, sql2);  // Add SQL query statement.
    sql_concat += sql2
    'strcat(sql_concat, rowid_buffer);  // Add SQL query statement.
    sql_concat += rowid_buffer
    'strcat(sql_concat, sql3);
    sql_concat += sql3

    '' We can only send one query at a time to sqlite3.
    return_code = sqlite3_prepare_v2(p_db, sql_concat, -1, @p_stmt, NULL)
    '' On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    '' is returned.
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code.
        Print #fpErr, "Failed to prepare data: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If


    return_code = sqlite3_step( p_stmt )  '' run once for one statement
    If return_code <> SQLITE_ROW Then  '' SQLITE_DONE==101, SQLITE_ROW==100
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code.
        Print #fpErr, "Step failed: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return 0
    End If

    return_code = sqlite3_column_int(p_stmt, 0)
    If return_code = 0 Then
        err_ret = 0
    Else  '' ==1
        err_ret = 1
    End If

    ''sqlite3_bind_*()  // After sqlit3_prepare_v2()
    ''sqlite3_clear_bindings(p_stmt);

    '' The sqlite3_finalize function destroys the prepared statement object.
    return_code = sqlite3_finalize( p_stmt )  '' Commit to the database.
    If return_code <> SQLITE_OK Then  '' SQLITE_OK==0
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Failed to finalize data: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    '' The sqlite3_close function closes the database connection.
    return_code = sqlite3_close(p_db)   '' SQLITE_OK==0
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Failed to close database: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    Return err_ret
End Function



'' delete row by "rowid"
'' Delete the row data by rowid.
Function db_delete_table_rowdata_rowid(Byval db_file_name As String, Byval db_table_name As String, Byval sql_rowid As Integer) As Integer

    Dim p_db As sqlite3 Ptr  '' database handle (structure).
	Dim p_stmt As sqlite3_stmt Ptr '' structure represents a single SQL statement
    Dim return_code As Integer = 0  '' sqlite3 return codes.
    Dim fpErr As Integer  '' This function error codes.

    return_code = sqlite3_open_v2( db_file_name, @p_db, SQLITE_OPEN_READWRITE, NULL )
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Can't open database: "; *Cast(zString Ptr,sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    Dim As String sql_concat  '' temp buffer [MAX 128 characters]
    Dim As String rowid_buffer  '' convert int to char for statement concat.
    '' Clear the buffer from the last statement. strcat() will concat on to
    '' the previous statement otherwise.
    'strcpy(sql_concat, "");

    rowid_buffer = Str(sql_rowid)
    'sprintf(rowid_buffer, "%d", sql_rowid); '' convert rowid int to string.

    ''"DELETE FROM TableName WHERE rowid = n;"

    Dim As String sql1 = "DELETE FROM "  ''
    Dim As String sql2 = " WHERE rowid = "  ''
    Dim As String sql3 = ";"  ''

    '' This can be replaced with sprintf()
    sql_concat += sql1
    'strcat(sql_concat, sql1);  '' Add SQL query statement.
    sql_concat += db_table_name
    'strcat(sql_concat, db_table_name);  '' Add table name to statement.
    sql_concat += sql2
    'strcat(sql_concat, sql2);  '' Add " WHERE rowid =  "
    sql_concat += rowid_buffer
    'strcat(sql_concat, rowid_buffer);  '' Add rowid.
    sql_concat += sql3
    'strcat(sql_concat, sql3);  '' Finish the statement with ";"

    '' We can only send one query at a time to sqlite3.
    return_code = sqlite3_prepare_v2(p_db, sql_concat, -1, @p_stmt, NULL)
    '' On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    '' is returned.
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code.
        Print #fpErr, "Failed to prepare data: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    '' The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    '' that there is another row ready. Our SQL statement returns only one row
    '' of data at a time, therefore, we call this function only once at a time.
    '' If we are writing or reading multiple lines of table then we will need to
    '' use sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.
    return_code = sqlite3_step( p_stmt )  '' run once for one statement
    ''    while( sqlite3_step( p_stmt ) == SQLITE_ROW ) {;}
    If return_code <> SQLITE_DONE Then  '' SQLITE_DONE==101, SQLITE_ROW==100
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code.
        Print #fpErr, "Step failed: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return 0
    End If

    ''sqlite3_bind_*()  '' After sqlit3_prepare_v2()
    ''sqlite3_clear_bindings(p_stmt);

    '' The sqlite3_finalize function destroys the prepared statement object.
    return_code = sqlite3_finalize( p_stmt )  '' Commit to the database.
    If return_code <> SQLITE_OK Then  '' SQLITE_OK==0
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Failed to finalize data: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    '' The sqlite3_close function closes the database connection.
    return_code = sqlite3_close(p_db)   '' SQLITE_OK==0
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Failed to close database: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    fpErr = Freefile()  '' DEBUG
    Open Err For Input As #fpErr  '' DEBUG
    Print #fpErr, "Successfully removed rowdata from table in "; db_file_name  '' DEBUG
    Close #fpErr  '' DEBUG

    Return 1
End Function



'' update/replace row data by rowid in a named table.
'' If the data already exists it will "overwrite" the row.
Function db_replace_table_rowdata_rowid(Byval db_file_name As String, Byval db_table_name As String, Byval sql_rowid As Integer, Byval db_field_names As String, Byval db_field_values As String) As Integer

    Dim p_db As sqlite3 Ptr  '' database handle (structure).
	Dim p_stmt As sqlite3_stmt Ptr '' structure represents a single SQL statement
    Dim return_code As Integer = 0  '' sqlite3 return codes.
    Dim fpErr As Integer  '' This function error codes.

    return_code = sqlite3_open_v2( db_file_name, @p_db, SQLITE_OPEN_READWRITE, NULL )
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Can't open database: "; *Cast(zString Ptr,sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    Dim As String sql_concat  '' temp buffer [MAX 128 characters]
    Dim As String rowid_buffer  '' convert int to char for statement concat.
    '' Clear the buffer from the last statement. strcat() will concat on to
    '' the previous statement otherwise.
    'strcpy(sql_concat, "");

    rowid_buffer = Str(sql_rowid)
    'sprintf(rowid_buffer, "%d", sql_rowid); '' convert int to str

    '' "Week, Employee_ID, Name, Monday, Tuesday, Wednesday, Thursday, Friday";
    '' "\"2\", \"36\", \"Bill Knight\", \"2\", \"3\", \"4\", \"5\", \"6\"";

    '' Our template for building the query statement.
    Dim As String sql1 = "REPLACE INTO "
    Dim As String sql2 = " (rowid, "
    Dim As String sql3 = ") VALUES("
    Dim As String sql4 = ", "
    Dim As String sql5 = ");"

    '' This can be replaced with anFB string concat in one line.
    
    'strcat(sql_concat, sql1);  '' Add SQL query statement.
    sql_concat += sql1
    'strcat(sql_concat, db_table_name);  '' Add table name to statement.
    sql_concat += db_table_name
    'strcat(sql_concat, sql2);  '' Add " ("
    sql_concat += sql2
    'strcat(sql_concat, db_field_names);  '' Add field name (column name).
    sql_concat += db_field_names
    'strcat(sql_concat, sql3);  '' Add ") VALUES("
    sql_concat += sql3
    'strcat(sql_concat, rowid_buffer);  '' convert rowid int to str char
    sql_concat += rowid_buffer
    'strcat(sql_concat, sql4);
    sql_concat += sql4
    'strcat(sql_concat, db_field_values);  '' Add last part of query statement.
    sql_concat += db_field_values
    'strcat(sql_concat, sql5);  '' Finish the statement with ");"
    sql_concat += sql5

    '' We can only send one query at a time to sqlite3.
    return_code = sqlite3_prepare_v2(p_db, sql_concat, -1, @p_stmt, NULL)
    '' On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    '' is returned.
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code.
        Print #fpErr, "Failed to prepare data: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    '' The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    '' that there is another row ready. Our SQL statement returns only one row
    '' of data at a time, therefore, we call this function only once at a time.
    '' If we are writing or reading multiple lines of table then we will need to
    '' use sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.
    return_code = sqlite3_step( p_stmt )  '' run once for one statement
    /'    while( sqlite3_step( p_stmt ) == SQLITE_ROW ) {;} '/
    If return_code <> SQLITE_DONE Then  '' SQLITE_DONE==101, SQLITE_ROW==100
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code.
        Print #fpErr, "Step failed: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return 0
    End If

    ''sqlite3_bind_*()  '' After sqlit3_prepare_v2()
    ''sqlite3_clear_bindings(p_stmt);

    '' The sqlite3_finalize function destroys the prepared statement object.
    return_code = sqlite3_finalize( p_stmt )  '' Commit to the database.
    If return_code <> SQLITE_OK Then  '' SQLITE_OK==0
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Failed to finalize data: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    '' The sqlite3_close function closes the database connection.
    return_code = sqlite3_close(p_db)   '' SQLITE_OK==0
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Failed to close database: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    fpErr = Freefile()  '' DEBUG
    Open Err For Input As #fpErr  '' DEBUG
    Print #fpErr, "Successfully inserted rowdata into table in "; db_file_name  '' DEBUG
    Close #fpErr  '' DEBUG

    Return 1
End Function


'' Insert row data into a named table at rowid. (Not recommended)
'' I had some issues with non contiguous rowid numbers. I have created a test
'' flag to skip empty rowid. Empty row id remain unchanged and all other
'' filled rows are moved down one slot. This is a bit hackish and not the
'' best method. We could use VACUUM to make the rowid index contiguous before
'' each routine requiring rowid manipulation, or we can copy the table to
'' a memory file with contiguous rowid, or last copy the enter table to our
'' application memory and perform the table tasks there before re-writting
'' the table fresh.
''
'' The current data at the rowid is written into a temporary buffer, the new row
'' data is then written to that rowid.
'' Each row is "shuffled" down one rowid at at time until the last rowid.
'' The final row of data is then inserted into a new (empty) rowid at the end
'' of the table.
''
'' Note this is considered poor practice in data base management as data is
'' generally not stored in any particular order and it is up to the calling
'' application to sort the data according to rules such as "By date" or "By Name".
''
'' In a large data base file this will be a slow task and could be prone to errors.
'' It can also disrupt the association of the rowid between different tables
'' in a relational database.
''
'' Table unique index rowid is auto generated in this table.
''
'' NOTE: Check if this is OK with non contiguous rowid numbers?
Function db_insert_table_rowdata_rowid(Byval db_file_name As String, Byval db_table_name As String, Byval sql_rowid As Integer, Byval db_field_names As String, Byval db_field_values As String, Byval number_columns As Integer,Byval number_rows As Integer) As Integer

    Dim p_db As sqlite3 Ptr  '' database handle (structure).
	Dim p_stmt As sqlite3_stmt Ptr '' structure represents a single SQL statement
    Dim return_code As Integer = 0  '' sqlite3 return codes.
    Dim fpErr As Integer  '' This function error codes.

    '' Test and skip missing rowid between read buffer and write buffer.
    '' It's a bit messy and complicated :/
    Dim As Integer step_ret = 0  '' Used for flags in missing rowid.
    Dim As Integer Count_Rows = sql_rowid  '' start counting from the first insert rowid.
    Dim As Integer test_rowid = 0  '' Used for flags in missing rowid.
    Dim As Integer W_Flag = 0  ''  Used for flags in missing rowid.
    Dim As Integer i = 0  '' Loop counter.

    Dim As String buffer  '' Temp column read buffer [MAX 128 characters]
    '' This 2 dimension array will hold the temporary data for the rows being read
    '' and then writen to the next line.
    Dim As String db_field_values_temp(2)  '' buffer to hold each line to move down +1

    '' This is the new data (row) to be inserted at the rowid.
    db_field_values_temp(0) = db_field_values  '' Copy first (new row) to R/W buffer.

    Dim As String sql_concat  '' temp buffer [MAX 512 characters]
    Dim As String rowid_buffer  '' convert int to char for statement concat.

    return_code = sqlite3_open_v2( db_file_name, @p_db, SQLITE_OPEN_READWRITE, NULL )
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Can't open database: "; *Cast(zString Ptr,sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    '' Starts at rowid up to last row + 1. +2 alows for the '<' ie. +1
    While Count_Rows < number_rows +2

''===================================================== Start step 1 read rowid
        '' rowid copy to string
        rowid_buffer = Str(sql_rowid)
        'sprintf(rowid_buffer, "%d", sql_rowid); '' convert int to str

        '' The +1 at the end of the table is empty and will create a read error,
        '' so we will skip this and go directly to writing the last +1 new line
        '' directly from the buffer.
        If Count_Rows < number_rows +1 Then
            '' strcat() must be proceeded by a strcpy, We must also clear the
            ''buffer with an empty string with "" for each read iteration.
            'strcpy(sql_concat, "");
            sql_concat = ""
            '' "SELECT rowid, * FROM "  '' Note that I am ignoring the rowid
            Dim As String sql1 = "SELECT rowid, * FROM "  '' Note the space after FROM
            Dim As String sql2 = " WHERE rowid =  "
            Dim As String sql3 = ";"

            sql_concat += sql1  '' Add SQL query statement.
            sql_concat += db_table_name  '' Add table name to statement.
            sql_concat += sql2  '' Add filter.
            sql_concat += rowid_buffer  '' This is the first replace rowid.
            sql_concat += sql3  '' Finish the sql statement

            '' We can only send one query at a time to sqlite3.
            return_code = sqlite3_prepare_v2(p_db, sql_concat, -1, @p_stmt, NULL)
            '' On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
            '' is returned.
            If return_code <> SQLITE_OK Then
                fpErr = Freefile()  '' DEBUG
                Open Err For Input As #fpErr  '' DEBUG
                '' This is error handling code.
                Print #fpErr, "X1 Failed to prepare data: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
                Close #fpErr  '' DEBUG
                sqlite3_close(p_db)
                Return -1
            End If

            db_field_values_temp(1) = ""  '' clear the row buffer for next read strcat()
            '' All of the data is collected in a single string with each column
            '' separated with the ',' delimiter.
            
            '' This while isn't required as step only retreives a single row.
            ''while sqlite3_step(p_stmt) = SQLITE_ROW
            step_ret = sqlite3_step(p_stmt)

            test_rowid = sqlite3_column_int(p_stmt, 0)
            If (test_rowid <> 0) Or (step_ret = SQLITE_ROW) Then

                W_Flag = 0  '' Reset the write flag if rowid has an entry.

                '' Can use sqlite3_column_count(statement/p_stmt) in place of number_columns.
                '' Changed to i = 1 to number_columns +1. The first rowid column is skipped.
                For i = 1 To number_columns +1 -1 Step +1  '' Count 1 to number_columns

                    '' To handle the return of NULL pointers as a string
                    '' data = (const char*)sqlite3_column_text( p_stmt, i );
                    ''        printf( "%s\n", data ? data : "[NULL]" );
                    ''        sprintf(buffer[strlen(data)], "%s\n", data ? data : "[NULL]" );

                    '' Copy each entry to a buffer. Each entry is a col.
                    buffer = *Cast(zString Ptr, sqlite3_column_text(p_stmt, i))  '' i == array column.

                    '' concat buffer to our return array.
                    '' Here we are reading the row into [1] of the temporary R/W buffer.
                    db_field_values_temp(1) += "'"
                    db_field_values_temp(1) += buffer
                    db_field_values_temp(1) += "'"
                    If i < number_columns +1 -1 Then '' Don't add ',' after last column.
                        'strcat(db_field_values_temp[1], ",");  '' add separator token between each col.
                        db_field_values_temp(1) += ","
                    End If
                Next i
            Else
                '' Skip read data on empty rowid.
                W_Flag += 1  '' Flag to write last line. Can use test_rowid instead.
                number_rows += 1  '' Correct the total row count to match last system rowid
            End If

            ''sqlite3_bind_*()  '' After sqlit3_prepare_v2()
            ''sqlite3_clear_bindings(p_stmt);

            '' The sqlite3_finalize function destroys the prepared statement object.
            return_code = sqlite3_finalize( p_stmt )  '' Commit to the database.
            If return_code <> SQLITE_OK Then  '' SQLITE_OK==0
                fpErr = Freefile()  '' DEBUG
                Open Err For Input As #fpErr  '' DEBUG
                '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
                Print #fpErr, "Failed to finalize data: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
                Close #fpErr  '' DEBUG
                sqlite3_close(p_db)
                Return -1
            End If

        End If  '' Count_Rows, END Skip reading +1 empty line
''=============================================================== end step 1
''==================================================== Start step 2. Write rowid

        If W_Flag < 1 Then  '' Skip writing empty buffer data.

        '' Clear the buffer from the last statement. strcat() will concat on to
        '' the previous statement otherwise.
        sql_concat = ""

        Dim As String sql01a = "REPLACE INTO "  ''
        Dim As String sql01b = "INSERT INTO "  ''
        Dim As String sql02a = " (rowid, "  ''
        Dim As String sql02b = " ("  ''
        Dim As String sql03 = ") VALUES("  ''
        Dim As String sql04 = ","  '' SQL as integer
        Dim As String sql05 = ");"

        '' Some logic to handle the last row inserted to a new rowid.
        If Count_Rows < number_rows +2 Then
            sql_concat += sql01a  '' "REPLACE INTO "
        Else  '' New row (original last row +1)
            sql_concat += sql01b  '' "INSERT INTO "
        End If

        sql_concat += db_table_name  '' Add table name to statement.

        If Count_Rows < number_rows +2 Then
            sql_concat += sql02a  '' " (rowid, "
        Else
            sql_concat += sql02b  '' " ("
        End If

        sql_concat += db_field_names  '' Add feild name (column name).

        If Count_Rows < number_rows +2 Then
            sql_concat += sql03  '' ") VALUES(\""
            sql_concat += rowid_buffer  '' This is the fist rowid for new values [As SQL INTEGER]
            sql_concat += sql04  '' delimit rowid, col_values, ...
        Else
            sql_concat += sql03  '' ") VALUES("
        End If

        '' Add the last row read into the same rowid
        sql_concat += db_field_values_temp(0)  '' Add the values.
        sql_concat += sql05  '' Finish the statement with ");"

        '' We can only send one query at a time to sqlite3.
        return_code = sqlite3_prepare_v2(p_db, sql_concat, -1, @p_stmt, NULL)
        '' On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
        '' is returned.
        If return_code <> SQLITE_OK Then
            fpErr = Freefile()  '' DEBUG
            Open Err For Input As #fpErr  '' DEBUG
            '' This is error handling code.
            Print #fpErr, "X2 Failed to prepare data: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
            Close #fpErr  '' DEBUG
            sqlite3_close(p_db)
            Return -1
        End If

        '' The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
        '' that there is another row ready. Our SQL statement returns only one row
        '' of data at a time, therefore, we call this function only once at a time.
        '' If we are writing or reading multiple lines of table then we will need to
        '' use sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.
        return_code = sqlite3_step( p_stmt )  '' run once for one statement
        ''    while( sqlite3_step( p_stmt ) == SQLITE_ROW ) {;}
        If return_code <> SQLITE_DONE Then  '' SQLITE_DONE==101, SQLITE_ROW==100
            fpErr = Freefile()  '' DEBUG
            Open Err For Input As #fpErr  '' DEBUG
            '' This is error handling code.
            Print #fpErr, "Step failed: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
            Close #fpErr  '' DEBUG
            sqlite3_close(p_db)
            Return 0
        End If

        ''sqlite3_bind_*()  '' After sqlit3_prepare_v2()
        ''sqlite3_clear_bindings(p_stmt);


        '' The sqlite3_finalize function destroys the prepared statement object.
        return_code = sqlite3_finalize( p_stmt )  '' Commit to the database.
        If return_code <> SQLITE_OK Then  '' SQLITE_OK==0
            fpErr = Freefile()  '' DEBUG
            Open Err For Input As #fpErr  '' DEBUG
            '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
            Print #fpErr, "Failed to finalize data: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
            Close #fpErr  '' DEBUG
            sqlite3_close(p_db)
            Return -1
        End If

        '' Copy the last read [1] back to position [0] for next read write cycle.
        '' This works a little like a last in first out buffer (LIFO).
        db_field_values_temp(0) = db_field_values_temp(1)
        
        End If  '' W_Flag

        sql_rowid += 1  '' Start read/write next rowid..
        Count_Rows += 1  '' So we don't count past the existing rows in the table.

    Wend  '' Count_Rows, Continue loop until last table row.
''================================================================== End step 2

    '' The sqlite3_close function closes the database connection.
    return_code = sqlite3_close(p_db)   '' SQLITE_OK==0
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Failed to close database: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    fpErr = Freefile()  '' DEBUG
    Open Err For Input As #fpErr  '' DEBUG
    Print #fpErr, "Successfully inserted rowdata into table in "; db_file_name  '' DEBUG
    Close #fpErr  '' DEBUG

    Return 1
End Function



'' Read row from rowid in named table.
Function db_read_table_rowdata_rowid(Byval db_file_name As String, Byval db_table_name As String, Byval sql_rowid As Integer, Byref db_tbl_rowid_data As String, Byval number_columns As Integer) As Integer

    Dim p_db As sqlite3 Ptr  '' database handle (structure).
	Dim p_stmt As sqlite3_stmt Ptr '' structure represents a single SQL statement
    Dim return_code As Integer = 0  '' sqlite3 return codes.
    Dim fpErr As Integer  '' This function error codes.

    Dim As String sql_concat  '' temp buffer [MAX 512 characters]
    Dim As String buffer  '' Temp column read buffer [MAX 128 characters]
    Dim As String rowid_buffer  '' convert int to char for statement concat.

    return_code = sqlite3_open_v2( db_file_name, @p_db, SQLITE_OPEN_READONLY, NULL )
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Can't open database: "; *Cast(zString Ptr,sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    '' rowid copy to string
    rowid_buffer = Str(sql_rowid)

    '' strcat() must be proceeded by a strcpy, We must also clear the
    ''buffer with an empty string with "" for each read iteration.
    sql_concat = ""
    ''char *sql1 = "SELECT rowid, * FROM ";  '' Note the space after FROM
    Dim As String sql1 = "SELECT * FROM "  '' Note the space after FROM
    Dim As String sql2 = " WHERE rowid =  "  ''
    Dim As String sql3 = ";"  ''

    'strcat(sql_concat, sql1);  '' Add SQL query statement.
    sql_concat += sql1
    'strcat(sql_concat, db_table_name);  '' Add table name to statement.
    sql_concat += db_table_name
    'strcat(sql_concat, sql2);  '' Add filter.
    sql_concat += sql2
    'strcat(sql_concat, rowid_buffer);  '' rowid to read from.
    sql_concat += rowid_buffer
    'strcat(sql_concat, sql3);  '' Finish the sql statement
    sql_concat += sql3

    '' We can only send one query at a time to sqlite3.
    return_code = sqlite3_prepare_v2(p_db, sql_concat, -1, @p_stmt, NULL)
    '' On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    '' is returned.
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code.
        Print #fpErr, "Failed to prepare data: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If


    While sqlite3_step(p_stmt) = SQLITE_ROW
        ''printf("DEBUG\n");  '' DEBUG
        '' Can use sqlite3_column_count(statement/p_stmt) in place of number_columns.
        Dim As Integer i = 0
        For i = 0 To number_columns -1 Step +1  '' Count 0 to number_columns-1
            '' To handle the return of NULL pointers as a string
            '' data = (const char*)sqlite3_column_text( p_stmt, i );
            ''        printf( "%s\n", data ? data : "[NULL]" );
            ''        sprintf(buffer[strlen(data)], "%s\n", data ? data : "[NULL]" );

            '' copy each entry to a buffer. Each entry is a col.
            buffer = *Cast(zString Ptr, sqlite3_column_text(p_stmt, i))  '' i == array column.
            ''printf("DEBUG %s\n", buffer);
            '' concat buffer to our return array.
            db_tbl_rowid_data += buffer

            If i < number_columns -1 Then  '' Don't add ', ' after last column.
                '' Remove space after ','
                db_tbl_rowid_data += ","  '' add separator token between each col.
            Else  '' Add line return, end of row.
                ''strcat(db_tbl_rowdata[cnt_row], "\n");
            End If
        Next i
    Wend

    ''sqlite3_bind_*()  '' After sqlit3_prepare_v2()
    ''sqlite3_clear_bindings(p_stmt);

    '' The sqlite3_finalize function destroys the prepared statement object.
    return_code = sqlite3_finalize( p_stmt )  '' Commit to the database.
    If return_code <> SQLITE_OK Then  '' SQLITE_OK==0
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Failed to finalize data: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    '' The sqlite3_close function closes the database connection.
    return_code = sqlite3_close(p_db)   '' SQLITE_OK==0
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Failed to close database: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    fpErr = Freefile()  '' DEBUG
    Open Err For Input As #fpErr  '' DEBUG
    Print #fpErr, "Successfully read rowdata from table in "; db_file_name  '' DEBUG
    Close #fpErr  '' DEBUG

    Return 1

End Function


'' Search for a string in a field name and return array of found rows.
'' As this only searches a single column we should not encounter duplicate rowid.
'' ## Only for tables where all field types are TEXT (String) except rowid. ##
Function db_search_table_rowdata_byfield(Byval db_file_name As String, Byval db_table_name As String, db_tbl_row_search() As String, Byval field_name As String, Byval db_search_string As String, Byval number_columns As Integer, Byref ret_array_length As Integer) As Integer

    '' Get column field names as array[][].
    '' get number of columns.
    '' return row length (number of array items).

    number_columns += 1  '' We are also including an extra column for the row ID number.
    Dim p_db As sqlite3 Ptr  '' database handle (structure).
	Dim p_stmt As sqlite3_stmt Ptr '' structure represents a single SQL statement
    Dim return_code As Integer = 0  '' sqlite3 return codes.
    Dim fpErr As Integer  '' This function error codes.

    return_code = sqlite3_open_v2( db_file_name, @p_db, SQLITE_OPEN_READONLY, NULL )
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Can't open database: "; *Cast(zString Ptr,sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    Dim As String sql_concat
    sql_concat = ""  '' Set the buffer with strcpy() before strcat().

    '' This can be replaced with sprintf()
    '' It is important that the field name and search value are enclosed
    '' within !"\"Value\"" and not "'Value'"!
    Dim As String sql1 = "SELECT rowid, * FROM "  '' Note the space after FROM
    Dim As String sql2 = " WHERE "  '' Note the space before and after WHERE
    Dim As String sql3 = " = "  '' Note the space before and after =
    Dim As String sql4 = ";"

    sql_concat += sql1
    sql_concat += db_table_name
    sql_concat += sql2
    sql_concat += field_name
    sql_concat += sql3
    sql_concat += db_search_string
    sql_concat += sql4

    '' We can only send one query at a time to sqlite3.
    return_code = sqlite3_prepare_v2(p_db, sql_concat, -1, @p_stmt, NULL)
    '' On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    '' is returned.
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code.
        Print #fpErr, "Failed to prepare data: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    '' The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    '' that there is another row ready. Our SQL statement returns only one row
    '' of data at a time, therefore, we call this function only once at a time.
    '' If we are writing or reading multiple lines of table then we will need to
    '' use sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.

    Dim As Integer cnt_row = 0
    ''int cnt_col = 0;
    Dim As String buffer  '' temp buffer [MAX 128 characters]

    While sqlite3_step(p_stmt) = SQLITE_ROW

        '' Can use sqlite3_column_count(statement/p_stmt) in place of number_columns.
        Dim As Integer i = 0
        For i = 0 To number_columns -1 Step +1  '' Count 0 to number_columns-1
            '' To handle the return of NULL pointers as a string
            '' data = (const char*)sqlite3_column_text( p_stmt, i );
            ''        printf( "%s\n", data ? data : "[NULL]" );
            ''        sprintf(buffer[strlen(data)], "%s\n", data ? data : "[NULL]" );

            '' copy each entry to a buffer. Each entry is a col.
            buffer = *Cast(zString Ptr, sqlite3_column_text(p_stmt, i))  '' i == array column.

            '' concat buffer to our return array.
            db_tbl_row_search(cnt_row) += buffer
            If i < number_columns -1 Then '' Dont add ', ' after last column.
                db_tbl_row_search(cnt_row) += ","  '' add separator token between each col.
            Else  '' Add line return, end of row.
                ''strcat(db_tbl_row_search[cnt_row], "\n");
            End If
            ''cnt_col++;  '' Need 3D array.
        Next i
        cnt_row += 1
    Wend

    ret_array_length = cnt_row  '' return the length of array of found rows.

    ''sqlite3_bind_*()  '' After sqlit3_prepare_v2()
    ''sqlite3_clear_bindings(p_stmt);

    '' The sqlite3_finalize function destroys the prepared statement object.
    return_code = sqlite3_finalize( p_stmt )  '' Commit to the database.
    If return_code <> SQLITE_OK Then  '' SQLITE_OK==0
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Failed to finalize data: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    '' The sqlite3_close function closes the database connection.
    return_code = sqlite3_close(p_db)   '' SQLITE_OK==0
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Failed to close database: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    fpErr = Freefile()  '' DEBUG
    Open Err For Input As #fpErr  '' DEBUG
    Print #fpErr, "Successfully retrieved table search data from "; db_file_name  '' DEBUG
    Close #fpErr  '' DEBUG

    Return 1
End Function


'' Search for string in all field names of the table and return array of found rows.
'' Duplicate row_id are filtered out.
'' This needs to be stepped through each column and filter for duplicate rowid.
'' ## Only for tables where all field types are TEXT (String) except rowid. ##
Function db_search_table_rowdata_allfields(Byval db_file_name  As String, Byval db_table_name  As String, db_tbl_row_search()  As String, db_tbl_col_name()  As String, Byval db_search_string As String, Byval number_columns As Integer, Byval number_rows As Integer, Byref ret_array_length As Integer) As Integer

    '' Be careful with this when looping fields!!! <- revise!
    '' We are also including an extra column for the row ID number.
    Dim As Integer number_columns2 = number_columns +1

    Dim p_db As sqlite3 Ptr  '' database handle (structure).
	Dim p_stmt As sqlite3_stmt Ptr '' structure represents a single SQL statement
    Dim return_code As Integer = 0  '' sqlite3 return codes.
    Dim fpErr As Integer  '' This function error codes.

    Dim As Integer cnt_col = 0  '' Counter to step though name of each column (field)
    Dim As Integer i = 0 '' Loop counters.
    Dim As Integer j = 0 '' Loop counters.

    Dim As Integer cnt_row = 0  '' Used to count through each row of found data to test for duplicate rows.
    Dim As String buffer  '' temp buffer [MAX 128 characters]
    Dim As Integer ch  '' character buffer. Alt ch As String Asc(), Chr()
    Dim As Integer x = 0  '' while loop to retrieve row_id up to first ','
    '' Flag to skip writing duplicate row to return array. Also skips ret array increment.
    Dim As Integer row_id_exists = 0
    Dim As String sql_concat  '' Build sql query statement.

    return_code = sqlite3_open_v2( db_file_name, @p_db, SQLITE_OPEN_READONLY, NULL )
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Can't open database: "; *Cast(zString Ptr,sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    '' I need to loop over the columns for full search...
    '' WHERE "field/column_name" = will need to be replaced in each loop.
    '' This search routine will only work for tables with type TEXT!

    '' Be careful of the + 1 in number_columns2 due to extra row_id column.
    '' rowid column is not counted when accessing the field names in the search.
    '' The return values from the search DO include the rowid.
    '' ====> START Loop each column name (field) for the search term.
    For cnt_col = 0 To number_columns2 -2 Step +1

        '' Clear the buffer from the last statement. strcat() will concat on to
        '' the previous statement otherwise.
        sql_concat = ""

        '' This can be replaced with sprintf()
        '' The \"name\" may not be required? !!!
        Dim As String sql1 = "SELECT rowid, * FROM "  '' Note the space after FROM
        Dim As String sql2 = !" WHERE \""  '' Note the space before and after WHERE
        Dim As String sql3 = !"\" = "  '' Note the space before and after =
        Dim As String sql4 = ";"

        'strcat(sql_concat, sql1);  '' Add SQL query statement.
        sql_concat += sql1
        'strcat(sql_concat, db_table_name);  '' Add table name to statement.
        sql_concat += db_table_name
        'strcat(sql_concat, sql2);  '' Add last part of query statement.
        sql_concat += sql2
        'strcat(sql_concat, db_tbl_col_name[cnt_col]);  '' Add field name (column name) from db_tbl_col_name[][] array.
        sql_concat += db_tbl_col_name(cnt_col)
        'strcat(sql_concat, sql3);  '' add " = "
        sql_concat += sql3
        'strcat(sql_concat, db_search_string);  '' Add last part of query statement.
        sql_concat += db_search_string
        'strcat(sql_concat, sql4);  '' Finish the statement with ";"
        sql_concat += sql4

        '' We can only send one query at a time to sqlite3.
        return_code = sqlite3_prepare_v2(p_db, sql_concat, -1, @p_stmt, NULL)
        '' On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
        '' is returned.
        If return_code <> SQLITE_OK Then
            fpErr = Freefile()  '' DEBUG
            Open Err For Input As #fpErr  '' DEBUG
            '' This is error handling code.
            Print #fpErr, "Failed to prepare data: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
            Close #fpErr  '' DEBUG
            sqlite3_close(p_db)
            Return -1
        End If

        '' The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
        '' that there is another row ready. Our SQL statement returns only one row
        '' of data at a time, therefore, we call this function only once at a time.
        '' If we are writing or reading multiple lines of table then we will need to
        '' use sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.
        While sqlite3_step(p_stmt) = SQLITE_ROW

            '' Can use sqlite3_column_count(statement/p_stmt) in place of number_columns.
            For i = 0 To number_columns2 -1 Step +1  '' Count 0 to number_columns-1
                '' To handle the return of NULL pointers as a string
                '' data = (const char*)sqlite3_column_text( p_stmt, i );
                ''        printf( "%s\n", data ? data : "[NULL]" );
                ''        sprintf(buffer[strlen(data)], "%s\n", data ? data : "[NULL]" );

                '' copy each entry to a buffer. Each entry is a col.
                buffer = *Cast(zString Ptr, sqlite3_column_text(p_stmt, i))  '' i == array column.

                ''====> START Test for repeat row_id ====>
                '' This can be improved in efficiency !!!!
                '' Do string compare test to see if the first column row_id
                '' already exists in our return search array. Skip copying that
                '' row if row_id already exists.
                '' This does not order the array entries by row_id and are entered
                '' in the order they are found during the search.
                If i = 0 Then
                    
                    For j = 0 To cnt_row Step +1  '' Search previous entries for duplicate row_id
                        x = 0  '' reset the character counter.
                        'ch = 0  '' Reset ch. May not be required anymore.
                        Dim As String token_buf  '' reset/clear the token buffer
                        '' extract the first col item row_id up to token ','
                        While 1
                            '' If the array element has NULL data an error will occur,
                            '' So we can test for en empty string and replace the
                            '' token with a ',' character. It has no rowid value
                            '' so will be used as the next row of data if no matching
                            '' rowid is found.
                            If len(db_tbl_row_search(j)) = 0 Then
                                Mid(token_buf,0) = ","
                                Exit While
                            else
                                ch = db_tbl_row_search(j)[x]  '' Get our rowid from previous finds.
                                If ch <> 44 Then  '' , 44 alt: ch As String ch Asc(), Chr(), ','
                                    Mid(token_buf,x) = Chr(ch)  '' copy int characters to the token buffer.
                                    x += 1
                                Else
                                '' Found ',' so loop the rest of the column data
                                '' without testing again until next row/column name if (i==0).
                                Exit While
                                End If
                            End If
                        Wend  '' (1)

                        '' Compare if rowid is already found in our return search array.
                        '' If rowid is already present, skip copying that row.
                        If buffer = token_buf Then
                            '' If True then skip copying this row to db_tbl_row_search().
                            row_id_exists = 1  '' set the copy, no copy flag.
                        End If
                    Next j
                End If  '' i
                ''====> END Test for repeat row_id ====>

                If row_id_exists = 0 Then  '' if row not exist already, write search found to return buffer.
                    '' Concat buffer to our return array. This will copy each column of the row
                    '' separated by the ', ' comma-space character. aka CSV format.
                    '' No return character '\n' is created as we use a separate array
                    '' element for each row. To print (write to file) as CSV add
                    '' the '\n' after each array element. See the loop that prints
                    '' this in main().
                    db_tbl_row_search(cnt_row) += buffer  '' Concat each column
                    If i < number_columns2 -1 Then '' Don't add ', ' after last column.
                        db_tbl_row_search(cnt_row) += ","  '' add separator token between each col.
                    End If
                End If  '' row_id_exists
            Next i

            If row_id_exists = 0 Then
                '' We wrote this row to the return array, so increment return row +1
                cnt_row += 1
            End If
            '' else Skip this row count for return array. row_id_exists == 1.
            row_id_exists = 0  '' Reset for next search column name.
        Wend  '' sqlite3_step

        ''sqlite3_bind_*()  '' After sqlit3_prepare_v2()
        ''sqlite3_clear_bindings(p_stmt);

        '' The sqlite3_finalize function destroys the prepared statement object.
        return_code = sqlite3_finalize( p_stmt )  '' Commit to the database.
        If return_code <> SQLITE_OK Then  '' SQLITE_OK==0
            fpErr = Freefile()  '' DEBUG
            Open Err For Input As #fpErr  '' DEBUG
            '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
            Print #fpErr, "Failed to finalize data: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
            Close #fpErr  '' DEBUG
            sqlite3_close(p_db)
            Return -1
        End If

    Next cnt_col  '' END loop/walk each column (field) Name search term.

    ret_array_length = cnt_row  '' return the length of array of found rows.

    '' The sqlite3_close function closes the database connection.
    return_code = sqlite3_close(p_db)   '' SQLITE_OK==0
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Failed to close database: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    fpErr = Freefile()  '' DEBUG
    Open Err For Input As #fpErr  '' DEBUG
    Print #fpErr, "Successfully retrieved table search data from "; db_file_name  '' DEBUG
    Close #fpErr  '' DEBUG

    Return 1
End Function

''==============================================================================
'' START Multiple types examples.


'' Insert binary row data into a named table.
'' If the data already exists will create a new row.
'' Table unique index ID INT is auto generated in this table.
Function db_insert_table_rowdata_bin(Byval db_file_name As String, Byval db_tbl_entry As String, bin_data() As Ubyte, Byval bin_data_len As Integer) As Integer

    Dim p_db As sqlite3 Ptr  '' database handle (structure).
	Dim p_stmt As sqlite3_stmt Ptr '' structure represents a single SQL statement
    Dim return_code As Integer = 0  '' sqlite3 return codes.
    Dim fpErr As Integer  '' This function error codes.

    return_code = sqlite3_open_v2( db_file_name, @p_db, SQLITE_OPEN_READWRITE, NULL )
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Can't open database: "; *Cast(zString Ptr,sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    '' We can only send one query at a time to sqlite3.
    return_code = sqlite3_prepare_v2(p_db, db_tbl_entry, -1, @p_stmt, NULL)
    '' On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    '' is returned.
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code.
        Print #fpErr, "Failed to prepare data: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    '' declare function sqlite3_bind_blob(byval as sqlite3_stmt ptr, byval as long, byval as const any ptr, byval n as long, byval as sub(byval as any ptr)) as long
    '' Alternative method.
    'Dim myPointer As Any Ptr
    'myPointer = @bin_data(0)
    'return_code = sqlite3_bind_blob(p_stmt, 1, myPointer, bin_data_len, SQLITE_STATIC)  '' SQLITE_TRANSIENT

    return_code = sqlite3_bind_blob(p_stmt, 1, @bin_data(0), bin_data_len, SQLITE_STATIC)  '' SQLITE_TRANSIENT
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code.
        Print #fpErr, "Failed to bind data: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    '' The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    '' that there is another row ready. Our SQL statement returns only one row
    '' of data at a time, therefore, we call this function only once at a time.
    '' If we are writing or reading multiple lines of table then we will need to
    '' use sqlite3_step() in a loop until end of SQLITE_ROW or SQLITE_DONE.
    return_code = sqlite3_step( p_stmt )  '' run once for one statement
    /'    while( sqlite3_step( p_stmt ) == SQLITE_ROW ) {;} '/
    If return_code <> SQLITE_DONE Then  '' SQLITE_DONE==101, SQLITE_ROW==100
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code.
        Print #fpErr, "Step failed: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return 0
    End If

    ''sqlite3_bind_*()  '' After sqlit3_prepare_v2()
    ''sqlite3_clear_bindings(p_stmt);

    '' The sqlite3_finalize function destroys the prepared statement object.
    return_code = sqlite3_finalize( p_stmt )  '' Commit to the database.
    If return_code <> SQLITE_OK Then  '' SQLITE_OK==0
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Failed to finalize data: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    '' The sqlite3_close function closes the database connection.
    return_code = sqlite3_close(p_db)   '' SQLITE_OK==0
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Failed to close database: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    fpErr = Freefile()  '' DEBUG
    Open Err For Input As #fpErr  '' DEBUG
    Print #fpErr, "Successfully inserted rowdata into table in "; db_file_name  '' DEBUG
    Close #fpErr  '' DEBUG

    Return 1
End Function


'' NOTE: SQLite3 does have it's own internal typeless data structure Mem.
'' typedef struct Mem Mem;
'' It is an extremely complex data structure that includes many other data
'' structures defined in the sqlite source. Also it is predominantly used
'' with the sqlite3_value/_* set of API functions.
'' typedef struct sqlite3_value sqlite3_value;
'' It is more convenient to create our own tag struct, union or linked list
'' for the following example.

'' List all rows from mixed data types.
'' Types:NULL,INTEGER,REAL,TEXT,BLOB
'' sqlite3_column_type() returned values:
'' SQLITE_NULL, SQLITE_INTEGER, SQLITE_FLOAT, SQLITE_TEXT, SQLITE_BLOB
'' https:''www.sqlite.org/c3ref/column_blob.html
'' data = (const char*)sqlite3_column_text( p_stmt, 0 );
''        printf( "%s\n", data ? data : "[NULL]" );
'' Note that number_elements should generally return number_rows.
Function db_list_table_all_types(Byval db_file_name As String, Byval db_table_name As String, variant_structure() As tagVARIANT, Byval number_columns As Integer, Byval number_rows As Integer, Byref ret_number_fields As Integer, Byref ret_number_elements As Integer) As Integer
    
    number_columns = number_columns  '' not used at this time
    number_rows = number_rows  '' not used at this time

    Dim p_db As sqlite3 Ptr  '' database handle (structure).
	Dim p_stmt As sqlite3_stmt Ptr '' structure represents a single SQL statement
    Dim return_code As Integer = 0  '' sqlite3 return codes.
    Dim fpErr As Integer  '' This function error codes.

    Dim As String sql_concat  '' Build sql query statement.
    Dim As Integer i = 0
    Dim As Integer x = 0
    Dim As Integer num_cols = 0
    Dim As Integer max_cols = 0
    Dim As Integer num_rows = 0
    Dim As Integer bytes_blob = 0

    '' Need a temporary buffer to transfer BLOB data from SQLite internal.
    '' This is used later in the function with sqlite3_column_blob().
    Dim bin_buffer As ZString Ptr  '' Must be defined at start of function.
    '' Allocate minimum memory. Will be reallocate using sqlite BLOB size.
    bin_buffer = Allocate(1 * Sizeof(ZString))
    Dim p_bb As ZString Ptr  '' Temp ptr for reallocate.

    return_code = sqlite3_open_v2( db_file_name, @p_db, SQLITE_OPEN_READONLY, NULL )
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Can't open database: "; *Cast(zString Ptr,sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    sql_concat = ""

    '' This can be replaced with sprintf()
    '' "select * from TableName"
    Dim As String sql1 = "SELECT rowid, * FROM "  '' Note the space after FROM
    Dim As String sql2 = ";"

    'strcat(sql_concat, sql1);  '' Add SQL query statement.
    sql_concat += sql1
    'strcat(sql_concat, db_table_name);  '' Add table name to statement.
    sql_concat += db_table_name
    'strcat(sql_concat, sql2);  '' Finish the statement with ";"
    sql_concat += sql2

    '' We can only send one query at a time to sqlite3.
    return_code = sqlite3_prepare_v2(p_db, sql_concat, -1, @p_stmt, NULL)
    '' On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    '' is returned.
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code.
        Print #fpErr, "Failed to prepare data: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    While sqlite3_step(p_stmt) <> SQLITE_DONE  '' or == SQLITE_OK
        num_cols = sqlite3_column_count(p_stmt)
        For i = 0 To num_cols -1 Step +1
            Select Case sqlite3_column_type(p_stmt, i)  '' (p_stmt, cidx (col index)

                '' Note: sqlite3_column_type()	?	Default datatype of the result
                Case SQLITE_NULL
                    '' Do stuff for NULL pointer, using variant_array[n].value.vval
                    '' This will denote an unused array element. It is possible to
                    '' use this data structure were we test for null as an empty element
                    '' or as and empty type
                    variant_structure(num_rows,i).sql_type = IS_NULL
                    sqlite3_column_text(p_stmt, i)  '' NULL to read column
                    variant_structure(num_rows,i).value.vval = NULL
                    Exit Select
                Case SQLITE_INTEGER
                    variant_structure(num_rows,i).sql_type = IS_INTEGER
                    variant_structure(num_rows,i).value.ival = sqlite3_column_int(p_stmt, i)
                    Exit Select
                Case SQLITE_FLOAT  '' REAL
                    variant_structure(num_rows,i).sql_type = IS_FLOAT
                    variant_structure(num_rows,i).value.rval = sqlite3_column_double(p_stmt, i)
                    Exit Select
                Case SQLITE_TEXT
                    variant_structure(num_rows,i).sql_type = IS_TEXT
                    variant_structure(num_rows,i).value.tval = *Cast(zString Ptr, sqlite3_column_text(p_stmt, i))
                    Exit Select
                Case SQLITE_BLOB
                    variant_structure(num_rows,i).sql_type = IS_BLOB
                    '' MAX 30720 bytes
                    bytes_blob = sqlite3_column_bytes(p_stmt, i)
                    variant_structure(num_rows,i).value.bval.blen = bytes_blob

                    '' There are a number of different data types that can store
                    '' BYTE data. Each has it's pros and cons as well as different
                    '' methods of use. I am using a ZString as it is more convenient
                    '' to to copy the SQLite internal BLOB buffer to a temporary
                    '' buffer without using arrays.
                    '' I then copp each array BYTE from the temp ZString buffer
                    '' to the UByte array in the structure using a loop.

                    '' Retrieve bin data as static ZString.
                    '' Max size of structure data array (variant_structure) is 30719 bytes
                    'Dim bin_buffer As ZString * 30719  '' This works as static size.
                    'bin_buffer = *Cast(ZString Ptr, sqlite3_column_blob(p_stmt, i))
                    'memcpy(dest as any ptr, src as any ptr, n as size_t) as any ptr
                    'memcpy(@bin_buffer, Cast(ZString Ptr, sqlite3_column_blob(p_stmt, i)), bytes_blob)

                    '' Retrieve bin data as Dynamic ZString array to size of bytes_blob.
                    'Dim bin_buffer As ZString Ptr  '' Must be defined at start of function.
                    'bin_buffer = allocate(bytes_blob * SizeOf(ZString) +1)
                    '*bin_buffer = *Cast(ZString Ptr, sqlite3_column_blob(p_stmt, i))

                    '' Retrieve bin data as Dynamic ZString array to size of bytes_blob
                    '' ZString array is realocated to the correct size of bytes_blob
                    '' Remember to free the dynamic memory after copying to the data
                    '' structure and before returning from the function.
                    ' Dim bin_buffer As ZString Ptr  '' Must be defined at start of function.
                    ' Dim p_bb As ZString Ptr
                    p_bb = Reallocate(bin_buffer, bytes_blob * Sizeof(ZString) +1)
                    bin_buffer = p_bb  '' Rename to our original pointer.
                    '' recover the full C array of data BLOB
                    '' We must use memcpy() or similar function for this.
                    memcpy(bin_buffer, Cast(ZString Ptr, sqlite3_column_blob(p_stmt, i)), bytes_blob)
                    '' Copy the UByte buffer to the ubyte structure union.
                    '' This is done one byte at a time.
                    ''Print "DEBUG:"  '' DEBUG
                    For x = 0 To bytes_blob -1 Step +1
                    variant_structure(num_rows,i).value.bval.bdata(x) = bin_buffer[x]
                    'print Hex(bin_buffer[x], 2);  '' DEBUG
                    'print Chr(bin_buffer[x]);  '' DEBUG
                    'print Hex(variant_structure(num_rows,i).value.bval.bdata(x), 2);  '' DEBUG
                    ''print Chr(variant_structure(num_rows,i).value.bval.bdata(x));  '' DEBUG
                Next x
                'Print ""  '' DEBUG

                    ''0000  ff d8 ff e2 02 1c 49 43 43 5f 50 52 4f 46 49 4c  ......ICC_PROFIL
                    Exit Select
                Case Else
                    '' Report an error, this shouldn't happen!
                    ''printf("##default= %d##\n", variant_array[j][i].type);
                    variant_structure(num_rows,i).sql_type = IS_NULL  '' ???
                    Exit Select
            End Select
        Next i

        If num_cols > max_cols Then
            ''Retrieve the longest column length. In most cases num_columns
            '' should always be the same length for each row.
            max_cols = num_cols
        End If
        num_rows += 1
    Wend

    '' Only counts the longest column returned.
    ret_number_fields = max_cols
    ret_number_elements = num_rows

    '' Free the dynamic memory
    Deallocate(bin_buffer)

    ''sqlite3_bind_*()  '' After sqlit3_prepare_v2()
    ''sqlite3_clear_bindings(p_stmt);

    '' The sqlite3_finalize function destroys the prepared statement object.
    return_code = sqlite3_finalize( p_stmt )  '' Commit to the database.
    If return_code <> SQLITE_OK Then  '' SQLITE_OK==0
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Failed to finalize data: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    '' The sqlite3_close function closes the database connection.
    return_code = sqlite3_close(p_db)   '' SQLITE_OK==0
    If return_code <> SQLITE_OK Then
        fpErr = Freefile()  '' DEBUG
        Open Err For Input As #fpErr  '' DEBUG
        '' This is error handling code. NOTE! As p_db is closed the error code may not be available!
        Print #fpErr, "Failed to close database: "; *Cast(zString Ptr, sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #fpErr  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If

    Return 1
End Function


'' --> START helper functions


'' End of header include guard.
#endif
