#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include <pgb/cpu/cpu.h>
#include <pgb/debug.h>

const char *help_text = \
	"pgb - Utility for executing Gameboy classic roms.\n"
	"\n"
	"Usage: ./pgb [-h] -r PATH\n"
	"\n"
	"Options:\n"
	"  -h, --help\n"
	"	Displays this help menu and then exits.\n"
	"  -r, --rom='PATH'\n"
	"	The path to a valid Gameboy rom image to execute.\n"
	"";

static
int start_emulating(const char *rom_path)
{
	int ret;
	size_t stepped_instructions, num_instructions;
	struct cpu cpu;

	cpu_init(&cpu);

	ret = cpu_load_rom_from_file(&cpu, rom_path);
	OK_OR_RETURN(ret == 0, ret);

	num_instructions = 0;
	do {
		ret = cpu_step(&cpu, 1, &stepped_instructions);
		OK_OR_BREAK(ret == 0);

		num_instructions += stepped_instructions;
	} while (!cpu_is_halted(&cpu));

	cpu_destroy(&cpu);

	printf("Stepped %zu instructions\n", num_instructions);

	return ret;
}

int main(int argc, char *argv[])
{
	int ret, opt;
	const char *rom_path = NULL;

	while ((opt = getopt(argc, argv, "r:h")) != -1) {
		switch (opt) {
		case 'r':
			rom_path = optarg;
			break;
		case 'h':
			printf("%s", help_text);
			exit(EXIT_SUCCESS);
		default:
			fprintf(stderr, "%s", help_text);
			exit(EXIT_FAILURE);
		}
	}

	if (rom_path == NULL) {
		fprintf(stderr, "Please provide a valid path to the rom you wish to emulate. Exiting.\n");
		exit(EXIT_FAILURE);
	}

	if (access(rom_path, F_OK) < 0) {
		fprintf(stderr, "The rom file '%s' does not exist at the given path. Exiting.\n", rom_path);
		exit(EXIT_FAILURE);
	}

	// TODO: Check to see if it is a valid Gameboy binary

	ret = start_emulating(rom_path);
	OK_OR_WARN(ret == 0);

	return ret;
}
