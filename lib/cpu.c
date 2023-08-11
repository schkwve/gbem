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
#include <bus.h>

cpu_ctx ctx = { 0 };

void cpu_init()
{
	ctx.regs.a = 0x01;
	ctx.regs.pc = 0x100;
}

bool cpu_step()
{
	if (!ctx.is_halted) {
		uint16_t pc = ctx.regs.pc;

		cpu_fetch_instruction();
		cpu_fetch_data();

		printf("%04x:%02x (%02x %02x %02x)\n", pc, ctx.instr->type, ctx.opcode,
			   bus_read(pc + 1), bus_read(pc + 2));

		if (ctx.instr == NULL) {
			exit(-1);
		}
		cpu_exec();
	}

	return true;
}

void cpu_fetch_instruction()
{
	ctx.opcode = bus_read(ctx.regs.pc++);
	ctx.instr = inst_get_by_opcode(ctx.opcode);
	if (ctx.instr == NULL) {
		fprintf(stderr, "Unknown Instruction: %02x\n", ctx.opcode);
		exit(-8);
	}
}

void cpu_fetch_data()
{
	ctx.dest_is_mem = false;
	ctx.mem_dest = 0;

	if (ctx.instr == NULL) {
		return;
	}

	switch (ctx.instr->mode) {
	case AM_IMP: {
		return;
	}
	case AM_R: {
		ctx.fetch_data = cpu_read_reg(ctx.instr->reg1);
		return;
	}
	case AM_R_R: {
		ctx.fetch_data = cpu_read_reg(ctx.instr->reg2);
		return;
	}
	case AM_R_D8: {
		ctx.fetch_data = bus_read(ctx.regs.pc);
		emu_cycle(1);
		ctx.regs.pc++;
		return;
	}
	case AM_R_D16:
	case AM_D16: {
		uint16_t lo = bus_read(ctx.regs.pc);
		emu_cycle(1);

		uint16_t hi = bus_read(ctx.regs.pc + 1);
		emu_cycle(1);

		ctx.fetch_data = lo | (hi << 8);

		ctx.regs.pc += 2;

		return;
	}
	case AM_MR_R: {
		ctx.fetch_data = cpu_read_reg(ctx.instr->reg2);
		ctx.mem_dest = cpu_read_reg(ctx.instr->reg1);
		ctx.dest_is_mem = true;

		if (ctx.instr->reg1 == RT_C) {
			ctx.mem_dest |= 0xFF00;
		}
		return;
	}
	case AM_R_MR: {
		uint16_t addr = cpu_read_reg(ctx.instr->reg2);

		if (ctx.instr->reg2 == RT_C) {
			addr |= 0xFF00;
		}

		ctx.fetch_data = bus_read(addr);
		emu_cycle(1);
		return;
	}
	case AM_R_HLI: {
		ctx.fetch_data = bus_read(cpu_read_reg(ctx.instr->reg2));
		emu_cycle(1);
		cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) + 1);
		return;
	}
	case AM_R_HLD: {
		ctx.fetch_data = bus_read(cpu_read_reg(ctx.instr->reg2));
		emu_cycle(1);
		cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) - 1);
		return;
	}
	case AM_HLI_R: {
		ctx.fetch_data = cpu_read_reg(ctx.instr->reg2);
		ctx.mem_dest = cpu_read_reg(ctx.instr->reg1);
		ctx.dest_is_mem = true;
		cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) + 1);
		return;
	}
	case AM_HLD_R: {
		ctx.fetch_data = cpu_read_reg(ctx.instr->reg2);
		ctx.mem_dest = cpu_read_reg(ctx.instr->reg1);
		ctx.dest_is_mem = true;
		cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) - 1);
		return;
	}
	case AM_R_A8: {
		ctx.fetch_data = bus_read(ctx.regs.pc);
		emu_cycle(1);
		ctx.regs.pc++;
		return;
	}
	case AM_A8_R: {
		ctx.mem_dest = bus_read(ctx.regs.pc) | 0xFF00;
		ctx.dest_is_mem = true;
		emu_cycle(1);
		ctx.regs.pc++;
		return;
	}
	case AM_HL_SPR: {
		ctx.fetch_data = bus_read(ctx.regs.pc);
		emu_cycle(1);
		ctx.regs.pc++;
		return;
	}
	case AM_D8: {
		ctx.fetch_data = bus_read(ctx.regs.pc);
		emu_cycle(1);
		ctx.regs.pc++;
		return;
	}
	case AM_A16_R:
	case AM_D16_R: {
		uint16_t lo = bus_read(ctx.regs.pc);
		emu_cycle(1);

		uint16_t hi = bus_read(ctx.regs.pc + 1);
		emu_cycle(1);

		ctx.mem_dest = lo | (hi << 8);
		ctx.dest_is_mem = true;

		ctx.regs.pc += 2;
		ctx.fetch_data = cpu_read_reg(ctx.instr->reg2);
		return;
	}
	case AM_MR_D8: {
		ctx.fetch_data = bus_read(ctx.regs.pc);
		emu_cycle(1);
		ctx.regs.pc++;
		ctx.mem_dest = cpu_read_reg(ctx.instr->reg1);
		ctx.dest_is_mem = true;
		return;
	}
	case AM_MR: {
		ctx.mem_dest = cpu_read_reg(ctx.instr->reg1);
		ctx.dest_is_mem = true;
		ctx.fetch_data = bus_read(cpu_read_reg(ctx.instr->reg1));
		emu_cycle(1);
		return;
	}
	case AM_R_A16: {
		uint16_t lo = bus_read(ctx.regs.pc);
		emu_cycle(1);
		uint16_t hi = bus_read(ctx.regs.pc + 1);
		emu_cycle(1);
		uint16_t addr = lo | (hi << 8);

		ctx.regs.pc += 2;
		ctx.fetch_data = bus_read(addr);
		emu_cycle(1);

		return;
	}
	default: {
		printf("Unknown Addressing Mode! %d (%02X)\n", ctx.instr->mode,
			   ctx.opcode);
		exit(-7);
		return;
	}
	}
}

uint8_t cpu_get_ie()
{
	return ctx.ie_reg;
}

void cpu_set_ie(uint8_t n)
{
	ctx.ie_reg = n;
}

void cpu_exec()
{
	IN_PROC proc = inst_get_proc(ctx.instr->type);

	if (!proc) {
		NOT_IMPLEMENTED();
	}

	proc(&ctx);
}
