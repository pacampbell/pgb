CC        := gcc
CFLAGS    := -std=gnu11 -Wall -Werror -fvisibility=hidden
LIBS      :=
AR        := ar
ARFLAGS   := rcs
TARGET    := pgb
INCLUDES  := -Iinclude
MAN_CC    := asciidoctor

DSTDIR    :=
PREFIX    := usr/local
PKGCONFIG := $(DSTDIR)/$(PREFIX)/lib/pkgconfig
MAN_DIR   := $(DSTDIR)/$(PREFIX)/share/man/man3

STATIC_LIB      := lib$(TARGET).a
DYNAMIC_LIB     := lib$(TARGET).so
TESTER          := lib$(TARGET)-dbg
PKG_CONFIG_FILE := $(TARGET).pc

DEBUG ?= 1

TARBALL_VERSION = master
TRABALL_DIR = .

DOC := \
	doc/pgb.1 \
	doc/pgb-debugger.3

OBJS := \
	src/pgb/libmain.o \
	src/pgb/cpu/clock.o \
	src/pgb/cpu/cpu.o \
	src/pgb/debugger/debugger.o \
	src/pgb/cpu/decoder.o \
	src/pgb/cpu/decoder/logical_decoder.o \
	src/pgb/cpu/decoder/table_decoder.o \
	src/pgb/cpu/interpreter/interpreter.o \
	src/pgb/cpu/registers.o \
	src/pgb/device/device.o \
	src/pgb/gpu/gpu.o \
	src/pgb/io/io.o \
	src/pgb/mmu/mmu.o

TESTER_OBJS := \
	src/pgb/tui/buffer.o \
	src/pgb/tui/break-cmd.o \
	src/pgb/tui/main.o \
	src/pgb/tui/help-cmd.o \
	src/pgb/tui/quit-cmd.o \
	src/pgb/tui/run-cmd.o \
	src/pgb/tui/step-cmd.o

ifeq ($(DEBUG),1)
	CFLAGS += -g -DDEBUG=1 -O0
else
	CFLAGS += -DDEBUG=0 -O1
endif

ifneq (,$(wildcard include/bios/nintendo_bios.h))
	CFLAGS += -DHAS_BIOS_ROM=1
else
	CFLAGS += -DHAS_BIOS_ROM=0
endif

.PHONY: all clean doc force help install uninstall

all: ## Builds manpages, tester utility and dynamic (.so) and static (.a) libraries.
all: doc libs debugger

