CC = gcc
CFLAGS = -Wall -Werror -o
CP = cp

compile:
	mkdir compile; \
	cd compile; \
	$(CC) $(CFLAGS) abs2home ../abs2home/abs2home.c;

clean:
	rm -rf compile;

install-main:
	make compile; \
	cp compile/abs2home /usr/local/bin; \
	make clean;

install:
	make install-main || make clean
