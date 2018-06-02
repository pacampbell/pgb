#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <pgb/cpu/decoder.h>
#include <pgb/cpu/isa.h>
#include <pgb/cpu/private/isa.h>
#include <pgb/cpu/registers.h>
#include <pgb/debug.h>
#include <pgb/device/device.h>

int cpu_init(struct cpu *cpu, const char *decoder_str)
{
	int ret;
	enum decoder_type decoder_type;

	cpu->rom_image.data = NULL;
	cpu->status.halted = false;

	ret = registers_init(&cpu->registers);
	OK_OR_RETURN(ret == 0, ret);

	ret = string_to_decoder_type(decoder_str, &decoder_type);
	OK_OR_RETURN(ret == 0, ret);

	ret = cpu_decoder_configure_decoder(decoder_type, &cpu->decoder);
	OK_OR_WARN(ret == 0);

	return ret;
}

int cpu_destroy(struct cpu *cpu)
{
	if (cpu->rom_image.data != NULL) {
		munmap(cpu->rom_image.data, cpu->rom_image.size);
	}

	return 0;
}

int cpu_load_rom_from_file(struct cpu *cpu, const char *path)
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

	ret = cpu_load_rom(cpu, data, rom_st.st_size);
	OK_OR_WARN(ret == 0);

	return ret;
}

int cpu_load_rom(struct cpu *cpu, uint8_t *data, size_t size)
{
	unsigned i;

	OK_OR_RETURN(data != NULL, -EINVAL);

	cpu->rom_image.data = data;
	cpu->rom_image.size = size;

	if (IS_DEBUG()) {
		for (i = 0; i < size; i++) {
			if (i != 0 && (i % 16 == 0))
				printf("\n");
			if (i == 0 || (i % 16) == 0)
				printf("0x%08x ", i);

			printf("%02x ", data[i]);
		}

		printf("\n");
	}

	return 0;
}

bool cpu_is_halted(struct cpu *cpu)
{
	return cpu->status.halted;
}

void cpu_dump_register_state(struct cpu *cpu)
{
	printf("+------+---------+\n");
	printf("| PC   | 0x%04x  |\n", cpu->registers.pc);
	printf("| SP   | 0x%04x  |\n", cpu->registers.sp);
	printf("| A    | 0x%02x    |\n", cpu->registers.a);
	printf("| BC   | 0x%02x%02x  |\n", cpu->registers.b, cpu->registers.c);
	printf("| DE   | 0x%02x%02x  |\n", cpu->registers.d, cpu->registers.e);
	printf("| HL   | 0x%02x%02x  |\n", cpu->registers.h, cpu->registers.l);
	printf("| CHNZ | %u %u %u %u |\n", cpu->registers.flags.c, cpu->registers.flags.h,
				       cpu->registers.flags.n, cpu->registers.flags.z);
	printf("+------+---------+\n");
}

static
int fetch(struct device *device, uint8_t *opcode)
{
	struct cpu *cpu;
	struct registers *registers;
	struct rom_image *rom_image;

	cpu = &device->cpu;
	registers = &cpu->registers;
	rom_image = &cpu->rom_image;

	OK_OR_RETURN(registers->pc < rom_image->size, -EINVAL);

	*opcode = rom_image->data[registers->pc];

	return 0;
}

static
void dump_instruction(struct device *device, struct isa_instruction *isa_instruction, uint8_t *instruction_buffer, size_t size)
{
	if (!IS_DEBUG())
		return;

	unsigned i, j;
	uint16_t immediate = 0;

	for (i = 1; i < size; i++) {
		immediate |= instruction_buffer[i] << ((i - 1) * 8);
	}

	if (isa_instruction->is_prefix)
		printf("%04x ", device->cpu.registers.pc - (uint16_t)(size + 1));
	else
		printf("%04x ", device->cpu.registers.pc - (uint16_t)size);

	for (j = 0; j < 3 - size; j++) {
		printf("  ");
	}

	if (isa_instruction->is_prefix) {
		printf("%02x", LR35902_OPCODE_PREFIX_CB);
	}

	for (j = 0; j < size; j++) {
		printf("%02x", instruction_buffer[j]);
	}

	printf(" %s", isa_instruction->assembly);
	if (size > 1) {
		printf("\t; ($%x)", immediate);
	}
	printf("\n");
}

static
int decode(struct device *device, uint8_t opcode, struct decoded_instruction *decoded_instruction)
{
	int ret;
	struct isa_instruction *isa_instruction;
	struct cpu *cpu;
	struct registers *registers;
	struct rom_image *rom_image;
	size_t num_bytes;
	uint8_t instruction_buffer[4];

	cpu = &device->cpu;
	registers = &cpu->registers;
	rom_image = &cpu->rom_image;

	if (opcode != LR35902_OPCODE_PREFIX_CB) {
		ret = isa_get_instruction(opcode, &isa_instruction);
		OK_OR_RETURN(ret == 0, ret);

		num_bytes = isa_instruction->num_bytes;
	} else {
		registers->pc += LR35902_OPCODE_PREFIX_CB_NUM_BYTES;
		ret = fetch(device, &opcode);
		OK_OR_RETURN(ret == 0, ret);

		ret = isa_prefix_cb_get_instruction(opcode, &isa_instruction);
		OK_OR_RETURN(ret == 0, ret);

		// XXX: The isa defines this instruction as 2 bytes wide, but that includes the prefix opcode we
		// XXX: already accounted for.
		num_bytes = isa_instruction->num_bytes - 1;
	}

	OK_OR_RETURN(registers->pc + (num_bytes - 1) < rom_image->size, -EINVAL);
	OK_OR_RETURN(num_bytes <= sizeof(instruction_buffer), -EINVAL);

	memcpy(instruction_buffer, rom_image->data + registers->pc, num_bytes);
	registers->pc += num_bytes;

	dump_instruction(device, isa_instruction, instruction_buffer, num_bytes);

	decoded_instruction->info = isa_instruction;
	// ret = cpu_decoder_decode_instruction(isa_instruction, instruction_buffer, decoded_instruction);
	// OK_OR_WARN(ret == 0);

	return ret;
}

