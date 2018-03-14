TARGET_DEVICE=atmega2560
CPUFREQ=16000000 
CC=/usr/bin/avr-gcc
OBJCOPY=/usr/bin/avr-objcopy
AR=/usr/bin/avr-ar
STRIP=/usr/bin/avr-strip

CFLAGS  = -Wall -Wextra -Werror -Wno-uninitialized -mmcu=$(TARGET_DEVICE) -O2 -DF_CPU=$(CPUFREQ) -mcall-prologues

all: test/test.hex \
		 test/speed.hex \
 	   test/stack.hex

KUMMER = obj/bigint_mul.o \
				 obj/bigint_sqr.o \
				 obj/bigint_red.o \
				 obj/fe1271_mulconst.o \
				 obj/fe1271_hdmrd.o \
				 obj/fe1271_add.o \
				 obj/fe1271_sub.o \
				 obj/fe1271_neg.o \
				 obj/fe1271_freeze.o \
				 obj/keccak.o \
				 obj/fe1271.o \
				 obj/ladder.o \
				 obj/compress.o \
				 obj/scalar.o \
				 obj/quad.o \
				 obj/dh.o \
				 obj/sign.o

TEST = test/print.c \
       test/avr.c \
       test/fail.c

SPEED = test/print.c \
        test/avr.c \
        test/fail.c \
        test/cpucycles.c



test/test: test/test.c obj/kummer.a $(TEST)
	$(CC) $(CFLAGS) $^ -o $@

test/speed: test/speed.c obj/kummer.a $(SPEED)
	$(CC) $(CFLAGS) $^ -o $@

test/stack: test/stack.c obj/kummer.a $(TEST)
	$(CC) $(CFLAGS) $^ -o $@

test/%.hex: test/%
	$(OBJCOPY) -O ihex -R .eeprom $^ $@

obj/kummer.a: $(KUMMER)
	$(AR) -ar cr $@ $^

obj/%.o: %.[cS]
	mkdir -p obj/
	$(CC) $(CFLAGS) -c $^ -o $@

obj/%.o: asm/%.S
	mkdir -p obj/
	$(CC) $(CFLAGS) -c $^ -o $@

.PHONY: clean

clean:
	-rm -r obj/*
	-rm -r test/test
	-rm -r test/speed
	-rm -r test/stack
