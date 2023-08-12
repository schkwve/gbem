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
#include <stack.h>

void int_handle(cpu_ctx *ctx, uint16_t addr)
{
	stack_push16(ctx->regs.pc);
	ctx->regs.pc = addr;
}

void cpu_req_int(interrupt_type t)
{
}

void cpu_handle_int(cpu_ctx *ctx)
{
}
