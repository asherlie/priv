CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic

priv: priv.c

.PHONY:
clean:
	rm priv
