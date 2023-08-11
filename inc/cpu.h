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

#ifndef __CPU_H_
#define __CPU_H_

#include <common.h>
#include <instr.h>

typedef struct {
	uint8_t a;
	uint8_t f;
	uint8_t b;
	uint8_t c;
	uint8_t d;
	uint8_t e;
	uint8_t h;
	uint8_t l;
	uint16_t pc;
	uint16_t sp;
} cpu_regs;

typedef struct {
	cpu_regs regs;
	uint16_t fetch_data;
	bool dest_is_mem;
	uint16_t mem_dest;
	uint8_t opcode;
	instruction *instr;

	bool is_halted;
	bool step_mode;
} cpu_ctx;

void cpu_init();
bool cpu_step();

void cpu_fetch_instruction();
void cpu_fetch_data();
void cpu_exec();

uint16_t reverse(uint16_t n);
uint16_t cpu_read_reg(reg_type rt);

#endif // __CPU_H_
