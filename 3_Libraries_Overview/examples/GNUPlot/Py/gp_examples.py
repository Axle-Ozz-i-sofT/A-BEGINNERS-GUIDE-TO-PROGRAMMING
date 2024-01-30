#-------------------------------------------------------------------------------
# Name:        gp_example.bas
# Purpose:     GNUPlot Examples
#
# Platform:    Win64, Ubuntu64
# Depends:     GNUPlot V 5.2.x plus
#
# Author:      Axle
# Created:     15/04/2023
# Updated:     17/04/2023
# Copyright:   (c) Axle 2023
# Licence:     MIT-0 No Attribution
#-------------------------------------------------------------------------------
# Notes:
# Linux may warn about elementary icon themes. It is OK to ignore this warning.
# If you wish to install the theme you can without changing the desktop icons.
# sudo apt update
# sudo apt install elementary-icon-theme
#
#-------------------------------------------------------------------------------
# Credits:
# http://www.physics.drexel.edu/~valliere/PHYS305/Monte_Carlo/pipes/gnuplot_pipe_demo.c
#

import os
import math

def main():
    if 1 == Con_IsREPL():
        print("This application is best run from OS Command interpreter")
        #Con_Pause()

    line_plot_file()
    #line_plot_inline()

    #sine_plot_inline()

    #circle_file()
    #circle_piped()

    # the following 2 functions must both be uncommented.
    #moving_circles_data()  # Create data file. Only required once.
    #plot_moving_circles()  # Plot data file.

    #plot_moving_circles_2()  # Plot all with pipe.

    #sine_test_2()  # another more comples sine plot example.

    #image_load_1()  # Loads an image with a scale plot overlay.
    #bar_graph_1()

    Con_Pause() # DEBUG Pause

    return 0
# <==== END Main


# Basic test using a script
def line_plot_file():
    # A very simple line graph with lines and points.
    # We can open a command instance with system(), call gnuplot with the
    # argument of our file name with the plot commands.
    # This is the most simple way to call a gnuplot script, but all of the
    # control for gnuplot exists entirely withing the script and assosiated
    # data files. We cannot offer more commands to gnuplot.
    # The gnuplot script has been created and saved seperately from our
    # application.

    # for windows
    if os.name == 'nt':
        os.system(".\\gnuplot\\bin\\gnuplot.exe linepoints.gp")
    # for mac and linux
    elif os.name == 'posix':
        os.system("gnuplot linepoints.gp")
    else:
        return None  # Other OS

# END Function

