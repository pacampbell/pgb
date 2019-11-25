#include <pgb/debug.h>
#include <pgb/device/device.h>
#include <pgb/tui/tui.h>

#define SHORT_HELP_TEXT "Command for starting emulation execution"

#define FULL_HELP_TEXT                                               \
	""                                                           \
	"run - Command for starting emulation execution\n"           \
	"\n"                                                         \
	"Usage: run\n"                                               \
	"\n"                                                         \
	"Description:\n"                                             \
	"    Starts the emulation execution from the begining of\n"  \
	"    execution time. If the emulator is already executing\n" \
	"    this command will force the emulation to start over\n"  \
	"    from the begining.\n"                                   \
	"\n"                                                         \
	"Example:\n"                                                 \
	"    > run\n"                                                \
	""

extern
volatile int g_pgb_sigint;

static
int run_cmd(struct libpgb_tui_state *tui_state, int argc, char **argv)
{
	int ret;
	size_t stepped_instructions;
	struct pgb_tui_breakpoint_entry *bp;

	while (true) {
		if (g_pgb_sigint)
			break;

		if (!TAILQ_EMPTY(tui_state->breakpoints)) {
			TAILQ_FOREACH(bp, tui_state->breakpoints, entries) {
				if (bp->address == tui_state->device->cpu.registers.pc)
					goto breakpoint_hit;
			}
		}

		ret = cpu_step(tui_state->device, 1, &stepped_instructions);
		OK_OR_BREAK(ret == 0);
	}

	g_pgb_sigint = 0;

breakpoint_hit:

	return 0;
}

struct tui_cmd tui_run_cmd =  {
	.name = "run",
	.short_help_text = SHORT_HELP_TEXT,
	.full_help_text = FULL_HELP_TEXT,
	.cmd_func = run_cmd
};
