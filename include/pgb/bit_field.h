#ifndef PGB_BIT_FIELD_H
#define PGB_BIT_FIELD_H

#define LSB(field) (0 ? field)
#define MSB(field) (1 ? field)
#define BIT_MASK(field) ((1 << (MSB(field) - LSB(field) + 1)) - 1)

#define BF_GET_FLD(field, value) ((value >> LSB(field)) & BIT_MASK(field))
#define BF_CLR_FLD(field, value) ((BIT_MASK(field) << LSB(field)) & value)
#define BF_SET_FLD(field, value, set) (((BIT_MASK(field) << LSB(field)) & value) | ((MASK(FIELD) & set) << LSB(field)))

#endif /* PGB_BIT_FIELD_H */