# Basic test using a pipe.
def line_plot_inline():

    # plot points (data) The same data from linepoints.gp
    x = [ 2015, 2016, 2017, 2018, 2019, 2020 ]
    y = [ 344, 543, 433, 232, 212, 343 ]

    # Path to gnuplot. -persist will keep the gnuplot window open and it will
    # need to be closed manually. In Windows we can send the exit or quit
    # command to close a persistant window, but this does not work under Linux.
    if os.name == 'nt':
        filename = ".\\gnuplot\\bin\\gnuplot.exe -persist"
    # for mac and linux
    elif os.name == 'posix':
        filename = "gnuplot -persist"
    else:
        return None  # Other OS

    # In this example we are going to open a one way pipe to gnuplot where
    # we can send commands and data directly from our application. No
    # script file is used as all of the commands are sent via the pipe file in
    # memory. GNUPlot is designed to poll this memory file for new commands.
    #FILE* fp_gnuplot;  # Open named pipe as gnuplot
    try:
        fp_gnuplot = os.popen(filename, mode = 'w')  # Open a file handle to the gnplot pipe

    except:
        print("An exception occurred")
        return -1

    # We are sending the plot and format commands as in "inline" command to
    # gnuplot via the pipe we opened above.
    # Everything after this format line is data.
    #fp_gnuplot.write("plot '-' u 1:2 t 'Price' w lp\n")  # Shorthand version of the commands below.
    fp_gnuplot.write("plot '-' using 1:2 title 'Price' with linespoints\n")

    # Loop through the data contained in the 2 arrays sending it to gnuplot.
    # If you notice the '\n' character the data will be formated as 2
    # columns of x, y data as in the previuos script example. If you open
    # linepoints.gp in a text editor you can see the construction of the
    # formatting and data sent to gnuplot.

    for i in range(0, 6):  # Note 6 points 0 to 5
        #fprintf(fp_gnuplot,"%d\t%d\n", x[i], y[i]);
        buffer = str(x[i]) + "\t" + str(y[i]) + "\n"
        fp_gnuplot.write(buffer)  # Print adds a new lin\n unsless

    # The option -e "command" may be used to force execution of a gnuplot
    # command once the data has been sent.
    # The letter "e" at the end terminates data entry.
    fp_gnuplot.write("e\n")
    print("Click Ctrl+d to quit...\n", end='')  # Writes to stdout
    fp_gnuplot.flush()  # Push all commands and data to the pipe file.

    print("1st Mouse Click or key press to clear the graph...\n", end='')
    fp_gnuplot.write("pause mouse any\n")  # Pause until mouse click or key press.
    fp_gnuplot.flush()

    fp_gnuplot.write("clear\n")  # Clear the gnuplot window.
    print("2nd Mouse Click or key press to quit...\n", end='')
    fp_gnuplot.write("pause mouse any\n")
    fp_gnuplot.flush()

    #gnuplot> help set Print
    #The `set Print` Command redirects the Output of the `Print` Command To a file.
    #
    #Syntax:
    #set Print
    #set Print "-"
    #set Print "<filename>" [Append]
    #set Print "|<shell_command>"
    #set Print $datablock [Append]
    #
    #`set Print` With no parameters restores Output To <STDERR>.  The <filename>
    #"-" means <STDOUT>. The `Append` flag causes the file To be opened in Append
    #mode.  A <filename> starting With "|" Is opened As a Pipe To the
    #<shell_command> On platforms that support piping.
    #
    #The destination For `Print` commands can also be a named Data block. Data
    #block names start With '$', see also `inline data`.

    # exit gnuplot can be used with Windows OS when gnuplot is called with -persist
    # This does not work under Linux.
    fp_gnuplot.write("exit gnuplot\n")
    fp_gnuplot.flush()  # This last fflush is really all that is required.
    fp_gnuplot.close()  # Close the pipe to gnuplot.
    # No messages are returned to the console until GNUPlot has closed.

    return None
# END Function

# basic sine test using pipe
def sine_plot_inline():
    # Using and writing to a pipe is very similar to writing to a file on disk.
    # Although I have used a string literal in popen() it is prefered to use
    # a variable as shown in other examples.
    # Remember to try different comands such as -persist to see the effects
    # on different systems. You can use the examples as a base to experiment with.
    if os.name == 'nt':
        #filename = ".\\gnuplot\\bin\\gnuplot.exe -persist"
        # Get a file handle
        try:
            # Open a file handle to the gnplot pipe
            fp_gnuplot = os.popen(".\\gnuplot\\bin\\gnuplot.exe -persist", mode = 'w')
        except:
            print("An exception occurred")
            return -1

    # for mac and linux
    elif os.name == 'posix':
        #filename = "gnuplot"
        try:
            # Open a file handle to the gnplot pipe
            fp_gnuplot = os.popen("gnuplot", mode = 'w')
        except:
            print("An exception occurred")
            return -1
    else:
        return None  # Other OS
    # END Test for OS

    # Plot a very simple sine wave. Gnuplot will use auto formating and scales
    # if none are set.
    # This will use an auto x scale of -10 to +10 and plot the y return
    # of x for each increment. In a real worrld plot we would be providing
    # a well defined x and y scale minimum and maximum using the "set" command.
    fp_gnuplot.write("plot sin(1/x)\n")  # x, y are gnuplot graph variables

    # The letter "e" at the end of the first column terminates data entry
    #fp_gnuplot.write("e\n")  # the end of data is not always used.
    fp_gnuplot.flush()

    # Pause. Note that I have not used " ... \n", end='') here.
    # Some print() statements use the C new line "\n", and thers use the
    # default Python new line.
    print("Mouse Click or key press to quit...")
    fp_gnuplot.write("pause mouse any\n")  # pause gnuplot
    fp_gnuplot.flush()

    # Exit Windows gnuplot -persist
    fp_gnuplot.write("exit gnuplot\n")  # exit/quit gnuplot
    fp_gnuplot.flush()
    fp_gnuplot.close()  # Close the pipe to gnuplot.

# END Function

