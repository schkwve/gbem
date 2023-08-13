/**
 * GbEM
 * Copyright (C) 2023 Jozef Nagy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef __LCD_H_
#define __LCD_H_

#include <common.h>

#define LCDC_BGW_ENABLE (BIT(lcd_get_context()->lcdc, 0))
#define LCDC_OBJ_ENABLE (BIT(lcd_get_context()->lcdc, 1))
#define LCDC_OBJ_HEIGHT (BIT(lcd_get_context()->lcdc, 2) ? 16 : 8)
#define LCDC_BG_MAP_AREA (BIT(lcd_get_context()->lcdc, 3) ? 0x9C00 : 0x9800)
#define LCDC_BGW_DATA_AREA (BIT(lcd_get_context()->lcdc, 4) ? 0x8000 : 0x8800)
#define LCDC_WIN_ENABLE (BIT(lcd_get_context()->lcdc, 5))
#define LCDC_WIN_MAP_AREA (BIT(lcd_get_context()->lcdc, 6) ? 0x9C00 : 0x9800)
#define LCDC_LCD_ENABLE (BIT(lcd_get_context()->lcdc, 7))

#define LCDS_MODE ((lcd_mode)(lcd_get_context()->lcds & 0b11))
#define LCDS_MODE_SET(mode)               \
	{                                     \
		lcd_get_context()->lcds &= ~0b11; \
		lcd_get_context()->lcds |= mode;  \
	}

#define LCDS_LYC (BIT(lcd_get_context()->lcds, 2))
#define LCDS_LYC_SET(b) (BIT_SET(lcd_get_context()->lcds, 2, b))

#define LCDS_STAT_INT(src) (lcd_get_context()->lcds & src)

typedef struct {
	uint8_t lcdc;
	uint8_t lcds;
	uint8_t scroll_x;
	uint8_t scroll_y;
	uint8_t ly;
	uint8_t ly_cmp;
	uint8_t dma;
	uint8_t bg_palette;
	uint8_t obj_palette[2];
	uint8_t win_y;
	uint8_t win_x;

	uint32_t bg_colors[4];
	uint32_t sp1_colors[4];
	uint32_t sp2_colors[4];
} lcd_ctx;

typedef enum { MODE_HBLANK, MODE_VBLANK, MODE_OAM, MODE_XFER } lcd_mode;

typedef enum {
	SS_HBLANK = (1 << 3),
	SS_VBLANK = (1 << 4),
	SS_OAM = (1 << 5),
	SS_LYC = (1 << 6),
} stat_src;

lcd_ctx *lcd_get_context();

void lcd_init();
uint8_t lcd_read(uint16_t addr);
void lcd_write(uint16_t addr, uint8_t val);
void update_palette(uint8_t palette_data, uint8_t palette);

#endif // __LCD_H_
