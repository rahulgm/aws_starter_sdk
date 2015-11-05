/*! \file wmerrno.h
 *  \brief Error Management
 *
 *  Copyright 2008-2015, Marvell International Ltd.
 *  All Rights Reserved.
 *
 */
#ifndef WM_ERRNO_H
#define WM_ERRNO_H

/* Get the module index number from error code (4th byte from LSB)*/
#define get_module_base(code) ((code&0xF000)>>12)

/* Get notifier message type i.e Error, Warning or Info (3rd byte from LSB)*/
#define get_notifier_msg_type(code) ((code&0x0F00)>>8)

/* Get module notifier code (2nd and 1st byte from LSB)*/
#define get_code(code) (code&0xFF)

#define MOD_ERROR_START(x)  (x << 12 | 0)
#define MOD_WARN_START(x)   (x << 12 | 1)
#define MOD_INFO_START(x)   (x << 12 | 2)

/* Create Module index */
#define MOD_GENERIC    0
/** JSON module index  */
#define MOD_JSON       7
/** Last module index .Applications can define their own modules beyond this */
#define MOD_ERR_LAST	50

/* Globally unique success code */
#define WM_SUCCESS 0

enum wm_errno {
	/* First Generic Error codes */
	WM_GEN_E_BASE = MOD_ERROR_START(MOD_GENERIC),
	WM_FAIL,
	WM_E_PERM,   /* Operation not permitted */
	WM_E_NOENT,  /* No such file or directory */
	WM_E_SRCH,   /* No such process */
	WM_E_INTR,   /* Interrupted system call */
	WM_E_IO,     /* I/O error */
	WM_E_NXIO,   /* No such device or address */
	WM_E_2BIG,   /* Argument list too long */
	WM_E_NOEXEC, /* Exec format error */
	WM_E_BADF,   /* Bad file number */
	WM_E_CHILD,  /* No child processes */
	WM_E_AGAIN,  /* Try again */
	WM_E_NOMEM,  /* Out of memory */
	WM_E_ACCES,  /* Permission denied */
	WM_E_FAULT,  /* Bad address */
	WM_E_NOTBLK, /* Block device required */
	WM_E_BUSY,   /* Device or resource busy */
	WM_E_EXIST,  /* File exists */
	WM_E_XDEV,   /* Cross-device link */
	WM_E_NODEV,  /* No such device */
	WM_E_NOTDIR, /* Not a directory */
	WM_E_ISDIR,  /* Is a directory */
	WM_E_INVAL,  /* Invalid argument */
	WM_E_NFILE,  /* File table overflow */
	WM_E_MFILE,  /* Too many open files */
	WM_E_NOTTY,  /* Not a typewriter */
	WM_E_TXTBSY, /* Text file busy */
	WM_E_FBIG,   /* File too large */
	WM_E_NOSPC,  /* No space left on device */
	WM_E_SPIPE,  /* Illegal seek */
	WM_E_ROFS,   /* Read-only file system */
	WM_E_MLINK,  /* Too many links */
	WM_E_PIPE,   /* Broken pipe */
	WM_E_DOM,    /* Math argument out of domain of func */
	WM_E_RANGE,  /* Math result not representable */
	WM_E_CRC,    /* Error in CRC check */
	WM_E_UNINIT  /* Module is not yet initialized */
};

#endif /* ! WM_ERRNO_H */
