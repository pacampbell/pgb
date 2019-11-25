#include <errno.h>
#include <stdlib.h>

#include <pgb/debug.h>
#include <pgb/device/device.h>
#include <pgb/tui/cmds.h>
#include <pgb/tui/tui.h>
#include <pgb/utils.h>

#define SHORT_HELP_TEXT "Command for executing n number of instructions"

#define FULL_HELP_TEXT                                                   \
	""                                                               \
	"step - Command for executing n number of instructions\n"        \
	"\n"                                                             \
	"Usage: step [n]\n"                                              \
	"\n"                                                             \
	"Description:\n"                                                 \
	"    Steps the execution of the currently emulated program\n"    \
	"    up to n number of instructions. If the number of\n"         \
	"    instructions is not provided, then a single instruction\n"  \
	"    is stepped.\n"                                              \
	"\n"                                                             \
	"Example:\n"                                                     \
	"    > step\n"                                                   \
	"    > step 5\n"                                                 \
	""

static
int step_cmd(struct libpgb_tui_state *tui_state, int argc, char **argv)
{
	int ret;
	size_t n, stepped_instructions;

	if (cpu_is_halted(&tui_state->device->cpu))
		return -EINVAL;

	if (argc > 1) {
		n = strtoul(argv[1], NULL, 0);
	} else {
		n = 1;
	}

	ret = cpu_step(tui_state->device, n, &stepped_instructions);
	OK_OR_WARN(ret == 0);

	return ret;
}

struct tui_cmd tui_step_cmd = {
	.name = "step",
	.short_help_text = SHORT_HELP_TEXT,
	.full_help_text = FULL_HELP_TEXT,
	.cmd_func = step_cmd
};
