CFLAGS=-framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL libs/raylib/macos/libraylib.a -I./libs/raylib/include
CC=clang

.PHONY: run
run:
	$(CC) $(CFLAGS) -g main.c -o SpacerLaser
	./SpacerLaser

.PHONY: build
build:
	$(CC) $(CFLAGS) -g main.c -o SpacerLaser
