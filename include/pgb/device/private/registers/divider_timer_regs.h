#ifndef PGB_DEVICE_PRIVATE_REGISTERS_DIVIDER_TIMER_REGS_H
#define PGB_DEVICE_PRIVATE_REGISTERS_DIVIDER_TIMER_REGS_H

#define DMG_REGISTER_DIV                                          0xff04:0xff04
#define DMG_REGISTER_TIMA                                         0xff05:0xff05
#define DMG_REGISTER_TMA                                          0xff06:0xff06
#define DMG_REGISTER_TAC                                          0xff07:0xff07
#define DMG_REGISTER_TAC_TIMER_CTRL                                         2:2
#define DMG_REGISTER_TAC_TIMER_CTRL_STOP                                      0
#define DMG_REGISTER_TAC_TIMER_CTRL_START                                     1
#define DMG_REGISTER_TAC_INPUT_CLOCK_SELECT                                 1:0
#define DMG_REGISTER_TAC_INPUT_CLOCK_SELECT_4096_HZ                           0
#define DMG_REGISTER_TAC_INPUT_CLOCK_SELECT_262144_HZ                         1
#define DMG_REGISTER_TAC_INPUT_CLOCK_SELECT_65536_HZ                          2
#define DMG_REGISTER_TAC_INPUT_CLOCK_SELECT_16384_HZ                          3

#endif /* PGB_DEVICE_PRIVATE_REGISTERS_DIVIDER_TIMER_REGS_H */
