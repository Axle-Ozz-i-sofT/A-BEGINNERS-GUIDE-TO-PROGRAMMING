''------------------------------------------------------------------------------
'' Name:        Example tagVARIENT Data type
'' Purpose:     SQLite3 basic examples.
''
'' Platform:    Win64, Ubuntu64
'' Depends:     
''
'' Author:      Axle
'' Created:     03/05/2023 (19/04/2023)
'' Updated:     15/06/2023
'' Copyright:   (c) Axle 2023
'' Licence:     MIT-0 No Attribution
''------------------------------------------------------------------------------
'' Notes:
'' These are just some simple constructions of a naive tag VARIENT data type
'' used in the multiple data types examples in examples_calls.bas and
'' example_sql3.bi
'' This structure is used in the last examples and is a naive implementation
'' of the data structure VARIANT. VARIANT can hold multiple data types in a
'' similar way to some dynamic typed scripting languages such as Python. For
'' more information see MS data type VARIANT or "Tagged union",
'' "Discriminated Union".
''------------------------------------------------------------------------------

/'
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
'/


''#define IS_INTEGER 1
''#define IS_FLOAT 2
''#define IS_TEXT 3
''#define IS_BLOB 4
''#define IS_NULL 5  // To Be tested


''=========================

'' Data structures, unions ( data type VARIANT )
'' Types:NULL,INTEGER,REAL,TEXT,BLOB
'' sqlite3_column_type() returned values:
'' affinities:
'' SQLITE_NULL, SQLITE_INTEGER, SQLITE_FLOAT, SQLITE_TEXT, SQLITE_BLOB
'' "tagged union"
type tagVARIANT_sql_type as long
enum
	IS_NULL = 0
	IS_INTEGER = 1
	IS_FLOAT = 2
	IS_TEXT = 3
	IS_BLOB = 5
end enum

union tagVARIANT_value
	vval as any ptr
	ival as long
	rval as double
	tval as zstring * 2048
	bval(0 to 2047) as ubyte
end union

type tagVARIANT
	sql_type as tagVARIANT_sql_type
	value as tagVARIANT_value
end type

''==================================

type struct_bval
	blen as long
	bdata(0 to 2047) as ubyte
end type

type tagVARIANTX_sql_type as long
enum
	IS_NULLX = 0
	IS_INTEGERX = 1
	IS_FLOATX = 2
	IS_TEXTX = 3
	IS_BLOBX = 5
end enum

union tagVARIANTX_value
	vval as any ptr
	ival as long
	rval as double
	tval as zstring * 2048
	bval as struct_bval
end union

type tagVARIANTX
	sql_type as tagVARIANTX_sql_type
	value as tagVARIANTX_value
end type

Declare Function main_procedure() As Integer
Declare Sub Test_2D_dynamic()
Declare Sub Test_2D_static()
Declare Sub Test_single()
Declare Function Con_Pause() As Integer

main_procedure()

Function main_procedure() As Integer


    'Test_single()
    'Test_2D_static()
    'Test_2D_dynamic()


    Dim As String character
    Dim As Byte s_character(256)
    Dim As UByte u_character(256)
    Dim As Integer start = 0  '' ASCII Byte range
    Dim As Integer end1 = 256  '' ASCII Byte range
    Dim As Integer bc = 0

    for bc = start To end1 -1 Step +1
        character += Chr(bc)
        s_character(bc) = bc
        u_character(bc) = bc
    Next bc

 print "Length bString = "; Len(character)

    '' NOTE: The extra characters in odd places when using Chr() as the first
    '' 32 ASCII values are terminal control characters.

    for bc = start To end1-1 Step +1
        'print Chr(character[bc]);
        print character[bc]; ",";  '' Char (aka int) Print as character %c
        'print Hex(character[bc]); ",";
    Next bc

    print ""
    print ""
    for bc = start To end1-1 Step +1
        'print Chr(s_character(bc));
        print s_character(bc); ",";
        'print Hex(s_character(bc)); ",";
    Next bc

    print ""
    print ""
    for bc = start To end1-1 Step +1
        'print Chr(u_character(bc));
        print u_character(bc); ",";
        'print Hex(u_character(bc)); ",";
    Next bc
    print ""


    Con_Pause()
    return 0
