#ifndef PGB_MMU_PRIVATE_MMU_APPENDIX_H
#define PGB_MMU_PRIVATE_MMU_APPENDIX_H

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
| Sprite Attribute Table         | [0xfe00, 0xfe9f] |
+--------------------------------+------------------+
| I/O Registers                  | [0xff00, 0xff7f] |
+--------------------------------+------------------+
| High Ram (HRAM)                | [0xff80, 0xfffe] |
+--------------------------------+------------------+
| Interrupts Enable Register     | [0xffff, 0xffff] |
+--------------------------------+------------------+
*/

#define LR35902_MMU_REGION_ROM_BANK_00_START                             0x0000
#define LR35902_MMU_REGION_ROM_BANK_00_END                               0x3fff
#define LR35902_MMU_REGION_ROM_BANK_01_NN_START                          0x4000
#define LR35902_MMU_REGION_ROM_BANK_01_NN_END                            0x7fff
#define LR35902_MMU_REGION_VIDEO_RAM_START                               0x8000
#define LR35902_MMU_REGION_VIDEO_RAM_END                                 0x9fff
#define LR35902_MMU_REGION_WORK_RAM_BANK_00_START                        0xc000
#define LR35902_MMU_REGION_WORK_RAM_BANK_00_END                          0xcfff
#define LR35902_MMU_REGION_WORK_RAM_BANK_01_NN_START                     0xd000
#define LR35902_MMU_REGION_WORK_RAM_BANK_01_NN_END                       0xdfff
#define LR35902_MMU_REGION_WORK_RAM_MIRROR_START                         0xe000
#define LR35902_MMU_REGION_WORK_RAM_MIRROR_END                           0xfdff
#define LR35902_MMU_REGION_SPRITE_ATTRIBUTE_TABLE_START                  0xfe00
#define LR35902_MMU_REGION_SPRITE_ATTRIBUTE_TABLE_END                    0xfe9f
#define LR35902_MMU_REGION_IO_REGISTERS_START                            0xff00
#define LR35902_MMU_REGION_IO_REGISTERS_END                              0xff7f
#define LR35902_MMU_REGION_HIGH_RAM_START                                0xff80
#define LR35902_MMU_REGION_HIGH_RAM_END                                  0xfffe
#define LR35902_MMU_REGION_INTERRUPT_ENABLE_REGISTER_START               0xffff
#define LR35902_MMU_REGION_INTERRUPT_ENABLE_REGISTER_END                 0xffff

#endif /* PGB_MMU_PRIVATE_MMU_APPENDIX_H */
