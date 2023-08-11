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
#include <emu.h>

static IN_PROC instr_func[] = {
	[IN_NONE] = proc_none, [IN_NOP] = proc_nop, [IN_LD] = proc_ld,
	[IN_JP] = proc_jp,	   [IN_DI] = proc_di,	[IN_XOR] = proc_xor
};

IN_PROC inst_get_proc(in_type type)
{
	return instr_func[type];
}

void proc_none(cpu_ctx *ctx)
{
	fprintf(stderr, "FATAL: Unknown opcode %02x! Exitting.\n", ctx->opcode);
	exit(-19);
}

void proc_nop(cpu_ctx *ctx)
{
	(void)ctx;
}

void proc_ld(cpu_ctx *ctx)
{
	(void)ctx;
	NOT_IMPLEMENTED();
}

void proc_jp(cpu_ctx *ctx)
{
	if (check_cond(ctx)) {
		ctx->regs.pc = ctx->fetch_data;
		emu_cycle(1);
	}
}

void proc_di(cpu_ctx *ctx)
{
	ctx->int_master_enabled = false;
}

void proc_xor(cpu_ctx *ctx)
{
	ctx->regs.a ^= ctx->fetch_data & 0xFF;
	cpu_set_flags(ctx, ctx->regs.a == 0, 0, 0, 0);
}
