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
#include <ram.h>
#include <cartridge.h>
#include <cpu.h>

uint8_t bus_read(uint16_t addr)
{
	// ROM Data
	if (addr < 0x8000) {
		return cart_read(addr);
	} else if (addr < 0xA000) {
		// CHR/BG Maps
		NOT_IMPLEMENTED();
	} else if (addr < 0xC000) {
		// Cartridge RAM
		return cart_read(addr);
	} else if (addr < 0xE000) {
		// WRAM
		return wram_read(addr);
	} else if (addr < 0xFE00) {
		// Reserved
		return 0;
	} else if (addr < 0xFEA0) {
		// OAM
		NOT_IMPLEMENTED();
	} else if (addr < 0xFF00) {
		// Reserved
		return 0;
	} else if (addr < 0xFF80) {
		// I/O
		NOT_IMPLEMENTED();
	} else if (addr == 0xFFFF) {
		// CPU Interrupt Enable
		return cpu_get_ie();
	} else {
		// High RAM / Zero Page
		return hram_read(addr);
	}
}

void bus_write(uint16_t addr, uint8_t val)
{
	if (addr < 0x8000) {
		cart_write(addr, val);
		return;
	} else if (addr < 0xA000) {
		// CHR/BG Maps
		NOT_IMPLEMENTED();
	} else if (addr < 0xC000) {
		// Cartridge RAM
		cart_write(addr, val);
	} else if (addr < 0xE000) {
		// WRAM
		wram_write(addr, val);
	} else if (addr < 0xFE00) {
		// Reserved
	} else if (addr < 0xFEA0) {
		// OAM
		NOT_IMPLEMENTED();
	} else if (addr < 0xFF00) {
		// Reserved
	} else if (addr < 0xFF80) {
		// I/O
		NOT_IMPLEMENTED();
	} else if (addr == 0xFFFF) {
		// CPU Interrupt Enable
		cpu_set_ie(val);
	} else {
		// High RAM / Zero Page
		hram_write(addr, val);
	}

	NOT_IMPLEMENTED();
}

uint16_t bus_read16(uint16_t addr)
{
	uint8_t lo = bus_read(addr);
	uint8_t hi = bus_read(addr + 1);
	return lo | (hi << 8);
}

void bus_write16(uint16_t addr, uint16_t val)
{
	bus_write(addr + 1, (val >> 8) & 0xFF);
	bus_write(addr, val & 0xFF);
}
