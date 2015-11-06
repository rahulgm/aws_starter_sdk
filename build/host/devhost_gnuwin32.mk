# Copyright (C) 2008-2015, Marvell International Ltd.
# All Rights Reserved.

os_dir := mingw

t_mconf = mconf_not_on_gnuwin32
mconf_not_on_gnuwin32:
	@echo ""
	@echo "The 'menuconfig' option is not supported in GNUWin32"
	@echo "Please use 'make config' instead"
	@echo ""
	@false
