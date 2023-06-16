//------------------------------------------------------------------------------
// Name:        Example tagVARIENT Data type
// Purpose:     SQLite3 basic examples.
//
// Platform:    Win64, Ubuntu64
// Depends:
//
// Author:      Axle
// Created:     03/05/2023 (19/04/2023)
// Updated:     15/06/2023
// Copyright:   (c) Axle 2023
// Licence:     MIT-0 No Attribution
//------------------------------------------------------------------------------
// Notes:
// These are just some simple constructions of a naive tag VARIENT data type
// used in the multiple data types examples in examples_calls.bas and
// example_sql3.bi
// This structure is used in the last examples and is a naive implementation
// of the data structure VARIANT. VARIANT can hold multiple data types in a
// similar way to some dynamic typed scripting languages such as Python. For
// more information see MS data type VARIANT or "Tagged union",
// "Discriminated Union".
//------------------------------------------------------------------------------


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <errno.h>
//#include <math.h>
//#include <conio.h>

/*
#define SQLITE_INTEGER  1
#define SQLITE_FLOAT    2
#define SQLITE_BLOB     4
#define SQLITE_NULL     5
#ifdef SQLITE_TEXT
# undef SQLITE_TEXT
#else
# define SQLITE_TEXT     3
#endif
#define SQLITE3_TEXT     3
*/


//#define IS_INTEGER 1
//#define IS_FLOAT 2
//#define IS_TEXT 3
//#define IS_BLOB 4
//#define IS_NULL 5  // To Be tested


//=========================

// Data structures, unions ( data type VARIANT )
// Types:NULL,INTEGER,REAL,TEXT,BLOB
// sqlite3_column_type() returned values:
// affinities:
// SQLITE_NULL, SQLITE_INTEGER, SQLITE_FLOAT, SQLITE_TEXT, SQLITE_BLOB
// "tagged union"


typedef struct tagVARIANT
    {
    enum { IS_NULL = 0, IS_INTEGER = 1, IS_FLOAT = 2, IS_TEXT = 3, IS_BLOB = 5 } type;
    union
        {
        void *vval;           // NULL (pointer to) this will denote an empty element.
        int ival;             // INTEGER
        double rval;           // REAL
        char tval[2048];       // TEXT (Max string (row) length 2048)
        unsigned char bval[2048];    // BLOB (binary)
        } value;
    } tagVARIANT;

typedef struct struct_bval
    {
    int blen;
    unsigned char bdata[2048];
    } struct_bval;

typedef struct tagVARIANTX
    {
    enum { IS_NULLX = 0, IS_INTEGERX = 1, IS_FLOATX = 2, IS_TEXTX = 3, IS_BLOBX = 5 } type;
    union
        {
        void *vval;           // NULL (pointer to) this will denote an empty element.
        int ival;             // INTEGER
        double rval;           // REAL
        char tval[2048];       // TEXT (Max string (row) length 2048)
        struct_bval bval;    // BLOB int len, uchar data (binary)
        } value;
    } tagVARIANTX;


//
void Test_2D_dynamic(void);
void Test_2D_static(void);
void Test_single(void);


int main(int argc, char *argv[])
    {

    // Un-comment test functions as required.
    //Test_single();
    //Test_2D_static();
    Test_2D_dynamic();

/*
    // Some basic Byte data tests. ( Comment out when doing function tests above)
    // Testing ASCII byte data 0 to 255. Testing byte 1s, 2s compliment.
    // Testing Print format for char, int, hex
    char character[260];
    signed char s_character[260];
    unsigned char u_character[260];
    int start = 0; // -127
    int end = 255;  // 127
    int bc = 0;


    for(bc = start; bc <= end; bc++)
        {
        character[bc] = bc;
        s_character[bc] = bc;
        u_character[bc] = bc;
        }

    for(bc = start; bc <= end; bc++)
        {
        //printf("%c,", character[bc]);
        printf("%d,", character[bc]);
        //printf("0x%02X,", character[bc]);  // Hex X=uc, x=lc
        }
    printf("\n\n");

    for(bc = start; bc <= end; bc++)
        {
        //printf("%c,", s_character[bc]);
        printf("%d,", s_character[bc]);
        //printf("0x%02X,", s_character[bc]);  // Hex X=uc, x=lc
        }
    printf("\n\n");

    for(bc = start; bc <= end; bc++)
        {
        //printf("%c,", u_character[bc]);
        printf("%d,", u_character[bc]);
        //printf("0x%02X,", u_character[bc]);  // Hex X=uc, x=lc
        }
    printf("\n");
*/

    printf("Press Enter to continue...");
    getchar();
    return 0;
    }  // End main()


