CFLAGS=$$(pkg-config --cflags raylib)
LDFLAGS=$$(pkg-config --libs raylib)
CC=clang

.PHONY: install
install:
	$(CC) $(CFLAGS) main.c $(LDFLAGS) -o SpacerLaser
