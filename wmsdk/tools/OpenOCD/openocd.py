#! /usr/bin/env python
# Copyright (C) 2015 Marvell International Ltd.
# All Rights Reserved.

# Load application to ram helper script
# Note: sys.stdout.flush() and sys.stderr.flush() are required for proper
# console output in eclipse

import os, sys, platform, subprocess
from sys import platform as _platform

IFC_FILE = "ftdi.cfg"
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))

# We define which as it may not be available on Windows
def which(program):
    if _platform == "win32" or _platform == "win64":
        program = program + '.exe'

    def is_exe(fpath):
        return os.path.isfile(fpath) and os.access(fpath, os.X_OK)

    fpath, fname = os.path.split(program)
    if fpath:
        if is_exe(program):
            return program
    else:
        for path in os.environ["PATH"].split(os.pathsep):
            path = path.strip('"')
            exe_file = os.path.join(path, program)
            if is_exe(exe_file):
                return exe_file
    return ""

if _platform == "linux" or _platform == "linux2":
    if (platform.machine() == "i686"):
        OPENOCD_PATH = which(SCRIPT_DIR + "/Linux/openocd")
    else:
        OPENOCD_PATH = which(SCRIPT_DIR + "/Linux/openocd64")
elif _platform == "darwin":
    OPENOCD_PATH = which("openocd")
elif _platform == "win32" or _platform == "win64":
    OPENOCD_PATH = which(SCRIPT_DIR + "/Windows/openocd")

if not len(OPENOCD_PATH):
    print "Error: Please install OpenOCD for your platform"
    sys.exit()

def print_usage():
	print ""
	print "Usage: ./openocd.py"
	sys.stdout.flush()

print "Using OpenOCD interface file", IFC_FILE
sys.stdout.flush()

subprocess.call ([OPENOCD_PATH, '-s', SCRIPT_DIR + '/interface', '-f', IFC_FILE, '-s', SCRIPT_DIR, '-f','openocd.cfg', '-c', 'gdb_port pipe; log_output openocd.log'])
sys.stderr.flush()
