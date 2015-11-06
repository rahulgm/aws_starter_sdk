# Copyright (C) 2008-2015, Marvell International Ltd.
# All Rights Reserved.

# Pull in the standard set of rules
include build/rules.mk

# Add our own (aws_starter_sdk-specific) rules on top

##################### Program (.bin) creation
define create_aws_sdk_prog
  $(1).app: $($(1)-output-dir-y)/$(1).bin

  $($(1)-output-dir-y)/$(1).bin: $($(1)-output-dir-y)/$(1).axf $$(t_axf2fw)
	$$(AT)$$(t_axf2fw) $$< $$@
	@echo " [bin] $$(abspath $$@)"

  $(1).app.clean: $(1).app.clean_bin

  .PHONY: $(1).app.clean_bin
  $(1).app.clean_bin:
	$$(AT)rm -f  $($(1)-output-dir-y)/$(1).bin
endef

$(foreach p,$(b-exec-y), $(eval $(call create_aws_sdk_prog,$(p))))
#####################

