##############################################################
# Name:          Practical task
# Purpose:       Employee package delivery tracker
# Requires:      See individual Modules following
# Author:        Axle, Daniel
# Contributors:  Add name(s) for anyone who might have helped here
# Copyright:     Add copyright info here, if any, such as
#                (c) Axle 2021, Daniel 2021
# Licence:       Add license here you'd like to use, such as creative
#                commons, GPL, Mozilla Public License, etc.
#                e.g. MIT
#                https://opensource.org/licenses/MIT
# Created:       07/09/2021 < add the date you start your
#                python file
# Last Modified: 20/10/2021 < add the last date you updated your
#                python source file
# Versioning    ("MAJOR.MINOR.PATCH") Such as Version 1.0.0
##############################################################
# NOTES:
# https://www.w3schools.com/python/default.asp
# ^ an excellent resource to check as you code
##############################################################
#------------------------------------------------------------
# NOTES:
# Updated 06/01/2022
# Example csv database.
#
# TODO: Error handling and string safe routines.
# TODO!!! Fix indentation ALL
# TODO: split statements exceeding 80 character width.
#
#------------------------------------------------------------

# I am only importing the sleep function from the time library
from time import sleep

# The interpreter or compiler must read and know all Functions and Subroutines
# that exist before entering the application main routine. Library function, as
# well as our program functions must be read first at the top of the page
# before entering the application. In Python the formal entry point is the
# first If statement, so we place
# if __name__ == '__main__':
#     main()
# as the last line to force the Python interpreter to read the entire page
# before starting the application main()

def main():  # Formal application entry after first IF

    while(True):
        # Forever loop. Needs a break, return, or exit() statement to
        # exit the loop.
        # Placing the menu inside of the while loop "Refreshes" the menu
        # if an incorrect value is supplied.
        # Notice the Multiline """ String

        option = None  # Declares a menu variable as empty.

        print("""==============================================
MAIN MENU
==============================================
1 - Enter Daily Packages Delivered
2 - Produce Daily Packages Delivered Report

9 - Exit The Application
-------------------------------------------------""")
        options = input("Enter your menu choice: ")
        print("----------------------------------------------")

        # Check what choice was entered and act accordingly
        # We can add as many choices as needed

        if options == '1':
            Enter_Daily_Packages_Delivered()
        elif options == '2':
            Produce_Packages_Delivered_Report()
        elif options == '9':
            print("Exiting the application...")
            sleep(1)  # Allow 1 second for the Exit notice to display.
            break
        else:
            print("Invalid option.\nPlease enter a number from the Menu Options.\n")

    # The following is optional and waits for user input to keep the OS console
    # open before exiting the application.
    input("Press [Enter] to exit.")

    # Returns to the line directly after the call to main()
    # at the bottom of the page.
    return None
## ---> END of MAIN MENU <---

def Enter_Daily_Packages_Delivered():
    # Set some variables to hold values for the application
    # Also helps for better code readability
    min_daily_deliveries = 80
    max_daily_deliveries = 170
    min_weekly_deliveries = 350
    max_weekly_deliveries = 700
    good_min_weekly_deliveries = 450
    good_max_weekly_deliveries = 600

    # Data Structure 1
    # https://www.w3schools.com/python/python_dictionaries.asp
    employee_packages_delivered = {
    "WeekNumber": "",
    "EmployeeID": "",
    "EmployeeName": "",
    "Monday": "",
    "Tuesday": "",
    "Wednesday": "",
    "Thursday": "",
    "Friday": "",
    }

    # employee_packages_delivered data structure:
    # 0 WeekNumber, 1 EmployeeID, and 2 EmployeeName are range 0-2
    # Days of week are range 3-7
    # Using these ranges we can loop between employee detail and day entries
    # See routine: Part A) Enter Daily Packages Delivered, and D) Summary for Employee Week

    starting_day = 3

    # Data Structure 2
    weekly_report = {
    "Employee 1": 0,
    "Employee 2": 0,
    "Employee 3": 0,
    }

# Part A) Enter Employee Details
    # Start of For loop for 3 Employees
    for employee_count, employee_number in enumerate(weekly_report):
        print()
        print()
        print("==============================================")
        print("Enter details for Employee", employee_count + 1)
        # employee_count variable starts at 0, so we + 1 to offset the print
        # employee number message.
        print("==============================================")
        employee_packages_delivered["WeekNumber"] = "week " + str(input("Enter the current working week number >> "))
        print()  # Line Break
        employee_packages_delivered["EmployeeID"] = str(input("Enter the Employee ID >> "))
        print()  # Line Break
        employee_packages_delivered["EmployeeName"] = str(input("Enter the employee name >> "))
        print("----------------------------------------------")
        print()  # Line Break