// tagVARIENT 2d dynamic array assign and read data.
void Test_2D_dynamic(void)
    {
    int j = 0, i = 0;
    // To use the type tagVARIANT
    // initialized to NULL, 0
    //tagVARIANT variant_array[3][10] = { .type = 0, { .vval = NULL, .ival = 0, .rval = 0.0, .tval = '\0', .bval = 0}};  // static arary of 10 tagVARIANT

    // We will have to intitialize the dynamic array in a for loop.
    int variant_array_len0 = 3;
    int variant_array_len = 10;  // Track the length of the array of VARIANT
    //int variant_array_elements = 0;  // Track the number of used elements in the array
    // Note: it is possible in a more complex structure to track the array size
    // and elements used within the structure.

    //tagVARIANT *variant_array = malloc(10 * sizeof(tagVARIANT));  // dynamic arary of 10 tagVARIANT

    // Create 2D dynamic array
    tagVARIANTX **variant_array = NULL;
    variant_array = (tagVARIANTX **) malloc(3 * sizeof(tagVARIANTX*));
    for(i = 0; i < 3; i++)
        {
        variant_array[i] = (tagVARIANTX *) malloc(10 * sizeof(tagVARIANTX));
        }

    // Assignment with zero values. This is not the same as initialization.
    // I have done this as at a minimum .type needs at least an IS_NULL = 0 and .vval = NULL for the switch case.
    // 0 terminating the char array to (string) .tval is also required for some string functions.
    for(i = 0; i < 3; i++)
        {
        for(j = 0; j < 10; j++)
            {
            variant_array[i][j].type = 0;
            variant_array[i][j].value.vval = NULL;
            variant_array[i][j].value.ival = 0;  // should not be needed
            variant_array[i][j].value.rval = 0.0;  // should not be needed
            strcpy(variant_array[i][j].value.tval, "");  // should not be needed
            variant_array[i][j].value.bval.blen = 0;
            variant_array[i][j].value.bval.bdata[0] = 0;  // should not be needed
            }
        }


    // Alternative initialisation. May fail with dynamic arrays.
    //for (j = 0; j < variant_array_len0; j++)
    //    {
    //    for (i = 0; i < variant_array_len; i++)  // < variant_array_elements
    //        {
    //        variant_array[j][i] = { .value = 0, { NULL, .ival = 0, .rval = 0.0, .tval = '\0', .bval = 0}};
    //        }
    //    }


    //===================================
    printf("DEBUG Assignment\n");
    //Populate variant_array
    for (j = 0; j < variant_array_len0; j++)
        {
        variant_array[j][0].type = IS_INTEGER;
        variant_array[j][0].value.ival = 3;
        //variant_array_elements++;

        variant_array[j][1].type = IS_TEXT;
        strcpy(variant_array[j][1].value.tval, "Hello world one");  // Max string length 2048.
        //variant_array_elements++;

        variant_array[j][2].type = IS_FLOAT;
        variant_array[j][2].value.rval = 4.85;
        //variant_array_elements++;

        variant_array[j][3].type = IS_INTEGER;
        variant_array[j][3].value.ival = 100;
        //variant_array_elements++;

        variant_array[j][4].type = IS_TEXT;
        strcpy(variant_array[j][4].value.tval, "The second TEXT");  // Max string length 2048.
        //variant_array_elements++;

        variant_array[j][5].type = IS_NULL;
        variant_array[j][5].value.vval = NULL ;  // void* 0 or void* NULL
        //variant_array_elements++;

        // [j][6], [j][7] left 0, NULL
        //case: SQLITE_BLOB
        //variant_array[j][i].type = IS_BLOB;
        //memcpy(variant_structure[num_rows][i].value.bval, sqlite3_column_blob(stmt, i), bytes_blob);
        unsigned char bin_data[16] = {0xff, 0xd8, 0xff, 0xe2, 0x02, 0x1c, 0x49, 0x43, 0x43, 0x5f, 0x50, 0x52, 0x4f, 0x46, 0x49, 0x4c};
        int bdata_len1 = 16;
        variant_array[j][6].type = IS_BLOB;
        variant_array[j][6].value.bval.blen = bdata_len1;
        for(int x = 0; x < bdata_len1; x++)
            {
            variant_array[j][6].value.bval.bdata[x] = bin_data[x];
            //variant_array[j][6].value.bval[x] = bin_data[x];
            }

        variant_array[j][8].type = IS_INTEGER;
        variant_array[j][8].value.ival = 200;
        //variant_array_elements++;

        variant_array[j][9].type = IS_INTEGER;
        variant_array[j][9].value.ival = 300;
        //variant_array_elements++;
        }

    int bdata_len2 = 0;

    printf("DEBUG Retreive\n");
    for (j = 0; j < variant_array_len0; j++)
        {
        printf("\n====== Count j = %d ======\n", j );
        for (i = 0; i < variant_array_len; i++)  // < variant_array_elements
            {
            printf("\nCount i = %d\n", i );
            // access each element of tag_VARIANT in variant_array[n]
            switch (variant_array[j][i].type)
                {
                case IS_NULL:
                    // Do stuff for NULL pointer, using variant_array[n].value.vval
                    // This will denote an unused array element. It is possible to
                    // use this data structure were we test for null as an empty element
                    // or as and empty type
                    printf("NULL= %p\n", variant_array[j][i].value.vval);  // Can use %u or %lu but not recommended.
                    break;
                case IS_INTEGER:
                    // Do stuff for INTEGER, using variant_array[n].value.ival
                    printf("INTEGER= %d\n", variant_array[j][i].value.ival);
                    break;
                case IS_FLOAT:
                    // Do stuff for REAL, using variant_array[n].value.rvar
                    printf("REAL= %f\n", variant_array[j][i].value.rval);
                    break;
                case IS_TEXT:
                    // Do stuff for TEXT, using variant_array[n].value.tval
                    printf("TEXT= %s\n", variant_array[j][i].value.tval);
                    break;
                case IS_BLOB:
                    // Do stuff for BLOB, using variant_array[n].value.bvar
                    //printf("BLOB= %u\n", variant_array[j][i].value.bval);
                    //int bytes_blob = bytes = sqlite3_column_bytes(pStmt, 0);
                    //memcpy(variant_structure[num_rows][i].value.bval, sqlite3_column_blob(stmt, i), bytes_blob);
                    //variant_array[i][j].value.bval.blen = bdata_len;
                    //variant_array[i][j].value.bval.bdata[x] = bin_data[x];

                    bdata_len2 = variant_array[j][i].value.bval.blen;
                    printf("BLOB = {");
                    for(int x = 0; x < bdata_len2; x++)
                        {
                        printf("0x%02X,", variant_array[j][i].value.bval.bdata[x]);  // As hexadecimal.
                        //printf("%d,", variant_array[j][i].value.bval[x]);  // As decimal.
                        }
                    printf("\b}");
                    //0000  ff d8 ff e2 02 1c 49 43 43 5f 50 52 4f 46 49 4c  ......ICC_PROFIL

                    break;
                default:
                    // Report an error, this shouldn't happen!
                    printf("##default= %d##\n", variant_array[j][i].type);
                    break;
                }
            }
        }

    // Free the dynamic memory!
    for(i = 0; i < 3; i++)
        {
        free(variant_array[i]);
        }
    free(variant_array);

    }  // END Function

