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
#include <common.h>

extern cpu_ctx ctx;

uint16_t reverse(uint16_t n)
{
	return ((n & 0xFF00) >> 8) | ((n & 0xFF) << 8);
}

bool check_cond(cpu_ctx *ctx)
{
	bool z = CPU_FLAG_Z;
	bool c = CPU_FLAG_C;

	switch (ctx->instr->cond) {
	case CT_NONE:
		return true;
	case CT_C:
		return c;
	case CT_NC:
		return !c;
	case CT_Z:
		return z;
	case CT_NZ:
		return !z;
	default:
		return false;
	}
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

void cpu_set_reg(reg_type rt, uint16_t val)
{
	switch (rt) {
	case RT_A:
		ctx.regs.a = val & 0xFF;
		break;
	case RT_F:
		ctx.regs.f = val & 0xFF;
		break;
	case RT_B:
		ctx.regs.b = val & 0xFF;
		break;
	case RT_C: {
		ctx.regs.c = val & 0xFF;
	} break;
	case RT_D:
		ctx.regs.d = val & 0xFF;
		break;
	case RT_E:
		ctx.regs.e = val & 0xFF;
		break;
	case RT_H:
		ctx.regs.h = val & 0xFF;
		break;
	case RT_L:
		ctx.regs.l = val & 0xFF;
		break;

	case RT_AF:
		*((uint16_t *)&ctx.regs.a) = reverse(val);
		break;
	case RT_BC:
		*((uint16_t *)&ctx.regs.b) = reverse(val);
		break;
	case RT_DE:
		*((uint16_t *)&ctx.regs.d) = reverse(val);
		break;
	case RT_HL: {
		*((uint16_t *)&ctx.regs.h) = reverse(val);
		break;
	}

	case RT_PC:
		ctx.regs.pc = val;
		break;
	case RT_SP:
		ctx.regs.sp = val;
		break;
	case RT_NONE:
		break;
	}
}

void cpu_set_flags(cpu_ctx *ctx, char z, char n, char h, char c)
{
	if (z != -1) {
		BIT_SET(ctx->regs.f, 7, z);
	}
	if (n != -1) {
		BIT_SET(ctx->regs.f, 6, n);
	}
	if (h != -1) {
		BIT_SET(ctx->regs.f, 5, h);
	}
	if (c != -1) {
		BIT_SET(ctx->regs.f, 4, c);
	}
}
