#!/usr/bin/python

import subprocess

def run_program(path):
    p = subprocess.Popen(path, shell=True, stdout=subprocess.PIPE)
    p.communicate()[0]

    if (p.returncode > 1):
        return False
    return True

# Program runs all the programs in the directory
beacon = './conways_sim FILE_INIT conway_init_file_beacon.txt test_beacon.txt'
blinker = './conways_sim FILE_INIT conway_init_file_blinker.txt test_blinker.txt'
toad = './conways_sim FILE_INIT conway_init_file_toad.txt test_toad.txt'
blinker_on_boarders = './conways_sim FILE_INIT conway_init_file_blinker_on_boarders.txt test_blinker_on_boarders.txt'
glider = './conways_sim FILE_INIT conway_init_file_glider.txt test_glider.txt'

run_program(beacon)
run_program(blinker)
run_program(toad)
run_program(blinker_on_boarders)
run_program(glider)


