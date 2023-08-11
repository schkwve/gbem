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

void bus_write(uint16_t addr, uint8_t val)
{
	(void)addr;
	(void)val;
	NOT_IMPLEMENTED();
}
