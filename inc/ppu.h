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

#ifndef __PPU_H_
#define __PPU_H_

#include <common.h>

static const int LINES_PER_FRAME = 154;
static const int TICKS_PER_LINE = 456;
static const int YRES = 144;
static const int XRES = 160;

typedef enum { FS_TILE, FS_DATA0, FS_DATA1, FS_IDLE, FS_PUSH } fetch_state;

typedef struct _fifo_entry {
	uint32_t value;
	struct _fifo_entry *next;
} fifo_entry;

typedef struct {
	fifo_entry *head;
	fifo_entry *tail;
	uint32_t size;
} fifo;

typedef struct {
	fetch_state cur_fetch_state;
	fifo pixel_fifo;
	uint8_t line_x;
	uint8_t pushed_x;
	uint8_t fetch_x;
	uint8_t bgw_fetch_data[3];
	uint8_t fetch_entry_data[6];
	uint8_t map_x;
	uint8_t map_y;
	uint8_t tile_y;
	uint8_t fifo_x;
} pixel_fifo_ctx;

typedef struct {
	uint8_t y;
	uint8_t x;
	uint8_t tile;

	uint8_t f_gbc_pn : 3;
	uint8_t f_gbc_vram_bank : 1;
	uint8_t f_pn : 1;
	uint8_t f_x_flip : 1;
	uint8_t f_y_flip : 1;
	uint8_t f_bgp : 1;
} oam_entry;

typedef struct {
	oam_entry oam_ram[40];
	uint8_t vram[0x2000];

	pixel_fifo_ctx pfc;

	uint32_t cur_frame;
	uint32_t line_ticks;
	uint32_t *framebuffer;
} ppu_ctx;

ppu_ctx *ppu_get_context();

void ppu_init();
void ppu_tick();

uint8_t ppu_oam_read(uint16_t addr);
void ppu_oam_write(uint16_t addr, uint8_t val);

uint8_t ppu_vram_read(uint16_t addr);
void ppu_vram_write(uint16_t addr, uint8_t val);

#endif // __PPU_H_
