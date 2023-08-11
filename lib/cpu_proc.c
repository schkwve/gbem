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
#include <bus.h>
#include <stack.h>
#include <emu.h>

// clang-format off
static IN_PROC instr_func[] = {
	[IN_NONE] = proc_none,
	[IN_NOP] = proc_nop,
	[IN_AND] = proc_and,
	[IN_XOR] = proc_xor,
	[IN_OR] = proc_or,
	[IN_CP] = proc_cp,
	[IN_CB] = proc_cb,
	[IN_INC] = proc_inc,
	[IN_DEC] = proc_dec,
	[IN_ADD] = proc_add,
	[IN_SUB] = proc_sub,
	[IN_SBC] = proc_sbc,
	[IN_ADC] = proc_adc,
	[IN_LD] = proc_ld,
	[IN_LDH] = proc_ldh,
	[IN_JP] = proc_jp,
	[IN_JR] = proc_jr,
	[IN_CALL] = proc_call,
	[IN_RET] = proc_ret,
	[IN_RETI] = proc_reti,
	[IN_RST] = proc_rst,
	[IN_DI] = proc_di,
	[IN_PUSH] = proc_push,
	[IN_POP] = proc_pop
};
// clang-format on

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

void proc_and(cpu_ctx *ctx)
{
	ctx->regs.a &= ctx->fetch_data;
	cpu_set_flags(ctx, ctx->regs.a == 0, 0, 1, 0);
}

void proc_xor(cpu_ctx *ctx)
{
	ctx->regs.a ^= ctx->fetch_data & 0xFF;
	cpu_set_flags(ctx, ctx->regs.a == 0, 0, 0, 0);
}

void proc_or(cpu_ctx *ctx)
{
	ctx->regs.a |= ctx->fetch_data & 0xFF;
	cpu_set_flags(ctx, ctx->regs.a == 0, 0, 0, 0);
}

void proc_cp(cpu_ctx *ctx)
{
	int n = (int)ctx->regs.a - (int)ctx->fetch_data;
	cpu_set_flags(ctx, n == 0, 1,
				  ((int)ctx->regs.a & 0x0F) - ((int)ctx->fetch_data & 0x0F) < 0,
				  n < 0);
}

void proc_cb(cpu_ctx *ctx)
{
	uint8_t op = ctx->fetch_data;
	reg_type reg = decode_reg(op & 0b111);
	uint8_t bit = (op >> 3) & 0b111;
	uint8_t bit_op = (op >> 6) & 0b111;
	uint8_t reg_val = cpu_read_reg8(reg);

	emu_cycle(1);

	if (reg == RT_HL) {
		emu_cycle(2);
	}

	switch (bit_op) {
	case 1:
		// BIT
		cpu_set_flags(ctx, !(reg_val & (1 << bit)), 0, 1, -1);
		return;
	case 2:
		// RST
		reg_val &= ~(1 << bit);
		cpu_set_reg8(reg, reg_val);
		return;
	case 3:
		// SET
		reg_val |= (1 << bit);
		cpu_set_reg8(reg, reg_val);
		return;
	}

	bool cflag = CPU_FLAG_C;

	switch (bit) {
	case 0: {
		// RLC
		bool set_c = false;
		uint8_t res = (reg_val << 1) & 0xFF;

		if ((reg_val & (1 << 7)) != 0) {
			res |= 1;
			set_c = true;
		}

		cpu_set_reg8(reg, res);
		cpu_set_flags(ctx, res == 0, false, false, set_c);
		return;
	}
	case 1: {
		// RRC
		uint8_t old = reg_val;
		reg_val >>= 1;
		reg_val |= (old << 7);

		cpu_set_reg8(reg, reg_val);
		cpu_set_flags(ctx, !reg_val, false, false, old & 1);
		return;
	}
	case 2: {
		// RL
		uint8_t old = reg_val;
		reg_val <<= 1;
		reg_val |= cflag;

		cpu_set_reg8(reg, reg_val);
		cpu_set_flags(ctx, !reg_val, false, false, !!(old & 0x80));
		return;
	}
	case 3: {
		// RR
		uint8_t old = reg_val;
		reg_val >>= 1;
		reg_val |= (cflag << 7);

		cpu_set_reg8(reg, reg_val);
		cpu_set_flags(ctx, !reg_val, false, false, old & 1);
		return;
		return;
	}
	case 4: {
		// SLA
		uint8_t old = reg_val;
		reg_val <<= 1;

		cpu_set_reg8(reg, reg_val);
		cpu_set_flags(ctx, !reg_val, false, false, !!(old & 0x80));
		return;
	}
	case 5: {
		// SRA
		uint8_t u = (int8_t)reg_val >> 1;
		cpu_set_reg8(reg, u);
		cpu_set_flags(ctx, !u, 0, 0, reg_val & 1);
		return;
	}
	case 6: {
		// SWAP
		reg_val = ((reg_val & 0xF0) >> 4) | ((reg_val & 0xF) << 4);
		cpu_set_reg8(reg, reg_val);
		cpu_set_flags(ctx, reg_val == 0, false, false, false);
		return;
	}
	case 7: {
		// SRL
		uint8_t u = reg_val >> 1;
		cpu_set_reg8(reg, u);
		cpu_set_flags(ctx, !u, 0, 0, reg_val & 1);
		return;
	}
	}

	fprintf(stderr, "FATAL: Invalid CB %02x\n", op);
	NOT_IMPLEMENTED();
}

