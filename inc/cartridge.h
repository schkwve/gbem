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

#ifndef __CARTRIDGE_H_
#define __CARTRIDGE_H_

#include <common.h>

typedef struct {
	uint8_t entry[4];
	uint8_t logo[0x30];

	char title[16];
	uint16_t new_lic_code;
	uint8_t sgb_flag;
	uint8_t type;
	uint8_t rom_size;
	uint8_t ram_size;
	uint8_t dest_code;
	uint8_t lic_code;
	uint8_t version;
	uint8_t checksum;
	uint16_t global_checksum;
} rom_header;

typedef struct {
	char filename[1024];
	uint32_t rom_size;
	uint8_t *rom_data;
	rom_header *header;
} cart_context;

bool cart_load(char *cart);

#endif // __CARTRIDGE_H_
