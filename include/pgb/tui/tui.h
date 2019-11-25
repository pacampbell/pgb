#ifndef PGB_TUI_TUI_H
#define PGB_TUI_TUI_H

#include <ncurses.h>
#include <sys/queue.h>

struct device;
struct pgb_tui_buffer;

struct pgb_tui_breakpoint_entry {
	TAILQ_ENTRY(pgb_tui_breakpoint_entry) entries;
	uint16_t address;
};

TAILQ_HEAD(pgb_tui_breakpoint, pgb_tui_breakpoint_entry);

enum tui_color_pair {
	TUI_COLOR_PAIR_ASM_CURR_LINE
};

struct libpgb_tui_state {
	WINDOW *input_window;
	WINDOW *asm_window;
	WINDOW *reg_window;
	bool has_colors;
	struct pgb_tui_buffer *buffer;
	struct device *device;
	struct pgb_tui_breakpoint *breakpoints;
	char previous_cmd[1024];
};

struct tui_cmd {
	const char *name;
	const char *short_help_text;
	const char *full_help_text;
	int (*cmd_func)(struct libpgb_tui_state *tui_state, int argc, char **argv);
};

#define TUI_SET_COLOR(tui_state, color_index) do { \
	if (!tui_state->has_colors) \
		break; \
	attron(COLOR_PAIR((color_index))); \
} while (0)

#define TUI_UNSET_COLOR(tui_state, color_index) do { \
	if (!tui_state->has_colors) \
		break; \
	attroff(COLOR_PAIR((color_index))); \
} while (0)

#endif /* PGB_TUI_TUI_H */
