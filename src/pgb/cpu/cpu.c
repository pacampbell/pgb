#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <pgb/cpu/decoder.h>
#include <pgb/cpu/instruction_info.h>
#include <pgb/cpu/interpreter.h>
#include <pgb/cpu/private/lr35902.h>
#include <pgb/cpu/registers.h>
#include <pgb/debug.h>
#include <pgb/device/device.h>
#include <pgb/mmu/private/mmu.h>
#include <pgb/utils.h>

LIBEXPORT
int cpu_init(struct cpu *cpu, const char *decoder_str)
{
	int ret;
	enum decoder_type decoder_type;

	cpu->status.halted = false;

	ret = registers_init(&cpu->registers);
	OK_OR_RETURN(ret == 0, ret);

	ret = string_to_decoder_type(decoder_str, &decoder_type);
	OK_OR_RETURN(ret == 0, ret);

	ret = cpu_decoder_configure_decoder(decoder_type, &cpu->decoder);
	OK_OR_WARN(ret == 0);

	return ret;
}

LIBEXPORT
int cpu_destroy(struct cpu *cpu)
{
	return 0;
}

LIBEXPORT
int cpu_load_rom_from_file(struct cpu *cpu, struct mmu *mmu, const char *path)
{
	int ret, fd;
	uint8_t *data;
	struct stat rom_st;

	fd = open(path, O_RDONLY);
	OK_OR_RETURN(fd >= 0, -EINVAL);

	ret = stat(path, &rom_st);
	OK_OR_RETURN(ret == 0, -EIO);

	data = mmap(NULL, rom_st.st_size, PROT_READ, MAP_SHARED, fd, 0);
	OK_OR_RETURN(data != MAP_FAILED, -EIO);

	// XXX: handle rom image larger than the size of section 00
	memcpy(mmu->ram, data, rom_st.st_size);

	munmap(data, rom_st.st_size);

	return 0;
}

LIBEXPORT
bool cpu_is_halted(struct cpu *cpu)
{
	return cpu->status.halted;
}

LIBEXPORT
int fetch(struct device *device, uint8_t *opcode, bool *found_prefix)
{
	struct cpu *cpu;
	struct mmu *mmu;
	struct registers *registers;

	cpu = &device->cpu;
	mmu = &device->mmu;
	registers = &cpu->registers;

	OK_OR_RETURN(registers->pc < MMU_REGION_SIZE(ROM_BANK_00), -EINVAL);

	*found_prefix = false;
	*opcode = mmu->ram[registers->pc];

	if (*opcode == LR35902_OPCODE_PREFIX_CB) {
		*found_prefix = true;

		OK_OR_RETURN((registers->pc + 1) < MMU_REGION_SIZE(ROM_BANK_00), -EINVAL);

		*opcode = mmu->ram[registers->pc + 1];
	}

	return 0;
}

LIBEXPORT
int decode(struct device *device, uint8_t opcode, bool is_prefix, struct decoded_instruction *decoded_instruction)
{
	int ret;

	ret = device->cpu.decoder.decode(device, opcode, is_prefix, decoded_instruction);
	OK_OR_WARN(ret == 0);

	return ret;
}

LIBEXPORT
int execute(struct device *device, struct decoded_instruction *decoded_instruction)
{
	int ret;

	ret = interpreter_execute_instruction(device, decoded_instruction);
	OK_OR_WARN(ret == 0);

	return ret;
}

LIBEXPORT
int cpu_step(struct device *device, size_t step, size_t *instructions_stepped)
{
	int ret = 0;
	size_t i;
	uint8_t opcode;
	struct decoded_instruction decoded_instruction;
	bool found_prefix;

	OK_OR_RETURN(!cpu_is_halted(&device->cpu), -EINVAL);

	for (i = 0; i < step; i++) {
		memset(&decoded_instruction, 0, sizeof(decoded_instruction));

		ret = fetch(device, &opcode, &found_prefix);
		OK_OR_BREAK(ret == 0);

		ret = decode(device, opcode, found_prefix, &decoded_instruction);
		OK_OR_BREAK(ret == 0);
		OK_OR_RETURN(decoded_instruction.info->instruction_class != INSTRUCTION_CLASS_INVALID, -EINVAL);

		ret = execute(device, &decoded_instruction);
		OK_OR_BREAK(ret == 0);
	}

	if (instructions_stepped != NULL)
		*instructions_stepped = i;

	return ret;
}

