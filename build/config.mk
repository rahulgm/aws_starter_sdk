# Copyright (C) 2008-2015, Marvell International Ltd.
# All Rights Reserved.

################# Build Configuration

b-output-dir-y  = bin
b-autoconf-file = wmsdk/src/incl/autoconf.h
global-cflags-y += -include $(b-autoconf-file)
