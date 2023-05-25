# Python Sqlite 3 C API binder module and Examples
## Suspended temporarily due to a naming conflict!. Source will return within a few days...
  
This is a collection of examples to illustrate a procedural method of interacting with C based shared objects (*.dll/*so). Shared libraries used in the examples use the CDECL calling convention.  
I have done this based upon the SQLite 3 binaries but the examples can also offer some insight into using your own dll/so or other libraries from Python 3.  

The Python module "sqlite3 — DB-API 2.0 interface for SQLite databases" is a far more functional and well tested library for using SQLite 3 in a python application, but strays a little from the Standard C API used natively in SQLite. As such I created a basic "Ctypes" binder so that I could continue to use the C API calls in the same way as I have for my C and FreeBASIC versions of these examples. The binder "ozz_sql3.py" follows a "procedural/functional" approach rather than the Pythonic "OOP" approach.  
  
***
Requires Sqlite3.dll (Version 3.34.1 dll.so) in the path or project directory. Windows beta, not tested on Linux.  
## Windows:  
https://www.sqlite.org/2021/sqlite-amalgamation-3340100.zip  
https://www.sqlite.org/2021/sqlite-dll-win64-x64-3340100.zip  
https://www.sqlite.org/2021/sqlite-doc-3340100.zip  
https://www.sqlite.org/2021/sqlite-tools-win32-x86-3340100.zip  

## Ubuntu:  
sudo apt update  
sudo apt install libsqlite3-dev  
/usr/lib/x86_64-linux-gnu/libsqlite3.so  
***
  
There are 4 Python modules in this repo and are organised as follows:  

* **example_calls.py**  
	A collection of basic examples that interact with the SQLite in a number of different ways. This set of examples makes use of a collection of wrapper functions contained in "example_sql3.py". Both "example_calls.py" and "example_sql3.py" are just examples and convenience wrappers to highlight some of the common methods as well as some of the different ways of interacting with the SQLite library.  
	The example SQLite transactions do not make up a functional application. You will need to un-comment or comment out sections as required to create, write or read from a database file or table.
* **example_sql3.py**  
	A collection of convenience wrapper functions to illustrate some of the methods of interacting with SQLite 3. This is a Python version of the C and FreeBASIC convenience wrappers. The main goal of these functions are to highlight the common sqlite3_prepare_(), sqlite3_step and sqlite3_finalize function set. These 3 SQLite API calls make up the common group of routines used in an SQLite 3 query.  
	The sqlite3_open and sqlite3_close API calls are used at the beginning and end of each wrapper function as a convenience to make each function stand alone. In practice we may only need to open the database file once in main() at the beginning of our application and close the database after all transactions have been completed. Although this looks like a lot of code, in an actual database application only a small fraction of the example code is required.  
	Use the wrapper functions as a guide for different tasks and methods from which you can practice designing your own database application. Some methods are shared between example_calls.py and example_sql3.py.  
  
* **ozz_sql3.py**  
	This is a Cytpes binder that allows data conversions between Pythons objects (data containers) and the C data types used in the SQLite shared library (binary dll/so). It is a very basic implementation that follows a procedural method as opposed to the Pythonic OOP method used in most Python libraries (Modules). It is not well tested and only provided as an example from which you can create your own binders if needed.  
	You can use the methods shown to link your own Python application to a Dynamic Linked Library (dll) in windows or a Shared Object (so) in Linux that you have created yourself in C, C++ or other compiled language. The Ctypes methods are based upon shared libraries using the CDECL calling convention. If you wish to use your own dll/so or a third party dll/so that uses the STDCALL convention then you may have to make some small modifications by following the Ctypes help documents.  
* **ozz_sql3_constants.py**  
	This is a file containing the common return codes and SQLite 3 constants from sqlite3.h  
	They are a required import for any source that makes use of them.

***
<br>
As well as the examples provided here I would also recommend obtaining a copy of a database management application to check your entries and queries for accuracy or errors in your code. Although SQLite does provide helpful return and error codes, the inside of the database file can be quite opaque from a programmatic perspective. Use the database management application along side of your IDE to monitor the database.  
If you are considering storing BLOBS in your database I would also recommend being familiar with a Hex Editor application as well as something like Notepad++. Byte lengths and actual bytes stored and returned must be checked for length and compared with the original data. A hex editor will allow you to format hex data from python as well as the database file. Notepad++ will allow an easy diff compare as well as total bytes compare between formatted hexadecimal.  
  
I am using the portable version of "DB Browser for SQLite"  
https://sqlitebrowser.org/dl/ for database management.  
And HXD for checking binary (Hex) entries  
https://mh-nexus.de/en/hxd/

Axle
