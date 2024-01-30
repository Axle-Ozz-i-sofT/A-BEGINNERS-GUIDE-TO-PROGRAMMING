# MPV Examples

MPV can be used as both a CLI interface or as a dynamic library. Both examples are shown.  
NOTE: The source uses the media file "Cascading_water.mp4" in the procect root directory.  
You can use any *.mp4 media file that you choose, or you can download the original used in the source from here:  

https://www.pexels.com/video/a-strong-current-of-a-river-water-cascading-downward-3706265/  

A Strong Current Of A River Water Cascading Downward  

https://www.pexels.com/license/  


pexels-street-donkey-3706265-1920x1080-30fps~1.mp4  
Cascading_water.mp4  

---
## CLI  
The CLI source, MPV executables and video.mp4 all go in the root directory together. Linux will use the standard system library locations.

\project  
\mpv.com  
\mpv.exe  
\main_cli.[c|bas|Py]  
\Cascading_water.mp4  


## libMPV  
simple_ex.[C|BAS|Py], mpv.conf and video.mp4 will be in the project root directory.  
The library headers and .DLL files must also be in the root directory along side the output executable for these examples on the Windows platform. Linux will use the standard system library locations.  

\project  
\libmpv-2.dll/so  
\client.h 
\render.h  
\render_gl.h  
\stream_cb.h  
\simple_ex.[c|bas]  
\mpv.conf  
\Cascading_water.mp4  

The python examples come with a non standard wrapper library "mpv.py for the MPV DLL/SO files and does not use any of the python MPV modules.  
You can obtain "mpv.py" here https://github.com/jaseg/python-mpv/tree/main  
The mpv.py module will be along side of the simple_ex.py example.  
The libmpv-2.dll/so must be in the system path or alongside of the python examples.  

\py_project  
\libmpv-2.dll/so  
\mpv.py  
\simple_ex.py 
\Cascading_water.mp4  
