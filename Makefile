CC        := gcc
CFLAGS    := -std=c11 -Wall -Werror -fPIC -fvisibility=hidden
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
PKG_CONFIG_FILE := $(TARGET).pc

DEBUG ?= 1

TARBALL_VERSION = master
TRABALL_DIR = ..

DOC       :=					\
	doc/pgb.1				\
	doc/pgb-debugger.3

OBJS      := \
	src/pgb/main.o				\
	src/pgb/cpu/clock.o			\
	src/pgb/cpu/cpu.o			\
	src/pgb/debugger/debugger.o		\
	src/pgb/cpu/decoder.o			\
	src/pgb/cpu/decoder/logical_decoder.o	\
	src/pgb/cpu/decoder/table_decoder.o	\
	src/pgb/cpu/interpreter/interpreter.o	\
	src/pgb/cpu/registers.o			\
	src/pgb/device/device.o			\
	src/pgb/io/io.o				\
	src/pgb/mmu/mmu.o

ifeq ($(DEBUG),1)
	CFLAGS += -g -DDEBUG=1 -O0
else
	CFLAGS += -DDEBUG=0 -O1
endif

.PHONY: all clean doc force help install uninstall

all: ## Builds manpages and dynamic (.so) and static (.a) libraries.
all: doc libs

install: ## Installs project headers, libraries, manpages, and pkg-config files.
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

uninstall: ## Uninstalls project headers, libraries, manpages, and pkg-config files.
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
	$(CC) -shared -fPIE -fpic -Wl,-E -o $@ $(OBJS)
	@if [ "$(DEBUG)" -eq "0" ]; then \
		strip --strip-all $(DYNAMIC_LIB); \
	fi

%.o: %.c .compiler_flags
	$(CC) $(CFLAGS) $(INCLUDES) -fPIC -c -MMD -MP $< -o $@

.compiler_flags: force
	@echo '$(CFLAGS)' | cmp -s - $@ || echo '$(CFLAGS)' > $@

-include $(OBJS:.o=.d)

help: ## Generates this help menu.
	@grep -hE '^\S+:.*##' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

clean: ## Removed all files generated by this Makefile.
	rm -f $(OBJS) .compiler_flags *.a *.so $(patsubst %.o, %.d, $(OBJS)) $(DOC)


