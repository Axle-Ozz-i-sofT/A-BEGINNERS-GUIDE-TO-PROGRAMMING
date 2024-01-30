//------------------------------------------------------------------------------
// Name:        gp_example.c
// Purpose:     GNUPlot Examples
//
// Platform:    Win64, Ubuntu64
// Depends:     GNUPlot V 5.2.x plus
//
// Author:      Axle
// Created:     12/04/2023
// Updated:     14/04/2023
// Copyright:   (c) Axle 2023
// Licence:     MIT-0 No Attribution
//------------------------------------------------------------------------------
// Notes:
// Linux may warn about elementary icon themes. It is OK to ignore this warning.
// If you wish to install the theme you can without changing the desktop icons.
// sudo apt update
// sudo apt install elementary-icon-theme
//
//------------------------------------------------------------------------------
// Credits:
// http://www.physics.drexel.edu/~valliere/PHYS305/Monte_Carlo/pipes/gnuplot_pipe_demo.c
//
//------------------------------------------------------------------------------

// The following is required for popen(), pclose() which are removed from the
// c99 standard. We can use the following line or manually write -std=gnuc99
// into the compiler options and untick the -std=c99 box [/]
// The if defines are preprocessor directives that swap in the correct code
// lines depending if we are using Windows or Linux.
#ifdef __unix__ // _linux__ (__linux__)
// Must be defined before stdio.h
#define  _XOPEN_SOURCE  // 500, 500, 700 or change -std=c99 to -std=gnuc99
#include <unistd.h>
#endif

#include <stdio.h>
#include <stdlib.h>
//#include <conio.h>
#include <string.h>

// The C std library can differ a little between systems and versions.
// I am using C99 in all example for compatibility with Windows msvcrt.dll
// which is the precursor to the current UCRTbase/d.dll
//#define _USE_MATH_DEFINES  // Undefined in ISO C-99
//#define _GNU_SOURCE  // Undefined in ISO C-99
// Note math.h libm must be linked under Linux. add -lm to linker options.
#include <math.h>

// PI is not guaranteed to be defined in all standard libraries
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define unused(x) (x) = (x)

// Basic GNUPlot examples showing script, command line and pipes commands
void line_plot_file(void);
void line_plot_inline(void);

void sine_plot_inline(void);

int circle_file(void);
void circle_piped(void);

int moving_circles_data( void );  // Create data file
int plot_moving_circles(void);  // Plot data file

int plot_moving_circles_2(void);  // Plot all with pipe.

int sine_test_2(void);
int image_load_1(void);
int bar_graph_1(void);

int Con_Sleep(int seconds); // Cross platform sleep()
int Con_Clear(void); // Cross platform clear console screen
void S_Pause(void); // Cross platform console pause until Enter
// !!! This is broken !!!
void S_Clear_Input_Buffer(void); // Safe Clear the Input Buffer
int S_getchar(void); // Cross platform safe get character (clears the input buffer)


int main(int argc, char *argv[])
    {
    unused(argc);  // Turns off the compiler warning for unused argc, argv
    unused(argv);  // Turns off the compiler warning for unused argc, argv

    // Un-comment the example function you wish to test.
    // NOTE: When sending commands to gnuplot it is generally accepted as
    // Non-Blocking, meaning that all commands are sent including pause commands
    // and control is returned to your application.

    //line_plot_file();
    //line_plot_inline();

    //sine_plot_inline();

    //circle_file();
    //circle_piped();

    // the following 2 functions must both be uncommented.
    //moving_circles_data();  // Create data file. Only required once.
    //plot_moving_circles();  // Plot data file.

    plot_moving_circles_2();  // Plot all with pipe.

    //sine_test_2();  // another more complex sine plot example.

    //image_load_1();  // Loads an image with a scale plot overlay.
    //bar_graph_1();

    //Con_Clear();
    S_Pause();  //
    return 0;
    }

// Basic test using a script
void line_plot_file(void)
    {
    // A very simple line graph with lines and points.
    // We can open a command instance with system(), call gnuplot with the
    // argument of our file name with the plot commands.
    // This is the most simple way to call a gnuplot script, but all of the
    // control for gnuplot exists entirely within the script and associated
    // data files. We cannot offer more commands to gnuplot.
    // The gnuplot script has been created and saved separately from our
    // application.
#ifdef _WIN32 // Windows 32-bit and 64-bit
    system(".\\gnuplot\\bin\\gnuplot.exe linepoints.gp");
#endif
#ifdef __unix__ // _linux__ (__linux__)
    system("gnuplot linepoints.gp");
#endif

    }

