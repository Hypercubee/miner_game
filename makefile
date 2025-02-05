

CFLAGS=-fsanitize=undefined -fstack-protector-strong -Wall -Wextra

main: src/main.c lib/libraylib.a lib/raylib.h
	cc src/main.c -omain -lraylib -L./lib -I./lib -lm $(CFLAGS)
