/*
 *  Copyright (C) 2008-2015, Marvell International Ltd.
 *  All Rights Reserved.
 */

#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#define DISPLAY_DEBUG
#ifdef DISPLAY_DEBUG
#define DISPLAY_LOG(...) ll_log("[display] " __VA_ARGS__)
#else
#define DISPLAY_LOG(...)
#endif

/* SSP port to use for Display Serial Communication */
#define DISPLAY_SSP SSP1_ID

/* Display Reset Pin */
#define DISPLAY_RESET_PIN GPIO_42
#define DISPLAY_RESET_PIN_MUX GPIO42_GPIO42

/* Command/Data Pin for Serial Communication */
#define DISPLAY_DCX_PIN GPIO_43
#define DISPLAY_DCX_PIN_MUX GPIO43_GPIO43

/* Back-light Pin */
#define DISPLAY_BKL_PIN GPIO_24
#define DISPLAY_BKL_PIN_MUX GPIO24_GPIO24

/* Pixel Format 5:6:5 RGB */
typedef struct pixel {
	uint8_t r : 5;
	uint8_t g : 6;
	uint8_t b : 5;
} __attribute__ ((packed)) pixel;

#define DISPLAY_X_RES 128
#define DISPLAY_Y_RES 160
#define DISPLAY_BUFF_SIZE (DISPLAY_X_RES * DISPLAY_Y_RES * sizeof(pixel))

/* Initialize display panel */
int display_init(void);

/* Update the display frame-buffer */
void display_update(void *display_buf);

/* Enter or exit display sleep mode */
typedef enum {
	SLEEP_ENTER,
	SLEEP_EXIT
} sleep_t;

void display_sleep(sleep_t mode);

/* Switch on or off display back-light */
typedef enum {
	BACKLIGHT_OFF,
	BACKLIGHT_ON
} backlight_t;

void display_backlight(backlight_t state);

#endif /* __DISPLAY_H__ */
