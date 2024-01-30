' Helloworld.bas

Declare Function main_procedure() As Integer
main_procedure()  ' Call main_procedure

function main_procedure() As Integer  ' Main procedure – “Formal Entry point”
    print "Hello world!"
    print "Press any key to continue..."
    sleep  ' Pause execution so we can view the console output before it closes.
    return 0
end Function
