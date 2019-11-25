#include <errno.h>
#include <stdlib.h>

#include <pgb/debug.h>
#include <pgb/device/device.h>
#include <pgb/tui/cmds.h>
#include <pgb/tui/tui.h>
#include <pgb/utils.h>

#define SHORT_HELP_TEXT "Command for setting breakpoints"

#define FULL_HELP_TEXT                                                   \
	""                                                               \
	"break - Command for setting breakpoints\n"                      \
	"\n"                                                             \
	"Usage: break [address]\n"                                       \
	"\n"                                                             \
	"Description:\n"                                                 \
	"    Sets a breakpoint at the given address. If no address is\n" \
	"    If no address is given, the breakpoint is set at the\n"     \
	"    current value of the pc register.\n"                        \
	"\n"                                                             \
	"Example:\n"                                                     \
	"    > break\n"                                                  \
	"    > break 0xff\n"                                             \
	""

static
int break_cmd(struct libpgb_tui_state *tui_state, int argc, char **argv)
{
	uint16_t address;
	struct pgb_tui_breakpoint_entry *entry;

	if (argc == 1) {
		address = tui_state->device->cpu.registers.pc;
	} else {
		address = strtoul(argv[1], NULL, 0);
	}

	entry = malloc(sizeof(*entry));
	OK_OR_RETURN(entry != NULL, -ENOMEM);

	entry->address = address;

	TAILQ_INSERT_TAIL(tui_state->breakpoints, entry, entries);

	return 0;
}

struct tui_cmd tui_break_cmd = {
	.name = "break",
	.short_help_text = SHORT_HELP_TEXT,
	.full_help_text = FULL_HELP_TEXT,
	.cmd_func = break_cmd
};