// Basic test using a pipe.
void line_plot_inline(void)
    {
    // plot points (data) The same data from linepoints.gp
    int x[] = { 2015, 2016, 2017, 2018, 2019, 2020 };
    int y[] = { 344, 543, 433, 232, 212, 343 };

    // Temporary buffer
    //char temp[128] = {'\0'};

    // Path to gnuplot. -persist will keep the gnuplot window open and it will
    // need to be closed manually. In Windows we can send the exit or quit
    // command to close a persistent window, but this does not work under Linux.
#ifdef _WIN32 // Windows 32-bit and 64-bit
    char filename[] = ".\\gnuplot\\bin\\gnuplot.exe -persist";
#endif
#ifdef __unix__ // _linux__ (__linux__)
    char filename[] = "gnuplot -persist";  // Note that gnuplot -persist will not close under Linux
#endif

    // In this example we are going to open a one way pipe to gnuplot where
    // we can send commands and data directly from our application. No
    // script file is used as all of the commands are sent via the pipe file in
    // memory. GNUPlot is designed to poll this memory file for new commands.
    FILE* fp_gnuplot;  // Open named pipe as gnuplot
    fp_gnuplot = popen(filename, "w");  // Open GNUPlot.exe CLI interface with write pipe.
    if (fp_gnuplot != NULL)
        {
        // We are sending the plot and format commands as in "inline" command to
        // gnuplot via the pipe we opened above.
        // Everything after this format line is data.
        //fprintf(gnuplot, "plot '-' u 1:2 t 'Price' w lp\n");  // Shorthand version of the commands below.
        fprintf(fp_gnuplot, "xplot '-' using 1:2 title 'Price' with linespoints\n");
        // Loop through the data contained in the 2 arrays sending it to gnuplot.
        // If you notice the '\n' character the data will be formatted as 2
        //columns of x, y data as in the previous script example. If you open
        // linepoints.gp in a text editor you can see the construction of the
        // formatting and data sent to gnuplot.
        for (int i = 0; i < 6; ++i)  // Plots 6 points 0 to 5
            {
            fprintf(fp_gnuplot,"%d\t%d\n", x[i], y[i]);
            }
        // The option -e "command" may be used to force execution of a gnuplot
        // command once the data has been sent.
        // The letter "e" at the end terminates data entry.
        fprintf(fp_gnuplot, "e\n");  // send end of data to gnuplot
        fprintf(stdout, "Click Ctrl+d to quit...\n");  // Writes a prompt to the console.
        fflush(fp_gnuplot);  // Push all commands and data to the pipe file.
        }

    fprintf(stdout, "1st Mouse Click or key press to clear the graph...\n");
    fprintf(fp_gnuplot, "pause mouse any\n");  // Pause until mouse click or key press.
    fflush(fp_gnuplot);

    fprintf(fp_gnuplot, "clear\n");  // Clear the gnuplot window.
    fprintf(stdout, "2nd Mouse Click or key press to quit...\n");
    fprintf(fp_gnuplot, "pause mouse any\n");
    fflush(fp_gnuplot);

    /*
    gnuplot> help set print
     The `set print` command redirects the output of the `print` command to a file.

     Syntax:
           set print
           set print "-"
           set print "<filename>" [append]
           set print "|<shell_command>"
           set print $datablock [append]

     `set print` with no parameters restores output to <STDERR>.  The <filename>
     "-" means <STDOUT>. The `append` flag causes the file to be opened in append
     mode.  A <filename> starting with "|" is opened as a pipe to the
     <shell_command> on platforms that support piping.

     The destination for `print` commands can also be a named data block. Data
     block names start with '$', see also `inline data`.
    */

    // exit gnuplot can be used with Windows OS when gnuplot is called with -persist
    // This does not work under Linux.
    fprintf(fp_gnuplot, "exit gnuplot\n" );
    fflush(fp_gnuplot);  // This last fflush is really all that is required.
    pclose(fp_gnuplot);  // Close the pipe to gnuplot.
    // No messages are returned to the console until GNUPlot has closed.

    }

// basic sine test using pipe
void sine_plot_inline(void)
    {
    FILE* fp_gnuplot;
    // Using and writing to a pipe is very similar to writing to a file on disk.
    // Although I have used a string literal in popen() it is preferred to use
    // a variable as shown in other examples.
    // Remember to try different comamnds such as -persist to see the effects
    // on different systems. You can use the examples as a base to experiment with.
#ifdef _WIN32 // Windows 32-bit and 64-bit
    fp_gnuplot = popen(".\\gnuplot\\bin\\gnuplot.exe -persist", "w");  //  -persist
#endif
#ifdef __unix__ // _linux__ (__linux__)
    fp_gnuplot = popen("gnuplot", "w");  //  -persist exit only works in Windows
#endif

    // Plot a very simple sine wave. Gnuplot will use auto formatting and scales
    // if none are set.
    if (fp_gnuplot != NULL)
        {
        // This will use an auto x scale of -10 to +10 and plot the y return
        // of x for each increment. In a real world plot we would be providing
        // a well defined x and y scale minimum and maximum using the "set" command.
        fprintf(fp_gnuplot, "plot sin(1/x)\n");  // x, y are gnuplot graph variables
        }

    // The letter "e" at the start of the first column terminates data entry
    //fprintf(fp_gnuplot, "e\n");  // the end of data is not always used.
    fflush(fp_gnuplot);

    // Pause
    fprintf(stdout, "Mouse Click or key press to quit...\n");
    fprintf(fp_gnuplot, "pause mouse any\n");  // pause gnuplot
    fflush(fp_gnuplot);

    // Exit Windows gnuplot -persist
    fprintf(fp_gnuplot, "exit gnuplot\n" );  // exit/quit gnuplot
    fflush(fp_gnuplot);
    pclose(fp_gnuplot);  // close pipe to gnuplot.

    }


