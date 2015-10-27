#!/bin/bash

# Copyright (C) 2015 Marvell International Ltd.
# All Rights Reserved.

# Flash programming wrapper/helper script (using OpenOCD flash commands)

OS=`uname`
MACHINE=`uname -m`
IFC_FILE=ftdi.cfg
SCRIPT_DIR=`dirname $0`

if [ $OS == "Linux" ]; then
	if [ $MACHINE == "i686" ]; then
		OPENOCD_PATH="$SCRIPT_DIR/Linux/openocd"
	else
		OPENOCD_PATH="$SCRIPT_DIR/Linux/openocd64"
	fi
else
	if [ $OS == "Darwin" ]; then
		OPENOCD_PATH=`which openocd`
	else
		OPENOCD_PATH="$SCRIPT_DIR/Windows/openocd"
	fi
fi

if [ ! -e "$OPENOCD_PATH" ]; then
	echo "Please install OpenOCD for your machine"
	exit 1
fi

print_usage() {
	echo ""
	echo "Usage:"
	echo "$0"
	echo " [--mcufw </path/to/mcufw>]"
	echo "          Write MCU firmware binary <bin> to flash"
	echo " [< -f | --flash> </path/to/flash_blob>]"
	echo "          Program entire flash"
	echo " [-h | --help] "
	echo "          Display usage"
}

while test -n "$1"; do
	case "$1" in
	--mcufw)
		if [ $# -gt 1 ]; then
			if [ $OS == "Linux" ] || [ $OS == "Darwin" ]; then
				BIN_FILE=$2
			else
				BIN_FILE=`cygpath -m $2`
			fi
		else
			print_usage
			exit 1
		fi
		shift 2
	;;
	--flash|-f)
		if [ $# -gt 1 ]; then
			if [ $OS == "Linux" ] || [ $OS == "Darwin" ]; then
				FACTORY_FILE=$2
			else
				FACTORY_FILE=`cygpath -m $2`
			fi
		else
			print_usage
			exit 1
		fi
		shift 2
	;;
	*)
		print_usage
		exit 1
	;;
	esac
done

# Generate a config file to instruct wmcore.cfg to configure flash banks
echo "# This is an auto-generated config file" > config.cfg
echo "set CONFIG_FLASH flash" >> config.cfg

# Flashes a file using OpenOCD flash commands
# Arguments: $1 = file to flash, $2 = address in flash, $3 = flashing message
flash_file () {
	if [ ! -z $1 ]; then
		if [ ! -e $1 ]; then
			echo "Error: File $1 not found"
			exit 1
		fi
		echo "$3..."
		$OPENOCD_PATH -s $SCRIPT_DIR/interface -f $IFC_FILE -s $SCRIPT_DIR -f config.cfg -f openocd.cfg -c init -c "program_image $2 $1" -c shutdown
		if [ $? == 0 ]; then
			echo "$3 done..."
		else
			echo "$3 failed..."
		fi
	fi
}

# Flash factory blob
flash_file "$FACTORY_FILE" 0x0 "Resetting flash to factory settings"

# Flash mcufw
flash_file "$BIN_FILE" 0x10000 "Writing MCU firmware to flash"

rm -f config.cfg
