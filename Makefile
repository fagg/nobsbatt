#Copyright 2021, Dr Ashton Fagg <ashton@fagg.id.au>
#
#Permission to use, copy, modify, and/or distribute this software for
#any purpose with or without fee is hereby granted, provided that the
#above copyright notice and this permission notice appear in all
#copies.
#
#THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
#WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
#WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
#AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
#DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
#PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
#TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
#PERFORMANCE OF THIS SOFTWARE.

CC=clang
CFLAGS=-Wall -Werror -pedantic -I/usr/X11R6/include -I/usr/X11R6/include/freetype2
LDFLAGS=-lX11 -lXft -L/usr/X11R6/lib

nobsbatt:
	$(CC) nobsbatt.c -o nobsbatt $(CFLAGS) $(LDFLAGS)

# Installation.
# Assumes OpenBSD, sticks executable into /usr/local/bin
install:
	/usr/bin/install -o root -g wheel -m 755 nobsbatt /usr/local/bin/nobsbatt

clean:
	rm -rf ./nobsbatt
