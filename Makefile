CC := gcc
CFLAGS := -Wall -Werror -std=c11
LIBS :=
TARGET := pgb

INCLUDE := include

OBJS := \
	src/pgb/main.o				\
	src/pgb/cpu/clock.o			\
	src/pgb/cpu/cpu.o			\
	src/pgb/cpu/decoder.o		\
	src/pgb/cpu/isa.o			\
	src/pgb/cpu/registers.o		\
	src/pgb/device/device.o		\
	src/pgb/mmu/mmu.o

.PHONY: all debug clean force

all: $(TARGET)

debug: CFLAGS += -g -DDEBUG
debug: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@  $^ $(LIBS)

%.o: %.c .compiler_flags
	$(CC) $(CFLAGS) -I$(INCLUDE) -c -MMD -MP $< -o $@

.compiler_flags: force
	@echo '$(CFLAGS)' | cmp -s - $@ || echo '$(CFLAGS)' > $@

-include $(OBJS:.o=.d)

clean:
	rm -f $(OBJS) $(patsubst %.o, %.d, $(OBJS)) $(TARGET) .compiler_flags
