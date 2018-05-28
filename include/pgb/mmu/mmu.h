#ifndef PGB_MMU_H
#define PGB_MMU_H

#include <stddef.h>
#include <stdint.h>

struct mmu_region {
	size_t base_addr;
	size_t size;
	uint8_t *data;
	const char *name;
	struct mmu_region *next;
};

struct mmu {
	size_t num_regions;
	struct mmu_region *head;
};

int mmu_init(struct mmu *mmu);
int mmu_destroy(struct mmu *mmu);

int mmu_read_byte(struct mmu *mmu, uint16_t address, uint8_t *result);
int mmu_read_word(struct mmu *mmu, uint16_t address, uint16_t *result);

int mmu_write_byte(struct mmu *mmu, uint16_t address, uint8_t value);
int mmy_write_word(struct mmu *mmu, uint16_t address, uint16_t value);

int mmu_add_address_space(struct mmu *mmu, const char *name, size_t start, size_t end);

#endif /* PGB_MMU_H */
