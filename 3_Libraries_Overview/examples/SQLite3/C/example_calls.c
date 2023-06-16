//------------------------------------------------------------------------------
// Name:        example_calls.c (based upon basics_1.c)
// Purpose:     SQLite3 basic examples.
//
// Platform:    Win64, Ubuntu64
// Depends:     v3.34.1 plus
//
// Author:      Axle
// Created:     03/05/2023 (19/04/2023)
// Updated:     28/05/2023
// Copyright:   (c) Axle 2023
// Licence:     MIT-0 No Attribution
//------------------------------------------------------------------------------
// Notes: basics_2.c ozz_sql3.h
//
// The examples are part of a small library ozz_sql3.h and are designed to
// illustrate some of the basics of SQLite 3. They are not organised as and
// application and are designed to be rearranged, modified or used as a base
// from which to create a small application. Some of the example calls have been
// commented out to allow for the basic creation and retrieval of some database
// tables. See ozz_sql3.h for a list of example functions and choose your
// own arrangement for the test below, or alternatively create a small app
// to accept user input and returns using the library functions.
// Modify the library functions as required for your own use.
//------------------------------------------------------------------------------
// TODO:
//
// Get the MAX length of Field Names?
// Get the length of a column, row entry?
// This is not a native API for SQLite, so a function would need to be created
// to analyse each individual row/col entry. For now I am just using arbitrary
// static limits of [128, [512], [2048] <- you can increase them if needed.
//
// Check array off by 1s.
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <errno.h>
//#include <math.h>
//#include <conio.h>
#include "sqlite3.h"
#include "example_sql3.h"

// Turn off compiler warnings for unused variables between (Windows/Linux etc.)
#define unused(x) (x) = (x)

