#include <errno.h>
#include <getopt.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <pgb/bios.h>
#include <pgb/debug.h>
#include <pgb/debugger/debugger.h>
#include <pgb/device/device.h>
#include <pgb/io/io.h>
#include <pgb/tui/buffer.h>
#include <pgb/tui/cmds.h>
#include <pgb/tui/tui.h>
#include <pgb/utils.h>

volatile int g_pgb_sigint = 0;

static
const char *help_text =
	"libpgb-dbg - Text based UI for debugging classic gameboy roms.\n"
	"\n"
	"Usage: ./libpgb-dbg [-h] [-b PATH] -r PATH\n"
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
void dump_register_state(WINDOW *reg_window, struct device *device)
{
	size_t i, j, n;
	int ret, cx, cy, wx, wy;
	struct cpu *cpu = &device->cpu;
	struct debugger_io_register_info *io_register_listing;

	mvwprintw(reg_window, 1, 2, "PC   0x%04x\n", cpu->registers.pc);
	mvwprintw(reg_window, 2, 2, "SP   0x%04x\n", cpu->registers.sp);
	mvwprintw(reg_window, 3, 2, "AF   0x%04x\n", cpu->registers.af);
	mvwprintw(reg_window, 4, 2, "BC   0x%04x\n", cpu->registers.bc);
	mvwprintw(reg_window, 5, 2, "DE   0x%04x\n", cpu->registers.de);
	mvwprintw(reg_window, 6, 2, "HL   0x%04x\n", cpu->registers.hl);
	mvwprintw(reg_window, 7, 2, "M    0x%04x\n", cpu->clock.m);
	mvwprintw(reg_window, 8, 2, "T    0x%04x\n", cpu->clock.t);
	mvwprintw(reg_window, 9, 2, "CHNZ 0b%u%u%u%u  \n",
		  cpu->registers.flags.carry, cpu->registers.flags.half_carry,
		  cpu->registers.flags.subtraction, cpu->registers.flags.zero);

	getmaxyx(reg_window, cy, cx);
	whline(reg_window, ACS_HLINE, cx);

	getyx(reg_window, cy, cx);
	mvwprintw(reg_window, cy, 2, "io registers");

	getyx(reg_window, cy, cx);

	ret = debugger_io_register_listing(device, &io_register_listing, &n);
	OK_OR_RETURN_VOID(ret == 0);

	getmaxyx(reg_window, wy, wx);
	(void)wx;

	j = 0;
	for (i = 0; i < n; i++) {
		j = i / (wy - cy - 2);

		mvwprintw(reg_window, cy + (i % (wy - cy) + 1), (j * 20) +  2,
			  "%-4s (0x%04x) 0x%02x\n",
			  io_register_listing[i].name, io_register_listing[i].address,
			  io_register_listing[i].value);
	}

	debugger_io_register_free(io_register_listing);
}

static
int draw_decorations(struct libpgb_tui_state *tui_state)
{
	// Draw the border
	box(tui_state->asm_window, 0, 0);
	box(tui_state->reg_window, 0, 0);
	box(tui_state->input_window, 0, 0);

	// Draw labels
	mvwprintw(tui_state->asm_window, 0, 2, "assembly");
	mvwprintw(tui_state->reg_window, 0, 2, "registers");
	mvwprintw(tui_state->input_window, 0, 2, "input");

	// XXX: Draw breakpoint markers

	return 0;
}

