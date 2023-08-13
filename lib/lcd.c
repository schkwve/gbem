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

#include <lcd.h>
#include <dma.h>

static lcd_ctx ctx = { 0 };

static unsigned long colors_default[4] = { 0xFFFFFFFF, 0xFFAAAAAA, 0xFF555555,
										   0xFF000000 };

lcd_ctx *lcd_get_context()
{
	return &ctx;
}

void lcd_init()
{
	ctx.lcdc = 0x91;
	ctx.scroll_x = 0;
	ctx.scroll_y = 0;
	ctx.ly = 0;
	ctx.ly_cmp = 0;
	ctx.bg_palette = 0xFC;
	ctx.obj_palette[0] = 0xFF;
	ctx.obj_palette[1] = 0xFF;
	ctx.win_y = 0;
	ctx.win_x = 0;

	for (int i = 0; i < 4; i++) {
		ctx.bg_colors[i] = colors_default[i];
		ctx.sp1_colors[i] = colors_default[i];
		ctx.sp2_colors[i] = colors_default[i];
	}
}

uint8_t lcd_read(uint16_t addr)
{
	uint8_t offset = (addr - 0xFF40);
	uint8_t *p = (uint8_t *)&ctx;
	return p[offset];
}

void lcd_write(uint16_t addr, uint8_t val)
{
	uint8_t offset = (addr - 0xFF40);
	uint8_t *p = (uint8_t *)&ctx;
	p[offset] = val;

	if (offset == 6) {
		// DMA
		dma_start(val);
	}

	if (addr == 0xFF47) {
		update_palette(val, 0);
	} else if (addr == 0xFF48) {
		update_palette(val & 0b11111100, 1);
	} else if (addr == 0xFF49) {
		update_palette(val & 0b11111100, 1);
	}
}

void update_palette(uint8_t palette_data, uint8_t palette)
{
	uint32_t *p_col = ctx.bg_colors;

	switch (palette) {
	case 1:
		p_col = ctx.sp1_colors;
		break;
	case 2:
		p_col = ctx.sp2_colors;
		break;
	}

	p_col[0] = colors_default[palette_data & 0b11];
	p_col[1] = colors_default[(palette_data >> 2) & 0b11];
	p_col[2] = colors_default[(palette_data >> 4) & 0b11];
	p_col[3] = colors_default[(palette_data >> 6) & 0b11];
}
