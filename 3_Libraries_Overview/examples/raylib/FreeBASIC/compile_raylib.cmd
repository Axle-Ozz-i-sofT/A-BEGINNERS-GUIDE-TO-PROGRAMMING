REM Makefile launcher for 64-bit Compile.
REM Please set the paths according to the compiler and source directory.
SET PATH=%PATH%;SET PATH=%PATH%;C:\Dev-WinLibs\winlibs-mingw-w64-x86_64-9.3.0-7.0.0-r3-sjlj\mingw64\bin
SET PATH=%PATH%;C:\Dev-WinLibs\winlibs-mingw-w64-x86_64-9.3.0-7.0.0-r3-sjlj\raylib\src
REM Recompile the Resource file for Windows x86-64
Rem Shared
windres raylib.dll.rc -o raylib.dll.rc.data --target=pe-x86-64
REM Static
windres raylib.rc -o raylib.rc.data --target=pe-x86-64
REM For 32-bit --target=pe-i386
REM Clean up the build environment.
mingw32-make clean
REM Using OpenGL V2.1 for VirtualBox compatibility.
mingw32-make V=1 -f Makefile PLATFORM=PLATFORM_DESKTOP RAYLIB_MODULE_RAYGUI=TRUE RAYLIB_LIBTYPE=STATIC GRAPHICS=GRAPHICS_API_OPENGL_33
REM mingw32-make V=1 -f Makefile PLATFORM=PLATFORM_DESKTOP RAYLIB_MODULE_RAYGUI=TRUE RAYLIB_LIBTYPE=SHARED GRAPHICS=GRAPHICS_API_OPENGL_21
REM mingw32-make V=1 -f Makefile PLATFORM=PLATFORM_DESKTOP RAYLIB_MODULE_RAYGUI=TRUE RAYLIB_LIBTYPE=STATIC GRAPHICS=GRAPHICS_API_OPENGL_11
pause