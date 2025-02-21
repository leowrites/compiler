#!/usr/bin/python3
#
# Assignment 4 Public tester
#
# Tests a program path and against an expected output
#

import tester
import re
import os, sys
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
# Change path to minicc here
exec_path = [SCRIPT_DIR + '/../build/src/minicc']

test_cases = [
    ['public_tests/var.c', b'', 1],
    ['public_tests/bool_fail.c', b'Redefinition of variable/parameter "z" in the same scope! (4:5)\nVariable and the assignment expression do not have the same type! (9:4)\n"+" opcode must have int type operand! (11:4)\n"&&"/"||" opcode must have bool operand! (13:8)\nFunction has return type "bool", but the returned expression has type "int"! (14:8)\nConditional expression in if statement has non-bool type! (13:8)\n', 2],
    ['public_tests/index_fail.c', b'Indexing an non-array variable! (7:4)\nBreak statement must appear inside a for statement! (12:8)\nThe function "main()" need to return a value at its end! (6:0)\n', 2],
]
tmp_files = ['output.bc']

def convert_bytes_to_set(b):
    s = b.decode("utf-8") 
    ss = s.split('\n')
    ss = set(ss)
    if '' in ss:
        ss.remove('')
    return ss

def main():
    test = tester.Core('Assignment 4 Public Tester', 5)

    # Public tests
    for case in test_cases:
        test_path = exec_path + [case[0]]
        # Supply the executable and test program path
        test.start_test(test_path)
        # Check the program output against a string and assign a mark
        test.actual = convert_bytes_to_set(test.actual)
        test.check_output(convert_bytes_to_set(case[1]), case[2])
        test.clean_up(tmp_files)

if __name__ == '__main__':
    main()
    
