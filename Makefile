PREFIX=/usr/local
FUNCPREFIX=fuzz
CFLAGS=-O3 -g -std=c11 -pthread -Wall -Wextra -pedantic -Wno-unused-parameter -DFUNCPREFIX=$(FUNCPREFIX)
AR=ar

OBJS=bt.o global.o mem.o

all: libnullfuzzer.a

libnullfuzzer.a: $(OBJS)
	$(AR) rcs $@ $^

clean:
	@rm -fv $(OBJS) libnullfuzzer.a

install: all
	install libnullfuzzer.a $(PREFIX)/lib

uninstall:
	@rm -fv $(PREFIX)/lib/libnullfuzzer.a
