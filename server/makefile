CC = gcc
CFLAGS = -Wall -I../module
LDFLAGS = -lpthread -ldl -lwiringPi

SRCS = server.c \
       ../module/init_module.c \
       ../module/led.c \
       ../module/buz.c \
       ../module/seg.c \
       ../module/cds.c

TARGET = server

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $(SRCS) $(LDFLAGS)

clean:
	rm -f $(TARGET) *.o

.PHONY: all clean
