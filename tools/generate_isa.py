#!/usr/bin/env python

import sys
import argparse
import json

def parse_cycle(cycle_data):
    parts = cycle_data.split('/')

    if len(parts) == 2:
        return int(parts[0]), int(parts[1])
    else:
        return int(parts[0]), int(parts[0])

def is_hex_literal(value):
    if (value[-1] == 'h' or value[-1] == 'H') and value[:-1].isdigit():
        return True
    return False

def determine_operand_type(mnemonic, operand):
    operand_type = None
    reg8 = ['A', 'B', 'C', 'D', 'E', 'F', 'H', 'L']
    reg16 = ['AF', 'BC', 'DE', 'HL', 'SP', 'PC']
    conditions = ['COND_Z', 'COND_NZ', 'COND_C', 'COND_NC']
    vectors = ['VEC_00H', 'VEC_08H', 'VEC_10H', 'VEC_18H', 'VEC_20H', 'VEC_28H', 'VEC_30H', 'VEC_38H']
    u3_enums = ['U3_0', 'U3_1', 'U3_2', 'U3_3', 'U3_4', 'U3_5', 'U3_6', 'U3_7']

    if operand in conditions:
        operand_type = 'CONDITION'
    elif operand in vectors:
        operand_type = 'VEC'
    elif operand in u3_enums:
        operand_type = 'U3'
    elif operand in reg8:
        operand_type = 'REGISTER8'
    elif operand in reg16:
        operand_type = 'REGISTER16'
    else:
        operand_type = operand

    return operand_type

def _parse_operand(mnemonic, operand, position):
    cf_instrs = ['CALL', 'JP', 'JR', 'RET']
    conditions = ['Z', 'NZ', 'C', 'NC']

    if operand is None:
        return 'NONE', 'NONE'

    if '(' in operand:
        raw_operand = operand.translate(None, '()+-')
        if '8' in raw_operand or len(raw_operand) == 1:
            if position == 0:
                operand_modifier = 'MEM_WRITE_8'
            else:
                operand_modifier = 'MEM_READ_8'
        elif '16' in raw_operand or len(raw_operand) == 2:
            if position == 0:
                operand_modifier = 'MEM_WRITE_16'
            else:
                operand_modifier = 'MEM_READ_16'
    else:
        raw_operand = operand
        operand_modifier = 'NONE'

    if mnemonic == 'STOP' or mnemonic == 'INVALID':
        raw_operand = 'NONE'
    elif mnemonic == 'PREFIX':
        raw_operand = 'NONE'
        operand_modifier = 'CB'
    elif mnemonic in cf_instrs and raw_operand in conditions:
        raw_operand = 'COND_{}'.format(raw_operand)
    elif raw_operand.isdigit() or is_hex_literal(raw_operand):
        if raw_operand[-1] == 'h':
            if raw_operand[:-1].isdigit():
                value = int(raw_operand[:-1], 16)
        elif raw_operand.isdigit():
            value = int(raw_operand)
        if mnemonic != 'RST':
            if value >= 0 and value <= 7:
                raw_operand = 'U3_{}'.format(value)
        else:
            raw_operand = 'VEC_{}'.format(raw_operand)
    elif 'a8' in raw_operand or 'd8' in raw_operand:
        raw_operand = 'U8'
    elif 'r8' in raw_operand:
        raw_operand = 'I8'
    elif 'a16' in raw_operand or 'd16' in raw_operand:
        raw_operand = 'U16'

    return raw_operand, operand_modifier

def parse_operands(mnemonic, operands):
    parts = operands.split(',')
    if len(parts) == 2:
        part_a = parts[0]
        part_b = parts[1]
    else:
        part_a = parts[0]
        part_b = None

    operand_a, modifier_a = _parse_operand(mnemonic, part_a, 0)
    operand_b, modifier_b = _parse_operand(mnemonic, part_b, 1)

    return operand_a.upper(), modifier_a, operand_b.upper(), modifier_b

def parse_mnemonic(mnemonic, parts):
    return mnemonic