# The following performs 2 seperate taks. The first part will ask
# for the radius and center coordinates of a circle. It will create a formated
# script file along with the x.y data calculated in our application.
#
# At the very end we will use system() to run gnuplot with the circle.txt file
# as the argument. We are not using a pipe in this example
#
# I recomend comparing the created "circle.txt against the code lines below.
# Plot a circle, write to file then send data file to gnuplot. Save plot as PNG.
#
# Please note that all Python inputs are strings!
def circle_file():
    # Type hints variable: type
    r: float = 0.0
    x: float = 0.0
    y: float = 0.0
    x0: float = 0.0
    y0: float = 0.0

    # Open our text file to create a gnuplot script with data.
    try:
        fp = open("circle.txt", "w")
    except:
        print("Error opening file!")
        return -1

    # Obtain the circle dimensions, and center of circle x, y.
    # Note! I have used try/except as an input gaurd for '' empty string.
    # Negative float values for Radius will default to +5.0
    try:
        r = float(input("Enter the radius of the circle to be plotted: "))
        if (r < 0):  ## Test must be possative radius.
            print("Radius must be a possative value. Setting raduis to 5.")
            r = float(5.0)
    except:  # ValueError:
        print("Invalid numeric input. Setting to value 5.")
        r = float(5.0)

    print("\nEnter the x and y-coordinates of the center:\n", end = '')
    try:
        x0 = float(input("Enter the x-coordinates of the center: "))
    except:  # ValueError:
        print("Invalid numeric input. Setting to value 5.")
        x0 = float(0.0)

    try:
        y0 = float(input("Enter the y-coordinates of the center: "))
    except:  # ValueError:
        print("Invalid numeric input. Setting to value 5.")
        y0 = float(0.0)

    # Create our plot formatting and write it to the script.
    # Note: Unset pause if printing to file.
    fp.write("set title \"Circle plot\"\n")  # Set the title.
    #fp.write("set term png size 600, 600\n")  # print to file
    #fp.write("set size 1,1\n")  # print to file
    #fp.write("set output \"image.png\"\n")  # print to file


    fp.write("set size square 1,1\n")  # set the graph to squre x,y and 1 to 1 ratio
    # set size ratio 1, set size ratio 2, set size ratio 0.5, etc

    # Create a margin/offset in the autoscale of the graph. at 0 offset the
    # plot touches the graph border.
    fp.write("set offsets 1, 1, 1, 1\n")

    # using 1:2 selects col 1 and col 2 for data. (x, y, z | 1:2:3)
    # 1:5 will select col 1 and col 5 for x, y data (tabe delimited csv)
    fp.write("plot '-' using 1:2 w l\n")  # ( with linespoints 1)

    # Create the polt x.y data for the circle. it is created as a 2 column
    # TAB delimited csv format. gnuplot uses the 1:2 to read from col 1 for y
    # and col 2 for x.
    # Not joining back to origin. See fix below
    # https://www.bragitoff.com/2017/08/plotting-exercises-c-gnuplot/

    # Note that a for loop with range() requires integers! Use while() instead.
    y = y0-r
    while (y <= y0 + r):  # >
        x = math.sqrt(r*r-(y-y0)*(y-y0))+x0
        buffer = str(x) + "\t" + str(y) + "\n"
        fp.write(buffer)
        y += 0.1

    y = y0+r
    while (y >= y0-r):  # <
        x = -math.sqrt(r*r-(y-y0)*(y-y0))+x0
        buffer = str(x) + "\t" + str(y) + "\n"
        fp.write(buffer)
        y -= 0.1

    # Hack to finish the circle. Connect last line to origin point.
    # Unfotunately the last dot plot does not create a line to the origin dot
    # of the cicle so I have redrawn the first dot plot to draw the last line
    # connection.
    y = y0-r  # reset y to the start value.
    x = -math.sqrt(r*r-(y-y0)*(y-y0))+x0
    buffer = str(x) + "\t" + str(y) + "\n"
    fp.write(buffer)

    # Send the last commands to the script file...
    fp.write("e\n")  # ends/finishes the 'plot' data mode.
    #fp.flush()

    fp.write("pause mouse any\n")  # pause until mouse or keyboard click.
    #fp.flush()

    # Save a PNG image of the plot. Note the 3 seperate script lines.
    # Notice that I have changed the output from the UI terminal to a file.
    # I have then run all of the previous commands and data this time
    # outputting to a PNG image file with "replot".
    fp.write("set terminal png\nset output \"Circle.png\"\nreplot\n")

    fp.write("exit gnuplot\n")  # exit gnuplot application.
    fp.flush()  # Push all data to the file.

    fp.close()  # close the text/script file.
    Con_sleep(1)  # Allow text file to close.

    # Execute gnuplot with the created text file as the script.
    # all gnuplot commands and data from above are in the script text file.
    # for windows
    if os.name == 'nt':
        os.system(".\\gnuplot\\bin\\gnuplot.exe -persist circle.txt")
    # for mac and linux
    elif os.name == 'posix':
        os.system("gnuplot circle.txt")
    else:
        return None  # Other OS

    # print/stdout is the console, not gnupolt.
    print("Mouse Click or key press to quit...\n", end='')

    return 0  # None