static
int execute(struct device *device, struct decoded_instruction *decoded_instruction)
{
	int ret;

	ret = isa_execute_instruction(device, decoded_instruction);
	OK_OR_WARN(ret == 0);

	return ret;
}

int cpu_step(struct device *device, size_t step, size_t *instructions_stepped)
{
	int ret;
	size_t i;
	uint8_t opcode;
	struct decoded_instruction decoded_instruction;

	OK_OR_RETURN(!cpu_is_halted(&device->cpu), -EINVAL);

	for (i = 0; i < step; i++) {
		ret = fetch(device, &opcode);
		OK_OR_BREAK(ret == 0);

		ret = decode(device, opcode, &decoded_instruction);
		OK_OR_BREAK(ret == 0);

		execute(device, &decoded_instruction);
		OK_OR_BREAK(ret == 0);
	}

	*instructions_stepped = i;

	return ret;
}

int cpu_register_read8(struct cpu *cpu, enum isa_operand reg, uint8_t *value)
{
	int ret = 0;

	switch (reg) {
	case ISA_OPERAND_A:
		*value = cpu->registers.a;
		break;
	case ISA_OPERAND_B:
		*value = cpu->registers.b;
		break;
	case ISA_OPERAND_C:
		*value = cpu->registers.c;
		break;
	case ISA_OPERAND_D:
		*value = cpu->registers.d;
		break;
	case ISA_OPERAND_E:
		*value = cpu->registers.e;
		break;
	case ISA_OPERAND_H:
		*value = cpu->registers.h;
		break;
	case ISA_OPERAND_L:
		*value = cpu->registers.l;
		break;
	default:
		ret = -EINVAL;
		break;
	}
	OK_OR_WARN(ret == 0);

	return ret;
}

int cpu_register_write8(struct cpu *cpu, enum isa_operand reg, uint8_t value)
{
	int ret = 0;

	switch (reg) {
	case ISA_OPERAND_A:
		cpu->registers.a = value;
		break;
	case ISA_OPERAND_B:
		cpu->registers.b = value;
		break;
	case ISA_OPERAND_C:
		cpu->registers.c = value;
		break;
	case ISA_OPERAND_D:
		cpu->registers.d = value;
		break;
	case ISA_OPERAND_E:
		cpu->registers.e = value;
		break;
	case ISA_OPERAND_H:
		cpu->registers.h = value;
		break;
	case ISA_OPERAND_L:
		cpu->registers.l = value;
		break;
	default:
		ret = -EINVAL;
		break;
	}
	OK_OR_WARN(ret == 0);

	return ret;
}

int cpu_register_read16(struct cpu *cpu, enum isa_operand reg, uint16_t *value)
{
	int ret = 0;

	switch (reg) {
	case ISA_OPERAND_AF:
		ret = -EINVAL;
		break;
	case ISA_OPERAND_BC:
		*value = ((cpu->registers.c << 8) | cpu->registers.b);
		break;
	case ISA_OPERAND_DE:
		*value = ((cpu->registers.e << 8) | cpu->registers.d);
		break;
	case ISA_OPERAND_HL:
		*value = ((cpu->registers.l << 8) | cpu->registers.h);
		break;
	case ISA_OPERAND_SP:
		*value = cpu->registers.sp;
		break;
	case ISA_OPERAND_PC:
		*value = cpu->registers.pc;
		break;
	default:
		ret = -EINVAL;
		break;
	}

	return ret;
}

int cpu_register_write16(struct cpu *cpu, enum isa_operand reg, uint16_t value)
{
	int ret = 0;

	switch (reg) {
	case ISA_OPERAND_AF:
		ret = -EINVAL;
		break;
	case ISA_OPERAND_BC:
		cpu->registers.c = (value & 0xff);
		cpu->registers.b = ((value >> 8) & 0xff);
		break;
	case ISA_OPERAND_DE:
		cpu->registers.e = (value & 0xff);
		cpu->registers.d = ((value >> 8) & 0xff);
		break;
	case ISA_OPERAND_HL:
		cpu->registers.l = (value & 0xff);
		cpu->registers.h = ((value >> 8) & 0xff);
		break;
	case ISA_OPERAND_SP:
		cpu->registers.sp = value;
		break;
	case ISA_OPERAND_PC:
		cpu->registers.pc = value;
		break;
	default:
		ret = -EINVAL;
		break;
	}

	return ret;
}