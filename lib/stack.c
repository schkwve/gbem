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

#include <stack.h>
#include <cpu.h>
#include <bus.h>

void stack_push(uint8_t data)
{
	cpu_get_regs()->sp--;
	bus_write(cpu_get_regs()->sp, data);
}

void stack_push16(uint16_t data)
{
	stack_push((data >> 8) & 0xFF);
	stack_push(data & 0xFF);
}

uint8_t stack_pop()
{
	return bus_read(cpu_get_regs()->sp++);
}

uint16_t stack_pop16()
{
	uint8_t lo = stack_pop();
	uint8_t hi = stack_pop();

	return (hi << 8) | lo;
}
