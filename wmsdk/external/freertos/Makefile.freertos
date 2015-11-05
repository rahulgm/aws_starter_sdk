# Copyright (C) 2008-2015, Marvell International Ltd.
# All Rights Reserved.

libs-y += libfreertos
libfreertos-objs-y := Source/list.c Source/queue.c Source/tasks.c
libfreertos-objs-y += Source/timers.c Source/portable/GCC/ARM_CM3/port.c
libfreertos-objs-y += Source/portable/MemMang/heap_4.c


libfreertos-cflags-$(DEBUG_HEAP) += -DDEBUG_HEAP
libfreertos-cflags-$(CONFIG_ENABLE_STACK_OVERFLOW_CHECK) += -DCONFIG_ENABLE_STACK_OVERFLOW_CHECK

