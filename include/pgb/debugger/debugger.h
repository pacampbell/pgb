#ifndef PGB_DEBUGGER_DEBUGGER_H
#define PGB_DEBUGGER_DEBUGGER_H

#include <stdlib.h>
#include <stdint.h>

struct device;

struct debugger_info {
	const char *mnemonic;
	uint8_t raw_data[4];
	uint16_t address;
};

int debugger_fetch_instructions(struct device *device, size_t num_instrs, struct debugger_info **info);

#endif /* PGB_DEBUGGER_DEBUGGER_H */