// tagVARIENT 2d static array assign and read data.
void Test_2D_static(void)
    {
    int j = 0, i = 0;
    // To use the type tagVARIANT
    // initialized to NULL, 0
    tagVARIANT variant_array[3][10];// = {{ 0, { .vval = NULL, .ival = 0, .rval = 0.0, .tval = '\0', .bval = 0}}};  // static array of 10 tagVARIANT
    //tagVARIANT *variant_array = malloc(10 * sizeof(tagVARIANT));  // dynamic arary of 10 tagVARIANT
    // We will have to intitialize the dynamic array in a for loop.
    int variant_array_len0 = 3;
    int variant_array_len = 10;  // Track the length of the array of VARIANT
    int variant_array_elements = 0;  // Track the number of used elements in the array
    // Note: it is possible in a more complex structure to track the array size
    // and elements used within the structure.

    //===================================

    //Populate variant_array (assignment).
    for (j = 0; j < variant_array_len0; j++)
        {
        variant_array[j][0].type = IS_INTEGER;
        variant_array[j][0].value.ival = 3;
        variant_array_elements++;

        variant_array[j][1].type = IS_TEXT;
        strcpy(variant_array[j][1].value.tval, "Hello world one");  // Max string length 2048.
        variant_array_elements++;

        variant_array[j][2].type = IS_FLOAT;
        variant_array[j][2].value.rval = 4.85;
        variant_array_elements++;

        variant_array[j][3].type = IS_INTEGER;
        variant_array[j][3].value.ival = 100;
        variant_array_elements++;

        variant_array[j][4].type = IS_TEXT;
        strcpy(variant_array[j][4].value.tval, "The second TEXT");  // Max string length 2048.
        variant_array_elements++;

        variant_array[j][5].type = IS_TEXT;
        strcpy(variant_array[j][5].value.tval, "3 Another TEXT");  // Max string length 2048.
        variant_array_elements++;

        // [j][6], [j][7] left 0, NULL

        variant_array[j][8].type = IS_INTEGER;
        variant_array[j][8].value.ival = 200;
        variant_array_elements++;

        variant_array[j][9].type = IS_INTEGER;
        variant_array[j][9].value.ival = 300;
        variant_array_elements++;
        }

    for (j = 0; j < variant_array_len0; j++)
        {
        printf("\n====== Count j = %d ======\n", j );
        for (i = 0; i < variant_array_len; i++)  // < variant_array_elements
            {
            printf("\nCount i = %d\n", i );
            // access each element of tag_VARIANT in variant_array[n]
            switch (variant_array[j][i].type)
                {
                case IS_NULL:
                    // Do stuff for NULL pointer, using variant_array[n].value.vval
                    // This will denote an unused array element. It is possible to
                    // use this data structure were we test for null as an empty element
                    // or as and empty type
                    printf("NULL= %p\n", variant_array[j][i].value.vval);  // Can use %u or %lu but not recommended.
                    break;
                case IS_INTEGER:
                    // Do stuff for INTEGER, using variant_array[n].value.ival
                    printf("INTEGER= %d\n", variant_array[j][i].value.ival);
                    break;
                case IS_FLOAT:
                    // Do stuff for REAL, using variant_array[n].value.rvar
                    printf("REAL= %f\n", variant_array[j][i].value.rval);
                    break;
                case IS_TEXT:
                    // Do stuff for TEXT, using variant_array[n].value.tval
                    printf("TEXT= %s\n", variant_array[j][i].value.tval);
                    break;
                case IS_BLOB:
                    // Do stuff for BLOB, using variant_array[n].value.bvar
                    // unsigned char is also an unsigned int (aka ASCII BYTE)
                    // The print format for char is implimenation defined.
                    // The pointer is a different size to the data and is a
                    // 64-bit unsigned long long int which can differ from
                    // system to system. When retreiving the data we will cast
                    // to an 8-bit byte instead.
                    printf("BLOB= %llu\n", (unsigned long long)variant_array[j][i].value.bval);
                    break;
                default:
                    // Report an error, this shouldn't happen!
                    printf("##default= %d##\n", variant_array[j][i].type);
                    break;
                }
            }
        }
    }  // END Function

