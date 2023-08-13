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

#include <cpu.h>
#include <io.h>
#include <timer.h>
#include <common.h>
#include <dma.h>
#include <lcd.h>

static char serial_data[2];

uint8_t io_read(uint16_t addr)
{
	if (addr == 0xFF01) {
		return serial_data[0];
	} else if (addr == 0xFF02) {
		return serial_data[1];
	} else if (BETWEEN(addr, 0xFF04, 0xFF07)) {
		return timer_read(addr);
	} else if (addr == 0xFF0F) {
		return cpu_get_int_flags();
	} else if (BETWEEN(addr, 0xFF40, 0xFF4B)) {
		return lcd_read(addr);
	}

	printf("Unsupported io_read: %04x\n", addr);
	return 0;
}

void io_write(uint16_t addr, uint8_t val)
{
	if (addr == 0xFF01) {
		serial_data[0] = val;
		return;
	} else if (addr == 0xFF02) {
		serial_data[1] = val;
		return;
	} else if (BETWEEN(addr, 0xFF04, 0xFF07)) {
		timer_write(addr, val);
		return;
	} else if (addr == 0xFF0F) {
		cpu_set_int_flags(val);
		return;
	} else if (BETWEEN(addr, 0xFF40, 0xFF4B)) {
		lcd_write(addr, val);
		return;
	}

	printf("Unsupported io_write: %04x\n", addr);
}
