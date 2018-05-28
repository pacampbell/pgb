#!/usr/bin/env python

import sys
import argparse

def parse_cycle(cycle_data):
    parts = cycle_data.split('/')

    if len(parts) == 2:
        return int(parts[0]), int(parts[1])
    else:
        return int(parts[0]), 0

def parse_complex_mnemonic(mnemonic, mnemonic_cont):
    parts = mnemonic_cont.split(',')
    if len(parts) == 2:
        part_a = parts[0]
        part_b = parts[1]
    else:
        part_a = parts[0]
        part_b = None

    if '(' in part_a:
        if 'a8' in part_a:
            mnemonic_determined_a = 'MEM_W_8'
        elif 'a16' in part_a:
            mnemonic_determined_a = 'MEM_W_16'
        elif len(part_a) == 3:
            mnemonic_determined_a = '{}_MEM_W_8'.format(part_a.translate(None, "()"))
        elif len(part_a) == 4:
            mnemonic_determined_a = '{}_MEM_W_16'.format(part_a.translate(None, "()"))
        else:
            mnemonic_determined_a = part_a
    else:
        if 'd8' in part_a or 'r8' in part_a:
            mnemonic_determined_a = 'IMM_8'
        elif 'd16' in part_a or 'r16' in part_a:
            mnemonic_determined_a = 'IMM_16'
        else:
            mnemonic_determined_a = part_a

    if part_b is not None:
        if '(' in part_b:
            if 'a8' in part_b:
                mnemonic_determined_b = "MEM_R_8"
            elif 'a16' in part_b:
                mnemonic_determined_b = "MEM_R_16"
            elif len(part_b) == 3:
                mnemonic_determined_b = '{}_MEM_R_8'.format(part_b.translate(None, "()"))
            elif len(part_b) == 4:
                mnemonic_determined_b = '{}_MEM_R_16'.format(part_b.translate(None, "()"))
            else:
                mnemonic_determined_b = part_b
        else:
            if 'd8' in part_b or 'r8' in part_b:
                mnemonic_determined_b = 'IMM_8'
            elif 'd16' in part_b or 'r16' in part_b:
                mnemonic_determined_b = 'IMM_16'
            else:
                mnemonic_determined_b = part_b

    if '+' in part_a:
        mnemonic = "LDI"
    elif '-' in part_a:
        mnemonic = "LDD"

    if part_b:
        if '+' in part_b:
            mnemonic = "LDI"
        elif '-' in part_b:
            mnemonic = "LDD"

    if part_a and part_b:
        result = '{}_{}_{}'.format(mnemonic, mnemonic_determined_a, mnemonic_determined_b)
    else:
        result = '{}_{}'.format(mnemonic, mnemonic_determined_a)

    result = result.translate(None, '()+-')
    result = result.upper()

    return result

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
            instruction['mnemonic'],
            instruction['opcode'],
            instruction['num_bytes'],
            instruction['c0'],
            instruction['c1'],
            instruction['flag_mask'],
            instruction['flag_z'],
            instruction['flag_n'],
            instruction['flag_h'],
            instruction['flag_c']
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

    # #define SET(MNEMONIC, OPCODE, NUM_BYTES, C0, C1, FLAG_MASK, FLAG_Z, FLAG_N, FLAG_H, FLAG_C)
    for instruction in parsed_instructions:
        print 'SET({}, 0x{:02x}, {}, {}, {}, 0x{:0x}, {}, {}, {}, {}, {}, {})'.format(
            instruction['mnemonic'], instruction['opcode'], instruction['num_bytes'],
            instruction['c0'], instruction['c1'], instruction['flag_mask'],
            instruction['flag_z'], instruction['flag_n'], instruction['flag_h'], instruction['flag_c'],
            str(is_prefix_cb).lower(), 'isa_instruction_{}'.format(instruction['mnemonic'].lower()))

def generate_c_functions(parsed_instructions):
    for instruction in parsed_instructions:
        print 'int isa_instruction_{}(struct isa_instruction *instruction, struct registers *registers, uint8_t *data)\n{{\n\treturn 0;\n}}\n'.format(instruction['mnemonic'].lower())

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
        mnemonic = parts[1]

        if len(parts) == 9:
            mnemonic = parse_complex_mnemonic(mnemonic, parts[2])
            num_bytes = int(parts[3])
            c0, c1 = parse_cycle(parts[4])
            flag_z, flag_n, flag_h, flag_c, flag_mask = parse_flags(parts[5:])
        else:
            num_bytes = int(parts[2])
            c0, c1 = parse_cycle(parts[3])
            flag_z, flag_n, flag_h, flag_c, flag_mask = parse_flags(parts[4:])

        instruction = {
            'mnemonic': mnemonic,
            'opcode': opcode,
            'num_bytes': num_bytes,
            'c0': c0, 'c1': c1,
            'flag_mask': flag_mask,
            'flag_z': flag_z,
            'flag_n': flag_n,
            'flag_h': flag_h,
            'flag_c': flag_c,
        }

        parsed_instructions.append(instruction)

    return parsed_instructions

def main():
    parser = argparse.ArgumentParser(description='Converts Gameboy LR35902 isa into C header files.')
    parser.add_argument('-s', '--source', dest='source_path', type=str, default='data/opcodes.txt',
                        help='Path to textfile containting opcode information.')
    parser.add_argument('-f', '--format', dest='output_format', type=str, default="header",
                        help='Controls the type of output generated by this script. Valid values are "header", "definition" and "functions".')
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
    elif args.output_format == 'functions':
        generate_c_functions(parsed_instructions)
    else:
        print 'The output format "{}" is not supported.'.format(args.output_format)


if __name__ == "__main__":
    sys.exit(main())
