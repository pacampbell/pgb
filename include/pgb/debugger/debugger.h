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

int debugger_fetch_instructions_from_address(struct device *device, struct debugger_info *info,
					     size_t n, size_t *num_decoded, uint16_t address);
int debugger_fetch_instructions(struct device *device, struct debugger_info *info,
				size_t n, size_t *num_decoded);
void free_debugger_info(struct debugger_info *info, size_t size);

struct debugger_io_register_info {
	uint16_t address;
	uint8_t value;
	const char *name;
};

int debugger_io_register_read(struct device *device, uint16_t address, uint8_t *value);
int debugger_io_register_address_to_name(uint16_t address, const char **name);
int debugger_io_register_listing(struct device *device,
				 struct debugger_io_register_info **_io_register_listing, size_t *n);
void debugger_io_register_free(struct debugger_io_register_info *io_register_listing);

#endif /* PGB_DEBUGGER_DEBUGGER_H */