install: ## Installs project headers, libraries, manpages, debugger, and pkg-config files.
install: doc libs
	mkdir -p $(DSTDIR)/$(PREFIX)/include
	mkdir -p $(DSTDIR)/$(PREFIX)/lib
	mkdir -p $(DSTDIR)/$(PREFIX)/share/man/man1
	mkdir -p $(DSTDIR)/$(PREFIX)/share/man/man3
	mkdir -p $(PKGCONFIG)
	cp -P $(STATIC_LIB) $(DYNAMIC_LIB) $(DSTDIR)/$(PREFIX)/lib
	cp -r include/$(TARGET) $(DSTDIR)/$(PREFIX)/include
	cp -r doc/*.1 $(DSTDIR)/$(PREFIX)/share/man/man1
	cp -r doc/*.3 $(DSTDIR)/$(PREFIX)/share/man/man3
	@echo "Creating $(PKGCONFIG)/$(PKG_CONFIG_FILE)"
	@echo 'prefix=/$(PREFIX)'               > $(PKGCONFIG)/$(PKG_CONFIG_FILE)
	@echo 'exec_prefix=$${prefix}'         >> $(PKGCONFIG)/$(PKG_CONFIG_FILE)
	@echo 'libdir=$${exec_prefix}/lib'     >> $(PKGCONFIG)/$(PKG_CONFIG_FILE)
	@echo 'includedir=$${prefix}/include'  >> $(PKGCONFIG)/$(PKG_CONFIG_FILE)
	@echo ""                               >> $(PKGCONFIG)/$(PKG_CONFIG_FILE)
	@echo "Name: pgb"                      >> $(PKGCONFIG)/$(PKG_CONFIG_FILE)
	@echo "Description: Gameboy emulator written in C." \
					       >> $(PKGCONFIG)/$(PKG_CONFIG_FILE)
	@echo "Version: 0.0.0"                 >> $(PKGCONFIG)/$(PKG_CONFIG_FILE)
	@echo ""                               >> $(PKGCONFIG)/$(PKG_CONFIG_FILE)
	@echo 'Libs: -L$${libdir} -l${TARGET}' >> $(PKGCONFIG)/$(PKG_CONFIG_FILE)
	@echo 'Libs.private:'                  >> $(PKGCONFIG)/$(PKG_CONFIG_FILE)
	@echo 'Cflags: -I$${includedir}'       >> $(PKGCONFIG)/$(PKG_CONFIG_FILE)

uninstall: ## Uninstalls project headers, libraries, manpages, debugger and pkg-config files.
	rm -f $(DSTDIR)/$(PREFIX)/lib/$(STATIC_LIB)
	rm -f $(DSTDIR)/$(PREFIX)/lib/$(DYNAMIC_LIB)
	rm -rf $(DSTDIR)/$(PREFIX)/include/$(TARGET)
	rm -f $(PKGCONFIG)/$(TARGET).pc
	rm -f $(DSTDIR)/$(PREFIX)/share/man/man1/$(TARGET)*.1
	rm -f $(DSTDIR)/$(PREFIX)/share/man/man3/$(TARGET)*.3

doc: ## Builds documentation for library.
doc: $(DOC)

%.1: %.1.adoc
	$(MAN_CC) -b manpage $<

%.3: %.3.adoc
	$(MAN_CC) -b manpage $<

libs: ## Builds dynamic (.so) and static (.a) libraries.
libs: $(STATIC_LIB) $(DYNAMIC_LIB)

$(STATIC_LIB): $(OBJS)
	$(AR) $(ARFLAGS) $@ $(filter-out src/pgb/main.o, $(OBJS))
	@if [ "$(DEBUG)" -eq "0" ]; then \
		strip --strip-unneeded $(STATIC_LIB); \
	fi

$(DYNAMIC_LIB): $(OBJS)
	$(CC) -shared -fpic -Wl,-elib_main -o $@ $(OBJS)
	@if [ "$(DEBUG)" -eq "0" ]; then \
		strip --strip-all $(DYNAMIC_LIB); \
	fi

debugger: ## Builds a console debug utility used to debug GB roms during execution
debugger: $(OBJS) $(TESTER_OBJS)
	$(CC) $(OBJS) $(TESTER_OBJS) -o $(TESTER) -lncurses

%.o: %.c .compiler_flags .banner
	$(CC) $(CFLAGS) $(INCLUDES) -fPIC -c -MMD -MP $< -o $@

.banner: force
	@echo "/* This file is auto-generated by the project Makefile */" > include/pgb/banner.h
	@echo "" >> include/pgb/banner.h
	@echo "#ifndef PGB_BANNER_H" >> include/pgb/banner.h
	@echo "#define PGB_BANNER_H" >> include/pgb/banner.h
	@echo "" >> include/pgb/banner.h
	@echo "#define LIB_NAME \"$(DYNAMIC_LIB)\"" >> include/pgb/banner.h
	@echo "#define LIB_BUILD_FLAGS \"$(CFLAGS)\"" >> include/pgb/banner.h
	@echo "#define LIB_BUILD_DEPS \"$(LIBS)\"" >> include/pgb/banner.h
	@echo "#define LIB_BUILD_DATE \"$(shell date)\"" >> include/pgb/banner.h
	@echo "#define LIB_AUTHOR \"Paul Campbell <pacampbell87@gmail.com>\"" >> include/pgb/banner.h
	@echo "#define GIT_BRANCH_NAME \"$(shell git symbolic-ref --short HEAD)\"" >> include/pgb/banner.h
	@echo "#define GIT_HEAD_HASH \"$(shell git rev-parse HEAD)\"" >> include/pgb/banner.h
	@echo "#define GIT_REPO_LOCATION \"https://github.com/pacampbell/lib-pgb\"" >> include/pgb/banner.h
	@echo "#define CC_VERSION \"$(shell $(CC) --version | head -n 1)\"" >> include/pgb/banner.h
	@echo "" >> include/pgb/banner.h
	@echo -n "#endif /* PGB_BANNER_H */" >> include/pgb/banner.h

.compiler_flags: force
	@echo '$(CFLAGS)' | cmp -s - $@ || echo '$(CFLAGS)' > $@

-include $(OBJS:.o=.d)

help: ## Generates this help menu.
	@grep -hE '^\S+:.*##' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

clean: ## Removed all files generated by this Makefile.
	rm -f $(OBJS) $(TESTER_OBJS) .compiler_flags *.a *.so $(TESTER) $(patsubst %.o, %.d, $(OBJS)) $(patsubst %.o, %.d, $(TESTER_OBJS)) $(DOC)