def create_unique_define(mnemonic, opcode, operand_a, operand_b, modifier_a, modifier_b):
    define_name = mnemonic

    if modifier_a == 'MEM_WRITE_8':
        memory_modifier = 'W8'
    elif modifier_a == 'MEM_WRITE_16':
        memory_modifier = 'W16'
    elif modifier_b == 'MEM_READ_8':
        memory_modifier = 'R8'
    elif modifier_b == 'MEM_READ_16':
        memory_modifier = 'R16'
    else:
        memory_modifier = None

    if memory_modifier is not None:
        define_name = '{}_{}'.format(define_name, memory_modifier)

    if mnemonic == 'PREFIX':
        define_name = '{}_{}'.format(define_name, modifier_a)
    elif mnemonic == 'INVALID':
        define_name = '{}_{:02X}'.format(define_name, opcode)

    if operand_a != 'NONE':
        if operand_a == 'VEC':
            define_name = '{}_{}'.format(define_name, modifier_a)
        else:
            define_name = '{}_{}'.format(define_name, operand_a)

    if operand_b != 'NONE':
        define_name = '{}_{}'.format(define_name, operand_b)

    return define_name

def parse_flags(flags_data):
    '''
    If flag is zero it is reset after the instruction
    If flag is one it is set
    If flag is - it is marked as a 3 (means it is not used)
    if flag is Z, N, H, or C it is marked as 2 (means  the flag is affected by its function)
    '''
    if len(flags_data) != 4:
        raise exception('Invalid data inside source opcode definition. Unable to parse. Please fix and try again.')

    flag_mask = 0x0
    flags = [0, 0, 0, 0]

    i = 3
    for flag in flags_data:
        if flag == '-':
            flags[i] = 3
        else:
            flag_mask = flag_mask | (1 << i)
            if flag.isdigit():
                flags[i] = int(flag)
            else:
                flags[i] = 2
        i = i - 1

    # flag_z, flag_n, flag_h, flag_c, flag_mask
    return flags[3], flags[2], flags[1], flags[0], flag_mask

def generate_c_header(name, prefix, parsed_instructions):
    print '/* This file was generated by tools/generate_isa.py */'
    print '#ifndef PGB_{}_H'.format(name)
    print '#define PGB_{}_H\n'.format(name)

    define_formats = [
        {'string': '#define {}_OPCODE_{{}}'.format(prefix), 'value': '0x{:02x}'},
        {'string': '#define {}_OPCODE_{{}}_NUM_BYTES'.format(prefix), 'value': '{:d}'},
        {'string': '#define {}_OPCODE_{{}}_NUM_CYCLES_C0'.format(prefix), 'value': '{:d}'},
        {'string': '#define {}_OPCODE_{{}}_NUM_CYCLES_C1'.format(prefix), 'value': '{:d}'},
        {'string': '#define {}_OPCODE_{{}}_FLAG_MASK'.format(prefix), 'value': '0x{:x}'},
        {'string': '#define {}_OPCODE_{{}}_FLAG_Z'.format(prefix), 'value': '{:d}'},
        {'string': '#define {}_OPCODE_{{}}_FLAG_N'.format(prefix), 'value': '{:d}'},
        {'string': '#define {}_OPCODE_{{}}_FLAG_H'.format(prefix), 'value': '{:d}'},
        {'string': '#define {}_OPCODE_{{}}_FLAG_C'.format(prefix), 'value': '{:d}'}
    ]

    for instruction in parsed_instructions:
        values = [
            instruction['define_name'],
            instruction['opcode'],
            instruction['num_bytes'],
            instruction['timing']['c0'],
            instruction['timing']['c1'],
            instruction['flags']['mask'],
            instruction['flags']['z'],
            instruction['flags']['n'],
            instruction['flags']['h'],
            instruction['flags']['c']
        ]

        i = 1
        for define_format in define_formats:
            define = define_format['string'].format(values[0])
            value = define_format['value'].format(values[i])

            print '{:75s} {:>4s}'.format(define, value)
            i = i + 1

    print '\n#endif /* PGB_{}_H */\n'.format(name)

