#ifndef PGB_DEVICE_DEVICE_H
#define PGB_DEVICE_DEVICE_H

#include <pgb/cpu/cpu.h>
#include <pgb/gpu/gpu.h>
#include <pgb/mmu/mmu.h>

struct device {
	struct cpu cpu;
	struct mmu mmu;
};

int device_init(struct device *device, const char *decoder_type);
int device_destroy(struct device *device);
int device_emulate(struct device *device, const char *rom_path);
int device_load_image_from_file(struct device *device, const char *rom_path);

#endif /* PGB_DEVICE_DEVICE_H */
