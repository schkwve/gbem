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

#include <common.h>
#include <bus.h>
#include <ui.h>
#include <emu.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

SDL_Window *sdlWindow;
SDL_Renderer *sdlRenderer;
SDL_Texture *sdlTexture;
SDL_Surface *screen;

SDL_Window *sdlDebugWindow;
SDL_Renderer *sdlDebugRenderer;
SDL_Texture *sdlDebugTexture;
SDL_Surface *debugScreen;

static int scale = 4;

static unsigned long tile_colors[4] = { 0xFFFFFFFF, 0xFFAAAAAA, 0xFF555555,
										0xFF000000 };

void ui_init(int width, int height)
{
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();

	SDL_CreateWindowAndRenderer(width, height, 0, &sdlWindow, &sdlRenderer);
	SDL_CreateWindowAndRenderer(16 * 8 * scale, 32 * 8 * scale, 0,
								&sdlDebugWindow, &sdlDebugRenderer);

	debugScreen = SDL_CreateRGBSurface(
		0, (16 * 8 * scale) + (16 * scale), (32 * 8 * scale) + (64 * scale), 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	sdlDebugTexture = SDL_CreateTexture(
		sdlDebugRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
		(16 * 8 * scale) + (16 * scale), (32 * 8 * scale) + (64 * scale));
}

void ui_handle_events()
{
	SDL_Event e;
	while (SDL_PollEvent(&e) > 0) {
		if (e.type == SDL_WINDOWEVENT &&
			e.window.event == SDL_WINDOWEVENT_CLOSE) {
			emu_get_context()->should_die = true;
		}
	}
}

void ui_update()
{
	ui_update_debug();
}

void ui_update_debug()
{
	int x_draw = 0;
	int y_draw = 0;
	int tile_num = 0;

	SDL_Rect rc;
	rc.x = 0;
	rc.y = 0;
	rc.w = debugScreen->w;
	rc.h = debugScreen->h;
	SDL_FillRect(debugScreen, &rc, 0xFF111111);

	uint16_t addr = 0x8000;

	for (int y = 0; y < 24; y++) {
		for (int x = 0; x < 16; x++) {
			display_tile(debugScreen, addr, tile_num, x_draw + (x * scale),
						 y_draw + (y * scale));
			x_draw += (8 * scale);
			tile_num++;
		}

		y_draw += (8 * scale);
		x_draw = 0;
	}

	SDL_UpdateTexture(sdlDebugTexture, NULL, debugScreen->pixels,
					  debugScreen->pitch);
	SDL_RenderClear(sdlDebugRenderer);
	SDL_RenderCopy(sdlDebugRenderer, sdlDebugTexture, NULL, NULL);
	SDL_RenderPresent(sdlDebugRenderer);
}

void display_tile(SDL_Surface *surface, uint16_t start_loc, uint16_t tile_num,
				  int x, int y)
{
	SDL_Rect rc;

	for (int tile_y = 0; tile_y < 16; tile_y += 2) {
		uint8_t b1 = bus_read(start_loc + (tile_num * 16) + tile_y);
		uint8_t b2 = bus_read(start_loc + (tile_num * 16) + tile_y + 1);

		for (int bit = 7; bit >= 0; bit--) {
			uint8_t hi = !!(b1 & (1 << bit)) << 1;
			uint8_t lo = !!(b2 & (1 << bit));

			uint8_t color = hi | lo;

			rc.x = x + ((7 - bit) * scale);
			rc.y = y + (tile_y / 2 * scale);
			rc.w = scale;
			rc.h = scale;

			SDL_FillRect(surface, &rc, tile_colors[color]);
		}
	}
}

void delay(uint32_t ms)
{
	SDL_Delay(ms);
}
