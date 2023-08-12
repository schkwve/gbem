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

#include <bus.h>
#include <dma.h>
#include <ppu.h>
#include <unistd.h>

static dma_ctx ctx;

void dma_start(uint8_t byte)
{
	ctx.is_active = true;
	ctx.byte = 0;
	ctx.delay = 2;
	ctx.value = byte;
}

void dma_tick()
{
	if (!ctx.is_active) {
		return;
	}

	if (ctx.delay) {
		ctx.delay--;
		return;
	}

	ppu_oam_write(ctx.byte, bus_read((ctx.value * 0x100) + ctx.byte));
	ctx.byte++;
	ctx.is_active = ctx.byte < 0xA0;
}

bool dma_is_transferring()
{
	return ctx.is_active;
}
