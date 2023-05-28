# Basic Sqlite3 "Hello world" in C, FreeBASIC and Python 3  
## Used to test the SQLite 3 dll/so library installs used in book Book 3 - Libraries Overview  
  
***
Requires Sqlite3.dll (Version 3.34.1 dll.so) in the path or project directory. Tested on Windows 10 x85-64 and Ubuntu 20.04 x86-64.  
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

The C source will require sqlite3.h header in the source directory. FreeBASIC used the built in "sqlite3.bi".
