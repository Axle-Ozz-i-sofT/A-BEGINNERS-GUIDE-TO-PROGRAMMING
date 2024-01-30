'-------------------------------------------------------------------------------
' Name:        gp_example.bas
' Purpose:     GNUPlot Examples
'
' Platform:    Win64, Ubuntu64
' Depends:     GNUPlot V 5.2.x plus
'
' Author:      Axle
' Created:     15/04/2023
' Updated:     
' Copyright:   (c) Axle 2023
' Licence:     MIT-0 No Attribution
'-------------------------------------------------------------------------------
' Notes:
' Linux may warn about elementary icon themes. It is OK to ignore this warning.
' If you wish to install the theme you can without changing the desktop icons.
' sudo apt update
' sudo apt install elementary-icon-theme
'
' Note that I am using C style Print escape characters,
' Print !"Text\n"; Where '!' Is C escapes and ';' Removes the default new line.
'-------------------------------------------------------------------------------
' Credits:
' http://www.physics.drexel.edu/~valliere/PHYS305/Monte_Carlo/pipes/gnuplot_pipe_demo.c
'


' Test if Windows or Unix OS
#ifdef __FB_WIN32__
#define OS_Windows 1  ' 1 = True (aka Bool)
#define OS_Unix 0
#endif

#ifdef __FB_UNIX__'__FB_LINUX__
' TODO
#define OS_Unix 1
#define OS_Windows 0  ' 0 = False (aka Bool)
#endif

#include "file.bi"

#Ifndef NULL
#Define NULL 0
#Endif

'' PI is not guaranteed to be defined in all standard libraries
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

' Define extra functions so we can place them at the bottom of the page.
Declare Function Con_Clear() As Integer  ' Cross platform clear console screen
Declare Function Con_Pause() As Integer  ' Cross platform console pause until Enter. GetKey Version
Declare Sub Clear_Stdin()  ' Cross platform safe get character (clears the input buffer)

'' Basic GNUPlot examples showing script, command line and pipes commands
Declare Sub line_plot_file()
Declare Sub  line_plot_inline()

Declare Sub sine_plot_inline()

Declare Function circle_file() As Integer
Declare Sub circle_piped()

Declare Function moving_circles_data() As Integer  ' Create data file
Declare Function plot_moving_circles() As Integer  ' Plot data file

Declare Function plot_moving_circles_2() As Integer  ' Plot all with pipe.

Declare Function sine_test_2() As Integer
Declare Function image_load_1() As Integer
Declare Function bar_graph_1() As Integer

Declare Function main_procedure() As Integer

main_procedure()

Function main_procedure() As Integer  ' Main procedure
    
    line_plot_file()
    'line_plot_inline()
    
    'sine_plot_inline()
    
    'circle_file()
    'circle_piped()
    
    '' the following 2 functions must both be uncommented.
    'moving_circles_data()  ' Create data file. Only required once.
    'plot_moving_circles()  ' Plot data file.
    
    'plot_moving_circles_2()  ' Plot all with pipe.
    
    'sine_test_2()  ' another more compiles sine plot example.
    
    'image_load_1()  ' Loads an image with a scale plot overlay.
    'bar_graph_1()
    
    Con_Pause() ' DEBUG Pause
    Return 0
End Function  ' END main_procedure <---


'' Basic test using a script
Sub line_plot_file()
    ' A very simple line graph with lines and points.
    ' We can open a command instance with system(), call gnuplot with the
    ' argument of our file name with the plot commands.
    ' This is the most simple way to call a gnuplot script, but all of the
    ' control for gnuplot exists entirely within the script and associated
    ' data files. We cannot offer more commands to gnuplot.
    ' The gnuplot script has been created and saved separately from our
    ' application.
    #ifdef __fb_win32__ ' Windows 32-bit and 64-bit
    'Shell !".\\gnuplot\\bin\\gnuplot.exe linepoints.gp"  ' With C escape characters.
    Shell ".\gnuplot\bin\gnuplot.exe linepoints.gp"  ' FB Strings.
    #endif
    #ifdef __fb_unix__'__FB_LINUX__
    Shell "gnuplot linepoints.gp"
    #endif
End Sub

