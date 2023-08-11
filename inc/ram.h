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

#ifndef __RAM_H_
#define __RAM_H_

#include <stddef.h>
#include <stdlib.h>

typedef struct {
	uint8_t wram[0x2000];
	uint8_t hram[0x80];
} ram_ctx;

uint8_t wram_read(uint16_t addr);
void wram_write(uint16_t addr, uint8_t val);
uint8_t hram_read(uint16_t addr);
void hram_write(uint16_t addr, uint8_t val);

#endif // __RAM_H_
