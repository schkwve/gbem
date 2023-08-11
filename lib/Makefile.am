lib_LTLIBRARIES = libgbem.la

libgbem_la_SOURCES = emu.c \
				bus.c \
				instr.c \
				cartridge.c \
				cpu.c \
				cpu_util.c \
				ppu.c \
				timer.c

libgbem_la_CPPFLAGS = -I${abs_top_srcdir}/inc
libgbem_la_LIBADD = -lSDL2 -lSDL2_ttf
AUTOMAKE_OPTIONS = subdir-objects