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

typedef struct {
	uint8_t y;
	uint8_t x;
	uint8_t tile;

	unsigned f_gbc_pn : 3;
	unsigned f_gbc_vram_bank : 1;
	unsigned f_pn : 1;
	unsigned f_x_flip : 1;
	unsigned f_y_flip : 1;
	unsigned f_bgp : 1;
} oam_entry;

typedef struct {
	oam_entry oam_ram[40];
	uint8_t vram[0x2000];
} ppu_ctx;

void ppu_init();
void ppu_tick();

uint8_t ppu_oam_read(uint16_t addr);
void ppu_oam_write(uint16_t addr, uint8_t val);

uint8_t ppu_vram_read(uint16_t addr);
void ppu_vram_write(uint16_t addr, uint8_t val);

#endif // __PPU_H_
