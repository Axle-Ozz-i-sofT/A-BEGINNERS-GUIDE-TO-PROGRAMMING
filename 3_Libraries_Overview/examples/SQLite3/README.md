# SQLite 3 Examples

SQLite is a liite database implimentatation based upon SQL.  
In the examples provided here I am making use of the SQLite shared object (dll/so) directly.  

The Python 3 implimeantation is a non standard implimentation created for the book. Please do not use it in it's current form in a commercial application.  

The Python modules from pip use a different syntax implimentation from the typical C API. This can make the Python library feel unfamilair when used along side of the C and BASIC examples.
As such I created a C API style wrapper for the shared library using the CTypes Python module. Ctypes allows for an application to make use of a shared object using runtime loading from within Python. This allows the programmer to make use of any compiled shared object (dll/so) from python as long as the licencing for the library permits it.  

I also wanted to offer an example of making use of the ctype module should you feel the need to create your own wrapper for your own shared library.  

## Directories
- Helloworld_C-FB-Py  
The basic "Hello world" test example used in the book for C, BreeBASIC and Python.  

- C  
- FreeBASIC  
- Python_3  
Extended set of examples showing the use of SQLite via the wrappers "example_sql3.py", "ozz_sql3.py" and "ozz_sql3_constants.py".  
"example_calls.py" is a test suite to show the interaction with the wrapper library. This is not an application but a full set of examples from which you can create a small application using SQLite. You can switch test calls on and off by commenting or uncommenting, but note some SQLite calls must be made in the correct order.  

If you are creating applications for SQLite then it is also worthwile having a copy of a seprate database designer application such as "DB Browser for SQLite" (sqlitebrowser).  

## Varient
Unlike Python C does not have a Varient data structure that can read and hold mixed data types. I made a quick "Varient" stuct in C as an example to show how to read mixed text and integer values from an SQL file.  

Remember these are just examples to use as a guide. If you are creating a commercial database application use the tested libraries and follow the security instructions in the SQLite guides.  
