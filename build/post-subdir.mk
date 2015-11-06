# Copyright (C) 2008-2015, Marvell International Ltd.
# All Rights Reserved.

# This file will be iteratively included after including every
# Makefile
# This file should santise all variables and reset them to null

# Note: All variables in this file should be defined as immediate
# variables in the rules.mk

### Libraries
# Fix liba-objs-y as follows
#  - include the full path to the directory
#  - change .c with .o
#  - add $(b-output-dir-y)/ as a prefix, so all the objects are in a directory together
#    note that this requires a corresponding stripping of $(b-output-dir-y)/ in the %.o:%.c rule
$(foreach l,$(libs-y),$(eval $(l)-objs-y := $(foreach s,$($(l)-objs-y),$(b-output-dir-y)/$(d)/$(s:%.c=%.o))))
$(foreach l,$(libs-y),$(eval $(l)-objs-y := $(foreach s,$($(l)-objs-y),$(s:%.S=%.o))))
$(foreach l,$(libs-y),$(eval $(l)-objs-y := $(foreach s,$($(l)-objs-y),$(s:%.s=%.o))))

b-clean-y += $(foreach l,$(libs-y),$($(l)-objs-y))
b-object-dir-y += $(foreach l,$(libs-y),$(sort $(dir $($(l)-objs-y))))

# Copy the libs-y in a separate collection variable
b-libs-paths-y += $(foreach l,$(libs-y),$(b-libs-output-dir-y)/$(l).a)
b-clean-y +=      $(foreach l,$(libs-y),$(b-libs-output-dir-y)/$(l).a)

# Copy the dependencies
b-deps-y +=  $(foreach l,$(libs-y),$(foreach s,$($(l)-objs-y),$(s:%.o=%.d)))
b-clean-y += $(foreach l,$(libs-y),$(foreach s,$($(l)-objs-y),$(s:%.o=%.d)))

# Handle prebuilt-libs-y
b-prebuilt-libs-y += $(foreach l,$(prebuilt-libs-y),$(d)/$(l).a)

# Nullify libs-y
b-libs-y += $(libs-y)
libs-y=
prebuilt-libs-y=

### Programs
# Board file handling
#  - use myprog-board-y if defined by myprog, otherwise assign BOARD_FILE to
#    myprog-board-y
#  - add board file to myprog-objs-y
$(foreach l,$(exec-y),$(eval $(l)-board-y ?= $(BOARD_FILE)))
$(foreach l,$(exec-y),$(eval $(l)-objs-y += $(notdir $($(l)-board-y))))
$(foreach l,$(exec-y),$(eval $(l)-output-dir-y := $(b-output-dir-y)/$(notdir ${$(l)-board-y:.c=})))

# Fix myprog-objs-y as follows
#  - include the full path to the directory
#  - change .c with .o
#  - add $(b-output-dir-y)/ as a prefix, so all the objects are in a directory together
#    note that this requires a corresponding stripping of $(b-output-dir-y)/ in the %.o:%.c rule
$(foreach l,$(exec-y),$(eval $(l)-objs-y := $(foreach s,$($(l)-objs-y),$(b-output-dir-y)/$(d)/$(s:%.c=%.o))))
$(foreach l,$(exec-y),$(eval $(l)-objs-y := $(foreach s,$($(l)-objs-y),$(s:%.s=%.o))))
$(foreach l,$(exec-y),$(eval $(l)-objs-y := $(foreach s,$($(l)-objs-y),$(s:%.S=%.o))))
b-clean-y += $(foreach l,$(exec-y),$($(l)-objs-y))
b-object-dir-y += $(foreach l,$(exec-y),$(sort $(dir $($(l)-objs-y))))

# Copy exec-y in a separate collection variable
b-exec-paths-y += $(foreach l,$(exec-y),$(b-output-dir-y)/$(l))
b-clean-y += $(foreach l,$(exec-y),$($(l)-output-dir-y)/$(l).bin $($(l)-output-dir-y)/$(l).axf $($(l)-output-dir-y)/$(l).ftfs $($(l)-output-dir-y)/$(l).map)


# Copy the dependencies
b-deps-y +=  $(foreach l,$(exec-y),$(foreach s,$($(l)-objs-y),$(s:%.o=%.d)))
b-clean-y += $(foreach l,$(exec-y),$(foreach s,$($(l)-objs-y),$(s:%.o=%.d)))
$(foreach l,$(exec-y),$(eval $(l)-dir-y := $(d)))
b-exec-y += $(exec-y)
exec-y=
