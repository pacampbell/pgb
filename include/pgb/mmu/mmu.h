#ifndef PGB_MMU_H
#define PGB_MMU_H

#include <stddef.h>
#include <stdint.h>

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

int mmu_read_region(struct mmu *mmu, uint16_t base_address, uint8_t *buffer, size_t *size);

#endif /* PGB_MMU_H */
