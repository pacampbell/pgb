#include <errno.h>
#include <stdint.h>
#include <stdlib.h>

#include <pgb/debug.h>
#include <pgb/mmu/mmu.h>
#include <pgb/utils.h>

/*
+--------------------------------+------------------+
| Section Name                   | Bounds           |
+--------------------------------+------------------+
| 16KB ROM bank 00               | [0x0000, 0x3fff] |
+--------------------------------+------------------+
| 16KB ROM Bank 01~NN            | [0x4000, 0x7fff] |
+--------------------------------+------------------+
| 8KB Video RAM (VRAM)           | [0x8000, 0x9fff] |
+--------------------------------+------------------+
| 4KB Work RAM (WRAM) bank 0     | [0xc000, 0xcfff] |
+--------------------------------+------------------+
| 4KB Work RAM (WRAM) bank 01~NN | [0xd000, 0xdfff] |
+--------------------------------+------------------+
| Work Ram Mirror                | [0xe000, 0xfdff] |
+--------------------------------+------------------+
| Sprite Attribute table         | [0xfe00, 0xfe9f] |
+--------------------------------+------------------+
| I/O Registers                  | [0xff00, 0xff7f] |
+--------------------------------+------------------+
| High Ram (HRAM)                | [0xff80, 0xfffe] |
+--------------------------------+------------------+
| Interrupts Enable Register     | [0xffff, 0xffff] |
+--------------------------------+------------------+
*/
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
