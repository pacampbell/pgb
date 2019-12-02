#include <pgb/debug.h>
#include <pgb/device/device.h>
#include <pgb/mmu/mmu.h>
#include <pgb/io/io.h>
#include <pgb/io/private/io.h>

int ioreg_rd8(struct mmu *mmu, uint16_t address, uint8_t *result)
{
	int ret;

	ret = mmu_read8(mmu, address, result);
	OK_OR_WARN(ret == 0);

	return ret;
}

int ioreg_wr8(struct mmu *mmu, uint16_t address, uint8_t value)
{
	int ret;

	ret = mmu_write8(mmu, address, value);
	OK_OR_WARN(ret == 0);

	return ret;
}

int ioreg_process_interrupt_request_register(struct device *device)
{
#if 0
	int ret;
	uint8_t result;

	ret = IO_RD8(device, IF, &result);
	OK_OR_RETURN(ret == 0, ret);

	if (BF_GET_FLD(IF, VBLANK)) {
	}
#endif
	return 0;
}
