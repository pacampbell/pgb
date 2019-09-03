#include <pgb/debug.h>
#include <pgb/device/device.h>
#include <pgb/mmu/mmu.h>
#include <pgb/io/io.h>
#include <pgb/io/private/io.h>

static
int address_to_name(uint16_t address, char **name)
{
	*name = "???";

	return 0;
}

int ioreg_rd8(struct device *device, uint16_t address, uint8_t *result)
{
	int ret;
	char *name;

	ret = mmu_read8(&device->mmu, address, result);
	OK_OR_RETURN(ret == 0, ret);

	if (IS_DEBUG()) {
		ret = address_to_name(address, &name);
		OK_OR_RETURN(ret == 0, ret);

		fprintf(stderr, "%s(0x%04x) => 0x%02x\n", name, address, *result);
	}

	return ret;
}

int ioreg_wr8(struct device *device, uint16_t address, uint8_t value)
{
	int ret;
	char *name;

	ret = mmu_write8(&device->mmu, address, value);
	OK_OR_RETURN(ret == 0, ret);

	if (IS_DEBUG()) {
		ret = address_to_name(address, &name);
		OK_OR_RETURN(ret == 0, ret);

		fprintf(stderr, "%s(0x%04x) <= 0x%02x\n", name, address, value);
	}

	return ret;
}
