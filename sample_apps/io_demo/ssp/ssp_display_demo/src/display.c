/*
 *  Copyright (C) 2008-2015, Marvell International Ltd.
 *  All Rights Reserved.
 */

/**
 * Summary:
 * Simple display driver which communicates over SSP driver as master.
 */

#include <wm_os.h>
#include <mdev_ssp.h>
#include <mdev_gpio.h>
#include "display.h"

/* SSP mdev handle */
static mdev_t *ssp;

#define SSP_FREQ 25000000

/* Display Mutex */
static os_mutex_t display_mutex;

/* Send a command to display over Serial */
static void ili9163c_cmd(uint8_t cmd)
{
	GPIO_WritePinOutput(DISPLAY_DCX_PIN, GPIO_IO_LOW);
	ssp_drv_write(ssp, &cmd, NULL, 1, 1);
}

/* Send 8-bit data to display over Serial */
static void ili9163c_data(uint8_t data)
{
	GPIO_WritePinOutput(DISPLAY_DCX_PIN, GPIO_IO_HIGH);
	ssp_drv_write(ssp, &data, NULL, 1, 1);
}

/* Reset the display panel */
static void ili9163c_reset()
{
	GPIO_PinMuxFun(DISPLAY_RESET_PIN, DISPLAY_RESET_PIN_MUX);
	GPIO_SetPinDir(DISPLAY_RESET_PIN, GPIO_OUTPUT);

	GPIO_WritePinOutput(DISPLAY_RESET_PIN, GPIO_IO_HIGH);
	_os_delay(1); /*Delay 1 ms */
	GPIO_WritePinOutput(DISPLAY_RESET_PIN, GPIO_IO_LOW);
	_os_delay(10); /*Delay 10 ms */
	GPIO_WritePinOutput(DISPLAY_RESET_PIN, GPIO_IO_HIGH);
	_os_delay(50); /*Delay 50 ms */

	/* Initialize Command/Data Select Pin and put in Command Mode */
	GPIO_PinMuxFun(DISPLAY_DCX_PIN, DISPLAY_DCX_PIN_MUX);
	GPIO_SetPinDir(DISPLAY_DCX_PIN, GPIO_OUTPUT);
	GPIO_WritePinOutput(DISPLAY_DCX_PIN, GPIO_IO_LOW);

	/* Switch On Backlight */
	GPIO_PinMuxFun(DISPLAY_BKL_PIN, DISPLAY_BKL_PIN_MUX);
	GPIO_SetPinDir(DISPLAY_BKL_PIN, GPIO_OUTPUT);
	GPIO_WritePinOutput(DISPLAY_BKL_PIN, GPIO_IO_HIGH);
}

/* Initialize the display panel */
static void ili9163c_init(void)
{
	/*  VCI = 2.8V */

	/* Start Initial Sequence */
	ili9163c_cmd(0x11);	/* Exit Sleep */
	_os_delay(20);

	ili9163c_cmd(0xEC);
	ili9163c_data(0x0C);

	ili9163c_cmd(0x26);	/* Set Default Gamma */
	ili9163c_data(0x04);

	ili9163c_cmd(0xB1);
	ili9163c_data(0x0A);
	ili9163c_data(0x14);

	ili9163c_cmd(0xC0);	/* Set VRH1[4:0] & VC[2:0] for VCI1 & GVDD */
	ili9163c_data(0x06);
	ili9163c_data(0x00);

	ili9163c_cmd(0xC1);	/* Set BT[2:0] for AVDD & VCL & VGH & VGL */
	ili9163c_data(0x02);

	ili9163c_cmd(0xC5);	/* Set VMH[6:0] & VML[6:0] for VOMH & VCOML */
	ili9163c_data(0x3C);
	ili9163c_data(0x40);

	ili9163c_cmd(0xC7);
	ili9163c_data(0xC3);

	ili9163c_cmd(0x3A);	/* Set Color Format */
	ili9163c_data(0x5);

	ili9163c_cmd(0x2A);	/* Set Column Address */
	ili9163c_data(0x00);
	ili9163c_data(0x00);
	ili9163c_data(0x00);
	ili9163c_data(0x7F);

	ili9163c_cmd(0x2B);	/* Set Page Address */
	ili9163c_data(0x00);
	ili9163c_data(0x00);
	ili9163c_data(0x00);
	ili9163c_data(0x9F);

	ili9163c_cmd(0x36);	/* Set Scanning Direction */
	ili9163c_data(0xC8);

	ili9163c_cmd(0xF2);	/* Enable Gamma bit */
	ili9163c_data(0x01);

	ili9163c_cmd(0xE0);
	ili9163c_data(0x3F);	/* p1 */
	ili9163c_data(0x18);	/* p2 */
	ili9163c_data(0x18);	/* p3 */
	ili9163c_data(0x26);	/* p4 */
	ili9163c_data(0x20);	/* p5 */
	ili9163c_data(0x0D);	/* p6 */
	ili9163c_data(0x46);	/* p7 */
	ili9163c_data(0xF3);	/* p8 */
	ili9163c_data(0x32);	/* p9 */
	ili9163c_data(0x09);	/* p10 */
	ili9163c_data(0x02);	/* p11 */
	ili9163c_data(0x02);	/* p12 */
	ili9163c_data(0x00);	/* p13 */
	ili9163c_data(0x00);	/* p14 */
	ili9163c_data(0x00);	/* p15 */

	ili9163c_cmd(0xE1);
	ili9163c_data(0x00);	/* p1 */
	ili9163c_data(0x27);	/* p2 */
	ili9163c_data(0x27);	/* p3 */
	ili9163c_data(0x09);	/* p4 */
	ili9163c_data(0x0F);	/* p5 */
	ili9163c_data(0x12);	/* p6 */
	ili9163c_data(0x39);	/* p7 */
	ili9163c_data(0xC0);	/* p8 */
	ili9163c_data(0x4D);	/* p9 */
	ili9163c_data(0x16);	/* p10 */
	ili9163c_data(0x1D);	/* p11 */
	ili9163c_data(0x2D);	/* p12 */
	ili9163c_data(0x3F);	/* p13 */
	ili9163c_data(0x3F);	/* p14 */
	ili9163c_data(0x3F);	/* p15 */

	ili9163c_cmd(0x36);	/*  RGB/BGR + Orientation*/
	ili9163c_data(0x40);

	ili9163c_cmd(0x29);	/*  Display On */
}