// Comment in or out examples as required.
int main(int argc, char *argv[])
    {
    unused(argc);  // Turns off the compiler warning for unused argc, argv
    unused(argv);  // Turns off the compiler warning for unused argc, argv

    // Examples defines.
    int err_return = 0;
    // sqlite version
    char ver_buffer[32] = {'\0'};
    int return_val = 0;
    // Note Linux examples will return 3.31.1, 3032001.
    char *char_want_version = "3.34.1";  // Windows tests
    int int_want_version = 3034001;// Windows tests

    // SQLite does not impose file name extension naming restriction, but it
    // is sound practice to use a naming convention that is descriptive of the
    // database version such as .sqlite3 (or .db3 .sqlite3, .db, .db3, .s3db, .sl3)
    char *file_name = "Example_DB.db";  // The name of a database.db


    // Short version check NO error returns.( return integer)
    return_val = sqlite3_get_version0();
    printf("Version = %d\n", return_val);
    if ( int_want_version == return_val)
        {
        printf("Correct version.\n");
        }
    else
        {
        printf("Incorrect version!\n");
        }
    printf("===========================================\n");

    // Short version check NO error returns. (return byref string)
    sqlite3_get_version1(ver_buffer);
    printf("Version = %s\n", ver_buffer);
    if ( 0 == strcmp(ver_buffer, char_want_version))
        {
        printf("Correct version.\n");
        }
    else
        {
        printf("Incorrect version!\n");
        }
    printf("===========================================\n");

    // Long version check with error returns.
    err_return = sqlite3_get_version2(ver_buffer);
    if (err_return == 0)
        {
        printf("Version error return = %d\n", err_return);
        }
    else if (err_return == 1)
        {
        printf("Version = %s\n", ver_buffer);
        if ( 0 == strcmp(ver_buffer, char_want_version))
            {
            printf("Correct version.\n");
            }
        else
            {
            printf("Incorrect version!\n");
            }
        }
    else  // == -1
        {
        printf("An internal error occurred.\n");
        }
    printf("===========================================\n");


    // File exists? ( check if a file name exists) "Example_DB.db".
    err_return = file_exists(file_name);
    if (err_return == 0)
        {
        printf("File %s Not found.\n", file_name);
        }
    else  // ==1
        {
        printf("File %s WAS found.\n", file_name);
        }
    printf("===========================================\n");


    // Test if "SQLite format 3" and if db file exists.
    err_return = db_file_exists(file_name);
    if (err_return == 0)
        {
        printf("%s Not found, or not SQLite3 database.\n", file_name);
        }
    else  // ==1
        {
        printf("%s Is an SQLite3 database.\n", file_name);
        }
    printf("===========================================\n");



    // Create SQLITE V3 db file as FileName.db
    // (or .db3 .sqlite3, .db, .db3, .s3db, .sl3)
    err_return = db_file_create(file_name);
    if (err_return == 2)
        {
        // Maybe add different error returns values.
        printf("%s already exists.\n", file_name);
        }
    else if (err_return == 1)
        {
        printf("%s successfully created.\n", file_name);
        }
    else  // err_return==0
        {
        printf("An internal error occurred.\n");
        }
    printf("===========================================\n");


    // Delete a named database file.
    // Remove block comments /* ... */ to use.
    /*
        err_return = db_file_delete(file_name);
        if (err_return == -1)
            {
            printf("%s was NOT deleted or not exists.\n", file_name);
            }
        else if(err_return == 1)
            {
            printf("%s was successfully deleted.\n", file_name);
            }
        else // == 0
            {
            printf(" File %s delete action terminated by user.\n", file_name);
            }
        printf("===========================================\n");
    */


    // TableName exists?
    char *db_table_name = "Hrs_worked_Tracker";

    err_return = db_table_exists(file_name, db_table_name);
    if (err_return == 0)
        {
        printf("The Table %s does NOT exist in %s.\n", db_table_name, file_name);
        }
    else if ( err_return == 1)
        {
        printf("The Table %s DOES exist in %s.\n", db_table_name, file_name);
        }
    else  // == -1
        {
        printf("There was an unknown error.\n");
        }
    printf("===========================================\n");



    // Get the total number of tables in a named database file.
    int number_tables_ret = 0;  // Variable to hold returned number of tables.

    err_return = db_get_number_tables(file_name, &number_tables_ret);
    if (err_return == 0)
        {
        printf("Could NOT retrieve number of tables from %s.\n", file_name);
        }
    else if ( err_return == 1)
        {
        printf("Retrieved number of tables from from %s.\n", file_name);
        }
    else  // == -1
        {
        printf("There was an unknown error.\n");
        }
    printf("Number of tables=%d\n", number_tables_ret);
    printf("===========================================\n");


    // Return an array of table names.

    // Create a dynamic 2D array.
    // Must be "Free"ed after the function call and data use.
    // will return a dynamic array of all TableName in the database.
    char **db_tbl_names = NULL;
    int Max_FieldName = 256;  // Maximum length of the column names.
    int i, j;
    // Malloc 2D array of characters.
    db_tbl_names = (char **) malloc(number_tables_ret * sizeof(char*));
    for(i = 0; i < number_tables_ret; i++)
        {
        db_tbl_names[i] = (char *) malloc(Max_FieldName * sizeof(char));
        }
    // Declare/assign 2D array of characters with zero string terminator.
    // The second dimension db_tbl_data2[Column_number]["String"] is now a
    // valid string.
    for(i = 0; i < number_tables_ret; i++)
        {
        for(j = 0; j < Max_FieldName; j++)
            {
            db_tbl_names[i][j] = '\0';
            }
        }

    // Return an array of table names.
    err_return = db_get_tablenames(file_name, db_tbl_names);
    if (err_return == 0)
        {
        printf("Could NOT retrieve table names from %s.\n", file_name);
        }
    else if ( err_return == 1)
        {
        printf("Retrieved table names from %s.\n", file_name);
        }
    else  // == -1
        {
        printf("There was an unknown error.\n");
        }

    printf("Returned table names:\n");
    for(i = 0; i < number_tables_ret; i++)
        {

        printf("%s\n", db_tbl_names[i]);

        }

    // deallocate dynamic memory.
    for (i = 0; i < number_tables_ret; i++)
        {
        free(db_tbl_names[i]);
        }
    free(db_tbl_names);

    printf("===========================================\n");


    // create table sqlite3 query statement examples.

    // Note that SQLite will add a rowid increment automatically. If the following
    // is added as a field ID_Name INTEGER PRIMARY KEY then this column will
    // become an alias for rowid. You do not have to add an entry for the
    // INTEGER PRIMARY KEY column as SQLite3 will automatically add the value.
    // Note the C string line continuation character \
    // Note: \ will throw a compiler warning as you can accidentally comment out
    // the next line using the continuation char.
    // We can also use = "My long string "
    //                   "on 2 lines";
    /* // Note! this will fail with current functions that only handle TEXT
    char *db_table1 = "CREATE TABLE IF NOT EXISTS Hrs_worked_Tracker\
                         (INDEX_ID INTEGER PRIMARY KEY\
                         , Week TEXT\
                         , Employee_ID TEXT\
                         , Name TEXT\
                         , Monday TEXT\
                         , Tuesday TEXT\
                         , Wednesday TEXT\
                         , Thursday TEXT\
                         , Friday TEXT);";
    */

    // A full sqlite3 query (statement) we can also create a query template
    // here or withing the function and add the data through concatenation.
    // This is exemplified in later functions.
    // Note: If the table exist already no error is returned.
    char *db_table1 = "CREATE TABLE IF NOT EXISTS Hrs_worked_Tracker\
                         (Week TEXT\
                         , Employee_ID TEXT\
                         , Name TEXT\
                         , Monday TEXT\
                         , Tuesday TEXT\
                         , Wednesday TEXT\
                         , Thursday TEXT\
                         , Friday TEXT);";


    err_return = db_table_create(file_name, db_table1);
    if (err_return == 0)
        {
        printf("Table could not be created in %s.\n", file_name);
        }
    else if ( err_return == 1)
        {
        printf("Table was successfully created in %s.\n", file_name);
        }
    else
        {
        printf("There was an unknown error.\n");
        }
    printf("===========================================\n");


    // Recheck if Table EXISTS after creating the empty table.
    err_return = db_table_exists(file_name, db_table_name);
    if (err_return == 0)
        {
        printf("The Table %s does NOT exist in %s.\n", db_table_name, file_name);
        }
    else if ( err_return == 1)
        {
        printf("The Table %s DOES exist in %s.\n", db_table_name, file_name);
        }
    else  // == -1
        {
        printf("There was an unknown error.\n");
        }
    printf("===========================================\n");


    /*
    // delete table. See Create_empty_db. Drop Table.
    char *db_table2 = "DROP TABLE IF EXISTS Hrs_worked_Tracker;";

    err_return = db_table_delete(file_name, db_table2);
    if (err_return == 0)
        {
        printf("Table NOT successfully deleted in %s.\n", file_name);
        }
    else if ( err_return == 1)
        {
        printf("Table successfully deleted %s.\n", file_name);
        }
    else  // == -1
        {
        printf("There was an unknown error.\n");
        }
    printf("===========================================\n");
    // recheck if Table EXISTS after deleting the table.
    */



    // add table entry/s (will add the data at the next available rowid)
    // Comment this out for later test if the entries become to large.
    // TableName must exist.
    // Single entry by Column Name:
    // "INSERT INTO table ( column2 ) VALUES( value2 );"
    // Full row:
    // "INSERT INTO table (column1,column2 ,..) VALUES( value1,	value2 ,...);"
    // Multiple rows:
    // "INSERT INTO table (column1,column2 ,..) \
    //              VALUES( value1,	value2 ,...), \
    //                    (value1,value2 ,...), \
    //                    ... \
    //                    (value1,value2 ,...);

    char *db_tbl_entry = "INSERT INTO Hrs_worked_Tracker \
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
                                           , \"9\");";

    err_return = db_insert_table_rowdata(file_name, db_tbl_entry);
    if (err_return == 0)
        {
        printf("Row data was NOT entered into %s.\n", file_name);
        }
    else if ( err_return == 1)
        {
        printf("Row data was entered into %s.\n", file_name);
        }
    else  // == -1
        {
        printf("There was an unknown error.\n");
        }
    printf("===========================================\n");


//============================================================================>>

    // Get number of columns in a named table.
    int number_cols_ret = 0;  // Variable for the returned number of columns.

    err_return = db_get_table_number_cols(file_name, db_table_name, &number_cols_ret);
    if (err_return == 0)
        {
        printf("Could not retrieve table %s column number from %s.\n", db_table_name, file_name);
        }
    else if ( err_return == 1)
        {
        printf("Retrieved table %s column number from %s.\n", db_table_name, file_name);
        }
    else  // == -1
        {
        printf("There was an unknown error.\n");
        }

    printf("\nTable number of columns:%d\n", number_cols_ret);
    printf("===========================================\n");


    // Get number of rows in a named table.
    int number_rows_ret = 0;  // Variable for the returned number of rows.

    err_return = db_get_table_number_rows(file_name, db_table_name, &number_rows_ret);
    if (err_return == 0)
        {
        printf("Could not retrieve table %s row number from %s.\n", db_table_name, file_name);
        }
    else if ( err_return == 1)
        {
        printf("Retrieved table %s row number from %s.\n", db_table_name, file_name);
        }
    else  // == -1
        {
        printf("There was an unknown error.\n");
        }

    printf("\nTable number of rows:%d\n", number_rows_ret);
    printf("===========================================\n");


    // TODO
    // Get the MAX length of Field Names?
    // Get the length of a column, row entry?
    // Aa function would need to be created to analyse each individual row/col
    // entry. For now I am just using arbitrary
    // static limits of [128, [512], [2048] <- you can increase them if needed.
    // see: sqlite3_column_bytes() to get the data length.


    // Return an array of column names (fields).
    // Must be "Free"ed after the function call and data use.
    // Will return a dynamic array of all column names the table.
    char **db_tbl_data0 = NULL;
    //int Max_FieldName = 256;  // Maximum length of the column names.
    //int i, j;
    // Malloc 2D array od characters.
    db_tbl_data0 = (char **) malloc(number_cols_ret * sizeof(char*));
    for(i = 0; i < number_cols_ret; i++)
        {
        db_tbl_data0[i] = (char *) malloc(Max_FieldName * sizeof(char));
        }
    // Declare 2D array of characters with zero string terminator.
    // The second dimension db_tbl_data2[Column_number]["String"] is now a
    // valid string.
    for(i = 0; i < number_cols_ret; i++)
        {
        for(j = 0; j < Max_FieldName; j++)
            {
            db_tbl_data0[i][j] = '\0';
            }
        }

    err_return = db_get_table_colnames(file_name, db_table_name, db_tbl_data0);
    if (err_return == 0)
        {
        printf("Could not retrieve table %s column names from %s.\n", db_table_name, file_name);
        }
    else if ( err_return == 1)
        {
        printf("Retrieved table %s column names from %s.\n", db_table_name, file_name);
        }
    else  // == -1
        {
        printf("There was an unknown error.\n");
        }

    printf("Array length = %d\n", number_cols_ret);
    printf("Table column names:\n");
    for( i = 0; i < number_cols_ret; i++)
        {
        printf("Col %d:%s\n", i, db_tbl_data0[i]);
        }

    // deallocate memory ( Moved to after table search). db_search_table_rowdata_allfields()
    /*
    for (i = 0; i < number_cols_ret; i++)
        {
        free(db_tbl_data0[i]);
        }
    free(db_tbl_data0);
    */
    printf("===========================================\n");


//==============================================================================

    // Retrieve all table entry/s to array[][]
    // Must be "Free"ed after the function call and data use.
    // Will return a dynamic array of all data it the table. The returned data
    // will need to be sorted for inspection or use.
    char **db_tbl_data1 = NULL;
    int Max_rowlength = 2048;  // Maximum length of the column names.
    //int i, j;
    // Malloc 2D array of characters.
    db_tbl_data1 = (char **) malloc(number_rows_ret * sizeof(char*));
    for(i = 0; i < number_rows_ret; i++)
        {
        db_tbl_data1[i] = (char *) malloc(Max_rowlength * sizeof(char));
        }
    // Declare 2D array of characters with zero string terminator.
    // The second dimension db_tbl_data2[Column_number]["String"] is now a
    // valid string.
    for(i = 0; i < number_rows_ret; i++)
        {
        for(j = 0; j < Max_rowlength; j++)
            {
            db_tbl_data1[i][j] = '\0';
            }
        }

    //int number_columns = 8;
    // The number of columns can also se found internally using
    // sqlite3_column_count(statement/stmt)
    int number_columns = number_cols_ret;  // From db_get_table_number_cols()

    err_return = db_list_table_rows_data(file_name, db_table_name, db_tbl_data1, number_columns);
    if (err_return == 0)
        {
        printf("Could not retrieve table %s data from %s.\n", db_table_name, file_name);
        }
    else if ( err_return == 1)
        {
        printf("Retrieved table %s data from %s.\n", db_table_name, file_name);
        }
    else  // == -1
        {
        printf("There was an unknown error.\n");
        }

    // Print the table data. Column 0 is the row_id.
    printf("Table data:\n");
    printf("Array length = %d\n", number_rows_ret);
    
    char concat[64] = {'\0'};
    strcpy(concat, "");

    // Simple example to obtain the rowid for other tasks.
    for( i = 0; i < number_rows_ret; i++)
        {
        printf("%s\n", db_tbl_data1[i]);  // Print the full row data.
        strcpy(concat, "");
        // Obtain the first column (rowid) bfor ','
        for(j = 0; j < strlen(db_tbl_data1[i]); j++)
        {
        if(db_tbl_data1[i][j] == ',')  // int 44
               {
               break;  // If we reach ',' then we have the value of rowid.
               }
            concat[j] = db_tbl_data1[i][j];  // Add each char of rowid.
            concat[j+1] = '\0';  // String terminator.
            }
        printf("rowid = %s\n", concat);  // Print the rowid as char string.
        // If you wish to use this rowid as an integer you will need to convert,
        // or cast it to an integer value. See: atoi().

        }



    // deallocate dynamic memory
    for (i = 0; i < number_rows_ret; i++)
        {
        free(db_tbl_data1[i]);
        }
    free(db_tbl_data1);
    printf("===========================================\n");


    /*
        // delete table entry/s by search word (dangerous!)
        // NOTE!!! This needs to be revised with more narrow focus and safeguards !!!
        // The following will delete ALL rows containing "1" and "Joe Blogs". It
        // is appropriate to check the entry row number index_id before deleting.
        char *db_row_entry = "DELETE FROM Hrs_worked_Tracker\
                             WHERE Week = \"1\" AND Name = \"Joe Blogs\";";

        err_return = db_delete_table_rowdata(file_name, db_row_entry);
        if (err_return == 0)
            {
            printf("Row data NOT deleted from %s.\n", file_name);
            }
        else if ( err_return == 1)
            {
            printf("Row data deleted from %s.\n", file_name);
            }
        else  // == -1
            {
            printf("There was an unknown error.\n");
            }
        printf("===========================================\n");
    */


//==============================================================================


    // Get number of rows in a named table.
    number_rows_ret = 0;  // Variable for the returned number of rows.
    //int db_get_table_number_rows(char *db_file_name, char *db_table_name, int *number_rows);
    err_return = db_get_table_number_rows(file_name, db_table_name, &number_rows_ret);
    if (err_return == 0)
        {
        printf("Could not retrieve table %s row number from %s.\n", db_table_name, file_name);
        }
    else if ( err_return == 1)
        {
        printf("Retrieved table %s row number from %s.\n", db_table_name, file_name);
        }
    else  // == -1
        {
        printf("There was an unknown error.\n");
        }

    printf("\nTable number of rows:%d\n", number_rows_ret);
    printf("===========================================\n");

    int number_rows = number_rows_ret;

    // TODO:
    // Get last rowid. Only works after a db table was opened and not
    // yet closed.
    //int last_id = sqlite3_last_insert_rowid(db);
    //printf("The last Id of the inserted row is %d\n", last_id);


    //========================================================================>>

    // Test if rowid exist in a table.
    //int db_table_rowid_exists(char *file_name, char *db_table_name, int rowid);
    int tbl_rowid = 2;

    err_return = db_table_rowid_exists(file_name, db_table_name, tbl_rowid);
    if (err_return == 0)
        {
        printf("Table %s rowid does not exist.\n", db_table_name);
        }
    else if ( err_return == 1)
        {
        printf("Table %s rowid does exist.\n", db_table_name);
        }
    else  // == -1
        {
        printf("There was an unknown error.\n");
        }

    printf("\nRowid:%d\n", tbl_rowid);

    printf("===========================================\n");


    /*
    // delete row by "rowid". This does not question or ask for confirmation!

    int sql_rowid1 = 2;
    //char *db_row_entry = "DELETE FROM Hrs_worked_Tracker WHERE rowid = 2;";
    if(sql_rowid1 <= number_rows_ret)
    {
        //int db_delete_table_rowdata_rowid(char *db_file_name, char *db_table_name, int sql_rowid);
        err_return = db_delete_table_rowdata_rowid(file_name, db_table_name, sql_rowid1);
        if (err_return == 0)
            {
            printf("Rowid data NOT deleted from %s.\n", file_name);
            }
        else if ( err_return == 1)
            {
            printf("Rowid data deleted from %s.\n", file_name);
            }
        else  // == -1
            {
            printf("There was an unknown error.\n");
            }

    }
    else
        {
        printf("rowid does not exist!\n");
        }
    printf("===========================================\n");
    */


    /*
    // update/replace by rowid. This will replace/overwrite existing row data.
    // This will replace the existing rowid or INDEX_ID with neww data for each
    // column name assigned.
    // Alternative rowid, INDEX_ID
    // Note using INT/INTEGER with current function will fail!

    char *db_table_name2 = "Hrs_worked_Tracker";
    int sql_rowid2 = 2;
    // This can be obtained from db_get_table_colnames(). use concatenation to create the following string.
    char *db_field_names = "Week, Employee_ID, Name, Monday, Tuesday, Wednesday, Thursday, Friday";
    char *db_field_values = "\"2\", \"36\", \"Jill Blogs\", \"9\", \"5\", \"4\", \"7\", \"6\"";

    // Original entry.
    //char *db_tbl_entry = "REPLACE INTO Hrs_worked_Tracker \
    //                               (rowid\
    //                               , Week\
    //                               , Employee_ID\
    //                               , Name\
    //                               , Monday\
    //                               , Tuesday\
    //                               , Wednesday\
    //                               , Thursday\
    //                               , Friday) \
    //                         VALUES( 3\
    //                                , \"2\"\
    //                               , \"36\"\
    //                               , \"Jill Blogs\"\
    //                               , \"9\"\
    //                               , \"5\"\
    //                               , \"5\"\
    //                               , \"7\"\
    //                               , \"8\");";


    if(number_rows >= sql_rowid2)
        {
        err_return = db_replace_table_rowdata_rowid(file_name, db_table_name2, sql_rowid2, db_field_names, db_field_values);
        if (err_return == 0)
            {
            printf("Rowid data was NOT replaced into %s.\n", file_name);
            }
        else if ( err_return == 1)
            {
            printf("Rowid data was replaced into %s.\n", file_name);
            }
        else  // == -1
            {
            printf("There was an unknown error.\n");
            }
        }
    else
        {
        printf("rowid does not exist!\n");
        }
    printf("===========================================\n");
    */



    // Get the current number of rows in the TableName.

    err_return = db_get_table_number_rows(file_name, db_table_name, &number_rows_ret);
    if (err_return == 0)
        {
        printf("Could not retrieve table %s number of rows from %s.\n", db_table_name, file_name);
        }
    else if ( err_return == 1)
        {
        printf("Retrieved table %s number of rows from %s.\n", db_table_name, file_name);
        }
    else  // == -1
        {
        printf("There was an unknown error.\n");
        }

    printf("\nTable number of rows:%d\n", number_rows_ret);

    printf("===========================================\n");
    number_rows = number_rows_ret;  // Used for db_insert_table_rowdata_rowid()


    /*
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
    // If rowid doesn't exist does not write.
    // Copies each row down 1 at a time to create space and new row at rowid in the table.
    // The new row is placed into the rowid using REPLACE INTO.
    // the last row is INSERT INTO a new rowid at the end of the table
    // Copy notes from function to here!!!
    // Test error handling!!!
    // Consider remove '\r\, '\n' etc. from strings.

    char *db_table_name3 = "Hrs_worked_Tracker";
    int sql_rowid3 = 2;
    // This can be obtained from db_get_table_colnames(). use concatenation to create the following string.
    char *db_field_names3 = "Week, Employee_ID, Name, Monday, Tuesday, Wednesday, Thursday, Friday";
    char *db_field_values3 = "\"2\", \"36\", \"Bill Roger\", \"2\", \"3\", \"4\", \"5\", \"6\"";

    if(number_rows >= sql_rowid3)
    {
    err_return = db_insert_table_rowdata_rowid(file_name, db_table_name3, sql_rowid3, db_field_names3, db_field_values3, number_columns, number_rows);
    if (err_return == 0)
        {
        printf("Row data was NOT inserted into %s.\n", file_name);
        }
    else if ( err_return == 1)
        {
        printf("Row data was inserted into %s.\n", file_name);
        }
    else  // == -1
        {
        printf("There was an unknown error.\n");
        }
    }
    else
    {
        printf("rowid does not exist!\n");
    }
    printf("===========================================\n");
    */


    //Read row from rowid. returned as csv string.
    char *db_table_name4 = "Hrs_worked_Tracker";
    int sql_rowid4 = 4;
    // This can be obtained from db_get_table_colnames(). use concatenation to create the following string.
    //char *db_field_names3 = "Week, Employee_ID, Name, Monday, Tuesday, Wednesday, Thursday, Friday";
    //char *db_field_values3 = "\"2\", \"36\", \"Bill Knight\", \"2\", \"3\", \"4\", \"5\", \"6\"";
    char db_tbl_rowid_data[2048] = {'\0'};  // MAX row data length 2048 characters.
    strcpy( db_tbl_rowid_data, "");

    if(number_rows >= sql_rowid4)
        {
        err_return = db_read_table_rowdata_rowid(file_name, db_table_name4, sql_rowid4, db_tbl_rowid_data, number_columns);
        if (err_return == 0)
            {
            printf("Rowid data was NOT read from %s.\n", file_name);
            }
        else if ( err_return == 1)
            {
            printf("Rowid data was read from %s.\n", file_name);
            }
        else  // == -1
            {
            printf("There was an unknown error.\n");
            }

        // Print the table data.
        printf("Table rowid data:\n");
        printf("%s\n", db_tbl_rowid_data);
        printf("number of columns = %d\n", number_columns);

        int csv_row_length = strlen(db_tbl_rowid_data);
        printf("csv_row_length characters=%d\n", csv_row_length);
        int ch = 0;

        for( i = 0; i < csv_row_length; i++)
            {
            ch = db_tbl_rowid_data[i];
            if(ch == ',')  // Find the delimiters
                {
                // Skip and new line. Removing white-space requires a little more.
                printf("\n");
                }
            else
                {
                printf("%c", ch);  // print each character for the column data.
                }
            // split data at ','
            }
        printf("\n");
        }
    else
        {
        printf("rowid does not exist!\n");
        }

    printf("===========================================\n");

    //========================================================================<<


    //========================================================================>>

    // Search table entry/s by column (field) name.
    // Dynamic array must be "Free"ed after the function call and data use.
    // Will return a dynamic array of all data in the table. The returned data
    // will need to be sorted for inspection or use.
    char **db_tbl_data3 = NULL;
    //int Max_FieldName = 128;  // Maximum length of the column names.
    //int i, j;
    // Malloc 2D array od characters.
    db_tbl_data3 = (char **) malloc(number_rows_ret * sizeof(char*));
    for(i = 0; i < number_rows_ret; i++)
        {
        db_tbl_data3[i] = (char *) malloc(Max_rowlength * sizeof(char));
        }

    // Declare 2D array of characters with zero string terminator.
    // The second dimension db_tbl_data3[Column_number]["String"] is now a
    // valid string.
    for(i = 0; i < number_rows_ret; i++)
        {
        for(j = 0; j < Max_rowlength; j++)
            {
            db_tbl_data3[i][j] = '\0';
            }
        }

    // Get the number of search rows found.
    int ret_array_length1 = 0;  // We cannot get the length of array elements in C
    // so we need to return the number of array positions that have been
    // populated from the search. Alternatively we can enumerate to full array
    // length from number_rows_ret and filter out empty '\0' elements.

    //char *db_row_search = "SELECT FROM Hrs_worked_Tracker\
    //                  WHERE ANY = \"Joe Blogs\";";

    // This needs to be converted to full table search. requires EXACT match.
    // #### The inverted commas are emitted in many examples. More research !! ###
    char *field_name = "\"Name\"";  // " 'value' " is not acceptable from C.
    char *db_search_string1 = "\"Joe Blogs\"";  // " \"value\" " is acceptable.
    //char *db_search_string = "\"Blogs\"";  // research wild cards :)
    //char temp_buffer[128] = {'\0'};
    //for( i = 0; i < number_columns; i++)
    //     {
    //     strcpy(temp_buffer,db_tbl_data0[i]);
    //     }


    // search TableName by column name (field) and search word.
    //int number_columns2 = 8;
    // number_columns will be +1 because we are also retrieving the row_id number.
    err_return = db_search_table_rowdata_byfield(file_name, db_table_name, db_tbl_data3, field_name, db_search_string1, number_columns, &ret_array_length1);
    if (err_return == 0)
        {
        printf("Could not retrieve table %s search data from %s.\n", db_table_name, file_name);
        }
    else if ( err_return == 1)
        {
        printf("Retrieved table %s search data from %s.\n", db_table_name, file_name);
        }
    else  // == -1
        {
        printf("There was an unknown error.\n");
        }

    // Print returned search result (full array).
    printf("Table column search data:\n");
    /*
    for( i = 0; i < number_rows_ret; i++)
        {
        printf("%s\n", db_tbl_data3[i]);
        }
    */
    // Print returned search result (actual number search rows found).
    printf("Array length = %d\n", ret_array_length1);
    for( i = 0; i < ret_array_length1; i++)
        {
        printf("%s\n", db_tbl_data3[i]);
        }

    // deallocate memory
    for (i = 0; i < number_rows_ret; i++)
        {
        free(db_tbl_data3[i]);
        }
    free(db_tbl_data3);
    printf("===========================================\n");



    // Search all columns (fields) for search string in table name. Must be EXACT
    // search word match. Returns array in the order of rows found without duplicates.
    // Dynamic array must be "Free"ed after the function call and data use.
    // Will return a dynamic array of all found rows in the table. The returned data
    // will need to be sorted for inspection or use.
    char **db_tbl_data4 = NULL;
    //int Max_FieldName = 128;  // Maximum length of the column names.
    //int i, j;
    // Malloc 2D array od characters.
    db_tbl_data4 = (char **) malloc((number_rows_ret) * sizeof(char*));  // * 7 is temp safety. remove.
    for(i = 0; i < (number_rows_ret); i++)
        {
        db_tbl_data4[i] = (char *) malloc(Max_rowlength * sizeof(char));
        }

    // Declare 2D array od characters with zero string terminator.
    // The second dimension db_tbl_data2[Column_number]["String"] is now a
    // valid string.
    for(i = 0; i < (number_rows_ret); i++)
        {
        for(j = 0; j < Max_rowlength; j++)
            {
            db_tbl_data4[i][j] = '\0';
            }
        }

    // Get the number of search result rows found.
    int ret_array_length2 = 0;  // We cannot get the length of array elements in C
    // so we need to return the number of array positions that have been
    // populated from the search. Alternatively we can enumerate to full array
    // length from number_rows_ret and filter out empty '\0' elements.

    //char *db_search_string2 = "\"Joe Blogs\"";
    //"\"2\", \"36\", \"Jill Blogs\", \"9\", \"5\", \"4\", \"7\", \"6\""  // DEBUG
    char *db_search_string2 = "\"9\"";  // 6, 9
    // # internalise field/column names ?
    // switch number_columns next to db_tbl_data0 (column name/field_name)
    err_return = db_search_table_rowdata_allfields(file_name, db_table_name, db_tbl_data4, db_tbl_data0, db_search_string2, number_columns, number_rows_ret, &ret_array_length2);
    if (err_return == 0)
        {
        printf("Could not retrieve table %s search data from %s.\n", db_table_name, file_name);
        }
    else if ( err_return == 1)
        {
        printf("Retrieved table %s search data from %s.\n", db_table_name, file_name);
        }
    else  // == -1
        {
        printf("There was an unknown error.\n");
        }

    // Print returned search result.
    printf("Table ALL column search data:\n");
    // Print returned search result (actual number search rows found).
    printf("Array length = %d\n", ret_array_length2);
    for( i = 0; i < ret_array_length2; i++)
        {
        printf("%s\n", db_tbl_data4[i]);
        }

    // deallocate memory
    for (i = 0; i < (number_rows_ret); i++)
        {
        free(db_tbl_data4[i]);
        }
    free(db_tbl_data4);

    // deallocate memory from get field names (column names).
    for (i = 0; i < number_cols_ret; i++)
        {
        free(db_tbl_data0[i]);
        }
    free(db_tbl_data0);

    printf("===========================================\n");

    S_Pause();
//==============================================================================
// START Multiple types examples.

   /*
    // Create a table and field for binary BLOBS
    char *db_table_namex = "DATA_Blobs";  // Table with single column BLOB
    // Test data.
    unsigned char bin_data[16] = {0xff, 0xd8, 0xff, 0xe2, 0x02, 0x1c, 0x49, 0x43, 0x43, 0x5f, 0x50, 0x52, 0x4f, 0x46, 0x49, 0x4c};
    int bin_data_len = 16;


    char *db_table5 = "CREATE TABLE IF NOT EXISTS DATA_Blobs\
                         (Binary_data BLOB);";

    err_return = db_table_create(file_name, db_table5);
    if (err_return == 0)
        {
        printf("Table %s could not be created in %s.\n", db_table_namex, file_name);
        }
    else if ( err_return == 1)
        {
        printf("Table %s was successfully created in %s.\n", db_table_namex, file_name);
        }
    else
        {
        printf("There was an unknown error %s.\n", db_table_namex );
        }
    printf("===========================================\n");


    // Insert some binary (BLOB) test data.

    // A variation of the INSERT statement using sqlite3_bind_*().
    // We can send data/values separately replacing the values into '?' using
    // various sqlite3_bind*() functions. It will be necessary to know the
    // data type and infinity before hand and use the select case as I have
    // in the examples for reading the data.
    char *db_tbl_entry5 = "INSERT INTO DATA_Blobs (Binary_data) VALUES(\?);";
    // Consider remove '\r\, '\n' etc. from strings.

    err_return = db_insert_table_rowdata_bin(file_name, db_tbl_entry5, bin_data, bin_data_len);
    if (err_return == 0)
        {
        printf("Row data was NOT entered into %s.\n", file_name);
        }
    else if ( err_return == 1)
        {
        printf("Row data was entered into %s.\n", file_name);
        }
    else  // == -1
        {
        printf("There was an unknown error.\n");
        }
    printf("===========================================\n");
    */

    //==========================================================================

    /*
    //int bin_data_len = 16;
    // Get number of columns in a named table.
    int number_cols_retx = 0;

    err_return = db_get_table_number_cols(file_name, db_table_namex, &number_cols_retx);
    if (err_return == 0)
        {
        printf("Could not retrieve table %s column number from %s.\n", db_table_namex, file_name);
        }
    else if ( err_return == 1)
        {
        printf("Retrieved table %s column number from %s.\n", db_table_namex, file_name);
        }
    else  // == -1
        {
        printf("There was an unknown error.\n");
        }

    printf("\nTable number of columns:%d\n", number_cols_retx);

    printf("===========================================\n");


    // Get number of rows in a named table.
    int number_rows_retx = 0;
    //int db_get_table_number_rows(char *db_file_name, char *db_table_name, int *number_rows);
    err_return = db_get_table_number_rows(file_name, db_table_namex, &number_rows_retx);
    if (err_return == 0)
        {
        printf("Could not retrieve table %s row number from %s.\n", db_table_namex, file_name);
        }
    else if ( err_return == 1)
        {
        printf("Retrieved table %s row number from %s.\n", db_table_namex, file_name);
        }
    else  // == -1
        {
        printf("There was an unknown error.\n");
        }

    printf("\nTable number of rows:%d\n", number_rows_retx);

    printf("===========================================\n");


    // More universal query function for mixed data types.
    // As you will see this is more complex than using a single data type in
    // the table columns. Personally for small database requirements I store
    // everything as TEXT and keep a track of the column affinity (data type)
    // in my calling application and convert values to other types as required.
    // The only exception to this is binary data (BLOBs) which would need to be
    // converted to TEXT using a Base64 encoder. I would not attempt to store
    // large amounts of binary data in this way. If you do have to store large
    // binary data sets such as images etc then you will need to make use of the
    // correct types and SQLite affinities as shown in this function.
    // This will offer a sound example to build more complex database queries.
    // See the modified version db_insert_table_rowdata_bin() for hints as how
    // to insert mixed data types based upon the select case examples and the
    // VARIANT structure examples.

    //int j = 0, i = 0;
    // To use the type tagVARIANT
    // Static array version initialised to NULL, 0
    // tagVARIANT variant_array[3][10] = { .type = 0, { .vval = NULL, .ival = 0, .rval = 0.0, .tval = '\0', .bval = 0}};  // static arary of 3 * 10 tagVARIANT

    // NOTE: SQLite3 does have it's own internal typeless data structure Mem.
    // typedef struct Mem Mem;
    // It is an extremely complex data structure that includes many other data
    // structures defined in the sqlite source. Also it is predominantly used
    // with the sqlite3_value/_* set of API functions.
    // typedef struct sqlite3_value sqlite3_value;
    // It is more convenient to create our own tag struct, union or linked list
    // for the following example.

    // We will have to initialise/assign the dynamic array in a for loop.
    // Get these values from int *ret_number_columns, int *ret_number_rows
    // Elements should be the same size as variant_array_len0 unless we
    // purposefully create an array larger then the number of rows in the table.
    int variant_array_rowlen = number_rows_retx;  // Always check the most recent number of rows in the table
    int variant_array_collen = number_cols_retx + 1;  // The number of columns + 1 for rowid

    // Track the returned number of used elements in the array.
    int ret_variant_field_elements = 0;
    int ret_variant_row_elements = 0;
    // Note: it is possible in a more complex structure to track the array size
    // and elements used within the structure.

    // Create 2D dynamic array using number rows and columns
    // tagVARIANT structure is declared in example_sql3.h
    tagVARIANT **variant_array = NULL;
    variant_array = (tagVARIANT **) malloc(variant_array_rowlen * sizeof(tagVARIANT*));
    for(i = 0; i < variant_array_rowlen; i++)
        {
        variant_array[i] = (tagVARIANT *) malloc(variant_array_collen * sizeof(tagVARIANT));  // 10 -> variant_array_collen
        }

    // Initialise with zero values. (Later values will be assigned to each element)
    // Note: The char/byte lengths are currently static with a MAX length
    // of 30720 bytes. For larger strings or byte arrays change the tagVARIANT
    // structure declaration to *tval, and *bdata.
    // Then re/malloc() here using the correct returned length of the TEXT/BOLB
    // Columns from the database table.
    for(i = 0; i < variant_array_rowlen; i++)
        {
        for(j = 0; j < variant_array_collen; j++)
            {
            variant_array[i][j].type = 0;  //
            variant_array[i][j].value.vval = NULL;
            variant_array[i][j].value.ival = 0;
            variant_array[i][j].value.rval = 0.0;
            strcpy(variant_array[i][j].value.tval, "");
            variant_array[i][j].value.bval.blen = 0;
            variant_array[i][j].value.bval.bdata[0] = 0;
            }
        }

    // More universal query function for mixed data types.
    // Note: The variant_array_collen, variant_array_rowlen are not currently
    // used in this function call as they are calculated within the function.
    err_return = db_list_table_all_types(file_name, db_table_namex, variant_array, variant_array_collen, variant_array_rowlen, &ret_variant_field_elements, &ret_variant_row_elements);
    if (err_return == 0)
        {
        printf("Could not retrieve %s table data from %s.\n", db_table_namex, file_name);
        }
    else if ( err_return == 1)
        {
        printf("Retrieved table %s data from %s.\n", db_table_namex, file_name);
        }
    else  // == -1
        {
        printf("There was an unknown error.\n");
        }

    // Test the size of returned number of row elements against our array[n]
    // DEBUG truth test for memory leaks/off by 1/ buffer over runs.
    if (variant_array_rowlen < ret_variant_row_elements)
        {
        printf("Error! Dynamic array is too small for number of rows.\n");
        }
    else if (variant_array_rowlen > ret_variant_row_elements)
        {
        printf("Good! Dynamic array is lager than number of rows.\n");
        }
    else if (variant_array_rowlen == ret_variant_row_elements)
        {
        printf("Good! Dynamic array same size as number of rows.\n");
        }
    else
        {
        printf("Unknown error!\n");  // should never occur
        }

    // Print all mixed data types returned from the table. Each column is sorted
    // to the relevant type by testing [][].type
    printf("Print mixed table data from variant_array.\n");
    // To DEBUG test against the original length of the array of VARIANT (bin_data_len = 16)
    int bdata_len2 = 0;
    int x = 0;
    // Step through each row element from the array.
    for (j = 0; j < variant_array_rowlen; j++)  // or ret_variant_row_elements
        {
        printf("\n====> Row element = %d\n", j );
        // Step through each column element from the row.
        for (i = 0; i < variant_array_collen; i++)  // < ret_variant_field_elements
            {
            printf("|Column element| = %d\n", i );
            // Access each element of tag_VARIANT in variant_array[n][n]...
            // Use this to select the correct usage of the returned data.
            switch (variant_array[j][i].type)
                {
                case IS_NULL:
                    // Do stuff for NULL pointer, using variant_array[n].value.vval
                    // This will denote an unused array element. It is possible to
                    // use this data structure were we test for null as an empty element
                    // or as and empty type
                    printf("NULL=%p,", variant_array[j][i].value.vval);
                    break;
                case IS_INTEGER:
                    // Do stuff for INTEGER, using variant_array[n].value.ival
                    printf("INTEGER=%d,", variant_array[j][i].value.ival);
                    break;
                case IS_FLOAT:
                    // Do stuff for REAL, using variant_array[n].value.rvar
                    printf("REAL=%f,", variant_array[j][i].value.rval);
                    break;
                case IS_TEXT:
                    // Do stuff for TEXT, using variant_array[n].value.tval
                    printf("TEXT=%s,", variant_array[j][i].value.tval);
                    break;
                case IS_BLOB:
                    // Do stuff for BLOB, using variant_array[n].value.bvar
                    // C cannot determin the length of a binary (byte) array, so
                    // the length of array must always be tracked separately.
                    bdata_len2 = variant_array[j][i].value.bval.blen;
                    // Debug test.
                    printf("byte len = %d\n", bdata_len2);
                    if(16 == bdata_len2)
                        {
                        printf("Returned bytes same length as original bytes.\n");
                        }
                    printf("BLOB = {");
                    for(x = 0; x < bdata_len2; x++)
                        {
                        //printf("0x%02X,", variant_array[j][i].value.bval.bdata[x]);  // As hexadecimal.
                        printf("%d,", variant_array[j][i].value.bval.bdata[x]);  // As decimal.
                        }
                    printf("\b}\n");  // '\b' is a backspace to remove the last ','
                    printf("BLOB = {");
                    for(x = 0; x < bdata_len2; x++)
                        {
                        //printf("0x%02X,", variant_array[j][i].value.bval.bdata[x]);  // As hexadecimal.
                        printf("0x%02x,", variant_array[j][i].value.bval.bdata[x]);  // As decimal.
                        }
                    printf("\b}\n");  // '\b' is a backspace to remove the last ','

                    printf("BLOB = {");
                    for(x = 0; x < bdata_len2; x++)
                        {
                        //printf("0x%02X,", variant_array[j][i].value.bval.bdata[x]);  // As hexadecimal.
                        printf("%d,", bin_data[x]);  // As decimal.
                        }
                    printf("\b}\n");  // '\b' is a backspace to remove the last ','

                    // Debug compare test.
                    int return_memcmp = memcmp( bin_data, variant_array[j][i].value.bval.bdata, bdata_len2);
                    printf("memcmp= %d\n", return_memcmp);
                    if (0 == memcmp( bin_data, variant_array[j][i].value.bval.bdata, bdata_len2))
                        {
                        printf("BLOB data is the same as the original.\n");
                        }

                    // Sql Hex entry. Use SQLite DB manager to confirm.
                    //0000  ff d8 ff e2 02 1c 49 43 43 5f 50 52 4f 46 49 4c  ......ICC_PROFIL

                    break;
                default:
                    // Report an error, this shouldn't happen!
                    printf("##default=%d##\n", variant_array[j][i].type);
                    break;
                }
            printf("\n");
            }
        printf("\n");
        }

    // Free the dynamic memory!
    for(i = 0; i < variant_array_rowlen; i++)  // or variant_array_row_elements
        {
        free(variant_array[i]);
        }
    free(variant_array);

    printf("===========================================\n");
    */

//=============================================================================
    /*
    // Test our original "Hrs_worked_Tracker" TEXT table with retrieve all data types.
    // This if from the first TEXT only table examples.

    // Retrieve current table name and column number.

    char *db_table_namex2 = "Hrs_worked_Tracker";

    // Get number of columns in a named table.
    //int number_cols_retx = 0;  // Previously defined.
    number_cols_retx = 0;
    err_return = db_get_table_number_cols(file_name, db_table_namex2, &number_cols_retx);
    if (err_return == 0)
        {
        printf("Could not retrieve table %s column number from %s.\n", db_table_namex2, file_name);
        }
    else if ( err_return == 1)
        {
        printf("Retrieved table %s column number from %s.\n", db_table_namex2, file_name);
        }
    else  // == -1
        {
        printf("There was an unknown error.\n");
        }

    printf("\nTable number of columns:%d\n", number_cols_retx);

    printf("===========================================\n");

    // Retrieve current table rows number.
    //int number_rows_retx = 0;  // Previously defined.
    number_rows_retx = 0;
    err_return = db_get_table_number_rows(file_name, db_table_namex2, &number_rows_retx);
    if (err_return == 0)
        {
        printf("Could not retrieve table %s row number from %s.\n", db_table_namex2, file_name);
        }
    else if ( err_return == 1)
        {
        printf("Retrieved table %s row number from %s.\n", db_table_namex2, file_name);
        }
    else  // == -1
        {
        printf("There was an unknown error.\n");
        }

    printf("\nTable number of rows:%d\n", number_rows_retx);

    printf("===========================================\n");


    // Retrieve all table data ("Hrs_worked_Tracker")
    // Previously defined int variant_array_rowlen;int variant_array_collen
    variant_array_rowlen = number_rows_retx;  // Always check the most recent number of rows in the table
    variant_array_collen = number_cols_retx + 1;  // The number of columns + 1 for rowid

    // Track the returned number of used elements in the array.
    // Previously defined int ret_variant_field_elements; int ret_variant_row_elements
    ret_variant_field_elements = 0;
    ret_variant_row_elements = 0;

    // Create 2D dynamic array using number rows and columns
    // tagVARIANT structure is declared in example_sql3.h
    //tagVARIANT **variant_array = NULL;
    // Check if name variant_array is being redefined after free()
    variant_array = (tagVARIANT **) malloc(variant_array_rowlen * sizeof(tagVARIANT*));
    for(i = 0; i < variant_array_rowlen; i++)
        {
        variant_array[i] = (tagVARIANT *) malloc(variant_array_collen * sizeof(tagVARIANT));  // 10 -> variant_array_collen
        }

    // Initialise with zero values. (Later values will be assigned to each element)
    // Note: The char/byte lengths are currently static with a MAX length
    // of 30720 bytes. For larger strings or byte arrays change the tagVARIANT
    // structure declaration to *tval, and *bdata.
    // Then re/malloc() here using the correct returned length of the TEXT/BOLB
    // Columns from the database table.
    for(i = 0; i < variant_array_rowlen; i++)
        {
        for(j = 0; j < variant_array_collen; j++)
            {
            variant_array[i][j].type = 0;
            variant_array[i][j].value.vval = NULL;
            variant_array[i][j].value.ival = 0;
            variant_array[i][j].value.rval = 0.0;
            strcpy(variant_array[i][j].value.tval, "");
            variant_array[i][j].value.bval.blen = 0;
            variant_array[i][j].value.bval.bdata[0] = 0;
            }
        }

    // More universal query function for mixed data types.
    // Note: The variant_array_collen, variant_array_rowlen are not currently
    // used in this function call as they are calculated within the function.
    err_return = db_list_table_all_types(file_name, db_table_namex2, variant_array, variant_array_collen, variant_array_rowlen, &ret_variant_field_elements, &ret_variant_row_elements);
    if (err_return == 0)
        {
        printf("Could not retrieve %s table data from %s.\n", db_table_namex2, file_name);
        }
    else if ( err_return == 1)
        {
        printf("Retrieved table %s data from %s.\n", db_table_namex2, file_name);
        }
    else  // == -1
        {
        printf("There was an unknown error.\n");
        }

    // Test the size of returned number of row elements against our array[n]
    // DEBUG truth test for memory leaks/off by 1/ buffer over runs.
    if (variant_array_rowlen < ret_variant_row_elements)
        {
        printf("Error! Dynamic array is too small for number of rows.\n");
        }
    else if (variant_array_rowlen > ret_variant_row_elements)
        {
        printf("Good! Dynamic array is lager than number of rows.\n");
        }
    else if (variant_array_rowlen == ret_variant_row_elements)
        {
        printf("Good! Dynamic array same size as number of rows.\n");
        }
    else
        {
        printf("Unknown error!\n");  // should never occur
        }

    // Print all mixed data types returned from the table. Each column is sorted
    // to the relevant type by testing [][].type
    printf("Print mixed table data from variant_array.\n");
    // To DEBUG test against the original length of the array of VARIANT (bin_data_len = 16)
    //int bdata_len2 = 0;
    //int x = 0;
    // Step through each row element from the array.
    for (j = 0; j < variant_array_rowlen; j++)  // or ret_variant_row_elements
        {
        printf("\n====> Row element = %d\n", j );
        // Step through each column element from the row.
        for (i = 0; i < variant_array_collen; i++)  // < ret_variant_field_elements
            {
            printf("|Column element| = %d\n", i );
            // Access each element of tag_VARIANT in variant_array[n][n]...
            // Use this to select the correct usage of the returned data.
            switch (variant_array[j][i].type)
                {
                case IS_NULL:
                    // Do stuff for NULL pointer, using variant_array[n].value.vval
                    // This will denote an unused array element. It is possible to
                    // use this data structure were we test for null as an empty element
                    // or as and empty type
                    printf("NULL=%p,", variant_array[j][i].value.vval);
                    break;
                case IS_INTEGER:
                    // Do stuff for INTEGER, using variant_array[n].value.ival
                    printf("INTEGER=%d,", variant_array[j][i].value.ival);
                    break;
                case IS_FLOAT:
                    // Do stuff for REAL, using variant_array[n].value.rvar
                    printf("REAL=%f,", variant_array[j][i].value.rval);
                    break;
                case IS_TEXT:
                    // Do stuff for TEXT, using variant_array[n].value.tval
                    printf("TEXT=%s,", variant_array[j][i].value.tval);
                    break;
                case IS_BLOB:
                    // Do stuff for BLOB, using variant_array[n].value.bvar
                    // C cannot determin the length of a binary (byte) array, so
                    // the length of array must always be tracked seperately.
                    bdata_len2 = variant_array[j][i].value.bval.blen;
                    // Debug test.
                    printf("byte len = %d\n", bdata_len2);
                    printf("BLOB = {");
                    for(x = 0; x < bdata_len2; x++)
                        {
                        //printf("0x%02X,", variant_array[j][i].value.bval.bdata[x]);  // As hexadecimal.
                        printf("%d,", variant_array[j][i].value.bval.bdata[x]);  // As decimal.
                        }
                    printf("\b}\n");  // '\b' is a backspace to remove the last ','
                    printf("BLOB = {");
                    for(x = 0; x < bdata_len2; x++)
                        {
                        //printf("0x%02X,", variant_array[j][i].value.bval.bdata[x]);  // As hexadecimal.
                        printf("0x%02x,", variant_array[j][i].value.bval.bdata[x]);  // As decimal.
                        }
                    printf("\b}\n");  // '\b' is a backspace to remove the last ','

                    break;
                default:
                    // Report an error, this shouldn't happen!
                    printf("##default=%d##\n", variant_array[j][i].type);
                    break;
                }
            printf("\n");
            }
        printf("\n");
        }

    // Free the dynamic memory!
    for(i = 0; i < variant_array_rowlen; i++)  // or variant_array_row_elements
        {
        free(variant_array[i]);
        }
    free(variant_array);

    printf("===========================================\n");
    */

    //========================================================================<<

    //========================================================================>>
    // ====>> Do insert and retrieve from mixed data types table. ===========>>

    /*
    // You will need this table name available to all of the following examples.
    char *db_table_mixed = "DATA_Mixed";  // Table with mixed data types.
    char sql_table_fields[1024] = {'\0'};


    // Note: REAL == SQLITE_FLOAT == IS_FLOAT == 2 (int constants)
    sprintf(sql_table_fields, "CREATE TABLE IF NOT EXISTS %s \
                              (Date TEXT\
                              , Week INTEGER\
                              , Employee_ID INTEGER\
                              , Name INTEGER\
                              , Avatar BLOB\
                              , Monday INTEGER\
                              , Tuesday INTEGER\
                              , Wednesday INTEGER\
                              , Thursday INTEGER\
                              , Friday INTEGER);", db_table_mixed);

    // Create mixed data table (empty).
    err_return = db_table_create(file_name, sql_table_fields);
    if (err_return == 0)
        {
        printf("Table %s could not be created in %s.\n", db_table_namex2, file_name);
        }
    else if ( err_return == 1)
        {
        printf("Table %s was successfully created in %s.\n", db_table_namex2, file_name);
        }
    else
        {
        printf("There was an unknown error %s.\n", db_table_namex2 );
        }
    printf("===========================================\n");


    // read image file Tux avatar for BLOB.
    char *file_tux = "Tux.jpg";  // 16,756 bytes

    int chars_Total = 0;
    unsigned char *bin_avatar = NULL;

    FILE *fp_tux = fopen(file_tux, "rb");// Open file for read ops.
    if(fp_tux == NULL)// Test if file open success.
        {
        printf("Failed to open file %s!\n", file_tux);
        //return 0;
        }
    else
        {
        // For obtaining a byte count. aka number of characters in a file.
        fseek(fp_tux, 0, SEEK_END);  // Set pointer to end of file.
        chars_Total = ftell(fp_tux);  // get counter value.
        rewind(fp_tux);  // Set pointer back to the start of the file.
        printf("chars_Total = %d\n", chars_Total);

        // Create a dynamic array of size chars_Total.
        bin_avatar = (unsigned char*)malloc(chars_Total * sizeof(unsigned char));
        if (bin_avatar != NULL)
            {
            // Read the binary data into the array.
            fread(bin_avatar, sizeof(unsigned char), chars_Total, fp_tux);
            }

        fclose(fp_tux); // Close the file.
        }

    // DEBUG test.
    //for ( i = 0; i < chars_Total; i++)
    //    {
    //    //printf("%d,", bin_avatar[i]);
    //    printf("0x%02x,", bin_avatar[i]);
    //    //printf("%c,", bin_avatar[i]);
    //    }
    //printf("\n");

    // Converting from 8bit byte(int) (2 * oct) to char required 2 * bytes + 1 for string terminator '\0'.
    // This will create a string of hex pairs 'ffb623 ...'.
    // SQLite requires the string formatted as The x'ffb623 ...' the x will be
    // added when creating the query.
    char hex_buffer[4] = {'\0'};
    char *bin_avatar_hexstr = (char*)malloc((2 * chars_Total +1) * sizeof(char));
    strcpy(bin_avatar_hexstr, "");  // Initiate the array for strcat()
    for ( i = 0; i < chars_Total; i++)
        {
        sprintf(hex_buffer, "%02x", bin_avatar[i]);  // Convert byte to hex.
        strcat(bin_avatar_hexstr, hex_buffer);  // add each hex to string.
        }
    // DEBUG print hex string.
    //printf("\nHex string:\n");
    //printf("%s\n", bin_avatar_hexstr);

    free(bin_avatar);  // Clear the first read buffer from file read.


    // The following method inserts the data as part of the query statement.
    // NOTE! It is better in practice to use the sqlite3_bind_blob() for this.

    // NOTE: Sqlite date, time, datetime, julianday and strftime are built in
    // functions and only accessed via the querry statements.
    // TEXT == YYYY-MM-DD HH:MM:SS == datetime( ... ) (default)
    // https://www.sqlite.org/lang_datefunc.html
    // If you need Date and Time functions outside of sqlite use the C <time.h>
    // functions. The date time will need to be formatted using sprintf() to match
    // with the ISO date format YYYY-MM-DD HH:MM:SS.


    // Placing the hex string directly into the query statement. The hex string
    // must be prefixed with 'x'  x'ffb623'.
    // The beter and safer way is to use ? placeholder and sqlite3_bind_* API.

    // Note that this process is a little more complicated than python due to
    // the need for dynamic arrays and data conversions.

    // Add additional 1024 buffer space for the query statement.
    char *sql_tbl_entry = (char*)malloc(((2 * chars_Total +1) + 1024) * sizeof(char));

    sprintf(sql_tbl_entry, "INSERT INTO %s\
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
                           , x\'%s\'\
                           , 7\
                           , 5\
                           , 8\
                           , 7\
                           , 9);", db_table_mixed, bin_avatar_hexstr);


    err_return = db_insert_table_rowdata(file_name, sql_tbl_entry);
    if (err_return == 0)
        {
        printf("Row data was NOT entered into %s.\n", file_name);
        }
    else if ( err_return == 1)
        {
        printf("Row data was entered into %s.\n", file_name);
        }
    else  // == -1
        {
        printf("There was an unknown error.\n");
        }

    free(bin_avatar_hexstr);
    printf("===========================================\n");
    */


    //=========================================================================

    /*
    // Retrive the previously inserted mixed data types.

    // Get number of columns in a named table.
    //int number_cols_retx = 0;  // Previously defined.
    number_cols_retx = 0;
    err_return = db_get_table_number_cols(file_name, db_table_mixed, &number_cols_retx);
    if (err_return == 0)
        {
        printf("Could not retrieve table %s column number from %s.\n", db_table_mixed, file_name);
        }
    else if ( err_return == 1)
        {
        printf("Retrieved table %s column number from %s.\n", db_table_mixed, file_name);
        }
    else  // == -1
        {
        printf("There was an unknown error.\n");
        }

    printf("\nTable number of columns:%d\n", number_cols_retx);
    printf("===========================================\n");


    // Retrive current table rows number.
    //int number_rows_retx = 0;  // Previously defined.
    number_rows_retx = 0;
    err_return = db_get_table_number_rows(file_name, db_table_mixed, &number_rows_retx);
    if (err_return == 0)
        {
        printf("Could not retrieve table %s row number from %s.\n", db_table_mixed, file_name);
        }
    else if ( err_return == 1)
        {
        printf("Retrieved table %s row number from %s.\n", db_table_mixed, file_name);
        }
    else  // == -1
        {
        printf("There was an unknown error.\n");
        }

    printf("\nTable number of rows:%d\n", number_rows_retx);
    printf("===========================================\n");

    // Row and column count trackers.
    variant_array_rowlen = number_rows_retx;  // Always check the most recent number of rows in the table
    variant_array_collen = number_cols_retx + 1;  // The number of columns + 1 for rowid

    printf("variant_array_collen=%d\n", variant_array_collen);

    ret_variant_field_elements = 0;
    ret_variant_row_elements = 0;  // Track the number of used elements in the array

    // Create 2D dynamic array using number rows and columns
    // tagVARIANT structure is declared in example_sql3.h
    //tagVARIANT **variant_array = NULL;
    variant_array = (tagVARIANT **) malloc(variant_array_rowlen * sizeof(tagVARIANT*));
    for(i = 0; i < variant_array_rowlen; i++)
        {
        variant_array[i] = (tagVARIANT *) malloc(variant_array_collen * sizeof(tagVARIANT));  // 10 -> variant_array_collen
        }

    // Initialise with zero values. (Later values will be assigned to each element)
    // Note: The char/byte lengths are currently static with a MAX length
    // of 30720 bytes. For larger strings or byte arrays change the tagVARIANT
    // structure declaration to *tval, and *bdata.
    // Then re/malloc() here using the correct returned length of the TEXT/BOLB
    // Columns from the database table.
    for(i = 0; i < variant_array_rowlen; i++)
        {
        for(j = 0; j < variant_array_collen; j++)
            {
            variant_array[i][j].type = 0;
            variant_array[i][j].value.vval = NULL;
            variant_array[i][j].value.ival = 0;
            variant_array[i][j].value.rval = 0.0;
            strcpy(variant_array[i][j].value.tval, "");
            variant_array[i][j].value.bval.blen = 0;
            variant_array[i][j].value.bval.bdata[0] = 0;
            }
        }


    // More universal query function for mixed data types.
    // Note: The variant_array_collen, variant_array_rowlen are not currently
    // used in this function call as they are calculated within the function.
    err_return = db_list_table_all_types(file_name, db_table_mixed, variant_array, variant_array_collen, variant_array_rowlen, &ret_variant_field_elements, &ret_variant_row_elements);
    if (err_return == 0)
        {
        printf("Could not retrieve %s table data from %s.\n", db_table_namex, file_name);
        }
    else if ( err_return == 1)
        {
        printf("Retrieved table %s data from %s.\n", db_table_namex, file_name);
        }
    else  // == -1
        {
        printf("There was an unknown error.\n");
        }
    printf("===========================================\n");

    // Print all mixed data types returned from the table. Each column is sorted
    // to the relevant type by testing [][].type
    printf("Print mixed table data from variant_array.\n");
    // To DEBUG test against the original length of the array of VARIANT (bin_data_len = 16)
    //int bdata_len2 = 0;
    //int x = 0;
    // Step through each row element from the array.
    for (j = 0; j < variant_array_rowlen; j++)  // or ret_variant_row_elements
        {
        printf("\n====> Row element = %d\n", j );
        // Step through each column element from the row.
        for (i = 0; i < variant_array_collen; i++)  // < ret_variant_field_elements
            {
            printf("|Column element| = %d\n", i );
            // Access each element of tag_VARIANT in variant_array[n][n]...
            // Use this to select the correct usage of the returned data.
            switch (variant_array[j][i].type)
                {
                case IS_NULL:
                    // Do stuff for NULL pointer, using variant_array[n].value.vval
                    // This will denote an unused array element. It is possible to
                    // use this data structure were we test for null as an empty element
                    // or as and empty type
                    printf("NULL=%p,", variant_array[j][i].value.vval);
                    break;
                case IS_INTEGER:
                    // Do stuff for INTEGER, using variant_array[n].value.ival
                    printf("INTEGER=%d,", variant_array[j][i].value.ival);
                    break;
                case IS_FLOAT:
                    // Do stuff for REAL, using variant_array[n].value.rvar
                    printf("REAL=%f,", variant_array[j][i].value.rval);
                    break;
                case IS_TEXT:
                    // Do stuff for TEXT, using variant_array[n].value.tval
                    printf("TEXT=%s,", variant_array[j][i].value.tval);
                    break;
                case IS_BLOB:
                    // Do stuff for BLOB, using variant_array[n].value.bvar
                    // C cannot determin the length of a binary (byte) array, so
                    // the length of array must always be tracked separately.
                    bdata_len2 = variant_array[j][i].value.bval.blen;
                    // Debug test.
                    printf("byte len = %d\n", bdata_len2);
                    /
                    printf("BLOB = {");
                    for(x = 0; x < bdata_len2; x++)
                        {
                        //printf("0x%02X,", variant_array[j][i].value.bval.bdata[x]);  // As hexadecimal.
                        printf("%d,", variant_array[j][i].value.bval.bdata[x]);  // As decimal.
                        }
                    printf("\b}\n");  // '\b' is a backspace to remove the last ','
                    /
                    printf("BLOB = {");
                    for(x = 0; x < bdata_len2; x++)
                        {
                        //printf("0x%02X,", variant_array[j][i].value.bval.bdata[x]);  // As hexadecimal.
                        printf("0x%02x,", variant_array[j][i].value.bval.bdata[x]);  // As decimal.
                        }
                    printf("\b}\n");  // '\b' is a backspace to remove the last ','

                    break;
                default:
                    // Report an error, this shouldn't happen!
                    printf("##default=%d##\n", variant_array[j][i].type);
                    break;
                }
            printf("\n");
            }
        printf("\n");
        }


    // Free the dynamic memory!
    for(i = 0; i < variant_array_rowlen; i++)  // or variant_array_row_elements
        {
        free(variant_array[i]);
        }
    free(variant_array);
    printf("===========================================\n");
    */

//============================================================================<<

    return 0;
    }



