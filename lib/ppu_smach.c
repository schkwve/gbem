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

#include <ppu.h>
#include <cpu.h>
#include <int.h>
#include <lcd.h>
#include <ppu_pipeline.h>
#include <ui.h>

static uint32_t target_frame_time = 1000 / 60;
static long prev_frame_time = 0;
static long start_timer = 0;
static long frame_count = 0;

void increment_ly()
{
	lcd_get_context()->ly++;

	if (lcd_get_context()->ly == lcd_get_context()->ly_cmp) {
		LCDS_LYC_SET(1);

		if (LCDS_STAT_INT(SS_LYC)) {
			cpu_req_int(IT_LCD_STAT);
		}
	} else {
		LCDS_LYC_SET(0);
	}
}

void ppu_mode_oam()
{
	if (ppu_get_context()->line_ticks >= 80) {
		LCDS_MODE_SET(MODE_XFER);

		ppu_get_context()->pfc.cur_fetch_state = FS_TILE;
		ppu_get_context()->pfc.line_x = 0;
		ppu_get_context()->pfc.fetch_x = 0;
		ppu_get_context()->pfc.pushed_x = 0;
		ppu_get_context()->pfc.fifo_x = 0;
	}
}

void ppu_mode_xfer()
{
	pipeline_process();

	if (ppu_get_context()->pfc.pushed_x >= XRES) {
		pipeline_fifo_reset();

		LCDS_MODE_SET(MODE_HBLANK);

		if (LCDS_STAT_INT(SS_HBLANK)) {
			cpu_req_int(IT_LCD_STAT);
		}
	}
}

void ppu_mode_vblank()
{
	if (ppu_get_context()->line_ticks >= TICKS_PER_LINE) {
		increment_ly();

		if (lcd_get_context()->ly >= LINES_PER_FRAME) {
			LCDS_MODE_SET(MODE_OAM);
			lcd_get_context()->ly = 0;
		}

		ppu_get_context()->line_ticks = 0;
	}
}

void ppu_mode_hblank()
{
	if (ppu_get_context()->line_ticks >= TICKS_PER_LINE) {
		increment_ly();

		if (lcd_get_context()->ly >= YRES) {
			LCDS_MODE_SET(MODE_VBLANK);

			cpu_req_int(IT_VBLANK);

			if (LCDS_STAT_INT(SS_VBLANK)) {
				cpu_req_int(IT_LCD_STAT);
			}

			ppu_get_context()->cur_frame++;

			//calc FPS...
			uint32_t end = get_ticks();
			uint32_t frame_time = end - prev_frame_time;

			if (frame_time < target_frame_time) {
				delay((target_frame_time - frame_time));
			}

			if (end - start_timer >= 1000) {
				uint32_t fps = frame_count;
				start_timer = end;
				frame_count = 0;

				printf("FPS: %d\n", fps);
			}

			frame_count++;
			prev_frame_time = get_ticks();

		} else {
			LCDS_MODE_SET(MODE_OAM);
		}

		ppu_get_context()->line_ticks = 0;
	}
}