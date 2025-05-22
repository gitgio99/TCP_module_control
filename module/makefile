# Makefile (모든 .so를 module 디렉토리에 생성)

MODULES = led buz cds seg
SO_LIBS = $(addprefix lib, $(addsuffix .so, $(MODULES)))

CC = gcc
CFLAGS = -Wall -fPIC
LDFLAGS = -lwiringPi

all: $(SO_LIBS)

lib%.so: %.c
	$(CC) -shared -fPIC -o $@ $< $(LDFLAGS)

clean:
	rm -f *.so
	rm -f *.o

.PHONY: all clean
