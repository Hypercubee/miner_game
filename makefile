

CFLAGS=-fsanitize=undefined -fstack-protector-strong -Wall -Wextra

SRCFILES=src/*

main: $(SRCFILES) lib/libraylib.a lib/raylib.h
	cc src/main.c -omain -lraylib -L./lib -I./lib -lm $(CFLAGS)
