#------------------------------------------------------------------------------
# Name:        error_check.c
# Purpose:     Example
#
# Platform:    Win64, Ubuntu64
#
# Author:      Axle
# Created:     16/12/2021
# Updated:     18/02/2022
# Copyright:   (c) Axle 2021
# Licence:     MIT No Attribution
#------------------------------------------------------------------------------

def main():

    # The MAX size of a String type in Py is limited to the "Dynamic Memory"
    # available to the system. Pythons internals will not aloww the contents
    # of the input buffer to go above what is available to the system.
    # We can't set a static size for the length (Memory) in Python.
    Input_Buffer = ""
    Zero_Passlength = 0  # No Password entered
    Min_Passlength = 6  # Pass Min length limit
    Max_Passlength = 12  # Pass Max length limit
    Max_Attempts = 3  # Attempts limit
    Attempts_Counter = 0  # Attempts count
    Opt_Out = ""  # Opt out question y/n

    # Test that the input data is within the expected limits of 6 to 12.
    # Loops until the user enters the correct data. In real life we would
    # also need an opt out after so many tries.
    # This may appear like a lot of extra code, but it is necessary to keep a
    # level of safety.
    # If you remove all of my "Comment explanations" you will find it is not
    # that much extra code :)
    # C++, BASIC and Python have far more built in buffer safeguards than C,
    # but it is still up to the coder to ensure that any external input is
    # within the expected range of data.
    while ((len(Input_Buffer) < Min_Passlength) \
        or (len(Input_Buffer) > Max_Passlength)):
        # The next conditional check is part of an unwind to break out of deeply
        # nested loops. Although there are other methods to do this as well, I 
        # wanted to keep a simple method arcoss all 3 languages.
        # The above "While" conditional test should break the loop if the Password
        # is the correct length without the folowing test, but I wanted to show
        # an unwind method just the same :)
        if (Opt_Out == "Y") or (Opt_Out == "y"):
            break  # Quit asking for a name and break out of the loop "Step 2".
            # If we wanted to reuse the Opt_Out variable again later I suggest
            # uncommenting the following line.
            #Opt_Out = ""  ' Reset Y/N the response variable.
        else:
            # Note that Python has built in limit for the input string length
            # so we only have to check the the return is withing the range and
            # data type expected.
            print("Please enter your password.")
            Input_Buffer = input(" Between 6 to 12 letters:")
            # The second part of our test is to see if the data is within the
            # range that is expected.
            if (len(Input_Buffer) == Zero_Passlength): # 0 length string.
                print("You did not enter your password...")
                Attempts_Counter += 1
            elif ((len(Input_Buffer) > Zero_Passlength) \
                and (len(Input_Buffer) < Min_Passlength)): # String shorter than 6.
                print("The password you entered is too short...")
                Attempts_Counter += 1
            elif (len(Input_Buffer) > Max_Passlength):  # String longer than 12.
                print("The password you entered is too long...")
                Attempts_Counter += 1
            else:  # String is withing range. Success.
                print("Your Password is " + Input_Buffer)

            # Limit the number of attempts and offer an opt out so that the
            # user is not caught in an endless loop if they decide to not
            # enter a name.
            if (Attempts_Counter >= Max_Attempts):
                # keep asking in the loop untill we get a valid Y/N response.
                while (Opt_Out != "y") and (Opt_Out != "Y"):
                    print("\nSorry you have reached the maximum number of tries!")
                    Opt_Out = input("Would you like to quit? (Y/N):")
                    if (Opt_Out == "y") or (Opt_Out == "Y"):
                        break  # Quit asking for a name and break out "step 1".
                               # Opt_Out = "Y" will be used in step 2.
                    elif (Opt_Out == "n") or (Opt_Out == "N"):
                        # reset the attempts counter (3 more tries).
                        Attempts_Counter = 0 
                        Opt_Out = ""  # reset the opt out counter.
                        break
                    else:
                        # ask again until we get a Y/N response.
                        print("Invalid response!")

    print("")
    print("File read error test")
    # Checking the error return of a function. This is always recomended when
    # the function handles data from an unknown source aka anything outside
    # of the source code of you application. This icludes "User Inputs",
    # "Data from a file or database", "Information from the web",
    # "Communication and data transfers to other apps" ++.
    # We can never guarentee the existance of data outside of our application
    # or if it will be the data that we have expected.
    #
    # Description of:
    # open(file, mode='r', buffering=-1, encoding=None, errors=None, \
    # newline=None, closefd=True, opener=None)
    # Description
    # https://docs.python.org/3/library/functions.html#open
    # Return Value:
    # Python 3 only returns an "Object" and no error values like C and FreeBASIC.
    # If module core throws an error it is called an exception and the application
    # will terminate if the exception has not been handled. For this we use the
    # try: except method to "Catch" the exception and make a decission on how
    # to manage the error.
    # https://www.w3schools.com/python/python_try_except.asp
    # https://docs.python.org/3/tutorial/errors.html
    # https://docs.python.org/3/library/exceptions.html
    # If it fails to open because the file does not exist for example, we need
    # to handle the error and either create the file, or tell the user that the
    # file could not be found, or any other number of options that are apropriate
    # to the context of your application. Don't ever let and error be passed to
    # your user with the horrible "Ding Sound" and the
    # "This program has terminated unexpectedly!" warning in production code. :)
    # in this example "filename.txt" does not exist so open( ...) will
    # create an exception error FileNotFoundError(2, 'No such file or directory')
    # It is possible that the file may not yet exist. Opening it
    # as "r" will return an exception.
    filename = "filename.txt"  # a dummy file name.
    try:
        # NOTE! With open will automaticaly close the file handle fp so we
        # don't need to use close(fp).
        with open(filename, "r") as fp:  # Open the File.
            # No errors, so do some file read operations.
            Con_Pause()  # wait until a key is pressed

    except FileNotFoundError as e:  # Handle the exception error.
        print("\nERROR! Cannot open file " + filename)
        print("Maybe the file has not yet been created.")
        # 5 differnt way of retreiving the error message.
        # With e.args[0] we can retreive the error number for some functions.
        # it is not possible to list all the exception types and errors for
        # Python 3 so we usually attempt to cover the most common.
        # except Exception as e: # Will polulate e with any exception class
        # caught with except. You will need to break down the sublevels of each
        # child class to show the actual error such as "FileNotFoundError"
        print(e)
        print(e.args)
        print(e.args[0], e.args[1])
        print(repr(e))
        print(f"{type(e).__name__} at line {e.__traceback__.tb_lineno} of {__file__}: {e}")
        Con_Pause()

    Con_Pause()  # DEBUG Pause
    return None
    # END Main() <---

# Console Pause wrapper.
# This is just a simple wrapper for Input(). It is
# benificial over time to create your own personal function library for common
# task as it removes the need to type in the 3 lines of code below every time
# you need a pause. If you look at my Input() function from the C example you
# can see that I have reduced some 15 lines of code to a single function call.
# This forms the basic principles for the creation of "Modular Code" and code
# libraries. We can use a library function instead of repeadedly writting
# common ("Boiler Plate") code in our main() source.
def Con_Pause():
    dummy = ""
    print("")
    dummy = input("Press [Enter] key to continue...")
    return None

if __name__ == '__main__':
    main()
    