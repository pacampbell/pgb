#ifndef PGB_IO_PRIVATE_IO_H
#define PGB_IO_PRIVATE_IO_H

#include <pgb/io/private/io_regs.h>
#include <pgb/bit_field.h>

#define IO_RD8(device, REG, result) mmu_read8(mmu, LR35902_IO_REGS_##REG, result)
#define IO_WR8(device, REG, value) mmu_write8(mmu, LR35902_IO_REGS_##REG, value)

#define IRF_GET_VALUE(REG, FLD, data) BF_GET_FLD(LR35902_IO_REGS_##REG##_##FLD, data)
#define IRF_SET_VALUE(REG, FLD, value, data) BF_SET_FLD(LR35902_IO_REGS_##REG##_##FLD, value, data)
#define IRF_CLR_VALUE(REG, FLD, data) BF_CLR_FLD(LR35902_IO_REGS_##REG##_##FLD, data)

#endif /* PGB_IO_PRIVATE_IO_H */