// The following performs 2 separate task.s The first part will ask
// for the radius and center coordinates of a circle. It will create a formated
// script file along with the x.y data calculated in our application.
//
// At the very end we will use system() to run gnuplot with the circle.txt file
// as the argument. We are not using a pipe in this example
//
// I recommend comparing the created "circle.txt" against the code lines below.
// Plot a circle, write to file then send data file to gnuplot. Save plot as PNG.
int circle_file(void)
    {
    double r;
    double x,y,x0,y0;

    // Open our text file to create a gnuplot script with data.
    FILE *fp=NULL;
    fp=fopen("circle.txt","w");  // 'w' over writes previous data.
    if (fp == NULL)
        {
        printf("Error opening file!\n");
        return -1;
        }

    // Obtain the circle dimensions, and center of circle x, y.
    // ### This requires an input guard!! :
    // https://stackoverflow.com/questions/42265038/how-to-check-if-user-enters-blank-line-in-scanf-in-c
    // Better to use fgets() or similar functions.
        printf("Enter the radius of the circle to be plotted:");
        if ( scanf("%6lf",&r) < 1)
           {
           printf("Error in input. Setting Radius to 5.0.\n");
           r = 5.0;
           S_Clear_Input_Buffer();  // Clear the input buffer.
           }
        else if (r < 1)
        {
            printf("Radius must be greater than 0. Setting Radius to 5.0.\n");
           r = 5.0;
        }
        printf("Enter the x and y-coordinates of the center:\n");
        printf("Enter the x-coordinates of the center: ");
        if ( scanf("%6lf",&x0) < 1)
           {
           printf("Error in input. Setting x axis to 0.0.\n");
           x0 = 0.0;
           S_Clear_Input_Buffer();  // Clear the input buffer.
           }

        printf("Enter the y-coordinates of the center: ");
        if ( scanf("%6lf",&y0) < 1)
           {
           printf("Error in input. Setting y axis to 0.0.\n");
           y0 = 0.0;
           S_Clear_Input_Buffer();  // Clear the input buffer.
           }

    // Create our plot formatting and write it to the script.
    // Note: Unset pause if printing to file.
    fprintf(fp,"set title \"Circle plot\"\n");  // Set the title.
    //fprintf(fp,"set term png size 600, 600\n");  // print to file
    //fprintf(fp,"set size 1,1\n");  // print to file
    //fprintf(fp,"set output \"image.png\"\n");  // print to file


    fprintf(fp,"set size square 1,1\n");  // set the graph to square x,y and 1 to 1 ratio
    // set size ratio 1, set size ratio 2, set size ratio 0.5, etc

    // Create a margin/offset in the autoscale of the graph. at 0 offset the
    // plot touches the graph border.
    fprintf(fp,"set offsets 1, 1, 1, 1\n");

    // using 1:2 selects col 1 and col 2 for data. (x, y, z | 1:2:3)
    // 1:5 will select col 1 and col 5 for x, y data (tab delimited csv)
    fprintf(fp,"plot '-' using 1:2 w l\n");  // ( with linespoints 1)

    // Create the plot x.y data for the circle. it is created as a 2 column
    // TAB delimited csv format. gnuplot uses the 1:2 to read from col 1 for y
    // and col 2 for x.
    // Not joining back to origin. See fix below
    // https://www.bragitoff.com/2017/08/plotting-exercises-c-gnuplot/
    for(y=y0-r; y<=y0+r; y=y+0.1)  // >
        {
        x=sqrt(r*r-(y-y0)*(y-y0))+x0;
        fprintf(fp,"%lf\t%lf\n",x,y);
        }
    for(y=y0+r; y>=y0-r; y=y-0.1)  // <
        {
        x=-sqrt(r*r-(y-y0)*(y-y0))+x0;
        fprintf(fp,"%lf\t%lf\n",x,y);

        }
    // Hack to finish the circle. Connect the last line to the origin point.
    // Unfortunately the last dot plot does not create a line to the origin dot
    // of the circle so I have redrawn the first dot plot to draw the last line
    // connection.
    y=y0-r;  // reset y to the start value.
    x=-sqrt(r*r-(y-y0)*(y-y0))+x0;
    fprintf(fp,"%lf\t%lf\n",x,y);


    // Send the last commands to the script file...
    fprintf(fp, "e\n");  // ends/finishes the 'plot' data mode.
    //fflush(fp);

    fprintf(fp, "pause mouse any\n");  // pause until mouse or keyboard click.
    //fflush(fp);

    // Save a PNG image of the plot. Note the 3 separate script lines.
    // Notice that I have changed the output from the UI terminal to a file.
    // I have then run all of the previous commands and data this time
    // outputting to a PNG image file with "replot".
    fprintf(fp, "set terminal png\nset output \"Circle.png\"\nreplot\n");

    fprintf( fp, "exit gnuplot\n" );  // exit gnuplot application.
    fflush(fp);  // Push all data to the file.

    fclose(fp);  // close the text/script file.
    Con_Sleep(1);  // Allow text file to close.

    // Execute gnuplot with the created text file as the script.
    // all gnuplot commands and data from above are in the script text file.
#ifdef _WIN32 // Windows 32-bit and 64-bit
    system(".\\gnuplot\\bin\\gnuplot.exe -persist circle.txt");
#endif
#ifdef __unix__ // _linux__ (__linux__)
// exit does not work with -persist on Linux.
    system("gnuplot circle.txt");
#endif

    // stdout is the console, not gnupolt.
    fprintf(stdout, "Mouse Click or key press to quit...\n");

    return 0;
    }

