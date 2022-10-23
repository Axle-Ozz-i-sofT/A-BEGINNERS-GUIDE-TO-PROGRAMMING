#-------------------------------------------------------------------------------
# Name:        Functions.py
# Purpose:     Examples
#
# Platform:    REPL*, Win64, Ubuntu64
#
# Author:      Axle
# Created:     22/02/2022
# Updated:
# Copyright:   (c) Axle 2022
# Licence:     MIT No Attribution
#-------------------------------------------------------------------------------

import sys

def main():


    # Unfortunately python does not contain any primitive data types. The best
    # we can achieve is the total size of the object but we cant obtain the
    # size of the primitives in the object.
    #    struct _longobject {
    #    long ob_refcnt;
    #    PyTypeObject *ob_type;
    #    size_t ob_size;
    #    long ob_digit[1];
    #};
    a = 16
    print("Size of variable a : ", sys.getsizeof(a))
    print("Size of Integer data type : ", sys.getsizeof(int))
    print("Size of String data type : ", sys.getsizeof(str()))
    print("Size of Double data type : ", sys.getsizeof(float()))

    input("")
if __name__ == '__main__':
    main()
