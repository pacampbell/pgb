#include <stdlib.h>
#include <pgb/tui/tui.h>

#define SHORT_HELP_TEXT "Command for exiting the debug utility"

#define FULL_HELP_TEXT                                               \
	""                                                           \
	"run - Command for exiting the debug utility\n"              \
	"\n"                                                         \
	"Usage: quit\n"                                              \
	"\n"                                                         \
	"Description:\n"                                             \
	"    Exits the emulation debug utility.\n"                   \
	"\n"                                                         \
	"Example:\n"                                                 \
	"    > quit\n"                                               \
	""

static
int quit_cmd(struct libpgb_tui_state *tui_state, int argc, char **argv)
{
	endwin();
	exit(0);
	return 0;
}

struct tui_cmd tui_quit_cmd =  {
	.name = "quit",
	.short_help_text = SHORT_HELP_TEXT,
	.full_help_text = FULL_HELP_TEXT,
	.cmd_func = quit_cmd
};
