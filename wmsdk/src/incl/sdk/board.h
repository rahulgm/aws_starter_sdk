/*! \file board.h
 * \brief Board Specific APIs
 *
 *  The board specific APIs are provided to help
 *  with porting the SDK onto new boards. These
 *  APIs are board specific and they need to be
 *  modified/implemented according to the actual
 *  board specifications.
 *
 *  Please verify that {board_file}.c file found under wmsdk/src/boards/
 *  reflects the correct hardware configuration for the project.
 */
/*
 *  Copyright 2008-2015, Marvell International Ltd.
 *  All Rights Reserved.
 */

#include <wmtypes.h>
#include <lowlevel_drivers.h>
#include <generic_io.h>

/** Frequency at which CPU should run
 *
 * Mention the frequency at which the CPU should
 * run here. If this frequency is greater than the
 * source clock, internal PLL will be configured.<br>
 *
 * NOTE: Valid frequencies depend upon the source clock.
 * Please refer to the datasheet for further details.
 *
 *  \return Frequency at which CPU should operate.
 */
int board_cpu_freq();

/** 32 KHz crystal
 *
 * If 32 KHz crystal is present on the board and
 * is functional, return true here.
 *
 *  \return true if 32 KHz crystal is present and
 *  functional, false otherwise.
 */
int board_32k_xtal();

/** 32 KHz Oscillator
 *
 * If 32 KHz oscillator is present on the board and
 * is functional, return true here.
 *
 *  \return true if 32 KHz oscillator is present and
 *  functional, false otherwise.
 */
int board_32k_osc();

/** 32 KHz Internal RC Calibration
 *
 * If 32 KHz crystal or oscillator is not present. The
 * internal rc32k can be calibrated using GPT.
 * NOTE1: On MW300 this needs the GPIO24 and GPIO25 to
 * be connected.
 * NOTE2: On MC200 this will work only if 32MHz crystal
 * is present on the board.
 *
 *  \return true if RC32K should be calibrated using GPT
 */
int board_rc32k_calib();

/** Push Button Pressed
 *
 * Depending upon how the push button is
 * connected, corresponding GPIO needs to
 * be checked for either set or reset.
 *
 * If no pushbuttons are implemented on the board,
 * always return FALSE (i.e. button not pressed).
 *
 * \param[in] pin GPIO pin to be used
 * \return TRUE if pressed, FALSE otherwise
 */
int board_button_pressed(int pin);

/** Power On GPIO settings
 *
 * Do board specific power GPIO settings here.
 */
void board_gpio_power_on();

/** UART pin config
 *
 * Select which pins will be used for different UART ports
 *
 * Leave Blank if UART is not implemented on the board.
 *
 * \param[in] id UART port number
 */
void board_uart_pin_config(int id);

/** I2C pin config
 *
 * Select which pins will be used for different I2C ports
 *
 * Leave Blank if I2C is not implemented on the board.
 *
 * \param[in] id I2C port number
 */
void board_i2c_pin_config(int id);

/** SSP pin config
 *
 * Select which pins will be used for different SSP ports
 *
 * Leave Blank if SSP is not implemented on the board.
 *
 * \param[in] id SSP port number
 * \param[in] cs If internal SSPx_FRM (cs=1) should be activated or not.
 */

void board_ssp_pin_config(int id, bool cs);

/** USB pin configuration
 *
 * Select pin configuration for USB.
 *
 */
void board_usb_pin_config();

/** Configuration of GPIO connected to LED 1
 *
 * \note If LED is not present on the board, the gpio member in \ref
 * output_gpio_cfg_t is set to -1 by this API.
 *
 * \return Output GPIO Configuration type \ref output_gpio_cfg_t
 */
output_gpio_cfg_t board_led_1();

/** Configuration of GPIO connected to LED 2
 *
 * \note If LED is not present on the board, the gpio member in \ref
 * output_gpio_cfg_t is set to -1 by this API.
 *
 * \return Output GPIO Configuration type \ref output_gpio_cfg_t
 */
output_gpio_cfg_t board_led_2();

/** Configuration of GPIO connected to LED 3
 *
 * \note If LED is not present on the board, the gpio member in \ref
 * output_gpio_cfg_t is set to -1 by this API.
 *
 * \return Output GPIO Configuration type \ref output_gpio_cfg_t
 */
output_gpio_cfg_t board_led_3();

/** Configuration of GPIO connected to LED 4
 *
 * \note If LED is not present on the board, the gpio member in \ref
 * output_gpio_cfg_t is set to -1 by this API.
 *
 * \return Output GPIO Configuration type \ref output_gpio_cfg_t
 */
output_gpio_cfg_t board_led_4();

/** Pushbutton 1
 *
 * \return GPIO connected to the pushbutton 1 or -WM_FAIL if such pushbutton is
 * not implemented on the board.
 */
int board_button_1();

/** Pushbutton 2
 *
 * \return GPIO connected to the pushbutton 2 or -WM_FAIL if such pushbutton is
 * not implemented on the board.
 */
int board_button_2();

/** Pushbutton 3
 *
 * \return GPIO connected to the pushbutton 3 or -WM_FAIL if such pushbutton is
 * not implemented on the board.
 */
int board_button_3();
