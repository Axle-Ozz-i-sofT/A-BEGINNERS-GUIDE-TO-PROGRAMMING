# raylib + raygui - Examples converted to FBC.  

Examples are based upon versions updated since release:  
* raylib V4.2.0  
* raygui V3.2  

Windows:  
Using RAYLIB_MODULE_RAYGUI=TRUE RAYLIB_LIBTYPE=SHARED  
This build includes raygui into the raylib shared library.  

Runtime required for examples:  
Windows:  
* libwinpthread-1.dll  
* raylib.dll  
Linking:  
* #inclib "raylibdll"  

Linux (Ubuntu):  
* raylib.so  
Linking:  
* #inclib "raylib"  


## WIITD/raylib-freebasic binders 22/10/2022  
Uses seperate raylib and raygui shared builds.  
RAYLIB_LIBTYPE=SHARED  
-DRAYGUI_IMPLEMENTATION -DBUILD_LIBTYPE_SHARED  

Runtime required for examples:  
Windows:  
* libwinpthread-1.dll  
* raylib.dll  
* raygui.dll  
Linking:  
* #inclib "raylibdll"  
* or  
* #inclib "rayguidll"  

Linux (Ubuntu):  
* raylib.so  
* raygui.so  
Linking:  
* #inclib "raylib"  
* or  
* #inclib "raygui"  

The FreeBASIC headers are the same. Just make note of the linker options between different shared library builds and adjust the #inclib "..." to suite.  


