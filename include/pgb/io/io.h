#ifndef PGB_IO_IO_H
#define PGB_IO_IO_H

#include <stddef.h>
#include <stdint.h>

struct device device;

int ioreg_rd8(struct device *device, uint16_t address, uint8_t *result);
int ioreg_wr8(struct device *device, uint16_t address, uint8_t value);

#define IOREG_RD8(device, REG, result) ioreg_rd8(device, LR35902_IO_REGS_##REG, result)
#define IOREG_WR8(device, REG, value) ioreg_wr8(device, LR35902_IO_REGS_##REG, value)

#endif /* PGB_IO_IO_H */
