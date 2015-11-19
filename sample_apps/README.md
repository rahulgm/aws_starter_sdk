Create new application code
====

To create a new application called ```my-new-app```, follow these steps :

* Create directory in called ```my-new-app```

   		  mkdir my-new-app
2. In this directory, create a ```Makefile```. Copy the following into
this file


		# Copyright (C) 2008-2015 Marvell International Ltd.
		# All Rights Reserved.
		#

		exec-y += my-new-app
		my-new-app-objs-y := src/main.c
		my-new-app-cflags-y := -I$(d)/src -DAPPCONFIG_DEBUG_ENABLE=1

		ifneq ($(wildcard $(d)/www),)
		my-new-app.app: my-new-app.ftfs
		my-new-app-ftfs-y     := $(d)/www
		my-new-app-ftfs-api-y := 100
		endif

		# Applications could also define custom linker files if required using following:
		#my-new-app-ld-y := /path/to/ldscript
		# Applications could also define custom board files if required using following:
		#my-new-app-board-y := /path/to/boardfile
* Put your source code in a directory called *src*

		$ tree
		.
		├── Makefile
		└── src
		    ├── aws_starter_root_ca_cert.h
		    └── main.c
		1 directory, 3 files

* From the root directory, build the your new app

		make APP=sample_apps/my-new-app
