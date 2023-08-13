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

#include <string.h>
#include <ppu.h>
#include <ppu_smach.h>
#include <lcd.h>

static ppu_ctx ctx = { 0 };

ppu_ctx *ppu_get_context()
{
	return &ctx;
}

void ppu_init()
{
	ctx.cur_frame = 0;
	ctx.line_ticks = 0;
	ctx.framebuffer = malloc(YRES * XRES * sizeof(32));

	ctx.pfc.line_x = 0;
	ctx.pfc.pushed_x = 0;
	ctx.pfc.fetch_x = 0;
	ctx.pfc.pixel_fifo.size = 0;
	ctx.pfc.pixel_fifo.head = ctx.pfc.pixel_fifo.tail = NULL;
	ctx.pfc.cur_fetch_state = FS_TILE;

	lcd_init();
	LCDS_MODE_SET(MODE_OAM);

	memset(ctx.oam_ram, 0, sizeof(ctx.oam_ram));
	memset(ctx.framebuffer, 0, YRES * XRES * sizeof(uint32_t));
}

void ppu_tick()
{
	ctx.line_ticks++;

	switch (LCDS_MODE) {
	case MODE_OAM:
		ppu_mode_oam();
		break;
	case MODE_XFER:
		ppu_mode_xfer();
		break;
	case MODE_VBLANK:
		ppu_mode_vblank();
		break;
	case MODE_HBLANK:
		ppu_mode_hblank();
		break;
	}
}

void ppu_oam_write(uint16_t address, uint8_t value)
{
	if (address >= 0xFE00) {
		address -= 0xFE00;
	}

	uint8_t *p = (uint8_t *)ctx.oam_ram;
	p[address] = value;
}

uint8_t ppu_oam_read(uint16_t address)
{
	if (address >= 0xFE00) {
		address -= 0xFE00;
	}

	uint8_t *p = (uint8_t *)ctx.oam_ram;
	return p[address];
}

void ppu_vram_write(uint16_t address, uint8_t value)
{
	ctx.vram[address - 0x8000] = value;
}

uint8_t ppu_vram_read(uint16_t address)
{
	return ctx.vram[address - 0x8000];
}