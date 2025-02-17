

CFLAGS=-fsanitize=undefined -fstack-protector-strong -Wall -Wextra

SRCFILES=src/*

all: main main.exe

main: $(SRCFILES) lib/linux/libraylib.a lib/raylib.h
	cc src/main.c -omain -lraylib -L./lib/linux -I./lib -lm $(CFLAGS)
main.exe: $(SRCFILES) lib/win/libraylib.a lib/raylib.h
	x86_64-w64-mingw32-gcc src/main.c -omain -L./lib/win -I./lib -lraylib -lgdi32 -lwinmm
