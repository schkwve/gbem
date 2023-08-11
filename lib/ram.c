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

#include <ram.h>
#include <stdio.h>
#include <stdlib.h>

static ram_ctx ctx;

uint8_t wram_read(uint16_t addr)
{
	addr -= 0xC000;
	return ctx.wram[addr];
}

void wram_write(uint16_t addr, uint8_t val)
{
	addr -= 0xC000;
	ctx.wram[addr] = val;
}

uint8_t hram_read(uint16_t addr)
{
	addr -= 0xFF80;
	return ctx.hram[addr];
}

void hram_write(uint16_t addr, uint8_t val)
{
	addr -= 0xFF80;
	ctx.hram[addr] = val;
}