End Function


Sub Test_2D_dynamic()

    Dim As Integer j = 0
    Dim As Integer i = 0
    Dim As Integer x = 0
'' To use the type tagVARIANT
'' initialized to NULL, 0
    ''tagVARIANT variant_array[3][10] = { .type = 0, { .vval = NULL, .ival = 0, .rval = 0.0, .tval = '\0', .bval = 0}};  // static arary of 10 tagVARIANT

'' We will have to intitialize the dynamic array in a for loop.
    Dim As Integer variant_array_len0 = 3
    Dim As Integer variant_array_len = 10  '' Track the length of the array of VARIANT
    Dim As Integer variant_array_elements = 0  '' Track the number of used elements in the array
    '' Note: it is possible in a more complex structure to track the array size
    '' and elements used within the structure.

    ''tagVARIANT *variant_array = malloc(10 * sizeof(tagVARIANT));  '' dynamic arary of 10 tagVARIANT

    '' Create 2D dynamic array
    '' Note! Array is oversized by 1,1
    Dim variant_array() As tagVARIANTX
    ReDim variant_array(3,10) As tagVARIANTX


    'tagVARIANTX **variant_array = NULL;
    'variant_array = (tagVARIANTX **) malloc(3 * sizeof(tagVARIANTX*));
    'for(i = 0; i < 3; i++)
    '    {
    '    variant_array[i] = (tagVARIANTX *) malloc(10 * sizeof(tagVARIANTX));
    '    }

    /'
    ''Assignment with zero values. This is not the same as initialization.
    '' I have done this as at a minimum .type needs at least an IS_NULL = 0 and .vval = NULL for the switch case.
    '' 0 termintaing the char array to (string) .tval is also required for some string functions.
    ' Note! Array is oversized by 1,1
    for i = 0 To 3 Step +1
        for j = 0 To 10 Step +1
            variant_array(i,j).sql_type = 0
            variant_array(i,j).value.vval = 0
            variant_array(i,j).value.ival = 0  '' should not be needed
            variant_array(i,j).value.rval = 0.0  '' should not be needed
            variant_array(i,j).value.tval = ""  '' should not be needed
            variant_array(i,j).value.bval.blen = 0
            variant_array(i,j).value.bval.bdata(0) = 0  '' should not be needed
        Next j
    Next i
    '/

    /'
        for (j = 0; j < variant_array_len0; j++)
            {
            for (i = 0; i < variant_array_len; i++)  // < variant_array_elements
                {
                variant_array[j][i] = { .value = 0, { NULL, .ival = 0, .rval = 0.0, .tval = '\0', .bval = 0}};
                }
            }
    '/


