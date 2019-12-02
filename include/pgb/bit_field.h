#ifndef PGB_BIT_FIELD_H
#define PGB_BIT_FIELD_H

#define LSB(field) (0 ? field)
#define MSB(field) (1 ? field)
#define BIT_FLD_MASK(field) ((1 << (MSB(field) - LSB(field) + 1)) - 1)

#define BF_GET_FLD(field, data) ((data >> LSB(field)) & BIT_FLD_MASK(field))
#define BF_CLR_FLD(field, data) ((~BIT_FLD_MASK(field) << LSB(field)) & data)
#define BF_SET_FLD(field, value, data) (BF_CLR_FLD(field, data) | (((value) & BIT_FLD_MASK(field)) << LSB(field)))

#endif /* PGB_BIT_FIELD_H */

