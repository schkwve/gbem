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

#ifndef __PPU_PIPELINE_H_
#define __PPU_PIPELINE_H_

#include <common.h>

void pipeline_process();
void pipeline_fetch();
void pipeline_push_pixel();

void pipeline_fifo_reset();
bool pipeline_fifo_add();
void pixel_fifo_push(uint32_t val);
uint32_t pixel_fifo_pop();

#endif // __PPU_PIPELINE_H_
