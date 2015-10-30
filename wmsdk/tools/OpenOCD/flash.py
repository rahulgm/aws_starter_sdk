#! /usr/bin/env python
# Copyright (C) 2015 Marvell International Ltd.
# All Rights Reserved.

# Flash programming wrapper/helper script (using OpenOCD flash commands)

from sys import platform as _platform
import os, sys, platform, getopt

IFC_FILE = "ftdi.cfg"
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))

if _platform == "linux" or _platform == "linux2":
    if (platform.machine() == "i686"):
        OPENOCD_PATH = os.path.dirname(os.path.abspath(__file__)) + "/Linux/openocd"
    else:
        OPENOCD_PATH = os.path.dirname(os.path.abspath(__file__)) + "/Linux/openocd64"
elif _platform == "darwin":
    OPENOCD_PATH = os.popen('which openocd').read().rstrip()
elif _platform == "win32" or _platform == "win64":
    OPENOCD_PATH = os.path.dirname(os.path.abspath(__file__)) + "/Windows/openocd.exe"

if (os.path.exists(OPENOCD_PATH) == False):
    print "Error: Please install OpenOCD for your platform"
    sys.exit()

def create_config():
    cfile = open ('config.cfg','w')
    cfile.write("# This is an auto-generated config file\n")
    cfile.write("set CONFIG_FLASH flash\n")
    cfile.close()

def cleanup():
    if (os.path.exists("config.cfg")):
        os.remove("config.cfg")

def exit():
    print_usage()
    cleanup()
    sys.exit()

def print_usage():
    print ""
    print "Usage:"
    print sys.argv[0]
    print " [--mcufw </path/to/mcufw>]"
    print "          Write MCU firmware binary <bin> to flash"
    print " [<-f | --flash> </path/to/flash_blob>]"
    print "          Program entire flash"
    print " [-h | --help] "
    print "          Display usage"

def flash_file(addr, msg, upload_file):
    if (os.path.isfile(upload_file) == False):
        print "Error: Could not find file", upload_file
        exit()
    print msg
    p = os.system(OPENOCD_PATH + " -s " + SCRIPT_DIR + "/interface -f " + IFC_FILE + " -s " + SCRIPT_DIR + " -f config.cfg -f openocd.cfg -c init -c \"program_image "+ addr + " " + upload_file + "\" -c shutdown")
    if (p==0):
        print msg + " done..."
    else:
        print msg + " failed..."

if len(sys.argv) <= 1:
    exit()

else:
    try:
        opts, args = getopt.getopt(sys.argv[1:], "f:", ["flash=","mcufw="])
    except getopt.GetoptError:
        exit()

    create_config()

    if ("-f" in sys.argv) or ("--flash" in sys.argv):
	try:
            index = sys.argv.index("-f")
        except ValueError:
            index = sys.argv.index("--flash")
        flash_file("0x0", "Resetting flash to factory settings", os.path.abspath(sys.argv[index + 1]).replace('\\', '/'))

    if ("--mcufw" in sys.argv):
        index = sys.argv.index("--mcufw")
        flash_file("0x10000", "Writing MCU firmware to flash", os.path.abspath(sys.argv[index + 1]).replace('\\', '/'))

    cleanup()