''===================================
    'print "DEBUG Assignment"  '' DEBUG
    '' Populate variant_array
    for j = 0 To variant_array_len0 -1 Step +1
        variant_array(j,0).sql_type = IS_INTEGER
        variant_array(j,0).value.ival = 3
        ''variant_array_elements++;

        variant_array(j,1).sql_type = IS_TEXT
        variant_array(j,1).value.tval = "Hello world one"  '' Max string length 2048.
        ''variant_array_elements++;

        variant_array(j,2).sql_type = IS_FLOAT
        variant_array(j,2).value.rval = 4.85
        ''variant_array_elements++;

        variant_array(j,3).sql_type = IS_INTEGER
        variant_array(j,3).value.ival = 100
        ''variant_array_elements++;

        variant_array(j,4).sql_type = IS_TEXT
        variant_array(j,4).value.tval = "The second TEXT"  '' Max string length 2048.
        ''variant_array_elements++;

        variant_array(j,5).sql_type = IS_TEXT
        variant_array(j,5).value.tval = "3 Another TEXT"  '' Max string length 2048.
        ''variant_array_elements++;

        '' [j][6], [j][7] left 0, NULL
        ''case: SQLITE_BLOB
        ''variant_array[j][i].type = IS_BLOB;
        ''memcpy(variant_structure[num_rows][i].value.bval, sqlite3_column_blob(stmt, i), bytes_blob);
        'unsigned char bin_data[16] = {0xff, 0xd8, 0xff, 0xe2, 0x02, 0x1c, 0x49, 0x43, 0x43, 0x5f, 0x50, 0x52, 0x4f, 0x46, 0x49, 0x4c};
        'int bdata_len1 = 16;
        Dim As UByte bin_data(0 To ...) = {&hff, &hd8, &hff, &he2, &h02, &h1c, &h49, &h43, &h43, &h5f, &h50, &h52, &h4f, &h46, &h49, &h4c}
        Dim As Integer bin_data_len = 16
        'print "bin_data Length= "; ubound(bin_data)
        
        variant_array(j,6).sql_type = IS_BLOB
        variant_array(j,6).value.bval.blen = bin_data_len

        for x = 0 To bin_data_len -1 Step +1

            variant_array(j,6).value.bval.bdata(x) = bin_data(x)
            ''variant_array[j][6].value.bval[x] = bin_data[x];
        Next x


        variant_array(j,8).sql_type = IS_INTEGER
        variant_array(j,8).value.ival = 200
        ''variant_array_elements++;

        variant_array(j,9).sql_type = IS_INTEGER
        variant_array(j,9).value.ival = 300
        ''variant_array_elements++;
    Next j


    Dim As Integer bdata_len2 = 0

    'print "DEBUG Retreive"  '' DEBUG
    for j = 0 To variant_array_len0 -1 Step +1
        print ""
        print "====== Count j = "; j; " ======"
        for i = 0 To variant_array_len -1 Step +1  '' < variant_array_elements
            print ""
            print "Count i = "; i
            '' access each element of tag_VARIANT in variant_array[n]
            Select Case variant_array(j,i).sql_type
            case IS_NULL
                '' Do stuff for NULL pointer, using variant_array[n].value.vval
                '' This will denote an unused array element. It is possible to
                '' use this data structure were we test for null as an empty element
                '' or as and empty type
                print "NULL= "; variant_array(j,i).value.vval
                Exit Select
            case IS_INTEGER
                '' Do stuff for INTEGER, using variant_array[n].value.ival
                print "INTEGER= "; variant_array(j,i).value.ival
                Exit Select
            case IS_FLOAT
                '' Do stuff for REAL, using variant_array[n].value.rvar
                print "REAL= "; variant_array(j,i).value.rval
                Exit Select
            case IS_TEXT
                '' Do stuff for TEXT, using variant_array[n].value.tval
                print "TEXT= "; variant_array(j,i).value.tval
                Exit Select
            case IS_BLOB
                '' Do stuff for BLOB, using variant_array[n].value.bvar
                ''May need to enumerate the array bval(n)
                bdata_len2 = variant_array(j,i).value.bval.blen
                print "BLOB= {";
                For x = 0 To bdata_len2 -1
                    print "&h"; Hex(variant_array(j,i).value.bval.bdata(x)); ",";  '' As hexadecimal.
                    'print variant_array(j,i).value.bval.bdata(x); ",";  '' As decimal.
                Next x
                Print !"\b}"  '' Note: Backspace '\b' may not work in all terminals.
                
                '' 0000  ff d8 ff e2 02 1c 49 43 43 5f 50 52 4f 46 49 4c  ......ICC_PROFIL
                Exit Select
            Case Else
                '' Report an error, this shouldn't happen!
                print "##default= "; variant_array(j,i).sql_type; "##"
                Exit Select
        End Select
        Next i

    Next j


    '' Free the dynamic memory!
    Erase variant_array

End Sub  '' END Function



Sub Test_2D_static()

    Dim As Integer j = 0
    Dim As Integer i = 0
    Dim As Integer b = 0
'' To use the type tagVARIANT
'' initialized to NULL, 0
    'tagVARIANT variant_array[3][10] = { 0, { .vval = NULL, .ival = 0, .rval = 0.0, .tval = '\0', .bval = 0}};  // static arary of 10 tagVARIANT
    ' Note! Array is oversized by 1,1
    Dim As tagVARIANT variant_array(3, 10)  '' Alternative (0 To 3, 0 To 10) aka (4 col, 11 col)

