#include <errno.h>
#include <stdint.h>
#include <stdlib.h>

#include <pgb/debug.h>
#include <pgb/mmu/mmu.h>
#include <pgb/utils.h>

int mmu_init(struct mmu *mmu)
{
	int ret;

	mmu->head = NULL;
	mmu->num_regions = 0;

	ret = mmu_add_address_space(mmu, "16KB ROM bank 00", 0x0000, 0x3fff);
	OK_OR_RETURN(ret == 0, -EIO);

	ret = mmu_add_address_space(mmu, "16KB ROM Bank 01~NN", 0x4000, 0x7fff);
	OK_OR_RETURN(ret == 0, -EIO);

	ret = mmu_add_address_space(mmu, "8KB Video RAM (VRAM)", 0x8000, 0x9fff);
	OK_OR_RETURN(ret == 0, -EIO);

	ret = mmu_add_address_space(mmu, "4KB Work RAM (WRAM) bank 0", 0xc000, 0xcfff);
	OK_OR_RETURN(ret == 0, -EIO);

	ret = mmu_add_address_space(mmu, "4KB Work RAM (WRAM) bank 01~NN", 0xd000, 0xdfff);
	OK_OR_RETURN(ret == 0, -EIO);

	ret = mmu_add_address_space(mmu, "Work Ram Mirror", 0xe000, 0xfdff);
	OK_OR_RETURN(ret == 0, -EIO);

	ret = mmu_add_address_space(mmu, "Sprite Attribute table", 0xfe00, 0xfe9f);
	OK_OR_RETURN(ret == 0, -EIO);

	ret = mmu_add_address_space(mmu, "I/O Registers", 0xff00, 0xff7f);
	OK_OR_RETURN(ret == 0, -EIO);

	ret = mmu_add_address_space(mmu, "High Ram (HRAM)", 0xff80, 0xfffe);
	OK_OR_RETURN(ret == 0, -EIO);

	ret = mmu_add_address_space(mmu, "Interrupts Enable Register", 0xffff, 0xffff);
	OK_OR_RETURN(ret == 0, -EIO);

	return 0;
}

static
int free_mmu_region(struct mmu_region *region)
{
	if (region == NULL) {
		return 0;
	}

	free_mmu_region(region->next);
	free(region);

	return 0;
}

int mmu_destroy(struct mmu *mmu)
{
	struct mmu_region *region;

	region = mmu->head;
	free_mmu_region(region);

	return 0;
}

int mmu_read_byte(struct mmu *mmu, uint16_t address, uint8_t *result)
{
	return 0;
}

int mmu_read_word(struct mmu *mmu, uint16_t address, uint16_t *result)
{
	return 0;
}

int mmu_write_byte(struct mmu *mmu, uint16_t address, uint8_t value)
{
	return 0;
}

int mmy_write_word(struct mmu *mmu, uint16_t address, uint16_t value)
{
	return 0;
}

int mmu_add_address_space(struct mmu *mmu, const char *name, size_t start, size_t end)
{
	int ret = 0;
	size_t region_size;
	uint8_t *data;
	struct mmu_region *region;

	region_size = end - start + 1;

	region = malloc(sizeof(*region));
	OK_OR_RETURN(region != NULL, -ENOMEM);

	region->base_addr = start;
	region->size = region_size;
	region->name = name;

	data = malloc(region_size);
	OK_OR_GOTO(data != NULL, free_and_exit);

	region->data = data;

free_and_exit:
	if (data == NULL) {
		ret = -ENOMEM;
		free(region);
	}

	return ret;
}