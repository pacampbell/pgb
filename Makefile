CC := gcc
CFLAGS := -Wall -Werror -std=c11
LIBS := `pkg-config --libs gtk+-3.0`
AR := ar
ARFLAGS := rcs
TARGET := pgb

INCLUDES := -Iinclude `pkg-config --cflags gtk+-3.0`

OBJS := \
	src/pgb/main.o				\
	src/pgb/cpu/clock.o			\
	src/pgb/cpu/cpu.o			\
	src/pgb/cpu/decoder.o			\
	src/pgb/cpu/decoder/logical_decoder.o	\
	src/pgb/cpu/decoder/table_decoder.o	\
	src/pgb/cpu/isa.o			\
	src/pgb/cpu/interpreter/interpreter.o	\
	src/pgb/cpu/registers.o			\
	src/pgb/device/device.o			\
	src/pgb/gui/gui.o			\
	src/pgb/mmu/mmu.o

.PHONY: all debug clean force

all: $(TARGET)

debug: CFLAGS += -g -DDEBUG
debug: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@  $^ $(LIBS)

libpgb.a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $(filter-out src/pgb/main.o, $(OBJS))

%.o: %.c .compiler_flags
	$(CC) $(CFLAGS) $(INCLUDES) -c -MMD -MP $< -o $@

.compiler_flags: force
	@echo '$(CFLAGS)' | cmp -s - $@ || echo '$(CFLAGS)' > $@

-include $(OBJS:.o=.d)

clean:
	rm -f $(OBJS) $(patsubst %.o, %.d, $(OBJS)) $(TARGET) .compiler_flags *.a *.so
