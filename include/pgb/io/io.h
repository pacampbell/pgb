#ifndef PGB_IO_IO_H
#define PGB_IO_IO_H

#include <stddef.h>
#include <stdint.h>

struct mmu;

int ioreg_rd8(struct mmu *mmu, uint16_t address, uint8_t *result);
int ioreg_wr8(struct mmu *mmu, uint16_t address, uint8_t value);

#define IOREG_RD8(mmu, REG, result) ioreg_rd8(mmu, LR35902_IO_REGS_##REG, result)
#define IOREG_WR8(mmu, REG, value) ioreg_wr8(mmu, LR35902_IO_REGS_##REG, value)

#endif /* PGB_IO_IO_H */
