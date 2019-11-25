#include <pgb/tui/tui.h>
#include <pgb/tui/cmds.h>
#include <pgb/utils.h>

#define SHORT_HELP_TEXT "Command for displaying help messages"

#define FULL_HELP_TEXT                                                   \
	""                                                               \
	"help - Command for displaying help messages\n"                  \
	"\n"                                                             \
	"Usage: help [COMMAND]\n"                                        \
	"\n"                                                             \
	"Description:\n"                                                 \
	"    If the help command us used with no arguments, the\n"       \
	"    short message for each command will be printed. If\n"       \
	"    the optional argument COMMAND is given, all available\n"    \
	"    commands will be scanned. If a match is found\n"            \
	"    the help text for that command will be displayed.\n"        \
	"\n"                                                             \
	"Example:\n"                                                     \
	"    > help\n"                                                   \
	"    > help run\n"                                               \
	""

static
struct tui_cmd *tui_cmds[] = {
#include <pgb/tui/cmds.def>
};

static
int help_cmd(struct libpgb_tui_state *tui_state, int argc, char **argv)
{
	unsigned i;
	struct tui_cmd *cmd;

	if (argc == 1) {
		for (i = 0; i < ARRAY_SIZE(tui_cmds); i++) {
			cmd = tui_cmds[i];
			mvwprintw(tui_state->input_window, 2 + i, 2, "%s - %s\n", cmd->name, cmd->short_help_text);
		}
	}

	return 0;
}

struct tui_cmd tui_help_cmd = {
	.name = "help",
	.short_help_text = SHORT_HELP_TEXT,
	.full_help_text = FULL_HELP_TEXT,
	.cmd_func = help_cmd
};
