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

#ifndef __INT_H_
#define __INT_H_

#include <cpu.h>

typedef enum {
	IT_VBLANK = 1,
	IT_LCD_START = 2,
	IT_TIMER = 4,
	IT_SERIAL = 8,
	IT_JOYPAD = 16
} interrupt_type;

void cpu_req_int(interrupt_type t);
void cpu_handle_int(cpu_ctx *ctx);

#endif // __INT_H_