# End Function


# Plot a circle, write directly to gnuplot using a pipe. Save plot as PNG.
# Ths is the same as the version that writes a script file except that we are
# now sending the commands and data directly to gnuplot via a pipe.
# This is sometime refered to as "inline programming" where the commands
# of another language are created within our primary application and then sent
# to the second application to be executed.
#
# Please note that all Python inputs are strings!
def circle_piped():

    if os.name == 'nt':
        filename = ".\\gnuplot\\bin\\gnuplot.exe"
    # for mac and linux
    elif os.name == 'posix':
        filename = "gnuplot -persist"
    else:
        return None  # Other OS

    r = 0.0
    x = 0.0
    y = 0.0
    x0 = 0.0
    y0 = 0.0

    # Open a pipe "os.popen()" to gnuplot.
    try:
        fp_gnuplot = os.popen(filename, mode = 'w')  # Open a file handle to the gnplot pipe
    except:
        print("An exception occurred")
        return -1

    # Obtain the circle dimensions, and center of circle x, y.
    # Note! I have used try/except as an input gaurd for '' empty string.
    # Negative float values for Radius will default to +5.0
    try:
        r = float(input("Enter the radius of the circle to be plotted: "))
        if (r < 1):
            print("Radius must be a possative value. Setting raduis to 5.")
            r = float(5.0)
    except:  # ValueError:
        print("Invalid numeric input. Setting to value 5.")
        r = float(5.0)

    print("\nEnter the x and y-coordinates of the center:\n", end = '')
    try:
        x0 = float(input("Enter the x-coordinates of the center: "))
    except:  # ValueError:
        print("Invalid numeric input. Setting to value 5.")
        x0 = float(0.0)

    try:
        y0 = float(input("Enter the y-coordinates of the center: "))
    except:  # ValueError:
        print("Invalid numeric input. Setting to value 5.")
        y0 = float(0.0)

    # Note: Unset pause if printing to file, or print after pause.
    fp_gnuplot.write("set title \"Circle plot\"\n")
    #fp_gnuplot.write("set term png size 600, 600\n")  # print to file
    #fp_gnuplot.write("set size 1,1\n")  # print to file
    #fp_gnuplot.write("set output \"image.png\"\n")  # print to file

    fp_gnuplot.write("set size square 1,1\n")  # set the graph to squre x,y and 1 to 1 ratio
    # set size ratio 1, set size ratio 2, set size ratio 0.5, etc

    # Create a margin/offset in the autscale of the graph. At 0 offset the
    # plot touches the graph border.
    fp_gnuplot.write("set offsets 1, 1, 1, 1\n")

    # using 1:2 selects col 1 and col 2 for data.
    fp_gnuplot.write("plot '-' using 1:2 w points pt 7 ps 1.5\n")  # ( with linespoints) pt 0 = dot, 7 = circle
    # https://livebook.manning.com/book/gnuplot-in-action-second-edition/chapter-9/161

    # Note that a for loop with range() requires integers! Use while() instead.
    y = y0-r
    while (y <= y0 + r):  # >
        x = math.sqrt(r*r-(y-y0)*(y-y0))+x0
        buffer = str(x) + "\t" + str(y) + "\n"
        fp_gnuplot.write(buffer)
        y += 0.1

    y = y0+r
    while (y >= y0-r):  # <
        x = -math.sqrt(r*r-(y-y0)*(y-y0))+x0
        buffer = str(x) + "\t" + str(y) + "\n"
        fp_gnuplot.write(buffer)
        y -= 0.1

    # Hack to finish the circle. Connect last line to origin point.
    #y = y0-r  # reset y to the start value.
    #x = -math.sqrt(r*r-(y-y0)*(y-y0))+x0
    #buffer = str(x) + "\t" + str(y) + "\n"
    #fp_gnuplot.write(buffer)


    fp_gnuplot.write("e\n")  # ends/finishes the 'plot' instruction mode.
    fp_gnuplot.flush()

    fp_gnuplot.write("pause mouse any\n")  # pause until mouse or keyboard click
    #fp_gnuplot.flush()

    print("Mouse Click or key press to save file and quit...\n", end='')

    # Save a PNG image of the plot.
    # Notice that I have changed the output from the UI terminal to a file.
    # I have then run all of the previous commands and data this time
    # outputting to a PNG image file with "replot".
    fp_gnuplot.write("set terminal png\nset output \"file.png\"\nreplot\n")

    fp_gnuplot.write("exit gnuplot\n")  # exit gnuplot application.
    fp_gnuplot.flush()

    fp_gnuplot.close()  # Close the pipe to gnuplot.

    return None
