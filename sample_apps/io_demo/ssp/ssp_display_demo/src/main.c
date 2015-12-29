/*
 *  Copyright (C) 2008-2015, Marvell International Ltd.
 *  All Rights Reserved.
 */

/**
 *
 * Summary:
 * Simple display application to display red, blue and green colors.
 *
 * Pins Used :
 * SSP1_CLK - IO_11
 * SSP1_FRM - IO_12
 * SSP1_TX  - IO_13
 *
 * DISPLAY_RESET_PIN - IO_42
 * DISPLAY_DCX_PIN - IO_43
 * DISPLAY_BACKLIGHT_PIN - IO_24
 *
 * See display.h to change the pins
 */

#include <wm_os.h>
#include "display.h"

pixel colors[] = {
		{.r = 0x1F, .g = 0x00, .b = 0x00}, /* Red */
		{.r = 0x00, .g = 0x1F, .b = 0x00}, /* Green */
		{.r = 0x00, .g = 0x00, .b = 0x1F}, /* Blue */
};
int no_colors = sizeof(colors) / sizeof(pixel);

pixel frame_buf[DISPLAY_X_RES * DISPLAY_Y_RES];

void change_display_data(pixel p)
{
	int i, j;

	for (i = 0; i < DISPLAY_Y_RES; i++)
		for (j = 0; j < DISPLAY_X_RES; j++) {
			frame_buf[(i * DISPLAY_X_RES) + j].r = p.r;
			frame_buf[(i * DISPLAY_X_RES) + j].g = p.g;
			frame_buf[(i * DISPLAY_X_RES) + j].b = p.b;
		}
}

/**
 * All application specific initializations are performed here.
 */
int main(void)
{
	int i = 0;

	/* Initialize wmstdio console */
	wmstdio_init(UART0_ID, 0);
	wmprintf("SSP Display Application Started\r\n");

	display_init();

	while (1) {
		change_display_data(colors[i++ % no_colors]);
		display_update(frame_buf);
		os_thread_sleep(2000);
	}

	return 0;
}