''tagVARIANT *variant_array = malloc(10 * sizeof(tagVARIANT));  // dynamic arary of 10 tagVARIANT
'' We will have to intitialize the dynamic array in a for loop.
    Dim As Integer  variant_array_len0 = 3
    Dim As Integer  variant_array_len = 10  '' Track the length of the array of VARIANT
    Dim As Integer  variant_array_elements = 0  '' Track the number of used elements in the array
    '' Note: it is possible in a more complex structure to track the array size
    '' and elements used within the structure.

    /'
    variant_array(0,0).sql_type = 0
    variant_array(0,0).value.vval = 0
    variant_array(0,0).value.ival = 0
    variant_array(0,0).value.rval = 0.0
    variant_array(0,0).value.tval = ""
    variant_array(0,0).value.bval(0) = 0
    '/

''===================================
    Dim As UByte bin_data(0 To ...) = {&hff, &hd8, &hff, &he2, &h02, &h1c, &h49, &h43, &h43, &h5f, &h50, &h52, &h4f, &h46, &h49, &h4c}
    Dim As Integer bin_data_len = 16
    'print "bin_data Length= "; ubound(bin_data)


    '' Populate variant_array (assignment)
    for j = 0 To variant_array_len0 -1 Step +1

        variant_array(j,0).sql_type = IS_INTEGER
        variant_array(j,0).value.ival = 3
        variant_array_elements += 1

        variant_array(j,1).sql_type = IS_TEXT
        variant_array(j,1).value.tval = "Hello world one"  '' Max string length 2048.
        variant_array_elements += 1

        variant_array(j,2).sql_type = IS_FLOAT
        variant_array(j,2).value.rval = 4.85
        variant_array_elements += 1

        variant_array(j,3).sql_type = IS_INTEGER
        variant_array(j,3).value.ival = 100
        variant_array_elements += 1

        variant_array(j,4).sql_type = IS_TEXT
        variant_array(j,4).value.tval = "The second TEXT"  '' Max string length 2048.
        variant_array_elements += 1

        variant_array(j,5).sql_type = IS_TEXT
        variant_array(j,5).value.tval = "3 Another TEXT"  '' Max string length 2048.
        variant_array_elements += 1

        '' [j][6], [j][7] left 0, NULL

        variant_array(j,8).sql_type = IS_INTEGER
        variant_array(j,8).value.ival = 200
        variant_array_elements += 1

        variant_array(j,9).sql_type = IS_INTEGER
        variant_array(j,9).value.ival = 300
        variant_array_elements += 1
    Next j


    for j = 0 To variant_array_len0 -1 Step +1
        print ""
        print "====== Count j = "; j; " ======"
        for i = 0 To variant_array_len -1 Step +1  '' < variant_array_elements
            print ""
            print "Count i = "; i
            '' access each element of tag_VARIANT in variant_array[n]
            Select Case variant_array(j,i).sql_type
            case IS_NULL
                '' Do stuff for NULL pointer, using variant_array[n].value.vval
                '' This will denote an unused array element. It is possible to
                '' use this data structure were we test for null as an empty element
                '' or as and empty type
                print "NULL= "; variant_array(j,i).value.vval
                Exit Select
            case IS_INTEGER
                '' Do stuff for INTEGER, using variant_array[n].value.ival
                print "INTEGER= "; variant_array(j,i).value.ival
                Exit Select
            case IS_FLOAT
                '' Do stuff for REAL, using variant_array[n].value.rvar
                print "REAL= "; variant_array(j,i).value.rval
                Exit Select
            case IS_TEXT
                '' Do stuff for TEXT, using variant_array[n].value.tval
                print "TEXT= "; variant_array(j,i).value.tval
                Exit Select
            case IS_BLOB
                '' Do stuff for BLOB, using variant_array[n].value.bvar
                ''May need to enumerate the array bval(n)
                print "BLOB= ";
                For b = 0 To bin_data_len -1
                    print "&h"; Hex(variant_array(j,i).value.bval(b)); ",";
                Next b
                Print ""
                Exit Select
            Case Else
                '' Report an error, this shouldn't happen!
                print "##default= "; variant_array(j,i).sql_type; "##"
                Exit Select
        End Select
        Next i

    Next j


