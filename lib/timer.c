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
#include <int.h>
#include <timer.h>
#include <common.h>

static timer_ctx ctx = { 0 };

timer_ctx *timer_get_context()
{
	return &ctx;
}

void timer_init()
{
	ctx.div = 0xAC00;
}

void timer_tick()
{
	uint16_t prev_div = ctx.div;
	ctx.div++;

	bool timer_update = false;
	switch (ctx.tac & (0b11)) {
	case 0b00:
		timer_update = (prev_div & (1 << 9)) && (!(ctx.div & (1 << 9)));
		break;
	case 0b01:
		timer_update = (prev_div & (1 << 3)) && (!(ctx.div & (1 << 3)));
		break;
	case 0b10:
		timer_update = (prev_div & (1 << 5)) && (!(ctx.div & (1 << 5)));
		break;
	case 0b11:
		timer_update = (prev_div & (1 << 7)) && (!(ctx.div & (1 << 7)));
		break;
	}

	if (timer_update && ctx.tac & (1 << 2)) {
		ctx.tima++;

		if (ctx.tima == 0xFF) {
			ctx.tima = ctx.tma;

			cpu_req_int(IT_TIMER);
		}
	}
}

void timer_write(uint16_t addr, uint8_t val)
{
	switch (addr) {
	case 0xFF04:
		// DIV
		ctx.div = 0;
		break;
	case 0xFF05:
		// TIMA
		ctx.tima = val;
		break;
	case 0xFF06:
		// TMA
		ctx.tma = val;
		break;
	case 0xFF07:
		// TAC
		ctx.tac = val;
		break;
	}
}

uint8_t timer_read(uint16_t addr)
{
	switch (addr) {
	case 0xFF04:
		// DIV
		return ctx.div >> 8;
	case 0xFF05:
		// TIMA
		return ctx.tima;
	case 0xFF06:
		// TMA
		return ctx.tma;
	case 0xFF07:
		// TAC
		return ctx.tac;
	}
	return 0;
}
