CROSS_CC=x86_64-w64-mingw32-gcc
NAME=sqlite
CFLAGS= \
	-Wall \
	-Iumbox/umka_api/ \
	-I. \
	-fPIC \
	-O3
LDFLAGS=

.PHONY: all
all: build

%_linux.umi: %.c Makefile
	@echo CC $@
	@$(CC) $(CFLAGS) -shared -o $@ $< sqlite3.c $(LDFLAGS)

%_windows.umi: %.c Makefile
	@echo CC $@
	@$(CROSS_CC) -shared $(CFLAGS) -o $@ $< sqlite3.c

.PHONY: build
build: $(NAME)_windows.umi $(NAME)_linux.umi
