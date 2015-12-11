#! /usr/bin/env python
# Copyright (C) 2015 Marvell International Ltd.
# All Rights Reserved.

# Load application to ram helper script
# Note: sys.stdout.flush() and sys.stderr.flush() are required for proper
# console output in eclipse

import os, sys, platform, getopt, subprocess
from sys import platform as _platform

SEMIHOST = False
IFC_FILE = "ftdi.cfg"
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
READELF = ""

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
	print "Usage: ./ramload.py <app.axf> [options]"
	print "Optional Usage:"
	print "[-s | --semihosting] Enable semihosting based console output"
	sys.stdout.flush()

if len(sys.argv) <= 1:
    print_usage()
    sys.exit()

try:
    opts, args = getopt.getopt(sys.argv[2:], "s", ["semihosting"])
    if len(args):
        print_usage()
        sys.exit()
except getopt.GetoptError:
    sys.exit()

if ("-s" in sys.argv) or ("--semihosting" in sys.argv):
    SEMIHOST = True

FILE = os.path.abspath(sys.argv[1]).replace('\\', '/')

if (os.path.isfile(FILE) == False):
    print "Error: Could not find file", FILE
    print_usage()
    sys.exit()

READELF = which('arm-none-eabi-readelf')

if not len(READELF):
    READELF = which('readelf')
    if not len(READELF):
        print "Error: readelf utility not found, please install binutils or appropriate package"
        sys.exit()

readelf_output = subprocess.Popen([READELF, FILE, '-h'],stdout=subprocess.PIPE)
readelf_output = readelf_output.stdout.read()

if ('Entry point' in readelf_output) is False:
    sys.exit("Error: Not an ELF file")

for item in readelf_output.split('\n'):
    if 'Entry point' in item:
        entry_point = item.strip()
        i = entry_point.index(':')
        entry_point = entry_point[i+1:].strip()

if ((int(entry_point, 16) & 0x1f000000) == 0x1f000000):
    print "Error: Provided firmware image (.axf) is XIP enabled and hence ramload is not possible, please use flashprog to flash the image (.bin)"
    sys.exit()

print "Using OpenOCD interface file", IFC_FILE
sys.stdout.flush()

if SEMIHOST is False:
    subprocess.call ([OPENOCD_PATH, '-s', SCRIPT_DIR + '/interface', '-f', IFC_FILE, '-s', SCRIPT_DIR, '-f','openocd.cfg', '-c', 'init', '-c', 'load ' + FILE + ' ' + str(entry_point) + ' ', '-c', 'shutdown'])
else:
    subprocess.call ([OPENOCD_PATH, '-s', SCRIPT_DIR + '/interface', '-f', IFC_FILE, '-s', SCRIPT_DIR, '-f','openocd.cfg', '-c', 'init', '-c', 'sh_load ' + FILE + ' ' + str(entry_point)])
sys.stderr.flush()
