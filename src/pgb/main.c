#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include <pgb/bios.h>
#include <pgb/device/device.h>
#include <pgb/debug.h>
#include <pgb/utils.h>

static
const char *help_text =
	"pgb - Library for emulating Gameboy classic roms.\n"
	"\n"
	"Usage: ./libpgb [-h] [-b PATH] -r PATH\n"
	"\n"
	"Options:\n"
	"  -h, --help\n"
	"    Displays this help menu and then exits.\n"
	"  -b, --bios='PATH'\n"
	"    The path to a valid Gameboy BIOS image to execute.\n"
	"    If not provided and library was built without BIOS data,\n"
	"    the default state approximating what BIOS does will set.\n"
	"  -d, --decoder='decoder'\n"
	"    The type of decoder to use during emulation. Valid options are 'logical' and table'.\n"
	"    If no value is provided, 'table' is used by default.\n"
	"  -r, --rom='PATH'\n"
	"    The path to a valid Gameboy rom image to execute.\n"
	"";

static
const char *decoder_types[] = {
	"table",
	"logical"
};

static
bool is_valid_decoder_type(const char *decoder_type)
{
	unsigned i;

	for (i = 0; i < ARRAY_SIZE(decoder_types); i++) {
		if (strcmp(decoder_types[i], decoder_type) == 0) {
			return true;
		}
	}

	return false;
}

static
int setup_bios(struct device *device, const char *bios_path)
{
	int ret;

	if (bios_path != NULL) {
		ret = device_load_bios_from_file(device, bios_path);
	} else if (internal_bios_rom != NULL) {
		ret = device_load_bios_from_address(device, internal_bios_rom, internal_bios_rom_length);
	} else {
		/* XXX: Move program counter to address 255 and setup default register values */
		ret = -EINVAL;
	}
	OK_OR_WARN(ret == 0);

	return ret;
}

static
int start_emulating(const char *bios_path, const char *rom_path, const char *decoder_type)
{
	int ret;
	size_t stepped_instructions, num_instructions = 0;
	struct device device;

	ret = device_init(&device, decoder_type);
	OK_OR_RETURN(ret == 0, ret);

	if (IS_DEBUG()) {
		cpu_dump_register_state(&device.cpu);
	}

	ret = setup_bios(&device, bios_path);
	OK_OR_RETURN(ret == 0, ret);

	ret = device_load_image_from_file(&device, rom_path);
	OK_OR_RETURN(ret == 0, ret);

	do {
		ret = cpu_step(&device, 1, &stepped_instructions);
		OK_OR_BREAK(ret == 0);

		num_instructions += stepped_instructions;
	} while (!cpu_is_halted(&device.cpu));

	device_destroy(&device);

	return ret;
}

int main(int argc, char *argv[])
{
	int ret, opt;
	const char *bios_path = NULL;
	const char *rom_path = NULL;
	const char *decoder_type = "table";

	while ((opt = getopt(argc, argv, "b:d:gr:h")) != -1) {
		switch (opt) {
		case 'b':
			bios_path = optarg;
			break;
		case 'd':
			decoder_type = optarg;
			break;
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

	if (bios_path != NULL) {
		if (access(bios_path, F_OK) < 0) {
			fprintf(stderr, "The BIOS file '%s' does not exist at the given path. Exiting.\n", bios_path);
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

	if (!is_valid_decoder_type(decoder_type)) {
		fprintf(stderr, "The given decoder type '%s', is not valid. Exiting.\n", decoder_type);
		exit(EXIT_FAILURE);
	}

	ret = start_emulating(bios_path, rom_path, decoder_type);
	OK_OR_WARN(ret == 0);

	return ret;
}
