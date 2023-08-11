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

#include <common.h>
#include <bus.h>
#include <cartridge.h>

uint8_t bus_read(uint16_t addr)
{
	// ROM Data
	if (addr < 0x8000) {
		return cart_read(addr);
	}

	NOT_IMPLEMENTED();
	return 0;
}

uint16_t bus_read16(uint16_t addr)
{
	uint8_t lo = bus_read(addr);
	uint8_t hi = bus_read(addr + 1);
	return lo | (hi << 8);
}

void bus_write(uint16_t addr, uint8_t val)
{
	if (addr < 0x8000) {
		cart_write(addr, val);
		return;
	}

	NOT_IMPLEMENTED();
}

void bus_write16(uint16_t addr, uint16_t val)
{
	bus_write(addr + 1, (val >> 8) & 0xFF);
	bus_write(addr, val & 0xFF);
}
