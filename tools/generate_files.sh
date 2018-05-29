#!/usr/bin/env bash

PROJECT_ROOT="."
SCRIPT_ROOT="$(dirname "$0")"

# Generate ISA files
./${SCRIPT_ROOT}/generate_isa.py --source data/opcodes.txt --name CPU_ISA --prefix LR35902 --format header > isa.h
./${SCRIPT_ROOT}/generate_isa.py --source data/opcodes.txt --name CPU_ISA --prefix LR35902 --format definition > isa.def

# Generate PREFIX CB files
./${SCRIPT_ROOT}/generate_isa.py --source data/prefix_cb_opcodes.txt --name CPU_PREFIX_CB --prefix LR35902_PREFIX_CB \
--format header --is-prefix-cb > prefix_cb.h
./${SCRIPT_ROOT}/generate_isa.py --source data/prefix_cb_opcodes.txt --name CPU_PREFIX_CB --prefix LR35902_PREFIX_CB \
--format definition --is-prefix-cb > prefix_cb.def

if [ -x "$(command -v git)" ]; then
	PROJECT_ROOT=$(git rev-parse --show-toplevel)
fi

# Move files to correct place.
mv isa.h "${PROJECT_ROOT}/include/pgb/cpu"
mv isa.def "${PROJECT_ROOT}/include/pgb/cpu/private"

mv prefix_cb.h "${PROJECT_ROOT}/include/pgb/cpu"
mv prefix_cb.def "${PROJECT_ROOT}/include/pgb/cpu/private"