# End Function

# Generate data for moving circles demo ''
# Michel Vallieres ''
#int main( int argc, char * argv[] )  // requires compiled.exe >> output.dat
# Rewrite to output to file.
# In this example we are writing only the data to a csv file. We will call the
# data file from our inline programming via a pipe with gnuplot.
# Sometimes we may need to plot data from another application that is exporting
# the data in a scv format. This could be data created with excell or access
# for example.
def moving_circles_data():

    cx: float = 0.0
    cy: float = 0.0
    angle: float = 0.0
    ang: float = 0.0
    radius: float = 0.0
    x: float = 0.0
    y: float = 0.0
    circles: int = 0

    # Open/create our text data file for writing.
    try:
        fp = open("circles.dat", "w")
    except:
        print("Error opening file!")
        return -1

    # Carry out the methods to plot a set of circles.
    # You will notice that 3 colomns of data are created.
    for circles in range(1, 11, 1):  # Note plots 1 to 10
        cx = circles
        cy = 1.3 * circles
        radius = 0.5
        for angle in range(0, 360, 1):  # Note: Plots 0 to 359
            # NOTE:  C, FB #define M_PI 3.14159265358979323846
            # Python 3 math.pi constant 3.141592653589793
            # This will give slightly different values in the plot.
            ang = math.pi*angle/180
            x = cx + radius * math.cos(ang)
            y = cy + radius * math.sin(ang)
            # Note the "\t" TAB delimiter between each column.
            buffer = str(angle) + "\t" + str(x) + "\t" + str(y) + "\n"
            fp.write(buffer)
        fp.write("#endframe\n")  # This is just a comment to seperate the data blocks.

    fp.close()  # close our data file "circles.dat"

    return 0
# End Function

# Demonstration of pipe stream connection to gnuplot
# Michel Vallieres
# In the second part of this example we are going to supply the gnuplot formating
# with "inline" programming via a pipe. We will also provide the data file from
# the above function to be plotted. If we are using a common data type we can
# substitute circles.dat with any other data file.
def plot_moving_circles():
    # file pointer for pipe connection
    if os.name == 'nt':
        filename = ".\\gnuplot\\bin\\gnuplot.exe -persist"
    # for mac and linux
    elif os.name == 'posix':
        filename = "gnuplot"
    else:
        return None  # Other OS

    # open pipe for writing
    try:
        fp_gnuplot = os.popen(filename, mode = 'w')  # Open a file handle to the gnplot pipe
    except:
        print("An exception occurred")
        return -1

    # Issue gnuplot commmands
    fp_gnuplot.write("set title \'C I R C L E S\'\n")
    fp_gnuplot.write("unset key\n")
    fp_gnuplot.write("set pointsize 0.5\n")
    fp_gnuplot.write("plot \'circles.dat\' using 2:3 with points\n")

    # flush buffer to make sure that all
    # commands are received by gnuplot
    fp_gnuplot.flush()

    # 5 sec delay
    # The child application will remain open untill the exit command is given,
    # but this may not always be the case.
    # We cauold send the pause command to gnuplot instead of the C sleep() function.
    Con_sleep(5)
    # or send a pause command to gnuplot instead of sleep()
    # pause 5  // Pause for 5 seconds; puase -1 wait for Enter (carrage return)
    #fp_gnuplot.write("pause mouse any\n")  # pause until mouse or keyboard click
    #fp_gnuplot.flush()

    # make gnuplot quit
    fp_gnuplot.write("exit gnuplot\n")  # exit gnuplot

    # close the pipe to gnuplot
    fp_gnuplot.close()  # close pipe to gnuplot.

    return 0
# End Function

