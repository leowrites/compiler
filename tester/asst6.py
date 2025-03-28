#!/usr/bin/python3
#
# Assignment 6 Private tester
#
# Tests a program path and against an expected output
#

import tester
import os, sys

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))

# Change path to minicc here
local_path = SCRIPT_DIR + '/../build/'
bcgen_cmd = [local_path + '/src/minicc', '-o', 'output.bc']
llgen_cmd = ['llvm-dis-15','output.bc','-o','output.ll']
optgen_cmd = ['opt-15', '-O0', '-load', local_path + '/src/liballoca2reg.so', 'output.bc', '-o', 'output_opt.bc', '-enable-new-pm=0']
lloptgen_cmd = ['llvm-dis-15','output_opt.bc','-o','output_opt.ll']
execgen_cmd = ['clang-15', 'output_opt.bc', local_path + '/minicio/libminicio.a', '-o', 'output_opt']
run_cmd = ['./output_opt']
test_cases = [
    ['public_tests/conds.c', 0, b'None', b'0 ', 2],
    ['public_tests/queen.c', 1, b'5', b'0 2 4 1 3 \n0 3 1 4 2 \n1 3 0 2 4 \n1 4 2 0 3 \n2 0 3 1 4 \n2 4 1 3 0 \n3 0 2 4 1 \n3 1 4 2 0 \n4 1 3 0 2 \n4 2 0 3 1 \n', 2],
    ['public_tests/hanoi.c', 1, b'5\n', b'1 0 2 \n2 0 1 \n1 2 1 \n3 0 2 \n1 1 0 \n2 1 2 \n1 0 2 \n4 0 1 \n1 2 1 \n2 2 0 \n1 1 0 \n3 2 1 \n1 0 2 \n2 0 1 \n1 2 1 \n5 0 2 \n1 1 0 \n2 1 2 \n1 0 2 \n3 1 0 \n1 2 1 \n2 2 0 \n1 1 0 \n4 1 2 \n1 0 2 \n2 0 1 \n1 2 1 \n3 0 2 \n1 1 0 \n2 1 2 \n1 0 2 \n', 2],
]
tmp_files = ['output.bc','output_opt.bc','output_opt','output.ll','output_opt.ll']
tmp_dir = ['build/']
def main():
    test = tester.Core('Assignment 6 Public Tester', 6)

    # Public tests
    for case in test_cases:
        cmds = []
        cmds.append(bcgen_cmd + [case[0]])
        cmds.append(llgen_cmd)
        cmds.append(optgen_cmd)
        cmds.append(lloptgen_cmd)
        cmds.append(execgen_cmd)
        test.generate_exec(cmds)
        datain = case[2]
        #Supply the executable and test program path
        test.exec_test(run_cmd, datain, case[1])
        # Check the program output against a string and assign a mark
        test.check_output(case[3], 1)
        test.opt_check_output('output.ll','output_opt.ll',case[4]-1)
        test.clean_up(tmp_files)

if __name__ == '__main__':
    main()
    
