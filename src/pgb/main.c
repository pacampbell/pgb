#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include <pgb/device/device.h>
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
	struct device device;

	ret = device_init(&device);
	OK_OR_RETURN(ret == 0, ret);

	ret = device_emulate(&device, rom_path);
	OK_OR_WARN(ret == 0);

	device_destroy(&device);

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

	ret = start_emulating(rom_path);
	OK_OR_WARN(ret == 0);

	return ret;
}