void proc_inc(cpu_ctx *ctx)
{
	uint16_t val = cpu_read_reg(ctx->instr->reg1) + 1;

	if (ctx->instr->reg1 >= RT_AF) {
		emu_cycle(1);
	}

	if (ctx->instr->reg1 == RT_HL && ctx->instr->mode == AM_MR) {
		val = bus_read(cpu_read_reg(RT_HL)) + 1;
		val &= 0xFF;
		bus_write(cpu_read_reg(RT_HL), val);
	} else {
		cpu_set_reg(ctx->instr->reg1, val);
		val = cpu_read_reg(ctx->instr->reg1);
	}

	if ((ctx->opcode & 0x03) == 0x03) {
		return;
	}

	cpu_set_flags(ctx, val == 0, 0, (val & 0x0F) == 0, -1);
}

void proc_dec(cpu_ctx *ctx)
{
	uint16_t val = cpu_read_reg(ctx->instr->reg1) + 1;

	if (ctx->instr->reg1 >= RT_AF) {
		emu_cycle(1);
	}

	if (ctx->instr->reg1 == RT_HL && ctx->instr->mode == AM_MR) {
		val = bus_read(cpu_read_reg(RT_HL)) + 1;
		val &= 0xFF;
		bus_write(cpu_read_reg(RT_HL), val);
	} else {
		cpu_set_reg(ctx->instr->reg1, val);
		val = cpu_read_reg(ctx->instr->reg1);
	}

	if ((ctx->opcode & 0x03) == 0x03) {
		return;
	}

	cpu_set_flags(ctx, val == 0, 0, (val & 0x0F) == 0, -1);
}

void proc_add(cpu_ctx *ctx)
{
	uint32_t val = cpu_read_reg(ctx->instr->reg1) + ctx->fetch_data;
	bool is_16bit = ctx->instr->reg1 >= RT_AF;

	if (is_16bit) {
		emu_cycle(1);
	}

	if (ctx->instr->reg1 == RT_SP) {
		val = cpu_read_reg(ctx->instr->reg1) + (char)ctx->fetch_data;
	}

	int z = (val & 0xFF) == 0;
	int h =
		(cpu_read_reg(ctx->instr->reg1) & 0x0F) + (ctx->fetch_data & 0x0F) >=
		0x10;
	int c = (int)(cpu_read_reg(ctx->instr->reg1) & 0xFF) +
				(int)(ctx->fetch_data & 0xFF) >
			0x100;

	if (is_16bit) {
		z = -1;
		h = (cpu_read_reg(ctx->instr->reg1) & 0xFFF) +
				(ctx->fetch_data & 0xFFF) >
			0x1000;
		uint32_t n = ((uint32_t)cpu_read_reg(ctx->instr->reg1)) +
					 ((uint32_t)ctx->fetch_data);
		c = n >= 0x10000;
	}

	if (ctx->instr->reg1 == RT_SP) {
		z = 0;
		h = (cpu_read_reg(ctx->instr->reg1) & 0xF) + (ctx->fetch_data & 0xF) >=
			0x10;
		c = (int)(cpu_read_reg(ctx->instr->reg1) & 0xFF) +
				(int)(ctx->fetch_data & 0xFF) >
			0x100;
	}

	cpu_set_reg(ctx->instr->reg1, val & 0xFFFF);
	cpu_set_flags(ctx, z, 0, h, c);
}

void proc_sub(cpu_ctx *ctx)
{
	uint16_t val = cpu_read_reg(ctx->instr->reg1) - ctx->fetch_data;

	int z = val == 0;
	int h = ((int)cpu_read_reg(ctx->instr->reg1) & 0xF) -
				((int)ctx->fetch_data & 0xF) <
			0;
	int c = ((int)cpu_read_reg(ctx->instr->reg1)) - ((int)ctx->fetch_data) < 0;

	cpu_set_reg(ctx->instr->reg1, val);
	cpu_set_flags(ctx, z, 1, h, c);
}

