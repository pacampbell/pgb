#ifndef PGB_DEVICE_PRIVATE_REGISTERS_SERIAL_REGS_H
#define PGB_DEVICE_PRIVATE_REGISTERS_SERIAL_REGS_H

#define DMG_LINK_CABLE_SB                                         0xff01:0xff01
#define DMG_LINK_CABLE_SB_DATA                                              7:0
#define DMG_LINK_CABLE_SC                                         0xff02:0xff02
#define DMG_LINK_CABLE_SC_TRANSFER_CTRL                                     1:1
#define DMG_LINK_CABLE_SC_TRANSFER_CTRL_NO_TRANSFER                           0
#define DMG_LINK_CABLE_SC_TRANSFER_CTRL_START                                 1
#define DMG_LINK_CABLE_SC_SHIFT_CLOCK                                       0:0
#define DMG_LINK_CABLE_SC_SHIFT_CLOCK_EXTERNAL                                0
#define DMG_LINK_CABLE_SC_SHIFT_CLOCK_INTERNAL                                1

#endif /* PGB_DEVICE_PRIVATE_REGISTERS_SERIAL_REGS_H */