def generate_c_def(is_prefix_cb, parsed_instructions):
    print '/* This file was generated by tools/generate_isa.py */'

    # SET(MNEMONIC, ASSEMBLY, OPCODE, OPERATION_CLASS, OPERAND_A, TYPE_A, MODIFIER_A, OPERAND_B, TYPE_B, MODIFIER_B, NUM_BYTES, C0, C1, FLAG_MASK, FLAG_Z, FLAG_N, FLAG_H, FLAG_C, IS_PREFIX)
    for instruction in parsed_instructions:
        print 'SET({}, "{}", 0x{:02x}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, 0x{:0x}, {}, {}, {}, {}, {})'.format(
              instruction['mnemonic'],
              instruction['assembly'],
              instruction['opcode'],
              'INSTRUCTION_CLASS_{}'.format(instruction['mnemonic']),
              'INSTRUCTION_OPERAND_{}'.format(instruction['operands']['a']['value']),
              'INSTRUCTION_OPERAND_TYPE_{}'.format(instruction['operands']['a']['type']),
              'INSTRUCTION_OPERAND_MODIFIER_{}'.format(instruction['operands']['a']['modifier']),
              'INSTRUCTION_OPERAND_{}'.format(instruction['operands']['b']['value']),
              'INSTRUCTION_OPERAND_TYPE_{}'.format(instruction['operands']['b']['type']),
              'INSTRUCTION_OPERAND_MODIFIER_{}'.format(instruction['operands']['b']['modifier']),
              instruction['num_bytes'],
              instruction['timing']['c0'],
              instruction['timing']['c1'],
              instruction['flags']['mask'],
              instruction['flags']['z'],
              instruction['flags']['n'],
              instruction['flags']['h'],
              instruction['flags']['c'],
              str(is_prefix_cb).lower())

def generate_json(parsed_instructions):
    json.dump(parsed_instructions, sys.stdout, indent=4)

def parse_input_file(source_path):
    with open(source_path, 'r') as source_file:
        lines = source_file.readlines()

    parsed_instructions = []

    for line in lines:
        line = line.strip()
        parts = line.split(' ')

        if len(parts) < 8:
            continue

        opcode = int(parts[0], 16)
        mnemonic = parse_mnemonic(parts[1], parts[2:])

        if len(parts) == 9:
            assembly = '{} {}'.format(mnemonic, parts[2])
            operand_a, modifier_a, operand_b, modifier_b = parse_operands(mnemonic, parts[2])
            num_bytes = int(parts[3])
            c0, c1 = parse_cycle(parts[4])
            flag_z, flag_n, flag_h, flag_c, flag_mask = parse_flags(parts[5:])
        else:
            assembly = mnemonic
            operand_a = 'NONE'
            operand_b = 'NONE'
            modifier_a = 'NONE'
            modifier_b = 'NONE'
            num_bytes = int(parts[2])
            c0, c1 = parse_cycle(parts[3])
            flag_z, flag_n, flag_h, flag_c, flag_mask = parse_flags(parts[4:])

        define_name = create_unique_define(mnemonic, opcode, operand_a, operand_b, modifier_a, modifier_b)
        type_a = determine_operand_type(mnemonic, operand_a)
        type_b = determine_operand_type(mnemonic, operand_b)

        instruction = {
            'mnemonic': mnemonic,
            'assembly': assembly,
            'define_name': define_name,
            'opcode': opcode,
            'num_bytes': num_bytes,
            'flags': {
                'z': flag_z,
                'n': flag_n,
                'h': flag_h,
                'c': flag_c,
                'mask': flag_mask
            },
            'timing': {
                'c0': c0,
                'c1': c1
            },
            'operands': {
                'a': {
                    'type': type_a,
                    'value': operand_a,
                    'modifier': modifier_a
                },
                'b': {
                    'type': type_b,
                    'value': operand_b,
                    'modifier': modifier_b
                }
            }
        }

        parsed_instructions.append(instruction)

    return parsed_instructions

def main():
    parser = argparse.ArgumentParser(description='Converts Gameboy LR35902 isa into C header files.')
    parser.add_argument('-s', '--source', dest='source_path', type=str, default='data/opcodes.txt',
                        help='Path to textfile containting opcode information.')
    parser.add_argument('-f', '--format', dest='output_format', type=str, default="header",
                        help='Controls the type of output generated by this script. Valid values are "header", "definition" and "json".')
    parser.add_argument('-n', '--name', dest='name', type=str, default='CPU_ISA',
                        help='Controls the name of the header guards produced.')
    parser.add_argument('-p', '--prefix', dest='prefix', type=str, default='LR35902',
                        help='Controls the prefix of the defined generated.')
    parser.add_argument('--is-prefix-cb', dest='is_prefix_cb', action='store_true',
                        help='Specifies boolean flag for struct isa_instruction.')

    args = parser.parse_args()
    parsed_instructions = parse_input_file(args.source_path)

    if args.output_format == 'header':
        generate_c_header(args.name, args.prefix, parsed_instructions)
    elif args.output_format == 'definition':
        generate_c_def(args.is_prefix_cb, parsed_instructions)
    elif args.output_format == 'json':
        generate_json(parsed_instructions)
    else:
        print 'The output format "{}" is not supported.'.format(args.output_format)


if __name__ == "__main__":
    sys.exit(main())
