#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <pgb/cpu/cpu.h>
#include <pgb/debug.h>
#include <pgb/cpu/private/isa.h>

void cpu_init(struct cpu *cpu)
{
	cpu->rom_image.data = NULL;
	cpu->status.halted = false;
}

void cpu_destroy(struct cpu *cpu)
{
	if (cpu->rom_image.data != NULL) {
		munmap(cpu->rom_image.data, cpu->rom_image.size);
	}
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

	for (i = 0; i < size; i++) {
		if (i != 0 && (i % 16 == 0))
			printf("\n");
		if (i == 0 || (i % 16) == 0)
			printf("0x%08x ", i);

		printf("%02x ", data[i]);
	}

	printf("\n");

	return 0;
}

bool cpu_is_halted(struct cpu *cpu)
{
	return cpu->status.halted;
}

void cpu_dump_register_state(struct cpu *cpu)
{
	const char *border_regs = "+------+----------+--------+";
	const char *border_flags = "+------+-------+";

	printf("%s\n", border_regs);
	printf("| %4s | %8s | %6s |\n", "Name", "Size", "Value");
	printf("%s\n", border_regs);
	printf("| %-4s | %8s |   0x%02x |\n", "A", "8 bits", cpu->registers.a);
	printf("%s\n", border_regs);
	printf("| %-4s | %8s |   0x%02x |\n", "B", "8 bits", cpu->registers.b);
	printf("%s\n", border_regs);
	printf("| %-4s | %8s |   0x%02x |\n", "C", "8 bits", cpu->registers.c);
	printf("%s\n", border_regs);
	printf("| %-4s | %8s |   0x%02x |\n", "D", "8 bits", cpu->registers.d);
	printf("%s\n", border_regs);
	printf("| %-4s | %8s |   0x%02x |\n", "E", "8 bits", cpu->registers.e);
	printf("%s\n", border_regs);
	printf("| %-4s | %8s |   0x%02x |\n", "H", "8 bits", cpu->registers.h);
	printf("%s\n", border_regs);
	printf("| %-4s | %8s |   0x%02x |\n", "L", "8 bits", cpu->registers.l);
	printf("%s\n", border_regs);
	printf("| %-4s | %8s | 0x%04x |\n", "PC", "16 bits", cpu->registers.pc);
	printf("%s\n", border_regs);
	printf("| %-4s | %8s | 0x%04x |\n", "SP", "16 bits", cpu->registers.sp);
	printf("%s\n", border_regs);

	printf("\n");

	printf("%s\n", border_flags);
	printf("| %4s | %5s |\n", "Flag", "Value");
	printf("%s\n", border_flags);
	printf("| %4s | %5u |\n", "C", cpu->registers.flags.c);
	printf("%s\n", border_flags);
	printf("| %4s | %5u |\n", "H", cpu->registers.flags.h);
	printf("%s\n", border_flags);
	printf("| %4s | %5u |\n", "N", cpu->registers.flags.n);
	printf("%s\n", border_flags);
	printf("| %4s | %5u |\n", "Z", cpu->registers.flags.z);
	printf("%s\n", border_flags);
}

static
int fetch(struct cpu *cpu, uint8_t *opcode)
{
	struct registers *registers;
	struct rom_image *rom_image;

	registers = &cpu->registers;
	rom_image = &cpu->rom_image;

	OK_OR_RETURN(registers->pc < rom_image->size, -EINVAL);

	*opcode = rom_image->data[registers->pc];

	return 0;
}

static
int decode(struct cpu *cpu, uint8_t opcode)
{
	return 0;
}

int cpu_step(struct cpu *cpu, size_t step, size_t *instructions_stepped)
{
	int ret;
	size_t i;
	uint8_t opcode;

	OK_OR_RETURN(!cpu_is_halted(cpu), -EINVAL);

	for (i = 0; i < step; i++) {
		ret = fetch(cpu, &opcode);
		OK_OR_BREAK(ret == 0);

		ret = decode(cpu, opcode);
		OK_OR_BREAK(ret == 0);

		// TODO: EXECUTE
		// TODO: UPDATE
	}

	return ret;
}
