CFLAGS=$$(pkg-config --cflags raylib)
LDFLAGS=$$(pkg-config --libs raylib)
CC=clang

.PHONY: run
run:
	$(CC) $(CFLAGS) -g main.c $(LDFLAGS) -o SpacerLaser
	./SpacerLaser

.PHONY: build
build:
	$(CC) $(CFLAGS) -g main.c $(LDFLAGS) -o SpacerLaser