'' Basic test using a pipe.
Sub line_plot_inline()
    
    '' plot points (data) The same data from linepoints.gp
    Dim As Integer x(6) => { 2015, 2016, 2017, 2018, 2019, 2020 }
    Dim As Integer y(6) => { 344, 543, 433, 232, 212, 343 }
    
    '' Path to gnuplot. -persist will keep the gnuplot window open and it will
    '' need to be closed manually. In Windows we can send the exit or quit
    '' command to close a persistent window, but this does not work under Linux.
    #ifdef __fb_win32__ ' Windows 32-bit and 64-bit
    Dim As String filename = !".\\gnuplot\\bin\\gnuplot.exe -persist"
    #endif
    #ifdef __fb_unix__'__FB_LINUX__
    Dim As String filename = !"gnuplot -persist"
    #endif
    
    '' In this example we are going to open a one way pipe to gnuplot where
    '' we can send commands and data directly from our application. No
    '' script file is used as all of the commands are sent via the pipe file in
    '' memory. GNUPlot is designed to poll this memory file for new commands.
    'FILE* fp_gnuplot;  '' Open named pipe as gnuplot
    Dim As Integer fp_gnuplot = Freefile  '' Get a file handle
    
    'fp_gnuplot = popen(filename, "w");  '' Open GNUPlot.exe CLI interface with write pipe.
    Open Pipe filename For Output As #fp_gnuplot  ' Output is the equivalent of 'w' in c.
    If fp_gnuplot <> NULL Then
        '' We are sending the plot and format commands as in "inline" command to
        '' gnuplot via the pipe we opened above.
        '' Everything after this format line is data.
        ''Print #gnuplot, !"plot '-' u 1:2 t 'Price' w lp\n";  ' Shorthand version of the commands below.
        Print #fp_gnuplot, !"plot '-' using 1:2 title 'Price' with linespoints\n";
        
        '' Loop through the data contained in the 2 arrays sending it to gnuplot.
        '' If you notice the '\n' character the data will be formatted as 2
        ''columns of x, y data as in the previous script example. If you open
        '' linepoints.gp in a text editor you can see the construction of the
        '' formatting and data sent to gnuplot.
        Dim i As Integer
        For i = 0 To 5 Step 1
            'fprintf(fp_gnuplot,"%d %d\n", x[i], y[i]);
            Print #fp_gnuplot,x(i), !"\t", y(i)  ' Print adds a new lin\n unless ;
        Next i
        
        '' The option -e "command" may be used to force execution of a gnuplot
        '' command once the data has been sent.
        '' The letter "e" at the end terminates data entry.
        Print #fp_gnuplot, !"e\n";
        Print !"Click Ctrl+d to quit...\n";  ' Writes to stdout
        Fileflush(fp_gnuplot)  '' Push all commands and data to the pipe file.
    End If
    
    Print !"1st Mouse Click or key press to clear the graph...\n";
    Print #fp_gnuplot, !"pause mouse any\n";  '' Pause until mouse click or key press.
    Fileflush(fp_gnuplot)
    
    Print #fp_gnuplot, !"clear\n";  '' Clear the gnuplot window.
    Print  !"2nd Mouse Click or key press to quit...\n";
    Print #fp_gnuplot, !"pause mouse any\n";
    Fileflush(fp_gnuplot)
    
    /'
    gnuplot> help set Print
    The `set Print` Command redirects the Output of the `Print` Command To a file.
    
    Syntax:
    set Print
    set Print "-"
    set Print "<filename>" [Append]
    set Print "|<shell_command>"
    set Print $datablock [Append]
    
    `set Print` With no parameters restores Output To <STDERR>.  The <filename>
    "-" means <STDOUT>. The `Append` flag causes the file To be opened in Append
    mode.  A <filename> starting With "|" Is opened As a Pipe To the
    <shell_command> On platforms that support piping.
    
    The destination For `Print` commands can also be a named Data block. Data
    block names start With '$', see also `inline data`.
    '/
    
    '' exit gnuplot can be used with Windows OS when gnuplot is called with -persist
    '' This does not work under Linux.
    Print #fp_gnuplot, !"exit gnuplot\n";
    Fileflush(fp_gnuplot)  '' This last fflush is really all that is required.
    Close #fp_gnuplot  ' Close the pipe to gnuplot.
    '' No messages are returned to the console until GNUPlot has closed.
    
End Sub

'' basic sine test using pipe
Sub sine_plot_inline()
    '' Using and writing to a pipe is very similar to writing to a file on disk.
    '' Although I have used a string literal in popen() it is preferred to use
    '' a variable as shown in other examples.
    '' Remember to try different commands such as -persist to see the effects
    '' on different systems. You can use the examples as a base to experiment with.
    Dim As Integer fp_gnuplot = Freefile  '' Get a file handle
    #ifdef __fb_win32__ ' Windows 32-bit and 64-bit
    Open Pipe ".\\gnuplot\\bin\\gnuplot.exe -persist" For Output As #fp_gnuplot
    #endif
    #ifdef __fb_unix__'__FB_LINUX__
    Open Pipe "gnuplot" For Output As #fp_gnuplot
    #endif
    
    '' Plot a very simple sine wave. Gnuplot will use auto formatting and scales
    '' if none are set.
    If fp_gnuplot <> NULL Then
        '' This will use an auto x scale of -10 to +10 and plot the y return
        '' of x for each increment. In a real world plot we would be providing
        '' a well defined x and y scale minimum and maximum using the "set" command.
        Print #fp_gnuplot, !"plot sin(1/x)\n";  ' x, y are gnuplot graph variables
    End If
    
    '' The letter "e" at the start of the first column terminates data entry
    ''Print #fp_gnuplot, !"e\n";  ' the end of data is not always used.
    Fileflush(fp_gnuplot)
    
    '' Pause
    Print !"Mouse Click or key press to quit...\n";
    Print #fp_gnuplot, !"pause mouse any\n";  ' pause gnuplot
    Fileflush(fp_gnuplot)
    
    '' Exit Windows gnuplot -persist
    Print #fp_gnuplot, !"exit gnuplot\n";  ' exit/quit gnuplot
    Fileflush(fp_gnuplot)
    Close #fp_gnuplot  ' close pipe to gnuplot.
    
