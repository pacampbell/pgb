#!/usr/bin/env bash

SCRIPT_ROOT="$(dirname "$0")"

# generate ISA files
./${SCRIPT_ROOT}/generate_isa.py --source data/opcodes.txt --name CPU_ISA --prefix LR35902 --format header > isa.h
./${SCRIPT_ROOT}/generate_isa.py --source data/opcodes.txt --name CPU_ISA --prefix LR35902 --format definition > isa.def
./${SCRIPT_ROOT}/generate_isa.py --source data/opcodes.txt --name CPU_ISA --prefix LR35902 --format functions > isa_functions.txt

# generate PREFIX CB files
./${SCRIPT_ROOT}/generate_isa.py --source data/prefix_cb_opcodes.txt --name CPU_PREFIX_CB --prefix LR35902_PREFIX_CB \
--format header --is-prefix-cb > prefix_cb.h
./${SCRIPT_ROOT}/generate_isa.py --source data/prefix_cb_opcodes.txt --name CPU_PREFIX_CB --prefix LR35902_PREFIX_CB \
--format definition --is-prefix-cb > prefix_cb.def
./${SCRIPT_ROOT}/generate_isa.py --source data/prefix_cb_opcodes.txt --name CPU_PREFIX_CB --prefix LR35902_PREFIX_CB \
--format functions --is-prefix-cb > prefix_functions.txt
