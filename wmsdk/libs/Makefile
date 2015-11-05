# Copyright (C) 2008-2015, Marvell International Ltd.
# All Rights Reserved.

# makefile to include pre-built libraries present in current directory

prebuilt-libs-y += $(foreach l,$(wildcard wmsdk/libs/*.a), $(patsubst wmsdk/libs/%.a,%,$(l)))
