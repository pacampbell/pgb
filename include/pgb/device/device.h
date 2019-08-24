#ifndef PGB_DEVICE_DEVICE_H
#define PGB_DEVICE_DEVICE_H

#include <pgb/cpu/cpu.h>
#include <pgb/gpu/gpu.h>
#include <pgb/mmu/mmu.h>

enum device_setting {
	DEVICE_SETTING_BOOT_ROM_PATH,
	DEVICE_SETTING_CARTRIDGE_PATH,
	DEVICE_SETTING_DECODER_TYPE
};

struct device {
	struct cpu cpu;
	struct mmu mmu;
	struct {
		const char *boot_rom_path;
		const char *loaded_cart_path;
		const char *decoder_type;
	} settings;
};

int device_init(struct device *device, const char *decoder_type);
int device_destroy(struct device *device);
int device_load_image_from_file(struct device *device, const char *rom_path);
int device_reset_system(struct device *device, const char *decoder_type, const char *boot_rom_path);
int device_configure_setting(struct device *device, enum device_setting setting, const char *value);

#endif /* PGB_DEVICE_DEVICE_H */
