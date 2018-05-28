#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <pgb/cpu/isa.h>
#include <pgb/cpu/private/isa.h>
#include <pgb/cpu/registers.h>
#include <pgb/debug.h>
#include <pgb/device/device.h>

int cpu_init(struct cpu *cpu)
{
	cpu->rom_image.data = NULL;
	cpu->status.halted = false;

	registers_init(&cpu->registers);

	return 0;
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
int decode(struct device *device, uint8_t opcode, struct isa_instruction **__isa_instruction,
	   uint8_t *instruction_buffer, size_t size)
{
	int ret;
	struct isa_instruction *isa_instruction;
	struct cpu *cpu;
	struct registers *registers;
	struct rom_image *rom_image;
	size_t num_bytes;

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
	OK_OR_RETURN(num_bytes <= size, -EINVAL);

	memcpy(instruction_buffer, rom_image->data + registers->pc, num_bytes);
	registers->pc += num_bytes;

	*__isa_instruction = isa_instruction;

	return 0;
}

static
int execute(struct device *device, struct isa_instruction *isa_instruction, uint8_t *instruction_buffer, size_t size)
{
	int ret;

	if (IS_DEBUG()) {
		unsigned i, j;
		uint16_t immediate = 0;
		size_t actual_size;

		actual_size = isa_instruction->is_prefix ? size - 1 : size;
		for (i = 1; i < actual_size; i++) {
			immediate |= instruction_buffer[i] << ((i - 1) * 8);
		}

		printf("%04x ", device->cpu.registers.pc - (uint16_t)size);

		for (j = 0; j < 3 - size; j++) {
			printf("  ");
		}

		if (isa_instruction->is_prefix) {
			printf("%02x", LR35902_OPCODE_PREFIX_CB);
		}

		for (j = 0; j < actual_size; j++) {
			printf("%02x", instruction_buffer[j]);
		}

		printf(" %s", isa_instruction->assembly);
		if (actual_size > 1) {
			printf("\t; ($%x)", immediate);
		}
		printf("\n");
	}

	ret = isa_execute_instruction(device, isa_instruction, instruction_buffer);
	OK_OR_WARN(ret == 0);

	return ret;
}

int cpu_step(struct device *device, size_t step, size_t *instructions_stepped)
{
	int ret;
	size_t i;
	uint8_t opcode;
	uint8_t instruction_buffer[4];
	struct isa_instruction *isa_instruction;

	OK_OR_RETURN(!cpu_is_halted(&device->cpu), -EINVAL);

	for (i = 0; i < step; i++) {
		ret = fetch(device, &opcode);
		OK_OR_BREAK(ret == 0);

		ret = decode(device, opcode, &isa_instruction, instruction_buffer, sizeof(instruction_buffer));
		OK_OR_BREAK(ret == 0);

		execute(device, isa_instruction, instruction_buffer, isa_instruction->num_bytes);
		OK_OR_BREAK(ret == 0);
	}

	*instructions_stepped = i;

	return ret;
}