void proc_sbc(cpu_ctx *ctx)
{
	uint16_t val = ctx->fetch_data + CPU_FLAG_C;

	int z = cpu_read_reg(ctx->instr->reg1) - val == 0;
	int h = ((int)cpu_read_reg(ctx->instr->reg1) & 0xF) -
				((int)ctx->fetch_data & 0xF) - ((int)CPU_FLAG_C) <
			0;
	int c = ((int)cpu_read_reg(ctx->instr->reg1)) - ((int)ctx->fetch_data) -
				((int)CPU_FLAG_C) <
			0;

	cpu_set_reg(ctx->instr->reg1, cpu_read_reg(ctx->instr->reg1) - val);
	cpu_set_flags(ctx, z, 1, h, c);
}

void proc_adc(cpu_ctx *ctx)
{
	uint16_t u = ctx->fetch_data;
	uint16_t a = ctx->regs.a;
	uint16_t c = CPU_FLAG_C;

	ctx->regs.a = (a + u + c) & 0xFF;
	cpu_set_flags(ctx, ctx->regs.a == 0, 0, (a & 0xF) + (u & 0xF) + c > 0xF,
				  a + u + c > 0xFF);
}

void proc_ld(cpu_ctx *ctx)
{
	if (ctx->dest_is_mem) {
		if (ctx->instr->reg2 >= RT_AF) {
			emu_cycle(1);
			bus_write16(ctx->mem_dest, ctx->fetch_data);
		} else {
			bus_write(ctx->mem_dest, ctx->fetch_data);
		}

		emu_cycle(1);
		return;
	}

	if (ctx->instr->mode == AM_HL_SPR) {
		uint8_t hflag =
			(cpu_read_reg(ctx->instr->reg2) & 0xF) + (ctx->fetch_data & 0xF) >=
			0x10;
		uint8_t cflag =
			(cpu_read_reg(ctx->instr->reg2) & 0xF) + (ctx->fetch_data & 0xFF) >=
			0x100;

		cpu_set_flags(ctx, 0, 0, hflag, cflag);
		cpu_set_reg(ctx->instr->reg1,
					cpu_read_reg(ctx->instr->reg2) + (char)ctx->fetch_data);

		return;
	}

	cpu_set_reg(ctx->instr->reg1, ctx->fetch_data);
}

void proc_ldh(cpu_ctx *ctx)
{
	if (ctx->instr->reg1 == RT_A) {
		cpu_set_reg(ctx->instr->reg1, bus_read(ctx->fetch_data | 0xFF00));
	} else {
		bus_write(ctx->mem_dest, ctx->regs.a);
	}

	emu_cycle(1);
}

void proc_jp(cpu_ctx *ctx)
{
	goto_addr(ctx, ctx->fetch_data, false);
}

void proc_jr(cpu_ctx *ctx)
{
	char rel = (char)(ctx->fetch_data & 0xFF);
	uint16_t addr = ctx->regs.pc + rel;
	goto_addr(ctx, addr, false);
}

void proc_call(cpu_ctx *ctx)
{
	goto_addr(ctx, ctx->fetch_data, true);
}

void proc_ret(cpu_ctx *ctx)
{
	if (ctx->instr->cond != CT_NONE) {
		emu_cycle(1);
	}

	if (check_cond(ctx)) {
		uint16_t lo = stack_pop();
		emu_cycle(1);
		uint16_t hi = stack_pop();
		emu_cycle(1);

		uint16_t n = (hi << 8) | lo;
		ctx->regs.pc = n;

		emu_cycle(1);
	}
}

void proc_reti(cpu_ctx *ctx)
{
	ctx->int_master_enabled = true;
	proc_ret(ctx);
}

void proc_rst(cpu_ctx *ctx)
{
	goto_addr(ctx, ctx->instr->param, true);
}

void proc_di(cpu_ctx *ctx)
{
	ctx->int_master_enabled = false;
}

void proc_push(cpu_ctx *ctx)
{
	uint16_t hi = (cpu_read_reg(ctx->instr->reg1) >> 8) & 0xFF;
	emu_cycle(1);
	stack_push(hi);

	uint16_t lo = cpu_read_reg(ctx->instr->reg1) & 0xFF;
	emu_cycle(1);
	stack_push(lo);

	emu_cycle(1);
}

void proc_pop(cpu_ctx *ctx)
{
	uint16_t lo = stack_pop();
	emu_cycle(1);
	uint16_t hi = stack_pop();
	emu_cycle(1);

	uint16_t n = (hi << 8) | lo;

	cpu_set_reg(ctx->instr->reg1, n);

	if (ctx->instr->reg1 == RT_AF) {
		cpu_set_reg(ctx->instr->reg1, n & 0xFFF0);
	}
}
