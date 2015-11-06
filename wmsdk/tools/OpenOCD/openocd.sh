#!/bin/bash

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
	echo "Usage: $0"
	echo "Optional Usage:"
	echo "[-i | --interface] JTAG hardware interface name, supported ones
	are ftdi, jlink, amontec, malink. Default is ftdi."
}

while test -n "$1"; do
	case "$1" in
	--interface|-i)
		if [ $# -gt 1 ]
		then
			IFC_FILE=$2.cfg
		else
			print_usage
			exit 1
		fi
		shift 2
	;;
	esac
done

echo "Using OpenOCD interface file $IFC_FILE"

$OPENOCD_PATH -s $SCRIPT_DIR/interface -f $IFC_FILE -s $SCRIPT_DIR -f openocd.cfg -c "gdb_port pipe; log_output openocd.log"
