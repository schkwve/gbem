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

#ifndef __TIMER_H_
#define __TIMER_H_

#include <common.h>

typedef struct {
	uint16_t div;
	uint8_t tima;
	uint8_t tma;
	uint8_t tac;
} timer_ctx;

timer_ctx *timer_get_context();

void timer_init();
void timer_tick();

void timer_write(uint16_t addr, uint8_t val);
uint8_t timer_read(uint16_t addr);

#endif // __TIMER_H_