// Plot a circle, write directly to gnuplot using a pipe. Save plot as PNG.
// This is the same as the version that writes a script file except that we are
// now sending the commands and data directly to gnuplot via a pipe.
// This is sometime referred to as "inline programming" where the commands
// of another language are created within our primary application and then sent
// to the second application to be executed.
void circle_piped(void)
    {
    // Seams to make no difference with -persist
    //char filename[] = ".\\gnuplot\\bin\\gnuplot.exe -persist";  // -persist
#ifdef _WIN32 // Windows 32-bit and 64-bit
    char filename[] = ".\\gnuplot\\bin\\gnuplot.exe";
#endif
#ifdef __unix__ // _linux__ (__linux__)
    char filename[] = "gnuplot";
#endif
    double r = 0.0;
    double x,y,x0,y0 = 0.0;

    FILE *fp_gnuplot = NULL;  // Open a file handle
    // Open a pipe "popen()" to gnuplot.
    fp_gnuplot = popen(filename, "w");  // Open GNUPlot.exe CLI interface with write pipe.
    if (fp_gnuplot != NULL)
        {
        // ### This requires an input guard!! :
        // https://stackoverflow.com/questions/42265038/how-to-check-if-user-enters-blank-line-in-scanf-in-c
        // Better to use fgets() or similar functions.
        //Get some metrics for the circle.
        printf("Enter the radius of the circle to be plotted:");
        if ( scanf("%6lf",&r) < 1)
           {
           printf("Error in input. Setting Radius to 5.0.\n");
           r = 5.0;
           S_Clear_Input_Buffer();  // Clear the input buffer.
           }
        else if (r < 1)
        {
            printf("Radius must be greater than 0. Setting Radius to 5.0.\n");
           r = 5.0;
        }
        printf("Enter the x and y-coordinates of the center:\n");
        printf("Enter the x-coordinates of the center: ");
        if ( scanf("%6lf",&x0) < 1)
           {
           printf("Error in input. Setting x axis to 0.0.\n");
           x0 = 0.0;
           S_Clear_Input_Buffer();  // Clear the input buffer.
           }

        printf("Enter the y-coordinates of the center: ");
        if ( scanf("%6lf",&y0) < 1)
           {
           printf("Error in input. Setting y axis to 0.0.\n");
           y0 = 0.0;
           S_Clear_Input_Buffer();  // Clear the input buffer.
           }

        // Note: Unset pause if printing to file, or print after pause.
        fprintf(fp_gnuplot,"set title \"Circle plot\"\n");
        //fprintf(fp_gnuplot,"set term png size 600, 600\n");  // print to file
        //fprintf(fp_gnuplot,"set size 1,1\n");  // print to file
        //fprintf(fp_gnuplot,"set output \"image.png\"\n");  // print to file


        fprintf(fp_gnuplot,"set size square 1,1\n");  // set the graph to square x,y and 1 to 1 ratio
        // set size ratio 1, set size ratio 2, set size ratio 0.5, etc

        // Create a margin/offset in the autoscale of the graph. At 0 offset the
        // plot touches the graph border.
        fprintf(fp_gnuplot,"set offsets 1, 1, 1, 1\n");

        // using 1:2 selects col 1 and col 2 for data.
        fprintf(fp_gnuplot,"plot '-' using 1:2 w points pt 7 ps 1.5\n");  // ( with linespoints) pt 0 = dot, 7 = circle
        // https://livebook.manning.com/book/gnuplot-in-action-second-edition/chapter-9/161

        // Primitive circle plot.
        for(y=y0-r; y<=y0+r; y=y+0.1)  // >
            {
            x=sqrt(r*r-(y-y0)*(y-y0))+x0;
            fprintf(fp_gnuplot,"%lf\t%lf\n",x,y);
            }
        for(y=y0+r; y>=y0-r; y=y-0.1)  // <
            {
            x=-sqrt(r*r-(y-y0)*(y-y0))+x0;
            fprintf(fp_gnuplot,"%lf\t%lf\n",x,y);

            }

        // hack to finish the circle. connect last line to origin point.
        //y=y0-r;
        //x=-sqrt(r*r-(y-y0)*(y-y0))+x0;
        //fprintf(fp_gnuplot,"%lf\t %lf\n",x,y);

        fprintf(fp_gnuplot, "e\n");  // ends/finishes the 'plot' instruction mode.
        fflush(fp_gnuplot);

        fprintf(fp_gnuplot, "pause mouse any\n");  // pause until mouse or keyboard click
        //fflush(fp_gnuplot);

        fprintf(stdout, "Mouse Click or key press to save file and quit...\n");

        // Save a PNG image of the plot.
        // Notice that I have changed the output from the UI terminal to a file.
        // I have then run all of the previous commands and data this time
        // outputting to a PNG image file with "replot".
        fprintf(fp_gnuplot, "set terminal png\nset output \"file.png\"\nreplot\n");

        fprintf(fp_gnuplot, "exit gnuplot\n" );  // exit gnuplot application.
        fflush(fp_gnuplot);

        }
    else
        {
        printf("There was a problem opening GNUPlot\n");
        }

    fclose(fp_gnuplot);  // close pipe to gnuplot.
    }


