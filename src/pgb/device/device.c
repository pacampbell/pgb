#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <pgb/debug.h>
#include <pgb/utils.h>
#include <pgb/device/device.h>

LIBEXPORT
int device_init(struct device *device, const char *decoder_type)
{
	int ret;

	ret = cpu_init(&device->cpu, decoder_type);
	OK_OR_RETURN(ret == 0, ret);

	ret = mmu_init(&device->mmu);
	OK_OR_RETURN(ret == 0, ret);

	return 0;
}

LIBEXPORT
int device_destroy(struct device *device)
{
	cpu_destroy(&device->cpu);
	mmu_destroy(&device->mmu);

	return 0;
}

LIBEXPORT
int device_load_bios_from_address(struct device *device, uint8_t *data, size_t len)
{
	struct mmu *mmu = &device->mmu;

	OK_OR_RETURN(data != NULL && len > 0, -EINVAL);
	// XXX: Make constant for 256 (ROM image size)
	OK_OR_RETURN(len == 256, -EINVAL);

	memcpy(mmu->ram, data, len);

	return 0;
}

LIBEXPORT
int device_load_bios_from_file(struct device *device, const char *bios_path)
{
	int ret, fd;
	uint8_t *data;
	struct stat bios_st;
	struct mmu *mmu = &device->mmu;

	fd = open(bios_path, O_RDONLY);
	OK_OR_RETURN(fd >= 0, -EINVAL);

	ret = stat(bios_path, &bios_st);
	OK_OR_RETURN(ret == 0, -EIO);

	// XXX: Make constant for 256 (ROM image size)
	OK_OR_RETURN(bios_st.st_size == 256, -EINVAL);

	data = mmap(NULL, bios_st.st_size, PROT_READ, MAP_SHARED, fd, 0);
	OK_OR_RETURN(data != MAP_FAILED, -EIO);

	memcpy(mmu->ram, data, bios_st.st_size);

	munmap(data, bios_st.st_size);

	return 0;
}

LIBEXPORT
int device_load_image_from_file(struct device *device, const char *rom_path)
{
	int ret, fd;
	uint8_t *data;
	struct stat rom_st;
	struct mmu *mmu = &device->mmu;

	fd = open(rom_path, O_RDONLY);
	OK_OR_RETURN(fd >= 0, -EINVAL);

	ret = stat(rom_path, &rom_st);
	OK_OR_RETURN(ret == 0, -EIO);

	data = mmap(NULL, rom_st.st_size, PROT_READ, MAP_SHARED, fd, 0);
	OK_OR_RETURN(data != MAP_FAILED, -EIO);

	// XXX: Handle rom image larger than the size of section 00
	// XXX: Rom images are loaded starting at address 256
	memcpy(mmu->ram + 0x100, data, rom_st.st_size);

	munmap(data, rom_st.st_size);

	return 0;
}

LIBEXPORT
int device_reset_system(struct device *device, const char *decoder_type, const char *boot_rom_path)
{
	int ret;

	ret = device_destroy(device);
	OK_OR_RETURN(ret == 0, ret);

	ret = device_init(device, decoder_type);
	OK_OR_RETURN(ret == 0, ret);

	ret = device_load_image_from_file(device, boot_rom_path);
	OK_OR_WARN(ret == 0);

	return ret;
}

LIBEXPORT
int device_configure_setting(struct device *device, enum device_setting setting, const char *value)
{
	int ret = 0;

	switch (setting) {
	case DEVICE_SETTING_BOOT_ROM_PATH:
		device->settings.boot_rom_path = value;
		break;
	case DEVICE_SETTING_CARTRIDGE_PATH:
		device->settings.loaded_cart_path = value;
		break;
	case DEVICE_SETTING_DECODER_TYPE:
		device->settings.decoder_type = value;
		break;
	default:
		ret = -EINVAL;
		break;
	}
	OK_OR_WARN(ret == 0);

	return ret;
}