End Sub

'' The following performs 2 separate tasks. The first part will ask
'' for the radius and center coordinates of a circle. It will create a formatted
'' script file along with the x.y data calculated in our application.
''
'' At the very end we will use system() to run gnuplot with the circle.txt file
'' as the argument. We are not using a pipe in this example
''
'' I recommend comparing the created "circle.txt" against the code lines below.
'' Plot a circle, write to file then send data file to gnuplot. Save plot as PNG.
Function circle_file() As Integer
    Dim r As Double
    Dim As Double x, y, x0, y0
    
    '' Open our text file to create a gnuplot script with data.
    Dim As Integer fp = Freefile
    Open "circle.txt" For Output As #fp  ' Open the text file for writing.
    If fp = NULL Then
        Print !"Error opening file!\n";
        Return -1
    End If

        '' Basic include guard. A character input will default to 0.
        '' Test if Radius 0 or negative value.
        '' Obtain the circle dimensions, and center of circle x, y.
        Input "Enter the radius of the circle to be plotted: ", r
        '' Input has a max size of the stack in GiBs so setting a limit is preferable.
        '' Unfortunately there is no easy way to do this in pure FreeBASIC.
        '' The only way is to invoke the C standard library...
        '' Safe Input alternative...(Downside: can't read typed characters.)
        ''Print "Enter the radius of the circle to be plotted: ";
        ''r = CDbl(Input(2))  ' CDbl() converts string to double
        ''Print r
        Print r

        If ( r < 1) Then
            Print "Radius must be greater than 0. Setting Radius to 5.0."
            r = 5.0
        End If

        Print !"\nEnter the x and y-coordinates of the center:"
        Input "Enter the x-coordinates of the center: ", x0
        Input "Enter the y-coordinates of the center: ", y0
    
    '' Create our plot formatting and write it to the script.
    '' Note: Unset pause if printing to file.
    Print #fp, !"set title \"Circle plot\"\n";  ' Set the title.
    ''Print #fp, !"set term png size 600, 600\n";  ' print to file
    ''Print #fp, !"set size 1,1\n")  ' print to file
    ''Print #fp, !"set output \"image.png\"\n")  ' print to file
    
    Print #fp, !"set size square 1,1\n";  ' set the graph to square x,y and 1 to 1 ratio
    '' set size ratio 1, set size ratio 2, set size ratio 0.5, etc
    
    '' Create a margin/offset in the autoscale of the graph. at 0 offset the
    '' plot touches the graph border.
    Print #fp, !"set offsets 1, 1, 1, 1\n";
    
    '' using 1:2 selects col 1 and col 2 for data. (x, y, z | 1:2:3)
    '' 1:5 will select col 1 and col 5 for x, y data (tab delimited csv)
    Print #fp, !"plot '-' using 1:2 w l\n";  ' ( with linespoints 1)

    '' Create the polt x.y data for the circle. it is created as a 2 column
    '' TAB delimited csv format. gnuplot uses the 1:2 to read from col 1 for y
    '' and col 2 for x.
    '' Not joining back to origin. See fix below
    '' https://www.bragitoff.com/2017/08/plotting-exercises-c-gnuplot/
    For y=y0-r To y0+r Step +0.1  ' >
        x = Sqr(r*r-(y-y0)*(y-y0))+x0
        Print #fp, x, !"\t", y
    Next y
    
    For y=y0+r To y0-r Step -0.1  ' <
        x = -Sqr(r*r-(y-y0)*(y-y0))+x0
        Print #fp, x, !"\t", y
    Next y
    '' Hack to finish the circle. Connect last line to origin point.
    '' Unfortunately the last dot plot does not create a line to the origin dot
    '' of the circle so I have redrawn the first dot plot to draw the last line
    '' connection.
    y = y0-r  ' reset y to the start value.
    x = -Sqr(r*r-(y-y0)*(y-y0))+x0
    Print #fp, x, !"\t", y
    
    '' Send the last commands to the script file...
    Print #fp, !"e\n";  ' ends/finishes the 'plot' data mode.
    ''Fileflush(fp)
    
    Print #fp, !"pause mouse any\n";  ' pause until mouse or keyboard click.
    ''Fileflush(fp)
    
    '' Save a PNG image of the plot. Note the 3 separate script lines.
    '' Notice that I have changed the output from the UI terminal to a file.
    '' I have then run all of the previous commands and data this time
    '' outputting to a PNG image file with "replot".
    Print #fp, !"set terminal png\nset output \"Circle.png\"\nreplot\n";
    
    Print #fp, !"exit gnuplot\n";  ' exit gnuplot application.
    Fileflush(fp)  ' Push all data to the file.
    
    Close #fp  ' close the text/script file.
    Sleep 1 ' Allow text file to close.
    
    '' Execute gnuplot with the created text file as the script.
    '' all gnuplot commands and data from above are in the script text file.
    #ifdef __fb_win32__ ' Windows 32-bit and 64-bit
    Shell ".\\gnuplot\\bin\\gnuplot.exe -persist circle.txt"
    #endif
    #ifdef __fb_unix__'__FB_LINUX__
    Shell "gnuplot circle.txt"
    #endif
    
    '' stdout is the console, not gnupolt.
    Print !"Mouse Click or key press to quit...\n";
    
    Return 0
End Function

'' Plot a circle, write directly to gnuplot using a pipe. Save plot as PNG.
'' This is the same as the version that writes a script file except that we are
'' now sending the commands and data directly to gnuplot via a pipe.
'' This is sometime referred to as "inline programming" where the commands
'' of another language are created within our primary application and then sent
'' to the second application to be executed.
Sub circle_piped()
    
    #ifdef __fb_win32__ ' Windows 32-bit and 64-bit
    Dim As String filename = ".\\gnuplot\\bin\\gnuplot.exe"
    #endif
    #ifdef __fb_unix__'__FB_LINUX__
    Dim As String filename = "gnuplot"
    #endif
    
    Dim As Double r
    Dim As Double x, y, x0, y0
    
    '' Open a pipe "popen()" to gnuplot.
    Dim As Integer fp_gnuplot = Freefile  '' Get a file handle
    Open Pipe filename For Output As #fp_gnuplot
    
    If fp_gnuplot <> NULL Then
        '' Basic include guard. A character input will default to 0.
        '' Test if Radius 0 or negative value.
        '' Obtain the circle dimensions, and center of circle x, y.
        Input "Enter the radius of the circle to be plotted: ", r
        '' Input has a max size of the stack in GiBs so setting a limit is preferable.
        '' Unfortunately there is no easy way to do this in pure FreeBASIC.
        '' The only way is to invoke the C standard library...
        '' Safe Input alternative...(Downside: can't read typed characters.)
        ''Print "Enter the radius of the circle to be plotted: ";
        ''r = CDbl(Input(2))  ' CDbl() converts string to double
        ''Print r
        If ( r < 1) Then
            Print "Radius must be greater than 0. Setting Radius to 5.0."
            r = 5.0
        End If

        Print !"\nEnter the x and y-coordinates of the center:"
        Input "Enter the x-coordinates of the center: ", x0
        Input "Enter the y-coordinates of the center: ", y0
        
        '' Note: Unset pause if printing to file, or print after pause.
        Print #fp_gnuplot, !"set title \"Circle plot\"\n";
        ''Print #fp_gnuplot, !"set term png size 600, 600\n";  ' print to file
        ''Print #fp_gnuplot, !"set size 1,1\n";  ' print to file
        ''Print #fp_gnuplot, !"set output \"image.png\"\n";  ' print to file
        
        Print #fp_gnuplot, !"set size square 1,1\n";  ' set the graph to square x,y and 1 to 1 ratio
        '' set size ratio 1, set size ratio 2, set size ratio 0.5, etc
        
        '' Create a margin/offset in the autoscale of the graph. At 0 offset the
        '' plot touches the graph border.
        Print #fp_gnuplot, !"set offsets 1, 1, 1, 1\n";
        
        '' using 1:2 selects col 1 and col 2 for data.
        Print #fp_gnuplot, !"plot '-' using 1:2 w points pt 7 ps 1.5\n";  ' ( with linespoints) pt 0 = dot, 7 = circle
        '' https://livebook.manning.com/book/gnuplot-in-action-second-edition/chapter-9/161
        
        For y=y0-r To y0+r Step +0.1  ' >
            x = Sqr(r*r-(y-y0)*(y-y0))+x0
            Print #fp_gnuplot, x, !"\t", y
        Next y
        
        For y=y0+r To y0-r Step -0.1  ' <
            x = -Sqr(r*r-(y-y0)*(y-y0))+x0
            Print #fp_gnuplot, x, !"\t", y
        Next y
        '' hack to finish the circle. connect the last line to the origin point.
        'y = y0-r  ' reset y to the start value.
        'x = -Sqr(r*r-(y-y0)*(y-y0))+x0
        'Print #fp_gnuplot, x, y
        
        Print #fp_gnuplot, !"e\n";  ' ends/finishes the 'plot' instruction mode.
        Fileflush(fp_gnuplot)
        
        Print #fp_gnuplot, !"pause mouse any\n";  ' pause until mouse or keyboard click
        ''Fileflush(fp_gnuplot)
        
        Print !"Mouse Click or key press to save file and quit...\n";
        
        '' Save a PNG image of the plot.
        '' Notice that I have changed the output from the UI terminal to a file.
        '' I have then run all of the previous commands and data this time
        '' outputting to a PNG image file with "replot".
        Print #fp_gnuplot, !"set terminal png\nset output \"file.png\"\nreplot\n";
        
        Print #fp_gnuplot, !"exit gnuplot\n";  ' exit gnuplot application.
        Fileflush(fp_gnuplot)

    Else
        Print !"There was a problem opening GNUPlot\n";
    End If
    
    Close #fp_gnuplot  ' close pipe to gnuplot.
End Sub

'' Generate data for moving circles demo ''
'' Michel Vallieres ''
''int main( int argc, char * argv[] )  // requires compiled.exe >> output.dat
'' Rewrite the to output to file.
'' In this example we are writing only the data to a csv file. We will call the
'' data file from our inline programming via a pipe with gnuplot.
'' Sometimes we may need to plot data from another application that is exporting
'' the data in a csv format. This could be data created with excel or access
'' for example.
Function moving_circles_data() As Integer
    
    Dim As Double cx, cy, angle, ang
    Dim As Double radius, x, y
    Dim As Integer circles
    
    '' Open/create our text data file for writing.
    Dim As Integer fp = Freefile
    Open "circles.dat" For Output As #fp  ' Open the text file for writing.
    If fp = NULL Then
        Print !"Error opening file!\n";  ' Our text file for the data.
        Return -1
    End If
    
    ' Carry out the methods to plot a set of circles.
    ' You will notice that 3 columns of data are created.
    For circles = 1 To 10 Step 1
        cx = circles
        cy = 1.3 * circles
        radius = 0.5
        For angle=0 To 359 Step 1.0  ' Plots 360 points 0 to 359
            ang = M_PI*angle/180
            x = cx + radius * Cos(ang)
            y = cy + radius * Sin(ang)
            ' Note the "\t" TAB delimiter between each column.
            Print #fp, angle, !"\t", x, !"\t", y
        Next angle
        Print #fp, !"#endframe\n";  ' This is just a comment to separate the data blocks.
    Next circles
    
    Close #fp  ' close our data file "circles.dat"
    Return 0
End Function

'' Demonstration of pipe stream connection to gnuplot
'' Michel Vallieres
'' In the second part of this example we are going to supply the gnuplot formatting
'' with "inline" programming via a pipe. We will also provide the data file from
'' the above function to be plotted. If we are using a common data type we can
'' substitute circles.dat with any other data file.
Function plot_moving_circles() As Integer
    '' file pointer for pipe connection
    #ifdef __fb_win32__ ' Windows 32-bit and 64-bit
    Dim As String filename = ".\\gnuplot\\bin\\gnuplot.exe  -persist"
    #endif
    #ifdef __fb_unix__'__FB_LINUX__
    Dim As String filename = "gnuplot"
    #endif
    
    '' open pipe for writing
    Dim As Integer gnuplot_fd = Freefile  '' Get a file handle
    Open Pipe filename For Output As #gnuplot_fd  ' Output is the equivalent of 'w' in c.
    
    If gnuplot_fd = NULL Then
        Print !"Error opening pipe to gnuplot\n";
        Return -1
    End If
    
    '' Issue gnuplot commands
    Print #gnuplot_fd, !"set title \'C I R C L E S\'\n";
    Print #gnuplot_fd, !"unset key\n";
    Print #gnuplot_fd, !"set pointsize 0.5\n";
    Print #gnuplot_fd, !"plot \'circles.dat\' using 2:3 with points \n";
    
    '' flush buffer to make sure that all
    '' commands are received by gnuplot
    Fileflush(gnuplot_fd)
    
    '' 5 sec delay
    '' The child application will remain open until the exit command is given,
    '' but this may not always be the case.
    '' We could send the pause command to gnuplot instead of the C sleep() function.
    Sleep 5000
    '' or send a pause command to gnuplot instead of sleep()
    '' pause 5  // Pause for 5 seconds; pause -1 wait for Enter (carriage return)
    ''Print #gnuplot_fd, !"pause mouse any\n";  ' pause until mouse or keyboard click
    ''Fileflush(gnuplot_fd)
    
    '' make gnuplot quit
    Print #gnuplot_fd, !"exit gnuplot\n";  ' exit gnuplot
    
    '' close the pipe to gnuplot
    Close #gnuplot_fd  ' close pipe to gnuplot.
    
    Return 0
End Function

'' Demonstration of pipe stream connection to gnuplot
'' Michel Vallieres 
'' This is the same plot example as the previous function except we
'' are constructing and sending commands plus data via the pipe.
'' I am using data blocks to draw each circle sequentially
'' giving the appearance of a moving circle.
'' We can use this method in a loop to repeatedly update with new
'' data to gnuplot as it arrives in our application.
'' Topic: "inline data and datablocks"
'' http://gnuplot.info/docs_5.5/loc3521.html
Function plot_moving_circles_2() As Integer
    Dim As Double cx, cy, angle, ang
    Dim As Double radius, x, y
    Dim As Integer circles
    
    #ifdef __fb_win32__ ' Windows 32-bit and 64-bit
    Dim As String filename = ".\\gnuplot\\bin\\gnuplot.exe  -persist"
    #endif
    #ifdef __fb_unix__'__FB_LINUX__
    Dim As String filename = "gnuplot"
    #endif
    
    '' open pipe for writing
    Dim As Integer gnuplot_fd = Freefile  '' Get a file handle
    Open Pipe filename For Output As #gnuplot_fd  ' Output is the equivalent of 'w' in c.
    If gnuplot_fd = NULL Then
        Print !"Error opening pipe to gnuplot\n";
        Return -1
    End If
    
    '' Issue gnuplot commands
    Print #gnuplot_fd, !"set title \'C I R C L E S\'\n";
    Print #gnuplot_fd, !"set size ratio -1\n";
    Print #gnuplot_fd, !"set xtics 0.5\nset ytics 0.5\n\n";  ' Try 1
    Print #gnuplot_fd, !"set xrange [0:12]\n";
    Print #gnuplot_fd, !"set yrange [0:14]\n\n";
    Print #gnuplot_fd, !"unset key\n";
    Print #gnuplot_fd, !"set pointsize 0.5\n";
    '' We no longer set "plot * using 2:3 with points \n" before the data.
    
    ' Carry out the methods to plot a set of circles.
    ' You will notice that 3 columns of data are created.
    For circles = 1 To 10 Step 1
        cx = circles
        cy = 1.3 * circles
        radius = 0.5
        '' Create datablocks for each 10 circles named $data1, $data2 ...
        Print #gnuplot_fd, !"$data" & circles & !" << EOD\n";
        For angle=0 To 359 Step 1.0  ' Plots 360 points 0 to 359
            ang = M_PI*angle/180
            x = cx + radius * Cos(ang)
            y = cy + radius * Sin(ang)
            ' Note the "\t" TAB delimiter between each column.
            Print #gnuplot_fd, angle, !"\t", x, !"\t", y
        Next angle
        Print #gnuplot_fd, !"EOD\n";  ' Marks the End Of Data
        '' Now we can draw the plot using the $data[n] name.
        Print #gnuplot_fd, !"plot $data" & circles & !" using 2:3 with points\n";
        '' wait for one second and repeat with new data. Try different time values.
        Print #gnuplot_fd, !"pause 0.2\n";
        Fileflush(gnuplot_fd)  ' Push the data to gnuplot.
    Next circles
    
    '' 3 sec delay after all data is sent and plotted.
    '' Be sure to read up on the different abilities of pause.
    Print #gnuplot_fd, !"pause 3 \"waiting 3 sec...\"\n";
    
    '' make gnuplot quit
    Print #gnuplot_fd, !"exit gnuplot\n";
    
    '' flush buffer to make sure that all
    '' commands are received by gnuplot
    Fileflush(gnuplot_fd)  ' Push the data to gnuplot.
    
    '' close the pipe to gnuplot
    Close #gnuplot_fd   ' close pipe to gnuplot.
    
    Return 0
End Function

'' A slightly more complex example from example 2
Function sine_test_2() As Integer
    
    #ifdef __fb_win32__ ' Windows 32-bit and 64-bit
    Dim As String filename = ".\\gnuplot\\bin\\gnuplot.exe"
    #endif
    #ifdef __fb_unix__'__FB_LINUX__
    Dim As String filename = "gnuplot"
    #endif
    
    '' open pipe for writing
    Dim As Integer fp_gnuplot = Freefile  '' Get a file handle
    Open Pipe filename For Output As #fp_gnuplot  ' Output is the equivalent of 'w' in c.
    If fp_gnuplot = NULL Then
        Print !"Error opening pipe to gnuplot\n";
        Return -1
    End If
    
    ''Print #fp_gnuplot, !"plot sin(x) with lines\n";  ' x. y are gnuplot graph variables
    ''Print #fp_gnuplot, !"replot cos(x) with lines\n";
    
    Print #fp_gnuplot, !"set xrange [0:10]\n";  ' [-10:10]
    Print #fp_gnuplot, !"set yrange [0:1]\n";  ' [-1:1]
    
    Print #fp_gnuplot, !"p sin(x) w l, cos(x) w l\n";
    
    '' Some hints for other common gnuplot commands to experiment with.
    '' Check the gnuplot help document for more information :)
    '' x and y tics  // set the scale values, color etc
    '' unset xtics
    '' unset ytics
    ''
    '' set xtics 0, 5, 10
    '' set ytics ("bottom" 0 , "top" 1)
    
    '' labels, titles, and legends
    '' p sin(x) w l title "sine wave", cos(x) w l title "cos(x)"  // labels/legend
    ''
    '' set nokey  // removes the key legend
    '' set key top left  // change the position of the graph key.
    '' top, bottom, left, right, and center
    '' set key at 1, 0.5
    ''
    '' note that title has 2 separate uses depending upon the context of where
    '' it is issued. It can be mane title above the graph, or a title within
    '' the graph.
    '' set title "Gnuplot Test"  // Main graph title.
    ''
    '' set size square  // sets the graphic window to square
    '' line and point types + color
    '' https://www.algorithm-archive.org/contents/plotting/plotting.html
    ''
    '' p sin(x) with lines dashtype 2 linecolor rgb "black" title "sin(x)"
    '' rep cos(x) w p pt 17 lc rgb "purple" t "cos(x)"
    ''
    '' gnuplot aliases:
    '' command         alias
    '' plot            p
    '' replot          rep
    '' with lines      w l
    '' with points     w p
    '' linecolor       lc
    '' pointtype       pt
    '' title           t
    ''
    '' Outputting the plot to file
    '' set terminal pngcairo
    '' set output "check.png"
    '' set terminal pngcairo size 640, 480
    ''
    '' Data files.
    '' file_0000.dat, file_0001.dat, file_0002.dat ...
    '' Data as TAB delimited csv (Comma delimited can be set)
    '
    ' Print #fp_gnuplot, !"set multiplot\n";  ' multiple plot graphs in one page.
    
    '' Print #fp_gnuplot, !"e\n";
    Fileflush(fp_gnuplot)
    
    '' stdout writes to the OS command line console, not gnuplot.
    Print !"Mouse Click or key press to quit...\n";
    Print #fp_gnuplot, !"pause mouse any\n";
    Fileflush(fp_gnuplot)
    
    Close #fp_gnuplot  ' close pipe to gnuplot.
    Return 0
End Function

'' Load PNG image, and add scale.
Function image_load_1() As Integer
    '' Path to gnuplot
    #ifdef __fb_win32__ ' Windows 32-bit and 64-bit
    Dim As String filename = ".\\gnuplot\\bin\\gnuplot.exe"  ' -persist
    #endif
    #ifdef __fb_unix__'__FB_LINUX__
    Dim As String filename = "gnuplot"
    #endif
    
    '' open pipe for writing
    Dim As Integer fp_gnuplot = Freefile  '' Get a file handle
    Open Pipe filename For Output As #fp_gnuplot  ' Output is the equivalent of 'w' in c.
    If fp_gnuplot = NULL Then
        Print !"Error opening pipe to gnuplot\n";
        Return -1
    End If
    
    Print #fp_gnuplot, !"set arrow from 31,20 to 495,20 nohead front ls 1\n";
    '' set for [ii=0:11] arrow from 31+ii*40,35 to 31+ii*40,45 nohead front ls 1
    Print #fp_gnuplot, !"set for [ii=0:11] arrow from 31+ii*40,15 to 31+ii*40,25 nohead front ls 1\n";
    '' set number and unit as different labels in order
    '' to get a smaller distance between them
    Print #fp_gnuplot, !"set label front '0'  at  25,30 front tc ls 1\n";
    Print #fp_gnuplot, !"set label front 'cm' at  37,30 front tc ls 1\n";
    Print #fp_gnuplot, !"set label front '5'  at 225,30 front tc ls 1\n";
    Print #fp_gnuplot, !"set label front 'cm' at 237,30 front tc ls 1\n";
    Print #fp_gnuplot, !"set label front '10' at 420,30 front tc ls 1\n";
    Print #fp_gnuplot, !"set label front 'cm' at 442,30 front tc ls 1\n";
    
    '' http://www.gnuplotting.org/images-within-a-graph/
    Print #fp_gnuplot, !"set size ratio -1\n";  ' x. y are gnuplot graph variables
    Print #fp_gnuplot, !"plot 'Tropical_fish.jpg' binary filetype=jpg with rgbimage\n";
    '' "apple-logo.png" binary filetype=png origin=(1960, 165) dx=1./10 dy=1./220000 with rgbimage notitle,\
    '' #Plot the background image
    '' plot "map.png" binary filetype=png w rgbimage
    Fileflush(fp_gnuplot)
    
    ''Print #fp_gnuplot, !"e\n";
    ''fflush(fp_gnuplot);
    
    Print !"Mouse Click or key press to quit...\n";
    Print #fp_gnuplot, !"pause mouse any\n";
    Fileflush(fp_gnuplot)
    
    Close #fp_gnuplot  ' close pipe to gnuplot.
    Return 0
End Function

'' Bar graph using boxes
'' https://gnuplot.sourceforge.net/demo/fillstyle.html
Function bar_graph_1() As Integer
    '' Path to gnuplot
    #ifdef __fb_win32__ ' Windows 32-bit and 64-bit
    Dim As String filename = ".\\gnuplot\\bin\\gnuplot.exe"  ' -persist
    #endif
    #ifdef __fb_unix__'__FB_LINUX__
    Dim As String filename = "gnuplot"
    #endif
    
    '' open pipe for writing
    Dim As Integer fp_gnuplot = Freefile  '' Get a file handle
    Open Pipe filename For Output As #fp_gnuplot  ' Output is the equivalent of 'w' in c.
    If fp_gnuplot = NULL Then
        Print !"Error opening pipe to gnuplot\n";
        Return -1
    End If
    
    '' The same data from the line graph in example function 2
    Dim As Integer x(6) => { 2015, 2016, 2017, 2018, 2019, 2020 }
    Dim As Integer y(6) => { 344, 543, 433, 232, 212, 343 }
    
    ''Print #fp_gnuplot, "unset key\n");
    Print #fp_gnuplot, !"set title \"Filled boxes - bar graph\"\n";
    Print #fp_gnuplot, !"set boxwidth 0.5 relative\n";  ' fixed box width 1/2
    ''Print #fp_gnuplot, !"set style fill solid 0.5\n";  ' Solid fill box (No border)
    Print #fp_gnuplot, !"set style fill solid 0.5 border -1\n";  ' With border
    ''set colorsequence [ default | classic | podo ]
    ''Print #fp_gnuplot, !"set colors classic\n";
    
    '' Use "variable" auto colors
    '' 1:3:2:xtic(2) == x:y,color variable:x labels | boxes linecolor variable
    Print #fp_gnuplot, !"plot '-' using 1:3:2:xtic(2) with boxes lc variable notitle\n";
    
    '' alternative: set colours from RGB palette (Comment out above line).
    ''Print #fp_gnuplot, !"set palette model RGB defined (0 \"green\", 1 \"yellow\", 2 \"red\", 3 \"green\", 4 \"yellow\", 5 \"red\")\n";
    ''Print #fp_gnuplot, !"plot '-' using 1:3:2:xtic(2) with boxes palette notitle\n";
    Dim i As Integer
    For i = 0 To 5 Step 1
        Print #fp_gnuplot, i, !"\t", Str(x(i)), !"\t", y(i)
    Next i
    '' The option -e "command" may be used to force execution of a gnuplot
    '' command once the data has been sent.
    '' The letter "e" at the end terminates data entry.
    Print #fp_gnuplot, !"e\n";  ' Tell gnuplot it is the end of data.
    
    Print !"Mouse Click or key press to quit...\n";
    Print #fp_gnuplot, !"pause mouse any\n";
    Fileflush(fp_gnuplot)
    
    Print #fp_gnuplot, !"exit gnuplot\n";
    Fileflush(fp_gnuplot)
    '' close the pipe to gnuplot
    '' pause -1 "Hit return to continue", reset
    ''Fileflush(fp_gnuplot)
    Close #fp_gnuplot  ' close pipe to gnuplot.
    Return 0
End Function


' --> START helper functions

' Console Clear
Function Con_Clear() As Integer
    ' The system() call allows the programmer to run OS command line batch commands.
    ' It is discouraged as there are more appropriate C functions for most tasks.
    ' I am only using it in this instance to avoid invoking additional OS API headers and code.
    If (OS_Windows) Then
        Shell "cls"
    Elseif (OS_Unix) Then
        Shell "clear"
    End If
    Return 0
End Function

' Console Pause (GetKey version)
Function Con_Pause() As Integer
    Dim As Long dummy
    Print("Press any key to continue...")
    dummy = Getkey
    Return 0
End Function

' A wrapper to flush/clear the keyboard input buffer
Sub Clear_Stdin()
    While Inkey <> ""  ' loop until the Inkey buffer is empty
    Wend
End Sub


