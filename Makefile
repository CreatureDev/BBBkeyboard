.c.o:
	$(CC) $(CFLGS) -c $^
CC=gcc
CLFGS=-g -Wall
PRG=test
OBJS=kbrd_gpio.o crbrd.o

all:$(PRG)

test: test.o $(OBJS)
	$(CC) $(CFLGS) -o $@ $^

kbrd:
	dtc -O dtb -o kbrd-00A0.dtbo -b 0 -@ kbrd.dts

clean:
	/bin/rm -f $(PRG) *.o a.out
