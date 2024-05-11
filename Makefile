# SPDX-FileCopyrightText: 2024 KUSANAGI Mitsuhisa <mikkun@mbg.nifty.com>
# SPDX-License-Identifier: GPL-3.0-or-later

CC          = gcc
CFLAGS      = -O2 -Wall
PREFIX      = /usr/local
SHLIBCFLAGS = -fPIC -shared
SHLIBS      = sdl2.so sdl2_mixer.so

CP    := cp
RM    := rm -f
MKDIR := mkdir -p
RMDIR := rmdir

.PHONY: all
all: $(SHLIBS)

sdl2.so: sdl2.c varinit_constant.c -lSDL2
	$(CC) $(CFLAGS) $(SHLIBCFLAGS) -o $@ $^

sdl2_mixer.so: sdl2_mixer.c varinit_constant.c -lSDL2_mixer
	$(CC) $(CFLAGS) $(SHLIBCFLAGS) -o $@ $^

.PHONY: clean
clean:
	$(RM) $(SHLIBS)

.PHONY: install
install:
	$(MKDIR) $(PREFIX)/lib/gawk
	$(CP) $(SHLIBS) $(PREFIX)/lib/gawk/

.PHONY: uninstall
uninstall:
	@for f in $(SHLIBS); \
	do \
		echo $(RM) $(PREFIX)/lib/gawk/$$f; \
		$(RM) $(PREFIX)/lib/gawk/$$f; \
	done
	$(RMDIR) $(PREFIX)/lib/gawk
