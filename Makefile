CFLAGS=$$(pkg-config --cflags raylib)
LDFLAGS=$$(pkg-config --libs raylib)
CC=clang

.PHONY: install
build:
	$(CC) $(CFLAGS) -g main.c $(LDFLAGS) -o SpacerLaser
	./SpacerLaser
