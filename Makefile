CC = gcc
CFLAGS = -Wall -Werror -std=c11
LIBS =
BINARY = pgb

INCLUDE = include

SRC = \
	src/pgb/main.c			\
	src/pgb/cpu/clock.c		\
	src/pgb/cpu/cpu.c		\
	src/pgb/cpu/isa.c		\
	src/pgb/cpu/prefix_cb.c		\
	src/pgb/cpu/registers.c

.PHONY: all debug clean

all: $(BINARY)

debug: CFLAGS += -g -DDEBUG
debug: $(BINARY)

$(BINARY): $(SRC)
	$(CC) $(CFLAGS) -o $@  $(SRC) -I$(INCLUDE) $(LIBS)

clean:
	rm -rf *.o $(BINARY)
