#ifndef PGB_MMU_PRIVATE_MMU_H
#define PGB_MMU_PRIVATE_MMU_H

#include <pgb/mmu/private/mmu_appendix.h>
#include <pgb/mmu/private/mmu_constants.h>

#define MMU_REGION_SIZE(REGION) (LR35902_MMU_REGION_##REGION##_END -  LR35902_MMU_REGION_##REGION##_START + 1)

#endif /* PGB_MMU_PRIVATE_MMU_H */