// Generate data for moving circles demo //
// Michel Vallieres //
//int main( int argc, char * argv[] )  // requires compiled.exe >> output.dat
// Rewrite to output to file.
// In this example we are writing only the data to a csv file. We will call the
// data file from our inline programming via a pipe with gnuplot.
// Sometimes we may need to plot data from another application that is exporting
// the data in a scv format. This could be data created with excel or access
// for example.
int moving_circles_data( void )
    {
    double cx, cy, angle, ang;
    double radius, x, y;
    int    circles;

    // Open/create our text data file for writing.
    FILE *fp=NULL;
    fp=fopen("circles.dat","w");  // Our text file for the data.
    if (fp == NULL)
        {
        printf("Error opening file!\n");
        return -1;
        }

    // Carry out the methods to plot a set of circles.
    // You will notice that 3 columns of data are created.
    for ( circles=1 ; circles<11; circles++)  // Plots 10 points 1 to 10
        {
        cx = circles;
        cy = 1.3 * circles;
        radius = 0.5;

        for ( angle=0 ; angle<360 ; angle=angle+1.0)  // Plots 360 points 0 to 359
            {
            ang = M_PI*angle/180;
            x = cx + radius * cos(ang);
            y = cy + radius * sin(ang);
            // Note the "\t" TAB delimiter between each column.
            fprintf(fp, "%f\t%f\t%f\n", angle, x, y );
            }
        fprintf(fp,"#endframe\n");  // This is just a comment to separate the data blocks.
        }

    fclose(fp);  // close our data file "circles.dat"
    return 0;
    }

// Demonstration of pipe stream connection to gnuplot
// Michel Vallieres
// In the second part of this example we are going to supply the gnuplot formatting
// with "inline" programming via a pipe. We will also provide the data file from
// the above function to be plotted. If we are using a common data type we can
// substitute circles.dat with any other data file.
int plot_moving_circles(void)
    {
    // file pointer for pipe connection
    FILE *gnuplot_fd;

    // open pipe for writing
#ifdef _WIN32 // Windows 32-bit and 64-bit
    char filename[] = ".\\gnuplot\\bin\\gnuplot.exe -persist";
#endif
#ifdef __unix__ // _linux__ (__linux__)
    char filename[] = "gnuplot";
#endif
    //if ( ( gnuplot_fd = popen( ".\\gnuplot\\bin\\gnuplot.exe -persist", "w" ) ) == NULL )
    if ( ( gnuplot_fd = popen( filename, "w" ) ) == NULL )  // Open GNUPlot.exe CLI interface with write pipe.
        {
        fprintf( stderr, "Error opening pipe to gnuplot\n" );
        exit(1);
        }

    // Issue gnuplot commands
    fprintf( gnuplot_fd, "set title \'C I R C L E S\'\n" );
    fprintf( gnuplot_fd, "unset key\n" );
    fprintf( gnuplot_fd, "set pointsize 0.5\n" );
    fprintf( gnuplot_fd, "plot \'circles.dat\' using 2:3 with points \n" );

    // flush buffer to make sure that all
    // commands are received by gnuplot
    fflush( gnuplot_fd );

    // 5 sec delay
    // The child application will remain open untill the exit command is given,
    // but this may not always be the case.
    // We cauold send the pause command to gnuplot instead of the C sleep() function.
#ifdef _WIN32 // Windows 32-bit and 64-bit
    _sleep( 5000 );  // Note _sleep is deprecated
#endif
#ifdef __unix__ // _linux__ (__linux__)
    sleep(5);
#endif
    // or send a pause command to gnuplot instead of sleep()
    // pause 5  // Pause for 5 seconds; pause -1 wait for Enter (carriage return)
    //fprintf(gnuplot_fd, "pause mouse any\n");  // pause until mouse or keyboard click
    //fflush(fp);

    // make gnuplot quit
    fprintf( gnuplot_fd, "exit gnuplot\n" );  // exit gnuplot

    // close the pipe to gnuplot
    pclose( gnuplot_fd );  // close pipe to gnuplot.

    return 0;
    }

