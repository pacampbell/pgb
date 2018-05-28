CC = gcc
CFLAGS = -Wall -Werror -std=c11
LIBS =
BINARY = pgb

INCLUDE = include

SRC = \
	src/pgb/main.c				\
	src/pgb/cpu/clock.c			\
	src/pgb/cpu/cpu.c			\
	src/pgb/cpu/isa.c			\
	src/pgb/cpu/registers.c		\
	src/pgb/device/device.c		\
	src/pgb/mmu/mmu.c

.PHONY: all debug clean

all: $(BINARY)

debug: CFLAGS += -g -DDEBUG
debug: $(BINARY)

$(BINARY): $(SRC)
	$(CC) $(CFLAGS) -o $@  $(SRC) -I$(INCLUDE) $(LIBS)

clean:
	rm -rf *.o $(BINARY)
