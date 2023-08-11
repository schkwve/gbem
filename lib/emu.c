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

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <emu.h>
#include <cartridge.h>
#include <cpu.h>

static emu_ctx ctx;

emu_ctx *emu_get_context()
{
	return &ctx;
}

void delay(uint32_t ms)
{
	SDL_Delay(ms);
}

int emu_run(int argc, char **argv)
{
	if (argc < 2) {
		printf("Usage: %s <rom_file>\n", argv[0]);
		return -1;
	}

	if (!cart_load(argv[1])) {
		printf("Failed to load ROM file: %s\n", argv[1]);
		return -2;
	}

	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();

	cpu_init();

	ctx.running = true;
	ctx.paused = false;
	ctx.ticks = 0;

	while (ctx.running) {
		if (ctx.paused) {
			delay(10);
			continue;
		}

		if (!cpu_step()) {
			printf("CPU Stopped\n");
			return -3;
		}

		ctx.ticks++;
	}

	return 0;
}
