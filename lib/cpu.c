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

cpu_ctx ctx = {0};

void cpu_init()
{
	ctx.regs.pc = 0x100;
}

bool cpu_step()
{
	if (!ctx.is_halted) {
		uint16_t pc = ctx.regs.pc;

		cpu_fetch_instruction();
		cpu_fetch_data();

		printf("%02x:%02x\n", ctx.opcode, pc);

		if (ctx.instr == NULL) {
			cpu_exec();
		}
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
		case AM_IMP: {	// nothing needs to be returned
			return;
		}
		case AM_R: {	// read data from a register
			ctx.fetch_data = cpu_read_reg(ctx.instr->reg1);
			return;
		}
		case AM_R_D8: {	// set an 8-bit value to a register
			ctx.fetch_data = bus_read(ctx.regs.pc);
			emu_cycle(1);
			ctx.regs.pc++;
			return;
		}
		case AM_D16: {	// set a 16-bit value
			uint16_t lo = bus_read(ctx.regs.pc);
			emu_cycle(1);
			uint16_t hi = bus_read(ctx.regs.pc + 1);
			emu_cycle(1);
			ctx.fetch_data = lo | (hi << 8);
			ctx.regs.pc += 2;
			return;
		}
		default: {
			printf("Unknown addressing mode: %d\n", ctx.instr->mode);
			exit(1);
		}
	}
}

void cpu_exec()
{
	IN_PROC proc = inst_get_proc(ctx.instr->type);

	if (!proc) {
		NOT_IMPLEMENTED();
	}

	proc(&ctx);
}