# Demonstration of pipe stream connection to gnuplot
# Michel Vallieres
# This is the same plot example as the previous function except we
# are constructing and and sending commands plus data via the pipe.
# I am using data blocks to draw each circle sequentially
# giving the appearance of a moving circle.
# We can use this method in a loop to repeatedly update with new
# data to gnuplot as it arrives in our application.
# Topic: "inline data and datablocks"
# http://gnuplot.info/docs_5.5/loc3521.html
def plot_moving_circles_2():
    cx: float = 0.0
    cy: float = 0.0
    angle: float = 0.0
    ang: float = 0.0
    radius: float = 0.0
    x: float = 0.0
    y: float = 0.0
    circles: int = 0

    # file pointer for pipe connection
    if os.name == 'nt':
        filename = ".\\gnuplot\\bin\\gnuplot.exe -persist"
    # for mac and linux
    elif os.name == 'posix':
        filename = "gnuplot"
    else:
        return None  # Other OS

    # open pipe for writing
    try:
        fp_gnuplot = os.popen(filename, mode = 'w')  # Open a file handle to the gnplot pipe
    except:
        print("An exception occurred")
        return -1

    # Issue gnuplot commmands
    fp_gnuplot.write("set title \'C I R C L E S\'\n")
    fp_gnuplot.write("set size ratio -1\n")
    fp_gnuplot.write("set xtics 0.5\nset ytics 0.5\n\n")  # Try 1
    fp_gnuplot.write("set xrange [0:12]\n")
    fp_gnuplot.write("set yrange [0:14]\n\n")
    fp_gnuplot.write("unset key\n")
    fp_gnuplot.write("set pointsize 0.5\n")
    # We no longer set "plot * using 2:3 with points \n" before the data.

    # Carry out the methods to plot a set of circles.
    # You will notice that 3 colomns of data are created.
    for circles in range(1, 11, 1):
        cx = circles
        cy = 1.3 * circles
        radius = 0.5
        # Creat datablocks for each 10 circles named $data1, $data2 ...
        buffer1 = "$data" + str(circles) +" << EOD\n"
        fp_gnuplot.write(buffer1)
        for angle in range(0, 360, 1):  # Note: Plots 0 to 359
            # NOTE:  C, FB #define M_PI 3.14159265358979323846
            # Python 3 math.pi constant 3.141592653589793
            # This will give slightly different values in the plot.
            ang = math.pi*angle/180
            x = cx + radius * math.cos(ang)
            y = cy + radius * math.sin(ang)
            # Note the "\t" TAB delimiter between each column.
            buffer2 = str(angle) + "\t" + str(x) + "\t" + str(y) + "\n"
            fp_gnuplot.write(buffer2)

        fp_gnuplot.write("EOD\n")  # Marke the End Of Data
        # Now we can draw the plot using the $data[n] name.
        buffer3 = "plot $data" + str(circles) + " using 2:3 with points\n"
        fp_gnuplot.write(buffer3)
        # wait for one second and repeat with new data. Try different time values.
        fp_gnuplot.write("pause 0.2\n")
        fp_gnuplot.flush()  # Push the data to gnuplot.

    # 3 sec delay after all data is sent and plotted.
    # Be sure to read up on the different abilities of pause.
    fp_gnuplot.write("pause 3 \"waiting 3 sec...\"\n")

    # make gnuplot quit
    fp_gnuplot.write("exit gnuplot\n")

    # flush buffer to make sure that all
    # commands are received by gnuplot
    fp_gnuplot.flush()  # Push the data to gnuplot.

    # close the pipe to gnuplot
    fp_gnuplot.close()   # close pipe to gnuplot.

    return 0
# End Function

