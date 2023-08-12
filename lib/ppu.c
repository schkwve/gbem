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

#include <ppu.h>

static ppu_ctx ctx = { 0 };

void ppu_init()
{
}

void ppu_tick()
{
}

uint8_t ppu_oam_read(uint16_t addr)
{
	if (addr >= 0xFE00) {
		addr -= 0xFE00;
	}

	uint8_t *p = (uint8_t *)ctx.oam_ram;
	return p[addr];
}

void ppu_oam_write(uint16_t addr, uint8_t val)
{
	if (addr >= 0xFE00) {
		addr -= 0xFE00;
	}

	uint8_t *p = (uint8_t *)ctx.oam_ram;
	p[addr] = val;
}

uint8_t ppu_vram_read(uint16_t addr)
{
	return ctx.vram[addr - 0x8000];
}

void ppu_vram_write(uint16_t addr, uint8_t val)
{
	ctx.vram[addr - 0x8000] = val;
}
