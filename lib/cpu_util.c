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

extern cpu_ctx ctx;

uint16_t reverse(uint16_t n)
{
	return ((n & 0xFF00) >> 8) | ((n & 0xFF) << 8);
}

uint16_t cpu_read_reg(reg_type rt)
{
	switch (rt) {
	case RT_A:
		return ctx.regs.a;
	case RT_F:
		return ctx.regs.f;
	case RT_B:
		return ctx.regs.b;
	case RT_C:
		return ctx.regs.c;
	case RT_D:
		return ctx.regs.d;
	case RT_E:
		return ctx.regs.e;
	case RT_H:
		return ctx.regs.h;
	case RT_L:
		return ctx.regs.l;

	case RT_AF:
		return reverse(*((uint16_t *)&ctx.regs.a));
	case RT_BC:
		return reverse(*((uint16_t *)&ctx.regs.b));
	case RT_DE:
		return reverse(*((uint16_t *)&ctx.regs.d));
	case RT_HL:
		return reverse(*((uint16_t *)&ctx.regs.h));

	case RT_PC:
		return ctx.regs.pc;
	case RT_SP:
		return ctx.regs.sp;
	default:
		return 0;
	}
}
