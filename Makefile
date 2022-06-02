CFLAGS = -Wall -pedantic -Werror -std=c99

LDFLAGS = -lX11 -lImlib2

SRC = scrot.c display_X11.c image_X11.c
HED = $(SRC:.c=.h)
OBJ = $(SRC:.c=.o)

PREFIX ?= /usr/local

all: scrot

scrot: $(OBJ)
	$(CC) $(CFLAGS) -o scrot $(OBJ) $(LDFLAGS)
	rm -f *.png *.o

${OBJ}: ${HED}

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.png *.o scrot

install: all
	install -D scrot $(DESTDIR)$(PREFIX)/bin/scrot
	rm scrot
