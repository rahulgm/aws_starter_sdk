# Copyright (C) 2008-2015, Marvell International Ltd.
# All Rights Reserved.

##################### Operating Variables
# The list of all the libraries in the system
b-libs-y :=
b-libs-paths-y :=
# The list of all the programs in the system
b-exec-y :=
b-exec-paths-y :=
# The list of all artifacts to clean
b-clean-y :=
# The list of dependencies
b-deps-y :=
# The output directory for this configuration, this is deferred on-purpose
b-output-dir-y ?=
# The list of object directories that should be created
b-object-dir-y :=
# The list of libraries that are pre-built somewhere outside
b-prebuilt-libs-y :=

# This is path of output directory for libraries
b-libs-output-dir-y ?= $(b-output-dir-y)/libs

#####################
##################### Subdirectory Inclusions
# The wildcard, let's users pass options like
# subdir-y += mydir/Makefile.ext
#    If the Makefile has a different name, that is facilitated by the 'if block'.
#    The standard case of handling 'directory' names is facilitated by the 'else block'.
#
define inc_mak
 ifeq ($(wildcard $(1)/Makefile),)
     d=$(dir $(1))
     include $(1)
 else
     d=$(1)
     include $(1)/Makefile
 endif
 include build/post-subdir.mk
endef

# Include rules.mk from all the subdirectories and process them
$(foreach dir,$(subdir-y),$(eval $(call inc_mak,$(dir))))

# Create output directories for all the objects
ignore := $(shell $(t_mkdir) -p $(b-object-dir-y))

.SUFFIXES:

#####################
##################### Object (.o) Creation

# All the prog-objs-y have prog-cflags-y as the b-trgt-cflags-y variable
$(foreach l,$(b-exec-y),$(eval $($(l)-objs-y): b-trgt-cflags-y := $($(l)-cflags-y)))
# All the lib-objs-y have lib-cflags-y as the b-trgt-cflags-y
# variable. This allows configuration flags specific only to certain
# libraries/programs. Only that may be dangerous.
$(foreach l,$(b-libs-y),$(eval $($(l)-objs-y): b-trgt-cflags-y := $($(l)-cflags-y)))

# The command for converting a .c to .o
# arg1 the C filename
# arg2 the object filename
#
# This file has the default rule that maps an object file from the standard
# build output directory to the corresponding C file in the src directory
#
define b-cmd-c-to-o
  $(AT)$(CC) $(b-trgt-cflags-y) $(global-cflags-y) -o $(2) -c $(1) -MMD
  @echo " [cc] $@"
endef

# Rule for creating an object file
# strip off the $(b-output-dir-y) from the C files pathname
$(b-output-dir-y)/%.o: %.c $(b-autoconf-file)
	$(call b-cmd-c-to-o,$<,$@)
$(b-output-dir-y)/%.o: %.S $(b-autoconf-file)
	$(call b-cmd-c-to-o,$<,$@)
$(b-output-dir-y)/%.o: %.s $(b-autoconf-file)
	$(call b-cmd-c-to-o,$<,$@)

-include $(b-deps-y)

#####################
##################### Library (.a) Creation

# Rule for creating a library
# Given liba
#  - create $(b-output-dir-y)/liba.a
#  - from $(liba-objs-y)
#  - create a target liba.a that can be used to build the lib
define create_lib
  $(1).a: $(b-libs-output-dir-y)/$(1).a

  $(b-libs-output-dir-y)/$(1).a: $$($(1)-objs-y)
	$$(AT)rm -f $$@
	$$(AT)$(t_mkdir) -p $(b-libs-output-dir-y)
	$$(AT)$$(AR) cru $$@ $$^
	@echo " [ar] $$@"

 .PHONY: $(1).a.clean
  $(1).a.clean:
	$$(AT)rm -f $(b-libs-output-dir-y)/$(1).a $$($(1)-objs-y) $$($(1)-objs-y:.o=.d)
	@echo " [clean] $(1)"

endef

$(foreach l,$(b-libs-y), $(eval $(call create_lib,$(l))))

.PHONY: all-libs
all-libs: $(foreach l,$(b-libs-y),$(l).a)


define b-cmd-axf
  $(AT)$(CC) -o $(2) $($(1)-objs-y) $($(1)-lflags-y) $(b-trgt-cflags-y) -Wl,--start-group $($(1)-extralibs-y) $(b-libs-paths-y) $(b-prebuilt-libs-y) -Wl,--end-group -T $($(1)-ld-y) -nostartfiles -Xlinker -M -Xlinker -Map=$(2:%.axf=%.map) -Xlinker --cref -Xlinker --gc-sections $(LDFLAGS-y) $(global-cflags-y)
  @echo " [axf] $(abspath $(2))"
endef

#####################
##################### Program (.axf) Creation
# Rule for creating a program
# Given myprog
#  - create $(b-output-dir-y)/myprog
#  - from $(myprog-objs-y)
#  - add dependency on all the libraries, linker scripts
#  - create a target app_name.app which can be used to build the app

define create_prog
  ifndef $(1)-ld-y
     $(1)-ld-y := $$(global-ld-y)
  endif

# $(1)-dir-y is created in build/post-subdir.mk
# by the name $(l)-dir-y

  %/$($(1)-dir-y)/$$(notdir $${$(1)-board-y:.c=.o}): $$($(1)-board-y) $(b-autoconf-file)
	$$(call b-cmd-c-to-o,$$<,$$@)

  $(b-output-dir-y)/$(1):$(1).app

# $(1)-output-dir-y is  $(b-output-dir-y)/<board-name>

  $(1).app: $($(1)-output-dir-y)/$(1).axf

  $($(1)-output-dir-y)/$(1).axf: $$($(1)-objs-y) $$(b-libs-paths-y) $$($(1)-ld-y) $$(b-prebuilt-libs-y)
	$$(AT)$(t_mkdir) -p $($(1)-output-dir-y)
	$$(call b-cmd-axf,$(1),$$@)


 .PHONY: $(1).app.clean
  $(1).app.clean:
	$$(AT)rm -f $($(1)-output-dir-y)/$(1).axf $($(1)-output-dir-y)/$(1).map $$($(1)-objs-y)  $$($(1)-objs-y:.o=.d)
	@echo " [clean] $(1)"
endef

$(foreach p,$(b-exec-y), $(eval $(call create_prog,$(p))))

#####################
##################### Miscellaneous handling

# Rule for clean
#
clean:
ifneq ($(APP_PATH),)
	@echo "[Cleaning] $(b-output-dir-y)/$(APP_PATH)"
	$(AT) rm -rf $(b-output-dir-y)/$(APP_PATH)
	$(AT) rm -f $(b-output-dir-y)/$(shell echo $(APP_PATH) | sed -s 's/.*\///').*
else
	$(AT)rm -f $(b-clean-y)
	@echo "[clean] all"
endif

# Rule for NOISY Output
ifneq ($(NOISY),1)
AT=@
endif

FORCE:
