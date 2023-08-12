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
#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

//#ifdef linux
#include <pthread.h>
//#endif

#include <common.h>
#include <emu.h>
#include <cartridge.h>
#include <cpu.h>
#include <timer.h>
#include <dma.h>
#include <ui.h>

static emu_ctx ctx;

emu_ctx *emu_get_context()
{
	return &ctx;
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

	ui_init(1024, 768);
	timer_init();
	cpu_init();

	//#ifdef linux
	pthread_t cpu;

	if (pthread_create(&cpu, NULL, cpu_run, NULL)) {
		fprintf(stderr, "FATAL: Failed to create thread\n");
		exit(-1);
	}
	//#endif

	while (!ctx.should_die) {
		usleep(1000);
		ui_handle_events();

		ui_update();
	}

	return 0;
}

void *cpu_run(void *p)
{
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
			return NULL;
		}

		ctx.ticks++;
	}

	return NULL;
}

void emu_cycle(int num)
{
	for (int i = 0; i < num; i++) {
		for (int n = 0; n < 4; n++) {
			ctx.ticks++;
			timer_tick();
		}
		dma_tick();
	}
}