# A sligltly more complex example from example 2
def sine_test_2():

    # file pointer for pipe connection
    if os.name == 'nt':
        filename = ".\\gnuplot\\bin\\gnuplot.exe"
    # for mac and linux
    elif os.name == 'posix':
        filename = "gnuplot"
    else:
        return None  # Other OS

    # open pipe for writing
    try:
        fp_gnuplot = os.popen(filename, mode = 'w')  # Open a file handle to the gnplot pipe
    except:
        print("An exception occurred")
        return -1

    #fp_gnuplot.write("plot sin(x) with lines\n")  # x. y are gnuplot graph variables
    #fp_gnuplot.write("replot cos(x) with lines\n")

    fp_gnuplot.write("set xrange [0:10]\n")  # [-10:10]
    fp_gnuplot.write("set yrange [0:1]\n")  # [-1:1]

    fp_gnuplot.write("p sin(x) w l, cos(x) w l\n")

    # Some hints for other common gnuplot commands to experiment with.
    # Check the gnuplot help document for more information :)
    # x and y tics  // set the scale values, color etc
    # unset xtics
    # unset ytics
    #
    # set xtics 0, 5, 10
    # set ytics ("bottom" 0 , "top" 1)
    #
    # labels, titles, and legends
    # p sin(x) w l title "sine wave", cos(x) w l title "cos(x)"  // lables/legend
    #
    # set nokey  // removes the key legend
    # set key top left  // change the position of the graph key.
    # top, bottom, left, right, and center
    # set key at 1, 0.5
    #
    # note that title has 2 seperate uses depending upon the context of where
    # it is issued. It can be mane title above the graph, or a title within
    # the graph.
    # set title "Gnuplot Test"  // Main graph title.
    #
    # set size square  // sets the graphic window to square
    # line and point types + color
    # https://www.algorithm-archive.org/contents/plotting/plotting.html
    #
    # p sin(x) with lines dashtype 2 linecolor rgb "black" title "sin(x)"
    # rep cos(x) w p pt 17 lc rgb "purple" t "cos(x)"
    #
    # gnuplot aliases:
    # command         alias
    # plot            p
    # replot          rep
    # with lines      w l
    # with points     w p
    # linecolor       lc
    # pointtype       pt
    # title           t
    #
    # Outputting the plot to file
    # set terminal pngcairo
    # set output "check.png"
    # set terminal pngcairo size 640, 480
    #
    # Data files.
    # file_0000.dat, file_0001.dat, file_0002.dat ...
    # Data as TAB delimited csv (Comma delimeted can be set)
    #
    # fp_gnuplot.write("set multiplot\n")  # multiple plot graphs in one page.

    # fp_gnuplot.write("e\n")
    fp_gnuplot.flush()

    # stdout writes to the OS command line console, not gnuplot.
    print("Mouse Click or key press to quit...\n", end='')
    fp_gnuplot.write("pause mouse any\n")
    fp_gnuplot.flush()

    fp_gnuplot.close()   # close pipe to gnuplot.
    return 0
# End Function

# Load PNG image, and add scale.
def image_load_1():
    # Path to gnuplot
    if os.name == 'nt':
        filename = ".\\gnuplot\\bin\\gnuplot.exe"
    # for mac and linux
    elif os.name == 'posix':
        filename = "gnuplot"
    else:
        return None  # Other OS

    # open pipe for writing
    try:
        fp_gnuplot = os.popen(filename, mode = 'w')  # Open a file handle to the gnplot pipe
    except:
        print("An exception occurred")
        return -1

    fp_gnuplot.write("set arrow from 31,20 to 495,20 nohead front ls 1\n")
    # set for [ii=0:11] arrow from 31+ii*40,35 to 31+ii*40,45 nohead front ls 1
    fp_gnuplot.write("set for [ii=0:11] arrow from 31+ii*40,15 to 31+ii*40,25 nohead front ls 1\n")
    # set number and unit as different labels in order
    # to get a smaller distance between them
    fp_gnuplot.write("set label front '0'  at  25,30 front tc ls 1\n")
    fp_gnuplot.write("set label front 'cm' at  37,30 front tc ls 1\n")
    fp_gnuplot.write("set label front '5'  at 225,30 front tc ls 1\n")
    fp_gnuplot.write("set label front 'cm' at 237,30 front tc ls 1\n")
    fp_gnuplot.write("set label front '10' at 420,30 front tc ls 1\n")
    fp_gnuplot.write("set label front 'cm' at 442,30 front tc ls 1\n")

    # http://www.gnuplotting.org/images-within-a-graph/
    fp_gnuplot.write("set size ratio -1\n")  # x. y are gnuplot graph variables
    fp_gnuplot.write("plot 'Tropical_fish.jpg' binary filetype=jpg with rgbimage\n")
    # "apple-logo.png" binary filetype=png origin=(1960, 165) dx=1./10 dy=1./220000 with rgbimage notitle,\
    # #Plot the background image
    # plot "map.png" binary filetype=png w rgbimage
    fp_gnuplot.flush()

    #fp_gnuplot.write("e\n")
    #fp_gnuplot.flush()

    print("Mouse Click or key press to quit...\n", end='')
    fp_gnuplot.write("pause mouse any\n")
    fp_gnuplot.flush()

    fp_gnuplot.close()  # close pipe to gnuplot.
    return 0
