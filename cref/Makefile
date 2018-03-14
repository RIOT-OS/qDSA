CC=gcc
CFLAGS  = -g -Wall -Wextra -Werror

all: test/test

kummer = obj/fe1271.o \
				 obj/ladder.o \
				 obj/scalar.o \
				 obj/hash.o \
				 obj/sign.o \
				 obj/dh.o \
				 obj/quad.o \
				 obj/compress.o

test/test: test/test.c test/print.c obj/kummer.a
	$(CC) $(CFLAGS) $^ -o $@

test/print: test/print.c obj/kummer.a
	$(CC) $(CFLAGS) $^ -o $@

obj/kummer.a: $(kummer)
	$(AR) -ar cr $@ $^

obj/%.o: %.[cS]
	mkdir -p obj/
	$(CC) $(CFLAGS) -c $^ -o $@

.PHONY: clean

clean:
	-rm -r obj/*
	-rm -r test/test

run:
	test/test
