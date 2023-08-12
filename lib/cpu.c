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
#include <int.h>
#include <timer.h>

cpu_ctx ctx = { 0 };

void cpu_init()
{
	ctx.regs.pc = 0x100;
	ctx.regs.sp = 0xFFFE;
	*((short *)&ctx.regs.a) = 0xB001;
	*((short *)&ctx.regs.b) = 0x1300;
	*((short *)&ctx.regs.d) = 0xD800;
	*((short *)&ctx.regs.h) = 0x4D01;
	ctx.ie_reg = 0;
	ctx.int_flags = 0;
	ctx.int_master_enabled = false;
	ctx.enable_ime = false;

	timer_get_context()->div = 0xABCC;
}

bool cpu_step()
{
	if (!ctx.is_halted) {
		cpu_fetch_instruction();
		emu_cycle(1);
		cpu_fetch_data();

		if (ctx.instr == NULL) {
			printf("NULL Instruction\n");
			exit(-1);
		}
		cpu_exec();
	} else {
		emu_cycle(1);
		if (ctx.int_flags) {
			ctx.is_halted = false;
		}
	}

	if (ctx.int_master_enabled) {
		cpu_handle_int(&ctx);
		ctx.enable_ime = false;
	}

	if (ctx.enable_ime) {
		ctx.int_master_enabled = true;
	}

	return true;
}

void cpu_fetch_instruction()
{
	ctx.opcode = bus_read(ctx.regs.pc++);
	ctx.instr = inst_get_by_opcode(ctx.opcode);
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

void cpu_req_int(interrupt_type t)
{
	ctx.int_flags |= t;
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
		exit(-1);
	}

	proc(&ctx);
}
