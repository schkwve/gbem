lib_LTLIBRARIES = libgbem.la

libgbem_la_SOURCES = emu.c \
				bus.c \
				instr.c \
				cartridge.c \
				cpu.c \
				cpu_proc.c \
				cpu_util.c \
				ram.c \
				io.c \
				stack.c \
				dma.c \
				ppu.c \
				int.c \
				timer.c \
				ui.c

libgbem_la_CPPFLAGS = -I${abs_top_srcdir}/inc
libgbem_la_LIBADD = -lSDL2 -lSDL2_ttf
AUTOMAKE_OPTIONS = subdir-objects