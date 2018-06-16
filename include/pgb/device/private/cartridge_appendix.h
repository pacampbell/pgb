#ifndef PGB_DEVICE_PRIVATE_CARTRIDGE_APPENDIX_H
#define PGB_DEVICE_PRIVATE_CARTRIDGE_APPENDIX_H

#define DMG_CARTRIDGE_HEADER_EXTENT                               0x014f:0x0100
#define DMG_CARTRIDGE_ENTRY_POINT                                 0x0103:0x0100
#define DMG_CARTRIDGE_NINTENDO_LOGO                               0x0133:0x0104
#define DMG_CARTRIDGE_TITLE                                       0x0143:0x0134
#define DMG_CARTRIDGE_MANUFACTURER_CODE                           0x0142:0x013f
#define DMG_CARTRIDGE_CGB_FLAG                                    0x0143:0x0143
#define DMG_CARTRIDGE_CGB_FLAG_SUPPORT_CGB                                 0x80
#define DMG_CARTRIDGE_CGB_FLAG_CGB_ONLY                                    0xc0
#define DMG_CARTRIDGE_NEW_LICENSEE_CODE                           0x0145:0x0144
#define DMG_CARTRIDGE_SGB_FLAG                                    0x0146:0x0146
#define DMG_CARTRIDGE_SGB_FLAG_UNSUPPORTED                                 0x00
#define DMG_CARTRIDGE_SGB_FLAG_SUPPORTED                                   0x03
#define DMG_CARTRIDGE_CARTRIDGE_TYPE                              0x0147:0x0147
#define DMG_CARTRIDGE_CARTRIDGE_TYPE_ROM_ONLY                              0x00
#define DMG_CARTRIDGE_CARTRIDGE_TYPE_MBC1                                  0x01
#define DMG_CARTRIDGE_CARTRIDGE_TYPE_MBC1_RAM                              0x02
#define DMG_CARTRIDGE_CARTRIDGE_TYPE_MBC1_RAM_BATTERY                      0x03
#define DMG_CARTRIDGE_CARTRIDGE_TYPE_MBC2                                  0x05
#define DMG_CARTRIDGE_CARTRIDGE_TYPE_MBC2_BATTERY                          0x06
#define DMG_CARTRIDGE_CARTRIDGE_TYPE_ROM_RAM                               0x08
#define DMG_CARTRIDGE_CARTRIDGE_TYPE_ROM_RAM_BATTERY                       0x09
#define DMG_CARTRIDGE_CARTRIDGE_TYPE_MMM01                                 0x0b
#define DMG_CARTRIDGE_CARTRIDGE_TYPE_MMM01_RAM                             0x0c
#define DMG_CARTRIDGE_CARTRIDGE_TYPE_MMM01_RAM_BATTERY                     0x0d
#define DMG_CARTRIDGE_CARTRIDGE_TYPE_MBC3_TIMER_BATTERY                    0x0f
#define DMG_CARTRIDGE_CARTRIDGE_TYPE_MBC3_TIMER_RAM_BATTERY                0x10
#define DMG_CARTRIDGE_CARTRIDGE_TYPE_MBC3                                  0x11
#define DMG_CARTRIDGE_CARTRIDGE_TYPE_MBC3_RAM                              0x12
#define DMG_CARTRIDGE_CARTRIDGE_TYPE_MBC3_RAM_BATTERY                      0x13
#define DMG_CARTRIDGE_CARTRIDGE_TYPE_MBC4                                  0x15
#define DMG_CARTRIDGE_CARTRIDGE_TYPE_MBC4_RAM                              0x16
#define DMG_CARTRIDGE_CARTRIDGE_TYPE_MBC4_RAM_BATTERY                      0x17
#define DMG_CARTRIDGE_CARTRIDGE_TYPE_MBC5                                  0x19
#define DMG_CARTRIDGE_CARTRIDGE_TYPE_MBC5_RAM                              0x1a
#define DMG_CARTRIDGE_CARTRIDGE_TYPE_MBC5_RAM_BATTERY                      0x1b
#define DMG_CARTRIDGE_CARTRIDGE_TYPE_MBC5_RUMBLE                           0x1c
#define DMG_CARTRIDGE_CARTRIDGE_TYPE_MBC5_RUMBLE_RAM                       0x1d
#define DMG_CARTRIDGE_CARTRIDGE_TYPE_MBC5_RUMBLE_RAM_BATTERY               0x1e
#define DMG_CARTRIDGE_CARTRIDGE_TYPE_POCKET_CAMERA                         0xfc
#define DMG_CARTRIDGE_CARTRIDGE_TYPE_BANDAI_TAMA5                          0xfd
#define DMG_CARTRIDGE_CARTRIDGE_TYPE_HUC3                                  0xfe
#define DMG_CARTRIDGE_CARTRIDGE_TYPE_HUC1_RAM_BATTERY                      0xff
#define DMG_CARTRIDGE_ROM_SIZE                                    0x0148:0x0148
#define DMG_CARTRIDGE_ROM_SIZE_32KB                                        0x00
#define DMG_CARTRIDGE_ROM_SIZE_64KB                                        0x01
#define DMG_CARTRIDGE_ROM_SIZE_128KB                                       0x02
#define DMG_CARTRIDGE_ROM_SIZE_256KB                                       0x03
#define DMG_CARTRIDGE_ROM_SIZE_512KB                                       0x04
#define DMG_CARTRIDGE_ROM_SIZE_1MB                                         0x05
#define DMG_CARTRIDGE_ROM_SIZE_2MB                                         0x06
#define DMG_CARTRIDGE_ROM_SIZE_4MB                                         0x07
#define DMG_CARTRIDGE_ROM_SIZE_1_1MB                                       0x52
#define DMG_CARTRIDGE_ROM_SIZE_1_2MB                                       0x53
#define DMG_CARTRIDGE_ROM_SIZE_1_5MB                                       0x54
#define DMG_CARTRIDGE_RAM_SIZE                                    0x0149:0x0149
#define DMG_CARTRIDGE_RAM_SIZE_NONE                                        0x00
#define DMG_CARTRIDGE_RAM_SIZE_2KB                                         0x01
#define DMG_CARTRIDGE_RAM_SIZE_8KB                                         0x02
#define DMG_CARTRIDGE_RAM_SIZE_32KB                                        0x03
#define DMG_CARTRIDGE_DESTINATION_CODE                            0x014a:0x014a
#define DMG_CARTRIDGE_DESTINATION_CODE_JAPANESE                            0x00
#define DMG_CARTRIDGE_DESTINATION_CODE_NON_JAPANESE                        0x01
#define DMG_CARTRIDGE_OLD_LICENSEE_CODE                           0x014b:0x014b
#define DMG_CARTRIDGE_MASK_ROM_VERSION_NUMBER                     0x014c:0x014c
#define DMG_CARTRIDGE_HEADER_CHECKSUM                             0x014d:0x014d
#define DMG_CARTRIDGE_GLOBAL_CHECKSUM                             0x014f:0x014e

#endif /* PGB_DEVICE_PRIVATE_CARTRIDGE_APPENDIX_H */