End Sub



Sub Test_single()


'' To use the type tagVARIANT
'' initialized to NULL, 0
    'tagVARIANT variant_array[10] = { 0, { NULL, .ival = 0, .rval = 0.0, .tval = '\0', .bval = 0}};  '' static arary of 10 tagVARIANT
    Dim As tagVARIANT variant_array(10)

    /'
    variant_array(0).sql_type = 0
    variant_array(0).value.vval = 0
    variant_array(0).value.ival = 0
    variant_array(0).value.rval = 0.0
    variant_array(0).value.tval = ""
    variant_array(0).value.bval(0) = 0
    '/
    
    
''tagVARIANT *variant_array = malloc(10 * sizeof(tagVARIANT));  // dynamic arary of 10 tagVARIANT
'' We will have to intitialize the dynamic array in a for loop.
    Dim As Integer variant_array_len = 10  '' Track the length of the array of VARIANT
    Dim As Integer variant_array_elements = 0  '' Track the number of used elements in the array
    '' Note: it is possible in a more complex structure to track the array size
    '' and elements used within the structure.
    Dim As Integer i = 0
    Dim As Integer b = 0

''===================================
    Dim As UByte bin_data(0 To ...) = {&hff, &hd8, &hff, &he2, &h02, &h1c, &h49, &h43, &h43, &h5f, &h50, &h52, &h4f, &h46, &h49, &h4c}
    Dim As Integer bin_data_len = 16
    'print "bin_data Length= "; ubound(bin_data)

''Polpulate variant_array

    variant_array(0).sql_type = IS_INTEGER
    variant_array(0).value.ival = 3
    variant_array_elements += 1
'print variant_array(0).value.ival

    variant_array(1).sql_type = IS_TEXT
    variant_array(1).value.tval = "Hello world and even some more text"  '' Max string length 2048.
    variant_array_elements += 1

    variant_array(2).sql_type = IS_FLOAT
    variant_array(2).value.rval = 6.05
    variant_array_elements += 1

    variant_array(8).sql_type = IS_INTEGER
    variant_array(8).value.ival = 10
    variant_array_elements += 1

    variant_array(9).sql_type = IS_BLOB
    For b = 0 To bin_data_len -1
        variant_array(9).value.bval(b) = bin_data(b)
    Next b
    variant_array_elements += 1


    for i = 0 To variant_array_len -1 Step +1  '' < variant_array_elements
        print ""
        print "Count i = "; i
        print "sql_type= "; variant_array(i).sql_type
        '' access each element of tag_VARIANT in variant_array[n]
        Select Case (variant_array(i).sql_type)
            case IS_NULL
                '' Do stuff for NULL pointer, using variant_array[n].value.vval
                '' This will denote an unused array element. It is possible to
                '' use this data structure were we test for null as an empty element
                '' or as and empty type
                print "NULL= "; variant_array(i).value.vval
                Exit Select
            case IS_INTEGER
                '' Do stuff for INTEGER, using variant_array[n].value.ival
                print "INTEGER= "; variant_array(i).value.ival
                Exit Select
            case IS_FLOAT
                '' Do stuff for REAL, using variant_array[n].value.rvar
                print "REAL= "; variant_array(i).value.rval
                Exit Select
            case IS_TEXT:
                '' Do stuff for TEXT, using variant_array[n].value.tval
                print "TEXT= "; variant_array(i).value.tval
                Exit Select
            case IS_BLOB:
                '' Do stuff for BLOB, using variant_array[n].value.bvar
                ''May need to enumerate the array bval(n)
                print "BLOB= ";
                For b = 0 To bin_data_len -1
                    print "&h"; Hex(variant_array(i).value.bval(b)); ",";
                Next b
                Print ""
                Exit Select
            Case Else
                '' Report an error, this shouldn't happen!
                print "##default= "; variant_array(i).sql_type; "##"
                Exit Select
        End Select
    Next i

End Sub


' Console Pause (GetKey version)
Function Con_Pause() As Integer
    Dim As Long dummy
    Print !"\nPress any key to continue..."
    dummy = Getkey
    Return 0
End Function














