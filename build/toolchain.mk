# Copyright (C) 2008-2015, Marvell International Ltd.
# All Rights Reserved.

CROSS_COMPILE=arm-none-eabi-

AS    := $(CROSS_COMPILE)gcc
CC    := $(CROSS_COMPILE)gcc
LD    := $(CROSS_COMPILE)ld
AR    := $(CROSS_COMPILE)ar
OBJCOPY := $(CROSS_COMPILE)objcopy
STRIP := $(CROSS_COMPILE)strip

