# Copyright (C) 2008-2015, Marvell International Ltd.
# All Rights Reserved.

# .config should already have been included before this

# Step 0: Default target is all
all:

# Step 1: Initialize the toolchain
include build/toolchain.mk

# Step 2: Setup our own variables

######## Default variables
board_name-$(CONFIG_CPU_MW300) := mw302_rd
BOARD ?= $(board_name-y)
BOARD_FILE ?= wmsdk/src/boards/$(BOARD).c

arch_name-$(CONFIG_CPU_MW300) := mw300

######## Compiler and Linker Flags
arch_target-$(CONFIG_CPU_MC200) := cortex-m3
arch_target-$(CONFIG_CPU_MW300) := cortex-m4

global-cflags-y := -mthumb -g -Os -fdata-sections -ffunction-sections -ffreestanding -MMD -Wall \
          -fno-strict-aliasing -fno-common  -mcpu=$(arch_target-y)
LDFLAGS-$(CONFIG_CPU_MW300) += -Xlinker --defsym=_rom_data=64

global-cflags-y += \
	 -Iwmsdk/src/incl/libc                            \
	 -Iwmsdk/src/incl/sdk                             \
	 -Iwmsdk/src/incl/sdk/drivers                     \
	 -Iwmsdk/src/incl/sdk/drivers/$(arch_name-y)      \
	 -Iwmsdk/src/incl/sdk/drivers/$(arch_name-y)/regs \

global-cflags-y-$(CONFIG_ENABLE_LTO) += -flto -ffat-lto-objects
LDFLAGS-$(CONFIG_ENABLE_LTO) += -Xlinker -flto

######### Tools
t_axf2fw = $(shell which wmsdk/tools/bin/$(os_dir)/axf2firmware | tail -1)
t_mkdir := $(shell which mkdir | tail -1)

######### Common Linker File Handling
# This can be overriden from the apps
global-ld-y := build/toolchains/GNU/$(arch_name-y).ld

######### XIP Handling
ifeq ($(XIP), 1)
  global-ld-y := build/toolchains/GNU/$(arch_name-y)-xip.ld
  global-ld-$(CONFIG_ENABLE_MCU_PM3) := build/toolchains/GNU/$(arch_name-y)-xip-pm3.ld
  global-cflags-y += -DCONFIG_XIP_ENABLE
endif

# Step 3: Handle development host specific options
# devhost is towards the end, so it can override stuff defined from above
include build/host/devhost.mk

# Step 4: Manage SDK configuration
# Include out configuration artifacts
include build/config.mk