# Part A) Enter packages delivered each day
        print("==============================================")
        print("Enter packages per day for employee ", employee_packages_delivered["EmployeeName"])
        print("==============================================")
        # https://www.w3schools.com/python/python_dictionaries.asp
        # https://www.programiz.com/python-programming/methods/built-in/enumerate
        for count, Day in enumerate(employee_packages_delivered):
            if  count >= starting_day:  # Monday ->
                print("Enter employee packages delivered for ", Day, " >> ", end='')
                employee_packages_delivered[Day] = str(input())
                print()  # Line Break
        print("----------------------------------------------")
        print()  # Line Break

# Part B) and D) Summary for Employee Week
        # Build our string week_ID_name_for_heading (String concatenation)
        # "Summary for employee ID:%s NAME:%s Week:%s"
        # 'Summary for employee'" ID:"43" NAME:"Jackson" Week:"17
        week_ID_name_for_heading = "ID:"
        week_ID_name_for_heading = week_ID_name_for_heading + employee_packages_delivered["EmployeeID"]
        week_ID_name_for_heading = week_ID_name_for_heading + " NAME:"
        week_ID_name_for_heading = week_ID_name_for_heading + employee_packages_delivered["EmployeeName"]
        week_ID_name_for_heading = week_ID_name_for_heading + " "
        week_ID_name_for_heading = week_ID_name_for_heading + employee_packages_delivered["WeekNumber"]

# Part B) and D)
        print("==============================================")
        print("Summary for employee", week_ID_name_for_heading)
        print("==============================================")
        day_within_limits_flag = 0
        # A variable to set up a flag that allows for a split if-else block
        # inside of the following loop.
        total_deliveries = 0
        for count, Day in enumerate(employee_packages_delivered):
            if  count >= starting_day:  # Monday ->
                # Add all deliveries for the weekly total.
                total_deliveries = total_deliveries + int(employee_packages_delivered[Day])
                if int(employee_packages_delivered[Day]) < min_daily_deliveries:
                    # See min_daily_deliveries variable at start of program
                    # where we set this amount.
                    day_within_limits_flag = 1
                    # Flag is set to true, so we can skip the following if that
                    # is outside of this loop.
                    print(employee_packages_delivered["EmployeeName"], "has not delivered enough packages on", Day)
                elif int(employee_packages_delivered[Day]) > max_daily_deliveries:
                    # See max_daily_deliveries variable at start of program
                    # where we set this amount.
                    day_within_limits_flag = 1
                    print(employee_packages_delivered["EmployeeName"], "has delivered too many packages on", Day)
            else:
                pass
        if day_within_limits_flag == 0:
            # The if-else block part 2.  If flag is set to 1 (True) we
            # can skip this.
            print(employee_packages_delivered["EmployeeName"], "has delivered within the expected daily packages.")

        print(employee_packages_delivered["EmployeeName"], "delivered a total of", total_deliveries, "packages in week", employee_packages_delivered["WeekNumber"])

# Part B)
        weekly_report[employee_number] = total_deliveries
        if total_deliveries < min_weekly_deliveries:
            # See min_weekly_deliveries variable at start of program
            # where we set this amount.
            print(employee_packages_delivered["EmployeeName"], "did not deliver enough packages in week", employee_packages_delivered["WeekNumber"])
        elif total_deliveries > max_weekly_deliveries:
            # See max_weekly_deliveries variable at start of program
            # where we set this amount.
            print(employee_packages_delivered["EmployeeName"], "delivered too many packages in week", employee_packages_delivered["WeekNumber"])
        else:
            print(employee_packages_delivered["EmployeeName"], "has delivered the expected weekly packages.")

        print("----------------------------------------------")
        print()  # Line Break

# Part C) Write to CSV
        # Note! The file write is still inside our for loop and appends the data
        # for each employee until the 3 employee records are reached.
        # The file is opened and then closed for each employee in this example.
        file_csv_out = "DailyDeliveries_DB.csv"
        # Output file to write to.  Notice we save not as *.txt but to *.csv
        # ==> Open Output file for text append ops
        with open(file_csv_out, "a") as file:
            str_buffer = ",".join(employee_packages_delivered.values())
            # Join the elements of employee_packages_delivered to a
            # single ',' delimited string.
            str_buffer = str_buffer + '\n'  # append a new line character \n
            file.write(str_buffer)
            # Write/append 'a' str_buffer to file.

    # End of For loop for 3 Employees <---

