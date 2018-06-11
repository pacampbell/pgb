#!/usr/bin/env bash

declare -A GENERATED_FILES

PROJECT_ROOT="."
SCRIPT_ROOT="$(dirname "$0")"

GENERATED_FILES+=( ["LR35902"]="data/opcodes.txt" ["LR35902_PREFIX_CB"]="data/prefix_cb_opcodes.txt" )

if [ -x "$(command -v git)" ]; then
	PROJECT_ROOT=$(git rev-parse --show-toplevel)
fi

for key in ${!GENERATED_FILES[@]}; do
	name="${key}"
	source_file="${GENERATED_FILES[${key}]}"
	output_file="$(echo ${name} | tr '[:upper:]' '[:lower:]')_ops"

	./${SCRIPT_ROOT}/generate_isa.py --source "${source_file}" --name "CPU_PRIVATE_${name}_OPS" --prefix "${name}" --format header > "${output_file}.h"
	./${SCRIPT_ROOT}/generate_isa.py --source "${source_file}" --name "CPU_PRIVATE_${name}_OPS" --prefix "${name}" --format definition > "${output_file}.def"
	# ./${SCRIPT_ROOT}/generate_isa.py --source "${source_file}" --name "CPU_PRIVATE_${name}_OPS" --prefix "${name}" --format json > "${output_file}.json"

	mv "${output_file}.h" "${PROJECT_ROOT}/include/pgb/cpu/private"
	mv "${output_file}.def" "${PROJECT_ROOT}/include/pgb/cpu/private"
done
