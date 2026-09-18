/**
 ******************************************************************************
 * @file           : ssd1306.h
 * @brief          : SSD1306 OLED display driver interface
 * @author         : Cem Ozan Yilmaz
 * @date           : 18.09.2026 (dd/mm/yyyy)
 ******************************************************************************
 * @details
 *
 * This header file provides the public interface and configuration definitions
 * for the custom STM32 HAL-based SSD1306 OLED display driver.
 *
 * It defines:
 * - SSD1306 display dimensions and framebuffer size
 * - SSD1306 I2C address and control bytes
 * - SSD1306 command definitions
 * - Display initialization interface
 * - Framebuffer update and clear interfaces
 * - Pixel drawing interface
 * - Character and string rendering interfaces
 *
 ******************************************************************************
 */

#ifndef SSD1306_H
#define SSD1306_H

#include "stm32l4xx_hal.h"
#include "fonts.h"

#define SSD1306_I2C_ADDR                        0x3C
#define SSD1306_CONTROL_CMD                     0x00
#define SSD1306_CONTROL_DATA                    0x40
#define SSD1306_SET_DISPLAY_OFF                 0xAE
#define SSD1306_SET_DISPLAY_ON                  0xAF
#define SSD1306_SET_CONTRAST                    0x81
#define SSD1306_SET_MEMORY_ADDRESSING_MODE      0x20
#define SSD1306_SET_MULTIPLEX_RATIO             0xA8
#define SSD1306_SEGMENT_REMAP                   0xA1
#define SSD1306_COM_SCAN_REMAP                  0xC8
#define SSD1306_SET_COM_PINS                    0xDA
#define SSD1306_SET_DISPLAY_OFFSET              0xD3
#define SSD1306_SET_START_LINE                  0x40
#define SSD1306_SET_DISPLAY_CLOCK               0xD5
#define SSD1306_SET_PRECHARGE                   0xD9
#define SSD1306_SET_VCOMH                       0xDB
#define SSD1306_CHARGE_PUMP                     0x8D
#define SSD1306_DISPLAY_FROM_RAM                0xA4
#define SSD1306_NORMAL_DISPLAY                  0xA6
#define SSD1306_SET_COLUMN_ADDRESS              0x21
#define SSD1306_SET_PAGE_ADDRESS                0x22

#define SSD1306_WIDTH   128
#define SSD1306_HEIGHT  64
#define SSD1306_BUFFER_SIZE (SSD1306_WIDTH * SSD1306_HEIGHT / 8) 

/* Initialize the OLED display with the selected I2C handle. */
void SSD1306_Init(I2C_HandleTypeDef *hi2c);

/* Draw or clear one pixel in the framebuffer. */
void SSD1306_DrawPixel(uint8_t x, uint8_t y, uint8_t state);

/* Send the framebuffer contents to the display. */
void SSD1306_UpdateScreen(void);

/* Clear the local framebuffer. */
void SSD1306_Clear(void);

/* Draw one character into the framebuffer. */
void SSD1306_WriteChar(uint8_t x, uint8_t y, char character, const FontDef *font);

/* Draw a null-terminated string into the framebuffer. */
void SSD1306_WriteString(uint8_t x, uint8_t y, const char *string, const FontDef *font);

#endif