# Part E) Employee Weekly Report
    not_enough_deliveries = 0
    too_many_deliveries = 0
    good_number_of_deliveries = 0

    for employee_number_counter_2 in weekly_report:
        if weekly_report[employee_number_counter_2] < min_weekly_deliveries:
            # See min_weekly_deliveries variable at start of program
            # where we set this amount.
            not_enough_deliveries = not_enough_deliveries + 1
        elif weekly_report[employee_number_counter_2] > max_weekly_deliveries:
            # See max_weekly_deliveries variable at start of program
            # where we set this amount.
            too_many_deliveries = too_many_deliveries + 1
        elif (weekly_report[employee_number_counter_2]
        > good_min_weekly_deliveries) and (int(weekly_report[employee_number_counter_2])
        < good_max_weekly_deliveries):
            # From earlier elif to here, this could be all on one line
            # Python recommends max. 99 characters to one line, so split this
            # statement across three lines
            # see https://www.python.org/dev/peps/pep-0008/#maximum-line-length
            good_number_of_deliveries = good_number_of_deliveries + 1
        else:
            pass

# Part E)
    print("=================================================")
    print("Weekly Employee Report")
    print("=================================================")
    print(not_enough_deliveries, "employees delivered less than 350 packages a week")
    print(too_many_deliveries, "employees delivered more than 700 packages a week")
    print(good_number_of_deliveries, "employees delivered between 450-600 packages a week")
    print("-------------------------------------------------")
    print()  # Line Break
    input("Press [Enter] to return to the MAIN MENU...")

    return None
# END of Enter_Daily_Packages_Delivered <---

def Produce_Packages_Delivered_Report():
# Part G)

    # fields = ['Week Number', 'Employee ID', 'Employee Name', 'Monday Hrs',
    # 'Tuesday  Hrs', 'Wednesday Hrs', 'Thursday Hrs', 'Friday Hrs']
    # range(start, stop)
    file_csv_in = "DailyDeliveries_DB.csv"
    # Our earlier CSV formatted file. Notice we open *.csv and not *.txt.
    csv_list = []
    # main Data List. CSV converted to list.

    # It is possible that the file may not yet exist. Opening it
    # as "r" will return an exception. Let's test if the file exists first.
    try:
        with open(file_csv_in, "r") as file:  # Open the CSV File.
        # read the file into Data Structure 3 (2 dimensional Linked List)
            for buffer in file:
                # Walk each line from the file (returns ',' in the string
                # with '\n' at the end
                buffer = buffer.strip('\n')
                # Strip the newline character from the line.
                buffer_list = buffer.split(',')
                # Separate the lines at ',' to a single list.
                csv_list.append(buffer_list)
                # Append each single dimension list to a 2 dimension list.

        print("==============================================")
        print("Packages Delivered Report")
        options = input('How many reports would you like to display >> ')
        # Will default to str/text input
        print("----------------------------------------------")

        # Check if the report number is more than the entries available.
        if len(csv_list) < int(options):
            int_rep_number = len(csv_list)
        else:
            int_rep_number = int(options)

        # Calculate the position of the last item in the list
        # minus our report number to display.
        report_start = len(csv_list) - 1
        report_stop = len(csv_list) - int_rep_number - 1
        report_step = -1
        # Walk over list -1 step at a time (or in other words, reversed)

        # Walk through the List in reverse order and print each
        # line(Row) as text.
        # remember that a list with 5 elements has an index
        # from [0] to [4], thus the -1
        # range() automatically converts real numbers to index numbers
        for i in range(report_start, report_stop, report_step):
            # csv_list steps backward through the number of rows.
            for j in range(len(csv_list[i])):
                # Step through each element(column) in the row.
                print(csv_list[i][j], end=" ")
                # print each cell value in the row. This will repeat for
                # the number of columns in j.
            print()  # End of row [j], next i in range()

        input("Press [Enter] to return to the MAIN MENU...")
        return None

    except FileNotFoundError:  # If the file does not yet exist.
        print("The CSV file has not yet been created.")
        print("Please select Option 1 from the MAIN Menu")
        print("to start the data entry.")
        input("Press [Enter] to return to the MAIN MENU...")
        return None

## ---> END Application Specific Routines <---

if __name__ == '__main__':
    main()

## ---> Script Exit <---
#-------------------------------------------------------------
# NOTES/TODO:
# I Keep the extra notes block here as I often drop unused or temporary working
# out down here until I am finished.
#
#
#
#-------------------------------------------------------------
