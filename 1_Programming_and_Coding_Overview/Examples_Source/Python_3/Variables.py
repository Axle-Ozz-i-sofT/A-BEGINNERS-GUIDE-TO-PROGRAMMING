#-------------------------------------------------------------------------------
# Name:        Variables.py
# Purpose:
#
# Platform:    REPL, Win64, Ubuntu64
#
# Author:      Axle
# Created:     31/01/2022
# Updated:     19/02/2022
# Copyright:   (c) Axle 2022
# Licence:     MIT No Attribution
#-------------------------------------------------------------------------------

## ---> MACROS
MAXVALUE = 128

## ---> Global declare & defines
config_max_str_len = 32  # An Integer
my_lstring = "Global string"  #  Pointer to a String literal.
my_variable_integer = 6  # An Integer
my_variable_string =""  # A String variable.

def main():

    # ---> Local declare & defines
    my_lstring = "Local string"  # *pointer to a literal.
    my_variable_integer = 3  # an Integer variable
    my_variable_string = ""  # a String variable.
    local_lstring = "Local to main()"

    # Using a simple MACRO
    print("The Maximum value allowed = ", MAXVALUE)

    # ---> Tests
    # config_max_str_len += 1;  // [Error] assignment of read-only variable 'config_max_str_len'
    print("# Test our Global const in local scope")
    print("main(), config_max_str_len = ", config_max_str_len)  # cannot be altered
    test_const()  # test in a different local scope.
    print("")

    # The Local overrides global variables!
    # Although they have the same name they are different variables.
    # Global variables should be used with great care and be made up of unique names.
    print("# The Local literal variable declaration overrides the Global variable.")
    print("main(), my_lstring = ", my_lstring)  # Local overrides global!
    print("main(), my_variable_integer = ", my_variable_integer)  # Local overrides global!
    print("main(), local_lstring = ", local_lstring)
    test_local_vs_global()  # test in a different local scope.
    print("")

    # The Local overrides global variables!
    print("# The literal is copied to the local variable in main()...")
    my_variable_string = "my_variable_string Local to main()"
    print("main(), my_variable_string = ", my_variable_string)
    test_variable()  # Test in a different local scope.
    # After changing the value of the Global variable...
    print("# The variable Local to main() has not altered.")
    print("main(), my_variable_string = ", my_variable_string)
    print("");

    input("Press [Enter] to exit.")  # Wait for keypress to keep the console open.
    return None
## ---> END of MAIN MENU <---

def test_const():
    # Cannot be altered
    print("test_const(), config_max_str_len = ", config_max_str_len)

def test_local_vs_global():
    local_lstring = "Local to test_local_vs_global()"
    global my_lstring
    global my_variable_integer
    # We have not blocked the global definition with the same local name.
    print("# and to the Global variable in test_variable().")
    print("test_lstring(), my_lstring = ", my_lstring)
    print("main(), my_variable_integer = ", my_variable_integer)
    print("main(), local_lstring = ", local_lstring)

def test_variable():
    global my_variable_string
    print("test_variable(), my_variable_string = ", my_variable_string)
    # The following copies the "string literal" into the Global variable.
    # The "my_variable_string Global" in the following function is a
    # true string literal as it has no variable associated with it until it
    # is copied into my_variable_string.
    my_variable_string = "my_variable_string Global"
    print("test_variable(), my_variable_string = ", my_variable_string)


if __name__ == '__main__':
    main()
