#ifndef PGB_IO_PRIVATE_IO_REGS_H
#define PGB_IO_PRIVATE_IO_REGS_H

#define LR35902_IO_REGS_P1                                              0xff00 /* RW */
#define LR35902_IO_REGS_P1_P10_IN_PORT                                  0:0
#define LR35902_IO_REGS_P1_P11_IN_PORT                                  1:1
#define LR35902_IO_REGS_P1_P12_IN_PORT                                  2:2
#define LR35902_IO_REGS_P1_P13_IN_PORT                                  3:3
#define LR35902_IO_REGS_P1_P14_OUT_PORT                                 4:4
#define LR35902_IO_REGS_P1_P15_OUT_PORT                                 5:5
#define LR35902_IO_REGS_SB                                              0xff01 /* RW */
#define LR35902_IO_REGS_SB_DATA                                         7:0
#define LR35902_IO_REGS_SC                                              0xff02 /* RW */
#define LR35902_IO_REGS_SC_SHIFT_CLOCK                                  0:0
#define LR35902_IO_REGS_SC_SHIFT_CLOCK_EXTERNAL_CLOCK                   0 /* 500KHz */
#define LR35902_IO_REGS_SC_SHIFT_CLOCK_INTERNAL_CLOCK                   1 /* 8192 Hz */
#define LR35902_IO_REGS_SC_TRANSFER_START_FLAG                          7:7
#define LR35902_IO_REGS_SC_TRANSFER_START_FLAG_NONE                     0
#define LR35902_IO_REGS_SC_TRANSFER_START_FLAG_START                    1
#define LR35902_IO_REGS_DIV                                             0xff04 /* RW/COW */
#define LR35902_IO_REGS_TIMA                                            0xff05 /* RW */
#define LR35902_IO_REGS_TMA                                             0xff06 /* RW */
#define LR35902_IO_REGS_TAC                                             0xff07 /* RW */
#define LR35902_IO_REGS_TAC_CLOCK_SELECT                                1:0
#define LR35902_IO_REGS_TAC_CLOCK_SELECT_4KHZ                           0
#define LR35902_IO_REGS_TAC_CLOCK_SELECT_262KHZ                         1
#define LR35902_IO_REGS_TAC_CLOCK_SELECT_65KHZ                          2
#define LR35902_IO_REGS_TAC_CLOCK_SELECT_16KHZ                          3
#define LR35902_IO_REGS_TAC_TIMER_CONTROL                               2:2
#define LR35902_IO_REGS_TAC_TIMER_CONTROL_STOP                          0
#define LR35902_IO_REGS_TAC_TIMER_CONTROL_START                         1
#define LR35902_IO_REGS_IF                                              0xff0f /* RW */
#define LR35902_IO_REGS_IF_VBLANK                                       0:0
#define LR35902_IO_REGS_IF_LCD_STAT                                     1:1
#define LR35902_IO_REGS_IF_TIMER                                        2:2
#define LR35902_IO_REGS_IF_SERIAL                                       3:3
#define LR35902_IO_REGS_IF_JOYPAD                                       4:4
#define LR35902_IO_REGS_NR10                                            0xff10 /* RW */
#define LR35902_IO_REGS_NR10_SWEEP_SHIFT                                2:0
#define LR35902_IO_REGS_NR10_SWEEP_SHIFT_OFF                            0
#define LR35902_IO_REGS_NR10_SWEEP_SHIFT_1_128HZ                        1
#define LR35902_IO_REGS_NR10_SWEEP_SHIFT_2_128HZ                        2
#define LR35902_IO_REGS_NR10_SWEEP_SHIFT_3_128HZ                        3
#define LR35902_IO_REGS_NR10_SWEEP_SHIFT_4_128HZ                        4
#define LR35902_IO_REGS_NR10_SWEEP_SHIFT_5_128HZ                        5
#define LR35902_IO_REGS_NR10_SWEEP_SHIFT_6_128HZ                        6
#define LR35902_IO_REGS_NR10_SWEEP_SHIFT_7_128HZ                        7
#define LR35902_IO_REGS_NR10_SWEEP_MODIFIER                             3:3
#define LR35902_IO_REGS_NR10_SWEEP_MODIFIER_ADDITION                    0
#define LR35902_IO_REGS_NR10_SWEEP_MODIFIER_SUBTRACTION                 1
#define LR35902_IO_REGS_NR10_SWEEP_TIME                                 6:4
#define LR35902_IO_REGS_NR11                                            0xff12 /* RW */
#define LR35902_IO_REGS_NR11_SOUND_LENGTH_DATA                          5:0
#define LR35902_IO_REGS_NR11_WAVE_DUTY_PATTERN                          7:6
#define LR35902_IO_REGS_NR11_12_5_PERCENT                               0
#define LR35902_IO_REGS_NR11_25_PERCENT                                 1
#define LR35902_IO_REGS_NR11_50_PERCENT                                 2
#define LR35902_IO_REGS_NR11_75_PERCENT                                 3
#define LR35902_IO_REGS_NR12                                            0xff13 /* RW */
#define LR35902_IO_REGS_NR12_NUM_ENVELOPE_SWEEP                         2:0
#define LR35902_IO_REGS_NR12_ENVELOPE_DIRECTION                         3:3
#define LR35902_IO_REGS_NR12_ENVELOPE_DIRECTION_ATTENUATE               0
#define LR35902_IO_REGS_NR12_ENVELOPE_DIRECTION_AMPLIFY                 1
#define LR35902_IO_REGS_NR12_INITIAL_VOLUME_ENVELOPE                    7:4
#define LR35902_IO_REGS_NR13                                            0xff14 /* W */
#define LR35902_IO_REGS_NR13_FREQUENCY_LSW                              7:0
#define LR35902_IO_REGS_NR14                                            0xff15 /* RW */
#define LR35902_IO_REGS_NR14_FREQUENCY_MSW                              2:0
#define LR35902_IO_REGS_NR14_COUNTER                                    6:6
#define LR35902_IO_REGS_NR14_COUNTER_CONSECUTIVE                        0
#define LR35902_IO_REGS_NR14_COUNTER_PERIOD                             1
#define LR35902_IO_REGS_NR14_START                                      7:7
#define LR35902_IO_REGS_NR21                                            0xff16 /* RW */
#define LR35902_IO_REGS_NR21_SOUND_LENGTH_DATA                          5:0
#define LR35902_IO_REGS_NR21_WAVE_DUTY_PATTERN                          7:6
#define LR35902_IO_REGS_NR21_12_5_PERCENT                               0
#define LR35902_IO_REGS_NR21_25_PERCENT                                 1
#define LR35902_IO_REGS_NR21_50_PERCENT                                 2
#define LR35902_IO_REGS_NR21_75_PERCENT                                 3
#define LR35902_IO_REGS_NR22                                            0xff17 /* RW */
#define LR35902_IO_REGS_NR22_NUM_ENVELOPE_SWEEP                         2:0
#define LR35902_IO_REGS_NR22_ENVELOPE_DIRECTION                         3:3
#define LR35902_IO_REGS_NR22_INITIAL_VOLUME_ENVELOPE                    7:4
#define LR35902_IO_REGS_NR23                                            0xff18 /* RW */
#define LR35902_IO_REGS_NR23_FREQUENCY_LSW                              7:0
#define LR35902_IO_REGS_NR24                                            0xff19 /* RW */
#define LR35902_IO_REGS_NR24_FREQUENCY_MSW                              2:0
#define LR35902_IO_REGS_NR24_COUNTER                                    6:6
#define LR35902_IO_REGS_NR24_COUNTER_CONSECUTIVE                        0
#define LR35902_IO_REGS_NR24_COUNTER_PERIOD                             1
#define LR35902_IO_REGS_NR24_START                                      7:7
#define LR35902_IO_REGS_NR30                                            0xff1a /* RW */
#define LR35902_IO_REGS_NR30_SOUND_CONTROL                              7:7
#define LR35902_IO_REGS_NR30_SOUND_CONTROL_STOP                         0
#define LR35902_IO_REGS_NR30_SOUND_CONTROL_OK                           1
#define LR35902_IO_REGS_NR31                                            0xff1b /* RW */
#define LR35902_IO_REGS_NR31_SOUND_LENGTH                               7:0
#define LR35902_IO_REGS_NR32                                            0xff1c /* RW */
#define LR35902_IO_REGS_NR32_OUTPUT_LEVEL                               6:5
#define LR35902_IO_REGS_NR32_OUTPUT_LEVEL_MUTE                          0
#define LR35902_IO_REGS_NR32_OUTPUT_LEVEL_4_4_WAVE_PATTERN              1
#define LR35902_IO_REGS_NR32_OUTPUT_LEVEL_1_2_WAVE_PATTERN              2
#define LR35902_IO_REGS_NR32_OUTPUT_LEVEL_1_4_WAVE_PATTERN              3
#define LR35902_IO_REGS_NR33                                            0xff1d /* RW */
#define LR35902_IO_REGS_NR33_FREQUENCY_LSW                              7:0
#define LR35902_IO_REGS_NR34                                            0xff1e /* RW */
#define LR35902_IO_REGS_NR34_FREQUENCY_MSW                              2:0
#define LR35902_IO_REGS_NR34_COUNTER                                    6:6
#define LR35902_IO_REGS_NR34_COUNTER_CONSECUTIVE                        0
#define LR35902_IO_REGS_NR34_COUNTER_PERIOD                             1
#define LR35902_IO_REGS_NR34_START                                      7:7
#define LR35902_IO_REGS_NR41                                            0xff20 /* RW */
#define LR35902_IO_REGS_NR41_SOUND_LENGTH_DATA                          5:0
#define LR35902_IO_REGS_NR42                                            0xff21 /* RW */
#define LR35902_IO_REGS_NR42_NUM_ENVELOPE_SWEEP                         2:0
#define LR35902_IO_REGS_NR42_ENVELOPE_DIRECTION                         3:3
#define LR35902_IO_REGS_NR42_ENVELOPE_DIRECTION_ATTENUATE               0
#define LR35902_IO_REGS_NR42_ENVELOPE_DIRECTION_AMPLIFY                 1
#define LR35902_IO_REGS_NR42_INITIAL_VOLUME_ENVELOPE                    7:4
#define LR35902_IO_REGS_NR43                                            0xff22 /* RW */
#define LR35902_IO_REGS_NR43_DIVIDER                                    2:0
#define LR35902_IO_REGS_NR43_DIVIDER_TIMES_2                            0
#define LR35902_IO_REGS_NR43_DIVIDER_TIMES_1                            1
#define LR35902_IO_REGS_NR43_DIVIDER_TIMES_1_OVER_2                     2
#define LR35902_IO_REGS_NR43_DIVIDER_TIMES_1_OVER_3                     3
#define LR35902_IO_REGS_NR43_DIVIDER_TIMES_1_OVER_4                     4
#define LR35902_IO_REGS_NR43_DIVIDER_TIMES_1_OVER_5                     5
#define LR35902_IO_REGS_NR43_DIVIDER_TIMES_1_OVER_6                     6
#define LR35902_IO_REGS_NR43_DIVIDER_TIMES_1_OVER_7                     7
#define LR35902_IO_REGS_NR44                                            0xff23 /* RW */
#define LR35902_IO_REGS_NR44_COUNTER                                    6:6
#define LR35902_IO_REGS_NR44_COUNTER_CONSECUTIVE                        0
#define LR35902_IO_REGS_NR44_COUNTER_PERIOD                             1
#define LR35902_IO_REGS_NR44_START                                      7:7
#define LR35902_IO_REGS_NR50                                            0xff24 /* RW */
#define LR35902_IO_REGS_NR50_SO2_CONTROL                                7:7
#define LR35902_IO_REGS_NR50_SO2_CONTROL_OFF                            0
#define LR35902_IO_REGS_NR50_SO2_CONTROL_ON                             1
#define LR35902_IO_REGS_NR50_SO2_OUTPUT_LEVEL                           6:4
#define LR35902_IO_REGS_NR50_SO1_CONTROL                                3:3
#define LR35902_IO_REGS_NR50_SO1_CONTROL_OFF                            0
#define LR35902_IO_REGS_NR50_SO1_CONTROL_ON                             1
#define LR35902_IO_REGS_NR50_SO1_OUTPUT_LEVEL                           2:0
#define LR35902_IO_REGS_NR51                                            0xff25 /* RW */
#define LR35902_IO_REGS_NR51_OUTPUT_SOUND_4_TO_SO2                      7:7
#define LR35902_IO_REGS_NR51_OUTPUT_SOUND_3_TO_SO2                      6:6
#define LR35902_IO_REGS_NR51_OUTPUT_SOUND_2_TO_SO2                      5:5
#define LR35902_IO_REGS_NR51_OUTPUT_SOUND_1_TO_SO2                      4:4
#define LR35902_IO_REGS_NR51_OUTPUT_SOUND_4_TO_SO1                      3:3
#define LR35902_IO_REGS_NR51_OUTPUT_SOUND_3_TO_SO1                      2:2
#define LR35902_IO_REGS_NR51_OUTPUT_SOUND_2_TO_SO1                      1:1
#define LR35902_IO_REGS_NR51_OUTPUT_SOUND_1_TO_SO1                      0:0
#define LR35902_IO_REGS_NR52                                            0xff26 /* RW */
#define LR35902_IO_REGS_NR52_MASTER_CONTROL                             7:7
#define LR35902_IO_REGS_NR52_MASTER_CONTROL_DISABLE                     0
#define LR35902_IO_REGS_NR52_MASTER_CONTROL_ENABLE                      1
#define LR35902_IO_REGS_NR52_SOUND4                                     3:3
#define LR35902_IO_REGS_NR52_SOUND4_DISABLE                             0
#define LR35902_IO_REGS_NR52_SOUND4_ENABLE                              1
#define LR35902_IO_REGS_NR52_SOUND3                                     2:2
#define LR35902_IO_REGS_NR52_SOUND3_DISABLE                             0
#define LR35902_IO_REGS_NR52_SOUND3_ENABLE                              1
#define LR35902_IO_REGS_NR52_SOUND2                                     1:1
#define LR35902_IO_REGS_NR52_SOUND2_DISABLE                             0
#define LR35902_IO_REGS_NR52_SOUND2_ENABLE                              1
#define LR35902_IO_REGS_NR52_SOUND1                                     0:0
#define LR35902_IO_REGS_NR52_SOUND1_DISABLE                             0
#define LR35902_IO_REGS_NR52_SOUND1_ENABLE                              1
#define LR35902_IO_REGS_LCDC                                            0xff40 /* RW */
#define LR35902_IO_REGS_LCDC_CONTROL                                    7:7
#define LR35902_IO_REGS_LCDC_CONTROL_STOP                               0
#define LR35902_IO_REGS_LCDC_CONTROL_OPERATION                          1
#define LR35902_IO_REGS_LCDC_TILE_MAP_DISPLAY_SELECT                    6:6
#define LR35902_IO_REGS_LCDC_TILE_MAP_DISPLAY_SELECT_0                  0 /* 0x9800 - 0x9bff */
#define LR35902_IO_REGS_LCDC_TILE_MAP_DISPLAY_SELECT_1                  1 /* 0x9c00 - 0x9fff */
#define LR35902_IO_REGS_LCDC_WINDOW_DISPLAY                             5:5
#define LR35902_IO_REGS_LCDC_WINDOW_DISPLAY_OFF                         0
#define LR35902_IO_REGS_LCDC_WINDOW_DISPLAY_ON                          1
#define LR35902_IO_REGS_LCDC_BG_WINDOW_TILE_DATA_SELECT                 4:4
#define LR35902_IO_REGS_LCDC_BG_WINDOW_TILE_DATA_SELECT_0               0 /* 0x8800 - 0x97ff */
#define LR35902_IO_REGS_LCDC_BG_WINDOW_TILE_DATA_SELECT_1               1 /* 0x8000 - 0x8fff */
#define LR35902_IO_REGS_LCDC_BG_TILE_MAP_DISPLAY_SELECT                 3:3
#define LR35902_IO_REGS_LCDC_BG_TILE_MAP_DISPLAY_SELECT_0               0 /* 0x9800 - 0x9bff */
#define LR35902_IO_REGS_LCDC_BG_TILE_MAP_DISPLAY_SELECT_1               1 /* 0x9c00 - 0x9fff */
#define LR35902_IO_REGS_LCDC_SPRITE_SIZE                                2:2
#define LR35902_IO_REGS_LCDC_SPRITE_SIZE_8_BY_8                         0
#define LR35902_IO_REGS_LCDC_SPRITE_SIZE_8_BY_16                        1
#define LR35902_IO_REGS_LCDC_SPRITE_DISPLAY                             1:1
#define LR35902_IO_REGS_LCDC_SPRITE_DISPLAY_OFF                         0
#define LR35902_IO_REGS_LCDC_SPRITE_DISPLAY_ON                          1
#define LR35902_IO_REGS_LCDC_BG_WINDOW_DISPLAY                          0:0
#define LR35902_IO_REGS_LCDC_BG_WINDOW_DISPLAY_OFF                      0
#define LR35902_IO_REGS_LCDC_BG_WINDOW_DISPLAY_ON                       1
#define LR35902_IO_REGS_STAT                                            0xff41 /* RW */
#define LR35902_IO_REGS_STAT_LYC_EQUALS_LY_CONINCIDENCE                 6:6
#define LR35902_IO_REGS_STAT_MODE2                                      5:5
#define LR35902_IO_REGS_STAT_MODE1                                      4:4
#define LR35902_IO_REGS_STAT_MODE0                                      3:3
#define LR35902_IO_REGS_STAT_MODE0_NON_SELECTION                        0
#define LR35902_IO_REGS_STAT_MODE0_SELECTION                            1
#define LR35902_IO_REGS_STAT_COINCIDENCE_FLAG                           2:2
#define LR35902_IO_REGS_STAT_COINCIDENCE_FLAG_LYC_NE_LCDC_LY            0
#define LR35902_IO_REGS_STAT_COINCIDENCE_FLAG_LYC_EQ_LCDC_LY            1
#define LR35902_IO_REGS_STAT_MODE_FLAG                                  1:0
#define LR35902_IO_REGS_STAT_MODE_FLAG_DURING_H_BLANK                   0
#define LR35902_IO_REGS_STAT_MODE_FLAG_DURING_V_BLANK                   1
#define LR35902_IO_REGS_STAT_MODE_FLAG_DURING_SEARCHING_OAM_RAM         2
#define LR35902_IO_REGS_STAT_MODE_FLAG_DURING_TRANSFER_TO_LCD_DRIVER    3
#define LR35902_IO_REGS_SCY                                             0xff42 /* RW */
#define LR35902_IO_REGS_SCX                                             0xff43 /* RW */
#define LR35902_IO_REGS_LY                                              0xff44 /* RW */
#define LR35902_IO_REGS_LYC                                             0xff45 /* RW */
#define LR35902_IO_REGS_DMA                                             0xff46 /* RW */
#define LR35902_IO_REGS_BGP                                             0xff47 /* RW */
#define LR35902_IO_REGS_BGP_DATA_DOT3                                   7:6
#define LR35902_IO_REGS_BGP_DATA_DOT2                                   5:4
#define LR35902_IO_REGS_BGP_DATA_DOT1                                   3:2
#define LR35902_IO_REGS_BGP_DATA_DOT0                                   1:0
#define LR35902_IO_REGS_OBP0                                            0xff48 /* RW */
#define LR35902_IO_REGS_OBP1                                            0xff49 /* RW */
#define LR35902_IO_REGS_WY                                              0xff4a /* RW */
#define LR35902_IO_REGS_WX                                              0xff4b /* RW */
#define LR35902_IO_REGS_IE                                              0xffff /* RW */
#define LR35902_IO_REGS_IE_VBLANK                                       0:0
#define LR35902_IO_REGS_IE_VBLANK_OFF                                   0
#define LR35902_IO_REGS_IE_VBLANK_ON                                    1
#define LR35902_IO_REGS_IE_LCD_STAT                                     1:1
#define LR35902_IO_REGS_IE_LCD_STAT_OFF                                 0
#define LR35902_IO_REGS_IE_LCD_STAT_ON                                  1
#define LR35902_IO_REGS_IE_TIMER                                        2:2
#define LR35902_IO_REGS_IE_TIMER_OFF                                    0
#define LR35902_IO_REGS_IE_TIMER_ON                                     1
#define LR35902_IO_REGS_IE_SERIAL                                       3:3
#define LR35902_IO_REGS_IE_SERIAL_OFF                                   0
#define LR35902_IO_REGS_IE_SERIAL_ON                                    1
#define LR35902_IO_REGS_IE_JOYPAD                                       4:4
#define LR35902_IO_REGS_IE_JOYPAD_OFF                                   0
#define LR35902_IO_REGS_IE_JOYPAD_ON                                    1

#endif /* PGB_IO_PRIVATE_IO_REGS_H */
