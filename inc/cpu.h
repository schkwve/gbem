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

#define CPU_FLAG_Z BIT(ctx->regs.f, 7)
#define CPU_FLAG_C BIT(ctx->regs.f, 4)

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

	bool int_master_enabled;
	uint8_t ie_reg;
} cpu_ctx;

typedef void (*IN_PROC)(cpu_ctx *);

IN_PROC inst_get_proc(in_type type);

void cpu_init();
bool cpu_step();

bool check_cond(cpu_ctx *ctx);

void cpu_fetch_instruction();
void cpu_fetch_data();
void cpu_exec();

uint8_t cpu_get_ie();
void cpu_set_ie(uint8_t n);

//// UTILS

uint16_t reverse(uint16_t n);
reg_type decode_reg(uint8_t reg);

uint16_t cpu_read_reg(reg_type rt);
cpu_regs *cpu_get_regs();

void cpu_set_reg(reg_type rt, uint16_t val);

uint8_t cpu_read_reg8(reg_type rt);
void cpu_set_reg8(reg_type rt, uint8_t val);

void cpu_set_flags(cpu_ctx *ctx, char z, char n, char h, char c);

void goto_addr(cpu_ctx *ctx, uint16_t addr, bool pushpc);

//// INSTRUCTIONS

void proc_none(cpu_ctx *ctx);
void proc_nop(cpu_ctx *ctx);
void proc_and(cpu_ctx *ctx);
void proc_xor(cpu_ctx *ctx);
void proc_or(cpu_ctx *ctx);
void proc_cp(cpu_ctx *ctx);
void proc_cb(cpu_ctx *ctx);
void proc_inc(cpu_ctx *ctx);
void proc_dec(cpu_ctx *ctx);
void proc_add(cpu_ctx *ctx);
void proc_sub(cpu_ctx *ctx);
void proc_sbc(cpu_ctx *ctx);
void proc_adc(cpu_ctx *ctx);
void proc_ld(cpu_ctx *ctx);
void proc_ldh(cpu_ctx *ctx);
void proc_jp(cpu_ctx *ctx);
void proc_jr(cpu_ctx *ctx);
void proc_call(cpu_ctx *ctx);
void proc_ret(cpu_ctx *ctx);
void proc_reti(cpu_ctx *ctx);
void proc_rst(cpu_ctx *ctx);
void proc_di(cpu_ctx *ctx);
void proc_push(cpu_ctx *ctx);
void proc_pop(cpu_ctx *ctx);

#endif // __CPU_H_