static
int list_asm(WINDOW *asm_window, struct libpgb_tui_state *tui_state,
	     unsigned num_instructions)
{
	int ret;
	size_t i, j, num_decoded;
	struct debugger_info *disasm_info;
	struct device *device = tui_state->device;

	disasm_info = malloc(num_instructions * sizeof(*disasm_info));
	OK_OR_RETURN(disasm_info != NULL, -ENOMEM);

	ret = debugger_fetch_instructions(device, disasm_info, num_instructions, &num_decoded);
	OK_OR_GOTO(ret == 0, free_and_exit);

	wprintw(asm_window, "\n");
	for (i = 0; i < num_decoded; i++) {
		if (i == 0) {
			mvwprintw(asm_window, i + 1, 0, ">");
			wattron(asm_window, A_REVERSE);
		}

		mvwprintw(asm_window, i + 1, 2, "0x%04x ", disasm_info[i].address);

		for (j = 0; j < disasm_info[i].num_bytes; j++) {
			wprintw(asm_window, "%02x ", disasm_info[i].raw_data[j]);
		}

		for (; j < ARRAY_SIZE(disasm_info[i].raw_data); j++) {
			wprintw(asm_window, "   ");
		}

		wprintw(asm_window, "%s ", disasm_info[i].assembly);

#if 0
		if (!disasm_info[i].comments.a && !disasm_info[i].comments.b) {
			wprintw(asm_window, "\n");
			continue;
		}
		wprintw(asm_window, "// ");

		if (disasm_info[i].comments.a && strlen(disasm_info[i].comments.a))
			wprintw(asm_window, "%s; ", disasm_info[i].comments.a);
		if (disasm_info[i].comments.b && strlen(disasm_info[i].comments.b))
			wprintw(asm_window, "%s; ", disasm_info[i].comments.b);
#endif

		wprintw(asm_window, "\n");
		if (i == 0) {
			wattroff(asm_window, A_REVERSE);
		}
	}

free_and_exit:
	free(disasm_info);

	return ret;
}

static
void refresh_windows(struct libpgb_tui_state *tui_state)
{
	wrefresh(tui_state->reg_window);
	wrefresh(tui_state->asm_window);
	wrefresh(tui_state->input_window);
}

static
int parse_cmd(const char *buffer, unsigned *_argc, char ***_argv)
{
	int ret = 0;
	unsigned i, argc, num_spaces;
	char *cmd, *str;
	char **argv;

	cmd = strdup(buffer);
	OK_OR_RETURN(cmd != NULL, -ENOMEM);

	/* Search for number of spaces to determine number of tokens */
	num_spaces = 0;
	for (i = 0; buffer[i] != '\0'; i++) {
		if (buffer[i] == ' ')
			num_spaces++;
	}

	argc = num_spaces + 1;

	// Allocate space for number of elements + 1 (for NULL sentinel)
	argv = malloc(sizeof(char*) * (argc + 1));
	OK_OR_GOTO(argv != NULL, cleanup_and_exit);

	i = 0;
	while((str = strsep(&cmd, " ")) != NULL) {
		argv[i++] = str;
	}
	argv[i] = NULL;

	*_argv = argv;
	*_argc = argc;

cleanup_and_exit:
	if (argv == NULL) {
		ret = -ENOMEM;
		free(cmd);
	}

	return ret;
}

static
struct tui_cmd *tui_cmds[] = {
#include <pgb/tui/cmds.def>
};

static
void handle_input(WINDOW *input_window, struct libpgb_tui_state *tui_state)
{
	int c, ret;
	unsigned i = 0, argc;
	char **argv;
	char buffer[1024];
	struct tui_cmd *tui_cmd;

	mvwprintw(input_window, 1, 2, "> ");
	while (true) {
		if (i >= ARRAY_SIZE(buffer))
			break;

		c = wgetch(input_window);

		/* Handle newline */
		if (c == '\n')
			break;

		/* Handle backspace */
		if (c == KEY_BACKSPACE) {
			buffer[i] = '\0';
			i = (i == 0) ? 0 : (i - 1);
			goto print_buffer_and_continue;
		}

		switch (c) {
		case KEY_LEFT:
			break;
		case KEY_RIGHT:
			break;
		/* XXX: Handle line history */
		case KEY_UP:
		case KEY_DOWN:
		default:
			break;
		}

		/* Skip non-printable characters */
		if (c < ' ' || c > '~')
			continue;

		buffer[i++] = c;

print_buffer_and_continue:
		mvwprintw(input_window, 1, 2, "> %.*s", i, buffer);
	}
	buffer[i] = '\0';

	if (strlen(buffer) == 0) {
		strncpy(buffer, tui_state->previous_cmd, ARRAY_SIZE(buffer));
	}

	ret = parse_cmd(buffer, &argc, &argv);
	OK_OR_RETURN_VOID(ret == 0);

	for (i = 0; i < ARRAY_SIZE(tui_cmds); i++) {
		tui_cmd = tui_cmds[i];
		if (strcmp(tui_cmd->name, argv[0]) == 0) {
			tui_cmd->cmd_func(tui_state, argc, argv);

			strncpy(tui_state->previous_cmd, buffer, ARRAY_SIZE(buffer));
		}
	}

	// free(argv[0]);
	free(argv);

	wrefresh(input_window);
}

