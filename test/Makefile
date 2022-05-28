CC=$(CROSS_COMPILE)gcc
CFLAGS=-static

cc-name=$(shell $(CC) -v 2>&1 | grep -q "clang version" && echo clang || echo gcc)

ifeq ($(cc-name),clang)
ifneq ($(CROSS_COMPILE),)
CFLAGS+=--target=$(CROSS_COMPILE:%-=%)
endif
endif

ifneq ($(HDRDIR),)
CFLAGS+=-I$(HDRDIR)/include
endif

SOURCES=$(wildcard *.c)
TARGETS=$(SOURCES:.c=)

.PHONY: all tests clean

all: tests

tests: $(TARGETS)

$(TARGETS): %: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)