# End Function

# Bar graph using boxes
# https://gnuplot.sourceforge.net/demo/fillstyle.html
def bar_graph_1():
    # Path to gnuplot
    if os.name == 'nt':
        filename = ".\\gnuplot\\bin\\gnuplot.exe"  # -persist
    # for mac and linux
    elif os.name == 'posix':
        filename = "gnuplot"
    else:
        return None  # Other OS

    # open pipe for writing
    try:
        fp_gnuplot = os.popen(filename, mode = 'w')  # Open a file handle to the gnplot pipe
    except:
        print("An exception occurred")
        return -1

    # The same data from the line graph in example function 2
    x = [ 2015, 2016, 2017, 2018, 2019, 2020 ]
    y = [ 344, 543, 433, 232, 212, 343 ]

    #fp_gnuplot.write("unset key\n")
    fp_gnuplot.write("set title \"Filled boxes - bar graph\"\n")
    fp_gnuplot.write("set boxwidth 0.5 relative\n")  # fixed box width 1/2
    #fp_gnuplot.write("set style fill solid 0.5\n")  # Solid fill box (No boarder)
    fp_gnuplot.write("set style fill solid 0.5 border -1\n")  # With boarder
    #set colorsequence [ default | classic | podo ]
    #fp_gnuplot.write("set colors classic\n")

    # Use "variable" auto colors
    # 1:3:2:xtic(2) == x:y,color variable:x labels | boxes linecolor variable
    fp_gnuplot.write("plot '-' using 1:3:2:xtic(2) with boxes lc variable notitle\n")

    # alternative: set colours from RBB palette (Comment out above line).
    #fp_gnuplot.write("set palette model RGB defined (0 \"green\", 1 \"yellow\", 2 \"red\", 3 \"green\", 4 \"yellow\", 5 \"red\")\n")
    #fp_gnuplot.write("plot '-' using 1:3:2:xtic(2) with boxes palette notitle\n")
    for i in range(0, 6):  # Note 6 points 0 to 5
        buffer = str(i) + "\t" + str(x[i]) + "\t" + str(y[i]) + "\n"
        fp_gnuplot.write(buffer)

    # The option -e "command" may be used to force execution of a gnuplot
    # command once the data has been sent.
    # The letter "e" at the end terminates data entry.
    fp_gnuplot.write("e\n")  # Tell gnuplot it is the end of data.

    print("Mouse Click or key press to quit...\n", end='')
    fp_gnuplot.write("pause mouse any\n")
    fp_gnuplot.flush()

    fp_gnuplot.write("exit gnuplot\n")
    fp_gnuplot.flush()
    # close the pipe to gnuplot
    # pause -1 "Hit return to continue", reset
    #fp_gnuplot.flush()
    fp_gnuplot.close()  # close pipe to gnuplot.
    return 0
# End Function


# --> START helper functions

# Test if we are inside of the REPL interactive interpreter.
# This function is in alpha and may not work as expected.
def Con_IsREPL():
    import os
    if os.sys.stdin and os.sys.stdin.isatty():
        if os.isatty(os.sys.stdout.fileno()):
            return 0  # OS Command Line
        else:
            return 1  # REPL - Interactive Linux?
    else:
        return 1  # REPL - Interactive Windows?
    return None

# Cross platform console clear.
# This function is in alpha and may not work as expected.
def Con_Clear():
    # The system() call allows the programmer to run OS command line batch commands.
    # It is discouraged as there are more appropriate C functions for most tasks.
    # I am only using it in this instance to avoid invoking additional OS API headers and code.
    import os
    if os.sys.stdin and os.sys.stdin.isatty():
        if os.isatty(os.sys.stdout.fileno()):# Clear function doesn't work in Py REPL
            # for windows
            if os.name == 'nt':
                os.system('cls')
            # for mac and linux
            elif os.name == 'posix':
                os.system('clear')
            else:
                return None  # Other OS
        else:
            return None  # REPL - Interactive Linux?
    else:
        return None  # REPL - Interactive Windows?

# Console Pause wrapper.
def Con_Pause():
    dummy = ""
    dummy = input("Press [Enter] key to continue...")
    return None

# Console sleep in seconds
def Con_sleep(times: float):
    import time
    time.sleep(times)
    return None

if __name__ == '__main__':
    main()
