#ifndef PGB_BIOS_H
#define PGB_BIOS_H

#include <stdint.h>
#include <stdlib.h>

#if HAS_BIOS_ROM == 1
#include <bios/nintendo_bios.h>
const size_t internal_bios_rom_length = nintendo_bios_rom_length;
uint8_t *internal_bios_rom = (uint8_t*) &nintendo_bios_rom;
#else
const size_t internal_bios_rom_length = 0;
uint8_t *internal_bios_rom = NULL;
#endif

#endif /* PGB_BIOS_H */
