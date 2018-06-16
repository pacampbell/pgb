#ifndef PGB_DEVICE_PRIVATE_REGISTERS_SOUND_REGS_H
#define PGB_DEVICE_PRIVATE_REGISTERS_SOUND_REGS_H

#define DMG_SOUND_NR10                                            0xff10:0xff10 /* RW */
#define DMG_SOUND_NR10_SWEEP_TIME                                           6:4
#define DMG_SOUND_NR10_SWEEP_FREQUENCY_ADJUST                               3:3
#define DMG_SOUND_NR10_SWEEP_FREQUENCY_ADJUST_INCREASE                        0
#define DMG_SOUND_NR10_SWEEP_FREQUENCY_ADJUST_DECREASE                        1
#define DMG_SOUND_NR10_NUM_SWEEP_SHIFT                                      2:0
#define DMG_SOUND_NR11                                            0xff11:0xff11 /* RW */
#define DMG_SOUND_NR11_WAVE_PATTERN_DUTY                                    7:6 /* RW */
#define DMG_SOUND_NR11_SOUND_LENGTH_DATA                                    5:0 /* WO */
#define DMG_SOUND_NR12                                            0xff12:0xff12 /* RW */
#define DMG_SOUND_NR12_INIT_VOLUME_ENVELOPE                                 7:4
#define DMG_SOUND_NR12_INIT_VOLUME_ENVELOPE_NO_SOUND                          0
#define DMG_SOUND_NR12_ENVELOPE_DIRECTION                                   3:3
#define DMG_SOUND_NR12_ENVELOPE_DIRECTION_DECREASE                            0
#define DMG_SOUND_NR12_ENVELOPE_DIRECTION_INCREASE                            1
#define DMG_SOUND_NR12_NUM_ENVELOPE_SWEEP                                   2:0
#define DMG_SOUND_NR13                                            0xff13:0xff13 /* WO */
#define DMG_SOUND_NR13_FREQUENCY_8_LSB                                      7:0
#define DMG_SOUND_NR14                                            0xff14:0xff14 /* RW */
#define DMG_SOUND_NR14_INITIAL                                              7:7 /* WO */
#define DMG_SOUND_NR14_INITIAL_RESTART_SOUND                                  1
#define DMG_SOUND_NR14_COUNTER                                              6:6 /* RW */
#define DMG_SOUND_NR14_COUNTER_STOP_WHEN_NR11_EXPIRES                         1
#define DMG_SOUND_NR14_FREQUENCY_3_MSB                                      2:0 /* WO */
#define DMG_SOUND_NR21                                            0xff16:0xff16 /* RW */
#define DMG_SOUND_NR21_WAVE_PATTERN_DUTY                                    7:6 /* RW */
#define DMG_SOUND_NR21_SOUND_LENGTH_DATA                                    5:0 /* WO */
#define DMG_SOUND_NR22                                            0xff17:0xff17 /* RW */
#define DMG_SOUND_NR22_INIT_VOLUME_ENVELOPE                                 7:4
#define DMG_SOUND_NR22_INIT_VOLUME_ENVELOPE_NO_SOUND                          0
#define DMG_SOUND_NR22_ENVELOPE_DIRECTION                                   3:3
#define DMG_SOUND_NR22_ENVELOPE_DIRECTION_DECREASE                            0
#define DMG_SOUND_NR22_ENVELOPE_DIRECTION_INCREASE                            1
#define DMG_SOUND_NR22_NUM_ENVELOPE_SWEEP                                   2:0
#define DMG_SOUND_NR23                                            0xff18:0xff18
#define DMG_SOUND_NR23_FREQUENCY_8_LSB                                      7:0
#define DMG_SOUND_NR24                                            0xff19:0xff19
#define DMG_SOUND_NR24_INITIAL                                              7:7 /* WO */
#define DMG_SOUND_NR24_INITIAL_RESTART_SOUND                                  1
#define DMG_SOUND_NR24_COUNTER                                              6:6 /* RW */
#define DMG_SOUND_NR24_COUNTER_STOP_WHEN_NR21_EXPIRES                         1
#define DMG_SOUND_NR24_FREQUENCY_3_MSB                                      2:0 /* WO */
#define DMG_SOUND_NR30                                            0xff1a:0xff1a /* RW */
#define DMG_SOUND_NR30_SOUND_CHANNEL_3_ENABLE                               7:7
#define DMG_SOUND_NR30_SOUND_CHANNEL_3_ENABLE_OFF                             0
#define DMG_SOUND_NR30_SOUND_CHANNEL_3_ENABLE_ON                              1
#define DMG_SOUND_NR31                                            0xff1b:0xff1b
#define DMG_SOUND_NR31_SOUND_LENGTH                                         7:0
#define DMG_SOUND_NR32                                            0xff1c:0xff1c /* RW */
#define DMG_SOUND_NR32_SELECT_OUTPUT_LEVEL                                  6:5
#define DMG_SOUND_NR32_SELECT_OUTPUT_LEVEL_MUTE                               0
#define DMG_SOUND_NR32_SELECT_OUTPUT_LEVEL_100                                1
#define DMG_SOUND_NR32_SELECT_OUTPUT_LEVEL_50                                 2
#define DMG_SOUND_NR32_SELECT_OUTPUT_LEVEL_25                                 3
#define DMG_SOUND_NR33                                            0xff1d:0xff1d /* WO */
#define DMG_SOUND_NR33_FREQUENCY_8_LSB                                      7:0
#define DMG_SOUND_NR34                                            0xff1e:0xff1e
#define DMG_SOUND_NR34_INITIAL                                              7:7 /* WO */
#define DMG_SOUND_NR34_INITIAL_RESTART_SOUND                                  1
#define DMG_SOUND_NR34_COUNTER                                              6:6 /* RW */
#define DMG_SOUND_NR34_COUNTER_STOP_WHEN_NR31_EXPIRES                         1
#define DMG_SOUND_NR34_FREQUENCY_3_MSB                                      2:0 /* WO */
#define DMG_SOUND_WAVE_PATTERN_RAM                                0xff3f:0xff30
#define DMG_SOUND_NR41                                            0xff20:0xff20
#define DMG_SOUND_NR41_SOUND_LENGTH_DATA                                    5:0
#define DMG_SOUND_NR42                                            0xff21:0xff21 /* RW */
#define DMG_SOUND_NR42_INIT_VOLUME_ENVELOPE                                 7:4
#define DMG_SOUND_NR42_INIT_VOLUME_ENVELOPE_NO_SOUND                          0
#define DMG_SOUND_NR42_ENVELOPE_DIRECTION                                   3:3
#define DMG_SOUND_NR42_ENVELOPE_DIRECTION_DECREASE                            0
#define DMG_SOUND_NR42_ENVELOPE_DIRECTION_INCREASE                            1
#define DMG_SOUND_NR42_NUM_ENVELOPE_SWEEP                                   2:0
#define DMG_SOUND_NR43                                            0xff22:0xff22 /* RW */
#define DMG_SOUND_NR43_SHIFT_CLOCK_FREQUENCY                                7:4
#define DMG_SOUND_NR43_COUNTER_STEP_WIDTH                                   3:3
#define DMG_SOUND_NR43_COUNTER_STEP_WIDTH_15_bits                             0
#define DMG_SOUND_NR43_COUNTER_STEP_WIDTH_7_bits                              1
#define DMG_SOUND_NR44                                            0xff23:0xff23 /* RW */
#define DMG_SOUND_NR44_INITIAL                                              7:7 /* WO */
#define DMG_SOUND_NR44_INITIAL_RESTART_SOUND                                  1
#define DMG_SOUND_NR44_COUNTER                                              6:6 /* RW */
#define DMG_SOUND_NR44_COUNTER_STOP_WHEN_NR41_EXPIRES                         1
#define DMG_SOUND_NR50                                            0xff23:0xff24 /* RW */
#define DMG_SOUND_NR50_OUTPUT_VIN_TO_S02_TERMINAL                           7:7
#define DMG_SOUND_NR50_OUTPUT_VIN_TO_S02_TERMINAL_DISABLE                     0
#define DMG_SOUND_NR50_OUTPUT_VIN_TO_S02_TERMINAL_ENABLE                      1
#define DMG_SOUND_NR50_SO2_VOLUME                                           6:4
#define DMG_SOUND_NR50_OUTPUT_VIN_TO_SO1_TERMINAL                           3:3
#define DMG_SOUND_NR50_OUTPUT_VIN_TO_SO1_TERMINAL_DISABLE                     0
#define DMG_SOUND_NR50_OUTPUT_VIN_TO_SO1_TERMINAL_ENABLE                      1
#define DMG_SOUND_NR50_SO1_VOLUME                                           2:0
#define DMG_SOUND_NR51                                            0xff25:0xff25 /* RW */
#define DMG_SOUND_NR51_OUTPUT_SOUND_4_TO_SO2_TERMINAL                       7:7
#define DMG_SOUND_NR51_OUTPUT_SOUND_3_TO_SO2_TERMINAL                       6:6
#define DMG_SOUND_NR51_OUTPUT_SOUND_2_TO_SO2_TERMINAL                       5:5
#define DMG_SOUND_NR51_OUTPUT_SOUND_1_TO_SO2_TERMINAL                       4:4
#define DMG_SOUND_NR51_OUTPUT_SOUND_4_TO_SO1_TERMINAL                       3:3
#define DMG_SOUND_NR51_OUTPUT_SOUND_3_TO_SO1_TERMINAL                       2:2
#define DMG_SOUND_NR51_OUTPUT_SOUND_2_TO_SO1_TERMINAL                       1:1
#define DMG_SOUND_NR51_OUTPUT_SOUND_1_TO_SO1_TERMINAL                       0:0
#define DMG_SOUND_NR52                                            0xff26:0xff26 /* RW */
#define DMG_SOUND_NR52_SOUND_ENABLE                                         7:7 /* RW */
#define DMG_SOUND_NR52_SOUND_ENABLE_OFF                                       0
#define DMG_SOUND_NR52_SOUND_ENABLE_ON                                        1
#define DMG_SOUND_NR52_SOUND_4_ENABLE                                       3:3 /* RO */
#define DMG_SOUND_NR52_SOUND_4_ENABLE_OFF                                     0
#define DMG_SOUND_NR52_SOUND_4_ENABLE_ON                                      1
#define DMG_SOUND_NR52_SOUND_3_ENABLE                                       2:2 /* RO */
#define DMG_SOUND_NR52_SOUND_3_ENABLE_OFF                                     0
#define DMG_SOUND_NR52_SOUND_3_ENABLE_ON                                      1
#define DMG_SOUND_NR52_SOUND_2_ENABLE                                       1:1 /* RO */
#define DMG_SOUND_NR52_SOUND_2_ENABLE_OFF                                     0
#define DMG_SOUND_NR52_SOUND_2_ENABLE_ON                                      1
#define DMG_SOUND_NR52_SOUND_1_ENABLE                                       0:0 /* RO */
#define DMG_SOUND_NR52_SOUND_1_ENABLE_OFF                                     0
#define DMG_SOUND_NR52_SOUND_1_ENABLE_ON                                      1

#endif /* PGB_DEVICE_PRIVATE_REGISTERS_SOUND_REGS_H */
