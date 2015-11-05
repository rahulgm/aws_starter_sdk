# Copyright (C) 2008-2015, Marvell International Ltd.
# All Rights Reserved.

os_dir := Windows
ifneq ($(findstring :, $(APP)),)
CYG := /cygdrive
APP_PATH := $(realpath $(CYG)/$(shell echo "$(APP)" | sed -e 's/\://g'))
endif

# $(CURDIR) should be SDK directory path
CC    := $(CURDIR)/build/toolchains/convertcc.sh
LD    := $(CURDIR)/build/toolchains/convertld.sh
AR    := $(CURDIR)/build/toolchains/convertar.sh

$(t_mconf): mconf_not_on_cygwin
mconf_not_on_cygwin:
	@echo ""
	@echo "The 'menuconfig' option is not supported in Cygwin"
	@echo "Please use 'make config' instead"
	@echo ""
	@false