// Demonstration of pipe stream connection to gnuplot
// Michel Vallieres
// This is the same plot example as the previous function except we
// are constructing and sending commands plus data via the pipe.
// I am using data blocks to draw each circle sequentially
// giving the appearance of a moving circle.
// We can use this method in a loop to repeatedly update with new
// data to gnuplot as it arrives in our application.
// Topic: "inline data and datablocks"
// http://gnuplot.info/docs_5.5/loc3521.html
int plot_moving_circles_2(void)
    {
    double cx, cy, angle, ang;
    double radius, x, y;
    int    circles;

    // file pointer for pipe connection
    FILE *gnuplot_fd;

    // open pipe for writing
#ifdef _WIN32 // Windows 32-bit and 64-bit
    char filename[] = ".\\gnuplot\\bin\\gnuplot.exe";
#endif
#ifdef __unix__ // _linux__ (__linux__)
    char filename[] = "gnuplot";
#endif
    //if ( ( gnuplot_fd = popen( ".\\gnuplot\\bin\\gnuplot.exe -persist", "w" ) ) == NULL )
    if ( ( gnuplot_fd = popen( filename, "w" ) ) == NULL )  // Open GNUPlot.exe CLI interface with write pipe.
        {
        fprintf( stderr, "Error opening pipe to gnuplot\n" );
        exit(1);
        }

    // Issue gnuplot commands
    fprintf( gnuplot_fd, "set title \'C I R C L E S\'\n" );
    fprintf( gnuplot_fd, "set size ratio -1\n");
    //fprintf( gnuplot_fd, "set xtics 0.5\nset ytics 0.5\n");  // try 1
    fprintf( gnuplot_fd, "set xrange [0:12]\n" );
    fprintf( gnuplot_fd, "set yrange [0:14]\n" );
    fprintf( gnuplot_fd, "unset key\n" );
    fprintf( gnuplot_fd, "set pointsize 0.5\n" );
    // We no longer set "plot * using 2:3 with points \n" before the data.

    // Carry out the methods to plot a set of circles.
    // You will notice that 3 columns of data are created.
    // #### Check for off by 1 359 vs 360 ####
    for ( circles=1 ; circles<11; circles++)  // Plot 10 points 1 to 10
        {
        cx = circles;
        cy = 1.3 * circles;
        radius = 0.5;
        // Create datablocks for each 10 circles named $data1, $data2 ...
        fprintf(gnuplot_fd, "$data%d << EOD\n", circles);

        for ( angle=0 ; angle<361 ; angle=angle+1.0)  // Plots 360 points 0 to 360
            {
            ang = M_PI*angle/180;
            x = cx + radius * cos(ang);
            y = cy + radius * sin(ang);
            // Note the "\t" TAB delimiter between each column.
            fprintf(gnuplot_fd, "%f\t%f\t%f\n", angle, x, y );
            }

        fprintf(gnuplot_fd, "EOD\n");  // Marks the End Of Data
        // Now we can draw the plot using the $data[n] name.
        fprintf( gnuplot_fd, "plot $data%d using 2:3 with points\n", circles );
        // wait for one second and repeat with new data. Try different time values.
        fprintf(gnuplot_fd, "pause 0.2\n");
        fflush( gnuplot_fd );  // Push the data to gnuplot.

        }

    // 3 sec delay after all data is sent and plotted.
    // Be sure to read up on the different abilities of pause.
    fprintf( gnuplot_fd, "pause 3 \"waiting 3 sec...\"\n" );

    // make gnuplot quit
    fprintf( gnuplot_fd, "exit gnuplot\n" );

    // flush buffer to make sure that all
    // commands are received by gnuplot
    fflush( gnuplot_fd );

    // close the pipe to gnuplot
    pclose( gnuplot_fd );  // close pipe to gnuplot.

    return 0;
    }