/* Initialize display panel */
int display_init(void)
{
	int ret;

	ret = os_mutex_create(&display_mutex, "DISPLAY_MUTEX",
			      OS_MUTEX_INHERIT);
	if (ret == -WM_FAIL) {
		DISPLAY_LOG("Failed to create display mutex\r\n");
		return ret;
	}

	ret = os_mutex_get(&display_mutex, OS_WAIT_FOREVER);
	if (ret == -WM_FAIL) {
		DISPLAY_LOG("Failed to get display mutex\r\n");
		return ret;
	}

	/* Initialize SSP Driver */
	ssp_drv_init(DISPLAY_SSP);

	/* Set SPI clock frequency */
	ssp_drv_set_clk(DISPLAY_SSP, SSP_FREQ);

	/* Configure SSP as SPI device. SSP is configured as master */
	ssp = ssp_drv_open(DISPLAY_SSP, SSP_FRAME_SPI, SSP_MASTER, DMA_DISABLE,
			-1, 0);

	/* Reset and initialize the display */
	ili9163c_reset();
	ili9163c_init();

	os_mutex_put(&display_mutex);

	return 0;
}

/* Update the display frame-buffer */
void display_update(void *display_buf)
{
	int ret;

	ret = os_mutex_get(&display_mutex, OS_WAIT_FOREVER);
	if (ret == -WM_FAIL) {
		DISPLAY_LOG("Failed to get display mutex\r\n");
		return;
	}

	ili9163c_cmd(0x2C);
	ssp_drv_close(ssp);

	/* Select Data Mode */
	GPIO_WritePinOutput(DISPLAY_DCX_PIN, GPIO_IO_HIGH);

	/* Transmit frame buffer in 16-bit data mode with DMA */
	ssp_drv_datasize(DISPLAY_SSP, SSP_DATASIZE_16);
	ssp_drv_set_clk(DISPLAY_SSP, SSP_FREQ);
	ssp = ssp_drv_open(DISPLAY_SSP, SSP_FRAME_SPI, SSP_MASTER, DMA_ENABLE,
			-1, 0);
	ssp_drv_write(ssp, (uint8_t *)display_buf, NULL, DISPLAY_BUFF_SIZE, 1);
	ssp_drv_close(ssp);

	/* Select Command Mode */
	ssp_drv_datasize(DISPLAY_SSP, SSP_DATASIZE_8);
	ssp_drv_set_clk(DISPLAY_SSP, SSP_FREQ);
	ssp = ssp_drv_open(DISPLAY_SSP, SSP_FRAME_SPI, SSP_MASTER, DMA_DISABLE,
			-1, 0);
	ili9163c_cmd(0x0);

	os_mutex_put(&display_mutex);
}

/* Enter or exit display sleep mode */
void display_sleep(sleep_t mode)
{
	int ret;

	ret = os_mutex_get(&display_mutex, OS_WAIT_FOREVER);
	if (ret == -WM_FAIL) {
		DISPLAY_LOG("Failed to get display mutex\r\n");
		return;
	}

	if (mode == SLEEP_ENTER)
		ili9163c_cmd(0x10);
	else
		ili9163c_cmd(0x11);
	_os_delay(150);

	os_mutex_put(&display_mutex);
}

/* Switch on or off display back-light */
void display_backlight(backlight_t state)
{
	int ret;

	ret = os_mutex_get(&display_mutex, OS_WAIT_FOREVER);
	if (ret == -WM_FAIL) {
		DISPLAY_LOG("Failed to get display mutex\r\n");
		return;
	}

	if (state == BACKLIGHT_ON)
		GPIO_WritePinOutput(DISPLAY_BKL_PIN, GPIO_IO_HIGH);
	else
		GPIO_WritePinOutput(DISPLAY_BKL_PIN, GPIO_IO_LOW);

	os_mutex_put(&display_mutex);
}
