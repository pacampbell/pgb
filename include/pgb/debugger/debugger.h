#ifndef PGB_DEBUGGER_DEBUGGER_H
#define PGB_DEBUGGER_DEBUGGER_H

#include <stdlib.h>
#include <stdint.h>

struct device;

struct debugger_info {
	const char *assembly;
	size_t num_bytes;
	uint8_t raw_data[4];
	uint16_t address;
	struct {
		char *a;
		char *b;
	} comments;
};

int debugger_fetch_instructions(struct device *device, struct debugger_info *info, size_t size);
void free_debugger_info(struct debugger_info *info, size_t size);

#endif /* PGB_DEBUGGER_DEBUGGER_H */
