#ifndef PGB_DEVICE_PRIVATE_REGISTERS_LCD_REGS_H
#define PGB_DEVICE_PRIVATE_REGISTERS_LCD_REGS_H

#define DMG_LCD_CONTROL_REGISTER                                  0xff40:0xff40
#define DMG_LCD_CONTROL_REGISTER_LCD_DISPLAY_ENABLE                         7:7
#define DMG_LCD_CONTROL_REGISTER_LCD_DISPLAY_ENABLE_OFF                       0
#define DMG_LCD_CONTROL_REGISTER_LCD_DISPLAY_ENABLE_ON                        1
#define DMG_LCD_CONTROL_REGISTER_WINDOW_TILE_MAP_DISPLAY_SELECT             6:6
#define DMG_LCD_CONTROL_REGISTER_WINDOW_TILE_MAP_DISPLAY_SELECT_9800_9BFF     0
#define DMG_LCD_CONTROL_REGISTER_WINDOW_TILE_MAP_DISPLAY_SELECT_9C00_9FFF     1
#define DMG_LCD_CONTROL_REGISTER_WINDOW_DISPLAY_ENABLE                      5:5
#define DMG_LCD_CONTROL_REGISTER_WINDOW_DISPLAY_ENABLE_OFF                    0
#define DMG_LCD_CONTROL_REGISTER_WINDOW_DISPLAY_ENABLE_ON                     1
#define DMG_LCD_CONTROL_REGISTER_BG_WINDOW_TILE_DATA_SELECT                 4:4
#define DMG_LCD_CONTROL_REGISTER_BG_WINDOW_TILE_DATA_SELECT_8800_97FF         0
#define DMG_LCD_CONTROL_REGISTER_BG_WINDOW_TILE_DATA_SELECT_8000_8FFF         1
#define DMG_LCD_CONTROL_REGISTER_BG_TILE_MAP_DISPLAY_SELECT                 3:3
#define DMG_LCD_CONTROL_REGISTER_BG_TILE_MAP_DISPLAY_SELECT_9800_9BFF         0
#define DMG_LCD_CONTROL_REGISTER_BG_TILE_MAP_DISPLAY_SELECT_9C00_9FFF         1
#define DMG_LCD_CONTROL_REGISTER_SPRITE_SIZE                                2:2
#define DMG_LCD_CONTROL_REGISTER_SPRITE_SIZE_8_X_8                            0
#define DMG_LCD_CONTROL_REGISTER_SPRITE_SIZE_8_X_16                           1
#define DMG_LCD_CONTROL_REGISTER_SPRITE_DISPLAY_ENABLE                      1:1
#define DMG_LCD_CONTROL_REGISTER_SPRITE_DISPLAY_ENABLE_OFF                    0
#define DMG_LCD_CONTROL_REGISTER_SPRITE_DISPLAY_ENABLE_ON                     1
#define DMG_LCD_CONTROL_REGISTER_BG_DISPLAY_ENABLE                          0:0
#define DMG_LCD_CONTROL_REGISTER_BG_DISPLAY_ENABLE_OFF                        0
#define DMG_LCD_CONTROL_REGISTER_BG_DISPLAY_ENABLE_ON                         1
#define DMG_LCD_STATUS_REGISTER                                   0xff41:0xff41
#define DMG_LCD_STATUS_REGISTER_LYC_EQ_LY_COINCIDENCE_INTERRUPT_ENABLE      6:6 /* RW */
#define DMG_LCD_STATUS_REGISTER_LYC_EQ_LY_COINCIDENCE_INTERRUPT_ENABLE_OFF    0
#define DMG_LCD_STATUS_REGISTER_LYC_EQ_LY_COINCIDENCE_INTERRUPT_ENABLE_ON     1
#define DMG_LCD_STATUS_REGISTER_MODE_2_OAM_INTERRUPT_ENABLE                 5:5 /* RW */
#define DMG_LCD_STATUS_REGISTER_MODE_2_OAM_INTERRUPT_ENABLE_OFF               0
#define DMG_LCD_STATUS_REGISTER_MODE_2_OAM_INTERRUPT_ENABLE_ON                1
#define DMG_LCD_STATUS_REGISTER_MODE_1_V_BLANK_INTERRUPT_ENABLE             4:4 /* RW */
#define DMG_LCD_STATUS_REGISTER_MODE_1_V_BLANK_INTERRUPT_ENABLE_OFF           0
#define DMG_LCD_STATUS_REGISTER_MODE_1_V_BLANK_INTERRUPT_ENABLE_ON            1
#define DMG_LCD_STATUS_REGISTER_MODE_0_H_BLANK_INTERRUPT_ENABLE             3:3 /* RW */
#define DMG_LCD_STATUS_REGISTER_MODE_0_H_BLANK_INTERRUPT_ENABLE_OFF           0
#define DMG_LCD_STATUS_REGISTER_MODE_0_H_BLANK_INTERRUPT_ENABLE_ON            1
#define DMG_LCD_STATUS_REGISTER_COINCIDENCE_FLAG                            2:2 /* RO */
#define DMG_LCD_STATUS_REGISTER_COINCIDENCE_FLAG_LYC_NE_LY                    0
#define DMG_LCD_STATUS_REGISTER_COINCIDENCE_FLAG_LYC_EQ_LY                    1
#define DMG_LCD_STATUS_REGISTER_MODE_FLAG                                   1:0 /* RO */
#define DMG_LCD_STATUS_REGISTER_MODE_FLAG_H_BLANK                             0
#define DMG_LCD_STATUS_REGISTER_MODE_FLAG_V_BLANK                             1
#define DMG_LCD_STATUS_REGISTER_MODE_FLAG_SEARCHING_OAM_RAM                   2
#define DMG_LCD_STATUS_REGISTER_MODE_FLAG_TRANSFERRING_DATA_TO_LCR_DRIVER     3
#define DMG_LCD_SCROLL_Y                                          0xff42:0xff42 /* RW */
#define DMG_LCD_SCROLL_X                                          0xff43:0xff43 /* RW */
#define DMG_LCD_LCDC_Y_COORDINATE                                 0xff44:0xff44 /* RO */
#define DMG_LCD_LY_COMPARE                                        0xff45:0xff45 /* RW */
#define DMG_LCD_WINDOW_Y_POSITION                                 0xff4a:0xff4a /* RW */
#define DMG_LCD_WINDOW_X_POSITION                                 0xff4b:0xff4b /* RW */
#define DMG_LCD_BG_PALETTE_DATA                                   0xff47:0xff47 /* RW */
#define DMG_LCD_BG_PALETTE_DATA_COLOR_3                                     7:6
#define DMG_LCD_BG_PALETTE_DATA_COLOR_3_WHITE                                 0
#define DMG_LCD_BG_PALETTE_DATA_COLOR_3_LIGHT_GRAY                            1
#define DMG_LCD_BG_PALETTE_DATA_COLOR_3_DARK_GRAY                             2
#define DMG_LCD_BG_PALETTE_DATA_COLOR_3_BLACK                                 3
#define DMG_LCD_BG_PALETTE_DATA_COLOR_2                                     5:4
#define DMG_LCD_BG_PALETTE_DATA_COLOR_2_WHITE                                 0
#define DMG_LCD_BG_PALETTE_DATA_COLOR_2_LIGHT_GRAY                            1
#define DMG_LCD_BG_PALETTE_DATA_COLOR_2_DARK_GRAY                             2
#define DMG_LCD_BG_PALETTE_DATA_COLOR_2_BLACK                                 3
#define DMG_LCD_BG_PALETTE_DATA_COLOR_1                                     3:2
#define DMG_LCD_BG_PALETTE_DATA_COLOR_1_WHITE                                 0
#define DMG_LCD_BG_PALETTE_DATA_COLOR_1_LIGHT_GRAY                            1
#define DMG_LCD_BG_PALETTE_DATA_COLOR_1_DARK_GRAY                             2
#define DMG_LCD_BG_PALETTE_DATA_COLOR_1_BLACK                                 3
#define DMG_LCD_BG_PALETTE_DATA_COLOR_0                                     1:0
#define DMG_LCD_BG_PALETTE_DATA_COLOR_0_WHITE                                 0
#define DMG_LCD_BG_PALETTE_DATA_COLOR_0_LIGHT_GRAY                            1
#define DMG_LCD_BG_PALETTE_DATA_COLOR_0_DARK_GRAY                             2
#define DMG_LCD_BG_PALETTE_DATA_COLOR_0_BLACK                                 3
#define DMG_LCD_DMA_TRANSFER_AND_START                            0xff46:0xff46 /* WO */

#endif /* PGB_DEVICE_PRIVATE_REGISTERS_LCD_REGS_H */