static
int start_emulating(struct libpgb_tui_state *tui_state)
{
	/*
	 * 1. Clear screen
	 * 2. Draw content
	 * 3. Draw decorations
	 * 4. Get input from keyboard
	 */
	while (true) {
		// Clear content
		wclear(tui_state->asm_window);
		wclear(tui_state->reg_window);
		wclear(tui_state->input_window);

		// Populate details
		dump_register_state(tui_state->reg_window, tui_state->device);
		list_asm(tui_state->asm_window, tui_state, 25);

		// Draw window decorations
		draw_decorations(tui_state);

		// Draw everything
		refresh();
		refresh_windows(tui_state);

		// Get input
		handle_input(tui_state->input_window, tui_state);
	}

	return 0;
}

static
int init_tui(struct libpgb_tui_state *tui_state)
{
	int ret, wx, wy;

	initscr();
	noecho();
	cbreak();

	getmaxyx(stdscr, wy, wx);

	tui_state->asm_window = newwin(wy - 10, wx - 60, 0, 0);
	OK_OR_RETURN(tui_state->asm_window != NULL, -EIO);

	tui_state->reg_window = newwin(wy, 60, 0, wx - 60);
	OK_OR_RETURN(tui_state->reg_window != NULL, -EIO);

	tui_state->input_window = newwin(10, wx - 60, wy - 10, 0);
	OK_OR_RETURN(tui_state->input_window != NULL, -EIO);

	keypad(tui_state->input_window, true);

	tui_state->has_colors = has_colors();
	if (tui_state->has_colors) {
		start_color();
		init_pair(TUI_COLOR_PAIR_ASM_CURR_LINE, COLOR_BLUE, COLOR_WHITE);
	}

	ret = buffer_init(&tui_state->buffer);
	OK_OR_WARN(ret == 0);

	return ret;
}

static
void destroy_tui()
{
	endwin();
}

static
void sigint_handler(int sig_num)
{
	g_pgb_sigint = 1;
}

static
int init_pgb_signal_handlers()
{
	signal(SIGINT, sigint_handler);

	return 0;
}

static
int init_pgb_device(struct libpgb_tui_state *tui_state, const char *bios_path,
		    const char *rom_path, const char *decoder_type)
{
	int ret;
	struct device *device;
	struct pgb_tui_breakpoint *breakpoints;

	device = malloc(sizeof(*device));
	OK_OR_RETURN(device != NULL, -ENOMEM);

	ret = device_init(device, decoder_type);
	OK_OR_GOTO(ret == 0, cleanup_and_exit);

	ret = setup_bios(device, bios_path);
	OK_OR_GOTO(ret == 0, cleanup_and_exit);

	ret = device_load_image_from_file(device, rom_path);
	OK_OR_GOTO(ret == 0, cleanup_and_exit);

	breakpoints = malloc(sizeof(*breakpoints));
	ret = !(breakpoints != NULL);
	OK_OR_GOTO(ret == 0, cleanup_and_exit);

	TAILQ_INIT(breakpoints);

	tui_state->device = device;
	tui_state->breakpoints = breakpoints;

cleanup_and_exit:
	if (ret != 0) {
		free(device);
	}

	return ret;
}

int main(int argc, char *argv[])
{
	int ret, opt;
	const char *bios_path = NULL;
	const char *rom_path = NULL;
	const char *decoder_type = "table";
	struct libpgb_tui_state tui_state;

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

	ret = init_pgb_signal_handlers();
	OK_OR_RETURN(ret == 0, ret);

	ret = init_pgb_device(&tui_state, bios_path, rom_path, decoder_type);
	OK_OR_RETURN(ret == 0, ret);

	ret = init_tui(&tui_state);
	OK_OR_RETURN(ret == 0, EXIT_FAILURE);

	ret = start_emulating(&tui_state);
	OK_OR_WARN(ret == 0);

	device_destroy(tui_state.device);
	destroy_tui();

	return ret;
}
