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
#include <ppu_pipeline.h>
#include <ppu.h>
#include <lcd.h>

void pixel_fifo_push(uint32_t value)
{
	fifo_entry *next = malloc(sizeof(fifo_entry));
	next->next = NULL;
	next->value = value;

	if (!ppu_get_context()->pfc.pixel_fifo.head) {
		ppu_get_context()->pfc.pixel_fifo.head = next;
		ppu_get_context()->pfc.pixel_fifo.tail = next;
	} else {
		ppu_get_context()->pfc.pixel_fifo.tail->next = next;
		ppu_get_context()->pfc.pixel_fifo.tail = next;
	}

	ppu_get_context()->pfc.pixel_fifo.size++;
}

uint32_t pixel_fifo_pop()
{
	if (ppu_get_context()->pfc.pixel_fifo.size <= 0) {
		fprintf(stderr, "ERR IN PIXEL FIFO!\n");
		exit(-8);
	}

	fifo_entry *popped = ppu_get_context()->pfc.pixel_fifo.head;
	ppu_get_context()->pfc.pixel_fifo.head = popped->next;
	ppu_get_context()->pfc.pixel_fifo.size--;

	uint32_t val = popped->value;
	free(popped);

	return val;
}

bool pipeline_fifo_add()
{
	if (ppu_get_context()->pfc.pixel_fifo.size > 8) {
		return false;
	}

	int x = ppu_get_context()->pfc.fetch_x -
			(8 - (lcd_get_context()->scroll_x % 8));

	for (int i = 0; i < 8; i++) {
		int bit = 7 - i;
		uint8_t hi = !!(ppu_get_context()->pfc.bgw_fetch_data[1] & (1 << bit));
		uint8_t lo = !!(ppu_get_context()->pfc.bgw_fetch_data[2] & (1 << bit))
					 << 1;
		uint32_t color = lcd_get_context()->bg_colors[hi | lo];

		if (x >= 0) {
			pixel_fifo_push(color);
			ppu_get_context()->pfc.fifo_x++;
		}
	}

	return true;
}

void pipeline_fetch()
{
	switch (ppu_get_context()->pfc.cur_fetch_state) {
	case FS_TILE: {
		if (LCDC_BGW_ENABLE) {
			ppu_get_context()->pfc.bgw_fetch_data[0] =
				bus_read(LCDC_BG_MAP_AREA + (ppu_get_context()->pfc.map_x / 8) +
						 (((ppu_get_context()->pfc.map_y / 8)) * 32));

			if (LCDC_BGW_DATA_AREA == 0x8800) {
				ppu_get_context()->pfc.bgw_fetch_data[0] += 128;
			}
		}

		ppu_get_context()->pfc.cur_fetch_state = FS_DATA0;
		ppu_get_context()->pfc.fetch_x += 8;
	} break;

	case FS_DATA0: {
		ppu_get_context()->pfc.bgw_fetch_data[1] =
			bus_read(LCDC_BGW_DATA_AREA +
					 (ppu_get_context()->pfc.bgw_fetch_data[0] * 16) +
					 ppu_get_context()->pfc.tile_y);

		ppu_get_context()->pfc.cur_fetch_state = FS_DATA1;
	} break;

	case FS_DATA1: {
		ppu_get_context()->pfc.bgw_fetch_data[2] =
			bus_read(LCDC_BGW_DATA_AREA +
					 (ppu_get_context()->pfc.bgw_fetch_data[0] * 16) +
					 ppu_get_context()->pfc.tile_y + 1);

		ppu_get_context()->pfc.cur_fetch_state = FS_IDLE;

	} break;

	case FS_IDLE: {
		ppu_get_context()->pfc.cur_fetch_state = FS_PUSH;
	} break;

	case FS_PUSH: {
		if (pipeline_fifo_add()) {
			ppu_get_context()->pfc.cur_fetch_state = FS_TILE;
		}

	} break;
	}
}

void pipeline_push_pixel()
{
	if (ppu_get_context()->pfc.pixel_fifo.size > 8) {
		uint32_t pixel_data = pixel_fifo_pop();

		if (ppu_get_context()->pfc.line_x >=
			(lcd_get_context()->scroll_x % 8)) {
			ppu_get_context()->framebuffer[ppu_get_context()->pfc.pushed_x +
										   (lcd_get_context()->ly * XRES)] =
				pixel_data;

			ppu_get_context()->pfc.pushed_x++;
		}

		ppu_get_context()->pfc.line_x++;
	}
}

void pipeline_process()
{
	ppu_get_context()->pfc.map_y =
		(lcd_get_context()->ly + lcd_get_context()->scroll_y);
	ppu_get_context()->pfc.map_x =
		(ppu_get_context()->pfc.fetch_x + lcd_get_context()->scroll_x);
	ppu_get_context()->pfc.tile_y =
		((lcd_get_context()->ly + lcd_get_context()->scroll_y) % 8) * 2;

	if (!(ppu_get_context()->line_ticks & 1)) {
		pipeline_fetch();
	}

	pipeline_push_pixel();
}

void pipeline_fifo_reset()
{
	while (ppu_get_context()->pfc.pixel_fifo.size) {
		pixel_fifo_pop();
	}

	ppu_get_context()->pfc.pixel_fifo.head = 0;
}