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
#include <pgb/cpu/interpreter.h>
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
	printf("| AF   | 0x%04x  |\n", cpu->registers.af);
	printf("| BC   | 0x%04x  |\n", cpu->registers.bc);
	printf("| DE   | 0x%04x  |\n", cpu->registers.de);
	printf("| HL   | 0x%04x  |\n", cpu->registers.hl);
	printf("| CHNZ | 0b%u%u%u%u  |\n", cpu->registers.flags.carry, cpu->registers.flags.half_carry,
					   cpu->registers.flags.subtraction, cpu->registers.flags.zero);
	printf("+------+---------+\n");
}

static
int fetch(struct device *device, uint8_t *opcode, bool *found_prefix)
{
	struct cpu *cpu;
	struct registers *registers;
	struct rom_image *rom_image;

	cpu = &device->cpu;
	registers = &cpu->registers;
	rom_image = &cpu->rom_image;

	OK_OR_RETURN(registers->pc < rom_image->size, -EINVAL);

	*found_prefix = false;
	*opcode = rom_image->data[registers->pc];

	if (*opcode == LR35902_OPCODE_PREFIX_CB) {
		*found_prefix = true;

		OK_OR_RETURN((registers->pc + 1) < rom_image->size, -EINVAL);

		*opcode = rom_image->data[registers->pc + 1];
	}

	return 0;
}

static
int decode(struct device *device, uint8_t opcode, bool is_prefix, struct decoded_instruction *decoded_instruction)
{
	int ret;

	ret = device->cpu.decoder.decode(device, opcode, is_prefix, decoded_instruction);
	OK_OR_WARN(ret == 0);

	return ret;
}

static
int execute(struct device *device, struct decoded_instruction *decoded_instruction)
{
	int ret;

	ret = interpreter_execute_instruction(device, decoded_instruction);
	OK_OR_WARN(ret == 0);

	return ret;
}

int cpu_step(struct device *device, size_t step, size_t *instructions_stepped)
{
	int ret;
	size_t i;
	uint8_t opcode;
	struct decoded_instruction decoded_instruction;
	bool found_prefix;

	OK_OR_RETURN(!cpu_is_halted(&device->cpu), -EINVAL);

	for (i = 0; i < step; i++) {
		ret = fetch(device, &opcode, &found_prefix);
		OK_OR_BREAK(ret == 0);

		ret = decode(device, opcode, found_prefix, &decoded_instruction);
		OK_OR_BREAK(ret == 0);

		execute(device, &decoded_instruction);
		OK_OR_BREAK(ret == 0);

		if (IS_DEBUG()) {
			cpu_dump_register_state(&device->cpu);
			getchar();
		}
	}

	*instructions_stepped = i;

	return ret;
}

int cpu_register_read8(struct cpu *cpu, enum decoded_instruction_register reg, uint8_t *value)
{
	int ret = 0;

	switch (reg) {
	case DECODED_INSTRUCTION_REGISTER_A:
		*value = cpu->registers.a;
		break;
	case DECODED_INSTRUCTION_REGISTER_B:
		*value = cpu->registers.b;
		break;
	case DECODED_INSTRUCTION_REGISTER_C:
		*value = cpu->registers.c;
		break;
	case DECODED_INSTRUCTION_REGISTER_D:
		*value = cpu->registers.d;
		break;
	case DECODED_INSTRUCTION_REGISTER_E:
		*value = cpu->registers.e;
		break;
	case DECODED_INSTRUCTION_REGISTER_F:
		*value = cpu->registers.f;
		break;
	case DECODED_INSTRUCTION_REGISTER_H:
		*value = cpu->registers.h;
		break;
	case DECODED_INSTRUCTION_REGISTER_L:
		*value = cpu->registers.l;
		break;
	default:
		ret = -EINVAL;
		break;
	}
	OK_OR_WARN(ret == 0);

	return ret;
}

int cpu_register_write8(struct cpu *cpu, enum decoded_instruction_register reg, uint8_t value)
{
	int ret = 0;

	switch (reg) {
	case DECODED_INSTRUCTION_REGISTER_A:
		cpu->registers.a = value;
		break;
	case DECODED_INSTRUCTION_REGISTER_B:
		cpu->registers.b = value;
		break;
	case DECODED_INSTRUCTION_REGISTER_C:
		cpu->registers.c = value;
		break;
	case DECODED_INSTRUCTION_REGISTER_D:
		cpu->registers.d = value;
		break;
	case DECODED_INSTRUCTION_REGISTER_E:
		cpu->registers.e = value;
		break;
	case DECODED_INSTRUCTION_REGISTER_F:
		cpu->registers.f = value;
		break;
	case DECODED_INSTRUCTION_REGISTER_H:
		cpu->registers.h = value;
		break;
	case DECODED_INSTRUCTION_REGISTER_L:
		cpu->registers.l = value;
		break;
	default:
		ret = -EINVAL;
		break;
	}
	OK_OR_WARN(ret == 0);

	return ret;
}

int cpu_register_read16(struct cpu *cpu, enum decoded_instruction_register reg, uint16_t *value)
{
	int ret = 0;

	switch (reg) {
	case DECODED_INSTRUCTION_REGISTER_AF:
		*value = cpu->registers.af;
		break;
	case DECODED_INSTRUCTION_REGISTER_BC:
		*value = cpu->registers.bc;
		break;
	case DECODED_INSTRUCTION_REGISTER_DE:
		*value = cpu->registers.de;
		break;
	case DECODED_INSTRUCTION_REGISTER_HL:
		*value = cpu->registers.hl;
		break;
	case DECODED_INSTRUCTION_REGISTER_SP:
		*value = cpu->registers.sp;
		break;
	case DECODED_INSTRUCTION_REGISTER_PC:
		*value = cpu->registers.pc;
		break;
	default:
		ret = -EINVAL;
		break;
	}
	OK_OR_WARN(ret == 0);

	return ret;
}

int cpu_register_write16(struct cpu *cpu, enum decoded_instruction_register reg, uint16_t value)
{
	int ret = 0;

	switch (reg) {
	case DECODED_INSTRUCTION_REGISTER_AF:
		cpu->registers.af = value;
		break;
	case DECODED_INSTRUCTION_REGISTER_BC:
		cpu->registers.bc = value;
		break;
	case DECODED_INSTRUCTION_REGISTER_DE:
		cpu->registers.de = value;
		break;
	case DECODED_INSTRUCTION_REGISTER_HL:
		cpu->registers.hl = value;
		break;
	case DECODED_INSTRUCTION_REGISTER_SP:
		cpu->registers.sp = value;
		break;
	case DECODED_INSTRUCTION_REGISTER_PC:
		cpu->registers.pc = value;
		break;
	default:
		ret = -EINVAL;
		break;
	}
	OK_OR_WARN(ret == 0);

	return ret;
}