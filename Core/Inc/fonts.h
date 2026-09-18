/**
 ******************************************************************************
 * @file           : fonts.h
 * @brief          : Bitmap font definitions and interface
 * @author         : Cem Ozan Yilmaz
 * @date           : 18.09.2026 (dd/mm/yyyy)
 ******************************************************************************
 * @details
 *
 * This header file defines the bitmap font interface used by the custom
 * SSD1306 OLED display driver.
 *
 * It defines:
 * - Font descriptor structure
 * - Font width and height information
 * - Bitmap row storage information
 * - Bitmap font data references
 * - Available font definitions
 * - Character glyph lookup interface
 *
 * Supported font sizes:
 * - 6x8
 * - 6x10
 * - 7x10
 * - 8x13
 * - 11x18
 * - 16x26
 *
 ******************************************************************************
 */

#ifndef FONTS_H
#define FONTS_H

#include <stdint.h>

/* Describes one fixed-size bitmap font table. */
typedef struct
{
    uint8_t width;
    uint8_t height;
    uint8_t bytes_per_row;
    const uint8_t *data;
} FontDef;


extern const FontDef Font_6x8;
extern const FontDef Font_6x10;
extern const FontDef Font_7x10;
extern const FontDef Font_8x13;
extern const FontDef Font_11x18;
extern const FontDef Font_16x26;


/* Return the bitmap data for one printable ASCII character. */
const uint8_t *Font_GetGlyph(const FontDef *font, char character);

#endif