// tagVARIENT 1d static array assign and read data.
// No BLOB tests.
void Test_single(void)
    {

    // To use the type tagVARIANT
    // initialized to NULL, 0
    tagVARIANT variant_array[10] = { 0, { NULL, .ival = 0, .rval = 0.0, .tval = '\0', .bval = 0}};  // static arary of 10 tagVARIANT
    //tagVARIANT *variant_array = malloc(10 * sizeof(tagVARIANT));  // dynamic arary of 10 tagVARIANT
    // We will have to intitialize the dynamic array in a for loop.
    int variant_array_len = 10;  // Track the length of the array of VARIANT
    int variant_array_elements = 0;  // Track the number of used elements in the array
    // Note: it is possible in a more complex structure to track the array size
    // and elements used within the structure.

    //===================================

    // Populate variant_array.

    variant_array[0].type = IS_INTEGER;
    variant_array[0].value.ival = 3;
    variant_array_elements++;

    variant_array[1].type = IS_TEXT;
    strcpy(variant_array[1].value.tval, "Hello world and even some more text");  // Max string length 2048.
    variant_array_elements++;

    variant_array[2].type = IS_FLOAT;
    variant_array[2].value.rval = 6.05;
    variant_array_elements++;

    variant_array[3].type = IS_NULL;
    variant_array[3].value.vval = NULL;
    variant_array_elements++;

    //variant_array[8].type = IS_INTEGER;
    //variant_array[8].value.ival = 10;
    //variant_array_elements++;

    //variant_array[9].type = IS_INTEGER;
    //variant_array[9].value.ival = 20;
    //variant_array_elements++;

    for (int i = 0; i < variant_array_len; i++)  // < variant_array_elements
        {
        printf("\nCount i = %d\n", i );
        // access each element of tag_VARIANT in variant_array[n]
        switch (variant_array[i].type)
            {
            case IS_NULL:
                // Do stuff for NULL pointer, using variant_array[n].value.vval
                // This will denote an unused array element. It is possible to
                // use this data structure were we test for null as an empty element
                // or as and empty type
                printf("NULL= %p\n", variant_array[i].value.vval);  // Can use %u or %lu but not recommended.
                break;
            case IS_INTEGER:
                // Do stuff for INTEGER, using variant_array[n].value.ival
                printf("INTEGER= %d\n", variant_array[i].value.ival);
                break;
            case IS_FLOAT:
                // Do stuff for REAL, using variant_array[n].value.rvar
                printf("REAL= %f\n", variant_array[i].value.rval);
                break;
            case IS_TEXT:
                // Do stuff for TEXT, using variant_array[n].value.tval
                printf("TEXT= %s\n", variant_array[i].value.tval);
                break;
            case IS_BLOB:
                // Do stuff for BLOB, using variant_array[n].value.bvar
                printf("BLOB= %u\n", variant_array[i].value.bval);
                break;
            default:
                // Report an error, this shouldn't happen!
                printf("##default= %d##\n", variant_array[i].type);
                break;
            }
        }
    }  // END Function


