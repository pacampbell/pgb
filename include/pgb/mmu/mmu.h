#ifndef PGB_MMU_H
#define PGB_MMU_H

#include <stddef.h>
#include <stdint.h>

#define LR35902_MMU_MEMORY_SIZE                                           65536
#define LR35902_MMU_NUM_REGIONS                                              10

struct mmu {
	uint8_t *ram;
	size_t size;
};

int mmu_init(struct mmu *mmu);
int mmu_destroy(struct mmu *mmu);

int mmu_read_byte(struct mmu *mmu, uint16_t address, uint8_t *result);
int mmu_read_word(struct mmu *mmu, uint16_t address, uint16_t *result);

int mmu_write_byte(struct mmu *mmu, uint16_t address, uint8_t value);
int mmu_write_word(struct mmu *mmu, uint16_t address, uint16_t value);

#endif /* PGB_MMU_H */
