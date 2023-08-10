#
# Copyright (c) 2023 Jozef Nagy
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#

CC := gcc
LD := $(CC)

INTERNAL_CFLAGS := -O2 -g3 -Wall -Wextra -Werror -pedantic -std=c99
INTERNAL_LDFLAGS :=

CFLAGS += $(INTERNAL_CFLAGS)
LDFLAGS += $(INTERNAL_LDFLAGS)

CFILES := $(shell find src -name "*.c")
OBJ := $(CFILES:.c=.o)

DEST := /usr/local/bin

PROGRAM := gbem

.PHONY: all
all: $(PROGRAM)

$(PROGRAM): $(OBJ)
	@printf " LD   $@\n"
	@$(LD) $(LDFLAGS) $(OBJ) -o $@

%.o: %.c
	@printf " CC   $^\n"
	@$(CC) $(CFLAGS) -c $< -o $@

.PHONY: format
format:
	@clang-format -i $(shell find src -name "*.c" -o -name "*.h")

.PHONY: docs
docs:
	@printf " DOXY docs/html\n"
	@mkdir -p docs
	@doxygen
	@printf " DOXY docs/latex\n"
	@make -C docs/latex 1>/dev/null
	@mv docs/latex/refman.pdf docs

.PHONY: install
install: $(PROGRAM)
	@printf " INSTALL\n"
	@sudo install -m 755 $(PROGRAM) $(DEST)

.PHONY: uninstall
uninstall:
	@printf " UNINSTALL\n"
	@sudo rm $(DEST)/$(PROGRAM)

.PHONY: clean
clean:
	@printf " CLEAN\n"
	@rm -rf $(OBJ) $(PROGRAM) docs/