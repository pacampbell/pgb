#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <pgb/debug.h>
#include <pgb/mmu/mmu.h>
#include <pgb/mmu/private/mmu.h>
#include <pgb/utils.h>

int mmu_init(struct mmu *mmu)
{
	mmu->size = LR35902_MMU_MEMORY_SIZE;
	mmu->ram = calloc(LR35902_MMU_MEMORY_SIZE, sizeof(uint8_t));
	OK_OR_RETURN(mmu->ram != NULL, -ENOMEM);

	return 0;
}

int mmu_destroy(struct mmu *mmu)
{
	mmu->size = 0;
	free(mmu->ram);
	return 0;
}

int mmu_read_byte(struct mmu *mmu, uint16_t address, uint8_t *result)
{
	OK_OR_RETURN(address < LR35902_MMU_MEMORY_SIZE, -EINVAL);

	*result = mmu->ram[address];
	return 0;
}

int mmu_read_word(struct mmu *mmu, uint16_t address, uint16_t *result)
{
	OK_OR_RETURN((address + 1) < LR35902_MMU_MEMORY_SIZE, -EINVAL);

	*result = (mmu->ram[address] << 8) | mmu->ram[address];

	return 0;
}

int mmu_write_byte(struct mmu *mmu, uint16_t address, uint8_t value)
{
	OK_OR_RETURN(address < LR35902_MMU_MEMORY_SIZE, -EINVAL);

	mmu->ram[address] = value;

	return 0;
}

int mmu_write_word(struct mmu *mmu, uint16_t address, uint16_t value)
{
	OK_OR_RETURN((address + 1) < LR35902_MMU_MEMORY_SIZE, -EINVAL);

	mmu->ram[address] = value & 0xff;
	mmu->ram[address + 1] = (value >> 8) & 0xff;

	return 0;
}

int mmu_read_region(struct mmu *mmu, uint16_t base_address, uint8_t *buffer, size_t *size)
{
	OK_OR_RETURN(base_address < LR35902_MMU_MEMORY_SIZE, -EINVAL);

	if ((base_address + *size) >= LR35902_MMU_MEMORY_SIZE) {
		*size = LR35902_MMU_MEMORY_SIZE - base_address;
	}

	memcpy(buffer, mmu->ram + base_address, *size);

	return 0;
}