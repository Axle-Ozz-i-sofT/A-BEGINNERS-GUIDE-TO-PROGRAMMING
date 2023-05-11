''------------------------------------------------------------------------------
'' Name:        sql_hello_world.c (based upon basics_2.c, ozz_sql3.h)
'' Purpose:     SQLite3 Hello world.
''
'' Platform:    Win64, Ubuntu64
'' Depends:     SQLite v3.34.1 plus (FB 3.34.0 | 3034000)
''
'' Author:      Axle
'' Created:     06/05/2023 (19/04/2023)
'' Updated:
'' Copyright:   (c) Axle 2023
'' Licence:     MIT-0 No Attribution
''------------------------------------------------------------------------------

'' Header comes standard in FreeBASIC
'' (Binary .dll, .so must be provided seperately in the path)
#include once "sqlite3.bi"
'' If required for other crt types and conversions. Note that "crt.bi" is already
'' included in "sqlite3.bi"
'#include once "crt.bi"

Declare Function main_procedure() As Integer
Declare Function sqlite3_get_version2(Byref ret_version As String) As Integer
Declare Function Con_Pause() As Integer
main_procedure()

Function main_procedure() As Integer  ' Main procedure
    
    Dim As String ver_buffer
    Dim As Integer return_code = 0
    
    '' Get our SQLite version. Confirmation that sqlite 3 is installed as a
    '' shared library and compiling/working correctly.
    '' Ensure that sqlite3.dll is in the system path or in the working directory
    '' of the project executable at run-time.
    '' NOTE: I am using the C API interface directly and not as a query. SQLite
    '' provides a limited number of helper MACROS that can be accessed directly
    '' with out opening a databse.
    Print "1 SQLite Version:"; *Cast(zString Ptr,sqlite3_libversion())
    Print "==========================================="
    
    '' Long version check with error returns.
    '' This shows the basic steps of an SQLite 3 query statement using an in
    '' memory temporary database (:memory:) to get the version number.
    return_code = sqlite3_get_version2(ver_buffer)
    If return_code = 0 Then
        Print "Version error return = " ;return_code
    Elseif return_code = 1 Then
        Print "2 SQLite Version:"; ver_buffer
    Else  '' = -1
        Print "An internal error occured."
    End If
    
    Print "==========================================="
    
    Con_Pause()
    Return 0
End Function  ' END main_procedure <---

'' Modified from:
'' https://zetcode.com/db/sqlitec/
'' Get SQLite version - query. (long function)
'' Returns string to ret_version buffer, as well as int sqlite error codes.
Function sqlite3_get_version2(Byref ret_version As String) As Integer
    
    Dim As sqlite3 Ptr p_db  '' database handle (structure).
	Dim As sqlite3_stmt Ptr statement  '' API result codes and error codes.
	Dim As Integer return_code = 0  '' structure represents a single SQL statement
    '' Result and error codes can be found here:
    '' https://www.sqlite.org/rescode.html
    
    '' return_code is the return error codes.
    '' Note: :memory: can be used instead of a file for temporary database
    '' operations.
    return_code = sqlite3_open(":memory:", @p_db)  '' Open Memory (RAM) data base.
    If return_code <> SQLITE_OK Then  '' integer 0
        Open Err For Input  As #1  '' DEBUG
        Print #1, "Cannot open database: "; *Cast(zString Ptr,sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #1  '' DEBUG
        Return -1
    End If
    
    '' START single SQLite3 query statement ==================================>>
    '' Only a single query statement can be executed at a time. The 3 functions
    '' sqlite3_prepare_v2, sqlite3_step and sqlite3_finalize must be used as
    '' a group in a routine for each sqlite query.
    '' Prepare -> Do Query -> Finalise and commit.
    
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
    '' for us SEE: sqlite3_clear_bindings(stmt);.
    return_code = sqlite3_prepare_v2(p_db, "SELECT SQLITE_VERSION()", -1, @statement, 0)
    '' On success, sqlite3_prepare_v2 returns SQLITE_OK; otherwise an error code
    '' is returned.
    If return_code <> SQLITE_OK Then
        '' This is error handling code for the sqlite3_prepare_v2 function call.
        Open Err For Input  As #1  '' DEBUG
        Print #1, !"Failed to prepare data: "; *Cast(zString Ptr,sqlite3_errmsg(p_db)); !" | "; return_code; !"\n"  '' DEBUG
        Close #1  '' DEBUG
        sqlite3_close(p_db)
        Return -1
    End If
    
    '' The sqlite3_step runs the SQL statement. SQLITE_ROW return code indicates
    '' that there is another row ready. Our SQL statement returns only one row
    '' of data in this case, therefore, we call this function only once.
    '' If we expected multiple lines of data (rows, columns) we would need to
    '' recover each table cel as a step within a loop until end of data
    '' (<>SQLITE_ROW).
    return_code = sqlite3_step(statement)
    If return_code = SQLITE_ROW Then
        '' const unsigned char *sqlite3_column_text(sqlite3_stmt*, int iCol);
        '' iCol refers to the current column in the return data. In this case
        '' there is only one column of return value, so we know the zero column
        '' contains the version number.
        '' Cast( Type, expression )  '' The return from the dll uses C data types.
        '' But is an unsigned char* so we have to convert it to a char*
        '' sqlite3_errmsg(byval as sqlite3 ptr) as const zstring ptr
        '' Note: I have found this conversion somewhat ambiguous.
        ret_version = *Cast(zString Ptr, sqlite3_column_text(statement, 0))
        
    Else
        Open Err For Input  As #1  '' DEBUG
        Print #1, "Step error: "; *Cast(zString Ptr,sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #1
        sqlite3_close(p_db)
        Return 0
    End If
    
    '' The sqlite3_finalize function destroys the prepared statement object and
    '' commits the changes to the databse file.
    return_code = sqlite3_finalize(statement)
    If return_code <> SQLITE_OK Then
        '' This is error handling code.
        Open Err For Input  As #1  '' DEBUG
        Print #1, "Failed to finalize data: "; *Cast(zString Ptr,sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #1
        sqlite3_close(p_db)
        Return -1
    End If
    
    '' The sqlite3_close function closes the database connection.
    sqlite3_close(p_db)
    If return_code <> SQLITE_OK Then
        '' This is error handling code.
        Open Err For Input  As #1  '' DEBUG
        Print #1, "Failed to close database: "; *Cast(zString Ptr,sqlite3_errmsg(p_db)); " | "; return_code  '' DEBUG
        Close #1
        sqlite3_close(p_db)
        Return -1
    End If
    
    Return 1
End Function

'' See the Basic SQLite 3 example source code library provided with the book.

' Console Pause (GetKey version)
Function Con_Pause() As Integer
    Dim As Long dummy
    Print !"\nPress any key to continue..."
    dummy = Getkey
    Return 0
End Function
