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

#ifndef __UI_H_
#define __UI_H_

#include <SDL2/SDL.h>
#include <common.h>

void ui_init(int width, int height);
void ui_handle_events();

void ui_update();
void ui_update_debug();

void display_tile(SDL_Surface *surface, uint16_t start_loc, uint16_t tile_num,
				  int x, int y);

#endif // __UI_H_
