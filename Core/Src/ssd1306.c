/**
 ******************************************************************************
 * @file           : ssd1306.c
 * @brief          : SSD1306 OLED display driver implementation
 * @author         : Cem Ozan Yilmaz
 * @date           : 18.09.2026 (dd/mm/yyyy)
 ******************************************************************************
 * @details
 *
 * This file implements a custom STM32 HAL-based driver for SSD1306 OLED
 * displays using the I2C communication interface.
 *
 * The driver:
 * - Sends SSD1306 commands and display data over I2C
 * - Initializes and configures the SSD1306 display controller
 * - Maintains a framebuffer for the 128x64 monochrome display
 * - Provides pixel-level drawing operations
 * - Renders bitmap font characters into the framebuffer
 * - Renders text strings at user-defined screen coordinates
 * - Transfers the framebuffer contents to the SSD1306 GDDRAM
 *
 ******************************************************************************
 */

#include "ssd1306.h"

static I2C_HandleTypeDef *ssd1306_i2c;
static uint8_t ssd1306_buffer[SSD1306_BUFFER_SIZE];


/* Send a single command byte to the SSD1306 controller. */
static HAL_StatusTypeDef SSD1306_WriteCommand(uint8_t command)
{
    uint8_t data[2];

    data[0] = SSD1306_CONTROL_CMD;
    data[1] = command;

    return HAL_I2C_Master_Transmit(ssd1306_i2c, SSD1306_I2C_ADDR << 1, data, 2, HAL_MAX_DELAY);
}


/* Send a single data byte to the SSD1306 controller. */
static HAL_StatusTypeDef SSD1306_WriteData(uint8_t data)
{
    uint8_t buffer[2];

    buffer[0] = SSD1306_CONTROL_DATA;
    buffer[1] = data;

    return HAL_I2C_Master_Transmit(ssd1306_i2c, SSD1306_I2C_ADDR << 1, buffer, 2, HAL_MAX_DELAY);
}


/* Initialize the SSD1306 controller for a 128x64 I2C OLED display. */
void SSD1306_Init(I2C_HandleTypeDef *hi2c)
{
    ssd1306_i2c = hi2c;

    SSD1306_WriteCommand(SSD1306_SET_DISPLAY_OFF);

    SSD1306_WriteCommand(SSD1306_SET_DISPLAY_CLOCK);
    SSD1306_WriteCommand(0x80);

    SSD1306_WriteCommand(SSD1306_SET_MULTIPLEX_RATIO);
    SSD1306_WriteCommand(0x3F);

    SSD1306_WriteCommand(SSD1306_SET_DISPLAY_OFFSET);
    SSD1306_WriteCommand(0x00);

    SSD1306_WriteCommand(SSD1306_SET_START_LINE);

    SSD1306_WriteCommand(SSD1306_CHARGE_PUMP);
    SSD1306_WriteCommand(0x14);

    SSD1306_WriteCommand(SSD1306_SET_MEMORY_ADDRESSING_MODE);
    SSD1306_WriteCommand(0x00);

    SSD1306_WriteCommand(SSD1306_SEGMENT_REMAP);

    SSD1306_WriteCommand(SSD1306_COM_SCAN_REMAP);

    SSD1306_WriteCommand(SSD1306_SET_COM_PINS);
    SSD1306_WriteCommand(0x12);

    SSD1306_WriteCommand(SSD1306_SET_CONTRAST);
    SSD1306_WriteCommand(0x7F);

    SSD1306_WriteCommand(SSD1306_SET_PRECHARGE);
    SSD1306_WriteCommand(0x22);

    SSD1306_WriteCommand(SSD1306_SET_VCOMH);
    SSD1306_WriteCommand(0x20);

    SSD1306_WriteCommand(SSD1306_DISPLAY_FROM_RAM);

    SSD1306_WriteCommand(SSD1306_NORMAL_DISPLAY);

    SSD1306_WriteCommand(SSD1306_SET_DISPLAY_ON);
}


/* Transfer the local framebuffer to the display memory. */
void SSD1306_UpdateScreen(void)
{
    SSD1306_WriteCommand(SSD1306_SET_COLUMN_ADDRESS);
    SSD1306_WriteCommand(0x00);
    SSD1306_WriteCommand(0x7F);

    SSD1306_WriteCommand(SSD1306_SET_PAGE_ADDRESS);
    SSD1306_WriteCommand(0x00);
    SSD1306_WriteCommand(0x07);

    for (uint16_t i = 0; i < SSD1306_BUFFER_SIZE; i++)
    {
        SSD1306_WriteData(ssd1306_buffer[i]);
    }
}


/* Set or clear one pixel in the local framebuffer. */
void SSD1306_DrawPixel(uint8_t x, uint8_t y, uint8_t state)
{
    if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT)
    {
        return;
    }

    uint16_t index = x + ((y / 8) * SSD1306_WIDTH);

    if (state)
    {
        ssd1306_buffer[index] |= (1 << (y % 8));
    }
    else
    {
        ssd1306_buffer[index] &= ~(1 << (y % 8));
    }
}


/* Clear all pixels in the local framebuffer. */
void SSD1306_Clear(void)
{
    for (uint16_t i = 0; i < SSD1306_BUFFER_SIZE; i++)
    {
        ssd1306_buffer[i] = 0x00;
    }
}

/* Draw one bitmap font character at the given position. */
void SSD1306_WriteChar(uint8_t x, uint8_t y, char character, const FontDef *font)
{
    if ((x + font->width) > SSD1306_WIDTH ||
        (y + font->height) > SSD1306_HEIGHT)
    {
        return;
    }

    const uint8_t *glyph = Font_GetGlyph(font, character);

    for (uint8_t row = 0; row < font->height; row++)
    {
        for (uint8_t col = 0; col < font->width; col++)
        {
            uint8_t byte = glyph[row * font->bytes_per_row + (col / 8)];

            uint8_t state = (byte >> (7 - (col % 8))) & 0x01;

            SSD1306_DrawPixel(x + col, y + row, state);
        }
    }
}

/* Draw a text string from left to right until the display edge is reached. */
void SSD1306_WriteString(uint8_t x, uint8_t y, const char *string, const FontDef *font)
{
    while (*string)
    {
        if ((x + font->width) > SSD1306_WIDTH)
        {
            break;
        }

        SSD1306_WriteChar(x, y, *string, font);

        x += font->width;
        string++;
    }
}