LIBEXPORT
int cpu_register_read8(struct cpu *cpu, enum instruction_operand reg, uint8_t *value)
{
	int ret = 0;

	switch (reg) {
	case INSTRUCTION_OPERAND_A:
		*value = cpu->registers.a;
		break;
	case INSTRUCTION_OPERAND_B:
		*value = cpu->registers.b;
		break;
	case INSTRUCTION_OPERAND_C:
		*value = cpu->registers.c;
		break;
	case INSTRUCTION_OPERAND_D:
		*value = cpu->registers.d;
		break;
	case INSTRUCTION_OPERAND_E:
		*value = cpu->registers.e;
		break;
	case INSTRUCTION_OPERAND_F:
		*value = cpu->registers.f;
		break;
	case INSTRUCTION_OPERAND_H:
		*value = cpu->registers.h;
		break;
	case INSTRUCTION_OPERAND_L:
		*value = cpu->registers.l;
		break;
	default:
		ret = -EINVAL;
		break;
	}
	OK_OR_WARN(ret == 0);

	return ret;
}

LIBEXPORT
int cpu_register_write8(struct cpu *cpu, enum instruction_operand reg, uint8_t value)
{
	int ret = 0;

	switch (reg) {
	case INSTRUCTION_OPERAND_A:
		cpu->registers.a = value;
		break;
	case INSTRUCTION_OPERAND_B:
		cpu->registers.b = value;
		break;
	case INSTRUCTION_OPERAND_C:
		cpu->registers.c = value;
		break;
	case INSTRUCTION_OPERAND_D:
		cpu->registers.d = value;
		break;
	case INSTRUCTION_OPERAND_E:
		cpu->registers.e = value;
		break;
	case INSTRUCTION_OPERAND_F:
		cpu->registers.f = value;
		break;
	case INSTRUCTION_OPERAND_H:
		cpu->registers.h = value;
		break;
	case INSTRUCTION_OPERAND_L:
		cpu->registers.l = value;
		break;
	default:
		ret = -EINVAL;
		break;
	}
	OK_OR_WARN(ret == 0);

	return ret;
}

LIBEXPORT
int cpu_register_read16(struct cpu *cpu, enum instruction_operand reg, uint16_t *value)
{
	int ret = 0;

	switch (reg) {
	case INSTRUCTION_OPERAND_AF:
		*value = cpu->registers.af;
		break;
	case INSTRUCTION_OPERAND_BC:
		*value = cpu->registers.bc;
		break;
	case INSTRUCTION_OPERAND_DE:
		*value = cpu->registers.de;
		break;
	case INSTRUCTION_OPERAND_HL:
		*value = cpu->registers.hl;
		break;
	case INSTRUCTION_OPERAND_SP:
		*value = cpu->registers.sp;
		break;
	case INSTRUCTION_OPERAND_PC:
		*value = cpu->registers.pc;
		break;
	default:
		ret = -EINVAL;
		break;
	}
	OK_OR_WARN(ret == 0);

	return ret;
}

LIBEXPORT
int cpu_register_write16(struct cpu *cpu, enum instruction_operand reg, uint16_t value)
{
	int ret = 0;

	switch (reg) {
	case INSTRUCTION_OPERAND_AF:
		cpu->registers.af = value;
		break;
	case INSTRUCTION_OPERAND_BC:
		cpu->registers.bc = value;
		break;
	case INSTRUCTION_OPERAND_DE:
		cpu->registers.de = value;
		break;
	case INSTRUCTION_OPERAND_HL:
		cpu->registers.hl = value;
		break;
	case INSTRUCTION_OPERAND_SP:
		cpu->registers.sp = value;
		break;
	case INSTRUCTION_OPERAND_PC:
		cpu->registers.pc = value;
		break;
	default:
		ret = -EINVAL;
		break;
	}
	OK_OR_WARN(ret == 0);

	return ret;
}