// A slightly more complex example from example 2
int sine_test_2(void)
    {
    // Path to gnuplot
#ifdef _WIN32 // Windows 32-bit and 64-bit
    char filename[] = ".\\gnuplot\\bin\\gnuplot.exe";  // -persist
#endif
#ifdef __unix__ // _linux__ (__linux__)
    char filename[] = "gnuplot";  // -persist
#endif

    FILE *fp_gnuplot;  // Open named pipe as gnuplot
    fp_gnuplot = popen(filename, "w");  // Open GNUPlot.exe CLI interface with write pipe.
    if (fp_gnuplot == NULL)
        {
        printf("Error opening file!\n");
        return -1;
        }

    //fprintf(fp_gnuplot, "plot sin(x) with lines\n");  // x. y are gnuplot graph variables
    //fprintf(fp_gnuplot, "replot cos(x) with lines\n");

    fprintf(fp_gnuplot, "set xrange [0:10]\n");  // [-10:10]
    fprintf(fp_gnuplot, "set yrange [0:1]\n");  // [-1:1]

    fprintf(fp_gnuplot, "p sin(x) w l, cos(x) w l\n");

    // Some hints for other common gnuplot commands to experiment with.
    // Check the gnuplot help document for more information :)
    // x and y tics  // set the scale values, color etc
    // unset xtics
    // unset ytics
    //
    // set xtics 0, 5, 10
    // set ytics ("bottom" 0 , "top" 1)

    // labels, titles, and legends
    // p sin(x) w l title "sine wave", cos(x) w l title "cos(x)"  // labels/legend
    //
    // set nokey  // removes the key legend
    // set key top left  // change the position of the graph key.
    // top, bottom, left, right, and center
    // set key at 1, 0.5
    //
    // note that title has 2 separate uses depending upon the context of where
    // it is issued. It can be mane title above the graph, or a title within
    // the graph.
    // set title "Gnuplot Test"  // Main graph title.
    //
    // set size square  // sets the graphic window to square
    // line and point types + color
    // https://www.algorithm-archive.org/contents/plotting/plotting.html
    //
    // p sin(x) with lines dashtype 2 linecolor rgb "black" title "sin(x)"
    // rep cos(x) w p pt 17 lc rgb "purple" t "cos(x)"
    //
    // gnuplot aliases:
    // command         alias
    // plot            p
    // replot          rep
    // with lines      w l
    // with points     w p
    // linecolor       lc
    // pointtype       pt
    // title           t
    //
    // Outputting the plot to file
    // set terminal pngcairo
    // set output "check.png"
    // set terminal pngcairo size 640, 480
    //
    // Data files.
    // file_0000.dat, file_0001.dat, file_0002.dat ...
    // Data as TAB delimited csv (Comma delimited can be set)
    //
    //fprintf(fp_gnuplot, "set multiplot\n");  // multiple plot graphs in one page.

    //fprintf(fp_gnuplot, "e\n");
    fflush(fp_gnuplot);

    // stdout writes to the OS command line console, not gnuplot.
    fprintf(stdout, "Mouse Click or key press to quit...\n");
    fprintf(fp_gnuplot, "pause mouse any\n");
    fflush(fp_gnuplot);

    fclose(fp_gnuplot);  // close pipe to gnuplot.
    return 0;
    }

// Load PNG image, and add scale.
int image_load_1(void)
    {
    // Path to gnuplot
#ifdef _WIN32 // Windows 32-bit and 64-bit
    char filename[] = ".\\gnuplot\\bin\\gnuplot.exe";  // -persist
#endif
#ifdef __unix__ // _linux__ (__linux__)
    char filename[] = "gnuplot";  // -persist
#endif

    FILE *fp_gnuplot;  // Open named pipe as gnuplot
    fp_gnuplot = popen(filename, "w");  // Open GNUPlot.exe CLI interface with write pipe.
    if (fp_gnuplot == NULL)
        {
        printf("Error opening file!\n");
        return -1;
        }

    fprintf(fp_gnuplot, "set arrow from 31,20 to 495,20 nohead front ls 1\n");
    // set for [ii=0:11] arrow from 31+ii*40,35 to 31+ii*40,45 nohead front ls 1
    fprintf(fp_gnuplot, "set for [ii=0:11] arrow from 31+ii*40,15 to 31+ii*40,25 nohead front ls 1\n");
    // set number and unit as different labels in order
    // to get a smaller distance between them
    fprintf(fp_gnuplot, "set label front '0'  at  25,30 front tc ls 1\n");
    fprintf(fp_gnuplot, "set label front 'cm' at  37,30 front tc ls 1\n");
    fprintf(fp_gnuplot, "set label front '5'  at 225,30 front tc ls 1\n");
    fprintf(fp_gnuplot, "set label front 'cm' at 237,30 front tc ls 1\n");
    fprintf(fp_gnuplot, "set label front '10' at 420,30 front tc ls 1\n");
    fprintf(fp_gnuplot, "set label front 'cm' at 442,30 front tc ls 1\n");

    // http://www.gnuplotting.org/images-within-a-graph/
    fprintf(fp_gnuplot, "set size ratio -1\n");  // x. y are gnuplot graph variables
    fprintf(fp_gnuplot, "plot 'Tropical_fish.jpg' binary filetype=jpg with rgbimage\n");
    // "apple-logo.png" binary filetype=png origin=(1960, 165) dx=1./10 dy=1./220000 with rgbimage notitle,\
    // #Plot the background image
    // plot "map.png" binary filetype=png w rgbimage
    fflush(fp_gnuplot);


    //fprintf(fp_gnuplot, "e\n");
    //fflush(fp_gnuplot);

    fprintf(stdout, "Mouse Click or key press to quit...\n");
    fprintf(fp_gnuplot, "pause mouse any\n");
    fflush(fp_gnuplot);

    fclose(fp_gnuplot);  // close pipe to gnuplot.
    return 0;
    }


// Bar graph using boxes
// https://gnuplot.sourceforge.net/demo/fillstyle.html
int bar_graph_1(void)
    {
    // Path to gnuplot
#ifdef _WIN32 // Windows 32-bit and 64-bit
    char filename[] = ".\\gnuplot\\bin\\gnuplot.exe";
#endif
#ifdef __unix__ // _linux__ (__linux__)
    char filename[] = "gnuplot";
#endif

    // The same data from the line graph in example function 2
    int x[] = { 2015, 2016, 2017, 2018, 2019, 2020 };
    int y[] = { 344, 543, 433, 232, 212, 343 };

    char buffer[128] = {'\0'};

    FILE *fp_gnuplot;  // Open named pipe as gnuplot
    fp_gnuplot = popen(filename, "w");  // Open GNUPlot.exe CLI interface with write pipe.
    if (fp_gnuplot == NULL)
        {
        printf("Error opening file!\n");
        return -1;
        }

    //fprintf(fp_gnuplot, "unset key\n");
    fprintf(fp_gnuplot, "set title \"Filled boxes - bar graph\"\n");
    fprintf(fp_gnuplot, "set boxwidth 0.5 relative\n");  // fixed box width 1/2
    //fprintf(fp_gnuplot, "set style fill solid 0.5\n");  // Solid fill box (No border)
    fprintf(fp_gnuplot, "set style fill solid 0.5 border -1\n");  // With border
    //set colorsequence [ default | classic | podo ]
    //fprintf(fp_gnuplot, "set colors classic\n");

    // Use "variable" auto colors
    // 1:3:2:xtic(2) == x:y,color variable:x labels | boxes linecolor variable
    fprintf(fp_gnuplot, "plot '-' using 1:3:2:xtic(2) with boxes lc variable notitle\n");

    // alternative: set colours from RGB palette (Comment out above line).
    //fprintf(fp_gnuplot, "set palette model RGB defined (0 \"green\", 1 \"yellow\", 2 \"red\", 3 \"green\", 4 \"yellow\", 5 \"red\")\n");
    //fprintf(fp_gnuplot, "plot '-' using 1:3:2:xtic(2) with boxes palette notitle\n");

    for (int i = 0; i < 6; ++i)
        {
        sprintf(buffer, "%d", x[i]);
        fprintf(fp_gnuplot,"%d\t%s\t%d\n", i, buffer, y[i]);
        }
    // The option -e "command" may be used to force execution of a gnuplot
    // command once the data has been sent.
    // The letter "e" at the end terminates data entry.
    fprintf(fp_gnuplot, "e\n");  // Tell gnuplot it is the end of data.

    fprintf(stdout, "Mouse Click or key press to quit...\n");
    fprintf(fp_gnuplot, "pause mouse any\n");
    fflush(fp_gnuplot);

    fprintf(fp_gnuplot, "exit gnuplot\n" );
    fflush(fp_gnuplot);
    // close the pipe to gnuplot
    // pause -1 "Hit return to continue", reset
    //fflush(fp_gnuplot);
    fclose(fp_gnuplot);  // close pipe to gnuplot.
    return 0;
    }


// ====> Helper functions

int Con_Sleep(int seconds)
    {
    // #include <stdlib.h>
    // Cross platform sleep in seconds
#ifdef _WIN32 // Windows 32-bit and 64-bit
    seconds = seconds * 1000;
    _sleep( seconds );  // Note _sleep is deprecated
#endif
#ifdef __unix__ // _linux__ (__linux__)
    sleep(seconds);
#endif
    return 0;
    }

// Console Clear
int Con_Clear(void)
    {
    // The system() call allows the programmer to run OS CLI batch commands.
    // It is discouraged as there are more appropriate C functions for most tasks.
    // I am only using it in this instance to avoid invoking additional OS API
    // headers and code.
#ifdef _WIN32 // Windows 32-bit and 64-bit
    system("cls");
#endif
#ifdef __unix__ // _linux__ (__linux__)

    system("clear");
#endif
    return 0;
    }

// Safe Pause
void S_Pause(void)
    {
    // This function is referred to as a wrapper for S_getchar()
    printf("Press Enter to continue...");
    S_getchar();// Uses S_getchar() for safety.
    }

// Must have characters or \n in the buffer or it blocks.
// Used after scanf() which can leave '\n' in the input buffer.
void S_Clear_Input_Buffer(void)
{
    S_getchar();
}

// Safe getcar() removes all artefacts from the stdin buffer.
int S_getchar(void)
    {
    // This function is referred to as a wrapper for getchar()
    int i = 0;
    int ret;
    int ch;
    // The following enumerates all characters in the buffer.
    while((ch = getchar()) != '\n' && ch != EOF )
        {
        // But only keeps and returns the first char.
        if (i < 1)
            {
            ret = ch;
            }
        i++;
        }
    return ret;
    }

